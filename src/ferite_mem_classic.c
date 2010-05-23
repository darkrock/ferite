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

void ferite_classic_memory_dump();

/*
 * this is to get huge amounts (well a fair amount) of debug info....
 * #define SUPER_VERBOSE
*/
//#define FERITE_MEM_DEBUG

#ifdef FERITE_MEM_DEBUG
#undef FERITE_MEM_DEBUG
#endif

int ferite_classic_malloc_count = 0;
int ferite_classic_calloc_count = 0;
int ferite_classic_realloc_count = 0;
int ferite_classic_free_count = 0;

int ferite_classic_end_freeup = -1;
int ferite_total_freeup = 0;

#define rmalloc(s)          malloc(s)
#define rcalloc(s,bs)       calloc(s,bs)
#define rrealloc(p,s)       realloc(p,s)
#define rfree(p)            free(p)

#ifdef THREAD_SAFE
AphexMutex *ferite_classic_memory_lock = NULL;
# define LOCK_MEMORY     aphex_mutex_lock( ferite_classic_memory_lock )
# define UNLOCK_MEMORY   aphex_mutex_unlock( ferite_classic_memory_lock )
#else
# define LOCK_MEMORY
# define UNLOCK_MEMORY
#endif

struct ferite_memory_block *mem_rootblk = NULL;

