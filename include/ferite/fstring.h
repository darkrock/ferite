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

#ifndef __FERITE_STRING_H__
#define __FERITE_STRING_H__

FERITE_API FeriteString *ferite_str_new( FeriteScript *script, char *str, size_t length, int encoding );
FERITE_API FeriteString *ferite_str_dup( FeriteScript *script, FeriteString *str );
FERITE_API void	         ferite_str_set( FeriteScript *script, FeriteString *str, char *data, size_t length, int encoding );
FERITE_API void	         ferite_str_destroy( FeriteScript *script, FeriteString *str );
FERITE_API int           ferite_str_cmp( FeriteScript *script, FeriteString *str1, FeriteString *str2 );
FERITE_API int           ferite_str_ncmp( FeriteScript *script, FeriteString *str1, FeriteString *str2, size_t size );
FERITE_API int 	         ferite_str_case_cmp( FeriteScript *script, FeriteString *str1, FeriteString *str2 );
FERITE_API int 	         ferite_str_case_ncmp( FeriteScript *script, FeriteString *str1, FeriteString *str2, size_t size );
FERITE_API int           ferite_str_cpy( FeriteScript *script, FeriteString *str1, FeriteString *str2 );
FERITE_API int           ferite_str_ncpy( FeriteScript *script, FeriteString *str1, FeriteString *str2, size_t size );
FERITE_API int           ferite_str_cat( FeriteScript *script, FeriteString *str1, FeriteString *str2 );
FERITE_API int           ferite_str_ncat( FeriteScript *script, FeriteString *str1, FeriteString *str2, size_t size );
FERITE_API int           ferite_str_find( FeriteScript *script, FeriteString *str, char *token );
FERITE_API int           ferite_str_delete( FeriteScript *script, FeriteString *str, size_t from, size_t to );
FERITE_API int           ferite_str_insert( FeriteScript *script, FeriteString *str, FeriteString *data, size_t pos );
FERITE_API FeriteString *ferite_str_replace( FeriteScript *script, FeriteString *str, FeriteString *what, FeriteString *with );
FERITE_API FeriteString *ferite_str_dup( FeriteScript *script, FeriteString *str );
FERITE_API int           ferite_str_data_cat( FeriteScript *script, FeriteString *str, void *data, size_t size );
FERITE_API FeriteString *ferite_str_escape( FeriteScript *script, FeriteString *str );

#define FE_CHARSET_DEFAULT 0
#define FE_CHARSET_UTF8    1

#endif
