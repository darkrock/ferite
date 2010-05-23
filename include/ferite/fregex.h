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

#ifndef __FERITE_REGEX_H__
#define __FERITE_REGEX_H__

#define F_RGX_MATCH  1
#define F_RGX_SWAP   2
#define F_RGX_ASSIGN 3

/* intern specific options */
#define F_RGX_COMPILE    0x0001
#define F_RGX_GLOBAL     0x0002
#define F_RGX_EVAL_SWP   0x0004

FERITE_API void           *ferite_regex_malloc( size_t size);
FERITE_API void            ferite_regex_free( void *ptr );
FERITE_API void            ferite_init_regex();
FERITE_API void            ferite_deinit_regex();
FERITE_API FeriteRegex    *ferite_create_regex();
FERITE_API FeriteRegex    *ferite_regex_dup( FeriteRegex *rgx );
FERITE_API FeriteRegex    *ferite_generate_regex( FeriteScript *script, char *pattern, char *flags );
FERITE_API void            ferite_delete_regex( FeriteRegex *rgx );
FERITE_API char           *ferite_regex_swap_vars( char *rgxBuf, FeriteScript *script, FeriteExecuteRec *er );
FERITE_API void           *ferite_compile_regex( FeriteScript *script, char *pattern, int options );

#endif /* __FERITE_REGEX_H__ */
