/*
 * Copyright (C) 1999-2007 Chris Ross
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

/*
 * a heap allocator based on jedi mind powers. and the algorithm described
 * in perl's malloc.c
 */

#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

#include "ferite.h"
#include "aphex.h"
#include <sys/time.h>
#include "fmem_jedi.h"

#ifdef DEBUG
#undef DEBUG
#endif

/* various constants that can be tweaked. BLOCK_COUNT must be bigger then NBUCKETS! */
#define NBUCKETS                30               /* number of chains to have */
#define BLOCK_COUNT             (NBUCKETS + 2)   /* number of chunks to allocate each time */
#define MAGIC                   42               /* the answer to life, the universe, everything and whether a pointer is correct */
#define DEAD_MAGIC              0xdeadbeef
#define FILL_BYTE               (char)0xfe

/* macors to keep real world stats correct */
#define rmalloc( size )         malloc( size ); real_stats.malloc_c++;
#define rcalloc( size, block )  calloc( size, block ); real_stats.calloc_c++;
#define rrealloc( ptr, size )   realloc( ptr, size ); real_stats.realloc_c++;
#define rfree( ptr )            free( ptr ); real_stats.free_c++;

/* these get the header and the body from a pointer */
#define PTR_GET_HEADER( ptr )   (FeriteMemoryChunkHeader*)((char *)ptr - sizeof(FeriteMemoryChunkHeader))
#define PTR_GET_BODY( ptr )     (void*)((char *)ptr + sizeof(FeriteMemoryChunkHeader))

/* threading stuff */
#ifdef THREAD_SAFE
AphexMutex *ferite_jedi_memory_lock = NULL;
# define LOCK_MEMORY()     		if( script && script->is_multi_thread ) aphex_mutex_lock( ferite_jedi_memory_lock )
# define UNLOCK_MEMORY()   		if( script && script->is_multi_thread ) aphex_mutex_unlock( ferite_jedi_memory_lock )
#else
# define LOCK_MEMORY()
# define UNLOCK_MEMORY()
#endif

typedef union ferite_memory_chunk_header FeriteMemoryChunkHeader;
union ferite_memory_chunk_header
{
	struct {
		FeriteMemoryChunkHeader *next;
		int magic;
	} storage;
    struct  {
        int index; /* bucket for index, < NBUCKETS */
        int magic; /* magic -> so we can check that the pointer is allocated and we haven't gone painfully wrong somewhere */
    } assigned_info;
    double alignment; /* force dword alignment */
};
extern int ferite_pow_lookup[];
static FeriteMemoryChunkHeader *ferite_jedi_free_chunks[NBUCKETS];
static long                     ferite_jedi_chunk_allocs[NBUCKETS];
static FeriteMemoryChunkHeader *ferite_jedi_big_chunks = NULL;

typedef struct
{
    long malloc_c;
    long calloc_c;
    long realloc_c;
    long free_c;
}
FeriteMemoryStats;

FeriteMemoryStats       real_stats; /* how many times we hit the OS */
FeriteMemoryStats       vrtl_stats; /* how many times we hit the local heap */

void ferite_jedi_catch() {
	fprintf( stderr, "ferite_jedi_catch(): Sleeping for gdb interruption for 30 seconds; (process id: %d)\n", getpid() );
	sleep(30);
	exit(255); 
}

void ferite_jedi_memory_init(void)
{
    int i = 0;

    FE_ENTER_FUNCTION;

	ferite_memory_init = ferite_jedi_memory_init;
	ferite_memory_deinit = ferite_jedi_memory_deinit;
	ferite_malloc = ferite_jedi_malloc;
	ferite_calloc = ferite_jedi_calloc;
	ferite_realloc = ferite_jedi_realloc;
	ferite_free = ferite_jedi_free;
	
    real_stats.malloc_c = 0;
    real_stats.calloc_c = 0;
    real_stats.realloc_c = 0;
    real_stats.free_c = 0;

    vrtl_stats.malloc_c = 0;
    vrtl_stats.calloc_c = 0;
    vrtl_stats.realloc_c = 0;
    vrtl_stats.free_c = 0;

    if( !ferite_hide_mem_use )
    {
#ifdef FERITE_MEM_DEBUG
        printf( "Jedi Memory System Active\n" );
#endif
    }

    for( i = 0; i < NBUCKETS; i++ ) {
        ferite_jedi_free_chunks[i] = NULL;
		ferite_jedi_chunk_allocs[i] = 0;
	}
#ifdef THREAD_SAFE
    ferite_jedi_memory_lock = aphex_mutex_recursive_create();
#endif
    FE_LEAVE_FUNCTION(NOWT);
}

