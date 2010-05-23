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

#ifndef __FERITE_H__
#define __FERITE_H__

#define FERITE_API  extern
#ifdef VCWIN32
# undef  FERITE_API
# ifdef FERITELIB_EXPORTS
#  define FERITE_API __declspec(dllexport)
# else
#  define FERITE_API __declspec(dllimport)
# endif
#endif

#define FE_USE_GENERATIONAL_GC 1

#include <ferite/freq.h>
#include <ferite/fstructs.h>

#ifdef __cplusplus
extern "C"
{
#endif
  
#define FERITE_NAME       "ferite"
#define FERITE_VERSION    "1.1.18"
#define FERITE_MAJOR      1
#define FERITE_MINOR      1
#define FERITE_RELEASE    18
   
FERITE_API int           ferite_init( int argc, char **argv );
FERITE_API int           ferite_deinit();
FERITE_API void          ferite_set_script_argv( int argc, char **argv );
FERITE_API FeriteScript *ferite_new_script();
FERITE_API int           ferite_script_load( FeriteScript *script, char *filename );
FERITE_API int           ferite_script_load_from_stream( FeriteScript *script, FILE *file );
FERITE_API int           ferite_cache_toggle( int state );
FERITE_API FeriteScript *ferite_script_compile( char *filename );
FERITE_API FeriteScript *ferite_script_compile_with_path( char *filename, char **paths );
FERITE_API int           ferite_script_execute( FeriteScript *script );
FERITE_API FeriteScript *ferite_duplicate_script( FeriteScript *script );
FERITE_API int           ferite_script_delete( FeriteScript *script );
FERITE_API void          ferite_show_help();
   
#define ferite_register_class( script, ns, name ) ferite_register_inherited_class( script, ns, name, NULL )
FERITE_API FeriteClass *ferite_register_inherited_class( FeriteScript *script, FeriteNamespace *ns, char *name, char *parent );
FERITE_API int ferite_register_class_function( FeriteScript *script, FeriteClass *classptr, FeriteFunction *f, int is_static );
FERITE_API int ferite_register_class_variable( FeriteScript *script, FeriteClass *classptr, char *name, FeriteVariable *variable, int is_static );
   
   /* user functions for getting parameters */
FERITE_API int ferite_get_parameters( FeriteVariable **list, int num_args, ... );
FERITE_API int ferite_get_parameter_count( FeriteVariable **list );
FERITE_API void *ferite_get_parameter( FeriteVariable **list, int num );
FERITE_API int ferite_get_required_parameter_list_size( FeriteScript *script, FeriteFunction *function );
   
   /* macros to cover internal functions */
#define fe_new_lng( name, val )             ferite_create_number_long_variable( script, name, val, FE_ALLOC )
#define fe_new_lng_static( name, val )    ferite_create_number_long_variable( script, name, val, FE_STATIC )
#define fe_new_dbl( name, val )             ferite_create_number_double_variable( script, name, val, FE_ALLOC )
#define fe_new_dbl_static( name, val )    ferite_create_number_double_variable( script, name, val, FE_STATIC )
#define fe_new_str( name, val, len, encoding )  ferite_create_string_variable_from_ptr( script, name, val, len, encoding, FE_ALLOC )
#define fe_new_str_static( name, val, len, encoding ) ferite_create_string_variable_from_ptr( script, name, val, len, encoding, FE_STATIC )
#define fe_new_obj( name )                  ferite_create_object_variable( script, name, FE_ALLOC )
#define fe_new_obj_static( name )    ferite_create_object_variable( script, name, FE_STATIC )
#define fe_new_array( name, size )          ferite_create_uarray_variable( script, name, size, FE_ALLOC )
#define fe_new_array_static( name, size )  ferite_create_uarray_variable( script, name, size, FE_STATIC )
#define fe_new_void( name )                 ferite_create_void_variable( script, name, FE_ALLOC )
#define fe_new_void_static( name )    ferite_create_void_variable( script, name, FE_STATIC )

#define FE_STRLEN( var )      VAS(var)->length
#define FE_STR2PTR( var )      VAS(var)->data
   
#define fe_create_ns_fnc( script, namespace, name, function, signature ) ferite_register_ns_function( script, namespace, ferite_create_external_function( script, name, function, signature ) )
#define fe_create_cls_fnc( script, class, name, function, signature, static ) ferite_register_class_function( script, class, ferite_create_external_function( script, name, function, signature ), static )
   
   /* macros to make writing functions easier */
#define FDECREF( obj ) do { (obj)->refcount--; if( (obj)->refcount < 0 ) ferite_debug_catch((obj),(obj)->refcount); } while(0)
#define FINCREF( obj ) (obj)->refcount++
#define FDECREFI( obj, amount ) do { (obj)->refcount -= (amount); if( (obj)->refcount < 0 ) ferite_debug_catch((obj),(obj)->refcount); } while(0)
#define FINCREFI( obj, amount ) (obj)->refcount += (amount)

   /* these are for return values */
#define FE_FALSE 0
#define FE_TRUE  !FE_FALSE
   
#define FE_NATIVE_FUNCTION( func_name ) \
	FERITE_API FeriteVariable *func_name ( FeriteScript *script, void *__container__, FeriteObject *current_recipient, FeriteFunction *function, FeriteVariable **params )

#define FE_CONTAINER_TO_OBJECT  (FeriteObject*)__container__
#define FE_CONTAINER_TO_CLASS   (FeriteClass*)__container__
#define FE_CONTAINER_TO_NS      (FeriteNamespace*)__container__
    
#define FE_RETURN_VAR( var ) do { \
   FeriteVariable *ferite_function_return = var; \
   FUD(("Variable %p being returned from function %s\n", var, __FUNCTION__)); \
   if( !FE_VAR_IS_DISPOSABLE( ferite_function_return ) ) \
     MARK_VARIABLE_AS_DISPOSABLE( ferite_function_return ); \
   return ferite_function_return; } while(0)
   
