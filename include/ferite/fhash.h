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

#ifndef __FERITE_HASH_H__
#define __FERITE_HASH_H__

FERITE_API unsigned int      ferite_hash_gen( char *key, size_t keylen );
FERITE_API FeriteHashBucket *ferite_create_hash_bucket( FeriteScript *script, char *key, void *value );
FERITE_API FeriteHash       *ferite_create_hash( FeriteScript *script, int size );
FERITE_API FeriteHash       *ferite_hash_dup( FeriteScript *script, FeriteHash *hash, void *(*ddup)(FeriteScript *,void *,void *), void *data2 );
FERITE_API void              ferite_delete_hash( FeriteScript *script, FeriteHash *hash, void (*cb)( FeriteScript *script,void *data) );
FERITE_API void              ferite_process_hash( FeriteScript *script, FeriteHash *hash, void (*cb)( FeriteScript *script,void *data,char *key) );
FERITE_API void              ferite_hash_add( FeriteScript *script, FeriteHash *hash, char *key, void *data );
FERITE_API void              ferite_hash_delete( FeriteScript *script, FeriteHash *hash, char *key );
FERITE_API void             *ferite_hash_get( FeriteScript *script, FeriteHash *hash, char *key );
FERITE_API FeriteHashBucket *ferite_hash_walk(FeriteScript *script, FeriteHash *hash, FeriteIterator *iter);
FERITE_API FeriteHash       *ferite_hash_grow( FeriteScript *script, FeriteHash *hash );
FERITE_API void              ferite_hash_print( FeriteScript *script, FeriteHash *hash );
FERITE_API void              ferite_hash_update( FeriteScript *script, FeriteHash *hash, char *key, void *data );

FERITE_API FeriteIterator   *ferite_create_iterator( FeriteScript *script );

#endif /* __FERITE_HASH_H__ */
