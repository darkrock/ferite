/*
 * Copyright (C) 2000-2007 Chris Ross
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

/**
* @class Obj
 * @description These set of functions are used to setup the base class 'Obj' within
 *			  the ferite system.
 */

FE_NATIVE_FUNCTION( ferite_obj_backtrace ) 
{
	FeriteVariable *var  = ferite_generate_backtrace( script, FE_TRUE );
	FE_RETURN_VAR(var);
}

char *ferite_parameters_to_string( FeriteScript *script, FeriteVariable **param_list );

FE_NATIVE_FUNCTION( ferite_obj_callFunction ) 
{
	FeriteVariable *functionName, *ret;
	int cp = ferite_get_parameter_count(params);
	FeriteVariable **np = ferite_create_parameter_list(script,cp+3);
	FeriteObject *self = __container__;
	FeriteFunction *callfunction = NULL;
	int i = 0;

	functionName = params[0];
	for(i = 1; i < cp ; i++) {
		np[i-1] = ferite_duplicate_variable( script, params[i], NULL);
		MARK_VARIABLE_AS_DISPOSABLE(np[i]);
	}

	callfunction = ferite_object_get_function_for_params( script, self, VAS(functionName)->data, np );
	if( callfunction == NULL ) {
		ferite_error( script, 0, "Unable to find function %s(%s) for use in callFunction\n", 
										VAS(functionName)->data, ferite_parameters_to_string(script,np) );
		FE_RETURN_VOID;
	}
	ret = ferite_call_function(script, self, current_recipient, callfunction, np);
	ferite_delete_parameter_list(script,np);

	if( ret != NULL ) {
		FE_RETURN_VAR(ret);
	}
	FE_RETURN_VOID;
}

FE_NATIVE_FUNCTION( ferite_obj_setVariable ) 
{
	FeriteObject *self = __container__;
	FeriteVariable *target = ferite_object_get_var( script, self, VAS(params[0])->data );
	if( target ) {
		if( ! ferite_variable_fast_assign( script, target, params[1] ) ) {
			ferite_error( script, 1, "Unable to assign value to object for attribute %s\n", VAS(params[0])->data );
			FE_RETURN_VOID;
		}
		FE_RETURN_TRUE;
	}
	ferite_error( script, 1, "Object has no such attribute %s\n", VAS(params[0])->data );
	FE_RETURN_VOID;
}

void ferite_add_object_class( FeriteScript *script )
{
	FeriteClass *klass = NULL;
	
	FE_ENTER_FUNCTION;
	klass = ferite_register_inherited_class( script, script->mainns, "Obj", NULL );
	fe_create_cls_fnc( script, klass, "backtrace", ferite_obj_backtrace, "", FE_TRUE );

	fe_create_cls_fnc( script, klass, "constructor", ferite_obj_constructor, "", FE_FALSE );
	fe_create_cls_fnc( script, klass, "constructor", ferite_obj_constructor, "", FE_TRUE );
	
	fe_create_cls_fnc( script, klass, "copy", ferite_obj_copy, "", FE_FALSE );
	fe_create_cls_fnc( script, klass, "hash", ferite_obj_hash, "", FE_FALSE );
	
	fe_create_cls_fnc( script, klass, "isSubclassOfClass", ferite_obj_isSubclassOfClass, "v", FE_TRUE );
	fe_create_cls_fnc( script, klass, "conformsToProtocol", ferite_obj_conformsToProtocol, "v", FE_TRUE );
	fe_create_cls_fnc( script, klass, "callFunction", ferite_obj_callFunction, "s.", FE_FALSE );
	fe_create_cls_fnc( script, klass, "setVariable", ferite_obj_setVariable, "sv", FE_FALSE );
	
	fe_create_cls_fnc( script, klass, "getClass", ferite_obj_getClass, "", FE_FALSE );
	fe_create_cls_fnc( script, klass, "respondsToMethod", ferite_obj_respondsToMethod, "s", FE_FALSE );
	
	fe_create_cls_fnc( script, klass, "rename", ferite_obj_rename, "ss", FE_FUNCTION_IS_DIRECTIVE );
	fe_create_cls_fnc( script, klass, "alias",  ferite_obj_alias, "ss", FE_FUNCTION_IS_DIRECTIVE );
	fe_create_cls_fnc( script, klass, "delete", ferite_obj_delete, "s", FE_FUNCTION_IS_DIRECTIVE );

	ferite_class_finish( script, klass );
	
	FE_LEAVE_FUNCTION(NOWT);
}
void ferite_add_closure_class( FeriteScript *script )
{
	FE_ENTER_FUNCTION;
	script->closureClassPtr = ferite_register_inherited_class( script, script->mainns, "Closure", "Obj" );
	ferite_class_finish( script, script->closureClassPtr );
	FE_LEAVE_FUNCTION(NOWT);
}

