/*
 * Copyright (C) 2000-2010 Chris Ross and various contributors
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

/**
* @group Compiler
 * @description Not all the compiler functions are documented, this is because they are internal
 *			  and dont concern anyone but the compiler hackers. If you are a compiler hacker
 *			  you should not need function descriptions :)
 */

#include "ferite.h"
#include <setjmp.h>
#include "aphex.h"
#include "fcache.h"

typedef struct _ferite_tag
{
	char *name;
	void *value;
} FeriteTag;
typedef struct _ferite_directive
{
	char *name;
	FeriteStack *values;
} FeriteDirective;

/* these are used to keep track of verious bit's and pieces, and makes
* it possible to compile ferite in one pass rather than several passes
* like other langyages :) */
FeriteStack *ferite_fwd_look_stack = NULL;
FeriteStack *ferite_bck_look_stack = NULL;
FeriteStack *ferite_break_look_stack = NULL;
FeriteStack *ferite_compiled_arrays_stack = NULL;
FeriteStack *ferite_argcount_stack = NULL;
FeriteStack *ferite_directive_stack = NULL;
FeriteStack *ferite_previous_directives_stack = NULL;

/* mwhahhaa, we can keep track of these things :) */
FeriteStack *ferite_compile_stack = NULL;
FeriteCompileRecord *ferite_current_compile = NULL;

/* make life easier :) */
#define CURRENT_NAMESPACE  ferite_current_compile->ns
#define CURRENT_FUNCTION   ferite_current_compile->function
#define CURRENT_CLASS	   ferite_current_compile->cclass
#define CURRENT_VARS	   ferite_current_compile->variables
#define CURRENT_SCRIPT	   ferite_current_compile->script
#define CURRENT_VARS_HASH  ferite_current_compile->local_variable_hash
#define CURRENT_SHAD_VARS  ferite_current_compile->shadowed_local_variables
#define CURRENT_SCOPE_FRM  ferite_current_compile->local_scope_frame
#define CURRENT_IN_CLOSURE ferite_current_compile->in_closure

#define STOP_COMPILE()	 { longjmp( ferite_compiler_jmpback, 1 ); }

/* stolen from ferite_scanner.l -> so we can report errors on files */
extern char   *ferite_scanner_file;
extern int	 ferite_scanner_lineno;
int			ferite_compile_error = 0;
int			ferite_compile_finishing_class = FE_FALSE;
jmp_buf		ferite_compiler_jmpback;
int			ferite_keep_native_function_data = 0;
int			ferite_compiler_current_block_depth = 0;
AphexMutex	*ferite_compiler_lock = NULL;
int			ferite_closure_count = 0;

