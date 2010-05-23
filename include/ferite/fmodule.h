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

#ifndef __FERITE_MODULE_H__
#define __FERITE_MODULE_H__

FERITE_API FeriteModule *ferite_create_module( char *name, char *filename );
FERITE_API void ferite_destroy_module_list( FeriteModule *mod );
FERITE_API int ferite_init_module_list();
FERITE_API void ferite_deinit_module_list();
FERITE_API void ferite_clear_module_search_list();
FERITE_API int ferite_load_module( FeriteScript *script, FeriteNamespace *ns, char *name );
FERITE_API int ferite_load_native_module( char *name, FeriteScript *script );
FERITE_API int ferite_load_script_module( FeriteScript *script, char *name, int do_extension );
FERITE_API FeriteModule *ferite_module_find( char *name );
FERITE_API int ferite_unload_native_module( char *module_name, FeriteScript *script );

FERITE_API void ferite_add_library_search_path( char *path );
FERITE_API void ferite_pop_library_search_path();
FERITE_API void ferite_set_library_native_path( char *path );
FERITE_API void ferite_pop_library_native_search_path();
FERITE_API void ferite_module_register_native_function( char *lookupname, FeriteVariable *(*ptr)( FeriteScript*,void*,FeriteObject*,FeriteFunction*,FeriteVariable ** ) );
FERITE_API void ferite_delete_native_function_record( FeriteScript *script, void *data );
FERITE_API void ferite_module_delete_native_function_list();
FERITE_API void *ferite_module_find_function( char *name );

FERITE_API void ferite_module_add_preload( char *name );
FERITE_API int ferite_module_do_preload( FeriteScript *script );
FERITE_API void ferite_module_register_fake_module( char *name, void *_register, void *_unregister, void *_init, void *_deinit );

#endif /* __FERITE_MODULE_H__ */
