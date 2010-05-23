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

#ifndef __FERITE_VARIABLES_H__
#define __FERITE_VARIABLES_H__

#define F_VAR_TYPE( EXPR )        (EXPR)->type
#define F_CAST_VAR_TYPE( EXPR )   ((FeriteVariable*)(EXPR))->type

#define F_VAR_UNDEFINED      0
#define F_VAR_VOID           1
#define F_VAR_LONG           2
#define F_VAR_STR            3
#define F_VAR_DOUBLE         4
#define F_VAR_OBJ            5
#define F_VAR_NS             6  /* hidden internal to make things work */
#define F_VAR_CLASS          7  /* blart, and again :) */
#define F_VAR_UARRAY         8
#define F_VAR_NUM            F_VAR_LONG
#define F_VAR_BOOL           9

#define FE_ALLOC  0
#define FE_STATIC  1

#define VAB( var )  var->data.lval
#define VAI( var )  var->data.lval
#define VAF( var )  var->data.dval
#define VAS( var )  var->data.sval
#define VAP( var )  var->data.pval
#define VAO( var )  var->data.oval
#define VAUA( var ) var->data.aval
#define VAN( var )  var->data.nval
#define VAC( var )  var->data.cval

#define PTR2VAR( var ) ((FeriteVariable *)var)
#define PARGX( var )   ((FeriteRegex *)(var))

#define MARK_VARIABLE_AS_DISPOSABLE( var )    if( var != NULL ) var->flags |= FE_FLAG_DISPOSABLE;
#define UNMARK_VARIABLE_AS_DISPOSABLE( var )  if( var != NULL ) var->flags &= ~FE_FLAG_DISPOSABLE;
#define MARK_VARIABLE_AS_COMPILED( var )      if( var != NULL ) var->flags |= FE_FLAG_COMPILED;
#define UNMARK_VARIABLE_AS_COMPILED( var )    if( var != NULL ) var->flags &= ~FE_FLAG_COMPILED;
#define MARK_VARIABLE_AS_FINAL( var )         if( var != NULL ) var->flags |= FE_FLAG_FINAL;
#define UNMARK_VARIABLE_AS_FINAL( var )       if( var != NULL ) var->flags &= ~FE_FLAG_FINAL;
#define MARK_VARIABLE_AS_FINALSET( var )      if( var != NULL ) var->flags |= FE_FLAG_FINALSET;
#define UNMARK_VARIABLE_AS_FINALSET( var )    if( var != NULL ) var->flags &= ~FE_FLAG_FINALSET;
#define MARK_VARIABLE_AS_STATIC( var )        if( var != NULL ) var->flags |= FE_FLAG_IS_STATIC;
#define UNMARK_VARIABLE_AS_STATIC( var )      if( var != NULL ) var->flags &= ~FE_FLAG_IS_STATIC;
#define MARK_VARIABLE_AS_PLACEHOLDER( var )   if( var != NULL ) var->flags |= FE_FLAG_PLACEHOLDER;
#define UNMARK_VARIABLE_AS_PLACEHOLDER( var ) if( var != NULL ) var->flags &= ~FE_FLAG_PLACEHOLDER;
#define MARK_VARIABLENAME_AS_STATIC( var )    if( var != NULL ) var->flags |= FE_FLAG_STATIC_NAME;
#define UNMARK_VARIABLENAME_AS_STATIC( var )  if( var != NULL ) var->flags &= ~FE_FLAG_STATIC_NAME;

#define FE_VAR_IS_DISPOSABLE( var )  ( var->flags & FE_FLAG_DISPOSABLE )
#define FE_VAR_IS_COMPILED( var )  ( var->flags & FE_FLAG_COMPILED )
#define FE_VAR_IS_FINAL( var )    ( var->flags & FE_FLAG_FINAL )
#define FE_VAR_IS_FINALSET( var )  ( var->flags & FE_FLAG_FINALSET )
#define FE_VAR_IS_STATIC( var )    ( var->flags & FE_FLAG_IS_STATIC )
#define FE_VAR_IS_PLACEHOLDER( var )  ( var->flags & FE_FLAG_PLACEHOLDER )
#define FE_VAR_NAME_IS_STATIC( var )  ( var->flags & FE_FLAG_STATIC_NAME )
#define FE_VAR_IS_NUMBER( var )    (var->type == F_VAR_LONG || var->type == F_VAR_DOUBLE || var->type == F_VAR_BOOL)