void ferite_init_compiler()
{
	FE_ENTER_FUNCTION;
	if( ferite_compiler_lock == NULL )
		ferite_compiler_lock = aphex_mutex_recursive_create();
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_deinit_compiler()
{
	FE_ENTER_FUNCTION;
	if( ferite_compiler_lock != NULL )
	{
		aphex_mutex_destroy( ferite_compiler_lock );
		ferite_compiler_lock = NULL;
	}
	FE_LEAVE_FUNCTION( NOWT );
}

FeriteCompileRecord *ferite_compile_record_alloc()
{
	FeriteCompileRecord *ptr = fmalloc_ngc(sizeof(FeriteCompileRecord));
	ptr->ns = NULL;
	ptr->function = NULL;
	ptr->cclass = NULL;
	ptr->variables = NULL;
	ptr->script = NULL;
	ptr->local_variable_hash = NULL;
	ptr->shadowed_local_variables = NULL;
	ptr->local_scope_frame = NULL;
	ptr->last_script_return = NULL;
	ptr->in_closure = FE_FALSE;
	ptr->want_container_finish = FE_TRUE;
	return ptr;
}

FeriteBkRequest *ferite_create_request( FeriteOp *op, int type )
{
	FeriteBkRequest *ptr = NULL;

	FE_ENTER_FUNCTION;
	ptr = fmalloc_ngc( sizeof( FeriteBkRequest ) );
	ptr->reqop = op;
	ptr->type = type;
	FE_LEAVE_FUNCTION( ptr );
}

void ferite_destroy_request( FeriteBkRequest *ptr )
{
	FE_ENTER_FUNCTION;
	ffree_ngc( ptr );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_delete_request_stack( FeriteStack *stack )
{
	int i;
	FE_ENTER_FUNCTION;
	for( i = 0; i <= stack->stack_ptr; i++ )
	{
		if( stack->stack[i] != NULL )
		{
			ffree_ngc( stack->stack[i] );
		}
	}
	ffree_ngc( stack->stack );
	ffree_ngc( stack );
	FE_LEAVE_FUNCTION( NOWT );
}
void ferite_delete_directives_stack( FeriteStack *stack )
{
	int i,j;
	FE_ENTER_FUNCTION;
	if( stack )
	{
		for( i = 0; i <= stack->stack_ptr; i++ )
		{
			if( stack->stack[i] != NULL )
			{
				FeriteDirective *directive = stack->stack[i];
				ffree_ngc( directive->name );
				for( j = 0; j <= directive->values->stack_ptr; j++ ) {
					FeriteVariable *ptr = directive->values->stack[j];
					if( ptr != NULL )
						ferite_variable_destroy( CURRENT_SCRIPT, ptr );
				}
				ferite_delete_stack( CURRENT_SCRIPT, directive->values );
				ffree_ngc( directive );
			}
		}
		ffree_ngc( stack->stack );
		ffree_ngc( stack );
	}
	FE_LEAVE_FUNCTION( NOWT );
}
void ferite_delete_directives_collection( FeriteStack *stack )
{
	int i;
	FE_ENTER_FUNCTION;
	if( stack != NULL )
	{
		for( i = 0; i <= stack->stack_ptr; i++ )
		{
			if( stack->stack[i] != NULL )
				ferite_delete_directives_stack(stack->stack[i]);
		}
		ffree_ngc( stack->stack );
		ffree_ngc( stack );
	}
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_start_compiler( FeriteScript *new_script )
{
	FE_ENTER_FUNCTION;
	aphex_mutex_lock( ferite_compiler_lock );
	ferite_current_compile = ferite_compile_record_alloc();
	CURRENT_SCRIPT = new_script;
	CURRENT_CLASS = NULL;
	CURRENT_VARS = NULL;
	CURRENT_FUNCTION = NULL;
	CURRENT_NAMESPACE = new_script->mainns;

	ferite_fwd_look_stack = ferite_create_stack( new_script, FE_COMPILER_INTERNAL_STACK_SIZE );
	ferite_bck_look_stack = ferite_create_stack( new_script, FE_COMPILER_INTERNAL_STACK_SIZE );
	ferite_break_look_stack = ferite_create_stack( new_script, FE_COMPILER_INTERNAL_STACK_SIZE );
	ferite_compile_stack = ferite_create_stack( new_script, FE_COMPILER_INTERNAL_STACK_SIZE );
	ferite_compiled_arrays_stack = ferite_create_stack( new_script, FE_COMPILER_INTERNAL_STACK_SIZE );
	ferite_argcount_stack = ferite_create_stack( new_script, FE_COMPILER_INTERNAL_STACK_SIZE );
	ferite_previous_directives_stack = ferite_create_stack( new_script, FE_COMPILER_INTERNAL_STACK_SIZE );
	FE_LEAVE_FUNCTION(NOWT);
}

void ferite_clean_compiler()
{
	FE_ENTER_FUNCTION;
	ferite_clean_parser();
	ferite_delete_request_stack( ferite_fwd_look_stack );
	ferite_delete_request_stack( ferite_bck_look_stack );
	ferite_delete_request_stack( ferite_break_look_stack );
	ferite_delete_request_stack( ferite_compile_stack );
	ferite_delete_request_stack( ferite_argcount_stack );
	ferite_delete_stack( NULL, ferite_compiled_arrays_stack );
	ferite_delete_directives_stack( ferite_directive_stack );
	ferite_delete_directives_collection( ferite_previous_directives_stack );
	ffree_ngc( ferite_current_compile );
	ferite_fwd_look_stack = NULL;
	ferite_bck_look_stack = NULL;
	ferite_break_look_stack = NULL;
	ferite_compile_stack = NULL;
	ferite_compiled_arrays_stack = NULL;
	ferite_argcount_stack = NULL;
	ferite_previous_directives_stack = NULL;
	ferite_directive_stack = NULL;
	ferite_do_function_cleanup();
	aphex_mutex_unlock( ferite_compiler_lock );
	FE_LEAVE_FUNCTION( NOWT );
}

/**
* @function ferite_compiler_keep_native_code
 * @declaration void ferite_compiler_keep_native_code()
 * @brief Tell ferite to keep the native code that is found within the compiled scripts
 */
void ferite_compiler_keep_native_code()
{
	FE_ENTER_FUNCTION;
	ferite_keep_native_function_data = 1;
	FE_LEAVE_FUNCTION(NOWT);
}

char *ferite_compiler_build_current_path_wannotation_wfunction( int annotate, int function )
{
	FeriteCompileRecord *c = NULL;
	char *currentPathNameBuffer = fmalloc_ngc(4096);
	char *sig = NULL;
	int i;
	
	FE_ENTER_FUNCTION;
	memset(currentPathNameBuffer, '\0', 4096);
	for( i = 1; i <= ferite_compile_stack->stack_ptr; i++ )
	{
		c = ferite_compile_stack->stack[i];
		if( c != NULL )
		{
			if( c->function != NULL )
			{
				FUD(("strcat'ing function\n" ));
				strcat( currentPathNameBuffer, c->function->name );
			}
			else if( c->cclass != NULL )
			{
				FUD(("strcat'ing class name\n" ));
				strcat( currentPathNameBuffer, c->cclass->name );
			}
			else if( c->ns != NULL )
			{
				if( c->ns->name == NULL ) continue;
				FUD(("strcat'ing namespace name\n" ));
				strcat( currentPathNameBuffer, c->ns->name );
			}
			strcat( currentPathNameBuffer, "." );
		}
	}
	if( CURRENT_FUNCTION != NULL && function )
	{
		if( annotate && CURRENT_FUNCTION->is_static ) {
			strcat( currentPathNameBuffer, "static." );
		}
		strcat( currentPathNameBuffer, CURRENT_FUNCTION->name );
		strcat( currentPathNameBuffer, "_" );

		sig = ferite_function_generate_sig_string( CURRENT_SCRIPT, CURRENT_FUNCTION );
		strcat( currentPathNameBuffer, sig );
		ffree_ngc( sig );
	}
	
	FE_LEAVE_FUNCTION( currentPathNameBuffer );
}
char *ferite_compiler_build_current_path_wannotation( int annotate ) {
	return ferite_compiler_build_current_path_wannotation_wfunction( annotate, FE_TRUE );
}
char *ferite_compiler_build_current_path() {
	return ferite_compiler_build_current_path_wannotation_wfunction( FE_FALSE, FE_TRUE );
}
char *ferite_compiler_entry_function( char *filename )
{
	static char entryFunctionNameBuffer[4096];

	FE_ENTER_FUNCTION;
	sprintf(entryFunctionNameBuffer, "entry:%s", filename);
	FE_LEAVE_FUNCTION( entryFunctionNameBuffer );
}

/**
* @function ferite_script_include
 * @declaration FeriteVariable *ferite_script_include( FeriteScript *script, char *filename )
 * @brief This is used to include another script at runtime
 * @description This is a runtime equivelent to:<nl/>
 * <nl/>
 * uses "foo";<nl/>
 * <nl/>
 * @param FeriteScript *script The script to include into
 * @param char *filename The name of the script to include
 * @return The value that is returned after executing the 'main' body of the included script
 */
FeriteVariable *ferite_script_include( FeriteScript *script, char *filename )
{
	FeriteScript *ptr = NULL;
	FeriteVariable *var = NULL;
	char *file, *path, *str = NULL;
	int retval = 0, cleanup = FE_FALSE;
	jmp_buf old_ferite_compiler_jmpback;

	FE_ENTER_FUNCTION;

	if( strcmp( filename, "" ) ==  0 ) {
		FE_LEAVE_FUNCTION(NULL);
	}

	/* load the script... */
	ptr = ferite_new_script();

	FUD(( "filename: %s\n", filename ));
	if( filename[0] == DIR_DELIM )
		file = fstrdup( filename );
	else
	{
		path = aphex_directory_name( script->filename );
		file = fcalloc( strlen( filename ) + strlen( path ) + 2, sizeof(char) );
		sprintf( file, "%s%s", path, filename );
		aphex_free( path );
	}
	FUD(( "real file: %s\n", file ));
	path = aphex_directory_name( file );
	ferite_add_library_search_path( path );
	aphex_free( path );

	ferite_script_load( ptr, file );
	ffree_ngc( file );

	if( ptr->scripttext != NULL )
		str = fstrdup( ptr->scripttext );
	else
	{
		FE_LEAVE_FUNCTION(NULL);
	}
	ferite_script_delete( ptr );

	if( ferite_compile_stack == NULL ) {
		ferite_start_compiler( script );
		cleanup = FE_TRUE;
	}
	else {
		ferite_stack_push( FE_NoScript, ferite_compile_stack, ferite_current_compile );
		ferite_current_compile = ferite_compile_record_alloc();
		CURRENT_SCRIPT = script;
		CURRENT_CLASS = NULL;
		CURRENT_VARS = NULL;
		CURRENT_FUNCTION = NULL;
		CURRENT_NAMESPACE = script->mainns;

		/* save the older enviroment */
		memcpy( &old_ferite_compiler_jmpback, &ferite_compiler_jmpback, sizeof(jmp_buf) );
		ferite_save_lexer();
	}

	FUD(("Setting up parser\n"));
	ferite_set_filename( filename );
	ferite_prepare_parser( str );
	FUD(("Running parser\n"));

	retval = setjmp( ferite_compiler_jmpback );
	if( retval == 0 )
	{  /* we compiled correctly */
		ferite_parse();

		var = ferite_current_compile->last_script_return;

		FUD(("Cleaning Up Parser\n"));
		if( cleanup )
			ferite_clean_compiler();
		else {
			ffree_ngc( ferite_current_compile );
			ferite_current_compile = ferite_stack_pop( FE_NoScript, ferite_compile_stack );
			ferite_restore_lexer();
		}

		ffree_ngc( str );
		ferite_pop_library_search_path();

		if( !cleanup ) {
			/* restore the enviroment */
			memcpy( &ferite_compiler_jmpback, &old_ferite_compiler_jmpback, sizeof(jmp_buf) );
		}
		FE_LEAVE_FUNCTION(var);
	}
	else {
		/* uh, *cough* we didn't */
		ferite_error( CURRENT_SCRIPT, 0, "Error including script `%s'\n", filename );
		if( cleanup )
			ferite_clean_compiler();
		else
			memcpy( &ferite_compiler_jmpback, &old_ferite_compiler_jmpback, sizeof(jmp_buf) );

		ffree_ngc( str );
		ferite_pop_library_search_path();

		FE_LEAVE_FUNCTION(NULL);
	}
	FE_LEAVE_FUNCTION(NULL);
}

/**
* @function ferite_script_eval
 * @declaration FeriteVariable *ferite_script_eval( FeriteScript *script, char *str )
 * @brief This is used to evalutate a string as a script at runtime
 * @param FeriteScript *script The script to include into
 * @param char *str The string to evaulate
 * @return The value that is returned after executing the 'main' body of the evaluated script
 */
FeriteVariable *ferite_script_eval( FeriteScript *script, char *str )
{
	FeriteVariable *var = NULL;
	int retval = 0, cleanup = FE_FALSE;
	jmp_buf old_ferite_compiler_jmpback;

	FE_ENTER_FUNCTION;
	if( ferite_compile_stack == NULL ) {
		ferite_start_compiler( script );
		cleanup = FE_TRUE;
	}
	else {
		ferite_stack_push( FE_NoScript, ferite_compile_stack, ferite_current_compile );
		ferite_current_compile = ferite_compile_record_alloc();
		CURRENT_SCRIPT = script;
		CURRENT_CLASS = NULL;
		CURRENT_VARS = NULL;
		CURRENT_FUNCTION = NULL;
		CURRENT_NAMESPACE = script->mainns;

		/* save the older enviroment */
		memcpy( &old_ferite_compiler_jmpback, &ferite_compiler_jmpback, sizeof(jmp_buf) );
		ferite_save_lexer();
	}
	FUD(("Setting up parser\n"));
	ferite_set_filename( "eval()" );
	ferite_prepare_parser( str );
//	printf("eval string: %s\n", str);
	FUD(("Running parser\n"));
	retval = setjmp( ferite_compiler_jmpback );
	if( retval == 0 )
	{
		ferite_parse();

		var = ferite_current_compile->last_script_return;

		if( cleanup )
			ferite_clean_compiler();
		else {
			FeriteNamespaceBucket *nsb = ferite_namespace_element_exists( CURRENT_SCRIPT, CURRENT_NAMESPACE, ferite_compiler_entry_function("eval()") );
			FeriteFunction *eval = nsb->data;

			nsb->data = eval->next;
			eval->next = NULL;
			
			ferite_delete_function_list( CURRENT_SCRIPT, eval );

			ffree_ngc( ferite_current_compile );
			ferite_current_compile = ferite_stack_pop( FE_NoScript, ferite_compile_stack );
			ferite_restore_lexer();
		}

		if( !cleanup ) {
			/* restore the enviroment */
			memcpy( &ferite_compiler_jmpback, &old_ferite_compiler_jmpback, sizeof(jmp_buf) );
		}
		FE_LEAVE_FUNCTION(var);
	}
	else
	{
		/* uh, *cough* we didn't */
		ferite_error( CURRENT_SCRIPT, 0, "Error evaluating script `%s'\n", str );
		if( cleanup )
			ferite_clean_compiler();
		else
			memcpy( &ferite_compiler_jmpback, &old_ferite_compiler_jmpback, sizeof(jmp_buf) );
		FE_LEAVE_FUNCTION(NULL);
	}
	FE_LEAVE_FUNCTION(NULL);
}

/**
* @function ferite_script_compile
 * @declaration FeriteScript *ferite_script_compile( char *filename )
 * @brief Compile a script with the default search paths
 * @param char *filename The name of the script to compile
 * @return A script which can either be executed or contains error information
 */
FeriteScript *ferite_script_compile( char *filename )
{
	FE_ENTER_FUNCTION;
	FE_LEAVE_FUNCTION( ferite_script_compile_with_path( filename, NULL ) );
}

/**
* @function ferite_script_compile_with_path
 * @declaration FeriteScript *ferite_script_compile_with_path( char *filename, char **paths )
 * @brief Compile a script with the default search paths aswell as an extra set
 * @param char *filename The name of the script to compile
 * @param char **paths A NULL terminated array of paths to add during compilation
 * @return A script which can either be executed or contains error information
 */
FeriteScript *ferite_script_compile_with_path( char *filename, char **paths )
{
	FeriteScript *ptr = NULL;

	FE_ENTER_FUNCTION;
	ptr = ferite_new_script();
	FE_LEAVE_FUNCTION(ferite_script_compile_with_script_and_path( ptr, filename, paths ));
}
/**
 * @function ferite_script_compile_with_script_and_path
 * @declaration FeriteScript *ferite_script_compile_with_script_and_path( FeriteScript *script, char *str, char **paths )
 * @brief Compile a script on disk into a given script object. This allows you to preload the script with data.
 * @param FeriteScript *script The script to use
 * @param char *filename The script filename to compile
 * @param char **paths A NULL terminated array of paths to add during compliation
 * @return The same script that was passed into the function
 */
FeriteScript *ferite_script_compile_with_script_and_path( FeriteScript *script, char *filename, char **paths )
{
	FE_ENTER_FUNCTION;
	ferite_script_load( script, filename );
	if( script->scripttext != NULL )
	 {
		ferite_set_filename( filename );
		ferite_compile_string_with_script_and_path( script, script->scripttext, paths );
		if( script->mainns != NULL )
		{
			FE_LEAVE_FUNCTION(script);
		}
		else
		{
			ferite_error( script, 0, "Fatal error compiling script \"%s\"\n", filename );
			FE_LEAVE_FUNCTION( script );
		}
	}
	else
	{
		ferite_error( script, 0, "Can't open script %s\n", filename );
		FE_LEAVE_FUNCTION( script );
	}
	FE_LEAVE_FUNCTION( script );
}

/**
* @function ferite_compile_string
 * @declaration FeriteScript *ferite_compile_string( char *str )
 * @brief Compile a string with the default search paths
 * @param char *str The string to compile
 * @return A script which can either be executed or contains error information
 */
FeriteScript *ferite_compile_string( char *str )
{
	FE_ENTER_FUNCTION;
	FE_LEAVE_FUNCTION( ferite_compile_string_with_path( str, NULL ) );
}

/**
* @function ferite_compile_string_with_path
 * @declaration FeriteScript *ferite_compile_string_with_path( char *str, char **paths )
 * @brief Compile a str with the default search paths aswell as an extra set
 * @param char *str The script to compile
 * @param char **paths A NULL terminated array of paths to add during compilation
 * @return A script which can either be executed or contains error information
 */
FeriteScript *ferite_compile_string_with_path( char *str, char **paths )
{
	FeriteScript *script = NULL;
	FE_ENTER_FUNCTION;
	script = ferite_new_script();
	script = ferite_compile_string_with_script_and_path( script, str, paths );
	FE_LEAVE_FUNCTION(script);
}

/**
 * @function ferite_compile_string_with_script_and_path
 * @declaration FeriteScript *ferite_compile_string_with_script_and_path( FeriteScript *script, char *str, char **paths )
 * @brief Compile a string into a given script object. This allows you to preload the script with data
 * @param FeriteScript *script The script to use
 * @param char *str The script string to compile
 * @param char **paths A NULL terminated array of paths to add during compliation
 * @return The same script that was passed into the function
 */
FeriteScript *ferite_compile_string_with_script_and_path( FeriteScript *script, char *str, char **paths )
{
	int path_count = 0;

	FE_ENTER_FUNCTION;
	ferite_compile_error = 0;
	if( script->filename )
		ffree_ngc( script->filename );
	script->filename = fstrdup(ferite_scanner_file);

	ferite_init_gc( script );

	if( ferite_scanner_file != NULL )
		ferite_stack_push( FE_NoScript, script->include_list, fstrdup(ferite_scanner_file) );

	ferite_start_compiler( script );

	ferite_add_object_class( script );
	ferite_add_closure_class( script );
	if( !ferite_keep_native_function_data )
	{
		FeriteVariable *v = NULL;
		FeriteNamespaceBucket *nsb = NULL;
		FeriteFunction *rename = NULL;

		ferite_do_add_variable( "err", F_VAR_OBJ, ferite_subtype_link(CURRENT_SCRIPT, "O"), FE_TRUE, FE_FALSE, FE_FALSE,
#ifdef THREAD_SAFE
								FE_TRUE,
#else
								FE_FALSE,
#endif
								FE_ITEM_IS_PUBLIC, FE_FALSE );
		ferite_do_add_variable( "null", F_VAR_OBJ, ferite_subtype_link(CURRENT_SCRIPT, "O"), FE_TRUE, FE_TRUE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );
		ferite_do_add_variable( "nowt", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_TRUE, FE_TRUE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );

		nsb = ferite_find_namespace( script,script->mainns,"null",FENS_VAR);
		if( nsb != NULL )
		{
			v = nsb->data;
			MARK_VARIABLE_AS_FINALSET(v);
		}
		nsb = ferite_find_namespace( script,script->mainns,"nowt",FENS_VAR);
		if( nsb != NULL )
		{
			v = nsb->data;
			MARK_VARIABLE_AS_FINALSET(v);
		}

		ferite_init_error_system( script, script->mainns );
		ferite_register_ns_variable( script, script->mainns, "argv", ferite_duplicate_variable( CURRENT_SCRIPT, ferite_ARGV, NULL ) );
		rename = fe_create_ns_fnc( script, script->mainns, "rename", ferite_namespace_item_rename, "ss" );
		rename->is_static = FE_FUNCTION_IS_DIRECTIVE;
	}

	/* we now have exclusive lock on the compiler, we now add our paths the list */
	if( paths != NULL )
	{
		while( paths[path_count] != NULL )
			ferite_add_library_search_path( paths[path_count++] );
	}

	FUD(("Setting up parser\n"));
	ferite_prepare_parser( str );

	if( ferite_module_do_preload( script ) == 0 )
	{
		ferite_clean_compiler();
		ferite_script_clean( script );
		FE_LEAVE_FUNCTION( script );
	}

	FUD(("Running parser\n"));
	if( setjmp( ferite_compiler_jmpback ) == 0 )
	{
		/* we compiled correctly */
		ferite_parse();
		FUD(("Cleaning Up Parser\n"));

		/* remove the paths from the system before we relinquish compiler control */
		if( paths != NULL )
		{
			while( path_count > 0 )
			{
				ferite_pop_library_search_path();
				path_count--;
			}
		}
		if( ferite_current_compile->last_script_return ) {
			ferite_variable_destroy( script, ferite_current_compile->last_script_return );
			ferite_current_compile->last_script_return = NULL;
		}

		ferite_clean_compiler();
		FE_LEAVE_FUNCTION( script );
	}
	else
	{
		/* uh, *cough* we didn't */
		if( ferite_scanner_file == NULL || strcmp( ferite_scanner_file, "-e" ) == 0 )
			ferite_error( CURRENT_SCRIPT, 0, "Fatal error compiling script\n" );

		/* remove the paths from the system before we relinquish compiler control */
		if( paths != NULL )
		{
			while( path_count > 0 )
			{
				ferite_pop_library_search_path();
				path_count--;
			}
		}

		ferite_clean_compiler();
		ferite_script_clean( script );
#ifdef DEBUG
		ferite_call_level = ferite_depth + 1; /* EVIL EVIL EVIL EVIL EVIL EVIL EVIL */
#endif
		FE_LEAVE_FUNCTION( script );
	}
	FE_LEAVE_FUNCTION( script );
}

int ferite_compiler_include_in_list( FeriteScript *script, char *name )
{
	int i = 0;

	FE_ENTER_FUNCTION;
	for( i = 0; i <= script->include_list->stack_ptr; i++ )
	{
		FUD(( "ferite_compiler_include_in_list: checking %s matching %s\n", name, (char *)(script->include_list->stack[i]) ));
		if( script->include_list->stack[i] != NULL && strcmp( name, script->include_list->stack[i] ) == 0 )
		{
			FE_LEAVE_FUNCTION(1);
		}
	}
	FE_LEAVE_FUNCTION(0);
}

void ferite_do_uses( char *name )
{
	FE_ENTER_FUNCTION;
	if( ferite_load_module( CURRENT_SCRIPT, CURRENT_NAMESPACE, name ) == 0 )
	{
		if( !ferite_keep_native_function_data ) {
			STOP_COMPILE();
		} else {
			CURRENT_SCRIPT->error_state = 0;
		}
	}
	FE_LEAVE_FUNCTION(NOWT);
}

int ferite_do_load_script( char *name )
{
	FeriteScript *script = CURRENT_SCRIPT;
	char *scripttext, *realname, *realpath;
	jmp_buf old_ferite_compiler_jmpback;

	int scripttext_destroy = FE_TRUE;
	
	FE_ENTER_FUNCTION;
	FUD(( "COMPILER: Request to load script %s\n", name ));
	realname = aphex_relative_to_absolute( name );

	//	printf( "load_script: %s\n", realname );

	if( ferite_compiler_include_in_list( CURRENT_SCRIPT, realname ) == 0 )
	{
		scripttext = ferite_cache_has_code( CURRENT_SCRIPT, realname );
		if( !scripttext ) {
			scripttext = aphex_file_to_string( realname );
			if( scripttext == NULL ) {
				aphex_free( realname );
				FE_LEAVE_FUNCTION(-1);
			} else {
				scripttext_destroy = FE_TRUE;
				if( scripttext[0] == '#' ) {
					int i = 0;
					for( i = 0; scripttext[i] != '\n'; i++ )
						scripttext[i] = ' ';
				}
				ferite_parser_script_normalise( scripttext );
				ferite_cache_register_code( CURRENT_SCRIPT, realname, scripttext );
			}
		} else {
			scripttext_destroy = FE_FALSE;
		}
		
		ferite_stack_push( FE_NoScript, CURRENT_SCRIPT->include_list, fstrdup(realname) );

		/* save the older enviroment */
		memcpy( &old_ferite_compiler_jmpback, &ferite_compiler_jmpback, sizeof(jmp_buf) );

		ferite_stack_push( FE_NoScript, ferite_compile_stack, ferite_current_compile );
		ferite_current_compile = ferite_compile_record_alloc();
		CURRENT_SCRIPT = script;
		CURRENT_CLASS = NULL;
		CURRENT_VARS = NULL;
		CURRENT_FUNCTION = NULL;
		CURRENT_NAMESPACE = script->mainns;

		ferite_save_lexer();
		ferite_set_filename( name );
		FUD(( "COMPILER: Setting up parser (ferite_do_load_script)\n"));

		ferite_prepare_parser_cr( scripttext, FE_FALSE );

		/* We need to add the path of this script to the list of search paths */
		realpath = aphex_directory_name( realname );
		ferite_add_library_search_path( realpath );
		aphex_free( realname );
		aphex_free( realpath );

		if( setjmp( ferite_compiler_jmpback ) == 0 )
		{
			FUD(( "COMPILER: Running parser (ferite_do_load_script)\n" ));
			ferite_parse();
			FUD(( "COMPILER: Cleaning Up Parser (ferite_do_load_script)\n" ));
			ferite_clean_parser();
			ferite_restore_lexer();
			
			if( scripttext_destroy ) 
				aphex_free( scripttext );

			if( ferite_current_compile->last_script_return ) {
				ferite_variable_destroy( script, ferite_current_compile->last_script_return );
				ferite_current_compile->last_script_return = NULL;
			}

			ffree_ngc( ferite_current_compile );
			ferite_current_compile = ferite_stack_pop( FE_NoScript, ferite_compile_stack );

			/* restore the enviroment */
			memcpy( &ferite_compiler_jmpback, &old_ferite_compiler_jmpback, sizeof(jmp_buf) );

			ferite_pop_library_search_path();
			FE_LEAVE_FUNCTION(1);
		}
		else
		{
			ferite_error( CURRENT_SCRIPT, 0, "Can't compile included file \"%s\", error on line %d\n", name, ferite_scanner_lineno );
			ferite_clean_parser();
			ferite_restore_lexer();
			ferite_compile_error = 1;
			memcpy( &ferite_compiler_jmpback, &old_ferite_compiler_jmpback, sizeof(jmp_buf) );
			if( scripttext_destroy ) 
				aphex_free( scripttext );

			ferite_pop_library_search_path();
			FE_LEAVE_FUNCTION(0);
		}
	}
	FUD(( "script %s has already been placed in the sciprt", name ));
	aphex_free( realname ); /* aphex uses malloc - so we have to use real free */
	FE_LEAVE_FUNCTION(1);
}
void ferite_do_script_finish() 
{
	FeriteNamespaceBucket *nsb = NULL;
	char *start_function = ferite_compiler_entry_function(ferite_scanner_file);

	/* here we call the start function */
	nsb = ferite_namespace_element_exists( CURRENT_SCRIPT, CURRENT_NAMESPACE, start_function );
	if( nsb && nsb->data ) {
		ferite_current_compile->last_script_return = ferite_script_function_execute( CURRENT_SCRIPT, CURRENT_NAMESPACE, NULL, nsb->data, NULL );
	} else {
		ferite_current_compile->last_script_return = NULL;
	}
}

size_t ferite_lexer_offset( char *name, int start );
void   ferite_lexer_jump( size_t amount );

void ferite_do_function_header( char *name, int is_static, int is_native, int is_atomic, int state, int final, int directive )
{
	FeriteFunction *new_function = NULL;
	FeriteNamespaceBucket *nsb = NULL;
	FeriteCompileRecord *rec = NULL;
	char *real_function_name = name;
	char *start_function = ferite_compiler_entry_function(ferite_scanner_file);
	int isclass = 0;

	FE_ENTER_FUNCTION;

	if( CURRENT_CLASS != NULL )
	{
		if( strcmp( real_function_name, "Constructor" ) == 0 || strcmp( real_function_name, CURRENT_CLASS->name ) == 0 )
		{
			ferite_warning( CURRENT_SCRIPT, "ferite no longer uses the class name for the constructor of a class. %s has been renamed to 'constructor' (%s).\n", real_function_name, CURRENT_CLASS->name );
			ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
			real_function_name = "constructor";
		}
		else if( strcmp( real_function_name, "Destructor" ) == 0 )
		{
			ferite_warning( CURRENT_SCRIPT, "ferite no longer uses the keyword Destructor for the destructor of a class. %s has been renamed to 'destructor' (%s).\n", real_function_name, CURRENT_CLASS->name );
			ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
			real_function_name = "destructor";
		}
	}
	if( directive )
		is_static = FE_FUNCTION_IS_DIRECTIVE;


	if( strcmp( name, start_function ) == 0 && ferite_compile_stack->stack_ptr == 0 && !(CURRENT_SCRIPT->is_executing) ) {
		real_function_name = "!__start__";
	}
	
	new_function = ferite_create_internal_function( CURRENT_SCRIPT, real_function_name );
	new_function->bytecode->filename = fstrdup( (ferite_scanner_file == NULL ? "" : ferite_scanner_file) );
	new_function->is_static = is_static;
	new_function->state = FE_ITEM_IS_PUBLIC;

	if( CURRENT_CLASS == NULL ) {
		/* add to a script */
		if( (nsb = ferite_namespace_element_exists( CURRENT_SCRIPT, CURRENT_NAMESPACE, real_function_name )) != NULL )
		{
			if( nsb->type != FENS_FNC ) {
				if( CURRENT_NAMESPACE == CURRENT_SCRIPT->mainns ) {
					ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
					ferite_error( CURRENT_SCRIPT, 0, "  There is already an item named %s in the top-level namespace (could be a global variable, namespace or class).\n", name );
				} else {
					ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );					
					ferite_error( CURRENT_SCRIPT, 0, "  There is already an item named %s in the %s namespace (could be a variable, namespace or class).\n", name, CURRENT_NAMESPACE->name );
				}
				ferite_delete_function_list( CURRENT_SCRIPT, new_function );
				STOP_COMPILE();
			}
		}
	}
	else
	{
		if( state >= FE_ITEM_IS_PRIVATE )
			new_function->state = state;
		else
			new_function->state = FE_ITEM_IS_PUBLIC;
		isclass = 1;
	}
	
	/* this is so that we have space for self && super in the function */
	ferite_stack_push( FE_NoScript, new_function->localvars, NULL );
	ferite_stack_push( FE_NoScript, new_function->localvars, NULL );

	rec = ferite_current_compile;
	ferite_stack_push( FE_NoScript, ferite_compile_stack, ferite_current_compile );
	ferite_current_compile = ferite_compile_record_alloc();
	CURRENT_SCRIPT = rec->script;
	CURRENT_FUNCTION = new_function;
	CURRENT_VARS = new_function->localvars;
	CURRENT_NAMESPACE = rec->ns;
	CURRENT_CLASS = rec->cclass;
	CURRENT_VARS_HASH = ferite_create_hash( CURRENT_SCRIPT, FE_FUNCTION_VARIABLE_SIZE );
	CURRENT_SHAD_VARS = ferite_create_stack( CURRENT_SCRIPT, FE_FUNCTION_VARIABLE_SIZE );
	CURRENT_SCOPE_FRM = NULL;

	ferite_compiler_current_block_depth = 0;

	/* setup the mutex */
	if( is_atomic == 1 ) {
#ifdef THREAD_SAFE
		new_function->lock = (void*)aphex_mutex_recursive_create();
#else
		ferite_warning( CURRENT_SCRIPT, "'atomic' keyword can not be used for function '%s' - please compile ferite with threading!\n", name );
		ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
#endif
	}
	FE_LEAVE_FUNCTION( NOWT );
}

extern int ferite_var_is_native;

char *ferite_signature_to_string( FeriteScript *script, FeriteFunction *func )
{
	static char parameters_buffer[1024];
	int i = 0;
	
	memset( parameters_buffer, '\0', 1024 );
	for( i = 0; i < func->arg_count; i++ )
	{
		FeriteParameterRecord *record = func->signature[i];
		if( record != NULL ) {
			if( record->variable ) {
				strcat( parameters_buffer, ferite_variable_id_to_str( script, record->variable->type ) );
			} else {
				strcat( parameters_buffer, "...");
			}
			if( (i+1) < func->arg_count )
				strcat( parameters_buffer, "," );
		}
	}
	return parameters_buffer;
}

int ferite_do_function_start()
{
	int did_jump_forward = FE_FALSE;
	
	FE_ENTER_FUNCTION;
	if( CURRENT_FUNCTION != NULL )
	{
		char *path = ferite_compiler_build_current_path_wannotation(FE_TRUE);
		FeriteFunction *function = ferite_cache_reference_function( CURRENT_SCRIPT, path );
		if( function ) {
			/* Remove the old function */
			ferite_delete_function_list( CURRENT_SCRIPT, CURRENT_FUNCTION );
			CURRENT_FUNCTION = function;
			ferite_lexer_jump( CURRENT_FUNCTION->length );
			/* We force a turn off of a native block parse */
			ferite_var_is_native = FE_FALSE;
			did_jump_forward = FE_TRUE;
			if( CURRENT_CLASS ) {
				CURRENT_FUNCTION->klass = CURRENT_CLASS;
			}
		} else {
			ferite_lexer_offset( path, FE_TRUE );
			ferite_do_add_variable( "super", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_FALSE, FE_TRUE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );
			ferite_do_add_variable( "self", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_FALSE, FE_TRUE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );
		}
		ffree_ngc(path);
		
		
		if( CURRENT_CLASS == NULL ) {
			if( !ferite_register_ns_function( CURRENT_SCRIPT, CURRENT_NAMESPACE, CURRENT_FUNCTION ) ) {
				if( strcmp(CURRENT_FUNCTION->name, ferite_compiler_entry_function("eval()")) != 0 ) {
					ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
					ferite_error( CURRENT_SCRIPT, 0, "  There is already a function '%s' with the parameter signature (%s) in the namespace %s.\n", 
									CURRENT_FUNCTION->name, ferite_signature_to_string(CURRENT_SCRIPT, CURRENT_FUNCTION), CURRENT_NAMESPACE->name );
					ferite_error( CURRENT_SCRIPT, 0, "  Please check the other functions with the same name and fix this error by changing its required parameters or removing it.\n" );
					STOP_COMPILE();
				} else {
					FeriteNamespaceBucket *nsb = ferite_namespace_element_exists( CURRENT_SCRIPT, CURRENT_NAMESPACE, CURRENT_FUNCTION->name );
					FeriteFunction *eval = nsb->data;
					nsb->data = CURRENT_FUNCTION;
					CURRENT_FUNCTION->next = eval;
				}
			}
		} else {
			if( !ferite_register_class_function( CURRENT_SCRIPT, CURRENT_CLASS, CURRENT_FUNCTION, CURRENT_FUNCTION->is_static ) ) {
				ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
				ferite_error( CURRENT_SCRIPT, 0, "  There is already a function '%s' with the parameter signature (%s) in the class %s.\n", 
								CURRENT_FUNCTION->name, ferite_signature_to_string(CURRENT_SCRIPT, CURRENT_FUNCTION), CURRENT_CLASS->name );
				ferite_error( CURRENT_SCRIPT, 0, "  Please check the other functions with the same name and fix this error by changing its required parameters or removing it.\n" );
				STOP_COMPILE();
			}
		}
	}
	FE_LEAVE_FUNCTION( did_jump_forward );
}
void ferite_do_function_return_type( char *type ) {
	FeriteVariableSubType *stype = NULL;
	FE_ENTER_FUNCTION;
	stype = ferite_subtype_link( CURRENT_SCRIPT, type );
	if( stype ) {
		CURRENT_FUNCTION->return_type = stype->type;
		CURRENT_FUNCTION->return_subtype = stype;
	}
	if( CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
		printf("Compile error with function return.\n");
		STOP_COMPILE();
	}
	FE_LEAVE_FUNCTION(NOWT);
}
void ferite_do_function_native_block( char *code, char *file, int line )
{
	FeriteFunctionNative *ptr = NULL;

	FE_ENTER_FUNCTION;
	ferite_do_function_cleanup();

	ferite_function_to_external( CURRENT_SCRIPT, CURRENT_FUNCTION );

	/* store the file information */
	ptr = fmalloc_ngc( sizeof(FeriteFunctionNative) );
	ptr->code = NULL;
	ptr->file = fstrdup( file );
	ptr->line = line;
	CURRENT_FUNCTION->native_information = ptr;

	if( ferite_keep_native_function_data == 1 )
		ptr->code = code;
	else
	{
		char *function_name_path = ferite_compiler_build_current_path();
		ffree_ngc( code );
		FUD(( "found native function %s\n", function_name_path ));
		/* find the correct function */
		CURRENT_FUNCTION->fncPtr = ferite_module_find_function( function_name_path );
		if( CURRENT_FUNCTION->fncPtr == NULL )
		{
			char *path = ferite_compiler_build_current_path_wannotation(FE_TRUE);
			ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
			ferite_error( CURRENT_SCRIPT, 0, "  Unable to locate native method '%s' - please check that it gets loaded in use the 'uses' statement.\n",
						  path );
			ffree_ngc(path);
			ffree_ngc(function_name_path);
			STOP_COMPILE();
		}
		ffree_ngc(function_name_path);
	}
	
	if( CURRENT_FUNCTION->name[0] != '!' ) {
		char *path = ferite_compiler_build_current_path_wannotation(FE_TRUE);
		CURRENT_FUNCTION->length = ferite_lexer_offset( path, FE_FALSE );
		ferite_cache_register_function( CURRENT_SCRIPT, path, CURRENT_FUNCTION );
		ffree_ngc(path);
	}
	
	ffree_ngc( ferite_current_compile );
	ferite_current_compile = ferite_stack_pop( FE_NoScript, ferite_compile_stack );
	FE_LEAVE_FUNCTION(NOWT);
}

void ferite_do_function_cleanup()
{
	int i = 0, j = 0;

	if( ferite_current_compile != NULL )
	{
		if( CURRENT_VARS_HASH != NULL )
			ferite_delete_hash( CURRENT_SCRIPT, CURRENT_VARS_HASH, ferite_delete_pointer );

		if( CURRENT_SHAD_VARS != NULL )
		{
			for( i = 0; i <= CURRENT_SHAD_VARS->stack_ptr; i++ )
			{
				FeriteStack *s = CURRENT_SHAD_VARS->stack[i];
				if( s != NULL )
				{
					for( j = 1; j <= s->stack_ptr; j++ )
					{
						FeriteTag *t = s->stack[j];
						if( t != NULL )
						{
							ffree_ngc( t->value );
							ffree_ngc( t );
						}
					}
					ferite_delete_stack( NULL, s );
				}
			}
			ferite_delete_stack( NULL, CURRENT_SHAD_VARS );
		}

		if( CURRENT_SCOPE_FRM != NULL )
		{
			for( j = 0; j <= CURRENT_SCOPE_FRM->stack_ptr; j++ )
			{
				FeriteTag *t = CURRENT_SCOPE_FRM->stack[j];
				if( t != NULL )
				{
					ffree_ngc( t->value );
					ffree_ngc( t );
				}
			}
			ferite_delete_stack( NULL, CURRENT_SCOPE_FRM );
		}

		CURRENT_VARS_HASH = NULL;
		CURRENT_SHAD_VARS = NULL;
		CURRENT_SCOPE_FRM = NULL;
	}
}

void ferite_do_function_footer()
{
	FE_ENTER_FUNCTION;
	if( ! CURRENT_FUNCTION->cached ) {
		char *entry_function = ferite_compiler_entry_function("eval()");
		ferite_do_exit();
		if( CURRENT_FUNCTION->name[0] != '!' && strcmp(CURRENT_FUNCTION->name,entry_function) != 0 ) {
			char *path = ferite_compiler_build_current_path_wannotation(FE_TRUE);
			CURRENT_FUNCTION->length = ferite_lexer_offset( path, FE_FALSE );
			ferite_cache_register_function( CURRENT_SCRIPT, path, CURRENT_FUNCTION );
			ffree_ngc(path);
//			printf("storring function: %s\n", path);
		}
		if( ferite_show_debug ) {
			printf("COMPILER: Dumping opcode list for %s\n", CURRENT_FUNCTION->name );
			ferite_opcode_dump( CURRENT_FUNCTION->bytecode );
		}
	} else if( CURRENT_FUNCTION->is_alias ) {
		/* We turn of the cached flag for later deletion */
		CURRENT_FUNCTION->cached = FE_FALSE;
	}
	ferite_do_function_cleanup();
	ffree_ngc( ferite_current_compile );
	ferite_current_compile = ferite_stack_pop( FE_NoScript, ferite_compile_stack );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_class_header( char *name, char *extends, int state )
{
	FeriteClass *classtmpl = NULL;
	FeriteNamespace *fns = CURRENT_NAMESPACE;
	FeriteNamespaceBucket *nsb = NULL;
	FeriteScript *scr = NULL;

	FE_ENTER_FUNCTION;
	if( name != NULL )
	{
		classtmpl = ferite_register_inherited_class( CURRENT_SCRIPT, CURRENT_NAMESPACE, name, extends );
		if( classtmpl == NULL )
		{
			ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
			ferite_error( CURRENT_SCRIPT, 0, "	Unable to create class '%s'\n", name );
			STOP_COMPILE();
		}
	}
	else
	{
		nsb = ferite_find_namespace( CURRENT_SCRIPT, CURRENT_NAMESPACE, extends, FENS_CLS );
		if( nsb == NULL )
		{
			if( CURRENT_NAMESPACE != CURRENT_SCRIPT->mainns )
				nsb = ferite_find_namespace( CURRENT_SCRIPT, CURRENT_SCRIPT->mainns, extends, FENS_CLS );
			if( nsb == NULL )
			{
				ferite_warning( CURRENT_SCRIPT, "Class '%s' doesn't exist, creating a new class\n", extends );
				ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
				classtmpl = ferite_register_inherited_class( CURRENT_SCRIPT, CURRENT_NAMESPACE, extends, NULL );
			}
			else
			{
				ferite_warning( CURRENT_SCRIPT, "Class '%s' doesn't exist within current namespace, checking from top level.\n", extends );
				ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
				classtmpl = nsb->data;
			}
		}
		else
		{
			classtmpl = nsb->data;
		}
		if( classtmpl == NULL )
		{
			ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
			ferite_error( CURRENT_SCRIPT, 0, "	Unable to create class '%s'\n", extends );
			STOP_COMPILE();
		}
		if( classtmpl->state == FE_ITEM_IS_FINAL )
		{
			ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
			ferite_error( CURRENT_SCRIPT, 0, "	Class '%s' is final and can't be modified\n", extends );
			STOP_COMPILE();
		}
	}

	ferite_stack_push( FE_NoScript, ferite_previous_directives_stack, ferite_directive_stack );
	ferite_directive_stack = ferite_create_stack( CURRENT_SCRIPT, 10 );

	classtmpl->state = state;

	ferite_stack_push( FE_NoScript, ferite_compile_stack, ferite_current_compile );
	scr = CURRENT_SCRIPT;

	ferite_current_compile = ferite_compile_record_alloc();
	CURRENT_VARS = NULL;
	CURRENT_CLASS = classtmpl;
	CURRENT_SCRIPT = scr;
	CURRENT_FUNCTION = NULL;
	CURRENT_NAMESPACE = fns;

	if( name == NULL )
		ferite_current_compile->want_container_finish = FE_FALSE;

	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_class_implements( char *implements )
{
	FeriteClass *klass = ferite_find_class( CURRENT_SCRIPT, CURRENT_SCRIPT->mainns, implements );

	if( klass == NULL )
	{
		ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
		ferite_error( CURRENT_SCRIPT, 0, "  Unable to find protocol '%s' for class %s.\n", implements, CURRENT_CLASS->name );
		STOP_COMPILE();
	}
	else if( klass->state != FE_ITEM_IS_PROTOCOL )
	{
		ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
		ferite_error( CURRENT_SCRIPT, 0, "  %s is not a defined protocol and can not be implemented by %s.\n", implements, CURRENT_CLASS->name );
		STOP_COMPILE();
	}
	ferite_stack_push( FE_NoScript, CURRENT_CLASS->impl_list, klass );
}

void ferite_do_class_footer()
{
	int i = 0;

	FE_ENTER_FUNCTION;
	if( ferite_show_debug )
	{
		FUD(("COMPILER: Dumping variable hash for class %s\n", CURRENT_CLASS->name ));
		ferite_hash_print( CURRENT_SCRIPT, CURRENT_CLASS->class_vars );
		ferite_hash_print( CURRENT_SCRIPT, CURRENT_CLASS->object_vars );
		FUD(("COMPILER: Dumping function hash for class %s\n", CURRENT_CLASS->name ));
		ferite_hash_print( CURRENT_SCRIPT, CURRENT_CLASS->object_methods );
		ferite_hash_print( CURRENT_SCRIPT, CURRENT_CLASS->class_methods );
	}
	
	ferite_do_apply_directives();
	ferite_directive_stack = ferite_stack_pop( FE_NoScript, ferite_previous_directives_stack );

	if( ferite_current_compile->want_container_finish && (!ferite_keep_native_function_data && !ferite_compile_finishing_class) )
	{
		ferite_compile_finishing_class = FE_TRUE;
		ferite_class_finish( CURRENT_SCRIPT, CURRENT_CLASS );
		ferite_compile_finishing_class = FE_FALSE;
		if( CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
			STOP_COMPILE();
		}
	}

	/* Here we check for implements classes. If they exist we do some checking. */
	for( i = 0; i <= CURRENT_CLASS->impl_list->stack_ptr; i++ )
	{
		FeriteClass *klass = CURRENT_CLASS->impl_list->stack[i];
		if( klass != NULL )
		{
			FeriteHashBucket *buk = NULL;
			FeriteIterator *iter = ferite_create_iterator( CURRENT_SCRIPT );

			while( (buk = (FeriteHashBucket*)ferite_hash_walk( CURRENT_SCRIPT, klass->object_methods, iter )) != NULL ) {
				FeriteClass *classWalker = CURRENT_CLASS;
				FeriteFunction *f1 = buk->data;
				FeriteFunction *f2 = NULL;
				
				while( classWalker != NULL ) {
					f2 = ferite_hash_get( CURRENT_SCRIPT, classWalker->object_methods, f1->name );
					if( f2 != NULL )
						break;
					classWalker = classWalker->parent;
				}

				if( f2 == NULL )
				{
					ferite_error( CURRENT_SCRIPT, 0, "Compile Error: Class %s does not correctly implement protocol %s\n", CURRENT_CLASS->name, klass->name );
					ferite_error( CURRENT_SCRIPT, 0, "  The function %s is not implemented.\n", f1->name );
					STOP_COMPILE();
				}
				if( !ferite_compare_functions( CURRENT_SCRIPT, f1, f2 ) )
				{
					ferite_error( CURRENT_SCRIPT, 0, "Compile Error: Class %s does not correctly implement protocol %s\n", CURRENT_CLASS->name, klass->name );
					ferite_error( CURRENT_SCRIPT, 0, "  The function %s is not correctly implemented.\n", f1->name );
					ffree_ngc( iter );
					STOP_COMPILE();
				}
			}
			ffree_ngc( iter );
		}
	}

	ffree_ngc( ferite_current_compile );
	ferite_current_compile = ferite_stack_pop( FE_NoScript, ferite_compile_stack );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_add_directive( char *name, FeriteStack *values )
{
	if( values->stack_ptr <= 127 ) {
		FeriteDirective *directive = fmalloc_ngc(sizeof(FeriteDirective));
		directive->name = fstrdup(name);
		directive->values = values;
		ferite_stack_push( FE_NoScript, ferite_directive_stack, directive );
	} else {
		ffree_ngc( name );
		ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
		ferite_error( CURRENT_SCRIPT, 0, "  You can pass a maximum of 127 paramters to a direcive.\n", name );
		STOP_COMPILE();
	}
}
void ferite_do_apply_directives()
{
	int i = 0, j = 0;
	FeriteVariable *params[128];

	FE_ENTER_FUNCTION;
	for( i = 0; i <= ferite_directive_stack->stack_ptr; i++ )
	{
		FeriteDirective *directive = ferite_directive_stack->stack[i];
		if( directive != NULL )
		{
			memset( params, 0, sizeof(FeriteVariable*) * 128 );
			for( j = 1; j <= directive->values->stack_ptr; j++ )
			{
				params[j-1] = directive->values->stack[j];
				params[j] = NULL;
			}
			if( CURRENT_CLASS != NULL )
			{
				FeriteFunction *directive_impl = ferite_class_get_function( CURRENT_SCRIPT, CURRENT_CLASS, directive->name );

				if( directive_impl != NULL ) {
					if( directive_impl->is_static == FE_FUNCTION_IS_DIRECTIVE ) {
						ferite_variable_destroy( CURRENT_SCRIPT, ferite_call_function( CURRENT_SCRIPT, CURRENT_CLASS, NULL, directive_impl, params ) );
					}
					else
					{
						ferite_warning( CURRENT_SCRIPT, "Attempting to apply class directive %s but %s is a function not a directive.\n", directive->name, directive->name );
						ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
					}
				} else {
					ferite_error(CURRENT_SCRIPT,0,"Unable to apply directive %s\n", directive->name);
					STOP_COMPILE();
				}
			}
			else if( CURRENT_NAMESPACE != NULL )
			{
				FeriteFunction *directive_impl = ferite_find_namespace_element_contents( CURRENT_SCRIPT, CURRENT_SCRIPT->mainns,
																						 directive->name, FENS_FNC );
				if( directive_impl != NULL ) {
					if( directive_impl->is_static == FE_FUNCTION_IS_DIRECTIVE ) {
						ferite_variable_destroy( CURRENT_SCRIPT, ferite_call_function( CURRENT_SCRIPT, CURRENT_NAMESPACE, NULL, directive_impl, params ) );
					}
					else
					{
						ferite_warning( CURRENT_SCRIPT, "Attempting to apply namespace directive %s but %s is a function not a directive.\n", directive->name, directive->name );
						ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
					}
				} else {
					ferite_error(CURRENT_SCRIPT,0,"Unable to apply directive %s\n", directive->name);
					STOP_COMPILE();
				}
			}

			if( CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
				ferite_error(CURRENT_SCRIPT,0,"Unable to apply directive %s\n", directive->name);
				STOP_COMPILE();
			}
			ffree_ngc( directive->name );
			for( j = 0; j <= directive->values->stack_ptr; j++ ) {
				FeriteVariable *ptr = directive->values->stack[j];
				if( ptr != NULL )
					ferite_variable_destroy( CURRENT_SCRIPT, ptr );
			}
			ferite_delete_stack( CURRENT_SCRIPT, directive->values );
			ffree_ngc( directive );
			ferite_directive_stack->stack[i] = NULL;
		}
	}
	ferite_delete_stack( CURRENT_SCRIPT, ferite_directive_stack );
	ferite_directive_stack = NULL;
	FE_LEAVE_FUNCTION(NOWT);
}

void ferite_do_namespace_header( char *name )
{
	FeriteNamespace *ns = NULL;
	FeriteScript *scr = NULL;

	FE_ENTER_FUNCTION;
	if( ferite_namespace_element_exists( CURRENT_SCRIPT, CURRENT_NAMESPACE, name ) == NULL )
	{
		FUD(( "registering namespace %s\n", name ));
		ns = ferite_register_namespace( CURRENT_SCRIPT, name, CURRENT_NAMESPACE );
		ferite_stack_push( FE_NoScript, ferite_compile_stack, ferite_current_compile );
		scr = CURRENT_SCRIPT;

		ferite_current_compile = ferite_compile_record_alloc();
		CURRENT_VARS = NULL;
		CURRENT_CLASS = NULL;
		CURRENT_SCRIPT = scr;
		CURRENT_FUNCTION = NULL;
		CURRENT_NAMESPACE = ns;

		ferite_stack_push( FE_NoScript, ferite_previous_directives_stack, ferite_directive_stack );
		ferite_directive_stack = ferite_create_stack( CURRENT_SCRIPT, 10 );
	}
	else
	{
		ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
		ferite_error( CURRENT_SCRIPT, 0, "  A namespace element called '%s' already exists.\n", name );
		STOP_COMPILE();
	}
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_namespace_extends( char *name )
{
	FeriteNamespace *ns = NULL;
	FeriteNamespaceBucket *nsb = NULL;
	FeriteScript *scr = NULL;

	FE_ENTER_FUNCTION;
	if( (nsb=ferite_find_namespace( CURRENT_SCRIPT, CURRENT_NAMESPACE, name, FENS_NS )) != NULL )
	{
		FUD(( "extending namespace %s\n", name ));
		ns = nsb->data;
		ferite_stack_push( FE_NoScript, ferite_compile_stack, ferite_current_compile );
		scr = CURRENT_SCRIPT;

		ferite_current_compile = ferite_compile_record_alloc();
		CURRENT_VARS = NULL;
		CURRENT_CLASS = NULL;
		CURRENT_SCRIPT = scr;
		CURRENT_FUNCTION = NULL;
		CURRENT_NAMESPACE = ns;

		ferite_stack_push( FE_NoScript, ferite_previous_directives_stack, ferite_directive_stack );
		ferite_directive_stack = ferite_create_stack( CURRENT_SCRIPT, 10 );
	}
	else
	{
		ferite_do_namespace_header( name );
	}
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_namespace_footer()
{
	FE_ENTER_FUNCTION;
	ferite_do_apply_directives();
	ferite_directive_stack = ferite_stack_pop( FE_NoScript, ferite_previous_directives_stack );

	ffree_ngc( ferite_current_compile );
	ferite_current_compile = ferite_stack_pop( FE_NoScript, ferite_compile_stack );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_begin_block()
{
	FE_ENTER_FUNCTION;
	if( CURRENT_SCOPE_FRM != NULL )
		ferite_stack_push( FE_NoScript, CURRENT_SHAD_VARS, CURRENT_SCOPE_FRM );
	CURRENT_SCOPE_FRM = ferite_create_stack( CURRENT_SCRIPT, FE_FUNCTION_VARIABLE_SIZE );
	FE_LEAVE_FUNCTION(NOWT);
}

void ferite_do_end_block()
{
	int i = 0;
	FeriteTag *tag = NULL;
	void *location = NULL;

	FE_ENTER_FUNCTION;
	if( CURRENT_SCOPE_FRM != NULL )
	{
		for( i = 1; i <= CURRENT_SCOPE_FRM->stack_ptr; i++ )
		{
			tag = CURRENT_SCOPE_FRM->stack[i];
			location = ferite_hash_get( CURRENT_SCRIPT, CURRENT_VARS_HASH, tag->name );
			ferite_hash_update( CURRENT_SCRIPT, CURRENT_VARS_HASH, tag->name, tag->value );
			ffree_ngc( location );
			ffree_ngc( tag );
		}
		ferite_delete_stack( CURRENT_SCRIPT, CURRENT_SCOPE_FRM );
	}
	CURRENT_SCOPE_FRM = ferite_stack_pop( FE_NoScript, CURRENT_SHAD_VARS );
	FE_LEAVE_FUNCTION(NOWT);
}

void ferite_do_closure_start()
{
	char *name = fmalloc_ngc(1024);
	char *current_path = ferite_compiler_build_current_path_wannotation_wfunction( FE_FALSE, FE_FALSE );
	FeriteNamespace *space = CURRENT_NAMESPACE;

	sprintf( name, "!closure:%d:%s", ++ferite_closure_count, current_path );
	ffree_ngc(current_path);
	
	CURRENT_NAMESPACE = CURRENT_SCRIPT->mainns;
	ferite_do_class_header( name, "Closure", 0 );
	ferite_cache_register_closure( CURRENT_SCRIPT, name, CURRENT_CLASS );
	CURRENT_CLASS->parent = NULL;
	CURRENT_NAMESPACE = space;
	ferite_do_function_header( "invoke", FE_FALSE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE, FE_FALSE );
	ffree_ngc( name );
}

void ferite_do_closure_end_of_args()
{
	ferite_do_function_start();
	CURRENT_IN_CLOSURE = FE_TRUE;
}

void ferite_do_closure_var_assign( char *name )
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_CLSRE_ASSGN;
	op->line = ferite_scanner_lineno;
	op->opdata = fstrdup(name);
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_closure_end()
{
	FeriteVariable *ferite_temp_variable = NULL;
	FeriteHash *vars = NULL;
	FeriteIterator *iter = NULL;
	FeriteHashBucket *bucket = NULL;
	FeriteClass *closure = NULL;
	int param_count = 0;
	int seen_self = FE_FALSE;
	int seen_super = FE_FALSE;
	
	closure = CURRENT_CLASS;
	closure->parent = CURRENT_SCRIPT->closureClassPtr;
	
	CURRENT_IN_CLOSURE = FE_FALSE;
	ferite_temp_variable = ferite_create_void_variable( CURRENT_SCRIPT, "compiled-in-return-value", FE_STATIC );
	MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
	ferite_do_push( ferite_temp_variable );
	ferite_do_function_footer();

	/* TODO: process the code for the function to see if we have to do any redirects
		* We go through the list of private variables within CURRENT_CLASS and go from there
		* */
	vars = CURRENT_CLASS->object_vars;

	ferite_do_function_header( "constructor", FE_FALSE, FE_FALSE, FE_FALSE, 0, FE_FALSE, FE_FALSE );

	/* Go through the variables and add them to the parameter list */
	iter = ferite_create_iterator( CURRENT_SCRIPT );
	while( (bucket = ferite_hash_walk( CURRENT_SCRIPT, vars, iter )) != NULL ){
		FUD(( "Adding %s to the parameter list\n", bucket->id ));
		if( strcmp( bucket->id, "self" ) == 0 ) {
			ferite_do_add_variable_to_paramlist( "temporaryself", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_REF );
			seen_self = FE_TRUE;
		}
		else if( strcmp( bucket->id, "super" ) == 0 ) {
			ferite_do_add_variable_to_paramlist( "temporarysuper", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_REF );
			seen_super = FE_TRUE;
		}
		else
			ferite_do_add_variable_to_paramlist( bucket->id, F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_REF );
	}
	ffree_ngc( iter );

	if( ! seen_self )
		ferite_do_add_variable_to_paramlist( "temporaryself", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_REF );
	if( ! seen_super )
		ferite_do_add_variable_to_paramlist( "temporarysuper", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_REF );

	ferite_do_function_start();

	/* Go through the variables again and add the assignment code */
	iter = ferite_create_iterator( CURRENT_SCRIPT );
	while( (bucket = ferite_hash_walk( CURRENT_SCRIPT, vars, iter )) != NULL ){
		char *name = bucket->id;
		FUD(( "Setting up assignment for %s\n", bucket->id ));
		if( strcmp( bucket->id, "self" ) == 0 ) 
			ferite_do_variable_push( "temporaryself" );
		else if( strcmp( bucket->id, "super" ) == 0 )
			ferite_do_variable_push( "temporarysuper" );
		else
			ferite_do_variable_push( bucket->id );
		ferite_do_closure_var_assign( name );
	}
	ffree_ngc( iter );
	
	if( ! seen_self ) {
		ferite_do_variable_push( "temporaryself" );
		ferite_do_closure_var_assign( "self" );
	}
	if( ! seen_super ) {
		ferite_do_variable_push( "temporarysuper" );
		ferite_do_closure_var_assign( "super" );
	}

	/* Finish off the function */
	ferite_temp_variable = ferite_create_void_variable( CURRENT_SCRIPT, "compiled-in-return-value", FE_STATIC );
	MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
	ferite_do_push( ferite_temp_variable );
	ferite_do_function_footer();

	/* Finish off the custom class */
	ferite_do_class_footer();

	ferite_temp_variable = ferite_create_class_variable( CURRENT_SCRIPT, "closure-reference-push", closure, FE_STATIC );
	MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
	ferite_do_push( ferite_temp_variable );
	
	iter = ferite_create_iterator( CURRENT_SCRIPT );
	while( (bucket = ferite_hash_walk( CURRENT_SCRIPT, vars, iter )) != NULL ){
		FUD(( "Passing in variable %s\n", bucket->id ));
		ferite_do_variable_push( bucket->id );
		param_count++;
	}
	ffree_ngc( iter );

	if( ! seen_self ) {
		ferite_do_variable_push( "self" );
		param_count++;
	}
	if( ! seen_super ) {
		ferite_do_variable_push( "super" );
		param_count++;
	}

	ferite_do_new_object( param_count );
}

void ferite_do_yield_block( int useCurrent )
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_SET_DELIVER;
	op->addr = useCurrent;
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_get_yield_block()
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_GET_DELIVER;
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_swap_top( int offset )
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_SWAP_TOP;
	op->line = ferite_scanner_lineno;
	op->addr = offset;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_add_variable( char *name, int type, FeriteVariableSubType *subtype, int is_global, int is_final, int is_static, int is_atomic, int state, int is_param )
{
	FeriteVariable *new_variable = NULL;

	switch( type )
	{
		case F_VAR_BOOL:
			new_variable = ferite_create_boolean_variable( CURRENT_SCRIPT, name, 0, FE_ALLOC );
			break;
		case F_VAR_NUM:
			new_variable = ferite_create_number_long_variable( CURRENT_SCRIPT, name, 0, FE_ALLOC );
			break;
		case F_VAR_STR:
			new_variable = ferite_create_string_variable( CURRENT_SCRIPT, name, NULL, FE_ALLOC );
			break;
		case F_VAR_OBJ:
			new_variable = ferite_create_object_variable( CURRENT_SCRIPT, name, FE_ALLOC );
			break;
		case F_VAR_UARRAY:
			new_variable = ferite_create_uarray_variable( CURRENT_SCRIPT, name, 0, FE_ALLOC );
			break;
		default:
			new_variable = ferite_create_void_variable( CURRENT_SCRIPT, name, FE_ALLOC );
	}

	if( new_variable != NULL )
	{
		if( is_final )
		{
			FUD(( "Marking variable %s as final.\n", name ));
			MARK_VARIABLE_AS_FINAL( new_variable );
		}
		new_variable->subtype = subtype;
		ferite_do_add_variable_with_value( name, new_variable, is_global, is_static, is_atomic, state, is_param );
	}
	else
	{
		ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
		ferite_error( CURRENT_SCRIPT, 0, "  Unable to create variable of type \"%s\"\n", type );
		ferite_compile_error = 1;
		STOP_COMPILE();
	}
}

void ferite_do_add_variable_with_value( char *name, FeriteVariable *new_variable, int is_global, int is_static, int is_atomic, int state, int is_parameter )
{
	FeriteHash *current_hash = NULL;
	FeriteTag *tag = NULL;
	void *location = NULL;
	int i = 0;

	FE_ENTER_FUNCTION;

	if( new_variable != NULL )
	{
		if( is_global )
			current_hash = CURRENT_NAMESPACE->data_fork;
		else
		{
			if( CURRENT_FUNCTION != NULL )
			{
				FeriteNamespaceBucket *nsb = ferite_namespace_element_exists(CURRENT_SCRIPT,CURRENT_SCRIPT->mainns,name);
				if( nsb != NULL )
				{
					ferite_warning( CURRENT_SCRIPT,
									"Declaration of %s '%s' shadows %s %s (line %d, in %s)\n",
									(is_parameter ? "parameter" : "variable"),
									name,
									ferite_namespace_bucket_type_to_str( CURRENT_SCRIPT, nsb->type ),
									name,
									ferite_scanner_lineno,
									ferite_scanner_file );
				}
				current_hash = CURRENT_VARS_HASH;
			}
			else if( CURRENT_CLASS != NULL )
			{
				if( is_static )
					current_hash = CURRENT_CLASS->class_vars;
				else
					current_hash = CURRENT_CLASS->object_vars;
			}
			else
				current_hash = CURRENT_NAMESPACE->data_fork;
		}

		if( ferite_hash_get( CURRENT_SCRIPT, current_hash, name ) != NULL && CURRENT_FUNCTION == NULL )
		{
			ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
			ferite_error( CURRENT_SCRIPT, 0, "  A variable called \"%s\" already exists\n", name );
			if( new_variable != NULL )
				ferite_variable_destroy( CURRENT_SCRIPT, new_variable );
			ferite_compile_error = 1;
			STOP_COMPILE();
		}

		if( is_atomic ) {
#ifdef THREAD_SAFE
			new_variable->lock = (void*)aphex_mutex_recursive_create();
#else
			ferite_warning( CURRENT_SCRIPT, "'atomic' keyword can not be used for variable '%s' - please compile ferite with threading!\n", name );
			ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
#endif
		}
		new_variable->state = FE_ITEM_IS_PUBLIC;

		if( is_global )
		{
			ferite_register_ns_variable( CURRENT_SCRIPT, CURRENT_NAMESPACE, name, new_variable );
		}
		else if( CURRENT_FUNCTION != NULL )
		{
			if( strcmp( name, "self" ) == 0 )
			{
				CURRENT_VARS->stack[1] = new_variable;
				ferite_hash_add( CURRENT_SCRIPT, CURRENT_VARS_HASH, name, ferite_int_to_ptr( 1 ) );
			}
			else if( strcmp( name, "super" ) == 0 )
			{
				CURRENT_VARS->stack[2] = new_variable;
				ferite_hash_add( CURRENT_SCRIPT, CURRENT_VARS_HASH, name, ferite_int_to_ptr( 2 ) );
			}
			else
			{
				ferite_stack_push( FE_NoScript, CURRENT_VARS, new_variable );
				location = ferite_hash_get( CURRENT_SCRIPT, CURRENT_VARS_HASH, name );
				
				if( location != NULL )
				{
					if( CURRENT_SCOPE_FRM != NULL )
					{
						for( i = 1; i <= CURRENT_SCOPE_FRM->stack_ptr; i++ )
						{
							tag = CURRENT_SCOPE_FRM->stack[i];
							if( tag != NULL && strcmp( tag->name, name ) == 0 )
							{
								ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
								ferite_error( CURRENT_SCRIPT, 0, "  A variable called \"%s\" has already been declared within the current scope of function %s\n", name, CURRENT_FUNCTION->name );
								ferite_compile_error = 1;
								STOP_COMPILE();
							}
						}

						tag = fmalloc_ngc( sizeof(FeriteTag) );
						tag->name = new_variable->vname;
						tag->value = location;

						ferite_stack_push( FE_NoScript, CURRENT_SCOPE_FRM, tag );
						ferite_hash_delete( CURRENT_SCRIPT, CURRENT_VARS_HASH, name );
						location = ferite_int_to_ptr( CURRENT_VARS->stack_ptr );
					}
					else
					{
						ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
						ferite_error( CURRENT_SCRIPT, 0, "  A variable called \"%s\" already exists within the same scope\n", name );
						ferite_compile_error = 1;
						STOP_COMPILE();
					}
				}
				else
				{
					location = ferite_int_to_ptr( CURRENT_VARS->stack_ptr );
					
					if( CURRENT_SCOPE_FRM ) {
						tag = fmalloc_ngc( sizeof(FeriteTag) );
						tag->name = new_variable->vname;
						tag->value = ferite_int_to_ptr( CURRENT_VARS->stack_ptr );
						ferite_stack_push( FE_NoScript, CURRENT_SCOPE_FRM, tag );
					}
				}

				ferite_hash_add( CURRENT_SCRIPT, CURRENT_VARS_HASH, name,  location);
			}
		}
		else if( CURRENT_CLASS != NULL )
		{
			ferite_register_class_variable( CURRENT_SCRIPT, CURRENT_CLASS, name, new_variable, is_static );
			if( state >= FE_ITEM_IS_PRIVATE )
				new_variable->state = state;
			else
				new_variable->state = FE_ITEM_IS_PUBLIC;
		}
		else
			ferite_register_ns_variable( CURRENT_SCRIPT, CURRENT_NAMESPACE, name, new_variable );

		FE_LEAVE_FUNCTION( NOWT );
	}
}

#ifdef DEBUG
# define REDUCE_DEPTH ferite_call_level -= 2
#else
# define REDUCE_DEPTH
#endif

#define CHECK_FUNCTION_CODE( blah ) \
if( CURRENT_FUNCTION == NULL ) { \
	ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file ); \
	ferite_compile_error = 1; \
	blah; \
	REDUCE_DEPTH; \
	STOP_COMPILE(); \
}

void ferite_do_add_variable_to_paramlist( char *name, int type, FeriteVariableSubType *subtype, int pass_type )
{
	FeriteVariable *new_variable = NULL;
	int max_argcount = 0;

	FE_ENTER_FUNCTION;
	CHECK_FUNCTION_CODE({ ffree_ngc(name); });

	max_argcount = FE_FUNCTION_PARAMETER_MAX_SIZE - 1;

	if( CURRENT_FUNCTION->arg_count == max_argcount )
	{
		ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );
		ferite_error( CURRENT_SCRIPT, 0, "  Maximum number of parameters for function '%s' exceeded, must be less than %d\n", CURRENT_FUNCTION->name, max_argcount );
		ferite_error( CURRENT_SCRIPT, 0, "  [If this is a constructor for a closure, it is usually due the closure trying to\n   bind to too many variables.]\n");
		ferite_compile_error = 1;
		REDUCE_DEPTH;
		STOP_COMPILE();
	}

	if( name[0] != '.' ) {
		ferite_do_add_variable( name, type, subtype, FE_FALSE, FE_FALSE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_TRUE ); /* add the variable to the function */
		/* now lets add the paramter */
		switch( type )
		{
			case F_VAR_BOOL:
				new_variable = ferite_create_boolean_variable( CURRENT_SCRIPT, name, 0, FE_ALLOC );
				break;
			case F_VAR_NUM:
				new_variable = ferite_create_number_long_variable( CURRENT_SCRIPT, name, 0, FE_ALLOC );
				break;
			case F_VAR_STR:
				new_variable = ferite_create_string_variable( CURRENT_SCRIPT, name, NULL, FE_ALLOC );
				break;
			case F_VAR_OBJ:
				new_variable = ferite_create_object_variable( CURRENT_SCRIPT, name, FE_ALLOC );
				break;
			case F_VAR_UARRAY:
				new_variable = ferite_create_uarray_variable( CURRENT_SCRIPT, name, 0, FE_ALLOC );
				break;
			default:
				new_variable = ferite_create_void_variable( CURRENT_SCRIPT, name, FE_ALLOC );
		}
		new_variable->subtype = subtype;
	}
	CURRENT_FUNCTION->signature[CURRENT_FUNCTION->arg_count] = fmalloc_ngc( sizeof( FeriteParameterRecord ) );
	CURRENT_FUNCTION->signature[CURRENT_FUNCTION->arg_count]->variable = new_variable;
	CURRENT_FUNCTION->signature[CURRENT_FUNCTION->arg_count]->has_default_value = FE_FALSE;
	CURRENT_FUNCTION->signature[CURRENT_FUNCTION->arg_count]->pass_type = pass_type;
	CURRENT_FUNCTION->signature[CURRENT_FUNCTION->arg_count]->is_dots = (name[0] == '.' ? FE_TRUE : FE_FALSE);
	CURRENT_FUNCTION->signature[CURRENT_FUNCTION->arg_count]->name = fstrdup(name);
	CURRENT_FUNCTION->arg_count++;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_exit()
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	CHECK_FUNCTION_CODE( NOWT );
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_EXIT;
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_self_push()
{
	FE_ENTER_FUNCTION;
	ferite_do_variable_push( "self" );
	FE_LEAVE_FUNCTION( NOWT );
}
void ferite_do_super_push()
{
	FE_ENTER_FUNCTION;
	ferite_do_variable_push( "super" );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_push( FeriteVariable *var )
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	CHECK_FUNCTION_CODE({ ferite_variable_destroy( CURRENT_SCRIPT, var ); });
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	OP_PUSH( op, var );
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_pop()
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	CHECK_FUNCTION_CODE( NOWT );
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_POP;
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_get_args()
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	CHECK_FUNCTION_CODE( NOWT );
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_ARGS;
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

void __ferite_do_variable_push( char *name, int fromClosure )
{
	FeriteOp *op = NULL;
	int *index = NULL;

	FE_ENTER_FUNCTION;

	FUD(("DO_PUSHVAR: WANTING TO PUSH VARIABLE %s\n", name ));
	CHECK_FUNCTION_CODE({ ffree_ngc( name ); });
	index = ferite_hash_get( CURRENT_SCRIPT, CURRENT_VARS_HASH, name);

	if( index != NULL && (!(strcmp(name,"self") == 0 || strcmp(name,"super") == 0) || ((!fromClosure && !CURRENT_IN_CLOSURE) || (fromClosure))) )
	{
		/* If we can find the variable locally, lets push it */
	 	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
		op->block_depth = ferite_compiler_current_block_depth;
		op->OP_TYPE = F_OP_PUSHINDEX;
		op->addr = *index;
		op->line = ferite_scanner_lineno;
	}
	else
	{
		/* Check that it's not a global because we dont want to look up all of those */
		FeriteNamespaceBucket *nsb = ferite_find_namespace( CURRENT_SCRIPT, CURRENT_SCRIPT->mainns, name, 0 );
		int wantDynamic = FE_TRUE;

		if( CURRENT_IN_CLOSURE == FE_TRUE ) /* Special case */
		{
			FeriteVariable *var = ferite_hash_get( CURRENT_SCRIPT, CURRENT_CLASS->object_vars, name ) ;

			if( nsb == NULL )
			{
				/* We currently have no local variable and therefore are referencing some outside force
				 * To fix this, we add a void variable to the class and add a self.var
				 *
				 * To make matters worse we have to remove the current function for a very short while :)
				 */
				FeriteFunction *cfunction = CURRENT_FUNCTION;
				CURRENT_FUNCTION = NULL;

				FUD(( "Can't locate %s, closure: %d\n", name, CURRENT_IN_CLOSURE ));

				/* Add the variable */
				if( var == NULL )
					ferite_do_add_variable( name, F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_FALSE, FE_TRUE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PRIVATE, FE_FALSE );

				CURRENT_FUNCTION = cfunction;

				/* Do the self, then attr */
				__ferite_do_variable_push( "self", FE_TRUE );
				ferite_do_variable_pushattr( name, FE_TRUE, FE_FALSE );

				wantDynamic = FE_FALSE;
			}
		}

		if( wantDynamic ) /* Do a dynamic bind */
		{
			if( nsb ) {
				op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
				op->block_depth = ferite_compiler_current_block_depth;
				op->OP_TYPE = F_OP_PUSHGLOBAL;
				op->opdata = fstrdup(name);
				op->addr = ferite_hamt_hash_gen(name);
				op->line = ferite_scanner_lineno;
			} else {
				op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
				op->block_depth = ferite_compiler_current_block_depth;
				op->OP_TYPE = F_OP_PUSHVAR;
				op->opdata = fstrdup( name );
				op->line = ferite_scanner_lineno;
			}
		}
	}
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_variable_push( char *name )
{
	__ferite_do_variable_push( name, FE_FALSE );
}

void ferite_do_variable_pushattr( char *name, int is_self, int is_super )
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;

	FUD(("DO_PUSHATTR: WANTING TO PUSH VARIABLE %s\n", name ));
	CHECK_FUNCTION_CODE({ ffree_ngc( name ); });
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_PUSHATTR;
	op->block_depth = ferite_compiler_current_block_depth;
	op->opdata = fstrdup( name );
	op->line = ferite_scanner_lineno;

	if( is_self )
		MARK_SOURCE_SELF(op);
	else if( is_super )
		MARK_SOURCE_RELATIVE(op);
	else
		MARK_SOURCE_EXTERNAL(op);

	FE_LEAVE_FUNCTION( NOWT );
}
void ferite_do_variable_reset( int type )
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_VRST;
	op->block_depth = ferite_compiler_current_block_depth;
	op->opdata = NULL;
	op->addr = type;
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_replace_op_with_index_assign( FeriteOp *op, int real_op )
{
	int *c = NULL;

	FE_ENTER_FUNCTION;
	op->OP_TYPE = F_OP_NOP;

	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	c = fmalloc_ngc(sizeof(int));
	*c = 3;
	OP_MANY(op, FERITE_OPCODE_array_index_assign, c);
	op->line = ferite_scanner_lineno;
	op->opdataf = (void*)((long)real_op);
	FE_LEAVE_FUNCTION(NOWT);
}
int ferite_last_op_is_array_index()
{
	FeriteOp *op = NULL;
	int valid = FE_FALSE;
	FE_ENTER_FUNCTION;
	op = ferite_current_op( CURRENT_FUNCTION->bytecode );
	if( op->OP_TYPE == F_OP_BINARY && op->addr == FERITE_OPCODE_array_index )
		valid = FE_TRUE;
	FE_LEAVE_FUNCTION(valid);
}
FeriteOp *ferite_do_get_current_op()
{
	FE_ENTER_FUNCTION;
	FE_LEAVE_FUNCTION( ferite_current_op( CURRENT_FUNCTION->bytecode ) );
}
void ferite_do_many_op( int opptr , int n)
{
	FeriteOp *op = NULL;
	int *c = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	c = fmalloc_ngc(sizeof(int));
	*c = n;
	OP_MANY(op, opptr, c);
	op->line = ferite_scanner_lineno;

	if( opptr == FERITE_OPCODE_array_index_assign ) {
		op->opdataf = (void*)FERITE_OPCODE_assign;
	}

	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_binary_op( int opptr )
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	OP_BINARY( op, opptr, NULL );
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_unary_op( int opptr )
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	OP_UNARY( op, opptr, NULL );
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

#define CHECK_PARAM_COUNT												  \
max_argcount = FE_FUNCTION_PARAMETER_MAX_SIZE - 1;					 \
if( CURRENT_FUNCTION->klass != NULL && !CURRENT_FUNCTION->is_static )  \
max_argcount -= 2;												 \
if( CURRENT_FUNCTION->arg_count > FE_FUNCTION_PARAMETER_MAX_SIZE )	 \
{																	\
	ferite_error( CURRENT_SCRIPT, 0, "Compile Error: on line %d, in %s\n", ferite_scanner_lineno, ferite_scanner_file );	\
	ferite_error( CURRENT_SCRIPT, 0, "  Maximum number of arguments for %s call '%s' exceeded, must be less than %d\n",	\
					  ( CURRENT_FUNCTION->klass == NULL ? "function" : "method" ), CURRENT_FUNCTION->name, max_argcount ); \
	ferite_compile_error = 1;										\
	REDUCE_DEPTH;													\
	STOP_COMPILE();												  \
}

void ferite_do_yield_call( int arg_count )
{
	FeriteOp *op = NULL;
	int max_argcount = 0;

	FE_ENTER_FUNCTION;

	CHECK_PARAM_COUNT;

	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_DELIVER;
	op->opdata = NULL;

	/* the function call data */
	op->opdataf = fmalloc_ngc( sizeof( FeriteOpFncData ) );
	op->opdataf->argument_count = arg_count;
	op->line = ferite_scanner_lineno;

	FE_LEAVE_FUNCTION( NOWT );
}
void ferite_do_push_main_namespace()
{
	FeriteOp *op = NULL;
	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_PUSHGLOBAL;
	op->opdata = NULL;
	op->addr = ferite_hamt_hash_gen("__ferite_script_main_namespace__");
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}
void ferite_do_function_call( char *name, int arg_count )
{
	FE_ENTER_FUNCTION;
	ferite_do_object_function_call( name, arg_count, FE_FALSE, FE_FALSE );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_object_function_call( char *name, int arg_count, int is_self, int is_super )
{
	FeriteOp *op = NULL;
	int max_argcount = 0;

	FE_ENTER_FUNCTION;

	CHECK_FUNCTION_CODE({ ffree_ngc( name ); });

	CHECK_PARAM_COUNT;

	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_METHOD;
	op->opdata = fstrdup( name );

	/* the function call data */
	op->opdataf = fmalloc_ngc( sizeof( FeriteOpFncData ) );
	op->opdataf->argument_count = arg_count;

	if( is_self )
		MARK_SOURCE_SELF(op);
	else if( is_super )
		MARK_SOURCE_RELATIVE(op);
	else
		MARK_SOURCE_EXTERNAL(op);

	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_if_statement()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	CHECK_FUNCTION_CODE( NOWT );
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_BNE;
	op->line = ferite_scanner_lineno;
	req = ferite_create_request( op, IF_JUMP_THEN );
	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );
	FE_LEAVE_FUNCTION( NOWT );
}

/*
 * This is the statement is purely here for && and ||, it's purpose is the
 * opposite of the above function.
 */
void ferite_do_not_if_statement()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	CHECK_FUNCTION_CODE( NOWT );
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_BIE;
	op->line = ferite_scanner_lineno;
	req = ferite_create_request( op, IF_JUMP_THEN );
	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_after_then_statement()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
    long addr;

	FE_ENTER_FUNCTION;
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = (FeriteBkRequest *)ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	FUD(("Popped %d off request stack\n", req->type));
	req->reqop->addr = addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );
	FE_LEAVE_FUNCTION( NOWT );
}

/* blum */
void ferite_do_after_then_before_else_statement()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *jump_else_op, *do_else_op;
	long do_else_op_offset;

	FE_ENTER_FUNCTION;
	/* we jump over the else block */
	jump_else_op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	jump_else_op->block_depth = ferite_compiler_current_block_depth;
	jump_else_op->OP_TYPE = F_OP_JMP;
	jump_else_op->line = ferite_scanner_lineno;

	/* we jump to this op when we do else {} */
	do_else_op_offset = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	do_else_op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode ); /* so we know where to jump, but dont put an NOP there */
	do_else_op->OP_TYPE = F_OP_NOP;

	req = (FeriteBkRequest *)ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	FUD(("Popped %d off request stack\n", req->type));
	req->reqop->addr = do_else_op_offset;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );

	req = ferite_create_request( jump_else_op, IF_JUMP_ELSE );
	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_after_else_statement()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
    long next_op;

	FE_ENTER_FUNCTION;
	next_op = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = (FeriteBkRequest *)ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	FUD(("Popped %d off request stack\n", req->type));
	req->reqop->addr = next_op;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );
	FE_LEAVE_FUNCTION( NOWT );
}

/* while stuff
*
* a:  expr
*	 BNE	b
*	 block
*	 JMP	a
* b:
*/

void ferite_do_while_begin()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
	long next_op;

	FE_ENTER_FUNCTION;
	next_op = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = ferite_create_request( op, WHILE_JUMP_TO );
	req->addr = next_op;
	ferite_stack_push( FE_NoScript, ferite_bck_look_stack, req );

	/* We need this to make nested loop structures work */
	req = ferite_create_request( NULL, BREAK_SEPARATOR );
	ferite_stack_push( FE_NoScript, ferite_break_look_stack, req );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_while_after_expr()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_BNE;
	op->block_depth = ferite_compiler_current_block_depth;
	op->line = ferite_scanner_lineno;
	req = ferite_create_request( op, WHILE_JUMP_BLOCK );
	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_while_end()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
	long next_addr;

	FE_ENTER_FUNCTION;
	/* hook up jump back */
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_JMP;
	op->block_depth = ferite_compiler_current_block_depth;
	op->line = ferite_scanner_lineno;

	next_addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	ferite_process_breaks( WHILE_JUMP_TO, next_addr );

	req = (FeriteBkRequest *)ferite_stack_pop( FE_NoScript, ferite_bck_look_stack );
	FUD(("Popped %d off request stack\n", req->type));
	/*	fprintf( stderr, "while end type %d\n", req->type );*/
	op->addr = req->addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(op->opdata) );
	ferite_destroy_request( req );

	/* hook up jump over */
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = (FeriteBkRequest *)ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	FUD(("Popped %d off request stack\n", req->type));
	req->reqop->addr = next_addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );
	FE_LEAVE_FUNCTION( NOWT );
}

