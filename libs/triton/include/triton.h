/*
 * Copyright (C) 2001-2007 Evan Webb, Chris Ross
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

#ifndef __TRITON_H__
#define __TRITON_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#ifdef USE_DLFCN
# include <dlfcn.h>
#endif 

#define LIB_OPEN void * __triton_low_open(const char *path)
#define LIB_SYM void * __triton_low_getsym(void *handle, const char *symbol)
#define LIB_CLOSE int __triton_low_close(void *handle)
#define LIB_INIT int __triton_low_init(void)
#define LIB_DEINIT void __triton_low_deinit(void)
#define LIB_ERROR const char * __triton_low_error(void)
#define LIB_EXT const char * __triton_low_module_ext(void)

#ifndef RTLD_LAZY
# define RTLD_LAZY      0x1
#endif

#ifndef RTLD_NOW
# define RTLD_NOW       0x2
#endif

#ifndef RTLD_LOCAL
# define RTLD_LOCAL     0x4
#endif

#ifndef RTLD_GLOBAL
# define RTLD_GLOBAL    0x8
#endif

#ifndef RTLD_NOLOAD
# define RTLD_NOLOAD    0x10
#endif

#ifndef RTLD_SHARED
# define RTLD_SHARED    0x20  /* not used, the default */
#endif

#ifndef RTLD_UNSHARED
# define RTLD_UNSHARED  0x40
#endif

#ifndef RTLD_NODELETE
# define RTLD_NODELETE  0x80
#endif

#ifndef RTLD_LAZY_UNDEF
# define RTLD_LAZY_UNDEF 0x100
#endif

void * __triton_low_open(const char *path);
void * __triton_low_getsym(void *handle, const char *symbol);
int __triton_low_close(void *handle);
int __triton_low_init(void);
void __triton_low_deinit(void);
const char * __triton_low_error(void);
const char * __triton_low_module_ext(void);

/* prototypes for the functions */
int triton_init(void);
void triton_deinit(void);
void *triton_open(char *file);
void *triton_openext(char *file);
void *triton_getsym(void *h, char *f);
int triton_close(void *h);
const char *triton_error(void);
const char *triton_library_ext(void);

#endif /* __TRITON_H__ */
