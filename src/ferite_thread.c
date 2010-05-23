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

#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

#include <ferite.h>
#include "aphex.h"

/**
 * @group Thread Group
 * @brief This group of functions provides a set of thread safe functions for managing and tracking running threads.
          They are mainly used to make sure that threads can validate the existence of other threads and so that the 
          main process thread of a ferite script wont exit until all running threads have ceased execution.
 */

/**
 * @function ferite_create_thread_group
 * @declaration FeriteThreadGroup *ferite_create_thread_group( FeriteScript *script )
 * @brief Create a thread group structure
 * @param FeriteScript *script The script context
 * @return A pointer to a FeriteThreadGroup structure on success, NULL otherwise
 */
FeriteThreadGroup *ferite_create_thread_group( FeriteScript *script )
{
    FeriteThreadGroup *ptr = NULL;

    FE_ENTER_FUNCTION;
#ifdef THREAD_SAFE
    ptr = fmalloc( sizeof( FeriteThreadGroup ) );
    ptr->lock = aphex_mutex_create();
    ptr->thread_list = ferite_create_stack( NULL, 1 );
    ptr->owner = script;
#endif
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_thread_group_destroy
 * @declaration void ferite_thread_group_destroy( FeriteScript *script, FeriteThreadGroup *group )
 * @brief Destroy a thread group. This function blocks.
 * @param FeriteScript *script The script context
 * @param FeriteThreadGroup *group The thread group to cleanup
 * @warning This function will block the calling thread/process until all threads within the group have dettached from
            the group/
 */
void ferite_thread_group_destroy( FeriteScript *script, FeriteThreadGroup *group )
{
    FE_ENTER_FUNCTION;
#ifdef THREAD_SAFE
    if( group != NULL )
    {
        ferite_thread_group_wait( script, group ); /* we should atleast make sure they have all stopped
                                                    * before we delete */
        ferite_delete_stack( NULL, group->thread_list );
        aphex_mutex_destroy( group->lock );
        ffree( group );
    }
#endif
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_thread_group_attach
 * @declaration void ferite_thread_group_attach( FeriteScript *script, FeriteThreadGroup *group, FeriteThread *thread )
 * @brief Attach a given FeriteThread to a thread group.
 * @param FeriteScript *script The script context
 * @param FeriteThreadGroup *group The thread group to attach to
 * @param FeriteThread *thread The thread to attach
 */
void ferite_thread_group_attach( FeriteScript *script, FeriteThreadGroup *group, FeriteThread *thread )
{
    int i = 0;

    FE_ENTER_FUNCTION;
#ifdef THREAD_SAFE
    if( group != NULL && thread != NULL )
    {
        aphex_mutex_lock( group->lock );

        /* search the thread list for a gap */
        for( i = 1; i <= group->thread_list->stack_ptr; i++ )
        {
            if( group->thread_list->stack[i] == NULL )
            {
                group->thread_list->stack[i] = thread;
                aphex_mutex_unlock( group->lock );
                FE_LEAVE_FUNCTION(NOWT);
            }
        }

        /* we didn't find a spot, put the thread at the end of the list */
        ferite_stack_push( FE_NoScript, group->thread_list, thread );
        aphex_mutex_unlock( group->lock );
    }
#endif
    FE_LEAVE_FUNCTION(NOWT);
}

/**
* @function ferite_thread_group_dettach
 * @declaration void ferite_thread_group_dettach( FeriteScript *script, FeriteThreadGroup *group, FeriteThread *thread )
 * @brief Dettach a given FeriteThread from a thread group.
 * @param FeriteScript *script The script context
 * @param FeriteThreadGroup *group The thread group to dettach from
 * @param FeriteThread *thread The thread to dettach
 */
void ferite_thread_group_dettach( FeriteScript *script, FeriteThreadGroup *group, FeriteThread *thread )
{
    int i = 0;

    FE_ENTER_FUNCTION;
#ifdef THREAD_SAFE
    if( group != NULL && thread != NULL )
    {
        aphex_mutex_lock( group->lock );
        /* search the thread list, remove the thread by setting it to NULL */
        for( i = 1; i <= group->thread_list->stack_ptr; i++ )
        {
            if( group->thread_list->stack[i] == thread )
            {
                group->thread_list->stack[i] = NULL;
                break;
            }
        }
        aphex_mutex_unlock( group->lock );
    }
#endif
    FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_thread_group_wait
 * @declaration void ferite_thread_group_wait( FeriteScript *script, FeriteThreadGroup *group )
 * @brief Block the calling process until all threads within the group have dettached.
 * @param FeriteScript *script The script context
 * @param FeriteThreadGroup *group The group to wait on
 */
void ferite_thread_group_wait( FeriteScript *script, FeriteThreadGroup *group )
{
    int i = 0, keep_going = FE_TRUE;

    FE_ENTER_FUNCTION;
#ifdef THREAD_SAFE
   /* we have to sit in a loop, check to see if group->thread_list is empty
  * if it is, return, if not, block until it is */
    if( group != NULL )
    {
        while( FE_TRUE )
        {
            keep_going = FE_FALSE;
            aphex_mutex_lock( group->lock );
            for( i = 1; i <= group->thread_list->stack_ptr; i++ )
            {
                if( group->thread_list->stack[i] != NULL )
                {
                    keep_going = FE_TRUE;
                    break; /* we dont need to keep looking if we have at least one thread */
                }
            }
            aphex_mutex_unlock( group->lock );
            if( keep_going == FE_FALSE )
              break;
            aphex_thread_sleep( NULL, 100 );
        }
    }
#endif
    FE_LEAVE_FUNCTION(NOWT);
}

/** @end */
