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

/* this is so that each registered class gets a unique id. this allows for native
 * code to check whether they are being passed one of their objects or someone elses. */
long ferite_internal_class_counter = 1000;

/**
 * @group Classes
 * @description Class are what objects are made from. There are a number of functions in this group
 *			  that allows for creation and manipulation of both objects and classes.
 */

/**
 * @function ferite_register_inherited_class
 * @declaration FeriteClass *ferite_register_inherited_class( FeriteScript *script, FeriteNamespace *ns, char *name, char *parent )
 * @brief Registers, creates and returns a class object
 * @param FeriteScript *script The current script being run
 * @param FeriteNamespace *ns	 The namespace in which the class should be registered
 * @param char *name   The name of the class eg. Socket
 * @param char *parent The name of the parent class, this is the class that will be inherited from, if NULL the class will inherit from the base class 'Obj'.
 * @description This is the only way to create a class natively.
 * @return The newly created class
 */
FeriteClass *ferite_register_inherited_class( FeriteScript *script, FeriteNamespace *ns, char *name, char *parent )
{
	FeriteClass *ptr = NULL, *klass = NULL;

	FE_ENTER_FUNCTION;

	if( (ferite_namespace_element_exists( script, ns, name )) != NULL )
	{
		ferite_error( script, 0, "An item '%s' already exists so the class can't be created.\n", name );
		FE_LEAVE_FUNCTION(NULL);
	}
	if( parent == NULL && strcmp( name, "Obj" ) )
	{
		parent = "Obj";
	}
	if( parent != NULL )
	{
		ptr = ferite_find_class( script, ns, parent );
		if( ptr == NULL )
		{
			ferite_warning( script, "Parent class %s does not exist. Not inheriting from it for %s.\n", parent, name );
			if( strcmp( parent, "Obj" ) )
			{
				parent = "Obj";
				ptr = ferite_find_class( script, ns, parent );
			}
			if( ptr == NULL )
			{
				ferite_error( script, 0, "Failed to even use Obj as a parent for %s\n", name );
				FE_LEAVE_FUNCTION( NULL );
			}
		}
		else if( ptr->state == FE_ITEM_IS_FINAL )
		{
			ferite_error( script, 0, "Parent class %s is final and can not be extended by new class %s\n", parent, name );
			FE_LEAVE_FUNCTION( NULL );
		}
		else if( ptr->state == FE_ITEM_IS_PROTOCOL )
		{
			ferite_error( script, 0, "Parent '%s' is a protocol and not a class and therefore can not be extended by new class %s\n", parent, name );
			FE_LEAVE_FUNCTION( NULL );
		}
	}
	klass = fmalloc( sizeof( FeriteClass ) );
	klass->name = fstrdup( name );
	klass->object_vars = ferite_create_hash( script, FE_CLASS_VARIABLE_HASH_SIZE );
	klass->class_vars = ferite_create_hash( script, FE_CLASS_VARIABLE_HASH_SIZE );
	klass->object_methods = ferite_create_hash( script, FE_CLASS_FUNCTION_HASH_SIZE );
	klass->class_methods = ferite_create_hash( script, FE_CLASS_FUNCTION_HASH_SIZE );
	klass->id = ++ferite_internal_class_counter;
	klass->odata = NULL;
	klass->parent = ptr;
	klass->next = NULL;
	klass->container = ns;
	klass->state = 0;
	klass->impl_list = ferite_create_stack( script, 5 );
	klass->cached = FE_FALSE;
	ferite_register_ns_class( script, ns, klass );

	FE_LEAVE_FUNCTION( klass );
}

/**
 * @function ferite_register_class_function
 * @declaration int ferite_register_class_function( FeriteScript *script, FeriteClass *klass, FeriteFunction *f )
 * @brief Register a function within a class
 * @param FeriteScript *script The current script
 * @param FeriteClass *klass The class to place the function in
 * @param FeriteFunction *f	  The function structure
 * @param int is_static Boolean, 0 = not static, 1 = static. Allows the specifcation of whether or not the function has static access within the class
 * @description It must be noted that the function being passed must not have the super, and self variables within their signitue, this
 *			  function will handle that automatically
 * @return 1 if function was registered correctly, 0 otherwise
 */
