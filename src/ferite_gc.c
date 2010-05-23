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

/* These are left undocumented because there is the abstract gc interface to document */

/*!
 * \fn void ferite_init_std_gc()
 * \brief Initialise the garbage collector
 */
void ferite_init_std_gc( FeriteScript *script )
{
    int i;
    FeriteStdGC *gc = NULL;

    FE_ENTER_FUNCTION;
    if( script->gc == NULL )
    {
        gc = script->gc = fmalloc( sizeof(FeriteStdGC) );
        gc->size = FE_GC_INITIAL_SIZE;
        gc->contents = fmalloc( sizeof(FeriteObject*) * gc->size );
        for( i = 0; i < gc->size; i++ )
			gc->contents[i] = NULL;
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/*!
 * \fn void ferite_deinit_std_gc()
 * \brief Clean the garbage collector up and shut it down
 */
void ferite_deinit_std_gc( FeriteScript *script )
{
    int i = 0;
    FeriteStdGC *gc = NULL;

    FE_ENTER_FUNCTION;
    if( script->gc != NULL )
    {
        gc = script->gc;
        FUD(("GC: +---------------------+\n"));
        FUD(("GC: | CLEANING UP GC      |\n"));
        FUD(("GC: +---------------------+\n"));
        for( i = 0; i < gc->size; i++ )
        {
            if( gc->contents[i] != NULL )
            {
                FUD(("Deleting object %s [%d][%d]\n", gc->contents[i]->name, gc->contents[i]->refcount, i ));
                ferite_delete_class_object( script, gc->contents[i], FE_TRUE );
            }
        }
        ffree( gc->contents );
        ffree( gc );
        script->gc = NULL;
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/*!
 * \fn void ferite_add_to_std_gc( FeriteObject *obj )
 * \brief Add an object to the GC
 * \param obj The FeriteObject to add
 *
 * This function will automaticaly grow the gc if it runs out of space
 */
void ferite_add_to_std_gc( FeriteScript *script, FeriteObject *obj )
{
    int i, j;
    FeriteStdGC *gc = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( script != NULL && script->gc != NULL );
    gc = script->gc;
    for( i = 0; i < gc->size; i++ )
    {
        if( gc->contents[i] == NULL )
        {
            FUD(( "GC: Object \"%s\" (%d)\n", obj->name, i ));
            gc->contents[i] = obj;
            FE_LEAVE_FUNCTION( NOWT );
        }
    }
    ferite_check_std_gc( script ); /* force gc checking if we run put of space */

    FUD(("GC: ERROR: no more space in the gc\n"));
    FUD(("GC: Increasing size\n" ));
    i = gc->size;
    j = gc->size;
    gc->size *= 2;
    gc->contents = frealloc( gc->contents, sizeof( FeriteObject ) * gc->size );
    for( ; j < gc->size; j++ )
      gc->contents[j] = NULL;
    gc->contents[i] = obj;
    FE_LEAVE_FUNCTION( NOWT );
}

/*!
 * \fn void ferite_check_std_gc()
 * \brief Check the GC and free up any objects that have yet to be cleaned up
 */
void ferite_check_std_gc( FeriteScript *script )
{
    int i;
    FeriteStdGC *gc = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( script != NULL && script->gc != NULL );
	script->gc_running = FE_TRUE;

    gc = script->gc;
    FUD(( "GC: Scanning gc for unreferenced objects\n" ));
    for( i = 0; i < gc->size; i++ )
    {
        if( gc->contents[i] != NULL && gc->contents[i]->refcount <= 0 )
        {
            FUD(( "GC: Destroying object \"%s\"\n", gc->contents[i]->name ));
            ferite_delete_class_object( script, gc->contents[i], FE_TRUE );
            gc->contents[i] = NULL;
        }
    }
	script->gc_count = 0;
	
	script->gc_running = FE_FALSE;
    FE_LEAVE_FUNCTION( NOWT );
}
void ferite_merge_std_gc( FeriteScript *script, void *g )
{
    int i;
    FeriteStdGC *gc = g;
 
    FE_ENTER_FUNCTION;
    FE_ASSERT( script != NULL );
    for( i = 0; i < gc->size; i++ )
    {
	if( gc->contents[i] != NULL && gc->contents[i]->refcount <= 0 )
	{
            ferite_delete_class_object( script, gc->contents[i], FE_TRUE );
	}
	else if( gc->contents[i] )
	{
	    ferite_add_to_std_gc( script, gc->contents[i] );
	}
    }
    FE_LEAVE_FUNCTION( NOWT );
}