void ferite_jedi_memory_deinit(void)
{
    void *ptr = NULL;
	int i = 0;

    FE_ENTER_FUNCTION;
    while( ferite_jedi_big_chunks != NULL )
    {
        ptr = ferite_jedi_big_chunks->storage.next;
        rfree( ferite_jedi_big_chunks );
        ferite_jedi_big_chunks = ptr;
    }
	
    if( !ferite_hide_mem_use ) /* 2/3's of all statistics are made up. unfortunatly not here */
    {
        printf( "Ferite Memory Usage Statistics (jedi)\n" );
        printf( " |- Virtual.. %ld mallocs, %ld callocs, %ld reallocs, %ld frees",
                vrtl_stats.malloc_c, vrtl_stats.calloc_c, vrtl_stats.realloc_c, vrtl_stats.free_c );
        printf( " [%ld block%s still allocated]\n", (vrtl_stats.malloc_c + vrtl_stats.calloc_c) - vrtl_stats.free_c,
                (((vrtl_stats.malloc_c + vrtl_stats.calloc_c) - vrtl_stats.free_c) == 1 ? "" : "s"));
        printf( " `- Real..... %ld mallocs, %ld callocs, %ld reallocs, %ld frees",
                real_stats.malloc_c, real_stats.calloc_c, real_stats.realloc_c, real_stats.free_c );
        printf( " [%ld block%s still allocated]\n", (real_stats.malloc_c + real_stats.calloc_c) - real_stats.free_c,
                (((real_stats.malloc_c + real_stats.calloc_c) - real_stats.free_c) == 1 ? "" : "s"));
		
		printf( "\nFerite Allocation Distribution\n" );
		for( i = 0; i < NBUCKETS; i++ ) {
			printf( " %c- Memory Bucket[%d] = %ld (size %d)\n", (i < (NBUCKETS - 1) ? '|' : '`'), i, ferite_jedi_chunk_allocs[i], ferite_pow_lookup[i] );
		}
    }
	
#ifdef THREAD_SAFE
    aphex_mutex_destroy( ferite_jedi_memory_lock );
#endif
    FE_LEAVE_FUNCTION(NOWT);
}

void ferite_jedi_dump_memory( int bucket )
{
    int i = 0;
    FeriteMemoryChunkHeader *hdr = NULL;

   /* dump a free bucket chain... */
    FUD(( "================================ Table Dump ==================================\n" ));
    hdr = ferite_jedi_free_chunks[bucket];
    while( hdr != NULL ) /* keep looping until we hit the end... */
    {
        FUD(( " [%d]> hdr=%p, hdr->storage.next=%p, %ld, sizeof(Header): %ld, pow[%d]=%d\n", ++i, hdr, hdr->storage.next,
              (long)((void*)hdr->storage.next) - (long)((void*)hdr), sizeof(FeriteMemoryChunkHeader), bucket, ferite_pow_lookup[bucket] ));
        hdr = hdr->storage.next;
    }
}

