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

/**
 * @group Namespaces
 * @description Namespaces provide a significant portion of the structure of a compiled ferite 
 *              script. There is always the main namespace within each script where global variables,
 *              top level functions, classes and namespaces are stored.
 */

/**
 * @function ferite_register_namespace
 * @declaration FeriteNamespace *ferite_register_namespace( FeriteScript *script, char *name, FeriteNamespace *parent )
 * @brief Create a new namespace and place it in the given namespace
 * @param FeriteScript *script The script
 * @param char *name The name of the new namespace
 * @param FeriteNamespace *parent The namespace in which to put the new namespace
 * @return The newly created namespace
 */
FeriteNamespace *ferite_register_namespace( FeriteScript *script, char *name, FeriteNamespace *parent )
{
    FeriteNamespace *ns = NULL;

    FE_ENTER_FUNCTION;
    ns = fmalloc( sizeof( FeriteNamespace ) );
    ns->name = fstrdup( name );
    ns->num = FE_NAMESPACE_INIT_SIZE;
    ns->code_fork_ref = fmalloc( sizeof( int ) );
    *(ns->code_fork_ref) = 1;
    ns->data_fork = ferite_create_hash( script, FE_NAMESPACE_INIT_SIZE );
    ns->code_fork = ferite_create_hash( script, FE_NAMESPACE_INIT_SIZE );
    if( parent != NULL )
    {
        ns->container = parent;
        ferite_register_namespace_element( script, parent, name, FENS_NS, ns );
    }
    else
        ns->container = NULL;
    FE_LEAVE_FUNCTION( ns );
}

/**
 * @function ferite_register_ns_function
 * @declaration FeriteFunction *ferite_register_ns_function( FeriteScript *script, FeriteNamespace *ns, FeriteFunction *f )
 * @brief Register a function within the namespace
 * @param FeriteScript *script The script
 * @param FeriteNamespace *ns The namespace in which to register the function
 * @param FeriteFunction *f The function to register
 * @return It simply returns the function that has been registered
 */