int ferite_register_class_function( FeriteScript *script, FeriteClass *klass, FeriteFunction *f, int is_static )
{
	FeriteFunction *function = NULL, *ptr = NULL;
	FeriteHash *target_hash = NULL;
	char *new_function_sig = NULL;
	int may_add = FE_TRUE;
	
	FE_ENTER_FUNCTION;
	if( klass != NULL )
	{
		target_hash = klass->object_methods;
		if( is_static )
			target_hash = klass->class_methods;
		
		f->is_static = is_static;
		f->klass = klass;
		
		if( (function = ferite_hash_get( script, target_hash, f->name )) != NULL )
		{
			if( strcmp( f->name, "destructor" ) == 0 )
			{
				ferite_error( script, 0, "You can only have one '%s' destructor in the class %s\n", (is_static ? "static" : "instance"), klass->name );
				FE_LEAVE_FUNCTION( FE_FALSE );
			}
			else if( strcmp( f->name, klass->name ) == 0 && is_static )
			{
				ferite_error( script, 0, "You can only have one static constructor in the class '%s'\n", klass->name );
				FE_LEAVE_FUNCTION( FE_FALSE );
			}
			
			new_function_sig = ferite_function_generate_sig_string( script, f );
			for( ptr = function; ptr != NULL; ptr = ptr->next ) {
				char *old_function_sig = ferite_function_generate_sig_string( script, ptr );
				if( strcmp( new_function_sig, old_function_sig ) == 0 ) {
					may_add = FE_FALSE;
				}
				ffree(old_function_sig);
				if( !may_add ) {
					break;
				}
			}

			ffree( new_function_sig );
			if( may_add ) {
				f->next = function->next;
				function->next = f;
			}
		}
		else
			ferite_hash_add( script, target_hash, f->name, f );

		FE_LEAVE_FUNCTION( may_add );
	}
	FE_LEAVE_FUNCTION( FE_FALSE );
}

/**
 * @function ferite_register_class_variable
 * @declaration int ferite_register_class_variable( FeriteScript *script, FeriteClass *klass, FeriteVariable *variable, int is_static )
 * @brief Register a variable within a class
 * @param FeriteScript *script	The current script
 * @param FeriteClass *klass  The class to attach the variable to
 * @param FeriteVariable *variable  The variable to register
 * @param int is_static Boolean, 0 = not static, 1 = static. Allows the specifcation of whether or not the variable has static access within the class
 * @return 1 on success, 0 otherwise
 */
int ferite_register_class_variable( FeriteScript *script, FeriteClass *klass, char *name, FeriteVariable *variable, int is_static )
{
	FE_ENTER_FUNCTION;
	
	if( klass != NULL )
	{
		if( variable != NULL )
		{
			if( is_static )
			{
				ferite_hash_add( script, klass->class_vars, name, variable );
				MARK_VARIABLE_AS_STATIC( variable );
			}
			else
				ferite_hash_add( script, klass->object_vars, name, variable );
		}
		else
		{
			ferite_error( script, 0, "Can't register a NULL variable in class %s", klass->name );
			FE_LEAVE_FUNCTION( FE_FALSE );
		}
	}
	else
	{
		ferite_error( script, 0, "Can't register a variable in a non existant class" );
		FE_LEAVE_FUNCTION( FE_FALSE );
	}
	FE_LEAVE_FUNCTION( FE_TRUE );
}

/**
 * @function ferite_class_finish
 * @declaration void ferite_class_finish( FeriteScript *script, FeriteClass *klass )
 * @brief Finish the class off and call any relevent functions needed.
 * @param FeriteScript *script The script the class sits in
 * @param FeriteClass *klass The class to finish off
 */