/* ********** FOR LOOP STUFF */

/* for( init , */

void ferite_do_for_loop_start()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
	long next_op;

	FE_ENTER_FUNCTION;
	next_op = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = ferite_create_request( op, FOR_TEST_START );
	req->addr = next_op;
	ferite_stack_push( FE_NoScript, ferite_bck_look_stack, req );

	/* We need this to make nested loop structures work */
	req = ferite_create_request( NULL, BREAK_SEPARATOR );
	ferite_stack_push( FE_NoScript, ferite_break_look_stack, req );

	FE_LEAVE_FUNCTION( NOWT );
}

/* test , */

void ferite_do_for_loop_itterate()
{

	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
	long next_op;

	FE_ENTER_FUNCTION;
	/* jump out of the for loop */
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_BNE;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;
	req = ferite_create_request( op, FOR_JUMP_BLOCK );
	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );

	/* jump over the increment block (3rd expr) of the while loop */
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_JMP;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;
	req = ferite_create_request( op, FOR_JUMP_INCR );
	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );

	/* push on the addrs of the increment block start */
	next_op = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = ferite_create_request( op, FOR_INCR_START );
	req->addr = next_op;
	ferite_stack_push( FE_NoScript, ferite_bck_look_stack, req );
	FE_LEAVE_FUNCTION( NOWT );
}

