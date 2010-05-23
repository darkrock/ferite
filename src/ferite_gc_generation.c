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

/* This is left undocumented as this is controlled by the abstract interface */

#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

#include "ferite.h"
#include <sys/timeb.h>
#include "aphex.h"

#ifdef THREAD_SAFE
# define LOCK_GC     aphex_mutex_lock( script->gc_lock )
# define UNLOCK_GC   aphex_mutex_unlock( script->gc_lock )
#else
# define LOCK_GC
# define UNLOCK_GC
#endif

#define FE_GENERATION_GC_DEFAULT_SIZE 32

FeriteGCGeneration *ferite_generation_create()
{
    FeriteGCGeneration *ptr = NULL;
    
    FE_ENTER_FUNCTION;
    ptr = fmalloc_ngc(sizeof(FeriteGCGeneration));
    ptr->size = FE_GENERATION_GC_DEFAULT_SIZE;
    ptr->next_free = 0;
    ptr->contents = fcalloc_ngc(sizeof(FeriteObject*)*ptr->size,1);
    ptr->younger = NULL;
    ptr->older = NULL;
    FE_LEAVE_FUNCTION(ptr);
}

void ferite_generation_destroy( FeriteScript *script, FeriteGCGeneration *g )
{
    int i = 0;
    
    FE_ENTER_FUNCTION;
    if( g != NULL )
    {
        if( g->older != NULL )
            ferite_generation_destroy( script, g->older );
        for( i = 0; i < g->next_free; i++ ) 
		{
            if( g->contents[i] != NULL )
                ferite_delete_class_object( script, g->contents[i], FE_TRUE );
		}
        if( g->contents != NULL )
			ffree( g->contents );
        ffree( g );
    }
    FE_LEAVE_FUNCTION(NOWT);
}

/*!
* \fn void ferite_init_gc()
 * \brief Initialise the garbage collector
 */
void ferite_init_generation_gc( FeriteScript *script )
{
    FE_ENTER_FUNCTION;
    if( script->gc == NULL )
        script->gc = ferite_generation_create();
    FE_LEAVE_FUNCTION( NOWT );
}

/*!
* \fn void ferite_deinit_generation_gc()
 * \brief Clean the garbage collector up and shut it down
 */