void ferite_class_finish( FeriteScript *script, FeriteClass *klass )
{
	FE_ENTER_FUNCTION;
	ferite_class_call_static_constructor( script, klass );
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_class_call_static_constructor
 * @declaration void ferite_class_call_static_constructor( FeriteScript *script, FeriteClass *klass )
 * @brief Call the static constructor on the class
 * @param FeriteScript *script The script in which the class resides
 * @param FeriteClass *klass The class
 * @description This function will traverse the heirachy within the class system to allow for parents to
 *				be called.
 */
void ferite_class_call_static_constructor( FeriteScript *script, FeriteClass *klass )
{
	FeriteFunction *function = NULL;
	FeriteVariable *return_value = NULL;
	
	FE_ENTER_FUNCTION;
	function = ferite_find_static_constructor( script, klass, NULL );
	if( function != NULL )
	{
		return_value = ferite_call_function( script, klass, NULL, function, NULL );
		ferite_variable_destroy( script, return_value );
	}	
	FE_LEAVE_FUNCTION(NOWT);	
}

/**
* @function ferite_class_call_static_destructor
 * @declaration void ferite_class_call_static_destructor( FeriteScript *script, FeriteClass *klass )
 * @brief Call the static destructor on the class
 * @param FeriteScript *script The script in which the class resides
 * @param FeriteClass *klass The class
 * @description This function will traverse the heirachy within the class system to allow for parents to
 *				be called.
 */
void ferite_class_call_static_destructor( FeriteScript *script, FeriteClass *klass )
{
	FeriteFunction *function = NULL;
	FeriteVariable *return_value = NULL;
	
	FE_ENTER_FUNCTION;
	function = ferite_class_get_function( script, klass, "destructor" );
	if( function != NULL )
	{
		return_value = ferite_call_function( script, klass, NULL, function, NULL );
		ferite_variable_destroy( script, return_value );
	}	
	FE_LEAVE_FUNCTION(NOWT);	
}


/**
 * @function ferite_delete_class
 * @declaration void ferite_delete_class( FeriteScript *script, FeriteClass *klass )
 * @brief Clean up and free the memory a class takes up
 * @param FeriteScript *script The current script
 * @param FeriteClass *klass The class to be deleted
 */
void ferite_delete_class( FeriteScript *script, FeriteClass *klass )
{
	FE_ENTER_FUNCTION;

	if( klass != NULL && !klass->cached )
	{
		FUD(("Deleting Class: %s\n", klass->name ));
		ferite_class_call_static_destructor( script, klass );
		
		FUD(("Deleting class contents: %s (%s) [%p]\n", ferite_generate_class_fqn( script, klass ), klass->name, klass));
		
		ferite_delete_hash( script, klass->object_vars, (void (*)(FeriteScript *,void *))ferite_variable_destroy );
		ferite_delete_hash( script, klass->class_vars, (void (*)(FeriteScript *,void *))ferite_variable_destroy );
		ferite_delete_function_hash( script, klass->object_methods );
		ferite_delete_function_hash( script, klass->class_methods );
		ferite_delete_stack( script, klass->impl_list );
		ffree( klass->name );
		ffree( klass );
	}

	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_find_class
 * @declaration FeriteClass *ferite_find_class( FeriteScript *script, FeriteNamespace *ns, char *name )
 * @brief Find a class within a namespace
 * @param FeriteScript *script The current script
 * @param FeriteNamespace *ns	 The top level namespace to look in
 * @param char *name   The name of the class to find.
 * @return The class on success or NULL otherwise
 */
FeriteClass *ferite_find_class( FeriteScript *script, FeriteNamespace *ns, char *name )
{
	FeriteClass *ptr = NULL;
	FeriteNamespaceBucket *nsb = NULL;

	FE_ENTER_FUNCTION;
	FUD(("Trying to find class %s\n", name));
	nsb = ferite_find_namespace( script, ns, name, FENS_CLS );
	if( nsb != NULL )
	{
		ptr = nsb->data;
		FE_LEAVE_FUNCTION( ptr );
	}
	nsb = ferite_find_namespace( script, script->mainns, name, FENS_CLS );
	if( nsb != NULL )
	{
		ptr = nsb->data;
		FE_LEAVE_FUNCTION( ptr );
	}
	FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_find_class_id
 * @declaration long ferite_find_class_id( FeriteScript *script, FeriteNamespace *ns, char *name )
 * @brief Find a class within a namespace, and return it's unique id
 * @param FeriteScript *script The current script
 * @param FeriteNamespace *ns	 The top level namespace to look in
 * @param char *name   The name of the class to find.
 * @return The id or 0 otherwise
 */
long ferite_find_class_id( FeriteScript *script, FeriteNamespace *ns, char *name )
{
	FeriteClass *ptr = NULL;

	FE_ENTER_FUNCTION;

	FUD(("Trying to find class %s\n", name));
	ptr = ferite_find_class( script, ns, name );
	if( ptr )
	{
		FE_LEAVE_FUNCTION( ptr->id );
	}
	FE_LEAVE_FUNCTION( 0 );
}

FeriteObjectVariable *ferite_duplicate_object_variable_list( FeriteScript *script, FeriteClass *klass )
{
	FeriteObjectVariable *ov = NULL;
	
	FE_ENTER_FUNCTION;
	ov = fmalloc( sizeof( FeriteObjectVariable ) );
	ov->variables = ferite_hash_dup( script, klass->object_vars, (void *(*)(FeriteScript *,void *,void*))ferite_duplicate_variable, NULL );
	ov->parent = NULL;
	if( klass->parent != NULL )
		ov->parent = ferite_duplicate_object_variable_list( script, klass->parent );
	ov->klass = klass;	
	FE_LEAVE_FUNCTION( ov );
}

void ferite_delete_object_variable_list( FeriteScript *script, FeriteObjectVariable *ov )
{
	FE_ENTER_FUNCTION;

	if( ov->parent != NULL )
		ferite_delete_object_variable_list( script, ov->parent );
	if( ov->variables != NULL )
		ferite_delete_hash( script, ov->variables, (void (*)(FeriteScript *,void *))ferite_variable_destroy );
	ffree( ov );
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_delete_class_object
 * @declaration void ferite_delete_class_object( FeriteObject *object )
 * @brief Dispose of an object, the only part of ferite that should be calling this is the garbage collector.
 * @param object The object to be nuked.
 * @description
 * If you want to get rid of an object the accepted method is as follows:<nl/>
 * <nl/>
 *	object->refcount = 0;<nl/>
 *	ferite_check_gc();<nl/>
 * <nl/>
 * This will dispose of the object properly. The object destructor will get called if the objects' class is native.
 */
void ferite_delete_class_object( FeriteScript *script, FeriteObject *object, int do_destructor )
{
	FeriteFunction *func = NULL;
	FeriteVariable *retv = NULL;
	FeriteVariable **params = NULL;
	FeriteClass *klass = NULL;

	FE_ENTER_FUNCTION;
	if( object != NULL ) {
		if( object->refcount < 0 )
			ferite_debug_catch( object, object->refcount );
		
		FUD(( "klass: %p, %s\n", object->klass, (object->klass ? object->klass->name : "") ));
		if( do_destructor && object->klass != NULL && object->klass->object_methods != NULL )
		{
			FUD(( "ferite_delete_class_object: trying to delete %s[%s]\n", object->name, object->klass->name ));
			for( klass = object->klass; func == NULL && klass != NULL; klass = klass->parent )
				func = ferite_hash_get( script, klass->object_methods, "destructor" );

			if( func != NULL ) {
				params = fmalloc( sizeof( FeriteVariable * ) * 3 );
				params[0] = NULL;
				params[1] = NULL;
				params[2] = NULL;

				/* we have the destructor */
				FUD(( "OPS: Calling destructor (%s) in class %s\n", func->name, object->klass->name));
				if( func->type == FNC_IS_EXTRL ) {
					EXTERNAL_ENTER(func);
					retv = (func->fncPtr)( script, object, NULL, func, params );
					EXTERNAL_EXIT();
				}
				else
					retv = ferite_script_function_execute( script, object, NULL, func, params );

				ffree( params );
				ferite_variable_destroy( script, retv );
			}
		}

		FUD(( "Deleting class %s's variable hash %p\n", object->name, object->variables ));
		if( object->variables != NULL )
		{
			ferite_delete_object_variable_list( script, object->variables );
			object->variables = NULL;
		}
		
		if( object->name != NULL )
			ffree( object->name );

		/* Object cache */
		if( script && script->objects && script->objects->stack_ptr < script->objects->size-1 ) {
			FUD(( "Wanting to stack %p [%d] %d\n", object, script->objects->size, object->refcount ));
			ferite_stack_push( script, script->objects, object );
		} else {
			FUD(( "Wanting to free %p [%d] %d\n", object, script->objects->size, object->refcount ));
			ffree( object );
		}
	}
	else
	  ferite_error( script, 0, "Error: trying to delete null object\n" );
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_object_get_var
 * @declaration FeriteVariable *ferite_object_get_var( FeriteScript *script, FeriteObject *object, char *name )
 * @brief Get a member variable from an object
 * @param FeriteScript *script The script
 * @param FeriteObject *object The object to get the variable from
 * @param char *name The name of the member to get
 * @return The variable on success, NULL otherwise
 */
FeriteVariable *ferite_object_get_var( FeriteScript *script, FeriteObject *object, char *name )
{
	FeriteVariable *ptr = NULL;
	FeriteObjectVariable *ov = NULL;
	
	FE_ENTER_FUNCTION;
	if( object != NULL )
	{		
		ov = object->variables;
		for( ov = object->variables; ov != NULL; ov = ov->parent )
		{
			ptr = ferite_hash_get( script, ov->variables, name );
			if( ptr != NULL && !FE_VAR_IS_STATIC(ptr) )
			{
				FE_LEAVE_FUNCTION( ptr );
			}
		}
	}
	FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_object_variable_class
 * @declaration FeriteClass *ferite_object_variable_class( FeriteScript *script, FeriteObject *object, char *name )
 * @brief Obtain the class in which a named variable appears within an object
 * @param FeriteScript *script The script
 * @param FeriteObject *object The object to look in
 * @param char *name The name of the member to get
 * @return The class if the varible exists and the class can be found, NULL otherwise
 * @description Due to the way that variables are stored within the class hierachy, it is useful to find out to which
 *			  class a variable exists.
 */
FeriteClass *ferite_object_variable_class( FeriteScript *script, FeriteObject *object, char *name )
{
	FeriteVariable *ptr = NULL;
	FeriteObjectVariable *ov = NULL;
	
	FE_ENTER_FUNCTION;
	if( object != NULL )
	{		
		ov = object->variables;
		for( ov = object->variables; ov != NULL; ov = ov->parent )
		{
			ptr = ferite_hash_get( script, ov->variables, name );
			if( ptr != NULL && !FE_VAR_IS_STATIC(ptr) )
			{
				FE_LEAVE_FUNCTION( ov->klass );
			}
		}
	}
	FE_LEAVE_FUNCTION( NULL );
}

/**
* @function ferite_class_variable_class
 * @declaration FeriteClass *ferite_class_variable_class( FeriteScript *script, FeriteClass *klass, char *name )
 * @brief Obtain the class in which a named variable appears within a class tree
 * @param FeriteScript *script The script
 * @param FeriteClass *klass The class to look in
 * @param char *name The name of the member to get
 * @return The class if the varible exists and the class can be found, NULL otherwise
 * @description Due to the way that variables are stored within the class hierachy, it is useful to find out to which
 *			  class a variable exists.
 */
FeriteClass *ferite_class_variable_class( FeriteScript *script, FeriteClass *klass, char *name )
{
	FeriteVariable *ptr = NULL;
	FeriteClass *k = NULL;
	
	FE_ENTER_FUNCTION;
	if( klass != NULL )
	{				
		for( k = klass; k != NULL; k = k->parent )
		{
			ptr = ferite_hash_get( script, k->class_vars, name );
			if( ptr != NULL )
			{
				FE_LEAVE_FUNCTION( k );
			}
		}
	}
	FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_class_get_var
 * @declaration FeriteVariable *ferite_class_get_var( FeriteScript *script, FeriteClass *klass, char *name )
 * @brief Get a member variable from an class
 * @param FeriteScript *script The script
 * @param FeriteClass *klass The class to get the variable from
 * @param char *name The name of the member to get
 * @return The variable on success, NULL otherwise
 */
FeriteVariable *ferite_class_get_var( FeriteScript *script, FeriteClass *klass, char *name )
{
	FeriteVariable *ptr = NULL;
	FeriteClass *k = NULL;
	
	FE_ENTER_FUNCTION;
	if( klass != NULL )
	{
		for( k = klass; k != NULL; k = k->parent )
		{
			ptr = ferite_hash_get( script, k->class_vars, name );
			if( ptr != NULL && FE_VAR_IS_STATIC(ptr) )
			{
				FE_LEAVE_FUNCTION( ptr );
			}
		}
	}
	FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_object_get_function
 * @declaration FeriteFunction *ferite_object_get_function( FeriteScript *script, FeriteObject *object, char *name )
 * @brief Get a function from an object
 * @param FeriteScript *script The script
 * @param FeriteObject *object The object to get the function from
 * @param char *name The name of the function
 * @return The function on success, NULL otherwise
 */
FeriteFunction *ferite_object_get_function( FeriteScript *script, FeriteObject *object, char *name )
{
	FeriteFunction *ptr = NULL;
	FeriteClass *cls = NULL;
	
	FE_ENTER_FUNCTION;
	if( object != NULL )
	{
		cls = object->klass;
		while( cls != NULL )
		{
			ptr = ferite_hash_get( script, cls->object_methods, name );
			if( ptr != NULL )
			{
				FE_LEAVE_FUNCTION( ptr );
			}
			cls = cls->parent;
		}
	}
	FE_LEAVE_FUNCTION( ptr );
}
/**
 * @function ferite_object_get_function_for_params
 * @declaration FeriteFunction *ferite_object_get_function_for_params( FeriteScript *script, FeriteObject *object, char *name, FeriteVariable **params )
 * @brief Get a function from an object for a specific set of parameters
 * @param FeriteScript *script The script
 * @param FeriteObject *object The object to get the function from
 * @param char *name The name of the function
 * @param FeriteVariable **params The parameters that wish to be passed to the function
 * @return The function on success, NULL otherwise
 * @description This function will traverse the inheritance tree the object is from and look for a function that matches the given name and
				parameters.
 */
FeriteFunction *ferite_object_get_function_for_params( FeriteScript *script, FeriteObject *object, char *name, FeriteVariable **params )
{
	FeriteFunction *ptr = NULL;
	FeriteFunction *retf = NULL;
	FeriteClass *klass = NULL;
	FeriteVariable **plist = NULL;

	FE_ENTER_FUNCTION;
	if( object != NULL )
	{
		plist = params;
		if( params == NULL )
		{
			plist = ferite_create_parameter_list( script,1 );
			plist[0] = NULL;
		}
		
		klass = object->klass;
		while( klass != NULL )
		{
			for( ptr = ferite_hash_get( script, klass->object_methods, name ); ptr != NULL; ptr = ptr->next )
			{
				if( ferite_check_params( script, plist, ptr ) == 1 )
				{
					retf = ptr;
					break;
				}
			}
			if( retf != NULL )
				break;
			
			klass = klass->parent;
		}
		if( params == NULL )
			ffree( plist );
	}
	FE_LEAVE_FUNCTION( retf );
}

/**
 * @function ferite_class_get_function
 * @declaration FeriteFunction *ferite_class_get_function( FeriteScript *script, FeriteClass *cls, char *name )
 * @brief Get a function from a class
 * @param FeriteScript *script The script
 * @param FeriteClass *cls The class to get the function from
 * @param char *name The name of the function
 * @return The function on success, NULL otherwise
 */
FeriteFunction *ferite_class_get_function(FeriteScript *script, FeriteClass *cls, char *name)
{
	FeriteFunction *ptr = NULL;
	FeriteClass *klass = cls;
	int doneTest = FE_FALSE;
	
	FE_ENTER_FUNCTION;
	while(klass != NULL)
	{
		ptr = ferite_hash_get(script, klass->class_methods, name);
		if( ptr != NULL )
		{
			FE_LEAVE_FUNCTION( ptr );
		}
		if( !doneTest )
		{
			if( strcmp( name, "destructor" ) == 0 ) /* We only look in the current class for the destructor, evil but necessary */
			break;
			doneTest = FE_TRUE;
		}
		klass = klass->parent;
	}
	FE_LEAVE_FUNCTION( NULL );
}
/**
* @function ferite_class_get_function_for_params
 * @declaration FeriteFunction *ferite_class_get_function_for_params( FeriteScript *script, FeriteClass *klass, char *name, FeriteVariable **params )
 * @brief Get a function from an object for a specific set of parameters
 * @param FeriteScript *script The script
 * @param FeriteClass *klass The class to get the function from
 * @param char *name The name of the function
 * @param FeriteVariable **params The parameters that wish to be passed to the function
 * @return The function on success, NULL otherwise
 * @description This function will traverse the inheritance tree the class is from and look for a function that matches the given name and
				parameters.
 */
FeriteFunction *ferite_class_get_function_for_params( FeriteScript *script, FeriteClass *klass, char *name, FeriteVariable **params )
{
	FeriteFunction *ptr = NULL;
	FeriteFunction *retf = NULL;
	FeriteVariable **plist = NULL;
	int doneTest = FE_FALSE;
	
	FE_ENTER_FUNCTION;
	if( klass != NULL )
	{
		plist = params;
		if( params == NULL )
		{
			plist = ferite_create_parameter_list( script,1 );
			plist[0] = NULL;
		}
		
		while( klass != NULL )
		{
			for( ptr = ferite_hash_get( script, klass->class_methods, name ); ptr != NULL; ptr = ptr->next )
			{
				if( ferite_check_params( script, plist, ptr ) == 1 )
				{
					retf = ptr;
					break;
				}
			}
			if( retf != NULL )
				break;
			
			if( !doneTest )
			{
				if( strcmp( name, "destructor" ) == 0 ) 
					/* We only look in the current class for the destructor, evil but necessary */
					break;
				doneTest = FE_TRUE;
			}	
			klass = klass->parent;
		}
		if( params == NULL )
			ffree( plist );
	}
	FE_LEAVE_FUNCTION( retf );
}

/**
 * @function ferite_class_has_function
 * @declaration int ferite_class_has_function(FeriteScript *script, FeriteClass *cls, char *name)
 * @brief Check to see if a class has the named function
 * @param FeriteScript *script The script
 * @param FeriteClass *cls The class to check in
 * @param char *name The name of the function
 * @return FE_TRUE on success, FE_FALSE otherwise
 */
int ferite_class_has_function(FeriteScript *script, FeriteClass *cls, char *name)
{
	FeriteFunction *ptr = ferite_class_get_function( script, cls, name );

	FE_ENTER_FUNCTION;
	if( ptr == NULL )
	{
		FE_LEAVE_FUNCTION( FE_FALSE );
	}
	FE_LEAVE_FUNCTION( FE_TRUE );
}

/**
 * @function ferite_object_has_var
 * @declaration int ferite_object_has_var(FeriteScript* script, FeriteObject* obj, char *id)
 * @brief Check to see whether an object has a certain variable
 * @param FeriteScript *script The script
 * @param FeriteObject *obj The object to check
 * @param char *id The name of the variable
 * @return If the variable exists FE_TRUE is returned, otherwise FE_FALSE
 */
int ferite_object_has_var(FeriteScript* script, FeriteObject* obj, char *id)
{
	FeriteVariable *ptr = NULL;

	FE_ENTER_FUNCTION;
	ptr = ferite_object_get_var( script, obj, id );
	if( ptr != NULL )
	{
		FE_LEAVE_FUNCTION( FE_TRUE );
	}
	FE_LEAVE_FUNCTION( FE_FALSE );
}

/**
 * @function ferite_object_set_var
 * @declaration void ferite_object_set_var( FeriteScript* script, FeriteObject* obj, char *id, FeriteVariable *d_var )
 * @brief Set the value of an objects variable
 * @param FeriteScript *script The script
 * @param FeriteObject *obj The object whose member is to be set
 * @param char *name The name of the variable
 * @param FeriteVariable *d_var The data used
 * @description If the variable exists, the value is updated, otherwise a new variable is added to the
 *			  objects variables
 */
void ferite_object_set_var( FeriteScript* script, FeriteObject* obj, char *id, FeriteVariable *d_var )
{
	FeriteVariable *ptr = NULL;
	FeriteObjectVariable *ov = NULL;
	
	FE_ENTER_FUNCTION;
	if( obj ) {
		UNMARK_VARIABLE_AS_DISPOSABLE( d_var );
		for( ov = obj->variables; ov != NULL; ov = ov->parent )
		{
			ptr = ferite_hash_get( script, ov->variables, id );
			if( ptr != NULL )
			{
				ferite_hash_update( script, ov->variables, id, (void*)d_var );
				ferite_variable_destroy( script, ptr );
				FE_LEAVE_FUNCTION(NOWT);
			}
		}
		ferite_hash_add( script, obj->variables->variables, id, d_var );
	}
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_object_call_super
 * @declaration void ferite_object_call_super( FeriteScript *script, FeriteObject *object, FeriteVariable **params )
 * @brief Call the objects parent class's constructor usefull for writing native classes
 * @param FeriteScript *script The script
 * @param FeriteVariable The container on which to call the super constructor
 * @param FeriteVariable **params The parameters to pass to the function call
 */
FeriteVariable *ferite_object_call_super( FeriteScript *script, FeriteVariable *container, FeriteVariable **params )
{
	FeriteFunction *function = NULL;
	FeriteClass *old_parent = NULL, *klass = NULL;
	FeriteVariable *rval = NULL;
	
	FE_ENTER_FUNCTION;
	
	if( F_VAR_TYPE(container) == F_VAR_OBJ )
	  klass = VAO(container)->klass;
	else if( F_VAR_TYPE(container) == F_VAR_CLASS )
	  klass = VAC(container);
	else
	{
		ferite_error( script, 0, "Can't call super on non-class/object container\n" );
		FE_LEAVE_FUNCTION( NULL );
	}
	
	if( klass->parent == NULL )
	{
		/* Our fake variable */
		rval = ferite_create_void_variable( script, "ferite_call_super", FE_STATIC );
		MARK_VARIABLE_AS_DISPOSABLE( rval );
		
		/* return no parental constructor to call */
		FE_LEAVE_FUNCTION( rval );
	}

	if( F_VAR_TYPE(container) == F_VAR_OBJ )
	{
		FeriteClass *k = VAO(container)->klass;
		VAO(container)->klass = k->parent;
		function = ferite_find_constructor( script, VAO(container), params );
		VAO(container)->klass = k;
	}
	else
	  function = ferite_find_static_constructor( script, klass->parent, params );
	
	if( function != NULL )
	{
		if( F_VAR_TYPE(container) == F_VAR_OBJ )
		{
			/* this basically makes the constructor being called play like it's own class */
			old_parent = VAO(container)->klass;
			VAO(container)->klass = old_parent->parent;
		}
		
		rval = ferite_call_function( script, VAP(container), NULL, function, params );

		if( F_VAR_TYPE(container) == F_VAR_OBJ )
		{
			/* swap it back */
			VAO(container)->klass = old_parent;
		}
	}
	else
	{
		/* No super constructor, lets just return void */
		rval = ferite_create_void_variable( script, "ferite_call_super", FE_STATIC );
		MARK_VARIABLE_AS_DISPOSABLE( rval );
	}
	
	FE_LEAVE_FUNCTION( rval );
}

/**
 * @function parser
 * @declaration int ferite_object_is_subclass( FeriteObject *obj, char *name )
 * @brief See if the object is from a subclass of the named class
 * @param FeriteObject *obj The object to check
 * @param char *name The name of the class
 * @return FE_TRUE if the object is, FE_FALSE otherwise
 */
int ferite_object_is_subclass( FeriteObject *obj, char *name )
{
	FeriteClass *ptr;

	FE_ENTER_FUNCTION;
	if( obj != NULL )
	{
		for( ptr = obj->klass; ptr != NULL; ptr = ptr->parent )
		{
			if( strcmp( ptr->name, name ) == 0 )
			{
				FE_LEAVE_FUNCTION( FE_TRUE );
			}
		}
	}
	FE_LEAVE_FUNCTION( FE_FALSE );
}

/**
 * @function ferite_class_is_subclass
 * @declaration int ferite_class_is_subclass( FeriteClass *klass, FeriteClass *subklass )
 * @brief Check to see if one class is the sub-class of another
 * @param FeriteClass *klass The class to check against
 * @param FeriteClass *subklass The class to check with
 * @return FE_TRUE if it is a sub-class, FE_FALSE otherwise
 */
int ferite_class_is_subclass( FeriteClass *klass, FeriteClass *subklass )
{
	FeriteClass *ptr = NULL;
	
	FE_ENTER_FUNCTION;
	if( klass != NULL && subklass != NULL )
	{
		for( ptr = subklass; ptr != NULL; ptr = ptr->parent )
		{
			if( ptr != NULL && ptr == klass )
			{
				FE_LEAVE_FUNCTION( FE_TRUE );
			}
		}		
	}		
	FE_LEAVE_FUNCTION( FE_FALSE );
}

/**
 * @function ferite_find_constructor
 * @declaration FeriteFunction *ferite_find_constructor( FeriteScript *script, FeriteClass *klass )
 * @brief Get a pointer to the first constructor in a class tree
 * @param FeriteScript *script The script
 * @param FeriteClass *klass The class to start with
 * @return A pointer to the function, or NULL otherwise
 */
FeriteFunction *ferite_find_constructor( FeriteScript *script, FeriteObject *object, FeriteVariable **params )
{
	FeriteFunction *func = NULL;

	FE_ENTER_FUNCTION;
	func = ferite_object_get_function_for_params( script, object, "constructor", params ); 
	FE_LEAVE_FUNCTION( func );
}

/**
 * @function ferite_find_static_constructor
 * @declaration FeriteFunction *ferite_find_static_constructor( FeriteScript *script, FeriteClass *klass )
 * @brief Get a pointer to the first static constructor in a class tree
 * @param FeriteScript *script The script
 * @param FeriteClass *klass The class to start with
 * @return A pointer to the function, or NULL otherwise
 */
FeriteFunction *ferite_find_static_constructor( FeriteScript *script, FeriteClass *klass, FeriteVariable **params )
{
	FeriteFunction *func = NULL;

	FE_ENTER_FUNCTION;
	func = ferite_class_get_function_for_params( script, klass, "constructor", params ); 
	FE_LEAVE_FUNCTION( func );
}

/**
 * @function ferite_class_dup
 * @declaration FeriteClass *ferite_class_dup( FeriteScript *script, FeriteClass *klass, FeriteNamespace *container )
 * @brief Create a complete duplicate of a class
 * @param FeriteScript *script The script
 * @param FeriteClass *klass The class to duplicate
 * @param FeriteNamespace *container A pointer to the namespace that will hold the class
 * @return A pointer to a new FeriteClass structure
 */
FeriteClass *ferite_class_dup( FeriteScript *script, FeriteClass *klass, FeriteNamespace *container )
{
	FeriteClass *ptr = NULL;

	FE_ENTER_FUNCTION;
	if( klass != NULL )
	{
		ptr = fmalloc(sizeof(FeriteClass));
		ptr->name = NULL /*REMOVE fstrdup( klass->name )*/;
		ptr->id = klass->id;
		/* FIXME: how to get the parent class done correctly */
		ptr->parent = NULL;
		ptr->class_vars = ferite_hash_dup( script, klass->class_vars, (void *(*)(FeriteScript *,void *,void*))ferite_duplicate_variable, NULL );
		/*REMOVE ptr->variables = ferite_duplicate_variable_hash( script, klass->variables );
		ptr->object_methods = ferite_hash_dup( script, klass->object_methods, (void *(*)(FeriteScript *,void *,void*))ferite_function_dup, ptr );
		ptr->class_methods = ferite_hash_dup( script, klass->class_methods, (void *(*)(FeriteScript *,void *,void*))ferite_function_dup, ptr );
		ptr->next = NULL;*/
		ptr->container = container;
	}
	FE_LEAVE_FUNCTION(ptr);
}

/**
 * @function ferite_state_to_str 
 * @declaration char *ferite_state_to_str( int state )
 * @brief Get a textual description of a object/class members state value
 * @param int state The state to give back
 * @return A point to a constant string
 */
char *ferite_state_to_str( int state )
{
	FE_ENTER_FUNCTION;   
	switch( state )
	{
	  case FE_ITEM_IS_PRIVATE:
		FE_LEAVE_FUNCTION( "private" );
		break;
	  case FE_ITEM_IS_PROTECTED:
		FE_LEAVE_FUNCTION( "protected" );
		break;
	  case FE_ITEM_IS_PUBLIC:
		FE_LEAVE_FUNCTION( "public" );
		break;
	}
	FE_LEAVE_FUNCTION( "unknown" );
}

/**
 * @function ferite_generate_namespace_fqn
 * @declaration char *ferite_generate_namespace_fqn( FeriteScript *script, FeriteNamespace *ns )
 * @brief Generate a C string containing the full path to a specified namespace
 * @param FeriteScript *script The script context
 * @param FeriteNamespace *ns The namespace whose path is required.
 * @return A c string containing the path
 * @warning It is up to the calling function to free up the memory returned
 */
char *ferite_generate_namespace_fqn( FeriteScript *script, FeriteNamespace *ns )
{
	char *name = NULL;
	
	FE_ENTER_FUNCTION;
	if( ns->name == NULL )
	{
		name = fmalloc( 1024 );
		memset( name, '\0', 1024 );
	}
	else
	{
		name = ferite_generate_namespace_fqn( script, ns->container );
		if( strlen( name ) > 0 )
			strcat( name, "." );
		strcat( name, ns->name );
	}
	FE_LEAVE_FUNCTION( name );
}

/**
* @function ferite_generate_class_fqn
 * @declaration char *ferite_generate_class_fqn( FeriteScript *script, FeriteClass *klass )
 * @brief Generate a C string containing the full path to a specified class
 * @param FeriteScript *script The script context
 * @param FeriteClass *klass The class whose path is required.
 * @return A c string containing the path
 * @warning It is up to the calling function to free up the memory returned
 */
char *ferite_generate_class_fqn( FeriteScript *script, FeriteClass *klass )
{
	char *name = ferite_generate_namespace_fqn( script, klass->container );
	
	FE_ENTER_FUNCTION;
	if( strlen( name ) > 0 )
		strcat( name, "." );
	strcat( name, klass->name );	
	FE_LEAVE_FUNCTION( name );
}

/**
 * @end
 */