/*	INCR ) */

void ferite_do_for_loop_block()
{
	FeriteBkRequest *req, *req_incr_start, *req_test_start;
	FeriteOp *op = NULL;
	long next_op;

	FE_ENTER_FUNCTION;
	/* so we dont lose this request */
	req_incr_start = ferite_stack_pop( FE_NoScript, ferite_bck_look_stack );
	FUD(("Popped %d off stack\n", req_incr_start->type ));

	/* setupa jmp op to go back to the test */
	req_test_start = ferite_stack_pop( FE_NoScript, ferite_bck_look_stack );
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_JMP;
	op->addr = req_test_start->addr;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(op->opdata) );
	FUD(("Popped %d off stack\n", req_test_start->type ));
	ferite_destroy_request( req_test_start );

	/* push first pop back onto the stack */
	ferite_stack_push( FE_NoScript, ferite_bck_look_stack, req_incr_start );
	FUD(("Popped %d off stack\n", req_incr_start->type ));

	/* now lets sort out the jump increment block code */
	next_op = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	FUD(("Popped %d off stack\n", req->type ));
	req->reqop->addr = next_op;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );

	FE_LEAVE_FUNCTION( NOWT );
}

/* block */

void ferite_do_for_loop_end()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
	long next_op;

	FE_ENTER_FUNCTION;
	/* jump back to incr block */
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_JMP;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;
	next_op = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );

	/* process dem breaks's */
	ferite_process_breaks( FOR_INCR_START, next_op );

	req = ferite_stack_pop( FE_NoScript, ferite_bck_look_stack );
	FUD(("Popped %d off stack\n", req->type ));
	op->addr = req->addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(op->opdata) );
	ferite_destroy_request( req );

	/* jump over block */
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	FUD(("Popped %d off stack\n", req->type ));
	req->reqop->addr = next_op;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );
	FE_LEAVE_FUNCTION( NOWT );
}

