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

#include "ferite.h"

#ifndef __FERITE_ERROR_H__
#define __FERITE_ERROR_H__

#define FE_NO_ERROR     0
#define FE_ERROR_THROWN 1

#define NO_ERROR(SCRIPT) (SCRIPT->error_state == FE_NO_ERROR)

FERITE_API void ferite_init_error_system( FeriteScript *script, FeriteNamespace *ns );
FERITE_API void ferite_raise_script_error( FeriteScript *script, int err, char *fmt, ... );
FERITE_API void ferite_vwarning(FeriteScript *script, char *errormsg, va_list *ap );
FERITE_API FeriteVariable *ferite_generate_backtrace( FeriteScript *script, int skip_first );
FERITE_API void ferite_error( FeriteScript *script, int err, char *errormsg, ... );
FERITE_API void ferite_vwarning(FeriteScript *script, char *errormsg, va_list *ap );
FERITE_API void ferite_warning( FeriteScript *script, char *errormsg, ... );
FERITE_API void ferite_set_error( FeriteScript *script, int num, char *fmt, ... );
FERITE_API char *ferite_get_error_log( FeriteScript *script );
FERITE_API char *ferite_get_error_string( FeriteScript *script );
FERITE_API char *ferite_get_warning_string( FeriteScript *script );
FERITE_API void ferite_reset_errors( FeriteScript *script );
FERITE_API void ferite_reset_warnings( FeriteScript *script );
FERITE_API int ferite_has_compile_error( FeriteScript *script );
FERITE_API int ferite_has_runtime_error( FeriteScript *script );
FERITE_API int ferite_has_warnings( FeriteScript *script );
FERITE_API void ferite_assert( char *fmt, ... );

extern jmp_buf ferite_exception_jmpback;
extern int ferite_exception_status;

#define MONITOR \
	do { \
		jmp_buf old_ferite_exception_jmpback; \
		int     old_ferite_exception_status = ferite_exception_status; \
		memcpy( &old_ferite_exception_jmpback, &ferite_exception_jmpback, sizeof(jmp_buf) ); \
		if ((ferite_exception_status = setjmp(ferite_exception_jmpback)) == 0) {

#define HANDLE \
		} else { \
			memcpy( &ferite_exception_jmpback, &old_ferite_exception_jmpback, sizeof(jmp_buf) ); 

#define HANDLEV(value) \
		} else if( ferite_exception_status == value ) { \
			memcpy( &ferite_exception_jmpback, &old_ferite_exception_jmpback, sizeof(jmp_buf) ); 

#define ENDMONITOR \
			ferite_exception_status = old_ferite_exception_status; \
		} \
	} while(0);

#define RAISE(val)   longjmp(ferite_exception_jmpback,val)

/* Exception codes */

#define RangeError     1
#define DivisionByZero 2
#define OutOfMemory    3

#endif