FeriteFunction *ferite_register_ns_function( FeriteScript *script, FeriteNamespace *ns, FeriteFunction *f )
{
	FeriteFunction *function = NULL, *ptr = NULL;
	char *new_function_sig = NULL;
	int may_add = FE_TRUE;
	
	FE_ENTER_FUNCTION;
	if( ns != NULL )
	{
		FeriteNamespaceBucket *nsb = NULL;
		if( (nsb = ferite_namespace_element_exists( script, ns, f->name )) != NULL ) {
			function = nsb->data;
			if( function ) {
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
			} else {
				nsb->data = f;
				may_add = FE_TRUE;
			}
		}
		else
			ferite_register_namespace_element( script, ns, f->name, FENS_FNC, f );

		FE_LEAVE_FUNCTION( (may_add ? f : NULL) );
	}
	FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_register_ns_variable
 * @declaration FeriteVariable *ferite_register_ns_variable( FeriteScript *script, FeriteNamespace *ns, FeriteVariable *var )
 * @brief Register a variable within a namespace
 * @param FeriteScript *script The script
 * @param FeriteNamespace *ns The namespace in which to register the variable
 * @param char *name The name in the namespace that will reference the variable
 * @param FeriteVariable *v The variable to register
 * @return It simply returns the variable that has been registered
 */
FeriteVariable *ferite_register_ns_variable( FeriteScript *script, FeriteNamespace *ns, char *name, FeriteVariable *var )
{
    FE_ENTER_FUNCTION;
    FE_ASSERT( ns != NULL && var != NULL );
    FUD(("NS: Adding variable %p\n", var));
    UNMARK_VARIABLE_AS_DISPOSABLE(var);
    ferite_register_namespace_element( script, ns, name, FENS_VAR, var );
    FE_LEAVE_FUNCTION( var );
}

/**
 * @function ferite_register_ns_class
 * @declaration FeriteClass *ferite_register_ns_class( FeriteScript *script, FeriteNamespace *ns, FeriteClass *classt )
 * @brief Register the class within the namespace
 * @param FeriteScript *script The script
 * @param FeriteNamespace *ns The namespace in which to reigster the class
 * @param FeriteClass *classt The class to register
 * @return It simply returns the class that has been registered
 */
FeriteClass *ferite_register_ns_class( FeriteScript *script, FeriteNamespace *ns, FeriteClass *classt )
{
    FE_ENTER_FUNCTION;
    FE_ASSERT( ns != NULL && classt != NULL );
    ferite_register_namespace_element( script, ns, classt->name, FENS_CLS, classt );
    FE_LEAVE_FUNCTION( classt );
}

void ferite_delete_namespace_element( FeriteScript *script, FeriteNamespaceBucket *nsb )
{
    FE_ENTER_FUNCTION;
    FUD(("NS: deleting namespace element %d[%p]\n", nsb->type, nsb));
    if( nsb->data != NULL )
    {
        switch( nsb->type )
        {
          case FENS_NS:
            ferite_delete_namespace( script, nsb->data );
            break;
          case FENS_VAR:
            ferite_variable_destroy( script, nsb->data );
            break;
          case FENS_FNC:
            ferite_delete_function_list( script, nsb->data );
            break;
          case FENS_CLS:
            ferite_delete_class( script, nsb->data );
            break;
          default:
            ferite_warning( script, "Trying to delete element of type '%d' from a namespace - Unknown Type", nsb->type );
        }
    }
    ffree( nsb );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_delete_namespace
 * @declaration int ferite_delete_namespace( FeriteScript *script, FeriteNamespace *ns )
 * @brief Completely free up all elements within the namespace
 * @param FeriteScript *script The script
 * @param FeriteNamespace *ns The namespace to delete
 * @return Always 1
 */
int ferite_delete_namespace( FeriteScript *script, FeriteNamespace *ns )
{
    FE_ENTER_FUNCTION;
    FE_ASSERT( ns != NULL );
    
	FUD(("Deleting namespace contents: %s (%s) [%p]\n", ferite_generate_namespace_fqn( script, ns ), ns->name, ns));
	
	ferite_delete_hash( script, ns->data_fork, (void (*)(FeriteScript*,void *))ferite_delete_namespace_element );
    
	/* Only delete the code fork if the ref count goes to zero */
	*(ns->code_fork_ref) -= 1;
	if( *(ns->code_fork_ref) == 0 )
	{
		ferite_delete_hash( script, ns->code_fork, (void (*)(FeriteScript*,void *))ferite_delete_namespace_element );
		ffree( ns->code_fork_ref );
	}
	
	if( ns->name != NULL )
        ffree( ns->name );
    ffree( ns );
    FE_LEAVE_FUNCTION( 1 );
}

FeriteNamespaceBucket *ferite_register_namespace_element( FeriteScript *script, FeriteNamespace *ns, char *name, int type, void *data )
{
    FeriteNamespaceBucket *nsb = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( ns != NULL && name != NULL );
    FE_ASSERT( type > 0 && data != NULL );
    
    nsb = fmalloc( sizeof( FeriteNamespaceBucket ) );
    nsb->type = type;
    nsb->data = data;
    
    if( type == FENS_VAR )
        ferite_hash_add( script, ns->data_fork, name, nsb );
    else
        ferite_hash_add( script, ns->code_fork, name, nsb );
    
    FE_LEAVE_FUNCTION( nsb );
}

/**
 * @function ferite_namespace_element_exists
 * @declaration FeriteNamespaceBucket *ferite_namespace_element_exists( FeriteScript *script, FeriteNamespace *ns, char *name )
 * @brief See if an element exists within the namespace (not recursive)
 * @param FeriteScript *script The script
 * @param FeriteNamespace *ns The namespace to look into
 * @param char *name The name of the element to look for
 * @return The FeriteNamespaceBucket for the name or NULL if it does not exist
 */
FeriteNamespaceBucket *ferite_namespace_element_exists( FeriteScript *script, FeriteNamespace *ns, char *name )
{
    FeriteNamespaceBucket *nsb = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( ns != NULL && name != NULL );
    nsb = ferite_hash_get( script, ns->code_fork, name );
	if( nsb == NULL )
		nsb = ferite_hash_get( script, ns->data_fork, name );
    FE_LEAVE_FUNCTION( nsb );
}

/**
 * @function ferite_delete_namespace_element_from_namespace
 * @declaration int ferite_delete_namespace_element_from_namespace( FeriteScript *script, FeriteNamespace *ns, char *name )
 * @brief Delete an element from the namespace
 * @param FeriteScript *script The script
 * @param FeriteNamespace *ns The namespace to look into
 * @param char *name The name of the element to delete
 * @return FE_TRUE if an element is deleted, FE_FALSE otherwise
 */
int ferite_delete_namespace_element_from_namespace( FeriteScript *script, FeriteNamespace *ns, char *name )
{
    FeriteNamespaceBucket *nsb = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( ns != NULL && name != NULL );
    nsb = ferite_namespace_element_exists( script, ns, name );
    if( nsb != NULL )
    {
		if( nsb->type == FENS_VAR )
			ferite_hash_delete( script, ns->data_fork, name );
		else
			ferite_hash_delete( script, ns->code_fork, name );
        ferite_delete_namespace_element( script, nsb );
        FE_LEAVE_FUNCTION(FE_TRUE);
    }
    FE_LEAVE_FUNCTION(FE_FALSE);
}

/**
 * @function ferite_rename_namespace_element
 * @declaration int ferite_rename_namespace_element( FeriteScript *script, FeriteNamespace *ns, char *from, char *to )
 * @brief Delete an element from the namespace
 * @param FeriteScript *script The script
 * @param FeriteNamespace *ns The namespace to look into
 * @param char *from The name of the element to rename 
 * @param char *to The name to rename to
 * @return FE_TRUE if an element is deleted, FE_FALSE otherwise
 */
int ferite_rename_namespace_element( FeriteScript *script, FeriteNamespace *ns, char *from, char *to )
{
    FeriteNamespaceBucket *nsb = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( ns != NULL && from != NULL );
    nsb = ferite_hash_get( script, ns->data_fork, from );
    if( nsb != NULL )
    {
        ferite_hash_delete( script, ns->data_fork, from );
        ferite_hash_add( script, ns->data_fork, to, nsb );
        FE_LEAVE_FUNCTION(1);
    }
    else
    {
        nsb = ferite_hash_get( script, ns->code_fork, from );
        if( nsb != NULL )
        {
            switch( nsb->type ) 
            {
                case FENS_FNC:
                    ffree( ((FeriteFunction*)nsb->data)->name );
                    ((FeriteFunction*)nsb->data)->name = fstrdup(to);
                    break;
                case FENS_CLS:
                    ffree( ((FeriteClass*)nsb->data)->name );
                    ((FeriteClass*)nsb->data)->name = fstrdup(to);
                    break;
            }
            ferite_hash_delete( script, ns->code_fork, from );
            ferite_hash_add( script, ns->code_fork, to, nsb );
            FE_LEAVE_FUNCTION(1);
        }
    }
    FE_LEAVE_FUNCTION(0);
}

/**
 * @function ferite_find_namespace
 * @declaration FeriteNamespaceBucket *ferite_find_namespace( FeriteScript *script, FeriteNamespace *parent, char *obj, int type )
 * @brief Recusively look in a namespace for an element
 * @param FeriteScript *script The script
 * @param FeriteNamespace *parent The namespace to look in
 * @param char *obj The name to locate
 * @param int type The type to look for
 * @return The namespace bucket if it is found otherwise NULL
 * @description The type dictates what is returned. No type is specified (0) then if any element is
 *              found then it will be returned. If a type is specified then it is checked to see if
 *              the bucket is of the correct type, if so it is returned, if not NULL is returned.<nl/>
 *              <nl/>
 *              The types are FENS_VAR, FENS_FNC, FENS_CLS, or FENS_NS.
 */
FeriteNamespaceBucket *ferite_find_namespace( FeriteScript *script, FeriteNamespace *parent, char *obj, int type )
{
    FeriteNamespaceBucket *nsb = NULL;
    char *buf;
    int i = 0;

    FE_ENTER_FUNCTION;
    FE_ASSERT( parent != NULL && obj != NULL );

    if( ferite_find_string( obj, "." ) == -1 )
    { 
        /* we need not split the string up */
        nsb = ferite_namespace_element_exists( script, parent, obj );
        if( type > 0 )
        {
            if( nsb == NULL || nsb->type != type )
            {
                FE_LEAVE_FUNCTION( NULL );
            }
        }
        FE_LEAVE_FUNCTION( nsb );
    }
    else
    { 
        /* we need to split the string up */
        buf = memset( fmalloc( strlen(obj)+1 ), '\0', strlen(obj) );
        for( i = 0; obj[i] != '.'; i++ );
        strncpy( buf, obj, i );
        nsb = ferite_namespace_element_exists( script, parent, buf );
        ffree( buf );
        if( nsb == NULL || nsb->type != FENS_NS )
        {
            FE_LEAVE_FUNCTION( NULL );
        }
        if( type == FENS_PARENT_NS && ferite_find_string( obj+i+1, "." ) == -1 )
        {
            FE_LEAVE_FUNCTION( nsb );
        }
        FE_LEAVE_FUNCTION( ferite_find_namespace( script, nsb->data, obj+i+1, type ) );
    }
    FE_LEAVE_FUNCTION( NULL );
}
/**
* @function ferite_find_namespace_element_contents
 * @declaration void *ferite_find_namespace_element_contents( FeriteScript *script, FeriteNamespace *parent, char *obj, int type )
 * @brief Recusively look in a namespace for an element and it's contents
 * @param FeriteScript *script The script
 * @param FeriteNamespace *parent The namespace to look in
 * @param char *obj The name to locate
 * @param int type The type to look for
 * @return The contents of the namespace bucket if it is found otherwise NULL
 * @description The type dictates what is returned. No type is specified (0) then if any element is
 *              found then it will be returned. If a type is specified then it is checked to see if
 *              the bucket is of the correct type, if so it is returned, if not NULL is returned.<nl/>
 *              <nl/>
 *              The types are FENS_VAR, FENS_FNC, FENS_CLS, or FENS_NS.
 */
void *ferite_find_namespace_element_contents( FeriteScript *script, FeriteNamespace *parent, char *obj, int type )
{
    FeriteNamespaceBucket *nsb = ferite_find_namespace( script, parent, obj, type );
    if( nsb != NULL )
        return nsb->data;
    return NULL;
}
    
/**
 * @function ferite_namespace_dup
 * @declaration FeriteNamespace *ferite_namespace_dup( FeriteScript *script, FeriteNamespace *ns, FeriteNamespace *container )
 * @brief Duplicate a namespace
 * @param FeriteScript *script The script
 * @param FeriteNamespace *ns The namespace to duplicate
 * @param FeriteNamespace *container The container of the new namespace
 * @return The new namespace
 */
FeriteNamespace *ferite_namespace_dup( FeriteScript *script, FeriteNamespace *ns, FeriteNamespace *container )
{
    FeriteNamespace *ptr = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( ns != NULL );
    ptr = fmalloc(sizeof(FeriteNamespace));
    ptr->num = ns->num;
    ptr->data_fork = ferite_hash_dup( script, ns->data_fork, (void *(*)(FeriteScript *,void *,void*))ferite_namespace_bucket_dup, ptr );
    
    /* Use the same code fork reference */
    ptr->code_fork = ns->code_fork;
    ptr->code_fork_ref = ns->code_fork_ref;
    *(ptr->code_fork_ref) += 1;
    
    ptr->container = container;
    if( ns->name != NULL )
        ptr->name = fstrdup( ns->name );
    else
        ptr->name = NULL;
    FE_LEAVE_FUNCTION(ptr);
}

FeriteNamespaceBucket *ferite_namespace_bucket_dup( FeriteScript *script, FeriteNamespaceBucket *nsb, FeriteNamespace *parent )
{
    FeriteNamespaceBucket *ptr = NULL;
    FeriteClass *klass = NULL, *klass_dup = NULL;
    //FeriteStack *stack = script->odata;
    
    FE_ENTER_FUNCTION;
    FE_ASSERT( nsb != NULL );
    ptr = fmalloc(sizeof(FeriteNamespaceBucket));
    ptr->type = nsb->type;
    switch( nsb->type )
    {
/*REMOVE  case FENS_NS:
        ptr->data = ferite_namespace_dup( script, nsb->data, parent );
        break; */
      case FENS_VAR:
        ptr->data = ferite_duplicate_variable( script, nsb->data, NULL );
        break;
/*REMOVE    case FENS_FNC:
        ptr->data = ferite_function_dup( script, nsb->data, NULL );
        break; */
      case FENS_CLS:
        ptr->data = ferite_class_dup( script, nsb->data, parent );
/*REMOVE        klass = ptr->data;
        klass_dup = nsb->data;
        if( klass_dup->parent != NULL )
        {
            klass->parent = (void *)ferite_class_full_name( script, klass_dup->parent );
            ferite_stack_push( stack, klass );
        }*/
        break; 
      default:
        ferite_warning( script, "Trying to duplicate element of type '%d' from a namespace - Unknown Type", nsb->type );
    }
    FE_LEAVE_FUNCTION(ptr);
}

/**
 * @function ferite_namespace_bucket_type_to_str
 * @declaration char *ferite_namespace_bucket_type_to_str( FeriteScript *script, int type )
 * @brief Take the type of a namespace bucket and convert it to a string
 * @param FeriteScript *script The script context
 * @param int type The bucket type
 * @return A string containing the contents, you do not need to free this string
 */
char *ferite_namespace_bucket_type_to_str( FeriteScript *script, int type )
{
    char *retval = NULL;
    FE_ENTER_FUNCTION;
    switch( type )
    {
        case FENS_CLS:
            retval = "class";
            break;
        case FENS_VAR:
            retval = "variable";
            break;
        case FENS_NS:
            retval = "namespace";
            break;
        case FENS_FNC:
            retval = "function";
            break;
    }
    FE_LEAVE_FUNCTION(retval);
}

FE_NATIVE_FUNCTION( ferite_namespace_item_rename )
{
    char *from = VAS(params[0])->data;
	char *to = VAS(params[1])->data;
	FeriteNamespace *self = FE_CONTAINER_TO_NS;
	FeriteVariable *ptr = fe_new_void_static("");
	
    FE_ENTER_FUNCTION;
    if( !ferite_rename_namespace_element( script, self, from, to ) )
    {
        ferite_warning( script, "Unable to find attribute '%s' to rename in namespace '%s'!\n", from, self->name );
    }
    FE_LEAVE_FUNCTION(ptr);
}

/**
 * @end
 */
