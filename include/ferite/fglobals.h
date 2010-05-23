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

#ifndef __FERITE_GLOBALS_H__
#define __FERITE_GLOBALS_H__

#ifdef WIN32
#pragma data_seg( ".GLOBALS" )
#endif

FERITE_API int ferite_call_level;
FERITE_API int ferite_show_debug;
FERITE_API int ferite_hide_mem_use;
FERITE_API int ferite_use_mm_with_pcre;
FERITE_API int ferite_is_strict;
FERITE_API int ferite_show_partial_implementation;
FERITE_API FeriteVariable *ferite_ARGV;

FERITE_API void  (*ferite_memory_init)(void);
FERITE_API void  (*ferite_memory_deinit)(void);
FERITE_API void *(*ferite_malloc)( size_t size, char *name, int line, FeriteScript *script );
FERITE_API void *(*ferite_calloc)( size_t size, size_t blk_size, char *name, int line, FeriteScript *script );
FERITE_API void *(*ferite_realloc)( void *ptr, size_t size, FeriteScript *script );
FERITE_API void  (*ferite_free)( void *ptr, char *file, int line, FeriteScript *script );

FERITE_API void  (*ferite_init_gc)( FeriteScript *script );
FERITE_API void  (*ferite_deinit_gc)( FeriteScript *script );
FERITE_API void  (*ferite_add_to_gc)( FeriteScript *script, FeriteObject *obj );
FERITE_API void  (*ferite_check_gc)( FeriteScript *script );
FERITE_API void  (*ferite_merge_gc)( FeriteScript *script, void *gc );

#ifdef WIN32
#pragma data_seg()
#endif

/* these allow you to tweak the size of hashes */

#define FE_ARRAY_DEFAULT_SIZE            32

#define FE_CLASS_VARIABLE_HASH_SIZE      36
#define FE_CLASS_FUNCTION_HASH_SIZE      36

#define FE_COMPILER_INTERNAL_STACK_SIZE  20

#define FE_EXECUTOR_STACK_SIZE           32
#define FE_DEEPEST_STACK_LEVEL           1024

#define FE_FUNCTION_PARAMETER_MAX_SIZE   32
#define FE_FUNCTION_OPCODE_INIT_SIZE     32
#define FE_FUNCTION_VARIABLE_SIZE        15 

#define FE_GC_INITIAL_SIZE               50
#define FE_GC_RUN_AFTER_OPS              10000

#define FE_NAMESPACE_INIT_SIZE           32

#define FE_SCANNER_STACK_SIZE            10

#define FE_SCRIPT_TOP_LEVEL_NS_SIZE      32
#define FE_SCRIPT_EXEC_STACK_SIZE        32

#define FE_SEARCH_PATH_STACK_SIZE        5

#define FE_CACHE_SIZE                    100

#endif /* __FERITE_GLOBALS_H__ */
