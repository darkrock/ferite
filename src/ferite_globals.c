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

#include <ferite/freq.h>
#include <ferite/fstructs.h>

/**
 * @group Engine Globals
 * @description This is where the main globals are held. These are used all 
 *              over the ferite engine and provide useful no-area specific
 *              functions.
 */

/* This is only used within debug to check function call traces  */
int                ferite_call_level = 0;

/** 
 * @variable ferite_show_debug
 * @type int 
 * @brief A flag to say whether or not we should show debug output 
 */
int                ferite_show_debug = 0;

/*! Should we hide memory use? */
int                ferite_hide_mem_use = 1;

/*! Should we use our mm with pcre ? Nope, not as default */
int                ferite_use_mm_with_pcre = 0;

int                ferite_is_strict = 0;

int                ferite_show_partial_implementation = 0;

/*! Generic function for memory management. Hides the actually memory manager */
void  (*ferite_memory_init)(void);
/*! Generic function for memory management. Hides the actually memory manager */
void  (*ferite_memory_deinit)(void);

/**
 * @function fmalloc
 * @declaration void *fmalloc( size_t size )
 * @brief Allocate a block of memory
 * @param size_t size The size of the memory required
 * @return A pointer to the allocated memory
 */
void *(*ferite_malloc)( size_t size, char *name, int line, FeriteScript *script );

/**
 * @function fcalloc
 * @declaration fcalloc( size_t size, size_t blk_size )
 * @brief Allocate a block of memory and have it zero'd to blk_size blocks
 * @param size_t size The size of memory to allocate
 * @param size_t blk_size Size of blocks
 * @return A pointer to the allocated memory
 */
void *(*ferite_calloc)( size_t size, size_t blk_size, char *name, int line, FeriteScript *script );

/**
 * @function frealloc
 * @declaration void *frealloc( void *ptr, size_t size )
 * @brief Reallocate a block of memory to be smaller or bigger
 * @param void *ptr The memory to resize
 * @param size_t size The new size of the memory block
 * @return The pointer to the block of memory
 */
void *(*ferite_realloc)( void *ptr, size_t size, FeriteScript *script );

/**
 * @function ffree
 * @declaration void ffree( void *ptr );
 * @brief Free a block of memory
 * @param void *ptr The memory to free up
 */
void  (*ferite_free)( void *ptr, char *file, int line, FeriteScript *script );

/* Function pointers for Garbage Collection */
void  (*ferite_init_gc)( FeriteScript *script );
void  (*ferite_deinit_gc)( FeriteScript *script );

/**
 * @function ferite_add_to_gc
 * @declaration void ferite_add_to_gc( FeriteScript *script, FeriteObject *obj )
 * @brief Add an object to the garbage collector
 * @param FeriteScript *script The script
 * @param FeriteObject *obj The object to add
 */
void  (*ferite_add_to_gc)( FeriteScript *script, FeriteObject *obj );

/**
 * @function ferite_check_gc
 * @declaration void ferite_check_gc( FeriteScript *script )
 * @brief Cause a check of the garbage collector
 * @param FeriteScript *script The script
 */
void  (*ferite_check_gc)( FeriteScript *script );
void  (*ferite_merge_gc)( FeriteScript *script, void *gc );

/**
 * @variable ferite_ARGV
 * @type FeriteVariable
 * @brief This is the arguments that can be passed to a script
 */
FeriteVariable *ferite_ARGV = NULL;

jmp_buf ferite_exception_jmpback;
int ferite_exception_status;

void ferite_debug_catch( void *p, int count ) 
{
	fprintf( stderr, "ferite_debug_catch(%p,%d): Sleeping for 30 seconds (process id: %d)\n", p, count, getpid() );
	sleep(30);
}

/**
 * @end
 */
