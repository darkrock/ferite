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

#ifndef _FERITE_FUNCTION_H_
#define _FERITE_FUNCTION_H_

#define FNC_IS_INTRL 1
#define FNC_IS_EXTRL 2

#define FE_BY_VALUE  1
#define FE_BY_REF    2

#define FE_FUNCTION_IS_DIRECTIVE 42

FERITE_API FeriteFunction *ferite_create_internal_function( FeriteScript *script, char *name );
FERITE_API FeriteFunction *ferite_create_external_function( FeriteScript *script, char *name, void *f, char *description );
FERITE_API FeriteFunction *ferite_create_alias_function( FeriteScript *script, FeriteFunction *to );
FERITE_API FeriteFunction *ferite_function_get( FeriteScript *script, char *name );
FERITE_API FeriteOp **ferite_function_get_compiled_code( FeriteScript *script, char *name );
FERITE_API void ferite_delete_function_list( FeriteScript *script, FeriteFunction *funcs );
FERITE_API void ferite_function_to_external( FeriteScript *script, FeriteFunction *func );
FERITE_API void ferite_delete_function_hash( FeriteScript *script, FeriteHash *hash );
FERITE_API int ferite_check_params( FeriteScript *script, FeriteVariable **params, FeriteFunction *function );
FERITE_API FeriteFunction *ferite_function_dup( FeriteScript *script, FeriteFunction *function, FeriteClass *container );
FERITE_API char *ferite_function_generate_sig_string( FeriteScript *script, FeriteFunction *f );
FERITE_API int ferite_compare_functions( FeriteScript *script, FeriteFunction *f1, FeriteFunction *f2 );

#endif /* _FERITE_FUNCTION_H_ */