void *ferite_jedi_malloc( size_t size, char *file, int line, FeriteScript *script )
{
    char *return_ptr = NULL, *magic = NULL;
    int target_bucket = 2 /* 2^3 = 8 bytes lower water level */, i = 0, *allocated_size = NULL;
    FeriteMemoryChunkHeader *ptr = NULL;
	size_t actual_size = 0;
	
	/* Whatever size that we happen to have, we need atleast 3 more bytes at the end to check for
	 * overwrites, bucket and magic */
	/* Get the correct bucket size (we should check the wastage and have an odd's and
	 * sods chain we allocate random chunks from -> but thats for another day...) */
    while( size > (size_t)ferite_pow_lookup[target_bucket] )
		target_bucket++;
	actual_size = (size_t)ferite_pow_lookup[target_bucket];
	
    FUD(( "Target bucket for data of %ld is %d(%d)\n", size, target_bucket, ferite_pow_lookup[target_bucket] ));

    LOCK_MEMORY();
	/* check to see if we have memory :), if not go a eat some :) */
    if( ferite_jedi_free_chunks[target_bucket] == NULL ) 
	{
		if( script && script->gc )
		{
			if( ! script->gc_running ) {
				ferite_check_gc( script );
			}
		}
		if( ferite_jedi_free_chunks[target_bucket] == NULL )
			ferite_jedi_morecore( target_bucket );
	}
    if( (ptr = ferite_jedi_free_chunks[target_bucket]) == NULL )
    {
		/* Oooh! We are up the creek, so to say  :( */
#ifdef FERITE_MEM_DEBUG
        fprintf( stderr, "JEDI: Out of memory. Oh dear. Oh dear. Go out and buy some more :)\n" );
#endif
        UNLOCK_MEMORY();
        ferite_jedi_catch();
		return NULL; /* Never reached */
    }

	/* Rebuild the chain */
    FUD(( "ferite_jedi_free_chunks[target_bucket]: %p\n", ferite_jedi_free_chunks[target_bucket] ));
    FUD(( "new ferite_jedi_free_chunks:            %p\n", ptr->storage.next ));
    ferite_jedi_free_chunks[target_bucket] = ptr->storage.next;

	/* Setup the information for the wild goose chase :) */
    ptr->assigned_info.index = target_bucket;
    ptr->assigned_info.magic = MAGIC;
	/* Get the memory chunk */
    return_ptr = PTR_GET_BODY(ptr);
	/* FILL_BYTE up the rest of the memory */
	for( i = size; i <= actual_size && (i - size) < 256; i++ ) {
		return_ptr[i] = FILL_BYTE;
	}
	/* Setup the allocated size */
	allocated_size = (int*)(((char *)return_ptr) + actual_size + 2);
	*allocated_size = (int)size;
	/* Setup the magic */
	magic = ((char*)return_ptr) + actual_size + 1;
	*magic = (char)MAGIC;
	
    FUD(( "returning: %p, %d\n", return_ptr, (int)((void *)return_ptr - (void *)ptr) ));
    vrtl_stats.malloc_c++;
    UNLOCK_MEMORY();
	
	/* Check for 4 byte alignment */
	if( ((long)return_ptr % 4) != 0 ) {
		fprintf(stderr, "Unaligned memory. Off by %ld for bucket %d (bucket size %d, requested size %ld)\n", ((long)return_ptr % 4), target_bucket, ferite_pow_lookup[target_bucket], size);
		ferite_jedi_catch();
		return NULL;
	}
	
	ferite_jedi_chunk_allocs[target_bucket]++;
	
    return return_ptr;
}

void *ferite_jedi_calloc( size_t size, size_t blk_size, char *file, int line, FeriteScript *script )
{ 
	/* surely the easist calloc *ever*? ;) */
	void *ptr = NULL;

    size *= blk_size;
    ptr = ferite_jedi_malloc( size, __FILE__, __LINE__, script );
    vrtl_stats.malloc_c--;
    vrtl_stats.calloc_c++;
    memset( ptr, 0, size );
    return ptr;
}

