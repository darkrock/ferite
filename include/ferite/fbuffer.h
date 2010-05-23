/*
 * Copyright (c) 2002-2007 Stephan Engström
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

#ifndef __FERITE_BUFFER_H__
#define __FERITE_BUFFER_H__

#define FE_DEFAULT_BUFFER_SIZE  1024

FERITE_API FeriteBuffer    *ferite_buffer_new( FeriteScript *script, size_t size );
FERITE_API void            ferite_buffer_add( FeriteScript *script, FeriteBuffer *buf, void *ptr, size_t size );
FERITE_API void           *ferite_buffer_alloc( FeriteScript *script, FeriteBuffer *buf, size_t size );
FERITE_API void            ferite_buffer_add_char( FeriteScript *script, FeriteBuffer *buf, int character );
FERITE_API void            ferite_buffer_del_char( FeriteScript *script, FeriteBuffer *buf );
FERITE_API void            ferite_buffer_add_fstr( FeriteScript *script, FeriteBuffer *buf, FeriteString *str );
FERITE_API void            ferite_buffer_add_str( FeriteScript *script, FeriteBuffer *buf, char *str );
FERITE_API void            ferite_buffer_add_long( FeriteScript *script, FeriteBuffer *buf, long data );
FERITE_API void            ferite_buffer_add_double( FeriteScript *script, FeriteBuffer *buf, double data );
FERITE_API void            ferite_buffer_add_variable( FeriteScript *script, FeriteBuffer *buf, FeriteVariable *v );
FERITE_API int             ferite_buffer_printf( FeriteScript *script, FeriteBuffer *buf, char *fmt, ... );
FERITE_API int             ferite_buffer_vprintf( FeriteScript *script, FeriteBuffer *buf, char *fmt, va_list *args );
FERITE_API void            ferite_buffer_merge( FeriteScript *script, FeriteBuffer *dest, FeriteBuffer *source );
FERITE_API void            ferite_buffer_delete( FeriteScript *script, FeriteBuffer *buf );

FERITE_API size_t          ferite_buffer_get_size( FeriteScript *script, FeriteBuffer *buf );
FERITE_API void           *ferite_buffer_get( FeriteScript *script, FeriteBuffer *buf, int *len );
FERITE_API int             ferite_buffer_to_fd( FeriteScript *script, FeriteBuffer *buf, int fd );
FERITE_API int             ferite_buffer_to_file( FeriteScript *script, FeriteBuffer *buf, FILE *f );
FERITE_API FeriteVariable *ferite_buffer_to_var( FeriteScript *script, FeriteBuffer *buf );
FERITE_API FeriteString   *ferite_buffer_to_str( FeriteScript *script, FeriteBuffer *buf );
FERITE_API FeriteVariable *ferite_sprintf(FeriteScript *script, char *format, FeriteVariable **params );
FERITE_API int             ferite_format( FeriteScript *script, FeriteBuffer *buf, char *format, FeriteVariable **params );

#endif /* __FERITE_BUFFER_H__ */