/* ************ do stuff */

void ferite_do_do_start()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
	long next_op;

	FE_ENTER_FUNCTION;
	next_op = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = ferite_create_request( op, DO_START );
	req->addr = next_op;
	ferite_stack_push( FE_NoScript, ferite_bck_look_stack, req );

	/* We need this to make nested loop structures work */
	req = ferite_create_request( NULL, BREAK_SEPARATOR );
	ferite_stack_push( FE_NoScript, ferite_break_look_stack, req );

	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_do_end()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
	long addr;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_BIE;

	/* process dem breaks's */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	ferite_process_breaks( DO_START, addr );

	req = ferite_stack_pop( FE_NoScript, ferite_bck_look_stack );
	FUD(("Popped %d off stack\n", req->type ));
	op->addr = req->addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(op->opdata) );
	ferite_destroy_request( req );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_new_object( int arg_count )
{
	FeriteOp *op = NULL;
	int max_argcount = 0;

	FE_ENTER_FUNCTION;
	CHECK_FUNCTION_CODE(NOWT);
	CHECK_PARAM_COUNT;

	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NEWOBJ;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;

	/* the function call data */
	op->opdataf = fmalloc_ngc( sizeof( FeriteOpFncData ) );
	op->opdataf->argument_count = arg_count;
	FE_LEAVE_FUNCTION( NOWT );
}

