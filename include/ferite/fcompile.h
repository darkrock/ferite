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

#ifndef __FERITE_COMPILER__
#define __FERITE_COMPILER__

#define FERITE_BLOCK_NOP          0
#define FERITE_BLOCK_CTRL         1
#define FERITE_BLOCK_VAR          2
#define IF_JUMP_THEN              3
#define IF_JUMP_ELSE              4
#define WHILE_JUMP_TO             5
#define WHILE_JUMP_BLOCK          6
#define FOR_TEST_START            7
#define FOR_JUMP_BLOCK            8
#define FOR_JUMP_INCR             9
#define FOR_INCR_START            10
#define DO_START                  11
#define DO_END                    12
#define ERR_START                 13
#define FIX_BLOCK_JMP             14
#define JMP_ERR_ELSE              15
#define SWITCH_CONTINUE_JUMP_TO   16
#define SWITCH_NEXT_CASE          17
#define SWITCH_NEXT_CASE_BLOCK    18

#define BREAK_SEPARATOR           19

FERITE_API void ferite_init_compiler();
FERITE_API void ferite_deinit_compiler();

FeriteBkRequest *ferite_create_request( FeriteOp *op, int type );
void ferite_destroy_request( FeriteBkRequest *ptr );
  
FERITE_API void ferite_compiler_keep_native_code();
FERITE_API int  ferite_compiler_include_in_list( FeriteScript *script, char *name );

int ferite_parse(void);
void ferite_prepare_parser( char *s );
void ferite_parser_script_normalise( char *str );
void ferite_prepare_parser_cr( char *script, int strip );
void ferite_clean_parser(void);

char *ferite_compiler_build_current_path_wannotation_wfunction( int annotate, int function );
char *ferite_compiler_build_current_path_wannotation( int annotate );
char *ferite_compiler_build_current_path();

FERITE_API FeriteScript *ferite_compile_string( char *str );
FERITE_API FeriteScript *ferite_compile_string_with_path( char *str, char **paths );
FERITE_API FeriteScript *ferite_script_compile_with_script_and_path( FeriteScript *script, char *filename, char **paths );
FERITE_API FeriteScript *ferite_compile_string_with_script_and_path( FeriteScript *script, char *str, char **paths );
FERITE_API void ferite_set_filename( char *name );
void ferite_save_lexer();
void ferite_restore_lexer();
void ferite_scanner_stop_dot_label();
void ferite_scanner_want_dot_label();
void ferite_scanner_want_typing_label();
void ferite_scanner_stop_typing_label();
void ferite_scanner_parse_macro( char *m );
int  ferite_scanner_end_macro();
void ferite_scanner_start_directive_list();
void ferite_scanner_stop_directive_list();
int  ferite_scanner_block_read( char current_character, char lhs, char rhs, char early_stop, int return_token, void *feplval );

char *ferite_compiler_entry_function( char *filename );
void ferite_do_uses( char *name );
int  ferite_do_load_script( char *name );
void ferite_do_script_finish();

FERITE_API FeriteVariable *ferite_script_include( FeriteScript *script, char *filename );
FERITE_API FeriteVariable *ferite_script_eval( FeriteScript *script, char *str );
void ferite_do_function_header( char *name, int is_static, int is_native, int is_atomic, int state, int final, int directive );
void ferite_do_function_return_type( char *type );
int  ferite_do_function_start();
void ferite_do_function_native_block( char *contents, char *file, int line );
void ferite_do_function_cleanup();
void ferite_do_function_footer();
void ferite_do_class_header( char *name, char *extends, int state );
void ferite_do_class_implements( char *implements );
void ferite_do_class_footer();
void ferite_do_add_directive( char *directive, FeriteStack *values );
void ferite_do_apply_directives();
void ferite_do_namespace_item_rename( char *from, char *to );
void ferite_do_namespace_header( char *name );
void ferite_do_namespace_extends( char *name );
void ferite_do_namespace_footer();
void ferite_do_begin_block();
void ferite_do_end_block();
void ferite_do_closure_start();
void ferite_do_closure_end_of_args();
void ferite_do_closure_end();
void ferite_do_yield_block( int useCurrent );
void ferite_do_get_yield_block();

void ferite_do_add_variable_to_paramlist( char *name, int type, FeriteVariableSubType *subtype, int pass_type );
void ferite_do_add_variable( char *name, int type, FeriteVariableSubType *subtype, int is_global, int is_final, int is_static, int is_atomic, int state, int is_param );
void ferite_do_add_variable_with_value( char *name, FeriteVariable *new_variable, int is_global, int is_static, int is_atomic, int state, int is_parameter );

