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

#include "ferite.h"
#include "aphex.h"

#ifdef VCWIN32
# include "snprintf.h" /* This is so that we have somethings */
#endif

/**
 * @group Variables
 * @description A set of functions to allow for variables to be created and played with. Each of the variable
 *              allocation routines take an argument called alloc, this allows for reduced memory consumption
 *              regarding the names of variables. For a variable name to be static it must be either externally
 *              allocated or a compiled in constant.<nl/>
 *              <nl/>
 *              e.g. "This is static text";<nl/>char *foo = strdup( "This is not (well the result of strdup)" );<nl/>
 *              <nl/>
 *              To make sure that the variable name is allocated simply pass FE_ALLOC, if you want the name to
 *              be set to that of the pointer passed, give the function FE_STATIC.<nl/>
 */

FeriteVariable *ferite_variable_alloc(FeriteScript *script, char *name, int alloc)
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    if( script && script->vars && script->vars->stack_ptr )
		ptr = ferite_stack_pop( script, script->vars );
    else
		ptr = fmalloc( sizeof( FeriteVariable ) );

    FUD(("Allocated Variable %p\n", ptr));
	ptr->vname = NULL;
    ptr->flags = 0;
    if( name ) {
		if( alloc == FE_ALLOC ) {
			ptr->vname = fstrdup( name );
			UNMARK_VARIABLENAME_AS_STATIC( ptr );
		}
    	else {
        	ptr->vname = name;
        	MARK_VARIABLENAME_AS_STATIC( ptr );
    	}
	}
    ptr->type = 0;
    ptr->data.lval = 0;
    ptr->index = -1;
    ptr->lock = NULL;
    ptr->accessors = NULL;
    ptr->refcount = 1;
    ptr->state = FE_ITEM_IS_PUBLIC;
	ptr->subtype = NULL;
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_variable_destroy
 * @declaration void ferite_variable_destroy( FeriteScript *script, FeriteVariable *var )
 * @brief Destroy a variable
 * @param FeriteScript *script The script the variable belongs to
 * @param FeriteVariable *var The variable to destroy
 * @description If a variable has got multiple references, the reference will be decremented. Once the reference 
                count reaches 0 (zero) the memory the variable uses will either be handed back to the variable 
                cache or the memory pool.
 */
