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

#ifndef __FERITE_NS_H__
#define __FERITE_NS_H__

#define FENS_NS  1
#define FENS_VAR 2
#define FENS_FNC 3
#define FENS_CLS 4
#define FENS_PARENT_NS 5

FERITE_API FeriteNamespace *ferite_register_namespace( FeriteScript *script,  char *name, FeriteNamespace *parent );
FERITE_API int ferite_delete_namespace( FeriteScript *script,  FeriteNamespace *ns );
FERITE_API FeriteNamespaceBucket *ferite_register_namespace_element( FeriteScript *script,  FeriteNamespace *ns, char *name, int type, void *data );
FERITE_API FeriteNamespaceBucket *ferite_namespace_element_exists( FeriteScript *script,  FeriteNamespace *ns, char *name );
FERITE_API FeriteNamespaceBucket *ferite_find_namespace( FeriteScript *script,  FeriteNamespace *parent, char *obj, int type );
FERITE_API void *ferite_find_namespace_element_contents( FeriteScript *script,  FeriteNamespace *parent, char *obj, int type );
FERITE_API int ferite_delete_namespace_element_from_namespace( FeriteScript *script,  FeriteNamespace *ns, char *name );
FERITE_API int ferite_rename_namespace_element( FeriteScript *script, FeriteNamespace *ns, char *from, char *to );
FERITE_API FeriteFunction *ferite_register_ns_function( FeriteScript *script,  FeriteNamespace *ns, FeriteFunction *f );
FERITE_API FeriteVariable *ferite_register_ns_variable( FeriteScript *script,  FeriteNamespace *ns, char *name, FeriteVariable *var );
FERITE_API FeriteClass *ferite_register_ns_class( FeriteScript *script, FeriteNamespace *ns, FeriteClass *classt );
FERITE_API FeriteNamespace *ferite_namespace_dup( FeriteScript *script, FeriteNamespace *ns, FeriteNamespace *parent );
FERITE_API FeriteNamespaceBucket *ferite_namespace_bucket_dup( FeriteScript *script, FeriteNamespaceBucket *nsb, FeriteNamespace *parent );
FERITE_API char *ferite_namespace_bucket_type_to_str( FeriteScript *script, int type );

FE_NATIVE_FUNCTION( ferite_namespace_item_rename );

#endif /* __FERITE_NS_H__ */