/* **************************************************************************************/
/* ******* ERROR STUFF ******************************************************************/
void ferite_do_monitor_block()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_ERR;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;
	req = ferite_create_request( op, ERR_START );
	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_before_handle_block()
{
	FeriteBkRequest *req, *req2;
	FeriteOp *op, *op_two;
    long addr;

	FE_ENTER_FUNCTION;

	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_ERR;
	op->addr = -1;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;

	op_two = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op_two->OP_TYPE = F_OP_JMP;
	op_two->line = ferite_scanner_lineno;
	op_two->block_depth = ferite_compiler_current_block_depth;
	req2 = ferite_create_request( op_two, FIX_BLOCK_JMP ); /* jump instruction to jump fix block */

	/* this sets the code to jump to the fix block if there is an error
		* goes back and sets the address to jump to */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_ERR;
	op->addr = -1;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;

	req = (FeriteBkRequest *)ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	FUD(("Popped %d off request stack\n", req->type));
	req->reqop->addr = addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );

	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req2 ); /* jump the fix block */

	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_after_fix_block()
{
	FeriteBkRequest *req = NULL;
	long addr;

	/* we dont have an else block */
	FE_ENTER_FUNCTION;
	/* the address to jump over the fix block */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
/*
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_ERR;
	op->addr = -1;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;
*/
	req = (FeriteBkRequest *)ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	req->reqop->addr = addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_after_fix_before_else_block()
{
	/* we have an else block */
	FeriteBkRequest *req, *req2;
	FeriteOp *op = NULL;
    long addr;

	FE_ENTER_FUNCTION;
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_JMP;
	op->line = (unsigned int)ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;
	req2 = ferite_create_request( op, JMP_ERR_ELSE ); /* jump instruction to jump else block */

	/* this sets the code to jump to the else block if there sn't an error */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_ERR;
	op->addr = -1;
	op->line = (unsigned int)ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(op->opdata) );

	req = (FeriteBkRequest *)ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	FUD(("Popped %d off request stack\n", req->type));
	req->reqop->addr = addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );

	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req2 ); /* jump the fix block */

	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_after_fix_else_statement()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *op = NULL;
    long addr;

	/* we need this to jump over the else block */
	FE_ENTER_FUNCTION;
	/* the address to jump over the else block */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	op = ferite_get_next_op_address( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_NOP;
	req = (FeriteBkRequest *)ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	req->reqop->addr = addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
	ferite_destroy_request( req );
	FE_LEAVE_FUNCTION( NOWT );
}