void ferite_do_self_push();
void ferite_do_super_push();
void ferite_do_push( FeriteVariable *var );
void ferite_do_pop();
void ferite_do_get_args();
void ferite_do_yield_call( int args );
void ferite_do_swap_top( int offset );
void ferite_do_variable_push( char *name );
void ferite_do_variable_pushattr( char *name, int is_self, int is_super );
void ferite_do_variable_reset( int type );
void ferite_do_many_op( int opptr, int n);
void ferite_do_binary_op( int opptr );
void ferite_do_unary_op( int opptr );
void ferite_do_push_main_namespace();
void ferite_do_function_call( char *name, int arg_count );
void ferite_do_object_function_call( char *name, int arg_count, int is_self, int is_super );
void ferite_do_exit();

void ferite_do_if_statement();
void ferite_do_not_if_statement();
void ferite_do_after_then_statement();
void ferite_do_after_then_before_else_statement();
void ferite_do_after_else_statement();
    
void ferite_do_while_begin();
void ferite_do_while_after_expr();
void ferite_do_while_end();

void ferite_do_for_loop_start();
void ferite_do_for_loop_itterate();
void ferite_do_for_loop_block();
void ferite_do_for_loop_end();

void ferite_do_do_start();
void ferite_do_do_end();

void ferite_do_new_object( int arg_count );

void ferite_do_monitor_block();
void ferite_do_before_handle_block();
void ferite_do_after_fix_block();
void ferite_do_after_fix_before_else_block();
void ferite_do_after_fix_else_statement();

void ferite_do_pre_switch();
void ferite_do_pre_case_block_start();
void ferite_do_default_block_start();
void ferite_do_case_block_start();
void ferite_do_case_block_end();
void ferite_do_post_switch();

void ferite_do_break();
void ferite_do_continue();
void ferite_process_breaks( int starttag, int address );
void ferite_do_raise();

void ferite_clean_compiler();

void ferite_do_create_builder_variable( char *name, char *data );

int ferite_last_op_is_array_index();
void ferite_do_replace_op_with_index_assign( FeriteOp *op, int real_op );
FeriteOp *ferite_do_get_current_op();

/* ferite_execute.c */
#define EXTERNAL_ENTER( FUNCTION )  \
{                                   \
	FeriteExecuteRec exec;          \
	exec.function = FUNCTION;       \
	exec.variable_list = NULL;      \
	exec.stack = NULL;              \
	exec.parent = script->gc_stack; \
	exec.line = (FUNCTION->native_information ? FUNCTION->native_information->line : 0); \
	script->gc_stack = &exec
	
#define EXTERNAL_EXIT()             \
	script->gc_stack = exec.parent; \
}

FERITE_API void ferite_clean_executor( FeriteScript *script );
FERITE_API void ferite_stop_execution( FeriteScript *script, int return_value );
FERITE_API int ferite_is_executing( FeriteScript *script );

FE_NATIVE_FUNCTION( ferite_script_function_execute );

FeriteVariable *ferite_script_real_function_execute( FeriteScript *script, void *container, FeriteObject *current_recipient, FeriteFunction *function, FeriteNamespace *mainns, FeriteExecuteRec *exec, FeriteVariable **params );
FERITE_API void ferite_clean_up_exec_rec( FeriteScript *script, FeriteExecuteRec *exec );

FERITE_API FeriteVariable **ferite_create_parameter_list( FeriteScript *script, int size );
FERITE_API FeriteVariable **ferite_add_to_parameter_list( FeriteVariable **list, FeriteVariable *var );
FERITE_API void ferite_delete_parameter_list( FeriteScript *script, FeriteVariable **list );
FERITE_API FeriteVariable **ferite_create_parameter_list_from_data( FeriteScript *script, char *format, ... );
FERITE_API FeriteVariable *ferite_call_function( FeriteScript *script, void *container, FeriteObject *block, FeriteFunction *function, FeriteVariable **params );

/* ferite_script.c */
FERITE_API int ferite_script_being_deleted( FeriteScript *script );
FERITE_API int ferite_script_clean( FeriteScript *script );
FERITE_API void ferite_init_cache( FeriteScript *script );
FERITE_API void ferite_free_cache( FeriteScript *script );
FERITE_API int ferite_script_attach_data( FeriteScript *script, char *key, void *data, FeriteAttachedDataCleanup cleanup );
FERITE_API int ferite_script_remove_data( FeriteScript *script, char *key );
FERITE_API void *ferite_script_fetch_data( FeriteScript *script, char *key );

#endif /* __FERITE_COMPILER__ */
