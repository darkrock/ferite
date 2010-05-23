/*
 * Copyright (C) 2000-2007 Chris Ross and various contributors
 * Copyright (C) 1999-2000 Chris Ross
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * o Neither the name of the ferite software nor the names of its contributors may
 *   be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

/* -*- mode: c; mode: fold; -*- */
#include "ferite.h"
#include "aphex.h"

/**
 * @group Functions
 * @description Various functions to deal with ferite function structures.
 */

/*!
 * @function ferite_create_internal_function
 * @declaration FeriteFunction *ferite_create_internal_function( FeriteScript *script, char *name )
 * @brief Allocate a FeriteFunction structure and set it up for use as a script function
 * @param FeriteScript *script The script it is being used within
 * @param FeriteFunction *name The name of the function to be created
 * @return A pointer to a FeriteFunction structure
 * @description
 * This function tends to be used only be ferite and is used purely to dump opcode data into
 * to create a native function see ferite_create_external_function()
 */
FeriteFunction *ferite_create_internal_function( FeriteScript *script, char *name )
{
	FeriteFunction *ptr;
	int i = 0;

	FE_ENTER_FUNCTION;
	FUD(("Creating Function: %s\n", name));
	ptr = fmalloc( sizeof( FeriteFunction ) );
	ptr->name = fstrdup( name );
	ptr->type = FNC_IS_INTRL;
	ptr->localvars = ferite_create_stack( script, FE_FUNCTION_VARIABLE_SIZE );
	ptr->bytecode = ferite_create_opcode_list( FE_FUNCTION_OPCODE_INIT_SIZE );
	ptr->signature = fmalloc( sizeof( FeriteParameterRecord* ) * FE_FUNCTION_PARAMETER_MAX_SIZE );
	for( i = 0; i < FE_FUNCTION_PARAMETER_MAX_SIZE; i++ )
		ptr->signature[i] = NULL;
	ptr->arg_count = 0;
	ptr->native_information = NULL;
	ptr->odata = NULL;
	ptr->lock = NULL;
	ptr->klass = NULL;
	ptr->is_static = FE_TRUE;
	ptr->state = FE_ITEM_IS_PUBLIC;
	ptr->next = NULL;
	ptr->is_alias = FE_FALSE;
	ptr->length = 0;
	ptr->cached = FE_FALSE;
	ptr->return_type = F_VAR_VOID;
	FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_external_function
 * @declaration FeriteFunction *ferite_create_external_function( FeriteScript *script, char *name, void *(*funcPtr)(FeriteScript *, FeriteFunction*, FeriteVariable **), char *description )
 * @brief Allocate a FeriteFunction structure and set it up for use as a native function
 * @param FeriteScript *script The script it is being used within
 * @param char *name The name of the function to be created
 * @param void *funcPtr A pointer to the native c function handling it
 * @param char *description A description of the functions signiture
 * @return A pointer to a FeriteFunction structure
 * @description
 * The description is pretty straight forward - it is simply a sequence of characters eg "oosna" They mean the following:<nl/>
 * <ul>
 * <li> <b>n</b> - number
 * <li> <b>s</b> - string
 * <li> <b>a</b> - array
 * <li> <b>o</b> - object
 * <li> <b>v</b> - void
 * <li> <b>.</b> - variable argument list
 * </ul>
 * <nl/>
 * This means that the function will only actually ever be called with the correct parameters.
 */
FeriteFunction *ferite_create_external_function( FeriteScript *script, char *name, void *f, char *description )
{
	FeriteFunction *ptr;
	int i;
	FeriteVariable *new_variable = NULL;

	FE_ENTER_FUNCTION;
	FUD(("Creating Function: %s\n", name));
	ptr = fmalloc( sizeof( FeriteFunction ) );
	ptr->name = fstrdup( name );
	ptr->type = FNC_IS_EXTRL;
	ptr->fncPtr = (FeriteVariable*(*)(FeriteScript*,void*,FeriteObject*,FeriteFunction*,FeriteVariable**))f;
	ptr->odata = NULL;
	ptr->bytecode = NULL;
	ptr->localvars = NULL;
	ptr->signature = fmalloc( sizeof( FeriteParameterRecord* ) * FE_FUNCTION_PARAMETER_MAX_SIZE );
	for( i = 0; i < FE_FUNCTION_PARAMETER_MAX_SIZE; i++ )
		ptr->signature[i] = NULL;
	ptr->arg_count = 0;

	for( i = 0; i < (signed)strlen( description ); i++ )
	{
		switch( description[i] )
		{
		  case 'a': new_variable = ferite_create_uarray_variable( script, "a", 0, FE_STATIC ); break;
		  case 's': new_variable = ferite_create_string_variable( script, "s", NULL, FE_STATIC ); break;
		  case 'n': new_variable = ferite_create_number_long_variable( script, "n", 0, FE_STATIC ); break;
		  case 'o': new_variable = ferite_create_object_variable( script, "o", FE_STATIC ); break;
		  case 'v': new_variable = ferite_create_void_variable( script, "v", FE_STATIC ); break;
		  case '?': new_variable = ferite_create_void_variable( script, "?", FE_STATIC ); break;
		  case '.': new_variable = ferite_create_void_variable( script, ".", FE_STATIC ); break;
		  default:
			ferite_error( script, 0, "Type '%c' not allowed for function signatures (%s)\n", name );
			break;
		}
		if( new_variable != NULL )
		{
			ptr->signature[ptr->arg_count] = fmalloc( sizeof( FeriteParameterRecord ) );
			ptr->signature[ptr->arg_count]->variable = new_variable;
			ptr->signature[ptr->arg_count]->has_default_value = FE_FALSE;
			ptr->signature[ptr->arg_count]->pass_type = FE_BY_VALUE;
			ptr->signature[ptr->arg_count]->name = NULL;
			ptr->signature[ptr->arg_count]->is_dots = FE_FALSE;
			if( description[i] == '.' )
				ptr->signature[ptr->arg_count]->is_dots = FE_TRUE;
			ptr->arg_count++;
		}
	}
	ptr->native_information = NULL;
	ptr->klass = NULL;
	ptr->lock = NULL;
	ptr->is_static = FE_TRUE;
	ptr->next = NULL;
	ptr->state = FE_ITEM_IS_PUBLIC;
	ptr->is_alias = FE_FALSE;
	ptr->length = 0;
	ptr->cached = FE_FALSE;
	ptr->return_type = F_VAR_VOID;
	FE_LEAVE_FUNCTION( ptr );
}

FeriteFunction *ferite_create_alias_function( FeriteScript *script, FeriteFunction *to )
{
	FeriteFunction *ptr = NULL;
	
	FE_ENTER_FUNCTION;
	if( to != NULL )
	{	   
		ptr = fmalloc( sizeof(FeriteFunction) );
		memcpy( ptr, to, sizeof(FeriteFunction) );
		ptr->is_alias = FE_TRUE;		
	}	
	FE_LEAVE_FUNCTION( ptr );
}

char *ferite_function_generate_sig_string( FeriteScript *script, FeriteFunction *f )
{
	int argcount = 0, i = 0;
	char *str = NULL;
	
	FE_ENTER_FUNCTION;
	
	if( f != NULL )
	{
		argcount = f->arg_count;
		str = fcalloc( argcount+1, sizeof(char) );
		for( i = 0; i < argcount; i++ )
		{
			if( f->signature[i]->is_dots ) {
				str[i] = 'E';
			} else {
				switch( F_VAR_TYPE(f->signature[i]->variable) )
				{
				  case F_VAR_LONG:
				  case F_VAR_DOUBLE:
					str[i] = 'n';
					break;
				  case F_VAR_STR:
					str[i] = 's';
					break;
				  case F_VAR_OBJ:
					str[i] = 'o';
					break;
				  case F_VAR_UARRAY:
					str[i] = 'a';
					break;
				  case F_VAR_VOID:
					str[i] = 'v';
					break;
				  case F_VAR_BOOL:
					str[i] = 'b';
					break;
				}
			}
		}
		str[i++] = '\0'; 
	}
	FE_LEAVE_FUNCTION( str );
}

/**
 * @function ferite_function_get
 * @declaration FeriteFunction *ferite_function_get( FeriteScript *script, char *name )
 * @brief Find a function in the top level namespace of a script
 * @param FeriteScript *script The script to search
 * @param char *name The name of the function
 * @return A pointer to the function on success, NULL on fail
 */
FeriteFunction *ferite_function_get( FeriteScript *script, char *name )
{
	FeriteNamespaceBucket *nsb = NULL;

	FE_ENTER_FUNCTION;
	FUD(("Searching for function %s\n", name ));
	nsb = ferite_namespace_element_exists( script, script->mainns, name );
	if( nsb && nsb->type == FENS_FNC )
	{
		FE_LEAVE_FUNCTION( nsb->data );
	}
	FUD(("ERROR: Could not find function %s\n", name));
	FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_function_get_compiled_code
 * @declaration FeriteOp **ferite_function_get_compiled_code( FeriteScript *script, char *name )
 * @brief Find a function in the top level namespace of a script and return it's opcode data
 * @param FeriteScript *script The script to search
 * @param char *name The name of the function
 * @return A pointer to the function's opcode on success, NULL on fail
 */
FeriteOp **ferite_function_get_compiled_code( FeriteScript *script, char *name )
{
	FeriteFunction *ptr;

	FE_ENTER_FUNCTION;

	FUD(("Searching for function %s\n", name ));
	ptr = ferite_function_get( script, name );
	if( ptr != NULL )
	{
		FUD(("Found %s\n", name ));
		FE_LEAVE_FUNCTION( ptr->bytecode->list );
	}
	FUD(("ERROR: Could not find function %s\n", name));
	FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_delete_function_list
 * @declaration void ferite_delete_function_list( FeriteScript *script, FeriteFunction *func )
 * @brief Delete a list of FeriteFunction's - on most occasions this is only deleteing on function
 * @param FeriteScript *script The current script
 * @param FeriteFuncion *func The top function in the list
 */
void ferite_delete_function_list( FeriteScript *script, FeriteFunction *func )
{
	int i;

	FE_ENTER_FUNCTION;
	if( func )
	{
		if( func->next != NULL )
		{
			ferite_delete_function_list( script, func->next );
			func->next = NULL;
		}

		if( func->cached == FE_FALSE ) {
			if( func->is_alias == FE_FALSE )
			{
				FUD(("Deleting Function: %s\n", func->name ));
				if( func->type == FNC_IS_INTRL )
				{
					for( i = 1; i <= func->localvars->stack_ptr; i++ )
					{
						if( func->localvars->stack[i] != NULL )
						  ferite_variable_destroy( script, func->localvars->stack[i] );
					}
					ferite_delete_stack( script, func->localvars );
					func->localvars = NULL;

					ferite_delete_opcode_list( script, func->bytecode );
					func->bytecode = NULL;
				}
		
				for( i = 0; i < func->arg_count; i++ )
				{
					if( func->signature[i] != NULL )
					{
						if( func->signature[i]->name )
							ffree( func->signature[i]->name );
						ferite_variable_destroy( script, func->signature[i]->variable );
						ffree( func->signature[i] );
					}
				}
		
				if( func->native_information != NULL )
				{
					if( func->native_information->file != NULL )
					{
						ffree( func->native_information->file );
					}
					if( func->native_information->code != NULL )
					{
						ffree( func->native_information->code );
					}
					ffree( func->native_information );
				}
		
#ifdef THREAD_SAFE
				if( func->lock != NULL )
				  aphex_mutex_destroy( func->lock );
#endif

				func->lock = NULL;

				ffree( func->signature );
				ffree( func->name );
			}
			ffree( func );
		}
	}
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_function_to_external
 * @declaration void ferite_function_to_external( FeriteScript *script, FeriteFunction *func )
 * @brief Convert an internal function to an external one
 * @param FeriteScript *script The script
 * @param FeriteFunction *func The function to convert
 */
void ferite_function_to_external( FeriteScript *script, FeriteFunction *func )
{
	int i = 0;
	
	FE_ENTER_FUNCTION;
	if( func != NULL )
	{
		FUD(("Converting Function: %s\n", func->name ));
		if( func->type == FNC_IS_INTRL )
		{
			for( i = 1; i <= func->localvars->stack_ptr; i++ )
			{
				if( func->localvars->stack[i] != NULL )
				  ferite_variable_destroy( script, func->localvars->stack[i] );
			}
			ferite_delete_stack( script, func->localvars );
			func->localvars = NULL;

			FUD(("Deleting opcode list\n"));
			ferite_delete_opcode_list( script, func->bytecode );
			func->bytecode = NULL;
		}
		
		func->fncPtr = NULL;
		func->type = FNC_IS_EXTRL;
	}
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_delete_function_hash
 * @declaration void ferite_delete_function_hash( FeriteScript *script, FeriteHash *hash )
 * @brief Delete a hash aswell as cleaning up all functions
 * @param FeriteScript *script The current script
 * @param FeriteHash *hash The hash to be deleted
 */
void ferite_delete_function_hash( FeriteScript *script, FeriteHash *hash )
{
	FE_ENTER_FUNCTION;
	ferite_delete_hash( script, hash, (void (*)(FeriteScript*,void *))ferite_delete_function_list );
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_check_params
 * @declaration int ferite_check_params( FeriteScript *script, FeriteVariable **params, FeriteParameterRecord **signature )
 * @brief Check the parameters against the function signiture and see if they match
 * @param FeriteScript *script The current script
 * @param FeriteVariable **params The parameters to be checked
 * @param FeriteParameterRecord **signature The funcions signature
 * @return 0 on fail, 1 on success
 */

/* NOTE: The warning messages are commented out because they mess with method overloading */
int ferite_check_params( FeriteScript *script, FeriteVariable **params, FeriteFunction *function )
{
	FeriteParameterRecord **signature = NULL;
	int arg_count, sig_count;
	int i, has_period = FE_FALSE;
	
	FE_ENTER_FUNCTION;
	
	arg_count = ferite_get_parameter_count (params);
	FUD(("Comparing %s: Argument count: %d\n", function->name, arg_count));
	signature = function->signature;
	sig_count = function->arg_count;
	
	/* If both the signature and the parameters are empty, we can safely assume that the params are correct */
	if (arg_count == 0 && sig_count == 0)
	{
		FUD(("Comparing %s: no parameters, no arguments - success\n", function->name));
		FE_LEAVE_FUNCTION( FE_TRUE );
	}
	
	for (i = 0; signature[i]; i++)
	{
		if (signature[i]->is_dots)
		{
			FUD(("Comparing %s: - found dots\n", function->name ));
			has_period = FE_TRUE;
			break;
		}
		if (!params[i])
		{
			if( signature[i]->has_default_value )
			{
				ferite_add_to_parameter_list( params, ferite_duplicate_variable( script, signature[i]->variable, NULL ) );
				continue;
			}
			FUD(("Comparing %s: - found invalid - missing parameter\n", function->name ));
			FE_LEAVE_FUNCTION( FE_FALSE );
		}
		
		FUD(("Comparing %s: comparing %d to %d\n", function->name, F_VAR_TYPE(signature[i]->variable), F_VAR_TYPE(params[i]) ));
		if ( !ferite_types_are_equal( script, F_VAR_TYPE(signature[i]->variable), F_VAR_TYPE(params[i]) ) )
		{
			/* Found an invalid type */
			FUD(("Comparing %s: - found invalid - type mismatch\n", function->name ));
			FE_LEAVE_FUNCTION( FE_FALSE );
		}
	}
	
	/* This is removed because we dont need to check the back end of the parameter lists */
	/* We keep it incase we want it in the future */
#if 0
	if ( FE_FALSE && has_period)
	{
		int offset;
		
		/* for when we dont have a var for the '...' */
		if( signature[i]->is_dots && !params[i] )
		{
			FE_LEAVE_FUNCTION( FE_TRUE );
		}
		
		i++; /* Skip the period signature */
		offset = arg_count - i;
		
		if (offset < 0)
		{
			/* Invalid Parameter count, an offset of less than zero means ABS(offset) parameters are missing */
			FE_LEAVE_FUNCTION( FE_FALSE );
		}
		
		for (; params[i + offset] && signature[i]; i++)
		{
			/* We cannot find another period.  If we do, something is WRONG */
			if (signature[i]->is_dots)
			{
				break;
			}
			
			if ( !ferite_types_are_equal( script, F_VAR_TYPE(signature[i]->variable), PTR2VAR(params[i+offset])->type ) )
			{
				FE_LEAVE_FUNCTION( FE_FALSE );
			}
		}
	}
	else
#endif
	{
		if( has_period )
		{
			FUD(("Comparing %s: - found valid - got dot\n", function->name ));
			FE_LEAVE_FUNCTION( FE_TRUE );
		}
		if ((params[i] && !signature[i]) || (!params[i] && signature[i]))
		{
			FUD(("Comparing %s: - found invalid - type mismatch\n", function->name ));
			FE_LEAVE_FUNCTION( FE_FALSE );
		}
	}
	FUD(("Comparing %s: - found valid\n", function->name ));
	FE_LEAVE_FUNCTION( FE_TRUE );
}

/**
 * @function ferite_function_dup
 * @declaration FeriteFunction *ferite_function_dup( FeriteScript *script, FeriteFunction *function, FeriteClass *container )
 * @brief Duplicate a function
 * @param FeriteScript *script The current script
 * @param FeriteFunction *function The function to duplicated
 * @param FeriteClass *container The class the function is part of [if it is part of a class]
 * @return The new function
 */
FeriteFunction *ferite_function_dup( FeriteScript *script, FeriteFunction *function, FeriteClass *container )
{
	FeriteFunction *ptr = NULL;
	int i = 0;

	FE_ENTER_FUNCTION;
	if( function != NULL )
	{
		ptr = fmalloc( sizeof( FeriteFunction ) );
		if( function->name != NULL )
		  ptr->name = fstrdup( function->name );
		else
		  ptr->name = NULL;
		
		ptr->type = function->type;
		ptr->is_static = function->is_static;
		ptr->arg_count = function->arg_count;

		if( function->lock != NULL )
		  ptr->lock = aphex_mutex_recursive_create();
		else
		  ptr->lock = NULL;

		ptr->klass = container;

		ptr->signature = fmalloc( sizeof( FeriteParameterRecord* ) * FE_FUNCTION_PARAMETER_MAX_SIZE );
		for( i = 0; i < FE_FUNCTION_PARAMETER_MAX_SIZE; i++ )
		  ptr->signature[i] = NULL;
		
		for( i = 0; i < (ptr->arg_count + 1); i++ )
		{
			if( function->signature[i] != NULL )
			{
				ptr->signature[i] = fmalloc( sizeof(FeriteParameterRecord) );
				ptr->signature[i]->variable = ferite_duplicate_variable( script, function->signature[i]->variable, NULL );
				ptr->signature[i]->has_default_value = function->signature[i]->has_default_value;
				ptr->signature[i]->pass_type = function->signature[i]->pass_type;
			}
		}

		if( function->native_information != NULL )
		{
			ptr->native_information = fmalloc(sizeof(FeriteFunctionNative));
			ptr->native_information->code = fstrdup(function->native_information->code);
			ptr->native_information->file = fstrdup(function->native_information->file);
			ptr->native_information->line = function->native_information->line;
		}
		else
		  ptr->native_information = NULL;

		switch( function->type )
		{
		  case FNC_IS_INTRL:
			ptr->localvars = ferite_duplicate_stack( script, function->localvars, (void*(*)(FeriteScript*,void*,void*))ferite_duplicate_variable, NULL );
			ptr->bytecode = ferite_opcode_dup( script, function->bytecode );
			break;
		  case FNC_IS_EXTRL:
			ptr->fncPtr = function->fncPtr;
			ptr->bytecode = NULL;
			break;
		}
		
		if( function->next != NULL )
		  ptr->next = ferite_function_dup( script, function->next, container );
		else
		  ptr->next = NULL;
		
		ptr->state = function->state;
		ptr->is_alias = function->is_alias;
		
	}
	FE_LEAVE_FUNCTION(ptr);
}

int ferite_compare_functions( FeriteScript *script, FeriteFunction *f1, FeriteFunction *f2 )
{
	int i = 0;
	
	FE_ENTER_FUNCTION;
	if( strcmp( f1->name, f2->name ) != 0 )
	{
		FE_LEAVE_FUNCTION( FE_FALSE );
	}
	if( f1->arg_count != f2->arg_count )
	{
		FE_LEAVE_FUNCTION( FE_FALSE );
	}
	for( i = 0; i < f1->arg_count; i++ )
	{
		if( !ferite_types_are_equal( script, F_VAR_TYPE(f1->signature[i]->variable), F_VAR_TYPE(f2->signature[i]->variable) ) )
		{
			FE_LEAVE_FUNCTION( FE_FALSE );
		}
	}
	FE_LEAVE_FUNCTION( FE_TRUE );
}

/**
 * @end
 */
