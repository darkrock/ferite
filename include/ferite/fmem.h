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

#ifndef __FERITE_MEMORY_H__
#define __FERITE_MEMORY_H__

#define fmalloc( size )           (ferite_malloc)( (size), __FILE__, __LINE__, script )
#define fmalloc_ngc( size )       (ferite_malloc)( (size), __FILE__, __LINE__, NULL )
#define fcalloc( a, b )           (ferite_calloc)( (a), (b), __FILE__, __LINE__, script )
#define fcalloc_ngc( a, b )       (ferite_calloc)( (a), (b), __FILE__, __LINE__, NULL )
#define frealloc( ptr, size )     (ferite_realloc)( (ptr), (size), script )
#define frealloc_ngc( ptr, size ) (ferite_realloc)( (ptr), (size), NULL )
#define ffree( ptr )              { (ferite_free)( (ptr), __FILE__, __LINE__, script ); ptr = NULL; }
#define ffree_ngc( ptr )          { (ferite_free)( (ptr), __FILE__, __LINE__, NULL ); ptr = NULL; }

FERITE_API void  ferite_classic_memory_init(void);
FERITE_API void  ferite_classic_memory_deinit(void);
FERITE_API void *ferite_classic_malloc( size_t size, char *name, int line, FeriteScript *script );
FERITE_API void *ferite_classic_calloc( size_t size, size_t blk_size, char *name, int line, FeriteScript *script );
FERITE_API void *ferite_classic_realloc( void *ptr, size_t size, FeriteScript *script );
FERITE_API void  ferite_classic_free( void *ptr, char *file, int line, FeriteScript *script );

#endif /* __FERITE_MEMORY_H__ */
