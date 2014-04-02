/* -*- mode: c; mode: fold; -*- */
/*
 * Copyright (C) 2000-2010 Chris Ross and various contributors
 * Copyright (C) 1999-2000 Chris Ross
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 *	list of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice,
 *	this list of conditions and the following disclaimer in the documentation
 *	and/or other materials provided with the distribution.
 * o Neither the name of the ferite software nor the names of its contributors may
 *	be used to endorse or promote products derived from this software without
 *	specific prior written permission.
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

#include "ferite.h"
#include "aphex.h"

extern FeriteOpTable ferite_op_table[];

/**
 * @group Executor
 * @description These functions allow for the execution of code. The main interface is
 *			  ferite_script_execute(), but to execute individual functions there is
 *			  ferite_call_function(). There are other convinience functions.
 */

/**
 * @function ferite_stop_execution
 * @declaration void ferite_stop_execution( FeriteScript *script )
 * @brief Stop the script executing
 * @param FeriteScript *script The script to stop
 */
void ferite_stop_execution( FeriteScript *script, int return_value )
{
	FE_ENTER_FUNCTION;
	script->keep_execution = FE_FALSE;
	script->return_value = return_value;
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_is_executing
 * @declaration int ferite_is_executing( FeriteScript *script )
 * @brief Return whether a script is executing
 * @param FeriteScript *script The script to query
 * @return FE_TRUE if the script is executing, FE_FALSE otherwise
 */
int ferite_is_executing( FeriteScript *script )
{
	FE_ENTER_FUNCTION;
	FE_LEAVE_FUNCTION( (script && script->is_executing ? FE_TRUE : FE_FALSE) );
}

/**
 * @function ferite_script_execute
 * @declaration int ferite_script_execute( FeriteScript *script )
 * @brief Run a script
 * @param FeriteScript *script The script to run
 * @return FE_TRUE on successfull execution, FE_FALSE otherwise
 */
int ferite_script_execute( FeriteScript *script )
{
	FeriteNamespaceBucket *nsb = NULL;
	FeriteFunction *func = NULL;
	FeriteVariable *err = NULL, *errstr = NULL, *erno = NULL, *rval = NULL;

	FE_ENTER_FUNCTION;

	if( script->mainns != NULL )
	{
		script->error_state = 0;
		script->is_executing = FE_TRUE;
		nsb = ferite_namespace_element_exists( script, script->mainns, "!__start__" );

		if( nsb != NULL )
		{
			func = nsb->data;
			rval = ferite_script_function_execute( script, script->mainns, NULL, func, NULL );
		
#ifdef THREAD_SAFE
			ferite_thread_group_wait( script, script->thread_group );
#endif
		
			/* clean up the system */
			if( rval != NULL )
			{
				if( F_VAR_TYPE(rval) == F_VAR_LONG && script->return_value == 0 )
				{
					script->return_value = VAI(rval);
				}
				ferite_variable_destroy( script, rval );
			}

			nsb = ferite_namespace_element_exists( script, script->mainns, "err" );
			if( script->error_state == FE_ERROR_THROWN ) /* error propogated up the system */
			{
				err = nsb->data;
				errstr = ferite_object_get_var( script, VAO(err), "str" );
				erno = ferite_object_get_var( script, VAO(err), "num" );
				if( script->error == NULL )
				{
					script->error = ferite_buffer_new( script, 0 );
					ferite_buffer_printf( script, script->error, "\n\n[ferite] Fatal Error: Execution stopped: On line %d, in file '%s':\n%s\n", script->current_op_line, script->current_op_file, VAS(errstr)->data );
				}
				FE_LEAVE_FUNCTION( 0 );
			}
			script->is_executing = FE_FALSE;
			FE_LEAVE_FUNCTION( FE_TRUE );
		}
	}
	else
	  ferite_error( script, 0, "Fatal Error: Unable to execute script - looks like the compile failed.\n" );
	FE_LEAVE_FUNCTION( FE_FALSE );
}

/**
 * @function ferite_script_function_execute
 * @declaration int ferite_script_function_execute( FeriteScript *script, void *container, FeriteFunction *function, FeriteVariable **params )
 * @brief Execute a function within a script
 * @param FeriteScript *script The current script
 * @param void *container The container the function is running from namespace/class/object
 * @param FeriteFunction *function The function to execute
 * @param FeriteVariable **params The calling arguments
 * @return The variable returned from the function
 */

FE_NATIVE_FUNCTION( ferite_script_function_execute )
{
	FeriteExecuteRec	 exec;
	FeriteStack			 exec_stack;
	FeriteVariable	  *targetvar = NULL, *rval = NULL;
	int i = 0, stop_execute = 0;
	int sig_count = function->arg_count;
	int offset = 3;

	FE_ENTER_FUNCTION;

	/*{{{ Checks before we do anything */
	FE_ASSERT( script != NULL && function != NULL );

	if( !script->is_executing )
	  stop_execute = FE_TRUE;

	script->keep_execution = FE_TRUE;
	/*}}}*/

	/*{{{ Obtain function, and create local variable hash */
	exec.function = function;
	exec.variable_list = (FeriteVariable**)ferite_duplicate_stack_contents( script, function->localvars, (void*(*)(FeriteScript*,void*,void*))ferite_duplicate_variable, NULL );
	exec.stack = &exec_stack;
	exec.stack->stack = fmalloc( sizeof( void * ) * 32 );
	exec.stack->stack_ptr = 0;
	exec.stack->size = 32;
	exec.block_depth = 0;
	exec.parent = script->gc_stack;
	script->gc_stack = &exec;
	/*}}}*/

	/*{{{ Copy parameter values over to the local variable stack */
	if( params != NULL ) /* handle function parameters */
	{
		/*
		 * Go through at add the all variables to the local variables. We either want to jump out when we hit a '.' or when we hit
		 * the super/self variables.
		 */
		for( i = 0; i < sig_count && function->signature[i] != NULL && !(function->signature[i]->is_dots); i++ )
		{
			/* Assign and use the return to add to the fncArgs array */
			if( function->signature[i]->pass_type == FE_BY_VALUE )
			{
				ferite_variable_fast_assign( script, exec.variable_list[i+offset], params[i] );
			}
			else				
			{	
				if( !FE_VAR_IS_DISPOSABLE(params[i]) )
				{
					/* We delete the existing var and then replace with ours */
					ferite_variable_destroy( script, exec.variable_list[i+offset] );
					exec.variable_list[i+offset] = ferite_get_variable_ref( script, params[i] );
				} 
				else
				{
					ferite_variable_fast_assign( script, exec.variable_list[i+offset], params[i] );
				}
			}
		}

		/* We are now at the 'dot' if we have one, or at an object - we now pick up the self and super variables */
		/* sort out super */
		targetvar = exec.variable_list[2];
		if( targetvar != NULL )
		{
			if( function->is_static )
			{ 
				/* We need this due to directives */  
				if( function->klass == NULL ) 
				{
					targetvar->type = F_VAR_NS;
					VAN(targetvar) = ((FeriteNamespace*)__container__)->container;
				}
				else 
				{
					targetvar->type = F_VAR_CLASS;
					VAC(targetvar) = ((FeriteClass*)__container__)->parent;
				}
			}
			else if( function->klass != NULL )
			{
				targetvar->type = F_VAR_OBJ;
				VAO(targetvar) = __container__;
				FINCREF(VAO(targetvar));
			}
			else
			{
				targetvar->type = F_VAR_NS;
				VAN(targetvar) = ((FeriteNamespace*)__container__)->container;
			}
		}
		
		/* sort out self */
		targetvar = exec.variable_list[1];
		if( targetvar != NULL )
		{
			if( function->is_static )
			{ 
				/* We need this due to directives */  
				if( function->klass == NULL ) 
				{
					targetvar->type = F_VAR_NS;
					VAN(targetvar) = ((FeriteNamespace*)__container__);
				}
				else 
				{
					targetvar->type = F_VAR_CLASS;
					VAC(targetvar) = ((FeriteClass*)__container__);
				}
			}
			else if( function->klass != NULL )
			{
				targetvar->type = F_VAR_OBJ;
				VAO(targetvar) = __container__;
				FINCREF(VAO(targetvar));
			}
			else
			{
				targetvar->type = F_VAR_NS;
				VAN(targetvar) = __container__;
			}
		}
	}
	/*}}}*/

	FUD(("EXECUTOR: Executing function %s %p\n", function->name, function->bytecode->list[1] ));

	script->stack_level++;
	if( script->stack_level > FE_DEEPEST_STACK_LEVEL )
	{
		ferite_error( script, 0, "Stack level too deep! (%d)\n", script->stack_level );
		FE_LEAVE_FUNCTION( rval );
	}
	
	rval = ferite_script_real_function_execute( script, __container__, current_recipient, function, script->mainns, &exec, params );
	script->stack_level--;

	script->gc_stack = exec.parent;
	ferite_clean_up_exec_rec( script, &exec );
	ffree(exec.stack->stack);

	if( stop_execute )
		script->is_executing = FE_FALSE;

	FE_LEAVE_FUNCTION( rval );
}

#ifndef WIN32
#define HAVE_INLINE inline
#else
#define HAVE_INLINE 
#endif

typedef struct __ferite_opcode_context
{
	long current_op_loc;
	int keep_function_running;
	FeriteObject *new_yield_block;
} FeriteOpcodeContext;

#define INLINE_OP_PARAMS	   FeriteScript *script, void *container, FeriteObject *current_recipient, FeriteOpcodeContext *context, \
							   FeriteFunction *function, FeriteNamespace *mainns, FeriteExecuteRec *exec, \
							   FeriteVariable **params, FeriteOp *current_op
#define INLINE_OP( NAME )	   static HAVE_INLINE void *NAME( INLINE_OP_PARAMS )
#define INLINE_OP_PTR( NAME )  void *(*NAME)( INLINE_OP_PARAMS )

#define CALL_INLINE_OP( NAME ) NAME( script, container, current_recipient, context, function, mainns, exec, params, current_op )

char *ferite_parameters_to_string( FeriteScript *script, FeriteVariable **param_list )
{
	static char buffer[1024];
	int i = 0;
	
	memset( buffer, '\0', 1024 );
	while( param_list[i] != NULL )
	{
		strcat( buffer, ferite_variable_id_to_str( script, F_VAR_TYPE(param_list[i]) ) );
		if( param_list[i+1] != NULL )
			strcat( buffer, "," );
		i++;
	}
	return buffer;
}
#ifdef DEBUG
int ferite_execute_call_depth = 0;
#endif

INLINE_OP( ferite_exec_funcall )
{
	FeriteNamespace		*ns = NULL;
	FeriteNamespaceBucket *nsb = NULL;
	FeriteVariable		*varone = NULL, *vartwo = NULL;
	FeriteVariable		*param_list[FE_FUNCTION_PARAMETER_MAX_SIZE], *rval = NULL;
	FeriteFunction		*trgt_function_call = NULL;
	FeriteClass			*swapped_parent_class = NULL;
	int					i, j, _arg_count;
	char				  *method_name = NULL;
	void				  *function_container = NULL;

	/*{{{ Create Parameter List */
	FUD(("CREATING PARAMETER LIST.\n"));
#ifdef DEBUG
	ferite_execute_call_depth++;
#endif
	memset(param_list, 0, sizeof(FeriteVariable*) * FE_FUNCTION_PARAMETER_MAX_SIZE);
	_arg_count = current_op->opdataf->argument_count;
	exec->stack->stack_ptr -= _arg_count;
	for( i = exec->stack->stack_ptr + 1, j = 0; j < _arg_count; i++, j++ ) {
		param_list[j] = exec->stack->stack[i];
		LOCK_VARIABLE( param_list[j] );
	}
	param_list[j] = NULL;
	FUD(("PARAMETER LIST CREATED.\n"));
	/*}}}*/
	switch( current_op->OP_TYPE ) {
		case F_OP_DELIVER: {
			if( current_recipient != NULL ) {
				// FUNCALL
				FeriteFunction *f = ferite_object_get_function( script, current_recipient, "invoke" );
				FUD(( "%s>>> Deliver: Pre: %p\n", ferite_stroflen(' ', (ferite_execute_call_depth*2)), context->new_yield_block ));
				rval = ferite_call_function( script, current_recipient, context->new_yield_block, f, param_list );
				FUD(( "%s<<< Deliver: Post: %p\n",  ferite_stroflen(' ', (ferite_execute_call_depth*2)),context->new_yield_block ));
			}	
			else
				ferite_error( script, 0, "Trying to call deliver when no block is supplied!\n" );
			break;
		}
		/*{{{ F_OP_METHOD (object and namespace) */
		case F_OP_METHOD: {
			vartwo = ferite_stack_pop( script, exec->stack );
			method_name = (char*)(current_op->opdata);
			function_container = VAP(vartwo);
			switch( F_VAR_TYPE(vartwo) ) {
				case F_VAR_OBJ: {
					FUD(( "Trying to find '%s' in '%s'\n", method_name, VAO(vartwo)->klass->name ));
					/* what we do here is check to see if they are searching the super class, if so, we swap the objects			*/
					/* template with that of it's parents - if it has no parent we don't bother..., we then look for the function  */
					/* that we are after..... i personally would consider this to be, as we english say, "dash cunning"			*/
					if( VAO(vartwo) == NULL ) {
						ferite_error( script, 0, "Trying to access method '%s' in a null object.\n", method_name );
						break;
					}

					/* Supar class */
					swapped_parent_class = VAO(vartwo)->klass;
					if( C_SOURCE_RELATIVE(current_op) ) { /* check for super */
						if( function->klass->parent != NULL && function->klass->parent != script->closureClassPtr )
							VAO(vartwo)->klass = function->klass->parent;
						else if( VAO(vartwo)->klass->parent != NULL )
							VAO(vartwo)->klass = VAO(vartwo)->klass->parent;
					}

					trgt_function_call = ferite_object_get_function_for_params( script, VAO(vartwo), method_name, param_list );
					VAO(vartwo)->klass = swapped_parent_class;

					/* Check to see if we have a function using the std methods */
					if( trgt_function_call == NULL ) {
						/* ok this is where we try and be dash cunning, fail miserably and settle for an autoload function. */
						trgt_function_call = ferite_object_get_function( script, VAO(vartwo), "method_missing" );
						if( trgt_function_call == NULL ) {
							char *klass_name = ferite_generate_class_fqn( script, VAO(vartwo)->klass );
							ferite_error( script, 0, "Unable to find method '%s(%s)' in object created from class '%s' (it could be static)\n", method_name, ferite_parameters_to_string(script, param_list), klass_name );
							ffree(klass_name);
							break;
						}
						else {
							/* we have an autoload function, soooooooo, what do we do? */
							varone = ferite_create_string_variable_from_ptr( script, "function-name", method_name, 0, FE_CHARSET_DEFAULT, FE_STATIC );
							ferite_add_to_parameter_list( param_list, varone );
							MARK_VARIABLE_AS_DISPOSABLE( varone );
							_arg_count++;
						}
					}

					if( trgt_function_call != NULL ) {
						/* Check the access controls on the function */
						char *klass_name = ferite_generate_class_fqn( script, VAO(vartwo)->klass );
						switch( trgt_function_call->state ) {
							case FE_ITEM_IS_PRIVATE: {
								FeriteVariable *tself = NULL;
								if( C_SOURCE_SELF(current_op) && function->klass && function->klass->parent == script->closureClassPtr )
									tself = ferite_object_get_var( script, (FeriteObject*)container, "self" );
								if( !(C_SOURCE_SELF(current_op) && ((tself && VAO(tself)->klass == trgt_function_call->klass) || (trgt_function_call->klass == function->klass))) ) {
									ferite_error( script, 0, "Trying to access a private method '%s' in object created from class '%s'\n", trgt_function_call->name, klass_name );
								}
								break;
							}
							case FE_ITEM_IS_PROTECTED: {
								FeriteVariable *tself = NULL;
								if( (C_SOURCE_SELF(current_op) || C_SOURCE_RELATIVE(current_op)) && function->klass && function->klass->parent == script->closureClassPtr ) {
									tself = ferite_object_get_var( script, (FeriteObject*)container, "self" );
								}
								if( !((C_SOURCE_SELF(current_op) || C_SOURCE_RELATIVE(current_op)) && ((tself && ferite_class_is_subclass(trgt_function_call->klass,VAO(tself)->klass)) || ferite_class_is_subclass(trgt_function_call->klass,function->klass))) ) {
									ferite_error( script, 0, "Trying to access a protected method '%s' in object created from class '%s'\n", trgt_function_call->name, klass_name );
								}
								break;
							}
							case FE_ITEM_IS_PUBLIC:
							default:
								/* Do Nothing */
								break;
						}
						ffree(klass_name);
					}
					break;
				}
				case F_VAR_CLASS: {
					trgt_function_call = ferite_class_get_function_for_params( script, VAC(vartwo), method_name, param_list );

					/* Do we have it */
					if( trgt_function_call == NULL ) {
						/* ok this is where we try and be dash cunning, fail miserably and settle for an autoload function. */
						trgt_function_call = ferite_class_get_function( script, VAC(vartwo), "method_missing" );
						if( trgt_function_call == NULL ) {
							char *klass_name = ferite_generate_class_fqn( script, VAC(vartwo));
							ferite_error( script, 0, "Unable to find method '%s(%s)' in class '%s' (it could be static)\n", method_name, ferite_parameters_to_string(script, param_list), klass_name );
							ffree(klass_name);
							break;
						}
						else {
							/* we have an autoload function, soooooooo, what do we do? */
							varone = ferite_create_string_variable_from_ptr( script, "function-name", method_name, 0, FE_CHARSET_DEFAULT, FE_STATIC );
							ferite_add_to_parameter_list( param_list, varone );
							MARK_VARIABLE_AS_DISPOSABLE( varone );
							_arg_count++;
						}
					}
					/* Check the access controls on the function */
					if( trgt_function_call != NULL  && FE_FALSE ) {
						char *klass_name = ferite_generate_class_fqn( script, VAC(vartwo) );
						switch( trgt_function_call->state ) {
							case FE_ITEM_IS_PRIVATE: {
								FeriteVariable *tself = NULL;
								if( C_SOURCE_SELF(current_op) && function->klass && function->klass->parent == script->closureClassPtr ) {
									tself = ferite_object_get_var( script, (FeriteObject*)container, "self" );
								}
								if( !(C_SOURCE_SELF(current_op) && ((tself && VAC(tself) == trgt_function_call->klass) || (trgt_function_call->klass == function->klass))) )
									ferite_error( script, 0, "Trying to access a private method '%s' in class '%s'\n", trgt_function_call->name, klass_name );
								break;
							}
							case FE_ITEM_IS_PROTECTED: {
								FeriteVariable *tself = NULL;
								if( (C_SOURCE_SELF(current_op) || C_SOURCE_RELATIVE(current_op)) && function->klass && function->klass->parent == script->closureClassPtr )
									tself = ferite_object_get_var( script, (FeriteObject*)container, "self" );
								if( !((C_SOURCE_SELF(current_op) || C_SOURCE_RELATIVE(current_op)) && ((tself && ferite_class_is_subclass(trgt_function_call->klass,VAC(tself))) || ferite_class_is_subclass(trgt_function_call->klass, function->klass))) )
									ferite_error( script, 0, "Trying to access a protected method '%s' in class '%s'\n", trgt_function_call->name, klass_name );
								break;
							}
							case FE_ITEM_IS_PUBLIC:
							default:
								/* Do Nothing */
								break;
						}
						ffree(klass_name);
					}
					break;
				}
				case F_VAR_NS: {
					nsb = ferite_namespace_element_exists( script, VAN(vartwo), method_name );
					if( nsb == NULL || nsb->type != FENS_FNC ) {
						nsb = ferite_namespace_element_exists( script, VAN(vartwo), "method_missing" );
						if( nsb == NULL  || nsb->type != FENS_FNC ) {
							char *klass_name = ferite_generate_namespace_fqn( script, VAN(vartwo) );							
							ferite_error( script, 0, "Unable to find method '%s(%s)' in namespace '%s'\n", method_name, ferite_parameters_to_string(script, param_list), klass_name );
							ffree(klass_name);
							break;
						}
						else {
							/* we have an autoload function, soooooooo, what do we do? */
							varone = ferite_create_string_variable_from_ptr( script, "function-name", method_name, 0, FE_CHARSET_DEFAULT, FE_STATIC );
							ferite_add_to_parameter_list( param_list, varone );
							MARK_VARIABLE_AS_DISPOSABLE( varone );								
							_arg_count++;
						}
					}
					trgt_function_call = nsb->data;						
					if( trgt_function_call != NULL ) {
						for( ; trgt_function_call != NULL; trgt_function_call = trgt_function_call->next ) {
							if( ferite_check_params( script, param_list, trgt_function_call ) != 0 )
								break;
						}

						if( trgt_function_call == NULL ) {
							ferite_error( script, 0, "Unable to find method %s.%s(%s) that accepts the parameters passed.\n", 
							VAN(vartwo)->name, method_name, ferite_parameters_to_string(script, param_list) );
						}
					}
					break;
				}
				case F_VAR_STR:
				case F_VAR_UARRAY:
				case F_VAR_DOUBLE:
				case F_VAR_LONG: {
					char *namespace_search = NULL;
					switch( F_VAR_TYPE(vartwo) ) {
						case F_VAR_STR: 
							namespace_search = "String";
							break;
						case F_VAR_UARRAY: 
							namespace_search = "Array";
							break;
						case F_VAR_DOUBLE:
						case F_VAR_LONG: 
							namespace_search = "Number";
							break;
					}
					nsb = ferite_namespace_element_exists( script, script->mainns, namespace_search );
					if( nsb != NULL ) {
						FeriteVariable *self_variable = vartwo;
						function_container = ns = nsb->data;
						nsb = ferite_namespace_element_exists( script, ns, method_name );

						param_list[_arg_count + 1] = NULL;
						for( i = _arg_count; i > 0; i-- )
							param_list[i] = param_list[i-1];
						if( FE_VAR_IS_DISPOSABLE(vartwo) )
							self_variable = ferite_get_variable_ref( script, vartwo );
						LOCK_VARIABLE(vartwo);
						param_list[0] = self_variable;
						_arg_count++;
						
						if( nsb == NULL || nsb->type != FENS_FNC ) {
							nsb = ferite_namespace_element_exists( script, ns, "method_missing" );
							if( nsb == NULL  || nsb->type != FENS_FNC ) {
								ferite_error( script, 0, "Unable to find method '%s(%s)' in %s namespace.\n", method_name, ferite_parameters_to_string(script, param_list),	namespace_search );
								break;
							}
							else {
								/* we have an autoload function, soooooooo, what do we do? */
								varone = ferite_create_string_variable_from_ptr( script, "function-name", method_name, 0, FE_CHARSET_DEFAULT, FE_STATIC );
								ferite_add_to_parameter_list( param_list, varone );
								MARK_VARIABLE_AS_DISPOSABLE( varone );
								_arg_count++;
							}
						}
						trgt_function_call = nsb->data;
						if( trgt_function_call != NULL ) {
							for( ; trgt_function_call != NULL; trgt_function_call = trgt_function_call->next ) {
								if( ferite_check_params( script, param_list, trgt_function_call ) != 0 )
									break;
							}
							if( trgt_function_call == NULL ) {
								ferite_error( script, 0, "Unable to find method %s.%s(%s) that accepts the parameters passed.\n",
								namespace_search, method_name, ferite_parameters_to_string(script, param_list) );
							}
						}
					}
					else {
						char *lowercase_namespace = fstrdup( namespace_search );
						ferite_lowercase( lowercase_namespace );
						ferite_error( script, 0, "Unable to find the %s namespace, please add 'uses \"%s\";' to your script.\n", namespace_search, lowercase_namespace );
						ffree( lowercase_namespace );
					}
					break;
				}
				default: {
					ferite_error( script, 0, "Expecting container found %s when trying to call the function %s()\n", ferite_variable_id_to_str( script, F_VAR_TYPE(vartwo) ), method_name );
					break;
				}
			}

			/* make a quick gettaway */
			if( script->error_state == FE_ERROR_THROWN ) {
				if( vartwo && FE_VAR_IS_DISPOSABLE( vartwo ) ) /* the var was created */
					ferite_variable_destroy( script, vartwo );
				break;
			}

			if( trgt_function_call != NULL ) {
				// FUNCALL
				LOCK_VARIABLE(trgt_function_call); /* lock the method */
				if( trgt_function_call->type == FNC_IS_EXTRL ) {
					if( trgt_function_call->fncPtr != NULL ) {
						EXTERNAL_ENTER( trgt_function_call );
						{
							FE_ENTER_NAMED_FUNCTION( trgt_function_call->name );
							if( trgt_function_call->native_information != NULL ) {
								script->current_op_file = trgt_function_call->native_information->file;
								script->current_op_line = trgt_function_call->native_information->line;
							}
							rval = (trgt_function_call->fncPtr)( script, function_container, context->new_yield_block, trgt_function_call, param_list );
							FE_LEAVE_NAMED_FUNCTION( trgt_function_call->name );
						}
						EXTERNAL_EXIT();
					}
					else
						ferite_error( script, 0, "Unable to execute external method %s (does it exist?)\n", trgt_function_call->name );
				}
				else /* internal function call */
				{
					rval = ferite_script_function_execute( script, function_container, context->new_yield_block, trgt_function_call, param_list );
				}
				UNLOCK_VARIABLE(trgt_function_call); /* unlock the method */
				if( script->error_state != FE_ERROR_THROWN ) {
					script->current_op_file = function->bytecode->filename; /* we do this because the other function might cause it to change */
					script->current_op_line = current_op->line;
				}
			}

			if( vartwo && FE_VAR_IS_DISPOSABLE( vartwo ) ) /* the var was created */
				ferite_variable_destroy( script, vartwo );
			break;
		}
		/*}}}*/
		/*{{{ F_OP_NEWOBJ */
		case F_OP_NEWOBJ: {
			FUD(("Creating new object\n"));
			vartwo = ferite_stack_pop( script, exec->stack );
			if( F_VAR_TYPE(vartwo) != F_VAR_CLASS ) {
				ferite_error( script, 0, "%s is not a class, bad luck, try again :)\n", ferite_variable_id_to_str(script,vartwo->type) );
				break;
			}
			rval = (void *)ferite_new_object( script, VAC(vartwo), param_list );
			if( script->error_state != FE_ERROR_THROWN ) {
				script->current_op_file = function->bytecode->filename; /* we do this because the other function might cause it to change */
				script->current_op_line = current_op->line;
			}
			if( vartwo && FE_VAR_IS_DISPOSABLE( vartwo ) ) /* the var was created */
				ferite_variable_destroy( script, vartwo );
			break;
		}
		/*}}}*/
	}
	for( i = 0; i < _arg_count; i++ ) {
		if( param_list[i] != NULL ) {
		UNLOCK_VARIABLE(param_list[i]);
		if( FE_VAR_IS_DISPOSABLE( param_list[i] ) )
			ferite_variable_destroy( script, param_list[i] );
		param_list[i] = NULL;
	}
	else
		break;
	}
	if( rval != NULL )
		ferite_stack_push( script, exec->stack, rval );

	if( context->new_yield_block != NULL ) {
		FDECREF(context->new_yield_block);
		context->new_yield_block = NULL;
	}
#ifdef DEBUG
	ferite_execute_call_depth--;
#endif
	return NULL;
}	
INLINE_OP( ferite_exec_pushattr )
{
	FeriteNamespaceBucket *nsb = NULL;
	FeriteVariable		*varone = NULL, *vartwo = NULL;
	FeriteVariable		**pml = NULL;
	FeriteString		  *str = NULL;
	FeriteFunction		*trgt_function_call = NULL;
	FeriteObjectVariable  *ov = NULL;
	char				  *var_name = NULL;
	
	var_name = (char *)(current_op->opdata);
	vartwo = ferite_stack_pop( script, exec->stack );
	switch( F_VAR_TYPE(vartwo) )
	{
		case F_VAR_OBJ:
		{
			FUD(( "Executing: Searching for '%s' in '%p'\n", var_name, VAP(vartwo) ));
			if( VAO(vartwo) != NULL )
			{
				/* Setup the variables in case of super */
				ov = VAO(vartwo)->variables;
				if( C_SOURCE_RELATIVE(current_op) )
					VAO(vartwo)->variables = VAO(vartwo)->variables->parent;
				
				varone = ferite_object_get_var( script, VAO(vartwo), var_name );
				
				/* Return to normal use */
				VAO(vartwo)->variables = ov;
				
				if( varone == NULL )
				{
					/* we see if the object has a .attribute_missing method - and call it */
					trgt_function_call = ferite_object_get_function( script, VAO(vartwo), "attribute_missing" );
					if( trgt_function_call == NULL )
					{
						ferite_error( script, 0, "Trying to access non-existant variable '%s' in object created from class '%s' (it could be static)\n", var_name, VAO(vartwo)->klass->name );
						break;
					}
					else
					{
						// FUNCALL
#ifdef DEBUG
						ferite_execute_call_depth++;
#endif
						FUD(( "%s>>> attribute_missing: Pre: %s: %p\n",  ferite_stroflen(' ', (ferite_execute_call_depth*2)), var_name, context->new_yield_block ));
						str = ferite_str_new( script, var_name, strlen( var_name ), FE_CHARSET_DEFAULT );
						pml = ferite_create_parameter_list_from_data( script, "s", str );
						varone = ferite_call_function( script, VAP(vartwo), context->new_yield_block, trgt_function_call, pml );
						ferite_delete_parameter_list( script, pml );
						ferite_str_destroy( script, str );
						FUD(( "%s<<< attribute_missing: Post: %s: %p\n",  ferite_stroflen(' ', (ferite_execute_call_depth*2)), var_name, context->new_yield_block ));
#ifdef DEBUG
						ferite_execute_call_depth--;
#endif
					}
				}
				
				if( varone != NULL )
				{
					switch( varone->state )
					{
						case FE_ITEM_IS_PRIVATE: {
							FeriteVariable *tself = NULL;
							FeriteClass *var_class = ferite_object_variable_class( script, VAO(vartwo), var_name );
							if( C_SOURCE_SELF(current_op) && function->klass && function->klass->parent == script->closureClassPtr )
								tself = ferite_object_get_var( script, (FeriteObject*)container, "self" );
							if( !(C_SOURCE_SELF(current_op) && ((tself && VAO(tself)->klass == var_class) || (var_class == function->klass))) )
								ferite_error( script, 0, "Trying to access a private variable '%s' in object created from class '%s'\n", var_name, VAO(vartwo)->klass->name );
							break;
						}
						case FE_ITEM_IS_PROTECTED: {
							FeriteVariable *tself = NULL;
							FeriteClass *var_class = ferite_object_variable_class( script, VAO(vartwo), var_name );
							if( (C_SOURCE_SELF(current_op) || C_SOURCE_RELATIVE(current_op)) && function->klass && function->klass->parent == script->closureClassPtr ) {
								tself = ferite_object_get_var( script, (FeriteObject*)container, "self" );
							}
							if( !((C_SOURCE_SELF(current_op) || C_SOURCE_RELATIVE(current_op)) && ((tself && ferite_class_is_subclass(var_class,VAO(tself)->klass)) || ferite_class_is_subclass(var_class, function->klass))) )
								ferite_error( script, 0, "Trying to access a protected variable '%s' in object created from class '%s'\n", var_name, VAO(vartwo)->klass->name );
							break;
						}
						case FE_ITEM_IS_PUBLIC:
						default:
							/* Do Nothing */
							break;
					}
				}
			}
			else
				ferite_error( script, 0, "Trying to access variable '%s' in a null\n", var_name );
			break;
		}
		case F_VAR_NS:
		{
			nsb = ferite_namespace_element_exists( script, VAN(vartwo), var_name );
			if( nsb == NULL )
			{
	
				if( varone == NULL )
				{
					/* we see if the object has a .attribute_missing method - and call it */
					trgt_function_call = ferite_find_namespace_element_contents( script, VAN(vartwo), "attribute_missing", FENS_FNC );
					if( trgt_function_call == NULL )
					{
						ferite_error( script, 0, "Can't find '%s' in namespace '%s'\n", var_name, VAN(vartwo)->name );
						break;
					}
					else
					{
						str = ferite_str_new( script, var_name, strlen( var_name ), FE_CHARSET_DEFAULT );
						pml = ferite_create_parameter_list_from_data( script, "s", str );
						varone = ferite_call_function( script, VAP(vartwo), context->new_yield_block, trgt_function_call, pml );
						ferite_delete_parameter_list( script, pml );
						ferite_str_destroy( script, str );
					}
				}
			}
			if( nsb != NULL ) {
				switch( nsb->type )
				{
					case FENS_NS:
						varone = ferite_create_namespace_variable( script, var_name, nsb->data, FE_STATIC );
						MARK_VARIABLE_AS_DISPOSABLE( varone );
						break;
					case FENS_VAR:
						varone = nsb->data;
						break;
					case FENS_CLS:
						varone = ferite_create_class_variable( script, var_name, nsb->data, FE_STATIC );
						MARK_VARIABLE_AS_DISPOSABLE( varone );
						break;
					default:					
						ferite_error( script, 0, "Expecting variable, class or namespace for '%s' but found function.\n", var_name );
						break;
				}
			}
			break;
		}
		case F_VAR_CLASS:
		{
			varone = ferite_class_get_var( script, VAC(vartwo), var_name );
			if( varone == NULL )
			{
				/* we see if the object has a .attribute_missing method - and call it */
				trgt_function_call = ferite_class_get_function( script, VAC(vartwo), "attribute_missing" );
				if( trgt_function_call == NULL )
				{
					ferite_error( script, 0, "Trying to access non-existant variable '%s' in class '%s' (it could be NON-static)\n", var_name, VAC(vartwo)->name );
					break;
				}
				else
				{
					// FUNCALL
#ifdef DEBUG
					ferite_execute_call_depth++;
#endif
					FUD(( "%s>>> attribute_missing: Pre: %s: %p\n",  ferite_stroflen(' ', (ferite_execute_call_depth*2)), var_name, context->new_yield_block ));
					str = ferite_str_new( script, var_name, strlen( var_name ), FE_CHARSET_DEFAULT );
					pml = ferite_create_parameter_list_from_data( script, "s", str );
					varone = ferite_call_function( script, VAP(vartwo), context->new_yield_block, trgt_function_call, pml );
					ferite_delete_parameter_list( script, pml );
					ferite_str_destroy( script, str );
 					FUD(( "%s<<< attribute_missing: Post: %s: %p\n",  ferite_stroflen(' ', (ferite_execute_call_depth*2)), var_name, context->new_yield_block ));
#ifdef DEBUG
					ferite_execute_call_depth++;
#endif
				}
			}
			
			if( varone != NULL )
			{
				switch( varone->state )
				{
					case FE_ITEM_IS_PRIVATE: {
						FeriteVariable *tself = NULL;
						FeriteClass *var_class = ferite_class_variable_class( script, VAC(vartwo), var_name );
						if( C_SOURCE_SELF(current_op) && function->klass && function->klass->parent == script->closureClassPtr )
							tself = ferite_object_get_var( script, (FeriteObject*)container, "self" );
						if( !(C_SOURCE_SELF(current_op) && ((tself && VAC(tself) == var_class) || (var_class == function->klass))) )
							ferite_error( script, 0, "Trying to access a private variable '%s' in class '%s'\n", var_name, VAC(vartwo)->name );
						break;
					}
					case FE_ITEM_IS_PROTECTED: {
						FeriteVariable *tself = NULL;
						FeriteClass *var_class = ferite_object_variable_class( script, VAO(vartwo), var_name );
						if( (C_SOURCE_SELF(current_op) || C_SOURCE_RELATIVE(current_op)) && function->klass && function->klass->parent == script->closureClassPtr )
							tself = ferite_object_get_var( script, (FeriteObject*)container, "self" );
						if( !((C_SOURCE_SELF(current_op) || C_SOURCE_RELATIVE(current_op)) && ((tself && ferite_class_is_subclass(var_class,VAC(tself))) || ferite_class_is_subclass(var_class, function->klass))) )
							ferite_error( script, 0, "Trying to access a protected variable '%s' in class '%s'\n", var_name, VAC(vartwo)->name );
						break;
					}
					case FE_ITEM_IS_PUBLIC:
					default:
						/* Do Nothing */
						break;
				}
			}
			break;
		}
		case F_VAR_UARRAY:
		{
			FeriteVariable *_tmp = fe_new_str_static("var", var_name, 0, FE_CHARSET_DEFAULT);
			varone = (ferite_array->get)( script, VAUA(vartwo), _tmp );
			ferite_variable_destroy( script, _tmp );
			if( varone == NULL ) {
				ferite_error( script, 0, "Trying to access non-existant keyed variable '%s' in array\n", var_name );
			}
			break;
		}
		default:
		{
			ferite_error( script, 0, "Can not get variable '%s' expecting object/namespace/class but found a %s.\n",
						  (char *)(current_op->opdata),
						  ferite_variable_id_to_str( script, F_VAR_TYPE(vartwo) ) );
			break;
		}
	}
	if( vartwo && FE_VAR_IS_DISPOSABLE( vartwo ) ) {
		if( varone ) {
			varone = ferite_get_variable_ref( script, varone );
		}
		ferite_variable_destroy( script, vartwo );
	}
	if( script->error_state == FE_ERROR_THROWN )
		return NULL;
	
	if( varone != NULL )
		ferite_stack_push( script, exec->stack, varone );
	else
	{
		ferite_error( script, 0, "Can't Find Variable '%s'\n",  (char *)(current_op->opdata) );
		return NULL;
	}	
	
	return NULL;
}
INLINE_OP( ferite_exec_swaptop )
{
	int sp = exec->stack->stack_ptr;
	void *tmp = exec->stack->stack[sp - (current_op->addr)];
	exec->stack->stack[sp - (current_op->addr)] = exec->stack->stack[sp - (current_op->addr + 1)];
	exec->stack->stack[sp - (current_op->addr + 1)] = tmp;
	return NULL;
}
INLINE_OP( ferite_exec_binary )
{
	FeriteVariable		*varone = NULL, *vartwo = NULL, *result = NULL;
	FeriteVariable		*(*binaryop)( FeriteScript *s, FeriteOp*, FeriteVariable *a, FeriteVariable *b );
	
	FUD(("BINARY\n"));
	vartwo = ferite_stack_pop( script, exec->stack );
	varone = ferite_stack_pop( script, exec->stack );
	binaryop = (FeriteVariable *(*)( FeriteScript *, FeriteOp*, FeriteVariable *, FeriteVariable * ))ferite_op_table[current_op->addr].ptr;
	if( (result =  binaryop( script, current_op, varone, vartwo )) )
		ferite_stack_push( script, exec->stack, result );
	if( FE_VAR_IS_DISPOSABLE( vartwo ) )
	{
		ferite_variable_destroy( script, vartwo );
	}
	if( FE_VAR_IS_DISPOSABLE( varone ) )
	{
		ferite_variable_destroy( script, varone );
	}
	
	return NULL;
}
INLINE_OP( ferite_exec_push )
{
	FUD(("PUSH\n"));
	ferite_stack_push( script, exec->stack, current_op->opdata );
	
	return NULL;
}
INLINE_OP( ferite_exec_pop )
{
	FeriteVariable *varone = ferite_stack_pop( script, exec->stack );
	FUD(("POP\n"));
	if( varone && FE_VAR_IS_DISPOSABLE( varone ) )
	{
		ferite_variable_destroy( script, varone );
	}
	
	return NULL;
}	
INLINE_OP( ferite_exec_args )
{
	int argcount = ferite_get_parameter_count( params ), i = 0;
	FeriteVariable *varone = ferite_create_uarray_variable( script, "fncArgs", argcount+1, FE_STATIC ), *vartwo = NULL;
	
	for( i = 0; i < argcount; i++ )
	{
		vartwo = ferite_duplicate_variable( script, params[i], NULL );
		(ferite_array->append)( script, VAUA(varone), vartwo, NULL, FE_ARRAY_ADD_AT_END );
	}
	MARK_VARIABLE_AS_DISPOSABLE( varone );
	ferite_stack_push( script, exec->stack, varone );
	
	return NULL;
}	
INLINE_OP( ferite_exec_get_deliver )
{
	FeriteVariable *vartwo = ferite_create_object_variable_with_data( script, "get-yield", current_recipient, FE_STATIC );
	MARK_VARIABLE_AS_DISPOSABLE( vartwo );
	ferite_stack_push( script, exec->stack, vartwo );
	
	return NULL;
}
INLINE_OP( ferite_exec_set_deliver )
{			 
	FeriteVariable *vartwo = NULL;
	
	FUD(("SET YIELD\n"));	
	if( context->new_yield_block != NULL ) {
		FDECREF(context->new_yield_block);
		context->new_yield_block = NULL;
	}	
	if( current_op->addr == FE_TRUE ) /* If true we use the current */ {
		context->new_yield_block = current_recipient;
		FUD(( "%s!!! Set Deliver (Current Yield Block): %p\n",  ferite_stroflen(' ', (ferite_execute_call_depth*2)), current_recipient ));
	}
	else
	{
		vartwo = ferite_stack_pop( script, exec->stack );	
		context->new_yield_block = VAO(vartwo);
		FUD(( "%s!!! Set Deliver (From Object): %p\n",  ferite_stroflen(' ', (ferite_execute_call_depth*2)), VAO(vartwo) ));
	}
	if( context->new_yield_block != NULL ) {
		FINCREF(context->new_yield_block);
	}	
	if( vartwo != NULL && FE_VAR_IS_DISPOSABLE( vartwo ) )
		ferite_variable_destroy( script, vartwo );
	return NULL;
}	
INLINE_OP( ferite_exec_clsre_assgn )
{				
	char *name = NULL;
	FeriteObject *self = (FeriteObject*)container;
	FeriteVariable *varone = ferite_stack_pop( script, exec->stack );
	
	name = (char*)(current_op->opdata);
	ferite_object_set_var( script, self, name, ferite_get_variable_ref( script, varone ) );
	
	return NULL;
}
INLINE_OP( ferite_exec_pushvar )
{
	FeriteVariable *varone = NULL;
	FeriteNamespaceBucket *nsb = ferite_namespace_element_exists( script, mainns, (char *)(current_op->opdata) );
	
	FUD(("PUSHVAR\n"));

	if( nsb != NULL )
	{
		switch( nsb->type )
		{
			case FENS_NS:
				varone = ferite_create_namespace_variable( script, (char *)(current_op->opdata), nsb->data, FE_STATIC );
				MARK_VARIABLE_AS_DISPOSABLE( varone );
				break;
			case FENS_VAR:
				varone = nsb->data;
				break;
			case FENS_CLS:
				varone = ferite_create_class_variable( script, (char *)(current_op->opdata), nsb->data, FE_STATIC );
				MARK_VARIABLE_AS_DISPOSABLE( varone );
				break;
			default:
				ferite_error( script, 0, "PUSHVAR with wrong type\n" );
		}
	}
	
	if( script->error_state == FE_ERROR_THROWN )
		return NULL;
	
	if( varone != NULL )
		ferite_stack_push( script, exec->stack, varone );
	else
		ferite_error( script, 0, "Can't find item named '%s'\n",  (char *)(current_op->opdata) );
	
	return NULL;
}	
INLINE_OP( ferite_exec_pushindex )
{
	FeriteVariable *varone = exec->variable_list[current_op->addr];
	ferite_stack_push( script, exec->stack, varone );
	
	return NULL;
}
INLINE_OP( ferite_exec_unary )
{
	FeriteVariable		*varone = NULL, *result = NULL;
	FeriteVariable		*(*unaryop)( FeriteScript *s, FeriteOp*, FeriteVariable *a );

	FUD(("UNARY\n"));
	varone = ferite_stack_pop( script, exec->stack );
	unaryop = (FeriteVariable *(*)( FeriteScript *, FeriteOp*, FeriteVariable * ))ferite_op_table[current_op->addr].ptr;
	if( (result = unaryop( script, current_op, varone )) )
		ferite_stack_push( script, exec->stack, result );
	if( FE_VAR_IS_DISPOSABLE( varone ) )
	{
		ferite_variable_destroy( script, varone );
	}
	
	return NULL;
}
INLINE_OP( ferite_exec_many )
{
	FeriteVariable *(*manyop)(FeriteScript *s, FeriteOp*, FeriteVariable **vars,int count);
	FeriteVariable **many = NULL, *result = NULL;
	int n, *count = NULL;
	
	FUD(("MANY\n"));
	count = (int*)(current_op->opdata);
	many = fmalloc(sizeof(FeriteVariable*) * *count);
	for( n = 0; n < *count; n++ )
	{
		many[n] = ferite_stack_pop( script, exec->stack );
		LOCK_VARIABLE( many[n] );
	}
	manyop = (FeriteVariable *(*)(FeriteScript *, FeriteOp*, FeriteVariable **,int))ferite_op_table[current_op->addr].ptr;
	if( (result = manyop(script, current_op, many, *count)) )
		ferite_stack_push( script, exec->stack, result );
	for( n = 0; n < *count; n++ )
	{
		UNLOCK_VARIABLE( many[n] );
		if( FE_VAR_IS_DISPOSABLE( many[n] ) )
		{
			ferite_variable_destroy(script, many[n]);
		}
	}
	ffree(many);	
	
	return NULL;
}
INLINE_OP( ferite_exec_case )
{
	FeriteVariable		*varone = NULL, *vartwo = NULL, *result = NULL;
	/*
	 * This will simply take the first operand, duplicate it, and then push it back onto
	 * the stack. It then lets the switch statement run into the next block.
	 */
	vartwo = ferite_stack_pop( script, exec->stack );
	varone = ferite_stack_pop( script, exec->stack );
	result = ferite_duplicate_variable( script, varone, NULL );
	MARK_VARIABLE_AS_DISPOSABLE( result );
	ferite_stack_push( script, exec->stack, result );
	ferite_stack_push( script, exec->stack, varone );
	ferite_stack_push( script, exec->stack, vartwo );
	
	return CALL_INLINE_OP( ferite_exec_binary );
}	
INLINE_OP( ferite_exec_vrst )
{
	FeriteVariable *varone = ferite_stack_pop( script, exec->stack );
	ferite_variable_convert_to_type( script, varone, current_op->addr );	
	return NULL;
}
INLINE_OP( ferite_exec_jmp )
{
	FUD(("JMP\n"));
	context->current_op_loc = current_op->addr;	
	return NULL;
}
INLINE_OP( ferite_exec_bie )
{
	FeriteVariable *varone = ferite_stack_pop( script, exec->stack );
	FUD(("BIE\n"));
	if( !ferite_variable_is_false( script, varone ) )
	 {
	FUD(("BIE: Branching\n" ));
	context->current_op_loc = current_op->addr;
	 }
	else
	 {
	FUD(("BIE: Not Branching\n" ));
	 }
	if( varone != NULL && FE_VAR_IS_DISPOSABLE( varone ) ) /* the var was created */
	 ferite_variable_destroy( script, varone );	
	return NULL;
}
INLINE_OP( ferite_exec_bne )
{
	FeriteVariable *varone = ferite_stack_pop( script, exec->stack );
	/*{{{ F_OP_BNE		(branch if not equal)			  */
	FUD(("BNE\n"));
	if( ferite_variable_is_false( script, varone) )
	{
		FUD(("BNE: Branching\n" ));
		context->current_op_loc = current_op->addr;
	}
	else
	{
		FUD(("BNE: Not Branching\n" ));
	}
	if( FE_VAR_IS_DISPOSABLE( varone ) ) /* the var was created */
		ferite_variable_destroy( script, varone );	  
	return NULL;
}
INLINE_OP( ferite_exec_exit )
{
	FeriteVariable *return_val = NULL, *varone = NULL;
	/*{{{ F_OP_EXIT	  (exit and return from a function)  */
	FUD(("Exiting\n"));
	context->keep_function_running = FE_FALSE; /* quit the function */
	varone = ferite_stack_pop( script, exec->stack );
	return_val = ferite_duplicate_variable( script, varone, NULL );
	MARK_VARIABLE_AS_DISPOSABLE( return_val );
	if( varone && FE_VAR_IS_DISPOSABLE( varone ) ) {
		ferite_variable_destroy( script, varone );
	}
	return return_val;
}
INLINE_OP( ferite_exec_raise )
{
	FeriteVariable *error = ferite_stack_pop( script, exec->stack );
	
	if( F_VAR_TYPE(error) == F_VAR_OBJ && 
		ferite_class_is_subclass( ferite_find_namespace_element_contents( script, script->mainns, "Error", FENS_CLS ), VAO(error)->klass ) ) 
	{
		FeriteVariable *str = ferite_object_get_var( script, VAO(error), "str" );
		FeriteVariable *global_error = ferite_find_namespace_element_contents( script, script->mainns, "err", FENS_VAR );

		if( VAO(error) != VAO(global_error) ) {
			ferite_error( script, 0, "%s", VAS(str)->data );
			ferite_variable_fast_assign( script, global_error, error );
		} else {
			script->error_state = FE_ERROR_THROWN;
		}
	} else {
		ferite_error( script, 42, "Trying to raise exception with an expression that isn't an error object (an object created from subclass of Error)\n" );
	}
	
	if( error && FE_VAR_IS_DISPOSABLE( error ) ) {
		ferite_variable_destroy( script, error );
	}
	return NULL;
}

INLINE_OP( ferite_exec_push_global )
{
	FeriteVariable *static_bind = ferite_amt_get( script, script->globals, current_op->addr );

	if( static_bind == NULL ) 
	{
		if( current_op->opdata ) {
			FeriteNamespaceBucket *nsb = ferite_find_namespace( script, script->mainns, current_op->opdata, 0 );
			switch( nsb->type ) {
				case FENS_NS :
					static_bind = ferite_create_namespace_variable( script, "static-bind-reference-push", nsb->data, FE_STATIC );
					MARK_VARIABLE_AS_COMPILED( static_bind );
					break;
				case FENS_CLS:
					static_bind = ferite_create_class_variable( script, "static-bind-reference-push", nsb->data, FE_STATIC );
					MARK_VARIABLE_AS_COMPILED( static_bind );
					break;
				case FENS_VAR:
					static_bind = nsb->data;
					break;
				default:
					ferite_error( script, 0, "PUSHVAR with wrong type\n" );
			}
		} else {
			static_bind = ferite_create_namespace_variable( script, "static-bind-reference-push", script->mainns, FE_STATIC );
			MARK_VARIABLE_AS_COMPILED( static_bind );
		}
		ferite_amt_set( script, script->globals, current_op->addr, static_bind );
	}
	
	if( script->error_state == FE_ERROR_THROWN )
		return NULL;
	
	if( static_bind != NULL )
		ferite_stack_push( script, exec->stack, static_bind );
	else
		ferite_error( script, 0, "Can't find item named '%s'\n",  (char *)(current_op->opdata) );
		
	return NULL;
}

typedef struct __ferite_opcode_lookup 
{
	int type;
	INLINE_OP_PTR( op );
} FeriteOpcodeLookup;

FeriteOpcodeLookup ferite_opcode_table[] = {
	{ F_OP_NOP, NULL },
	{ F_OP_BINARY, ferite_exec_binary },
	{ F_OP_UNARY, ferite_exec_unary },
	{ F_OP_FUNCTION, ferite_exec_funcall }, 
	{ F_OP_METHOD, ferite_exec_funcall }, 
	{ F_OP_NEWOBJ, ferite_exec_funcall }, 
	{ F_OP_JMP, ferite_exec_jmp },
	{ F_OP_EXIT, ferite_exec_exit },
	{ F_OP_PUSH, ferite_exec_push },
	{ F_OP_PUSHVAR, ferite_exec_pushvar },
	{ F_OP_PUSHINDEX, ferite_exec_pushindex },
	{ F_OP_PUSHATTR, ferite_exec_pushattr },
	{ F_OP_POP, ferite_exec_pop },
	{ F_OP_BIE, ferite_exec_bie },
	{ F_OP_BNE, ferite_exec_bne },
	{ F_OP_CLSRE_ASSGN, ferite_exec_clsre_assgn },
	{ F_OP_ERR, NULL },
	{ F_OP_MANY, ferite_exec_many },
	{ F_OP_CASE, ferite_exec_case }, /* force onto next op, should be binary - needs to force onto next within case and goto binary ? */
	{ F_OP_ARGS, ferite_exec_args },
	{ F_OP_DELIVER, ferite_exec_funcall }, 
	{ F_OP_SET_DELIVER, ferite_exec_set_deliver }, 
	{ F_OP_GET_DELIVER, ferite_exec_get_deliver },
	{ F_OP_SWAP_TOP, ferite_exec_swaptop },
	{ F_OP_VRST, ferite_exec_vrst },
	{ F_OP_RAISE, ferite_exec_raise },
	{ F_OP_PUSHGLOBAL, ferite_exec_push_global }
};

void __ferite_clean_up_exec_rec_stack( FeriteScript *script, FeriteExecuteRec *exec )
{
	int i = 0, counter = 0;
	FeriteVariable *targetvar = NULL;
	
	FE_ENTER_FUNCTION;
	/*{{{ Clean up execution stack */
	counter = 0;	
	for( i = 1; i <= exec->stack->stack_ptr; i++ )
	{
		targetvar = exec->stack->stack[i];
		if( targetvar && FE_VAR_IS_DISPOSABLE( targetvar ) )
		{
			FUD(("[%d/%d] DESTROYING STACK VARIABLE %p (%s)\n", i, exec->stack->stack_ptr, targetvar, ferite_variable_id_to_str( script, F_VAR_TYPE(targetvar)) ));
			ferite_variable_destroy( script, targetvar );
			counter++;
		}
		exec->stack->stack[i] = NULL;
	}
	exec->stack->stack_ptr = 0;
	FUD(( "%d variables lefton stack\n", counter ));
	FUD(("IN TOTAL %d VARIABLES WHERE NOT DEALLOCATED\n", counter));
	/*}}}*/	
	FE_LEAVE_FUNCTION(NOWT);
}
/**
 * @function ferite_clean_up_exec_rec
 * @declaration void ferite_clean_up_exec_rec( FeriteScript *script, FeriteExecuteRec *exec )
 * @brief Clean up and Execution Record
 * @param FeriteScript *script The current script
 * @param FeriteExecuteRex *exec	Pointer to the execution record
 */
void ferite_clean_up_exec_rec( FeriteScript *script, FeriteExecuteRec *exec )
{
	int i = 0;
	FE_ENTER_FUNCTION;
	__ferite_clean_up_exec_rec_stack( script, exec );
	
	/*{{{ Clean up local scope variables */
	FUD(("DELETING LOCAL VARIABLES\n" ));
	for( i = 1; i <= exec->function->localvars->stack_ptr; i++ )
	{
		if( exec->variable_list[i] != NULL )
		  ferite_variable_destroy( script, exec->variable_list[i] );
	}
	ffree( exec->variable_list );
	/*}}}*/
	FE_LEAVE_FUNCTION( NOWT );
}


/**
 * @function ferite_script_real_function_execute
 * @declaration FeriteVariable *ferite_script_real_function_execute( FeriteScript *script, void *container, FeriteFunction *function, FeriteNamespace *ns, FeriteExecuteRec *exec, FeriteVariable **params )
 * @brief Execute a eval operator compiled script
 * @param FeriteScript *script The current script to run
 * @param void *container The container the function is running from namespace/class/object
 * @param FeriteFunction *function The function to execute
 * @param FeriteNamespace *ns The scripts main namespace
 * @param FeriteExecuteRec *exec The execute records for the current script
 * @param FeriteVariable **params The parameters passed to the function
 * @return The return value from the function on successfull execution, NULL otherwise
 */
FeriteVariable *ferite_script_real_function_execute( FeriteScript *script, void *container, FeriteObject *current_recipient, FeriteFunction *function, FeriteNamespace *mainns, FeriteExecuteRec *exec, FeriteVariable **params )
{
#define ERROR_UPPER_BOUND 99
	/*{{{ Variables */
	FeriteOp		*current_op = NULL;
	FeriteOp		**opcode_list = NULL;
	FeriteVariable *return_val = NULL;
	FeriteOpcodeContext *context, stack_context;
	int			 error_op_location = 0, error_array[ERROR_UPPER_BOUND + 1];
	/*}}}*/

	FE_ENTER_FUNCTION;
	context = &stack_context;
	
	context->current_op_loc = 0;
	context->new_yield_block = NULL;
	context->keep_function_running = FE_TRUE;
	
	opcode_list = function->bytecode->list;
	current_op = opcode_list[0];
	context->current_op_loc++;

	script->current_op_file = function->bytecode->filename;

	FUD(("EXECUTION STARTING\n"));
	while( context->keep_function_running && script->keep_execution )
	{
		FUD(("[%p] ", current_op ));
		exec->line = current_op->line;
		script->current_op_line = current_op->line;
		exec->block_depth = current_op->block_depth;

		if( ferite_opcode_table[current_op->OP_TYPE].op != NULL )
			return_val = CALL_INLINE_OP((ferite_opcode_table[current_op->OP_TYPE].op));
		else 
		{		
			switch( current_op->OP_TYPE )
			{
				case F_OP_ERR:
				{
					/*{{{ F_OP_ERR		(set an error handler)			 */
					if( current_op->addr == -1 )
					{  /* reset the error counter */
						script->error_state = FE_NO_ERROR;
						if( error_op_location >= 0 )
							error_array[error_op_location] = 0;
						if( error_op_location > 0 )
							error_op_location--;
						FUD(( "%s--- Remove Exception: %d\n",  ferite_stroflen(' ', (ferite_execute_call_depth*2)), error_op_location ));						
					}
					else
					{
						FUD(("ERROR HANDLER: Setting Error location to %ld",  current_op->addr ));
						FUD(( "%s+++ Instate Exception: %d (%ld)\n",  ferite_stroflen(' ', (ferite_execute_call_depth*2)), error_op_location, current_op->addr ));						
						error_array[error_op_location] = current_op->addr;
						error_op_location++;
						if( error_op_location >= ERROR_UPPER_BOUND ) {
							ferite_error( script, 0, "Error Stack Overflow!\n" );
							error_op_location = 0;
						}
					}
					break;
				}
				case F_OP_NOP:
					break;
				default:
					ferite_error( script, 0, "Unknown op type [%d]\n", current_op->OP_TYPE );
			}
		}
		
		/*{{{ error checking */
		FUD(( "ERROR STATE: %d\n", script->error_state ));
		if( script->error_state != FE_ERROR_THROWN )
		{
			current_op = opcode_list[context->current_op_loc];
			context->current_op_loc++;
		}
		else
		{
			FUD(( "ERROR STATE: reported...\n" ));
			if( error_op_location < 1 || error_array[error_op_location-1] == 0 )
			{ 
				/* there is no error handler propogate upwards */
				FUD(( "ERROR STATE: No error handler found... bombing out.\n" ));
				FUD(( "EXEC: detected error - stoping execution\n" ));
				ferite_error( script, 0, "	in %s:%d (function: '%s' in '%s')\n", function->bytecode->filename, current_op->line, function->name,
							  (function->klass ? function->klass->name : 
												 (((FeriteNamespace*)container)->name == NULL ? "top-level-script" : ((FeriteNamespace*)container)->name)));
				context->keep_function_running = FE_FALSE;
			}
			else
			{
				FUD(( "ERROR STATE: Going to error handler code\n" ));
				context->current_op_loc = error_array[error_op_location-1];
				current_op = opcode_list[context->current_op_loc];
				context->current_op_loc++;
				/* We clean the stack because the exception could have occured mid statement */
				__ferite_clean_up_exec_rec_stack( script, exec );
				/* We also need to reset the error log otherwise we will get errors we have caught */
				ferite_reset_errors( script );
			}
		}
		
		if( !context->keep_function_running || !script->keep_execution )
			break;
		
		/*{{{ GARBAGE COLLECTOR */
		script->gc_count++;
		if( script->gc_count > FE_GC_RUN_AFTER_OPS )
			ferite_check_gc( script );		
		/*}}}*/
	}

	FUD(("EXECUTION COMPLETE. Have a nice day :). (%s)\n", function->name));
	FE_LEAVE_FUNCTION( return_val );
}

/**
 * @function ferite_create_parameter_list
 * @declaration FeriteVariable **ferite_create_parameter_list( FeriteScript *script, int size )
 * @brief Create a parameter list, NULLify it and then return it
 * @param FeriteScript *script The script
 * @param int size The number of parameters to hold
 * @return The created list
 */
FeriteVariable **ferite_create_parameter_list( FeriteScript *script, int size )
{
	FeriteVariable **list = NULL;

	FE_ENTER_FUNCTION;
	list = fcalloc( sizeof( FeriteVariable* ) * (size+1), sizeof(char) );
	FE_LEAVE_FUNCTION( list );
}

/**
 * @function ferite_add_to_parameter_list
 * @declaration FeriteVariable **ferite_add_to_parameter_list( FeriteVariable **list, FeriteVariable *var )
 * @brief Place a parameter within the next availible place within the parameter list
 * @param FeriteVariable **list The list to place it in
 * @param FeriteVariable *var  The variable to place within the list
 * @return The list passed to the function
 */
FeriteVariable **ferite_add_to_parameter_list( FeriteVariable **list, FeriteVariable *var )
{
	int size = ferite_get_parameter_count( list );

	FE_ENTER_FUNCTION;
	list[size] = var;
	list[size+1] = NULL;
	FE_LEAVE_FUNCTION( list );
}

/**
 * @function ferite_delete_parameter_list
 * @declaration void ferite_delete_parameter_list( FeriteScript *script, FeriteVariable **list )
 * @brief Delete a parameter list, and destroy any disposable variables
 * @param FeriteScript *script The current script
 * @param FeriteVariable **list	The list to be deleted
 */
void ferite_delete_parameter_list( FeriteScript *script, FeriteVariable **list )
{
	int i = 0;
	int size = ferite_get_parameter_count( list );

	FE_ENTER_FUNCTION;
	while( list[i] != NULL && i < size )
	{
		if( list[i] )
		{
			UNLOCK_VARIABLE(list[i]);
			if( FE_VAR_IS_DISPOSABLE( list[i] ) )
			  ferite_variable_destroy( script, PTR2VAR(list[i]) );
		}
		i++;
	}
	ffree( list );
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_create_parameter_list_from_data
 * @declaration FeriteVariable **ferite_create_parameter_list_from_data( FeriteScript *script, char *format, ... )
 * @brief This function is used to make creating a parameter list very easy.
 * @param script The current script
 * @param format The signiture for the parameters
 * @description
 * This function makes the whole process of creating a parameter list very easy. It allows
 * you to create a list from the actual data and not have to worry about the finer details of
 * ferite variables. It returns a parameter list.<nl/>
 * <nl/>
 * e.g.:<nl/>
 * ferite_create_parameter_list_from_data( script, "nns", 2.3, 2, "Jello" );<nl/>
 * <nl/>
 * The formats are as follows:<nl/>
 *	n = number<nl/>
 *	s = string<nl/>
 *	o = object<nl/>
 *	a = array
 * @return The list that is created.
 */
FeriteVariable **ferite_create_parameter_list_from_data( FeriteScript *script, char *format, ... )
{
	FeriteVariable **retval = NULL;
	FeriteVariable *var = NULL;
	va_list		  ap;
	int			  i = 0;

	retval = fmalloc( sizeof( FeriteVariable * ) * (strlen(format) + 3) );
	memset( retval, '\0', sizeof( FeriteVariable * ) * (strlen(format) + 3) );

	va_start( ap, format );
	for( i = 0; i < (signed)strlen(format); i++ )
	{
		switch( format[i] )
		{
		  case 'n':
			var = ferite_create_number_double_variable( script, "list_from_data-number", va_arg( ap, double ), FE_STATIC );
			break;
		  case 'l':
			var = ferite_create_number_long_variable( script, "list_from_data-number", va_arg( ap, long ), FE_STATIC );
			break;
		  case 's':
			var = ferite_create_string_variable( script, "list_from_data-string", va_arg( ap, FeriteString * ), FE_STATIC );
			break;
		  case 'c':
 			  var = ferite_create_string_variable_from_ptr( script, "list_from_data-string", va_arg( ap, char * ), 0, FE_CHARSET_DEFAULT, FE_STATIC );
			  break;
		  case 'o':
			  var = ferite_create_object_variable( script, "list_from_data-object", FE_STATIC );
			  VAO(var) = va_arg( ap, FeriteObject * );
			  FINCREF(VAO(var));
			break;
		  case 'a':
			  var = ferite_create_uarray_variable( script, "list_from_data-array", 0, FE_STATIC );
			  (ferite_array->destroy)( script, VAUA(var) );
			  VAUA(var) = (ferite_array->duplicate)( script, va_arg( ap, FeriteUnifiedArray *) );
			  break;
		}
		MARK_VARIABLE_AS_DISPOSABLE( var );
		retval[i] = var;
	}
	va_end( ap );
	return retval;
}

/**
 * @function ferite_call_function
 * @declaration FeriteVariable *ferite_call_function( FeriteScript *script, FeriteFunction *function, FeriteVariable **params )
 * @brief This will call any function from it's function pointer and a parameter list
 * @param FeriteScript *script The current script
 * @param FeriteFunction *function The function to be called
 * @param FeriteVariable **params The parameter list to be passed to the function
 * @description This function will work on either an internal or native function and will happily choose the correct function
 *			  if it happens to be overloaded.
 */
FeriteVariable *ferite_call_function( FeriteScript *script, void *container, FeriteObject *block, FeriteFunction *orig_function, FeriteVariable **params )
{
	FeriteVariable *retval = NULL;
	FeriteFunction *function = NULL;
	FeriteVariable **plist = NULL;
	int script_exec_state = 0;
	
	FE_ENTER_FUNCTION;
	
	if( orig_function != NULL )
	{
		script_exec_state = script->is_executing;
		script->is_executing = FE_TRUE;
		
		plist = params;
		if( params == NULL )
		{
			plist = ferite_create_parameter_list( script,1 );
			plist[0] = NULL;
		}
		
		for( function = orig_function; function != NULL; function = function->next )
		{
			if( ferite_check_params( script, plist, function ) == 1 )
			{
				LOCK_VARIABLE( function );
				if( function->type == FNC_IS_EXTRL )
				{
					/* This is used to hook up the execute record */
					EXTERNAL_ENTER( function );
					if( function->fncPtr != NULL )
						retval = (function->fncPtr)( script, container, block, function, plist );
					else
						retval = ferite_create_void_variable( script, "error...", FE_STATIC );
					EXTERNAL_EXIT();
				}
				else
				{
					retval = ferite_script_function_execute( script, container, block, function, plist );
					if( script->error_state == FE_ERROR_THROWN )
						retval = ferite_create_void_variable( script, "error...", FE_STATIC );
				}
				UNLOCK_VARIABLE( function );
				break;
			}
		}
		if( function == NULL )
			ferite_error( script, 0, "Incorrect parameters for function %s\n", orig_function->name );
		
		if( params == NULL )
			ffree( plist );
		
		script->is_executing = script_exec_state;
	}
	else
		ferite_error( script, 0, "Unable to execute NULL function\n" );
	
	FE_LEAVE_FUNCTION( retval );
}

/**
 * @end
 */