/* the dreaded switch statment */

void ferite_do_pre_switch()
{
	FeriteBkRequest *req = NULL;
	FeriteOp *jmp_op, *pop_op;
	long addr;

	FE_ENTER_FUNCTION;

	/* JMP */
	jmp_op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	jmp_op->OP_TYPE = F_OP_JMP;
	jmp_op->line = ferite_scanner_lineno;
	jmp_op->block_depth = ferite_compiler_current_block_depth;

	/* POP */
	pop_op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	pop_op->OP_TYPE = F_OP_POP;
	pop_op->line = ferite_scanner_lineno;
	pop_op->block_depth = ferite_compiler_current_block_depth;

	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	jmp_op->addr = addr;
	MARK_VARIABLE_AS_COMPILED( PTR2VAR(jmp_op->opdata) );

	/* this is where the continue jumps to in a switch statement */
	req = ferite_create_request( jmp_op, SWITCH_CONTINUE_JUMP_TO );
	req->addr = addr;
	ferite_stack_push( FE_NoScript, ferite_bck_look_stack, req );

	/* We need this to make nested loop structures work */
	req = ferite_create_request( NULL, BREAK_SEPARATOR );
	ferite_stack_push( FE_NoScript, ferite_break_look_stack, req );

	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_case_block_start()
{
	FeriteOp *op = NULL;
	FeriteBkRequest *req = NULL;
	long addr = 0;

	FE_ENTER_FUNCTION;

	/* case expr */
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_CASE;
	op->addr = FERITE_OPCODE_case;
	op->opdata = NULL;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;

	/* BNE */
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_BNE;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;

	/* here we hook up the addresses such that we come here if we hit this case block */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	req = ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	if( req == NULL || req->type != SWITCH_NEXT_CASE_BLOCK )
	{
		if( req != NULL )
		{
			ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );
		}
	}
	else
	{
		req->reqop->addr = addr;
		MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
		ferite_destroy_request( req );
	}

	/* this sets up the branch to the next case block if the case statement doesn't match */
	req = ferite_create_request( op, SWITCH_NEXT_CASE );
	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );

	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_default_block_start()
{
	FeriteBkRequest *req = NULL;
	long addr = 0;

	FE_ENTER_FUNCTION;

	/* here we hook up the addresses such that we come here if we hit this case block */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	req = ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	if( req == NULL || req->type != SWITCH_NEXT_CASE_BLOCK )
	{
		if( req != NULL )
		{
			ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );
		}
	}
	else
	{
		req->reqop->addr = addr;
		MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
		ferite_destroy_request( req );
	}
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_case_block_end()
{
	FeriteOp *op = NULL;
	FeriteBkRequest *req = NULL;
	long addr= 0;

	FE_ENTER_FUNCTION;

	/* jump over the next case block */
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_JMP;
	op->line = ferite_scanner_lineno;
	op->block_depth = ferite_compiler_current_block_depth;

	/* link up the jump to the next case statement */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	req = ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	if( req->type != SWITCH_NEXT_CASE )
	{
		ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );
	}
	else
	{
		req->reqop->addr = addr;
		MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
		ferite_destroy_request( req );
	}

	/* this is for the afore mentioned jump */
	req = ferite_create_request( op, SWITCH_NEXT_CASE_BLOCK );
	ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );

	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_post_switch()
{
	FeriteOp *op = NULL;
	FeriteBkRequest *req = NULL;
	long addr;

	FE_ENTER_FUNCTION;

	/* here we hook up the addresses such that we come here if we hit this case block */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	req = ferite_stack_pop( FE_NoScript, ferite_fwd_look_stack );
	if( req != NULL )
	{
		if( req->type != SWITCH_NEXT_CASE_BLOCK )
		{
			/*	fprintf( stderr, "foobar %d:%d\n", __LINE__, req->type );*/
			ferite_stack_push( FE_NoScript, ferite_fwd_look_stack, req );
		}
		else
		{
			req->reqop->addr = addr;
			MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata) );
			ferite_destroy_request( req );
		}
	}

	/* hook up breaks to jump to the pop */
	addr = ferite_get_next_op_loc( CURRENT_FUNCTION->bytecode );
	ferite_process_breaks( SWITCH_CONTINUE_JUMP_TO, addr );

	req = ferite_stack_pop( FE_NoScript, ferite_bck_look_stack );
	ferite_destroy_request( req );

	/* POP */
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->OP_TYPE = F_OP_POP;
	op->block_depth = ferite_compiler_current_block_depth;
	op->line = ferite_scanner_lineno;

	FE_LEAVE_FUNCTION( NOWT );
}

