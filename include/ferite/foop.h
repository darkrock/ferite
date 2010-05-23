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

#ifndef __FERITE_CLASS_H__
#define __FERITE_CLASS_H__

#define FE_ITEM_IS_PRIVATE   0
#define FE_ITEM_IS_PROTECTED 1
#define FE_ITEM_IS_PUBLIC    2
#define FE_ITEM_IS_ABSTRACT  3
#define FE_ITEM_IS_FINAL     4
#define FE_ITEM_IS_PROTOCOL  5

/* Class */
FERITE_API FeriteClass     *ferite_class_variable_class( FeriteScript *script, FeriteClass *klass, char *name );
FERITE_API int              ferite_class_has_function(FeriteScript *script, FeriteClass *cls, char *name);
FERITE_API FeriteVariable  *ferite_class_get_var( FeriteScript *script, FeriteClass *klass, char *name );
FERITE_API int              ferite_class_is_subclass( FeriteClass *klass, FeriteClass *subklass );
FERITE_API FeriteClass     *ferite_class_dup( FeriteScript *script, FeriteClass *klass, FeriteNamespace *container );
FERITE_API void             ferite_class_call_static_constructor( FeriteScript *script, FeriteClass *klass );
FERITE_API void             ferite_class_call_static_destructor( FeriteScript *script, FeriteClass *klass );
FERITE_API void             ferite_class_finish( FeriteScript *script, FeriteClass *klass );
FERITE_API FeriteFunction  *ferite_class_get_function(FeriteScript *script, FeriteClass *cls, char *name);
FERITE_API FeriteFunction  *ferite_class_get_function_for_params( FeriteScript *script, FeriteClass *klass, char *name, FeriteVariable **params );

/* Deleting things */
FERITE_API void             ferite_delete_object_variable_list( FeriteScript *script, FeriteObjectVariable *ov );
FERITE_API void             ferite_delete_class( FeriteScript *script, FeriteClass *classp );
FERITE_API void             ferite_delete_object( FeriteObject *obj );
FERITE_API void             ferite_delete_class_object( FeriteScript *script, FeriteObject *obj, int do_destructor );
FERITE_API void             ferite_delete_objects( FeriteObject *obj );

/* Duplication */
FERITE_API FeriteObjectVariable *ferite_duplicate_object_variable_list( FeriteScript *script, FeriteClass *klass );

/* Finding things */
FERITE_API FeriteClass     *ferite_find_class( FeriteScript *script, FeriteNamespace *ns, char *name );
FERITE_API long             ferite_find_class_id( FeriteScript *script, FeriteNamespace *ns, char *name );
FERITE_API FeriteObject    *ferite_find_object( FeriteScript *script, FeriteObject *parent, char *obj );
FERITE_API FeriteFunction  *ferite_find_constructor( FeriteScript *script, FeriteObject *object, FeriteVariable **params );
FERITE_API FeriteFunction  *ferite_find_static_constructor( FeriteScript *script, FeriteClass *klass, FeriteVariable **params );
FERITE_API char            *ferite_generate_namespace_fqn( FeriteScript *script, FeriteNamespace *ns );
FERITE_API char            *ferite_generate_class_fqn( FeriteScript *script, FeriteClass *klass );

/* Objects */
FERITE_API FeriteClass     *ferite_object_variable_class( FeriteScript *script, FeriteObject *object, char *name );
FERITE_API FeriteVariable  *ferite_object_call_super( FeriteScript *script, FeriteVariable *container, FeriteVariable **params );
FERITE_API int              ferite_object_is_subclass( FeriteObject *obj, char *name );
FERITE_API void             ferite_object_set_var(FeriteScript* script,FeriteObject* obj, char *id, FeriteVariable *data);
FERITE_API FeriteVariable  *ferite_object_get_var( FeriteScript *script, FeriteObject *object, char *name );
FERITE_API FeriteFunction  *ferite_object_get_function( FeriteScript *script, FeriteObject *object, char *name );
FERITE_API FeriteFunction  *ferite_object_get_function_for_params( FeriteScript *script, FeriteObject *object, char *name, FeriteVariable **params );
FERITE_API int              ferite_object_has_var(FeriteScript* script, FeriteObject* obj, char *id);
    
/* State */
FERITE_API char            *ferite_state_to_str( int state );

#endif /* __FERITE_CLASS_H__ */