void ferite_deinit_generation_gc( FeriteScript *script )
{
    FE_ENTER_FUNCTION;
    if( script->gc != NULL )
    {
        FUD(("GC: +-----------------------------+\n"));
        FUD(("GC: | CLEANING UP Generational GC |\n"));
        FUD(("GC: +-----------------------------+\n"));
        ferite_generation_destroy( script, script->gc );
		script->gc = NULL;
    }
    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_add_to_generation_gc_unlocked( FeriteScript *script, FeriteObject *obj )
{
    FeriteGCGeneration *g = script->gc;
    FE_ENTER_FUNCTION;
    if( g != NULL )
    {
        if( g->next_free >= g->size )
        { 
            /* we have no more space in this generation */
            ferite_check_gc_generation( script, g );
        }
        /* add the object */
        g->contents[g->next_free++] = obj;
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/*!
* \fn void ferite_add_to_generation_gc( FeriteObject *obj )
 * \brief Add an object to the GC
 * \param obj The FeriteObject to add
 *
 * This function will automaticaly grow the gc if it runs out of space
 */
int ferite_object_in_gc( FeriteScript *script, FeriteGCGeneration *g, FeriteObject *obj ) {
	int i = 0;
	
	if( g == NULL )
		return FE_FALSE;
	for( i = 0; i < g->next_free; i++ ) {
		if( g->contents[i] == obj )
			return FE_TRUE;
	}	
	return ferite_object_in_gc( script, g->older, obj );
}

void ferite_add_to_generation_gc( FeriteScript *script, FeriteObject *obj )
{
    FE_ENTER_FUNCTION;
    LOCK_GC;    
#ifdef DEBUG
	if( ferite_object_in_gc( script, script->gc, obj ) ) {
		printf( "Already added to the gc: %p\n", obj );
		ferite_debug_catch( obj, obj->refcount );
	}
#endif
    ferite_add_to_generation_gc_unlocked( script, obj );
    UNLOCK_GC;    
    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_check_generation_gc( FeriteScript *script )
{
    FE_ENTER_FUNCTION;
    LOCK_GC;
	if( script->gc_running ) {
		fprintf( stderr, "ferite_check_generation_gc(): Recursive gc call - this is not good.\n" );
		ferite_debug_catch( NULL, 0 );
	}
	script->gc_running = FE_TRUE;
    ferite_check_gc_generation( script, script->gc );
	script->gc_running = FE_FALSE;
    UNLOCK_GC;    
    FE_LEAVE_FUNCTION(NOWT);
}

/*!
* \fn void ferite_gc_check_generation()
 * \brief Check the GC and free up any objects that have yet to be cleaned up
 */
void ferite_check_gc_generation( FeriteScript *script, FeriteGCGeneration *g )
{
    int i = 0;
    FeriteGCGeneration *o = NULL;
    
    FE_ENTER_FUNCTION;
    FE_ASSERT( script != NULL && script->gc != NULL );
    
	FUD(( "g->start: %d\n", script->gc_count ));
    for( i = 0; i < g->next_free; i++ )
    {
		FeriteObject *object = g->contents[i];
		g->contents[i] = NULL; /* We have to dead the object as soon as possible to make sure 
			                      that if we get double checks and object doesn't get double free'd */

		FUD(( "g->contents[%i]->refcount = %d [%s]\n", i, (!object ? 0 : object->refcount), (!object ? "" : object->klass->name )));
        if( object ) {
			if( object->refcount <= 0 ) {
	            ferite_delete_class_object( script, object, FE_TRUE );
	        } 
			else /* the object isnt' ready to free'd */
	        {
	            o = g->older;
	            if( o != NULL )
	            {
	                if( o->next_free >= o->size )
	                {
	                    /*
	                     * not enough space in the older generation so we check that
	                     * generation for objects that can be deleted
	                     */
	                    /*printf( "not enough space in older, checking it\n" );*/
	                    ferite_check_gc_generation( script, o );
	                }
	            }
	            else /* we dont have an older generation so create a new one */
	            {
	                /*printf( "older == NULL\n" );*/
	                o = ferite_generation_create();
					FUD(("Creating a new generation [%p]\n", script ));
	                /*printf( "o=%p, o->contents=%p\n", o, o->contents );*/
	                g->older = o;
	                /*printf( "g->older=%p\n", g->older );*/
	                o->younger = g;
	                /*printf( "o->younger=%p\n", o->younger );*/
	            }
	            /* put the object in the next generation */
	            o->contents[o->next_free++] = object;
	        }
		}
    }
	FUD(( "g->end\n" ));

    /* by the time we have got here we should have an empty generation, so
        * we just set the next_free to 0 */
    g->next_free = 0;
    
    /* If the older generation is empty and is the top generation */
    if( g->older != NULL && g->older->older == NULL && g->older->next_free == 0 )
    {
        /*printf( "destroying older generation\n" );*/
#if 0
		fprintf( stderr, "Deleting older generation [%p]\n", script );
#endif
        ferite_generation_destroy( script, g->older );
        g->older = NULL;
    }
	
	script->gc_count = 0;
    FE_LEAVE_FUNCTION( NOWT );
}

/*!
* \fn void ferite_gc_generation_merge()
 * \brief Merge dead threads GC to parents GC, clear up all we can first
 * */
void ferite_merge_generation_gc( FeriteScript *script, void *g )
{
    int i = 0;
#if 0
	int count = 0;
#endif
    FeriteGCGeneration *lowerBound = g;
    FeriteGCGeneration *upperBound = NULL;
    FeriteGCGeneration *gc = NULL;

    LOCK_GC;
    for( gc = lowerBound; gc != NULL; gc = gc->older ) {
        /* Make a note of the upper bound */
        if( gc->older == NULL )
            upperBound = gc;
        /* Delete unreferenced objects */
        for( i = 0; i < gc->next_free; i++ )
        {
            if( gc->contents[i] != NULL && gc->contents[i]->refcount <= 0 )
                ferite_delete_class_object( script, gc->contents[i], FE_TRUE );
            else {
#if 0
				FeriteVariable *var = ferite_call_function( script, gc->contents[i], NULL, ferite_object_get_function( script, gc->contents[i], "toString" ), NULL );
				count++;
				fprintf( stderr, "Merging object of class %s [%d]\n", gc->contents[i]->klass->name, gc->contents[i]->refcount );
				fprintf( stderr, "  -> %s\n", VAS(var)->data );
#endif
                ferite_add_to_generation_gc_unlocked( script, gc->contents[i] );
			}
            gc->contents[i] = NULL;
        }
    }
    ferite_generation_destroy( script, lowerBound );
#if 0
	fprintf( stderr, "Merge GC [%p] %d objects\n", script, count );
#endif
    UNLOCK_GC; /* We unlock here because it means we don't clobber ourselves two lines down */
}