FE_NATIVE_FUNCTION( ferite_obj_constructor )
{
	FeriteVariable *ptr = NULL;
	
	FE_ENTER_FUNCTION;
	ptr = ferite_create_void_variable( script, "", FE_STATIC );
	MARK_VARIABLE_AS_DISPOSABLE( ptr );
	FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function isSubclassofClass
 * @brief Tests to see if the class is a subclass of the parameter
 * @param void klass The class to check
 * @return true on success false otherwise.
 */
FE_NATIVE_FUNCTION( ferite_obj_isSubclassOfClass )
{
	FeriteClass *klass = __container__;
	FeriteClass *parentklass = VAC(params[0]);
	FeriteVariable *ptr = NULL;
	
	FE_ENTER_FUNCTION;
	
	if( F_VAR_TYPE(params[0]) == F_VAR_CLASS )
	{
		while( klass != NULL ) 
		{
			if( klass == parentklass ) {
				ptr = ferite_create_number_long_variable( script, "", FE_TRUE, FE_STATIC );
				break;
			}
			klass = klass->parent;
		}
	}
	
	if( ptr == NULL )
		ptr = ferite_create_number_long_variable( script, "", FE_FALSE, FE_STATIC );
	
	MARK_VARIABLE_AS_DISPOSABLE( ptr );
	FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function conformsToProtocol
 * @brief Test to see if the object conforms to a protocol
 * @description Tests to see if the object conforms to a specific protocol
 * @param void protocol The protocol to test against
 * @return true on success false otherwise.
 */
FE_NATIVE_FUNCTION( ferite_obj_conformsToProtocol )
{
	FeriteClass *klass = __container__;
	FeriteClass *prot = VAC(params[0]);
	FeriteVariable *ptr = NULL;
	
	FE_ENTER_FUNCTION;
	
	if( F_VAR_TYPE(params[0]) == F_VAR_CLASS )
	{
		int i = 0;
		
		for( i = 0; i <= klass->impl_list->stack_ptr; i++ )
		{
			if( klass->impl_list->stack[i] == prot )
				ptr = ferite_create_number_long_variable( script, "", FE_TRUE, FE_STATIC );
		}
	}
	
	if( ptr == NULL )
		ptr = ferite_create_number_long_variable( script, "", FE_FALSE, FE_STATIC );
	
	MARK_VARIABLE_AS_DISPOSABLE( ptr );
	FE_LEAVE_FUNCTION(ptr);
}

/**
 * @function getClass
 * @declaration function getClass()
 * @brief Obtain a reference to the class that the object is instantiated from
 * @return A class
 */
FE_NATIVE_FUNCTION( ferite_obj_getClass )
{
	FeriteVariable *ptr = NULL;
	FeriteObject *self = __container__;
	
	FE_ENTER_FUNCTION;
	
	ptr = ferite_create_class_variable( script, "getClass", self->klass, FE_STATIC );  
	
	MARK_VARIABLE_AS_DISPOSABLE( ptr );
	FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function respondsToMethod
 * @brief Returns true if the object responds to the method name given
 * @param string method The name of the method to look for
 * @return true if it responds
 */
FE_NATIVE_FUNCTION( ferite_obj_respondsToMethod )
{
	FeriteVariable *ptr = NULL;
	FeriteFunction *func = NULL;
	FeriteObject *self = __container__;
	
	FE_ENTER_FUNCTION;
	
	func = ferite_object_get_function( script, self, VAS(params[0])->data );
	ptr = ferite_create_number_long_variable( script, "", (func != NULL), FE_STATIC );
	
	MARK_VARIABLE_AS_DISPOSABLE( ptr );
	FE_LEAVE_FUNCTION( ptr );
}

/** 
* @function copy
* @brief Return a copy of the object 
* @declaration function copy()
* @return A copy of the object and all it's values
*/
FE_NATIVE_FUNCTION( ferite_obj_copy )
{
	FeriteVariable *ptr = NULL;
	FeriteObject *self = FE_CONTAINER_TO_OBJECT;
	
	FE_ENTER_FUNCTION;
	ptr = ferite_create_object_variable( script, "copy", FE_STATIC );
	
	VAO(ptr) = fmalloc( sizeof( FeriteObject ) );
	
	VAO(ptr)->name = fstrdup( self->name );
	VAO(ptr)->oid = self->oid;
	VAO(ptr)->odata = NULL;
	VAO(ptr)->refcount = 1;
	VAO(ptr)->klass = self->klass;
	
	/* Copy the variables */
	VAO(ptr)->variables = ferite_duplicate_object_variable_list( script, self->klass );
	VAO(ptr)->functions = self->functions;
	
	MARK_VARIABLE_AS_DISPOSABLE( ptr );
	
	ferite_add_to_gc( script, VAO(ptr) );
	
	FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function hash
 * @brief Create a hash key for the object
 * @declaration function hash()
 * @return A string containing a unique hash key for the object
 */
FE_NATIVE_FUNCTION( ferite_obj_hash )
{
	FeriteVariable *ptr = NULL;
	FeriteObject *self = FE_CONTAINER_TO_OBJECT;
	char buf[1024];
	
	FE_ENTER_FUNCTION;
	sprintf( buf, "object/%s/%p", self->klass->name, self );
	
	ptr = ferite_create_string_variable_from_ptr( script, "Obj.hash()", buf, strlen(buf), FE_CHARSET_DEFAULT, FE_STATIC );
	MARK_VARIABLE_AS_DISPOSABLE( ptr );
	
	FE_LEAVE_FUNCTION( ptr );
}

FE_NATIVE_FUNCTION( ferite_obj_alias ) 
{
	FeriteVariable *ptr = fe_new_void_static("");
	char *from = VAS(params[0])->data;
	char *to = VAS(params[1])->data;
	FeriteFunction *fromfunc = NULL;
	FeriteFunction *tofunc = NULL;
	FeriteClass *self = FE_CONTAINER_TO_CLASS;
	int nothing_done = FE_TRUE;
	
	FE_ENTER_FUNCTION;
	/* Object */
	fromfunc = ferite_hash_get( script, self->object_methods, from );
	tofunc = ferite_hash_get( script, self->object_methods, to );	
	if( tofunc != NULL )
	{
		ferite_warning( script, "Can not alias '%s' to '%s', as '%s' already exists within the class '%s'\n", from, to, to, self->name );
		FE_LEAVE_FUNCTION(ptr);	
	}
	else
	{
		if( fromfunc != NULL )
		{
			tofunc = ferite_create_alias_function( script, fromfunc );
			ferite_hash_add( script, self->object_methods, to, tofunc );			
			FE_LEAVE_FUNCTION(ptr);	
		}
		else
			nothing_done = FE_TRUE;
	}
	
	/* Class */
	tofunc = ferite_hash_get( script, self->class_methods, from );
	fromfunc = ferite_hash_get( script, self->class_methods, to );
	if( tofunc != NULL )
	{
		ferite_warning( script, "Can not alias '%s' to '%s', as '%s' already exists within the class '%s'\n", from, to, to, self->name );
		FE_LEAVE_FUNCTION(ptr);	
	}
	else
	{
		if( fromfunc != NULL )
		{
			tofunc = ferite_create_alias_function( script, fromfunc );
			ferite_hash_add( script, self->class_methods, to, tofunc );			
			FE_LEAVE_FUNCTION(ptr);	
		}
		else
			nothing_done = FE_TRUE;
	}
	
	if( nothing_done ) {
		ferite_warning( script, "Can not alias '%s' to '%s', as '%s' can not be found\n", from, to, from );
	}
	FE_LEAVE_FUNCTION( ptr );
}

FE_NATIVE_FUNCTION( ferite_obj_rename ) 
{
	FeriteVariable *ptr = fe_new_void_static("");
	char *from = VAS(params[0])->data;
	char *to = VAS(params[1])->data;
	FeriteClass *self = FE_CONTAINER_TO_CLASS;
	FeriteVariable *var = NULL;
	FeriteFunction *func = NULL;
	
	FE_ENTER_FUNCTION;
	func = ferite_hash_get( script, self->object_methods, from );
	if( func != NULL )
	{
		FUD(( "COMPILE: renaming object function %s to %s\n", from, to ));
		ferite_hash_delete( script, self->object_methods, from );
		ferite_hash_add( script, self->object_methods, to, func );
		FE_LEAVE_FUNCTION(ptr);
	}
	func = ferite_hash_get( script, self->class_methods, from );
	if( func != NULL )
	{
		FUD(( "COMPILE: renaming class function %s to %s\n", from, to ));
		ferite_hash_delete( script, self->class_methods, from );
		ferite_hash_add( script, self->class_methods, to, func );
		FE_LEAVE_FUNCTION(ptr);
	}
	
	var = ferite_hash_get( script, self->class_vars, from );
	if( var != NULL )
	{
		FUD(( "COMPILE: renaming variable %s to %s\n", from, to ));
		ferite_hash_delete( script, self->class_vars, from );
		ferite_hash_add( script, self->class_vars, to, var );
		FE_LEAVE_FUNCTION(ptr);
	}
	
	var = ferite_hash_get( script, self->object_vars, from );
	if( var != NULL )
	{
		FUD(( "COMPILE: renaming variable %s to %s\n", from, to ));
		ferite_hash_delete( script, self->object_vars, from );
		ferite_hash_add( script, self->object_vars, to, var );
		FE_LEAVE_FUNCTION(ptr);
	}
	ferite_warning( script, "Unable to find class attribute '%s' to rename in class '%s'!\n", from, self->name );
	FE_LEAVE_FUNCTION( ptr );
}

FE_NATIVE_FUNCTION( ferite_obj_delete ) 
{
	return NULL;
}

/**
 * @end
 */

/*{{{ FeriteVariable *ferite_build_object( FeriteScript *script, FeriteClass *nclass) */
FeriteVariable *ferite_build_object( FeriteScript *script, FeriteClass *nclass)
{
	FeriteVariable *ptr = NULL;

	FE_ENTER_FUNCTION;

	if( nclass != NULL )
	{
		FUD(("BUILDOBJECT: Creating an instance of %s\n", nclass->name ));
		ptr = ferite_create_object_variable( script, nclass->name, FE_ALLOC );
		if( script )
			VAO(ptr) = ferite_stack_pop( script, script->objects );
		if( VAO(ptr) == NULL ) {
			VAO(ptr) = fmalloc( sizeof( FeriteObject ) );
			FUD(( "Allocating object %p\n", VAO(ptr) ));
		}
		VAO(ptr)->name = fstrdup( nclass->name );
		VAO(ptr)->klass = nclass;
		
		FUD(("BUILDOBJECT: Creating a duplicate varaible hash\n"));
		VAO(ptr)->variables = ferite_duplicate_object_variable_list( script, nclass );
		
		FUD(("BUILDOBJECT: Linking function list up\n"));
		VAO(ptr)->functions = nclass->object_methods;
		
		VAO(ptr)->oid = nclass->id;
		VAO(ptr)->odata = NULL;
		
		VAO(ptr)->refcount = 1;

		ferite_add_to_gc( script, VAO(ptr) );
	}
	FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/**
 * @group Objects
 */
/*{{{ FeriteVariable *ferite_new_object( FeriteScript *script, FeriteClass *nclass, FeriteVariable **plist ) */
/**
 * @function ferite_new_object
 * @declaration FeriteVariable *ferite_new_object( FeriteScript *script, FeriteClass *nclass, FeriteVariable **plist)
 * @brief Create a new object - the C equivalent to the ferite 'new' keyword.
 * @param FeriteScript *script The script to create the object within.
 * @param FeriteClass *nclass The class to instantiate an object from
 * @param FeriteVariable **plist The parameters to be passed to the objects constructor. This can be NULL for an empty
		  list.
 * @return A variable that references the newly created object. 
 * @warning The return variable is marked as disposable. If you wish to retain a reference and return the object from a
	native function you must 'UNMARK_VARIABLE_AS_DISPOSABLE'
 */
FeriteVariable *ferite_new_object( FeriteScript *script, FeriteClass *nclass, FeriteVariable **plist)
{
	FeriteVariable *ptr = NULL, *rval = NULL;
	FeriteFunction *func = NULL;
	FeriteVariable **params = NULL;
	
	FE_ENTER_FUNCTION;
	if(nclass != NULL)
	{		
		if( nclass->state == FE_ITEM_IS_ABSTRACT )
		{
			ferite_error( script, 0, "You can't create instances of the abstract class %s\n", nclass->name );
			FE_LEAVE_FUNCTION( NULL );
		}
		else if( nclass->state == FE_ITEM_IS_PROTOCOL )
		{
			ferite_error( script, 0, "You can't create instances of the protocol %s\n", nclass->name );
			FE_LEAVE_FUNCTION( NULL );
		}
		
		if( plist != NULL )
			params = plist;
		else
			params = ferite_create_parameter_list( script,3 );
			
		ptr = ferite_build_object(script,nclass);

		FUD(("NEWOBJECT: Marking as disposable\n" ));
		MARK_VARIABLE_AS_DISPOSABLE( ptr );
		
		FUD(("NEWOBJECT: Searching for constructor\n" ));
		func = ferite_find_constructor( script, VAO(ptr), params );
		if( func != NULL ) {
			FUD(("OPS: Calling constructor in class %s\n", nclass->name ));
			
			if( func->type == FNC_IS_EXTRL ) {
				EXTERNAL_ENTER(func);
				rval = (func->fncPtr)( script, VAO(ptr), NULL, func, params );
				EXTERNAL_EXIT();
			}
			else
				rval = ferite_script_function_execute( script, VAO(ptr), NULL, func, params );
			
			if( rval == NULL || (rval != NULL && F_VAR_TYPE(rval) == F_VAR_OBJ && VAO(rval) == NULL) )
			{
				if( rval == NULL ) {
					/* So that I don't forget again, this is if the function returns NULL, not if the
					 * ferite function called returns null */
					ferite_error( script, 0, "Unable to instantiate object from class '%s'\n", nclass->name );
				}
				/* clean up the class */
				ferite_delete_object_variable_list( script, VAO(ptr)->variables );
				VAO(ptr)->variables = NULL;
				VAO(ptr)->functions = NULL;
				VAO(ptr)->klass = NULL;
				FDECREF(VAO(ptr));
				
				/* set our return val to NULL */
				VAO(ptr) = NULL;
			}
			if( rval != NULL )
				ferite_variable_destroy( script, rval );
		}
		else			
			ferite_error( script, 0, "Unable to find constructor of the class '%s' for the given parameters\n", nclass->name );
		
		if( plist == NULL )
			ferite_delete_parameter_list( script, params );	   
	}
	FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/
/** @end */