#define FE_RETURN_LONG( val ) \
   FE_RETURN_VAR( ferite_create_number_long_variable( script,"external_function_return_int_" , val, FE_STATIC ) )

#define FE_RETURN_BOOL( val ) \
   FE_RETURN_VAR( ferite_create_boolean_variable( script,"external_function_return_bool_" , (val ? FE_TRUE : FE_FALSE), FE_STATIC ) )
   
#define FE_RETURN_DOUBLE( val ) \
   FE_RETURN_VAR( ferite_create_number_double_variable( script, "external_function_return_float_" , val, FE_STATIC ) )
   
#define FE_RETURN_VOID \
   FE_RETURN_VAR( ferite_create_void_variable( script, "external_function_return_void_" , FE_STATIC ) );

#define FE_RETURN_STR( str, freeme ) do { \
   FeriteVariable *ferite_function_return= ferite_create_string_variable( script, "external_function_return_string_" , str, FE_STATIC ); \
   if( freeme == FE_TRUE ) { ferite_str_destroy( script, str ); } \
   MARK_VARIABLE_AS_DISPOSABLE( ferite_function_return ); \
   return ferite_function_return; } while(0)

#define FE_RETURN_CSTR( str, freeme ) do { \
    char *__real__str__ = str; \
    FeriteVariable *ferite_function_return= ferite_create_string_variable_from_ptr( script, "external_function_return_string_" , __real__str__, 0, FE_CHARSET_DEFAULT, FE_STATIC ); \
    if( freeme == FE_TRUE ) { ffree( __real__str__ ); } \
    MARK_VARIABLE_AS_DISPOSABLE( ferite_function_return ); \
    return ferite_function_return; } while(0)

#define FE_RETURN_TRUE    FE_RETURN_VAR( ferite_create_boolean_variable( script,"external_function_return_bool-true_" , FE_TRUE, FE_STATIC ) )
#define FE_RETURN_FALSE   FE_RETURN_VAR( ferite_create_boolean_variable( script,"external_function_return_bool-false_" , FE_FALSE, FE_STATIC ) )

#define FE_RETURN_NULL_OBJECT \
   FE_RETURN_VAR( ferite_create_object_variable( script, "external_function_return_null_object_" , FE_STATIC ) )

#define FE_RETURN_OBJECT( val ) do { \
   FeriteVariable *ferite_function_return= ferite_create_object_variable( script, "external_function_return_object_" , FE_STATIC ); \
   if( val != NULL ) { VAO(ferite_function_return) = val; FINCREF(val); } \
   MARK_VARIABLE_AS_DISPOSABLE( ferite_function_return ); \
   return ferite_function_return; } while(0)
   
#define FE_RETURN_ARRAY( val ) do { \
   FeriteVariable *ferite_function_return= ferite_create_uarray_variable( script, "external_function_return_array_" , val->size, FE_STATIC ); \
   ferite_uarray_destroy( script, val ); \
   VAUA(ferite_function_return) = ferite_uarray_dup( script, val ); \
   MARK_VARIABLE_AS_DISPOSABLE( ferite_function_return ); \
   return ferite_function_return; } while(0)
   
#define FE_CLEAN_STRING(s)   fcalloc( s, sizeof(char) )

#define FE_NoScript          NULL

#include <ferite/famt.h>
#include <ferite/fcontainer.h>
#include <ferite/fthread.h>
#include <ferite/fglobals.h>
#include <ferite/fhash.h>
#include <ferite/fdebug.h>
#include <ferite/fstack.h>
#include <ferite/fns.h>
#include <ferite/fvariables.h>
#include <ferite/fregex.h>
#include <ferite/fops.h>
#include <ferite/fopcode.h>
#include <ferite/futils.h>
#include <ferite/fstring.h>
#include <ferite/fmem.h>
#include <ferite/fgc.h>
#include <ferite/ffunction.h>
#include <ferite/fmodule.h>
#include <ferite/fcompile.h>
#include <ferite/foop.h>
#include <ferite/ferror.h>
#include <ferite/farray.h>
#include <ferite/fbuffer.h>

#include <ferite/fobj.h> /* As this is the native class 'Obj' we need the macros here for compilation!*/    

#ifdef __cplusplus
}
#endif

#endif /* __FERITE_H__ */
