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

#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

#include "ferite.h"

#ifdef VCWIN32
# include "snprintf.h" /* This is so that we have somethings */
#endif

/**
 * @group Error System
 * @description There are a number of things a lot of languages lack and that is error handling.
 *			  The functions in this group allow for the raising of exceptions during runtime, 
 *			  and compile time.
 */

FeriteVariable *ferite_generate_backtrace( FeriteScript *script, int skip_first )
{
	FeriteVariable *ptr = ferite_create_uarray_variable( script, "backtrace", FE_ARRAY_DEFAULT_SIZE, FE_STATIC);
	FeriteExecuteRec *exec = script->gc_stack;
	FeriteExecuteRec *start = script->gc_stack;
	FE_ENTER_FUNCTION;

#define FUNCTION_FILE( FUNCTION ) \
			(FUNCTION->native_information ? FUNCTION->native_information->file : \
				(FUNCTION->bytecode ? FUNCTION->bytecode->filename : "unknown"))

	if( skip_first ) {
		start = exec = exec->parent;
	}
	
	for( ; exec != NULL; exec = exec->parent ) {
		FeriteVariable *subArray = ferite_create_uarray_variable( script, "backtrace-subSection", FE_ARRAY_DEFAULT_SIZE, FE_STATIC);
		FeriteVariable *variableArray = ferite_create_uarray_variable( script, "backtrace-variableArray", FE_ARRAY_DEFAULT_SIZE, FE_STATIC);
		FeriteVariable *value = NULL;
		FeriteFunction *function = exec->function;
		char *name = function->name;
		char *file = FUNCTION_FILE(function);
		char *cont = (function->klass ? function->klass->name : "");
		
		if( strcmp(name, "!__start__") == 0 )
			name = "top-level-script";
		if( strncmp(cont, "!__closure__", strlen("!__closure__")) == 0 )
			cont = "closure";

		value = fe_new_str_static( "function", name, 0, FE_CHARSET_DEFAULT );
		ferite_uarray_add( script, VAUA(subArray), value, "function", FE_ARRAY_ADD_AT_END );

		value = fe_new_str_static( "file", file, 0, FE_CHARSET_DEFAULT );
		ferite_uarray_add( script, VAUA(subArray), value, "file", FE_ARRAY_ADD_AT_END );

		value = fe_new_lng_static( "line", exec->line );
		ferite_uarray_add( script, VAUA(subArray), value, "line", FE_ARRAY_ADD_AT_END );

		value = ferite_create_boolean_variable( script, "static", function->is_static, FE_STATIC );
		ferite_uarray_add( script, VAUA(subArray), value, "static", FE_ARRAY_ADD_AT_END );
		
		value = fe_new_str_static( "container", cont, 0, FE_CHARSET_DEFAULT );
		ferite_uarray_add( script, VAUA(subArray), value, "container", FE_ARRAY_ADD_AT_END );
		
		if( exec->variable_list ) {
			int i = 3; // We start at 3 becuase that is where the parameters start
			for( i = 3; i <= exec->function->localvars->stack_ptr; i++ ) {
				if( exec->variable_list[i] != NULL && exec->variable_list[i]->vname[0] != '!' )	 {
					char name_buffer[1024];
					FeriteVariable *duplicate = ferite_duplicate_variable( script, exec->variable_list[i], NULL );
					sprintf(name_buffer, "%d:%s", i, duplicate->vname);
					ferite_uarray_add( script, VAUA(variableArray), duplicate, name_buffer, FE_ARRAY_ADD_AT_END );
				}
			}
			ferite_uarray_add( script, VAUA(subArray), variableArray, "variables", FE_ARRAY_ADD_AT_END );
		}
		
		ferite_uarray_add( script, VAUA(ptr), subArray, NULL, FE_ARRAY_ADD_AT_END );
	}
	FE_LEAVE_FUNCTION(ptr);
}
/**
 * @function ferite_raise_script_error
 * @declaration void ferite_raise_script_error( FeriteScript *script, int err, char *fmt, ... )
 * @brief Raise an exception within the ferite engine.
 * @param FeriteScript *script The running script
 * @param int err	The error code
 * @param char *fmt	The format of the error string
 * @description Use the same formating codes as printf with this function
 */
