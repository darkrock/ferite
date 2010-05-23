/*
 * Copyright (C) 2001-2007 Chris Ross
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

#include "ferite.h"
#include "thread_header.h"

FeriteScript *ferite_thread_create_script( FeriteScript *script )
{
	FeriteScript *new_script = NULL;
	FeriteScript *real_parent = NULL;
 
	FE_ENTER_FUNCTION;

	real_parent = script;


	new_script = fmalloc( sizeof( FeriteScript ) );
	memcpy( new_script, real_parent, sizeof( FeriteScript ) );

	/* Create a lock for this script */
	new_script->gc_lock = aphex_mutex_recursive_create();

	/* Create new cache for this thread */
	ferite_init_cache( new_script );

	/* Create new GC for this thread */
	new_script->gc = NULL;
	new_script->gc_stack = NULL;

	/* If we are here then we have not got any errors or warnings reset! :) */
	new_script->error = NULL;
	new_script->warning = NULL;
	new_script->stack_level = 0;

	/* Hook up the parent */
	new_script->parent = real_parent;

	FE_LEAVE_FUNCTION(new_script);
}

void ferite_thread_destroy_script( FeriteScript *script, FeriteThread *ctx, int fd )
{
    FeriteThreadGroup *group = NULL;
    FeriteThread *thread = NULL;
    
    FE_ENTER_FUNCTION;

/*	printf( "Destroying script: %p, Thread: %p, fd: %d\n", ctx->script, ctx, fd ); */
	
    /* We need these to make sure we clean up correctly */
    group = script->thread_group;
    thread = ctx;
    
    /* If you want exceptions to be passed on */
    if( ctx->script->error != NULL )
        ferite_reset_errors( ctx->script );

    if( ctx->script->warning != NULL )
        ferite_reset_warnings( ctx->script );

    ferite_free_cache( ctx->script );

    aphex_mutex_destroy( ctx->script->gc_lock );
    ctx->script->gc_lock = NULL;

    aphex_thread_destroy( ctx->ctxt );    
    
    ffree( ctx->script );
    ffree( ctx );

    if( !fd ) {
        /* We don't need to do this if we have been called from the destructor */
        ferite_thread_group_dettach( group->owner, group, thread );
    }
    FE_LEAVE_FUNCTION(NOWT);
}

void *ferite_thread_execute( void *ptr )
{
    FeriteThread *thread = (FeriteThread*)ptr;
    FeriteObject *obj = thread->obj;
    FeriteScript *script = thread->script;
    FeriteFunction *function = NULL;
	
	void *gc;
	void *target;
	
    FE_ENTER_FUNCTION;
	
	script->parent->is_multi_thread = FE_TRUE;
	script->is_multi_thread = FE_TRUE;
	
/*	printf( "RunningThread(before): %p, ctx: %p refcount: %d\n", script, thread, obj->refcount ); */
	
    function = ferite_object_get_function_for_params( script, obj, "run", NULL );
    if( function == NULL )
    {
        ferite_error( script, 0, "No run method found in Thread class!\n" );
        FE_LEAVE_FUNCTION( NULL );
    }

    ferite_thread_group_attach( script, script->thread_group, thread );
    thread->running = FE_TRUE;
	FINCREF(obj);

    ferite_init_gc( script );
    ferite_variable_destroy( script, ferite_call_function( script, obj, NULL, function, NULL ) );
    target = script->thread_group->owner;
    gc = script->gc;
    script->gc = NULL;

/*	printf( "RunningThread(after): %p, ctx: %p refcount: %d\n", script, thread, obj->refcount ); */

	/* If you want exceptions to be passed on */
    if( script->error != NULL )
    {
        if( thread->pass_exceptions )
        {
            char *value = ferite_get_error_string( script );
            ferite_error( script->thread_group->owner, 0, "Exception in thread: (errors will follow this message)\n%s", value );
            ffree( value );
        }
        ferite_reset_errors( script );
    }
    
    /* If we are no longer referenced we need to clean up */
    if( obj->refcount == 1 )
	{
        ferite_thread_destroy_script( thread->script->thread_group->owner, thread, FE_FALSE );    
		obj->odata = NULL;
	}
    else 
    {
        ferite_thread_group_dettach( script, script->thread_group, thread );
        thread->running = FE_FALSE;
    }
	
	FDECREF(obj);
    ferite_merge_gc( target, gc );
    
    FE_LEAVE_FUNCTION( NULL );
}
