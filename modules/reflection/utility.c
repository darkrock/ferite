#include <ferite.h>
#include "reflection_header.h"

void reflection_variable_get( FeriteScript *script, FeriteVariable *var )
{
    FeriteObject *object = var->accessors->odata;
    FeriteFunction *function = ferite_object_get_function( script, object, "get" );
    FeriteVariable *rval = NULL;
    void *a = var->accessors;

    FE_ENTER_FUNCTION;

    if( function != NULL )
    {
        rval = ferite_call_function( script, object, NULL, function, NULL );

        if( F_VAR_TYPE(rval) != F_VAR_VOID )
        {
            var->accessors = NULL;
            ferite_variable_fast_assign( script, var, rval );
            var->accessors = a;
        }

        ferite_variable_destroy( script, rval );
    }

    FE_LEAVE_FUNCTION(NOWT);
}

void reflection_variable_set( FeriteScript *script, FeriteVariable *var, FeriteVariable *newvalue )
{
    FeriteObject *object = var->accessors->odata;
    FeriteFunction *function = ferite_object_get_function( script, object, "set" );
    FeriteVariable **plist = ferite_create_parameter_list( script, 4 );
    FeriteVariable *rval = NULL;
    void *a = var->accessors;

    FE_ENTER_FUNCTION;

    if( function != NULL )
    {
        plist = ferite_add_to_parameter_list( plist, ferite_duplicate_variable( script, newvalue, NULL ) );
        MARK_VARIABLE_AS_DISPOSABLE( plist[0] );
        rval = ferite_call_function( script, object, NULL, function, plist );

        if( F_VAR_TYPE(rval) != F_VAR_VOID )
        {
            var->accessors = NULL;
            ferite_variable_fast_assign( script, var, rval );
            var->accessors = a;
        }

        ferite_variable_destroy( script, rval );
    }
    ferite_delete_parameter_list( script, plist );
    FE_LEAVE_FUNCTION(NOWT);
}

void reflection_variable_cleanup( FeriteScript *script, void *odata )
{
    FeriteObject *object = odata;
    FeriteFunction *function = ferite_object_get_function( script, object, "cleanup" );
    FeriteVariable *rval = NULL;

    FE_ENTER_FUNCTION;

    if( function != NULL )
    {
        rval = ferite_call_function( script, object, NULL, function, NULL );
        ferite_variable_destroy( script, rval );
    }
	FDECREF(object);
    FE_LEAVE_FUNCTION(NOWT);
}

FeriteVariable *reflection_ns_get_list( FeriteScript *script, FeriteNamespace *space, int type )
{
    FeriteHashBucket *buk = NULL;
    FeriteVariable *array = NULL, *var = NULL;
    FeriteIterator *iter = NULL;
    FeriteFunction *func = NULL;
    
    FE_ENTER_FUNCTION;
    array = ferite_create_uarray_variable( script, "NS:getFunctions", space->code_fork->size, FE_STATIC);
    iter = ferite_create_iterator(script);
    while((buk = (FeriteHashBucket*)ferite_hash_walk(script,space->code_fork,iter)) != NULL)
    {
        FeriteNamespaceBucket *b = buk->data;
        if( F_VAR_TYPE(b) == type )
        {
            func = b->data;
            var = fe_new_str(buk->id,func->name, 0, FE_CHARSET_DEFAULT);
            ferite_uarray_add(script,VAUA(array),var,NULL,FE_ARRAY_ADD_AT_END);
        }
    }
    ffree(iter);
    FE_LEAVE_FUNCTION(array);
}
