/*
 * Copyright (C) 1999-2009 Chris Ross
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

#ifdef HAVE_LIBGC
#define GC_THREADS
#define GC_REDIRECT_TO_LOCAL
#include "gc.h"
#endif

#include "ferite.h"
#include "aphex.h"
#include "fglobals.h"
#include "fmem_libgc.h"

void  ferite_libgc_memory_init(void)
{
    FE_ENTER_FUNCTION;
#ifdef HAVE_LIBGC
	ferite_memory_init = ferite_libgc_memory_init;
	ferite_memory_deinit = ferite_libgc_memory_deinit;
	ferite_malloc = ferite_libgc_malloc;
	ferite_calloc = ferite_libgc_calloc;
	ferite_realloc = ferite_libgc_realloc;
	ferite_free = ferite_libgc_free;
#else
	fprintf(stderr, "Unable to load libgc memory manager - it hasn't been compiled against. Sorry.\n");
	exit(0);
#endif

    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_libgc_memory_deinit(void)
{
    FE_ENTER_FUNCTION;
    FE_LEAVE_FUNCTION( NOWT );
}

void *ferite_libgc_malloc( size_t size, char *file, int line, FeriteScript *script )
{
#ifdef HAVE_LIBGC
	return GC_MALLOC(size);
#else
	return NULL;
#endif
}

void *ferite_libgc_calloc( size_t size, size_t blk_size, char *file, int line, FeriteScript *script )
{
#ifdef HAVE_LIBGC
	void *ptr = GC_MALLOC(size * blk_size);
	memset(ptr, 0, (size * blk_size));
	return ptr;
#else
	return NULL;
#endif
}

void *ferite_libgc_realloc( void *targetptr, size_t size, FeriteScript *script )
{
#ifdef HAVE_LIBGC
	return GC_REALLOC(targetptr, size);
#else
	return NULL;
#endif
}

void ferite_libgc_free( void *targetptr, char *file, int line, FeriteScript *script )
{
}