int ferite_jedi_validate_pointer( char *ptr, char *file, int line ) 
{
    FeriteMemoryChunkHeader *hdr = NULL;
    int bucket = 0;
	
    if( ptr != NULL )
    {
        hdr = PTR_GET_HEADER(ptr);
        FUD(( "validating %p (hdr=%p,diff=%ld)\n", ptr, hdr, (long)((void*)ptr) - (long)((void*)hdr) ));
        if( hdr->assigned_info.magic == MAGIC )
        {
			int actual_size = 0, i = 0, *allocated_size = NULL;
			int want_catch = FE_FALSE;
			char *magic = NULL, *cptr = (char*)ptr;
			
            bucket = hdr->assigned_info.index;
			actual_size = ferite_pow_lookup[hdr->assigned_info.index];
			allocated_size = (int*)(((char *)ptr) + actual_size + 2);
#ifdef DEBUG
			magic = ((char*)ptr) + actual_size + 1;
			/* Check the tail magic */
			if( *magic != MAGIC ) {
				fprintf( stderr, "JEDI: Detected memory corruption. Expecting %d for tail magic, but got %d (addy %p) (culprit %s, line %d) [%d:%d] [%d]\n", MAGIC, *magic, ptr, file, line, bucket, actual_size, *allocated_size );
				ferite_jedi_catch();
				return 0;
			}
			/* Check the overwrite issues */
			for( i = *allocated_size; i <= actual_size && (i - *allocated_size) < 256 ; i++ ) {
				if( (char)cptr[i] != FILL_BYTE ) 
				{	
					fprintf( stderr, "JEDI: Detected memory overwrite at %d bytes after memory block (addy %p) (culprit %s, line %d)\n", (i - *allocated_size), ptr, file, line );
					fprintf( stderr, "      (Start: %d, End: %ld)\n", *allocated_size, (actual_size - sizeof(int) - 1) );
					want_catch = FE_TRUE;
				}
			}
			if( want_catch ) {
				ferite_jedi_catch();
				return 0;
			}
			return *allocated_size;
        }
        else if( hdr->assigned_info.magic == DEAD_MAGIC ) /* We have a double free */
		{
			fprintf( stderr, "JEDI: Detected a double free on pointer %p (culprit %s, line %d)\n", ptr, file, line );
			ferite_jedi_catch();
		}
#else
		return *allocated_size;
 	}
#endif
		else /* fubar'd pointer -> we must know about these */
        {
			int i = 0;
			char *string = ((char*)hdr) - 128;
			fprintf( stderr, "JEDI: Detected memory corruption. Expecting %d for magic, but got %d (addy %p) (culprit %s, line %d)\n", MAGIC, hdr->assigned_info.magic, ptr, file, line );
            fprintf( stderr, "JEDI: MEM DATA: `" );
			for( i = 0; i < 256; i++ ) {
				if( ((unsigned char)string[i]) < 32 ) {
					fprintf( stderr, "\\%d", (unsigned char)string[i] );
				} else {
					fprintf( stderr, "%c", (char)string[i] );
				}
			}
			fprintf( stderr, "'\n" );
			ferite_jedi_catch();
		}
    }
    else 
	{
		fprintf( stderr, "JEDI: trying to free a NULL pointer in %s on line %d\n", file, line );
		ferite_jedi_catch();
	}
	return 0;
}
void *ferite_jedi_realloc( void *ptr, size_t size, FeriteScript *script )
{
    FeriteMemoryChunkHeader *hdr = NULL;
	int allocated_size = 0;
    long old_size = 0, old_index = 0;
    void *new_ptr = NULL;
    int target_bucket = 3;

	allocated_size = ferite_jedi_validate_pointer( ptr, __FILE__, __LINE__ );
    if( ptr != NULL && allocated_size > 0 )
    { 
		/* we want to copy the old memory to the new memory */
        hdr = PTR_GET_HEADER( ptr );
        old_size = ferite_pow_lookup[hdr->assigned_info.index];
        old_index = hdr->assigned_info.index;
		
		/* we now have two options, either keep the same buffer, or copy to a bigger one */
		while( size > (size_t)ferite_pow_lookup[target_bucket] )
			target_bucket++;
		
		if( target_bucket <= old_index ) 
		{
			int *new_allocated_size = NULL, i= 0;
			char *new_magic = NULL;
			char *cptr = (char*)ptr;
			
			/* FILL_BYTE up the rest of the memory */
			for( i = size; i <= old_size; i++ ) 
				cptr[i] = FILL_BYTE;
			/* Setup the allocated size */
			new_allocated_size = (int*)(((char *)cptr) + old_size + 2);
			*new_allocated_size = (int)size;
			/* Setup the magic */
			new_magic = ((char*)cptr) + old_size + 1;
			*new_magic = (char)MAGIC;
			return ptr;
		}
		
		/* this is an evil slow method of realloc - but necessary */
        new_ptr = ferite_jedi_malloc( size, __FILE__, __LINE__, script );
        memcpy( new_ptr, ptr, (size > allocated_size ? allocated_size : size) );
		
		LOCK_MEMORY();
		/* now we move the older ptr onto it's old block */
        hdr->storage.next = ferite_jedi_free_chunks[old_index];
		hdr->storage.magic = DEAD_MAGIC;
        ferite_jedi_free_chunks[old_index] = hdr;
        vrtl_stats.malloc_c--;
        vrtl_stats.realloc_c++;
        UNLOCK_MEMORY();
    } else if( allocated_size == 0 ) {
		/* FIXME */
	}
    return new_ptr;
}

void ferite_jedi_free( void *ptr, char *file, int line, FeriteScript *script )
{
    FeriteMemoryChunkHeader *hdr = NULL;
    int bucket = 0;
	int amount = ferite_jedi_validate_pointer( ptr, file, line );
	
    if( ptr != NULL && amount > 0 )
    {
        hdr = PTR_GET_HEADER(ptr);
		bucket = hdr->assigned_info.index;
		/* relink the chain */
		FUD(( "Setting next as %p\n", ferite_jedi_free_chunks[bucket] ));
		LOCK_MEMORY();
		hdr->storage.next = ferite_jedi_free_chunks[bucket];
		hdr->storage.magic = DEAD_MAGIC;
		FUD(( "Setting new header as %p\n", hdr ));
		ferite_jedi_free_chunks[bucket] = hdr;
		UNLOCK_MEMORY();
		vrtl_stats.free_c++;
	}
}