void  ferite_classic_memory_init(void)
{
    FE_ENTER_FUNCTION;
	ferite_memory_init = ferite_classic_memory_init;
	ferite_memory_deinit = ferite_classic_memory_deinit;
	ferite_malloc = ferite_classic_malloc;
	ferite_calloc = ferite_classic_calloc;
	ferite_realloc = ferite_classic_realloc;
	ferite_free = ferite_classic_free;

#ifdef FERITE_MEM_DEBUG
    FUD(("Internal Memory Deubgging On.\n"));
    mem_rootblk = malloc(sizeof( struct ferite_memory_block ));
    mem_rootblk->data = NULL;
    mem_rootblk->size = 0;
    mem_rootblk->file = strdup( "ROOTMEMBLK" );
    mem_rootblk->next = NULL;
    if( !ferite_hide_mem_use )
    {
        printf( "Classic Memory System Active\n" );
    }

#endif
#ifdef THREAD_SAFE
    ferite_classic_memory_lock = aphex_mutex_create();
#endif
    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_classic_memory_clear_block( struct ferite_memory_block *ptr )
{
    if( ptr != NULL )
    {
        if( ptr->next )
            ferite_classic_memory_clear_block( ptr->next );
        if( !ferite_hide_mem_use && ptr->line != 0 )
            printf( "Freeing block[%p] [%d bytes] allocated on file: %s, line %d\n", ptr->data, ptr->size, ptr->file, ptr->line );
        ferite_total_freeup += ptr->size;
        /* this is just here so i can see what string it is allocated */
        if( ptr->line == 198 || ptr->line == 66 || ptr->line == 54 )
            printf( "%s\n", (char *)ptr->data );
		if( strcmp( ptr->file, "ferite_string.c") == 0 && ptr->line == 99 ) {
			printf("FeriteString.data: %s\n", (char *)ptr->data);
		}
        free( ptr->data );
        free( ptr->file );
        free( ptr );
        ferite_classic_end_freeup++;
    }
}

void ferite_classic_memory_deinit(void)
{
    FE_ENTER_FUNCTION;
    if( ! ferite_hide_mem_use )
    {
#ifdef FERITE_MEM_DEBUG
        ferite_classic_memory_dump();
#endif
        printf( "Ferite Memory Usage Statistics\n" );
        printf( " |-> %d mallocs, %d callocs, %d reallocs - %d frees\n", ferite_classic_malloc_count, ferite_classic_calloc_count, ferite_classic_realloc_count, ferite_classic_free_count );
        printf( " `-> ( %d block(s) still allocated )\n", (ferite_classic_malloc_count + ferite_classic_calloc_count) - ferite_classic_free_count );
    }
#ifdef FERITE_MEM_DEBUG
    if( !ferite_hide_mem_use )
        printf("Freeeing Up Unallocated Memory\n");
    ferite_classic_memory_clear_block( mem_rootblk );
    if( !ferite_hide_mem_use )
        printf("Free'd %d [%d bytes] allocations\n", ferite_classic_end_freeup, ferite_total_freeup );
#endif
#ifdef THREAD_SAFE
    aphex_mutex_destroy( ferite_classic_memory_lock );
#endif
    FE_LEAVE_FUNCTION( NOWT );
}

void *ferite_classic_malloc( size_t size, char *file, int line, FeriteScript *script )
{
#ifdef FERITE_MEM_DEBUG
    struct ferite_memory_block *ptr;
#endif
    
    LOCK_MEMORY;
    ferite_classic_malloc_count++;
    
#ifdef FERITE_MEM_DEBUG
    for( ptr = mem_rootblk; ptr->next != NULL; ptr = ptr->next )
        ;
    ptr->next = malloc(sizeof( struct ferite_memory_block ));
    ptr->next->data = rmalloc( size );
    ptr->next->size = size;
    ptr->next->file = strdup( file );
    ptr->next->line = line;
    if( !ptr->next->data )
    {
        fprintf( stderr, "ERROR: Unable to allocate %ld bytes.\n", size );
        exit(-1);
    }
    ptr->next->next = NULL;
    FUD(( "Allocating %p\n", ptr->next->data ));
    UNLOCK_MEMORY;
    return ptr->next->data;
#else
    UNLOCK_MEMORY;
    return rmalloc( size );
#endif
}

void *ferite_classic_calloc( size_t size, size_t blk_size, char *file, int line, FeriteScript *script )
{
#ifdef FERITE_MEM_DEBUG
    struct ferite_memory_block *ptr;
#endif
    
    LOCK_MEMORY;
    ferite_classic_calloc_count++;
    
#ifdef FERITE_MEM_DEBUG
    for( ptr = mem_rootblk; ptr->next != NULL; ptr = ptr->next )
        ;
    ptr->next = malloc(sizeof( struct ferite_memory_block ));
    ptr->next->data = rcalloc( size, blk_size );
    ptr->next->size = size * blk_size;
    ptr->next->file = strdup( file );
    ptr->next->line = line;
    if( !ptr->next->data )
        FUD(("ERROR: Unable to (c)allocate %ld bytes.\n", size * blk_size));
    ptr->next->next = NULL;
    UNLOCK_MEMORY;
    return ptr->next->data;
#else
    UNLOCK_MEMORY;
    return rcalloc( size, blk_size );
#endif
}

void *ferite_classic_realloc( void *targetptr, size_t size, FeriteScript *script )
{
#ifdef FERITE_MEM_DEBUG
    struct ferite_memory_block *ptr;
#endif
    
    if( targetptr == NULL )
        return ferite_classic_malloc( size, "ferite_mem.c", 125, script );
    else
    {
        LOCK_MEMORY;
        ferite_classic_realloc_count++;
        
#ifdef FERITE_MEM_DEBUG
        for( ptr = mem_rootblk; ptr->next != NULL; ptr = ptr->next )
        {
            if( ptr->next->data == targetptr )
            {
                ptr->next->data = rrealloc( ptr->next->data, size );
                ptr->next->size = size;
                UNLOCK_MEMORY;
                return ptr->next->data;
            }
        }
#endif
        targetptr = rrealloc( targetptr, size );
        UNLOCK_MEMORY;
        return targetptr;
    }
}

void ferite_classic_free( void *targetptr, char *file, int line, FeriteScript *script )
{
#ifdef FERITE_MEM_DEBUG
    struct ferite_memory_block *ptr, *temp_ptr;
    
    if( targetptr == NULL )
        printf( "Trying to free NULL ptr on %s:%d\n", file, line );
    
    LOCK_MEMORY;
    for( ptr = mem_rootblk; ptr->next != NULL; ptr = ptr->next )
    {
        if( ptr->next->data == targetptr )
        {
# ifdef SUPER_VERBOSE
            FUD(("INFO: Found memory block (%p). Freeing.\n", ptr->next->data));
# endif
            ferite_classic_free_count++;
            
            temp_ptr = ptr->next;
            ptr->next = temp_ptr->next;
# ifdef SUPER_VERBOSE
            FUD(("INFO: Freeing %p\n", temp_ptr->data ));
# endif
            rfree( temp_ptr->data );
# ifdef SUPER_VERBOSE
            FUD(("  * Free'd data\n"));
# endif
            free( temp_ptr->file );
# ifdef SUPER_VERBOSE
            FUD(("  * Free'd filename\n"));
# endif
            free( temp_ptr );
# ifdef SUPER_VERBOSE
            FUD(("  * Free'd memory block\n"));
# endif
            
            UNLOCK_MEMORY;
            return;
        }
    }
    FUD(("* URGH. Can't find block(%p) in memory list. Freeing Anyways.\n", targetptr));
    free( targetptr );
    UNLOCK_MEMORY;
#else
    LOCK_MEMORY;
    if( targetptr == NULL )
        printf( "Trying to free null in %s at %d\n", file, line );
    else
        ferite_classic_free_count++;
    rfree( targetptr );
    UNLOCK_MEMORY;
#endif
}

void ferite_classic_memory_dump()
{
    long memory_allocated = 0;
    struct ferite_memory_block *ptr;
    
    if( mem_rootblk != NULL && mem_rootblk->next != NULL )
    {
        fprintf( stderr, "Currently Allocated Memory:\n" );
        fprintf( stderr, "+-----------+---------+----------------------------------------------------------------+-------+\n");
        fprintf( stderr, "| Address   |  Amount | File                                                           |  Line |\n");
        fprintf( stderr, "+-----------+---------+----------------------------------------------------------------+-------+\n");
        for (ptr = mem_rootblk->next; ptr != NULL; ptr = ptr->next)
        {
            fprintf(stderr, "| %9p | %7d | %-62.62s | %5d |\n", ptr->data, ptr->size, ptr->file, ptr->line);
            memory_allocated += ptr->size;
        }
        fprintf( stderr, "+-----------+---------+----------------------------------------------------------------+-------+\n");
        fprintf( stderr, "Total Memory Allocated = %ld bytes ( ~%ld kb ).\n", memory_allocated, (memory_allocated / 1024) + 1);
    }
}