FERITE_API FeriteVariable     *ferite_variable_alloc(FeriteScript *script, char *name, int alloc);
FERITE_API void                ferite_variable_destroy( FeriteScript *script, FeriteVariable *var );
FERITE_API FeriteVariable     *ferite_get_variable_ref( FeriteScript *script, FeriteVariable *variable );

FERITE_API FeriteVariable     *ferite_create_string_variable( FeriteScript *script, char *name, FeriteString *data, int alloc );
FERITE_API FeriteVariable     *ferite_create_string_variable_from_ptr( FeriteScript *script, char *name, char *data, size_t length, int encoding, int alloc );
FERITE_API FeriteVariable     *ferite_create_boolean_variable( FeriteScript *script, char *name, long data, int alloc );
FERITE_API FeriteVariable     *ferite_create_number_long_variable( FeriteScript *script, char *name, long data, int alloc );
FERITE_API FeriteVariable     *ferite_create_number_double_variable( FeriteScript *script, char *name, double data, int alloc );
FERITE_API FeriteVariable     *ferite_create_object_variable( FeriteScript *script, char *name, int alloc );
FERITE_API FeriteVariable     *ferite_create_object_variable_with_data( FeriteScript *script, char *name, FeriteObject *o, int alloc );
FERITE_API FeriteVariable     *ferite_create_uarray_variable( FeriteScript *script, char *name, int size, int alloc );
FERITE_API FeriteVariable     *ferite_duplicate_variable( FeriteScript *script, FeriteVariable *var, void *extra );
FERITE_API FeriteVariable     *ferite_create_void_variable( FeriteScript *script, char *name, int alloc );
FERITE_API FeriteVariable     *ferite_create_class_variable( FeriteScript *script, char *name, FeriteClass *klass, int alloc );
FERITE_API FeriteVariable     *ferite_create_namespace_variable( FeriteScript *script, char *name, FeriteNamespace *ns, int alloc );
FERITE_API char               *ferite_variable_id_to_str( FeriteScript *script, int variable );
FERITE_API int                 ferite_variable_is_false( FeriteScript *script, FeriteVariable *var );

FERITE_API long                ferite_number_as_long( FeriteScript *script, FeriteVariable *var );
FERITE_API double              ferite_number_as_double( FeriteScript *script, FeriteVariable *var );
FERITE_API FeriteString       *ferite_variable_to_str( FeriteScript *script, FeriteVariable *var,int quote);
FERITE_API void                ferite_create_variable_accessors( FeriteScript *script, FeriteVariable *var, void *get, void *set, void *cleanup, void *odata );
FERITE_API void                ferite_set_variable_name( FeriteScript *script, FeriteVariable *var, char *newname );
FERITE_API void                ferite_set_static_variable_name( FeriteScript *script, FeriteVariable *var, char *newname );
FERITE_API int                 ferite_types_are_equal( FeriteScript *script, int typea, int typeb );
FERITE_API int                 ferite_variable_fast_assign( FeriteScript *script, FeriteVariable *left, FeriteVariable *right );
FERITE_API void                ferite_variable_convert_to_type( FeriteScript *script, FeriteVariable *var, int type );
FERITE_API char                ferite_variable_type_to_char( FeriteScript *script, int type );
FERITE_API void                ferite_delete_pointer( FeriteScript *script, void *p );
FERITE_API int                 ferite_fast_variable_cmp( FeriteScript *script, FeriteVariable *left, FeriteVariable *right );

FERITE_API FeriteVariableSubType *ferite_create_subtype( FeriteScript *script, short type );
FERITE_API FeriteString       *ferite_subtype_to_string( FeriteScript *script, FeriteVariableSubType *subtype );
FERITE_API FeriteVariableSubType *ferite_subtype_link( FeriteScript *script, char *type );
FERITE_API void                ferite_subtype_destroy( FeriteScript *script, FeriteVariableSubType *type );

#endif /* __FERITE_VARIABLES_H__ */

