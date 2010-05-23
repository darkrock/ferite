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

#ifndef __FERITE_UARRAY_H__
#define __FERITE_UARRAY_H__

#define FE_ARRAY_ADD_AT_END   -1
#define FE_ARRAY_ADD_AT_START -2

FERITE_API FeriteUnifiedArray *ferite_uarray_create( FeriteScript *script );
FERITE_API void                ferite_uarray_destroy( FeriteScript *script,FeriteUnifiedArray *array);
FERITE_API void                ferite_uarray_add( FeriteScript *script,FeriteUnifiedArray *array, FeriteVariable *var, char *id, int index);
FERITE_API FeriteVariable     *ferite_uarray_get_index( FeriteScript *script, FeriteUnifiedArray *array, int index );
FERITE_API FeriteVariable     *ferite_uarray_get( FeriteScript *script,FeriteUnifiedArray *array, FeriteVariable *var);
FERITE_API FeriteVariable     *ferite_uarray_get( FeriteScript *script,FeriteUnifiedArray *array, FeriteVariable *index );
FERITE_API FeriteVariable     *ferite_uarray_set( FeriteScript *script,FeriteUnifiedArray *array, FeriteVariable *index, FeriteVariable *rhs );
FERITE_API void                ferite_uarray_del_var( FeriteScript *script,FeriteUnifiedArray *array, FeriteVariable *i);
FERITE_API void                ferite_uarray_del_index( FeriteScript *script,FeriteUnifiedArray *array, int i);
FERITE_API FeriteUnifiedArray *ferite_uarray_dup( FeriteScript *script,FeriteUnifiedArray *array );

FERITE_API FeriteVariable     *ferite_uarray_pop( FeriteScript *script,FeriteUnifiedArray *array);
FERITE_API FeriteString	   *ferite_uarray_to_str( FeriteScript *script, FeriteUnifiedArray *array);
FERITE_API void                ferite_uarray_push( FeriteScript *script,FeriteUnifiedArray *array, FeriteVariable *var);
FERITE_API void                ferite_uarray_unshift( FeriteScript *script,FeriteUnifiedArray *array, FeriteVariable *var);
FERITE_API FeriteVariable     *ferite_uarray_shift( FeriteScript *script,FeriteUnifiedArray *array);

FERITE_API FeriteVariable     *ferite_uarray_get_from_string( FeriteScript *script, FeriteUnifiedArray *array, char *id );
FERITE_API FeriteVariable     *ferite_uarray_delete_from_string( FeriteScript *script, FeriteUnifiedArray *array, char *id );

FERITE_API int                 ferite_uarray_cmp( FeriteScript *script, FeriteUnifiedArray *left, FeriteUnifiedArray *right );
FERITE_API void                ferite_uarray_set_size( FeriteScript *script, FeriteUnifiedArray *array, int size );

#endif