void ferite_jedi_morecore( int bucket )
{
    int i = 0;
    void *new_block = NULL;
	int footer_size = sizeof(int) + 4; /* Int for the chunk size, 2 for the fill byte and magic, and 2 for buffering */
    int chunk_size = ferite_pow_lookup[bucket] + sizeof(FeriteMemoryChunkHeader) + footer_size;
    int actual_block_count = BLOCK_COUNT - (bucket < 10 ? 0 : bucket);
    FeriteMemoryChunkHeader *hdr = NULL, *free_chunks = NULL;
    //printf("%d\n",jedi_count++);
    FUD(( "in more core -> allocating for %d\n", bucket ));

	/* check to see if we have actually run out of space on bucket list */
    if( ferite_jedi_free_chunks[bucket] )
		return;
	
	/* this is where the buckets will be */
    new_block = rmalloc( (chunk_size * actual_block_count) + /* blocks of memory */
                         sizeof(FeriteMemoryChunkHeader) );  /* initial header   */
    ((FeriteMemoryChunkHeader*)new_block)->storage.next = ferite_jedi_big_chunks; /* hook up  header */
    ((FeriteMemoryChunkHeader*)new_block)->storage.magic = DEAD_MAGIC; /* hook up  header */
    ferite_jedi_big_chunks = new_block; /* this now becomes the headerder */

   /* the memory on this chunk needs to be setup as follows:
    *       /---------------------\
    *   [header][memory         ][header][memory         ]
    *   |<-a ->||<-p_l[bucket]->||<-a ->||<-p_l[bucket]->|  */
    hdr = free_chunks = (FeriteMemoryChunkHeader*)(((char*)new_block) + sizeof(FeriteMemoryChunkHeader)); /* get the top of our memory */

#ifdef FERITE_MEM_DEBUG
	{
		struct timeval t;
		gettimeofday( &t, NULL );
//		fprintf( stderr, "================================ \"Morecore\",\"%ld\",\"%ld.%ld\" ================================\n", ferite_pow_lookup[bucket], t.tv_sec, t.tv_usec );
		FUD(( "================================ \"Morecore\",\"%d\",\"%ld.%d\" ================================\n", ferite_pow_lookup[bucket], t.tv_sec, t.tv_usec ));
	}
#else
	FUD(( "================================ \"Morecore\",\"%ld\" ================================\n", ferite_pow_lookup[bucket] ));
#endif
    for( i = 0; i < actual_block_count-1; i++ ) /* go through the block linking up the blocks */
    {
        hdr->storage.next =  (FeriteMemoryChunkHeader*)(((char*)hdr) + ferite_pow_lookup[bucket] + sizeof(FeriteMemoryChunkHeader) + footer_size);
		hdr->storage.magic = DEAD_MAGIC;
        FUD(( " [%d]> hdr=%p, hdr->storage.next=%p, %ld, sizeof(Header): %ld, pow[%d]=%d\n", i, hdr, hdr->storage.next,
              (long)((void*)hdr->storage.next) - (long)((void*)hdr), sizeof(FeriteMemoryChunkHeader), bucket, ferite_pow_lookup[bucket] ));
        hdr = hdr->storage.next;
    }
    hdr->storage.next = NULL; /* stop the chain -> so we know when to allocate more */
	hdr->storage.magic = DEAD_MAGIC;
    FUD(( " [%d]> hdr=%p, hdr->storage.next=%p, %ld, sizeof(Header): %ld, pow[%d]=%d\n", i, hdr, hdr->storage.next,
          (long)((void*)hdr->storage.next) - (long)((void*)hdr), sizeof(FeriteMemoryChunkHeader), bucket, ferite_pow_lookup[bucket] ));

   /* link the memory in */
    ferite_jedi_free_chunks[bucket] =  free_chunks;
   /* dump the table so that we can check that is correlates with the above table */
#ifdef FERITE_MEM_DEBUG
    ferite_jedi_dump_memory( bucket );
#endif
    FUD(( "morecore: ferite_jedi_free_chunks[bucket]: %p\n", ferite_jedi_free_chunks[bucket] ));
}