void ferite_variable_destroy( FeriteScript *script, FeriteVariable *var )
{
    FE_ENTER_FUNCTION;
    if( var != NULL )
    {
		FDECREF(var);
        if( var->refcount > 0 )
        {
            if( F_VAR_TYPE(var) == F_VAR_OBJ && VAO(var) != NULL ) {
				FDECREF(VAO(var));
			}
            FE_LEAVE_FUNCTION(NOWT);
        }
        
        if( var->accessors != NULL )
        {
            FUD(( "got accessors\n" ));
            if( var->accessors->cleanup != NULL && var->accessors->owner )
            {
                FUD(( "calling var->cleanup()\n" ));
                (var->accessors->cleanup)( script, var->accessors->odata );
                var->accessors->odata = NULL;
            }
            ffree( var->accessors );
        }
        FUD(("Freeing %s [%p](%s)\n", var->vname, var, ferite_variable_id_to_str( script, F_VAR_TYPE(var) ) ));
        switch( F_VAR_TYPE(var) )
        {
          case  F_VAR_STR:
            FUD(("It's a string. Freeing\n"));
            ferite_str_destroy( script, VAS(var) );
            FUD(("Variable String Data Free'd\n"));
            break;
          case F_VAR_OBJ:
            if( VAO(var) != NULL && !ferite_script_being_deleted( script ) )
            {
              /* we only care about reference counting if we are not being deleted */
                FUD(("Reducing reference count for object variable"));
				FDECREF(VAO(var));
                FUD((" :: %d\n", VAO(var)->refcount ));
            }
            break;
          case  F_VAR_UARRAY:
            ferite_uarray_destroy( script, VAUA(var));
            break;
        }
#ifdef THREAD_SAFE
        if( var->lock != NULL )
        {
            aphex_mutex_destroy( var->lock );
            var->lock = NULL;
        }
#endif
        if( !FE_VAR_NAME_IS_STATIC( var ) && var->vname != NULL )
          ffree( var->vname );

        FUD(("Variable Name Free'd\n"));

        if( script && script->vars && (script->vars->stack_ptr < (script->vars->size-1)) )
			ferite_stack_push( script, script->vars, var );
        else
			ffree( var );

    }
    FUD(("Variable Free'd\n"));
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_get_variable_ref
 * @declaration FeriteVariable *ferite_get_variable_ref( FeriteScript *script, FeriteVariable *variable )
 * @brief Obtain a reference to an existing variable.
 * @param FeriteScript *script The script context
 * @param FeriteVariable *variable The variable to obtain a reference count to
 * @return The same variable pointed to by 'variable'
 */
FeriteVariable *ferite_get_variable_ref( FeriteScript *script, FeriteVariable *variable )
{
    FE_ENTER_FUNCTION;
    if( variable != NULL )
    {
        if( F_VAR_TYPE(variable) == F_VAR_OBJ && VAO(variable) != NULL ) {
			FINCREF(VAO(variable));
		}
		FINCREF(variable);
    }
    FE_LEAVE_FUNCTION( variable );
}

/**
 * @function ferite_duplicate_variable
 * @declaration FeriteVariable *ferite_duplicate_variable( FeriteScript *script, FeriteVariable *var, void *extra )
 * @brief Duplicate a variable
 * @param FeriteScript *script The script the variable belongs to
 * @param FeriteVariable *var The variable to duplicate
 * @param void *extra Ignore this, just pass NULL to the function
 * @return A duplicate of the variable
 */
FeriteVariable *ferite_duplicate_variable( FeriteScript *script, FeriteVariable *var, void *extra )
{
    FeriteVariable *ptr = NULL;
    int alloc;
    FE_ENTER_FUNCTION;
    FE_ASSERT( var != NULL );
    alloc = (FE_VAR_NAME_IS_STATIC( var )) ? FE_STATIC : FE_ALLOC;
    switch( F_VAR_TYPE(var) )
    {
      case F_VAR_VOID:
        ptr = ferite_create_void_variable( script, var->vname, alloc );
        break;
	  case F_VAR_BOOL:
		ptr = ferite_create_boolean_variable( script, var->vname, VAB(var), alloc );
		break;
      case F_VAR_LONG:
        ptr = ferite_create_number_long_variable( script, var->vname, VAI(var), alloc );
        break;
      case F_VAR_DOUBLE:
        ptr = ferite_create_number_double_variable( script, var->vname, VAF(var), alloc );
        break;
      case F_VAR_STR:
        ptr = ferite_create_string_variable( script, var->vname, VAS(var), alloc );
        break;
      case F_VAR_OBJ:
        ptr = ferite_create_object_variable_with_data( script, var->vname, VAO(var), alloc );
        break;
      case F_VAR_UARRAY:
        ptr = ferite_create_uarray_variable( script, var->vname, VAUA(var)->size, alloc );
        ferite_uarray_destroy( script, VAUA(ptr) );
        VAUA(ptr) = ferite_uarray_dup( script, VAUA(var) );
        break;
      case F_VAR_NS:
        ptr = ferite_create_namespace_variable( script, var->vname, VAN(var), alloc );
        break;
      case F_VAR_CLASS:
        ptr = ferite_create_class_variable( script, var->vname, VAC(var), alloc );
        break;
      default:
        ferite_error( script, 0, "Can not duplicate variable of type %d",  F_VAR_TYPE(var) );
        FE_LEAVE_FUNCTION( ptr );
    }
    ptr->flags = var->flags;
    if( FE_VAR_IS_DISPOSABLE(ptr) )
    {
        UNMARK_VARIABLE_AS_DISPOSABLE( ptr );
    }
    else
    {
        if( var->accessors != NULL )
        {
            ptr->accessors = fmalloc( sizeof(FeriteVariableAccessors) );
            ptr->accessors->get = var->accessors->get;
            ptr->accessors->set = var->accessors->set;
            ptr->accessors->cleanup = var->accessors->cleanup;
            ptr->accessors->odata = var->accessors->odata;
            ptr->accessors->owner = FE_FALSE;
        }
    }

    ptr->state = var->state;
	ptr->subtype = var->subtype;
	
    if( FE_VAR_IS_COMPILED(ptr) ) {
		UNMARK_VARIABLE_AS_COMPILED( ptr );
		UNMARK_VARIABLE_AS_FINALSET( ptr );
	}
    ptr->index = var->index;
#ifdef THREAD_SAFE
    if( var->lock != NULL )
		ptr->lock = (void*)aphex_mutex_recursive_create();
#endif

	FUD(("Duplicating variable %p[%s] into variable %p[%s]\n", var, var->vname, ptr, ptr->vname));
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_string_variable
 * @declaration FeriteVariable *ferite_create_string_variable( FeriteScript *script, char *name, FeriteString *data, int alloc )
 * @brief Allocate a FeriteVariable and set it up to be a string from a FeriteString
 * @param FeriteScript *script The script
 * @param char *name The name of the variable
 * @param FeriteString *data The data to set the default value of the variable to
 * @param int alloc Whether or not to set the variable's name as static or allocated
 * @return Returns a newly created string variable
 * @description As ferite's string variable is based upon FeriteString it can happily handle binary data
 */
FeriteVariable *ferite_create_string_variable( FeriteScript *script, char *name, FeriteString *data, int alloc )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    ptr = ferite_variable_alloc( script, name, alloc );
    F_VAR_TYPE(ptr) = F_VAR_STR;
    VAS(ptr) = ferite_str_dup( script, data );
	ptr->subtype = ferite_subtype_link( script, "S" );
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_string_variable_from_ptr
 * @declaration FeriteVariable *ferite_create_string_variable( FeriteScript *script, char *name, char *data, int length, int encoding, int alloc )
 * @brief Create a FeriteVariable and set it up to be a string from a pointer and length
 * @param FeriteScript *script The script
 * @param char *name The name of the variable
 * @param char *data The data to set the default value of the variable to
 * @param int length The amount of data that exists - required to be greater than 0 for binary data. For a null terminated string, 0 will cause ferite to work out the length of the data.
 * @param int encoding The method that was used to encode the string, if you are unsure use FE_CHARSET_DEFAULT
 * @param int alloc Whether or not to set the variable's name as static or allocated
 * @return Returns a newly created string variable
 */
FeriteVariable *ferite_create_string_variable_from_ptr( FeriteScript *script, char *name, char *data, size_t length, int encoding, int alloc )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    ptr = ferite_variable_alloc( script, name, alloc );
    F_VAR_TYPE(ptr) = F_VAR_STR;
    VAS(ptr) = ferite_str_new( script, data, length, encoding );
	ptr->subtype = ferite_subtype_link( script, "S" );
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_number_long_variable
 * @declaration FeriteVariable *ferite_create_number_long_variable( FeriteScript *script, char *name, long data, int alloc )
 * @brief Create a FeriteVariable and set it up to be a number variable of type long
 * @param FeriteScript *script The script
 * @param char *name The name of the variable
 * @param long data The initial value
 * @param alloc Whether or not to set the variable's name as static or allocated
 * @return Returns a newly created number variable
 */
FeriteVariable *ferite_create_boolean_variable( FeriteScript *script, char *name, long data, int alloc )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    ptr = ferite_variable_alloc( script, name, alloc );
    F_VAR_TYPE(ptr) = F_VAR_BOOL;
    VAB( ptr ) = data;
	ptr->subtype = ferite_subtype_link( script, "B" );
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_number_long_variable
 * @declaration FeriteVariable *ferite_create_number_long_variable( FeriteScript *script, char *name, long data, int alloc )
 * @brief Create a FeriteVariable and set it up to be a number variable of type long
 * @param FeriteScript *script The script
 * @param char *name The name of the variable
 * @param long data The initial value
 * @param alloc Whether or not to set the variable's name as static or allocated
 * @return Returns a newly created number variable
 */
FeriteVariable *ferite_create_number_long_variable( FeriteScript *script, char *name, long data, int alloc )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    ptr = ferite_variable_alloc( script, name, alloc );
    F_VAR_TYPE(ptr) = F_VAR_LONG;
    VAI( ptr ) = data;
	ptr->subtype = ferite_subtype_link( script, "L" );
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_number_double_variable
 * @declaration FeriteVariable *ferite_create_number_double_variable( FeriteScript *script, char *name, double data, int alloc )
 * @brief Create a FeriteVariable and set it up to be a number variable of type double
 * @param FeriteScript *script The script
 * @param char *name The name of the variable
 * @param double data The initial value
 * @param alloc Whether or not to set the variable's name as static or allocated
 * @return Returns a newly created number variable
 */
FeriteVariable *ferite_create_number_double_variable( FeriteScript *script, char *name, double data, int alloc )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    ptr = ferite_variable_alloc( script, name, alloc );
    F_VAR_TYPE(ptr) = F_VAR_DOUBLE;
    VAF( ptr ) = data;
	ptr->subtype = ferite_subtype_link( script, "D" );
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_object_variable
 * @declaration FeriteVariable *ferite_create_object_variable( FeriteScript *script, char *name, int alloc )
 * @brief Create a FeriteVariable and set it up to be an object variable pointing to 'null'
 * @param FeriteScript *script The script
 * @param char *name The name of the variable
 * @param alloc Whether or not to set the variable's name as static or allocated
 * @return Returns a newly created object variable
 */
FeriteVariable *ferite_create_object_variable( FeriteScript *script, char *name, int alloc )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    ptr = ferite_variable_alloc( script, name, alloc );
    F_VAR_TYPE(ptr) = F_VAR_OBJ;
    VAO(ptr) = NULL;
	ptr->subtype = ferite_subtype_link( script, "O" );
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_object_variable_with_data
 * @declaration FeriteVariable *ferite_create_object_variable_with_data( FeriteScript *script, char *name, FeriteObject *o, int alloc )
 * @brief Create a FeriteVariable and set it up to be an object variable pointing to 'o'
 * @param FeriteScript *script The script
 * @param char *name The name of the variable
 * @param alloc Whether or not to set the variable's name as static or allocated
 * @return Returns a newly created object variable referencing the varuabke that has been passed in.
 * @description This function can be made to mimic ferite_create_object_variable by passing NULL in for 'o'
 */
FeriteVariable *ferite_create_object_variable_with_data( FeriteScript *script, char *name, FeriteObject *o, int alloc )
{
    FeriteVariable *var = NULL;

    FE_ENTER_FUNCTION;
    var = ferite_create_object_variable( script, name, alloc );
    VAO(var) = o;
    if( VAO(var) != NULL ) {
		FINCREF(VAO(var));
	}
    FE_LEAVE_FUNCTION( var );
}

/**
 * @function ferite_create_uarray_variable
 * @declaration FeriteVariable *ferite_create_uarray_variable( FeriteScript *script, char *name, int size, int alloc )
 * @brief Create a FeriteVariable and set it up to be an array
 * @param FeriteScript *script The script
 * @param char *name The name of the variable
 * @param int size The intial size of the array [This is currently ignored and is taken as a 'hint']
 * @param int alloc Whether or not to set the variable's name as static or allocated
 * @return Returns a newly created array variable
 */
FeriteVariable *ferite_create_uarray_variable( FeriteScript *script, char *name, int size, int alloc )
{
    FeriteVariable *ptr;
    int rsize = 0;

    FE_ENTER_FUNCTION;
    rsize = ( size == 0 ) ? FE_ARRAY_DEFAULT_SIZE  : size;
    ptr = ferite_variable_alloc(script, name, alloc);
    F_VAR_TYPE(ptr) = F_VAR_UARRAY;
    VAUA(ptr) = ferite_uarray_create( script );
 	ptr->subtype = ferite_subtype_link( script, "A" );
   FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_void_variable
 * @declaration FeriteVariable *ferite_create_void_variable( FeriteScript *script, char *name, int alloc )
 * @brief Create a FeriteVariable and set it up to be an void variable
 * @param FeriteScript *script The script
 * @param char *name The name of the variable
 * @param alloc Whether or not to set the variable's name as static or allocated
 * @return Returns a newly created void variable
 */
FeriteVariable *ferite_create_void_variable( FeriteScript *script, char *name, int alloc )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    ptr = ferite_variable_alloc(script, name, alloc);
    F_VAR_TYPE(ptr) = F_VAR_VOID;
	ptr->subtype = ferite_subtype_link( script, "V" );
    FE_LEAVE_FUNCTION( ptr );
}


/**
 * @function ferite_create_class_variable
 * @declaration FeriteVariable *ferite_create_class_variable( FeriteScript *script, char *name, FeriteClass *klass, int alloc )
 * @brief Create a variable that references an existing class within the ferite script
 * @param FeriteScript *script The script context
 * @param char *name The name of the variable
 * @param FeriteClass *klass A pointer to the class to reference
 * @param int alloc Whether or not to allocate the variable name
 * @return A variable that references the supplied class
 */
FeriteVariable *ferite_create_class_variable( FeriteScript *script, char *name, FeriteClass *klass, int alloc )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    ptr = ferite_variable_alloc(script, name, alloc);
    F_VAR_TYPE(ptr) = F_VAR_CLASS;
    ptr->data.pval = klass;
	ptr->subtype = ferite_subtype_link( script, "C" );
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_namespace_variable
 * @declaration FeriteVariable *ferite_create_namespace_variable( FeriteScript *script, char *name, FeriteNamespace *ns, int alloc )
 * @brief Create a variable that references an existing namespace within the ferite script
 * @param FeriteScript *script The script context
 * @param char *name The name of the variable
 * @param FeriteNamespace *ns A pointer to the namespace to reference
 * @param int alloc Whether or not to allocate the variable name
 * @return A variable that references the supplied namespace
 */
FeriteVariable *ferite_create_namespace_variable( FeriteScript *script, char *name, FeriteNamespace *ns, int alloc )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    ptr = ferite_variable_alloc(script, name, alloc);
    F_VAR_TYPE(ptr) = F_VAR_NS;
    ptr->data.pval = ns;
	ptr->subtype = ferite_subtype_link( script, "N" );
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_variable_id_to_str
 * @declaration char *ferite_variable_id_to_str( FeriteScript *script, int variable )
 * @brief Get the name of a variable type id
 * @param FeriteScript *script The script
 * @param int variable The type of the variable
 * @return A string, e.g. void, string or number
 */
char *ferite_variable_id_to_str( FeriteScript *script, int variable )
{
    FE_ENTER_FUNCTION;
    switch( variable )
    {
      case F_VAR_VOID:
        FE_LEAVE_FUNCTION( "void" );
      case F_VAR_STR:
        FE_LEAVE_FUNCTION( "string" );
      case F_VAR_LONG:
        FE_LEAVE_FUNCTION( "number" );
      case F_VAR_DOUBLE:
        FE_LEAVE_FUNCTION( "number" );
      case F_VAR_OBJ:
        FE_LEAVE_FUNCTION( "object" );
      case F_VAR_UARRAY:
        FE_LEAVE_FUNCTION( "array" );
      case F_VAR_NS:
        FE_LEAVE_FUNCTION( "namespace" );
      case F_VAR_CLASS:
        FE_LEAVE_FUNCTION( "class" );
	  case F_VAR_BOOL:
		FE_LEAVE_FUNCTION( "boolean" );
    }
    ferite_error( script, 0, "ERROR: Trying to id variable of type %d - UNKNOWN\n", variable );
    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_variable_is_false
 * @declaration int ferite_variable_is_false( FeriteScript *script, FeriteVariable *var )
 * @brief Find out whether a given FeriteVariable is false
 * @param FeriteScript *script The script
 * @param FeriteVariable *var The variable to check
 * @return FE_TRUE if the variable is false, FE_FALSE otherwise
 */
int ferite_variable_is_false( FeriteScript *script, FeriteVariable *var )
{
    int retval = FE_FALSE;
    
    FE_ENTER_FUNCTION;
    if( var != NULL )
    {
        switch( F_VAR_TYPE(var) )
        {
            case F_VAR_VOID:
                retval = FE_TRUE;
                break;
            case F_VAR_LONG:
                if( VAI(var) == 0 )
                    retval = FE_TRUE;
                break;
            case F_VAR_DOUBLE:
                if( VAF(var) < 0.000001 && VAF(var) > -0.000001 )
                    retval = FE_TRUE;
                break;
            case F_VAR_STR:
                if( FE_STRLEN(var) == 0 )
                    retval = FE_TRUE;
                break;
            case F_VAR_OBJ:
                if( VAO(var) == NULL )
                    retval = FE_TRUE;
                break;
            case F_VAR_UARRAY:
                if( VAUA(var)->size == 0 )
                    retval = FE_TRUE;
                break;
			case F_VAR_BOOL:
				if( !VAB(var) )
					retval = FE_TRUE;
				break;
			case F_VAR_CLASS:
				if( !VAC(var) )
					retval = FE_TRUE;
				break;
			case F_VAR_NS:
				if( !VAN(var) )
					retval = FE_TRUE;
				break;
            default:
                ferite_error( script, 0,  "Can't tell whether type %d has a truth value\n", F_VAR_TYPE(var) );
                retval = FE_TRUE;
        }
    }
    else
        retval = FE_TRUE;
    
    FE_LEAVE_FUNCTION( retval );
}

/**
 * @function ferite_number_as_long
 * @declaration long ferite_number_as_long( FeriteScript *script, FeriteVariable *var )
 * @brief Provide, for a given variable the long representation.
 * @param FeriteScript *script The script context
 * @param FeriteVariable *var The variable to convert
 * @return The value of the variable
 * @warning It is important to check script->error_state for ERROR_THROWN. This happens when the variable is not a 
            number type.
 */
long ferite_number_as_long( FeriteScript *script, FeriteVariable *var )
{
    FE_ENTER_FUNCTION;
    FE_ASSERT( var != NULL );
    if( F_VAR_TYPE(var) == F_VAR_LONG )
    {
        FE_LEAVE_FUNCTION( VAI(var) );
    }
    else if( F_VAR_TYPE(var) == F_VAR_DOUBLE )
    {
        FE_LEAVE_FUNCTION( (long)VAF(var) );
    }
	else if( F_VAR_TYPE(var) == F_VAR_BOOL )
	{
		FE_LEAVE_FUNCTION( VAB(var) );
	}
    else
      ferite_error( script, 0, "Can't convert variable to long\n" );
    FE_LEAVE_FUNCTION(-1);
}

/**
* @function ferite_number_as_double
 * @declaration double ferite_number_as_double( FeriteScript *script, FeriteVariable *var )
 * @brief Provide, for a given variable the double representation.
 * @param FeriteScript *script The script context
 * @param FeriteVariable *var The variable to convert
 * @return The value of the variable
 * @warning It is important to check script->error_state for ERROR_THROWN. This happens when the variable is not a 
            number type.
 */
double ferite_number_as_double( FeriteScript *script, FeriteVariable *var )
{
    FE_ENTER_FUNCTION;
    FE_ASSERT( var != NULL );
    if( F_VAR_TYPE(var) == F_VAR_LONG )
    {
        FE_LEAVE_FUNCTION( (double)VAI(var) );
    }
    else if( F_VAR_TYPE(var) == F_VAR_DOUBLE )
    {
        FE_LEAVE_FUNCTION( VAF(var) );
    }
	else if( F_VAR_TYPE(var) == F_VAR_BOOL )
	{
		FE_LEAVE_FUNCTION( (double)VAB(var) );
	}
    else
      ferite_error( script, 0, "Can't convert variable to double\n" );
    FE_LEAVE_FUNCTION(-1);
}

/**
 * @function ferite_variable_to_str
 * @declaration FeriteString *ferite_variable_to_str( FeriteScript *script, FeriteVariable *var, int quote )
 * @brief Get a ferite string representation of a variable
 * @param FeriteScript *script The script
 * @param FeriteVariable *var The variable to convert
 * @param int quote Whether or not to quote the string format. This only applies to string variables.
 * @return A FeriteString
 * @description This function works as would be aspected as for all variables but with special behavior for
 *              objects. If there is a .toString() method in the object it will be called with it's results
 *              used as the return value for this function, if the method does not exist then the return will
 *              be a string including the real address of the object along with a statement regarding the
 *              toString method.
 */
FeriteString *ferite_variable_to_str( FeriteScript *script, FeriteVariable *var, int quote )
{
   FeriteBuffer *buf = NULL;
   FeriteString *str = NULL;
   char s[80];
   int len;

   FE_ENTER_FUNCTION;
   if( var != NULL )
   {
	   memset( s, '\0', 80 );
	   switch( F_VAR_TYPE(var) )
	   {
		   case F_VAR_LONG:
			   len = snprintf( s, 80, "%ld", VAI(var));
			   str = ferite_str_new( script, s, len, FE_CHARSET_DEFAULT );
			   break;
		   case F_VAR_DOUBLE:
			   len = snprintf( s, 80, "%f", VAF(var));
			   str = ferite_str_new( script, s, len, FE_CHARSET_DEFAULT );
			   break;
		   case F_VAR_BOOL:
			   len = snprintf( s, 80, "%s", (VAB(var) == FE_FALSE ? "false" : "true") );
			   str = ferite_str_new( script, s, len, FE_CHARSET_DEFAULT );				
			   break;
		   case  F_VAR_STR:
			   if(quote)
			   {
				   FeriteString *escaped_str = ferite_str_escape( script, VAS(var) );
				   buf = ferite_buffer_new(script, FE_DEFAULT_BUFFER_SIZE);
				   ferite_buffer_printf(script, buf,"\"%.*s\"", escaped_str->length,escaped_str->data);
				   str = ferite_buffer_to_str(script, buf );
				   ferite_str_destroy( script, escaped_str );
			   }
			   else
				   str = ferite_str_dup( script, VAS(var) );
			   break;
		   case F_VAR_UARRAY:
			   str = ferite_uarray_to_str(script, VAUA(var));
			   break;
		   case F_VAR_OBJ:
		   {
			   FeriteFunction *toString = NULL;
			   FeriteVariable *retval = NULL;
			   FeriteVariable **params = NULL;
			   
			   buf = ferite_buffer_new(script, FE_DEFAULT_BUFFER_SIZE);
			   if( VAO(var) != NULL )
			   {
				   /* find a .toString() method within the object */
				   if( (toString = ferite_object_get_function_for_params( script, VAO(var), "toString", NULL )) != NULL )
				   {
					   params = ferite_create_parameter_list(script,2);
					   retval = ferite_call_function( script, VAO(var), NULL, toString, params );
					   /* get the return value */
					   if( retval != NULL )
					   {
						   /* it should be a string */
						   if( F_VAR_TYPE(retval) == F_VAR_STR )
							   ferite_buffer_printf( script, buf, "%.*s", FE_STRLEN(retval), FE_STR2PTR(retval) );
						   else
						   { /* otherwise, lets convert it, but avoid recursion [in case .toString() returns self] */
							   if( F_VAR_TYPE(retval) != F_VAR_OBJ || (F_VAR_TYPE(retval) == F_VAR_OBJ && VAO(retval) != VAO(var)) )
							   {
								   str = ferite_variable_to_str( script, retval, FE_FALSE );
								   ferite_buffer_printf( script, buf, "%.*s", str->length, str->data );
							   }
							   else
							   {
								   ferite_buffer_printf( script, buf, "<object(%s) '%s' 0x%x>", (VAO(retval) != NULL ? VAO(retval)->name : ""),retval->vname,VAO(retval));
							   }
						   }
						   ferite_variable_destroy( script, retval );
					   }
					   else
					   {
						   ferite_buffer_printf( script, buf,"<object(%s) '%s' 0x%x [invalid .toString()]>", (VAO(var) != NULL ? VAO(var)->name : ""),var->vname,VAO(var));
					   }
					   ferite_delete_parameter_list( script, params );
				   }
				   else
				   {
					   ferite_buffer_printf( script, buf,"<object(%s) '%s' 0x%x [no .toString() defined]>", (VAO(var) != NULL ? VAO(var)->name : ""),var->vname,VAO(var));
				   }
			   }
			   else
			   {
				   ferite_buffer_printf( script, buf, "null" );
			   }
			   str = ferite_buffer_to_str( script, buf );
			   break;
		   }
		   case F_VAR_CLASS:
		   {
			   FeriteClass *klass = VAP(var);
			   buf = ferite_buffer_new(script,FE_DEFAULT_BUFFER_SIZE);
			   ferite_buffer_printf( script, buf, "<class '%s'%s%s>", klass->name, (klass->parent != NULL?" extends ":""), (klass->parent != NULL?klass->parent->name:"") );
			   str = ferite_buffer_to_str( script, buf );
			   break;
		   }
		   case F_VAR_NS:
		   {
			   FeriteNamespace *ns = VAP(var);
			   buf = ferite_buffer_new(script, FE_DEFAULT_BUFFER_SIZE);
			   ferite_buffer_printf( script, buf, "<namespace '%s'>", ns->name );
			   str = ferite_buffer_to_str( script, buf );
			   break;
		   }
		   case F_VAR_VOID:
			   str = ferite_str_new( script, "", strlen(""), FE_CHARSET_DEFAULT );
			   break;
		   default:
			   str = ferite_str_new( script, "(not a variable)", strlen("(not a variable)"), FE_CHARSET_DEFAULT );
	   }
   }
	else
	{
	   ferite_error( script, 0, "NULL Variable, bad.\n" );
	   str = ferite_str_new( script, "(no variable)", strlen("(no variable)"), FE_CHARSET_DEFAULT );
	}

   if( buf != NULL )
     ferite_buffer_delete(script, buf);
   FE_LEAVE_FUNCTION( str );
}

/**
 * @function ferite_set_variable_name
 * @declaration void ferite_set_variable_name( FeriteScript *script, FeriteVariable *var, char *newname )
 * @brief Mark the variable name as allocated and set it
 * @param FeriteScript *script The script context
 * @param FeriteVariable *var The variable to apply the name to
 * @param char *newname The new name to use
 */
void ferite_set_variable_name( FeriteScript *script, FeriteVariable *var, char *newname )
{
    FE_ENTER_FUNCTION;
    if( !FE_VAR_NAME_IS_STATIC( var ) )
    {
        ffree( var->vname );
    }
    else
    {
        UNMARK_VARIABLENAME_AS_STATIC( var );
    }
    var->vname = fstrdup( newname );
    FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_set_static_variable_name
 * @declaration void ferite_set_static_variable_name( FeriteScript *script, FeriteVariable *var, char *newname )
 * @brief Mark the variable name as not allocated and change it to the new name
 * @param FeriteScript *script The script context
 * @param FeriteVariable *var The variable to apply the name to
 * @param char *newname The new name to use
 */
void ferite_set_static_variable_name( FeriteScript *script, FeriteVariable *var, char *newname )
{
    FE_ENTER_FUNCTION;
    if( !FE_VAR_NAME_IS_STATIC( var ) )
    {
        ffree( var->vname );
        MARK_VARIABLENAME_AS_STATIC( var );
    }
    var->vname = newname;
    FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_variable_type_to_char
 * @declaration char ferite_variable_type_to_char( FeriteScript *script, int type )
 * @brief For a given variable type, provide the first character of the variable's type.
 * @param FeriteScript *script The script context
 * @param int type The variable type
 * @return A character describing the type.
 */
char ferite_variable_type_to_char( FeriteScript *script, int type )
{
    char c = ' ';
    
    FE_ENTER_FUNCTION;
    switch( type )
    {
		case F_VAR_BOOL:
			c = 'b';
			break;
        case F_VAR_LONG:
        case F_VAR_DOUBLE:
            c = 'n';
            break;
        case F_VAR_STR:
            c = 's';
            break;
        case F_VAR_OBJ:
            c = 'o';
            break;
        case F_VAR_UARRAY:
            c = 'a';
            break;
        case F_VAR_CLASS:
        case F_VAR_NS:
        case F_VAR_VOID:
            c = 'v';
            break;
    }
    FE_LEAVE_FUNCTION( c );
}


/**
 * @function ferite_types_are_equal
 * @declaration int ferite_types_are_equal( FeriteScript *script, int typea, int typeb )
 * @brief Check to see, in ferite's eye, whether or not the two types are equal and can be assigned to
 * @param FeriteScript *script The script
 * @param int typea The first type
 * @param int typeb The second type
 * @return FE_TRUE if they are considered the same, FE_FALSE otherwise
 */
int ferite_types_are_equal( FeriteScript *script, int typea, int typeb )
{
    FE_ENTER_FUNCTION;

    /* Yep they are the same */
    if( typea == typeb )
    {
        FE_LEAVE_FUNCTION( FE_TRUE );
    }

    /* Check the numbers! */
    if( typea == F_VAR_LONG && typeb == F_VAR_DOUBLE )
    {
        FE_LEAVE_FUNCTION( FE_TRUE );
    }

    if( typea == F_VAR_LONG && typeb == F_VAR_BOOL )
    {
        FE_LEAVE_FUNCTION( FE_TRUE );
    }

    if( typeb == F_VAR_LONG && typea == F_VAR_DOUBLE )
    {
        FE_LEAVE_FUNCTION( FE_TRUE );
    }

    if( typea == F_VAR_VOID )
    {
        FE_LEAVE_FUNCTION( FE_TRUE );
    }

    /* I guess we are SOL */
    FE_LEAVE_FUNCTION( FE_FALSE );
}
int ferite_variable_fast_assign( FeriteScript *script, FeriteVariable *left, FeriteVariable *right ) 
{
	int result = FE_TRUE;
	
	FE_ENTER_FUNCTION;
	if( F_VAR_TYPE(left) == F_VAR_VOID )
		ferite_variable_convert_to_type( script, left, F_VAR_TYPE(right) );

	if( (F_VAR_TYPE(left) != F_VAR_TYPE(right)) &&
		!(FE_VAR_IS_NUMBER(left) && FE_VAR_IS_NUMBER(right)) 
	 ) {
		FE_LEAVE_FUNCTION(FE_FALSE);
	}
	
	left->type = F_VAR_TYPE(right);
	switch( F_VAR_TYPE(right) ) 
	{
	  case F_VAR_VOID: break;
	  case F_VAR_BOOL:
		VAB(left) = (VAB(right) ? FE_TRUE : FE_FALSE);
		break;
      case F_VAR_LONG:
      case F_VAR_DOUBLE:
		if( F_VAR_TYPE(left) == F_VAR_LONG ) 
			VAI(left) = VAI(right);
		else
			VAF(left) = VAF(right);
        break;
      case F_VAR_STR:
		ferite_str_cpy( script, VAS(left), VAS(right) );
        break;
      case F_VAR_OBJ:
		if( VAO(left) ) {
			FDECREFI(VAO(left), left->refcount);
		}
		if( VAO(right) ) {
			 FINCREFI(VAO(right), left->refcount);
		}
		VAO(left) = VAO(right);
        break;
      case F_VAR_UARRAY:
        ferite_uarray_destroy( script, VAUA(left) );
        VAUA(left) = ferite_uarray_dup( script, VAUA(right) );
        break;
      case F_VAR_NS:
		VAN(left) = VAN(right);
        break;
      case F_VAR_CLASS:
		VAC(left) = VAC(right);
        break;
      default:
        result = FE_FALSE;
	}
	FE_LEAVE_FUNCTION(result);
}
int ferite_fast_variable_cmp( FeriteScript *script, FeriteVariable *left, FeriteVariable *right )
{
#define FE_VAR_TEST( TEST ) if( !(TEST) ) { FE_LEAVE_FUNCTION(FE_FALSE); }

	FE_ENTER_FUNCTION;
	/* check their values */	
	if( F_VAR_TYPE(left) != F_VAR_TYPE(right) )
		return FE_FALSE;
	
	switch( F_VAR_TYPE(left) )
	{
		case F_VAR_LONG:
			FE_VAR_TEST( VAI(left) == VAI(right) );
			break;
		case F_VAR_DOUBLE:
			FE_VAR_TEST( VAF(left) == VAF(right) );
			break;
		case F_VAR_STR:
			FE_VAR_TEST( ferite_str_cmp( script, VAS(left), VAS(right) ) == 1 );
			break;
		case F_VAR_OBJ:
			FE_VAR_TEST( VAO(left) == VAO(right) );
			break;
		case F_VAR_UARRAY:
			FE_VAR_TEST( ferite_uarray_cmp( script, VAUA(left), VAUA(right) ) == 1 );
			break;
		case F_VAR_CLASS:
			FE_VAR_TEST( VAC(left) == VAC(right) );
			break;
		case F_VAR_NS:
			FE_VAR_TEST( VAN(left) == VAN(right) );
			break;
		case F_VAR_VOID:
			break;
		default:
			ferite_error( script, 0, "EEEK: unknown type %s in variable comparison!\n", ferite_variable_id_to_str( script, F_VAR_TYPE(left) ) );
			FE_LEAVE_FUNCTION(FE_FALSE);
	}
#undef FE_VAR_TEST
	FE_LEAVE_FUNCTION(FE_TRUE);
}
/**
 * @function ferite_variable_convert_to_type
 * @declaration void ferite_variable_convert_to_type( FeriteScript *script, FeriteVariable *var, int type )
 * @brief Convert a variable to another type
 * @param FeriteScript *script The script
 * @param FeriteVariable *var The variable to convert
 * @param int type The type to convert the variable to
 */
void ferite_variable_convert_to_type( FeriteScript *script, FeriteVariable *var, int type )
{
    FE_ENTER_FUNCTION;

    switch( F_VAR_TYPE(var) )
    {
      case F_VAR_OBJ:
		  if( VAO(var) != NULL ) {
			  FDECREF(VAO(var));
		  }
		  VAO(var) = NULL;
		  break;
      case F_VAR_STR:
		  ferite_str_destroy( script, VAS(var) );
		  break;
      case F_VAR_UARRAY:
		  ferite_uarray_destroy( script, VAUA(var));
		  break;
    }
	
    F_VAR_TYPE(var) = type;
    var->data.lval = 0;
	
	switch( type ) 
	{
		case F_VAR_STR:
			VAS(var) = ferite_str_new( script, "", 0, FE_CHARSET_DEFAULT );
			break;
		case F_VAR_UARRAY:
			VAUA(var) = ferite_uarray_create( script );
			break;
	}
    FE_LEAVE_FUNCTION(NOWT);
}

void ferite_delete_pointer( FeriteScript *script, void *p )
{
    if( p != NULL ) {
        ffree( p );
    }
}

/**
 * @end
 */

/**
 * @group Variable Accessors
 * @description These are used to provide a mechanism for an application to have special
 * access to individual variables allowing for transparent control of data
 * and lazy updating of information. The mechanism provides three callbacks:
 * 'get', 'set', and 'cleanup'. It also provides a means of tagging data to
 * variables in a similar fashion of objects with an ->odata pointer that
 * is not touched by ferite. Please see include/ferite/fstructs.h for the
 * actual structures used.<nl/>
 * <nl/>
 * <b>How They Work</b><nl/>
 * <nl/>
 * <b>Get</b><nl/>
 * <nl/>
 *   When a variable is accessed in ferite either to just read or assign - it
 *   is 'gotten'. This means that in both of the statements below, the variable
 *   'foo' it 'gotten'.<nl/>
 * <nl/>
 * <code>x = bar.foo;<nl/>
 * bar.foo = 2 * x;</code><nl/>
 * <nl/>
 *   When a variable is 'gotten', and it has accessors on it, the 'get' method
 *   will be run on it. This will pass the script and the variable to the method.
 *   The method can then modify the variable as it wants, and then return. It
 *   does not return the variable.<nl/>
 * <nl/>
 *   The get function has the prototype:<nl/>
 * <nl/>
 * <code>void get( FeriteScript *script, FeriteVariable *var )</code>
 * <nl/>
 * <b>Set</b><nl/>
 * <nl/>
 *   When a variable is assigned to [after it has been gotten], it will have the
 *   'set' method called on it, this gets passed the script, the variable and
 *   the variable that is to be assigned to it [known as RHS var]. Note: ferite
 *   will already have updated the variable with the RHS var. This means that the
 *   programmer need only to the minial amount of work. You must not modify the
 *   rhs variable, it is only given to you so that you can do your own special
 *   assignment if you _have_ to. I warn against it greatly.<nl/>
 * <nl/>
 *   The set function has the prototype:<nl/>
 * <nl/>
 * <code>void set( FeriteScript *script, FeriteVariable *lhs, FeriteVariable *rhs )</code>
 * <nl/>
 * <b>Cleanup</b><nl/>
 * <nl/>
 *   The final mechanism is for cleaning up the ->odata information. It simply
 *   calls the cleanup function with the script and the ->odata pointer so it
 *   can be cleaned up.<nl/>
 * <nl/>
 *   The cleanup function has the prototype:<nl/>
 * <nl/>
 * <tab/>void cleanup( FeriteScript *script, void *odata );<nl/>
 * <nl/>
 * You need not register all of these. To create the structures on a variable
 * call the function:<nl/>
 * <nl/>
 * <code>void ferite_create_variable_accessors( FeriteScript *script,
 *                                          FeriteVariable *var,
 *                                          void *get,
 *                                          void *set,
 *                                          void *cleanup,
 *                                          void *odata ) </code>
 * <nl/>
 * Where 'var' is the variable to attach to, 'get', 'set' and 'cleanup' are
 * pointers to the functions, and odata is the initial odata. You can pass the
 * function NULL's to just not have them setup.
 *
 */

/**
 * @function ferite_create_variable_accessors
 * @declaration void ferite_create_variable_accessors( FeriteScript *script, FeriteVariable *var, void *get, void *set, void *cleanup, void *odata )
 * @brief Create the variable accessors
 * @param FeriteScript *script The script
 * @param FeriteVariable *var The variable to add the accessors to it
 * @param void *get A pointer to the get function (can be NULL)
 * @param void *set A pointer to the set function (can be NULL)
 * @param void *cleanup A pointer to the cleanup function (can be NULL)
 * @param void *odata A pointer to some data that can be attached to the variable, ferite does not touch this _AT ALL_
 */
void ferite_create_variable_accessors( FeriteScript *script, FeriteVariable *var, void *get, void *set, void *cleanup, void *odata )
{
    FE_ENTER_FUNCTION;
    FE_ASSERT( var != NULL );
    if( var->accessors == NULL )
      var->accessors = fmalloc( sizeof(FeriteVariableAccessors) );
    var->accessors->get = get;
    var->accessors->set = set;
    var->accessors->cleanup = cleanup;
    var->accessors->odata = odata;
    var->accessors->owner = FE_TRUE;
    FE_LEAVE_FUNCTION(NOWT);

}

/**
 * @end
 */

FeriteVariableSubType *ferite_create_subtype( FeriteScript *script, short type ) {
	FeriteVariableSubType *subtype = fmalloc(sizeof(FeriteVariableSubType));
	FE_ENTER_FUNCTION;
	subtype->name = NULL;
	subtype->type = type;
	subtype->data._subtype = NULL;
	FE_LEAVE_FUNCTION(subtype);
}

FeriteString *ferite_subtype_to_string( FeriteScript *script, FeriteVariableSubType *subtype ) {
	if( subtype->name == NULL ) {
		subtype->name = ferite_str_new( script, NULL, 0, FE_CHARSET_DEFAULT );
		
		switch( subtype->type ) {
			case F_VAR_LONG:   ferite_str_data_cat( script, subtype->name, "L", 1 ); break;
			case F_VAR_DOUBLE: ferite_str_data_cat( script, subtype->name, "D", 1 ); break;
			case F_VAR_STR:    ferite_str_data_cat( script, subtype->name, "S", 1 ); break;
			case F_VAR_VOID:   ferite_str_data_cat( script, subtype->name, "V", 1 ); break;
			case F_VAR_BOOL:   ferite_str_data_cat( script, subtype->name, "B", 1 ); break;
			case F_VAR_OBJ:
				ferite_str_data_cat( script, subtype->name, "O", 1 ); 
				if( subtype->data._class ) {
					char *name = ferite_generate_class_fqn( script, subtype->data._class );
					ferite_str_data_cat( script, subtype->name, ":", 1 );
					ferite_str_data_cat( script, subtype->name, name, strlen(name) );
					ferite_str_data_cat( script, subtype->name, ";", 1 );
					ffree(name);
				}
				break;
			case F_VAR_CLASS:
				ferite_str_data_cat( script, subtype->name, "C", 1 ); 
				if( subtype->data._class ) {
					char *name = ferite_generate_class_fqn( script, subtype->data._class );
					ferite_str_data_cat( script, subtype->name, ":", 1 );
					ferite_str_data_cat( script, subtype->name, name, strlen(name) );
					ferite_str_data_cat( script, subtype->name, ";", 1 );
					ffree(name);
				}
				break;
			case F_VAR_NS:
				ferite_str_data_cat( script, subtype->name, "N", 1 ); 
				if( subtype->data._namespace ) {
					char *name = ferite_generate_namespace_fqn( script, subtype->data._namespace );
					ferite_str_data_cat( script, subtype->name, ":", 1 );
					ferite_str_data_cat( script, subtype->name, name, strlen(name) );
					ferite_str_data_cat( script, subtype->name, ";", 1 );
					ffree(name);
				}
				break;
			case F_VAR_UARRAY:
				ferite_str_data_cat( script, subtype->name, "A", 1 ); 
				if( subtype->data._subtype ) {
					ferite_str_cat( script, subtype->name, ferite_subtype_to_string( script, subtype->data._subtype ) );
				}
				break;
			default:
				ferite_str_data_cat( script, subtype->name, "U", 1 ); break;
		}
	}
	return subtype->name;
}

FeriteVariableSubType *ferite_subtype_link( FeriteScript *script, char *type ) {
	FeriteVariableSubType *subtype = NULL;
	char target_class[1024];
	int length = 0, i = 0;
	
	if( script == NULL )
		return NULL;
	
	if( type == NULL )
		return NULL;
		
	length = strlen(type);
	if( length == 0 )
		return NULL;
		
	subtype = ferite_hamt_get( script, script->types, type );
	
	if( subtype ) {
		return subtype;
	}
	
	switch( type[0] ) {
		case 'L': subtype = ferite_create_subtype( script, F_VAR_LONG ); break;
		case 'D': subtype = ferite_create_subtype( script, F_VAR_DOUBLE ); break;
		case 'S': subtype = ferite_create_subtype( script, F_VAR_STR ); break;
		case 'V': subtype = ferite_create_subtype( script, F_VAR_VOID ); break;
		case 'B': subtype = ferite_create_subtype( script, F_VAR_BOOL ); break;
		case 'U': subtype = ferite_create_subtype( script, F_VAR_UNDEFINED ); break;
		case 'O':
			subtype = ferite_create_subtype( script, F_VAR_OBJ );
			if( length > 1 && type[1] == ':' ) {
				for( i = 2; i < length && type[i] != ';'; i++ )
					;
				memset( target_class, 0, length );
				strncpy( target_class, type + 2, (i - 2) );
				subtype->data._class = ferite_find_namespace_element_contents( script, script->mainns, target_class, FENS_CLS );
				if( ferite_show_partial_implementation ) {
					fprintf( stderr, "Trying to find object %s (%p)\n", target_class, subtype->data._class);
				}
				if( subtype->data._class == NULL ) {
					ferite_subtype_destroy( script, subtype );
					ferite_error(script, 0, "Unable to locate class for object subtype: %s\n", target_class);
					return NULL;
				}
			}
			break;
		case 'C':
			subtype = ferite_create_subtype( script, F_VAR_CLASS );
			if( length > 1 && type[1] == ':' ) {
				for( i = 2; i < length && type[i] != ';'; i++ )
					;
				memset( target_class, 0, length );
				strncpy( target_class, type + 2, (i - 2) );
				subtype->data._class = ferite_find_namespace_element_contents( script, script->mainns, target_class, FENS_CLS );
				if( ferite_show_partial_implementation ) {
					fprintf( stderr, "Trying to find class %s (%p)\n", target_class, subtype->data._class);
				}
				if( subtype->data._class == NULL ) {
					ferite_subtype_destroy( script, subtype );
					ferite_error(script, 0, "Unable to locate class for class subtype: %s\n", target_class);
					return NULL;
				}
			}
			break;
		case 'N':
			subtype = ferite_create_subtype( script, F_VAR_NS );
			if( length > 1 && type[1] == ':' ) {
				for( i = 2; i < length && type[i] != ';'; i++ )
					;
				memset( target_class, 0, length );
				strncpy( target_class, type + 2, (i - 2) );
				subtype->data._namespace = ferite_find_namespace_element_contents( script, script->mainns, target_class, FENS_NS );
				if( ferite_show_partial_implementation ) {
					fprintf( stderr, "Trying to find namespace %s (%p)\n", target_class, subtype->data._namespace);
				}
				if( subtype->data._namespace == NULL ) {
					ferite_subtype_destroy( script, subtype );
					ferite_error(script, 0, "Unable to locate namespace for namespace subtype: %s\n", target_class);
					return NULL;
				}
			}
			break;
		case 'A':
			subtype = ferite_create_subtype( script, F_VAR_UARRAY );
			if( length > 1 ) {
				subtype->data._subtype = ferite_subtype_link( script, type + 1 );
				if( subtype->data._subtype == NULL ) {
					ferite_subtype_destroy( script, subtype );
					return NULL;
				}
			}
			break;
	}

	if( subtype ) {
		if( ferite_show_partial_implementation ) {
			fprintf( stderr, "Creating type for '%s'\n", type);
		}
		ferite_hamt_set( script, script->types, type, subtype );
	}
	return subtype;
}

void ferite_subtype_destroy( FeriteScript *script, FeriteVariableSubType *type ) {
	if( type->name ) {
		ferite_str_destroy( script, type->name );
	}
	ffree( type );
}