/* loop control */
void ferite_do_break()
{
	FeriteOp *op = NULL;
	FeriteBkRequest *req, *breq;
	int i;

	FE_ENTER_FUNCTION;
	for( i = ferite_bck_look_stack->stack_ptr; i > 0; i-- )
	{
		req = ferite_bck_look_stack->stack[i];
		if( req->type == FOR_INCR_START || req->type == WHILE_JUMP_TO || req->type == DO_START || req->type == SWITCH_CONTINUE_JUMP_TO )
		{
			op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
			op->OP_TYPE = F_OP_JMP;
			op->line = ferite_scanner_lineno;
			breq = ferite_create_request( op, req->type );
			ferite_stack_push( FE_NoScript, ferite_break_look_stack, breq );
			break;
		}
	}
	if( op == NULL )
	{
		ferite_warning( CURRENT_SCRIPT, "Trying to use break in non-looping block. (ignoring)\n" );
	ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
	}
	FE_LEAVE_FUNCTION(NOWT);
}

/* for this we simply go up the ferite_bck_look_stack and get the address of the first
* structure :) */
void ferite_do_continue()
{
	FeriteOp *op = NULL;
	FeriteBkRequest *req = NULL;
	int i;

	FE_ENTER_FUNCTION;
	for( i = ferite_bck_look_stack->stack_ptr; i > 0; i-- )
	{
		req = ferite_bck_look_stack->stack[i];
		if( req->type == FOR_INCR_START || req->type == WHILE_JUMP_TO || req->type == DO_START || req->type == SWITCH_CONTINUE_JUMP_TO )
		{
			op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
			op->OP_TYPE = F_OP_JMP;
			op->addr = req->addr;
			op->line = ferite_scanner_lineno;
			MARK_VARIABLE_AS_COMPILED( PTR2VAR(op->opdata) );
			break;
		}
	}
	if( op == NULL )
	{
		ferite_warning( CURRENT_SCRIPT, "Trying to use continue in non-looping block. (ignoring)\n" );
	ferite_warning( CURRENT_SCRIPT, "  [on line %d, in %s]\n", ferite_scanner_lineno, ferite_scanner_file );
	}
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_process_breaks( int starttag, long address )
{
	FeriteBkRequest *req = NULL;

	FE_ENTER_FUNCTION;
	req = ferite_stack_top( FE_NoScript, ferite_break_look_stack );
	while( req != NULL && req->type == starttag )
	{
		ferite_stack_pop( FE_NoScript, ferite_break_look_stack );

		req->reqop->addr = address;
		MARK_VARIABLE_AS_COMPILED( PTR2VAR(req->reqop->opdata ) );

		ferite_destroy_request( req );
		req = ferite_stack_top( FE_NoScript, ferite_break_look_stack );
	}

	if( req->type == BREAK_SEPARATOR )
	{
		ferite_stack_pop( FE_NoScript, ferite_break_look_stack );
		ferite_destroy_request( req );
	}

	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_do_raise()
{
	FeriteOp *op = NULL;

	FE_ENTER_FUNCTION;
	CHECK_FUNCTION_CODE( NOWT );
	op = ferite_get_next_op( CURRENT_FUNCTION->bytecode );
	op->block_depth = ferite_compiler_current_block_depth;
	op->OP_TYPE = F_OP_RAISE;
	op->line = ferite_scanner_lineno;
	FE_LEAVE_FUNCTION( NOWT );
}

/* this is builder stuff */
void ferite_do_create_builder_variable( char *name, char *data )
{
	FeriteVariable *var = NULL;
	FeriteNamespaceBucket *nsb = NULL;
	FeriteScript *script = NULL;

	FE_ENTER_FUNCTION;
	if( ferite_keep_native_function_data != 0 )
	{
		nsb = ferite_namespace_element_exists( CURRENT_SCRIPT, CURRENT_SCRIPT->mainns, name );
		if( nsb != NULL )
		{
			/* the variable already exists, so we simply extend it :) */
			var = nsb->data;
			ferite_str_data_cat( NULL, VAS(var), data, strlen(data) );
			FE_LEAVE_FUNCTION(NOWT);
		}
		ferite_register_ns_variable( CURRENT_SCRIPT, CURRENT_SCRIPT->mainns, name,
									 fe_new_str( name, data, strlen(data), FE_CHARSET_DEFAULT ) );
	}
	FE_LEAVE_FUNCTION(NOWT);
}

/**
* @end
 */
