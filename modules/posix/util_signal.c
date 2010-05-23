/*
 * Copyright (C) 2001-2007 Chris Ross, Stephan Engstrom, Alex Holden et al
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

#include "util_signal.h"
#include "../../libs/aphex/include/aphex.h"

static FeriteStack *ferite_signal_list = NULL;
static void *ferite_signal_lock = NULL;

FeriteSysSignal *ferite_signal_create( FeriteScript *script, FeriteObject *object, FeriteFunction *function, int sig )
{
    FeriteSysSignal *ptr = NULL;

    FE_ENTER_FUNCTION;
    ptr = fmalloc( sizeof( FeriteSysSignal ) );
    ptr->script = script;
    ptr->object = object;
    ptr->function = function;
    ptr->sig = sig;
    FE_LEAVE_FUNCTION( ptr );
}

void ferite_signal_init()
{
    FE_ENTER_FUNCTION;
    ferite_signal_list = ferite_create_stack( NULL, 5 );
    ferite_signal_lock = aphex_mutex_create();
    FE_LEAVE_FUNCTION(NOWT);
}

void ferite_signal_deinit()
{
    FeriteSysSignal *sig = NULL;
    int i = 0;

    FE_ENTER_FUNCTION;
    if( ferite_signal_list != NULL )
    {
        for( i = 1; i <= ferite_signal_list->stack_ptr; i++ )
        {
            sig = ferite_signal_list->stack[i];
            if( sig != NULL )
            {
                if( sig->object != NULL ) {
					FDECREF(sig->object);
				}
                ffree_ngc( sig );
                ferite_signal_list->stack[i] = NULL;
            }
        }
        ferite_delete_stack( NULL, ferite_signal_list );
        ferite_signal_list = NULL;

        aphex_mutex_destroy( ferite_signal_lock );
        ferite_signal_lock = NULL;
    }
    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_signal_remove_script( FeriteScript *script )
{
    FeriteSysSignal *sig = NULL;
    int i = 0;

    FE_ENTER_FUNCTION;

    aphex_mutex_lock( ferite_signal_lock );
    for( i = 1; i <= ferite_signal_list->stack_ptr; i++ )
    {
        sig = ferite_signal_list->stack[i];
        if( sig != NULL && sig->script == script )
        {
            if( sig->object != NULL ) {
				FDECREF(sig->object);
			}
            ffree( sig );
            ferite_signal_list->stack[i] = NULL;
        }
    }
    aphex_mutex_unlock( ferite_signal_lock );

    FE_LEAVE_FUNCTION(NOWT);
}

int ferite_signal_register_handler( FeriteScript *script, FeriteObject *object, FeriteFunction *function, int s )
{
    FeriteSysSignal *sig = NULL;
    int i = 0, index = 0, done = FE_FALSE;

    FE_ENTER_FUNCTION;
    if( function != NULL )
    {
        aphex_mutex_lock( ferite_signal_lock );
      /*
       * We need to find the first availible place. We also need to check that another handler
       * for this script and signal are present. If they do we replace the current handler. If
       * it doesn't we insert one in
       */
        for( i = 1; i <= ferite_signal_list->stack_ptr; i++ )
        {
            if( ferite_signal_list->stack[i] == NULL && index == 0 )
              index = i;
            else
            {
                sig = ferite_signal_list->stack[i];
                if( sig->script == script && sig->sig == s ) /* We have got an existing handler
                                                              * so we simply replace it */
                {
                    sig->object = object;
                    sig->function = function;
                    done = FE_TRUE;
                    break;
                }
            }
        }

        if( done == FE_FALSE ) /* we are here and we havent sorted out the signal handler
                                * in our list */
        {
            sig = ferite_signal_create( script, object, function, s );
            if( index > 0 )
              ferite_signal_list->stack[index] = sig;
            else
              ferite_stack_push( script, ferite_signal_list, sig );
        }
        aphex_mutex_unlock( ferite_signal_lock );
    }
    FE_LEAVE_FUNCTION( FE_TRUE );
}

int ferite_signal_register_object_handler( FeriteScript *script, FeriteObject *object, char *name, int s )
{
    FeriteFunction *function = NULL;

    FE_ENTER_FUNCTION;
    if( script != NULL && object != NULL && name != NULL )
    {
        function = ferite_object_get_function( script, object, name );
        if( function != NULL )
        {
            FINCREF(object); /* we increase the refcount so our object doesnt get free'd up */
            ferite_signal_register_handler( script, object, function, s );
            FE_LEAVE_FUNCTION( FE_TRUE );
        }
    }
    FE_LEAVE_FUNCTION( FE_FALSE );
}

int ferite_signal_register_function_handler( FeriteScript *script, char *name, int s )
{
    FeriteNamespaceBucket *nsb = NULL;

    FE_ENTER_FUNCTION;
    if( script != NULL && name != NULL )
    {
        nsb = ferite_find_namespace( script, script->mainns, name, FENS_FNC );
        if( nsb != NULL )
        {
            ferite_signal_register_handler( script, NULL, nsb->data, s );
            FE_LEAVE_FUNCTION( FE_TRUE );
        }
    }
    FE_LEAVE_FUNCTION( FE_FALSE );
}

int ferite_signal_unregister_handler( FeriteScript *script, int s )
{
    FeriteSysSignal *sig = NULL;
    int i = 0;

    FE_ENTER_FUNCTION;

    aphex_mutex_lock( ferite_signal_lock );
    for( i = 1; i <= ferite_signal_list->stack_ptr; i++ )
    {
        sig = ferite_signal_list->stack[i];
        if( sig != NULL && sig->script == script && sig->sig == s )
        {
            if( sig->object != NULL ) {
				FDECREF(sig->object);
			}
            ffree( sig );
            ferite_signal_list->stack[i] = NULL;
            break;
        }
    }
    aphex_mutex_unlock( ferite_signal_lock );

    FE_LEAVE_FUNCTION(FE_TRUE);
}

void ferite_signal_handler( int s )
{
    FeriteSysSignal *sig = NULL;
    FeriteVariable **params = NULL;
    FeriteVariable *retval = NULL;
    int i = 0;

    FE_ENTER_FUNCTION;
    aphex_mutex_lock( ferite_signal_lock );

    for( i = 1; i <= ferite_signal_list->stack_ptr; i++ )
    {
        sig = ferite_signal_list->stack[i];
        if( sig != NULL && sig->sig == s )
        {
            params = ferite_create_parameter_list( sig->script, 4 );
            params[0] = ferite_create_number_long_variable( sig->script, "ferite_signal_handler", s, FE_STATIC );
            MARK_VARIABLE_AS_DISPOSABLE( params[0] );

            retval = ferite_call_function( sig->script, (sig->object != NULL ? (void*)sig->object : (void*)sig->script->mainns), NULL, sig->function, params );
            if( retval != NULL )
              ferite_variable_destroy( sig->script, retval );
            ferite_delete_parameter_list( sig->script, params );
        }
    }

    aphex_mutex_unlock( ferite_signal_lock );
    FE_LEAVE_FUNCTION(NOWT);
}