void ferite_raise_script_error( FeriteScript *script, int err, char *fmt, ... )
{
	FeriteNamespaceBucket *nsb = NULL;
	FeriteVariable *global_error_object = NULL, *new_error_object = NULL, *backtrace = NULL;
	FeriteVariable *error_object_str = NULL, *error_object_num = NULL, *error_object_backtrace = NULL;
	FeriteBuffer *error_buffer = NULL;
	char *msg;
	va_list ap;

	FE_ENTER_FUNCTION;
	va_start( ap, fmt );

	error_buffer = ferite_buffer_new(script, 0);
	ferite_buffer_vprintf( script, error_buffer, fmt, &ap );
	msg = ferite_buffer_get( script, script->error, NULL );
	
	FUD(("ERROR RAISED: %s %d\n", msg, err ));

	nsb = ferite_namespace_element_exists( script, script->mainns, "err" );
	FE_ASSERT( nsb && nsb->type == FENS_VAR );
	global_error_object = nsb->data;
	script->error_state = FE_ERROR_THROWN;

	if( VAO(global_error_object) == NULL )
	{
		nsb = ferite_namespace_element_exists( script, script->mainns, "Error" );
		if( nsb == NULL )
		{
			FE_LEAVE_FUNCTION( NOWT );
			exit(1);
		}
		new_error_object = ferite_new_object( script, nsb->data, NULL );
		VAO(global_error_object) = VAO(new_error_object);
		FINCREF(VAO(global_error_object));
		ferite_variable_destroy( script, new_error_object );
	}

	error_object_str = ferite_object_get_var( script, VAO(global_error_object), "str" );
	ferite_str_set( script, VAS(error_object_str), msg, strlen(msg), FE_CHARSET_DEFAULT );
	ffree( msg );

	error_object_num = ferite_object_get_var( script, VAO(global_error_object), "num" );
	VAI(error_object_num) = err;
	
	backtrace = ferite_generate_backtrace( script, FE_FALSE );
	error_object_backtrace = ferite_object_get_var( script, VAO(global_error_object), "backtrace");
	ferite_variable_fast_assign( script, error_object_backtrace, backtrace );
	
	ferite_buffer_delete( script, error_buffer );
	
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_verror
 * @declaration void ferite_verror( FeriteScript *script, char *errormsg, va_list *ap )
 * @brief Raise an error
 * @param FeriteScript *script The script
 * @param int err The error number associated with the error
 * @param char *errormsg The error with formating codes in it
 * @param va_list *ap The list of arguments
 */
void ferite_verror( FeriteScript *script, int err, char *errormsg, va_list *ap )
{
	char *real_errormsg = fstrdup(errormsg);
	int  length = strlen(real_errormsg);
	
	FE_ENTER_FUNCTION;

	if( real_errormsg[length - 1] == '\n' ) {
		real_errormsg[length - 1] = '\0';
	}
	if( script == NULL )
	{
		vprintf(real_errormsg, *ap );
		printf("\n");
		ffree( real_errormsg );
		FE_LEAVE_FUNCTION( NOWT );
	}
	if( script->error == NULL )
		script->error = ferite_buffer_new( script, 0 );

	ferite_buffer_add_str( script, script->error, "Error: " );
	ferite_buffer_vprintf( script, script->error, real_errormsg, ap );
	ferite_buffer_add_str( script, script->error, "\n" );
	
	if( script->error_state != FE_ERROR_THROWN ) {
		if( ferite_is_executing( script ) ) {
			int len = 0, sub_length = strlen("Error: ");
			char *ptr = ferite_buffer_get( script, script->error, &len );
			char *real_ptr = ferite_replace_string( ptr, "%", "%%" );
			char *msg = fmalloc(len + 1);
			memcpy( msg, real_ptr + sub_length, len - sub_length );
			ferite_raise_script_error( script, err, msg );
			ffree( msg );
			ffree( ptr );
			ffree( real_ptr );
		}
		script->error_state = FE_ERROR_THROWN;
	}
	ffree( real_errormsg );
	FE_LEAVE_FUNCTION( NOWT );
}
/**
 * @function ferite_error
 * @declaration void ferite_error( FeriteScript *script, char *errormsg, ... )
 * @brief Throw an error within the engine
 * @param FeriteScript *script   The scipt we are using
 * @param int err The error number associated with the error
 * @param char *errormsg The error information
 * @param . .. The formating values
 */
void ferite_error( FeriteScript *script, int err, char *errormsg, ... )
{
	va_list ap;

	FE_ENTER_FUNCTION;
	va_start( ap, errormsg );
	ferite_verror( script, err, errormsg, &ap);
	va_end( ap );
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_vwarning
 * @declaration void ferite_vwarning( FeriteScript *script, char *errormsg, ... )
 * @brief Display a warning message. This does not cause an exception
 * @param FeriteScript *script   The current script
 * @param char *errormsg The warning to be displayed
 * @param va_list *ap The formatting values
 */
void ferite_vwarning( FeriteScript *script, char *errormsg, va_list *ap )
{

	FE_ENTER_FUNCTION;

	if( script == NULL )
	{
		printf("ferite_warning(): script was called with NULL, this shouldn't happen\n");
		vprintf(errormsg, *ap );
/*#ifdef DEBUG*/
		printf("ferite_warning(): sleeping for gdb interruption for 10 seconds\n");
		sleep(10);
/*#endif*/
		FE_LEAVE_FUNCTION( NOWT );
	}

	if( script->warning == NULL )
	  script->warning = ferite_buffer_new( script, 0 );

	ferite_buffer_add_str( script, script->warning, "Warning: " );
	if( ferite_is_executing( script ) )
	  ferite_buffer_printf( script, script->warning, "[%s:%d] ", script->current_op_file, script->current_op_line );
	ferite_buffer_vprintf( script, script->warning, errormsg, ap );

	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_warning
 * @declaration void ferite_warning( FeriteScript *script, char *errormsg, ... )
 * @brief Display a warning message. This does not cause an exception
 * @param FeriteScript *script   The current script
 * @param char *errormsg The warning to be displayed
 * @param . .. The formatting values
 */
void ferite_warning( FeriteScript *script, char *errormsg, ... )
{
	va_list ap;

	FE_ENTER_FUNCTION;
	va_start( ap, errormsg );

 	ferite_vwarning( script, errormsg, &ap );

	va_end( ap );
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_get_error_log
 * @declaration char *ferite_get_error_log( FeriteScript *script )
 * @brief Get a null terminated string containing the error and warning logs on a script
 * @param FeriteScript *script The script to get the errror logs from
 * @return A null terminated string, you will need to ffree the string when done to prevent memory leak
 */
char *ferite_get_error_log( FeriteScript *script )
{
	int err_size = 0, warn_size = 0;
	char *msg, *err_ptr, *warn_ptr;

	FE_ENTER_FUNCTION;
	if( script->error )
	  err_ptr = ferite_buffer_get( script, script->error, &err_size );
	else
	  err_ptr = fstrdup("");
	if( script->warning )
	  warn_ptr = ferite_buffer_get( script, script->warning, &warn_size );
	else
	  warn_ptr = fstrdup("");
	msg = fmalloc( err_size + warn_size + 1 );
	strcpy( msg, warn_ptr );
	strcat( msg, err_ptr );
	ffree( err_ptr );
	ffree( warn_ptr );
	FE_LEAVE_FUNCTION( msg );
}

/**
 * @function ferite_get_error_string
 * @declaration char *ferite_get_error_string( FeriteScript *script )
 * @brief Get a null terminated string containing the error log
 * @param FeriteScript *script The script whose errors are required
 * @return  A null terminated string, you will need to ffree the string when done to prevent memory leak
 */
char *ferite_get_error_string( FeriteScript *script )
{
	char *msg;
	FE_ENTER_FUNCTION;
	if( script->error )
	  msg = ferite_buffer_get( script, script->error, NULL );
	else
	  msg = fstrdup("");
	FE_LEAVE_FUNCTION( msg );
}

/**
 * @function ferite_get_warning_string
 * @declaration char *ferite_get_warning_string( FeriteScript *script )
 * @brief Get a null terminated string containing the warning log
 * @param FeriteScript *script The script whose warnings are required
 * @return  A null terminated string, you will need to ffree the string when done to prevent memory leak
 */
char *ferite_get_warning_string( FeriteScript *script )
{
	char *msg;
	FE_ENTER_FUNCTION;
	if( script->warning )
	  msg = ferite_buffer_get( script, script->warning, NULL );
	else
	  msg = fstrdup("");
	FE_LEAVE_FUNCTION( msg );
}

/**
 * @function ferite_reset_warnings
 * @declaration void ferite_reset_warnings( FeriteScript *script )
 * @brief Reset any warnings on the script
 * @param FeriteScript *script The script to check
 */
void ferite_reset_warnings( FeriteScript *script )
{
	FE_ENTER_FUNCTION;
	if( script->warning != NULL )
	{
		ferite_buffer_delete( script, script->warning );
		script->warning = NULL;
	}
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_reset_errors
 * @declaration void ferite_reset_errors( FeriteScript *script )
 * @brief Reset any errors on the script
 * @param FeriteScript *script The script to check
 */
void ferite_reset_errors( FeriteScript *script )
{
	FE_ENTER_FUNCTION;
	if( script->error != NULL )
	{
		ferite_buffer_delete( script, script->error );
		script->error = NULL;
		script->error_state = 0;
	}
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_has_compile_error
 * @declaration int ferite_has_compile_error( FeriteScript *script )
 * @brief Check to see if the script has had a compilation error
 * @param FeriteScript *script The script to check
 * @return FE_TRUE if a compilation error occured, FE_FALSE otherwise
 */
int ferite_has_compile_error( FeriteScript *script )
{
	FE_ENTER_FUNCTION;
	FE_LEAVE_FUNCTION( ( script->error == NULL ) ? FE_FALSE : FE_TRUE );
}

/**
 * @function ferite_has_warnings
 * @declaration int ferite_has_warnings( FeriteScript *script )
 * @brief Check to see if the script has warnings
 * @param FeriteScript *script The script to check
 * @return FE_TRUE if there are warnings, FE_FALSE otherwise
 */
int ferite_has_warnings( FeriteScript *script )
{
	FE_ENTER_FUNCTION;
	FE_LEAVE_FUNCTION( (script->warning == NULL) ? FE_FALSE : FE_TRUE );
}

/**
 * @function ferite_has_runtime_error
 * @declaration int ferite_has_runtime_error( FeriteScript *script )
 * @brief Check to see if the script has had a runtime error
 * @param FeriteScript *script The script to check
 * @return FE_TRUE if a compilation error occured, FE_FALSE otherwise
 */
int ferite_has_runtime_error( FeriteScript *script )
{
	FE_ENTER_FUNCTION;
	FE_LEAVE_FUNCTION( ( script->error_state == FE_ERROR_THROWN ) ? FE_TRUE : FE_FALSE );
}

FE_NATIVE_FUNCTION( ferite_error_constructor_sn ) 
{
	FeriteObject *self = FE_CONTAINER_TO_OBJECT;
	FeriteVariable *str = ferite_object_get_var( script, self, "str" );
	FeriteVariable *num = ferite_object_get_var( script, self, "num" );
	FeriteVariable *errbacktrace = ferite_object_get_var( script, self, "backtrace");
	FeriteVariable *backtrace = NULL;

	FeriteString *pstr = VAS(params[0]);
	int pnum = VAI(params[1]);

	ferite_str_set( script, VAS(str), pstr->data, pstr->length, FE_CHARSET_DEFAULT );
	VAI(num) = pnum;

	backtrace = ferite_generate_backtrace( script, FE_TRUE );
	ferite_variable_fast_assign( script, errbacktrace, backtrace );

	FE_RETURN_VOID;
}
FE_NATIVE_FUNCTION( ferite_error_constructor_s ) 
{
	FeriteObject *self = FE_CONTAINER_TO_OBJECT;
	FeriteVariable *str = ferite_object_get_var( script, self, "str" );
	FeriteVariable *errbacktrace = ferite_object_get_var( script, self, "backtrace");
	FeriteVariable *backtrace = NULL;
	FeriteString *pstr = VAS(params[0]);
	
	ferite_str_set( script, VAS(str), pstr->data, pstr->length, FE_CHARSET_DEFAULT );
	backtrace = ferite_generate_backtrace( script, FE_TRUE );
	ferite_variable_fast_assign( script, errbacktrace, backtrace );

	FE_RETURN_VOID;
}

FE_NATIVE_FUNCTION( ferite_error_backtrace ) 
{
	FeriteObject *self = FE_CONTAINER_TO_OBJECT;
	FeriteVariable *r = NULL;
	FE_ENTER_FUNCTION;
	r = ferite_object_get_var( script, self, "backtrace" );
	FE_LEAVE_FUNCTION(r);
}
/**
 * @function ferite_init_error_system
 * @declaration void ferite_init_error_system( FeriteScript *script, FeriteNamespace *ns )
 * @brief Setup the special error handling class on a script
 * @param FeriteScript *script The current script
 * @param FeriteNamespace *ns	 The namespace in which the class should be registered
 */
void ferite_init_error_system( FeriteScript *script, FeriteNamespace *ns )
{
	FeriteClass *ferite_error_class = NULL;
	FeriteVariable *var, *errobj = NULL;
	FeriteNamespaceBucket *nsb = NULL;
	
	FE_ENTER_FUNCTION;
	ferite_error_class = ferite_register_class( script, ns, "Error" );
	ferite_register_class_variable( script, ferite_error_class, "num", ferite_create_number_long_variable( script, "num", 0, FE_STATIC ), 0 );
	ferite_register_class_variable( script, ferite_error_class, "str", ferite_create_string_variable( script, "str", NULL, FE_STATIC ), 0 );
	ferite_register_class_variable( script, ferite_error_class, "backtrace", ferite_create_uarray_variable( script, "backtrace", 32, FE_STATIC ), 0 );

	fe_create_cls_fnc( script, ferite_error_class, "constructor", ferite_error_constructor_sn, "sn", FE_FALSE );
	fe_create_cls_fnc( script, ferite_error_class, "constructor", ferite_error_constructor_s, "s", FE_FALSE );
	fe_create_cls_fnc( script, ferite_error_class, "backtrace", ferite_error_backtrace, "", FE_FALSE );
	
	nsb = ferite_find_namespace( script, script->mainns, "err", FENS_VAR );
	if( nsb != NULL ) {
		errobj = ferite_build_object( script, ferite_error_class );
		var = nsb->data;
		VAO(var) = VAO(errobj);
		VAO(errobj) = NULL;
		ferite_variable_destroy( script, errobj );
	}
	
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_set_error
 * @declaration void ferite_set_error( FeriteScript *script, int num, char *fmt, ... )
 * @brief Same as ferite_error except this wont raise an exception at runtime
 */
void ferite_set_error( FeriteScript *script, int num, char *fmt, ... )
{
	FeriteNamespaceBucket *nsb = NULL;
	FeriteVariable *global_error_object = NULL, *new_error_object = NULL;
	FeriteVariable *errstr = NULL, *erno = NULL;
	va_list ap;
	char *buf = NULL;

	FE_ENTER_FUNCTION;

	if( !script->is_being_deleted && (script->parent == NULL || !script->parent->is_being_deleted) )
	{
		buf = fmalloc( 4096 );
		va_start( ap, fmt );
		vsprintf( buf, fmt, ap );

		nsb = ferite_namespace_element_exists( script, script->mainns, "err" );
		FE_ASSERT( nsb && nsb->type == FENS_VAR );
		global_error_object = nsb->data;

		if( VAO(global_error_object) == NULL )
		{
			nsb = ferite_namespace_element_exists( script, script->mainns, "Error" );
			new_error_object = ferite_new_object( script, nsb->data, NULL );
			VAO(global_error_object) = VAO(new_error_object);
			FINCREF(VAO(global_error_object));
			ferite_variable_destroy( script, new_error_object );
		}

		errstr = ferite_object_get_var( script, VAO(global_error_object), "str" );
		ferite_str_set( script, VAS(errstr), buf, strlen(buf), FE_CHARSET_DEFAULT );

		erno = ferite_object_get_var( script, VAO(global_error_object), "num" );
		VAI(erno) = num;

		ffree( buf );
		va_end( ap );
	}
	FE_LEAVE_FUNCTION( NOWT );
}
/* This function will never return, set ferite_assert_debug to generate
 * a segfault to get a backtrace */
int ferite_assert_debug = 0;
void ferite_assert( char *fmt, ... )
{
	char *p = NULL;
	va_list ap;
	va_start( ap, fmt );
	ferite_vwarning( NULL, fmt, &ap );
	va_end( ap );
	if( ferite_assert_debug )
	  *p = '\0';
	exit( -1 );
}

/**
 * @end
 */
