%{
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

/*
 *  This is the parser for the ferite programming language
 *  Written by Chris Ross (chris@ferite.org)
 */

#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

#include "ferite.h"
#include "ferite/fcompile.h"
#include "ferite/foop.h"
#include <string.h>
#include <setjmp.h>

#ifdef VCWIN32
# include "snprintf.h" /* This is so that we have somethings */
#endif

#define YYERROR_VERBOSE

extern jmp_buf  ferite_compiler_jmpback;
extern int	    ferite_scanner_lineno;
extern char	   *ferite_scanner_file;
extern int	    ferite_compile_error;
extern char	   *ferite_last_token_alloc;
extern FeriteCompileRecord *ferite_current_compile;

extern FeriteStack *ferite_compiled_arrays_stack;
extern FeriteStack *ferite_argcount_stack;

char		   *ferite_last_function = NULL;
int			 	ferite_last_type = 0;
FeriteString   *ferite_last_clamp = NULL;
FeriteVariable *ferite_temp_variable = NULL;
char			ferite_fetypebuf[1024];
char			ferite_fenamebuf[1024];
int			 	ferite_var_is_global = FE_FALSE;
int			 	ferite_var_is_params = FE_FALSE;
int			 	ferite_var_is_final = FE_FALSE;
int			 	ferite_var_is_static = FE_FALSE;
int			 	ferite_var_is_atomic = FE_FALSE;
int			 	ferite_var_is_native = FE_FALSE;
int			 	ferite_var_pass_type = FE_BY_VALUE;
int			 	ferite_item_state = FE_ITEM_IS_PRIVATE;
int			 	ferite_class_state = 0;
int			 	ferite_current_arg_count = 0;
int			 	ferite_var_array_count = 0;
int			 	ferite_namespace_naturaln = 0;
int			 	ferite_function_is_directive = 0;
FeriteStack	   *ferite_directive_parameters = NULL;
FeriteHash	   *ferite_expansion_hash = NULL;
FeriteHash	   *ferite_expansion_map_hash = NULL;
long		 	ferite_expansion_map_offset = 0;
int			 	ferite_objcall_is_self = FE_FALSE;
int			 	ferite_objcall_is_super = FE_FALSE;
int				ferite_function_return_type = F_VAR_VOID;
FeriteOp	   *ferite_assign_op_to_change[128];
int			 	ferite_assign_is_array[128];
int			 	ferite_assign_loc = 0;
int				ferite_return_type_was_provided = FE_FALSE;
int				ferite_seeking_return_type_fix = FE_FALSE;
int				ferite_hint_depth = 0;
char			ferite_hint_typestring[512];
FeriteVariableSubType *ferite_hint_current_type = NULL;

int  feplex();
void feperror( char *message );
void ferite_scanner_go_native();

void ferite_lexer_fix_native_blocks();
void ferite_scanner_want_end_hint();
void ferite_scanner_stop_end_hint();

#define FECT	ferite_last_token_alloc = NULL

#define CURRENT_SCRIPT ferite_current_compile->script

#define ASSIGNMENT_SETUP() do { \
	ferite_assign_loc++; \
	ferite_assign_op_to_change[ferite_assign_loc] = NULL; \
	ferite_assign_is_array[ferite_assign_loc] = FE_FALSE; \
	if( ferite_last_op_is_array_index() ) { \
		ferite_assign_op_to_change[ferite_assign_loc] = ferite_do_get_current_op(); \
		ferite_assign_is_array[ferite_assign_loc] = FE_TRUE; \
	} } while (0)
#define ASSIGNMENT_TEARDOWN( ACTUALOP, SUBOP ) do { \
	if( ferite_assign_is_array[ferite_assign_loc] ) \
		ferite_do_replace_op_with_index_assign( ferite_assign_op_to_change[ferite_assign_loc], SUBOP ); \
	else \
		ferite_do_binary_op( ACTUALOP ); \
	ferite_assign_loc--; } while (0)

	 /***
	* Native Code Support
	***/
	 extern char ferite_current_native_block_file[4096];
	 extern int  ferite_current_native_block_line;

%}

%union {
	char   *text;
	int	    inumber;
	double  fnumber;
}

%left	 ','
%left	 T_ASSIGN T_PLUS_ASSIGN T_MINUS_ASSIGN T_MULT_ASSIGN T_DIVIDE_ASSIGN T_LEFT_SHIFT_ASSIGN T_RIGHT_SHIFT_ASSIGN T_BINARY_AND_ASSIGN T_BINARY_OR_ASSIGN T_BINARY_XOR_ASSIGN
%left	 T_LOGICAL_OR
%left	 T_LOGICAL_AND T_RARROW
%left	 T_BINARY_OR
%left	 T_BINARY_AND
%left	 T_BINARY_XOR
%left	 T_EQUALS T_LESSTHAN_EQUALS T_GRTRTHAN_EQUALS T_LESSTHAN T_GRTRTHAN T_NOTEQUAL
%left	 T_ISA T_INSTANCEOF
%left	 T_LEFT_SHIFT T_RIGHT_SHIFT
%left	 T_PLUS T_MINUS
%left	 T_MULT T_DIVIDE T_MODULUS
%right	 T_INCR T_DECR T_NOT_OPERATOR
%nonassoc T_UMINUS T_UPLUS
%token	 T_OBJOP
%nonassoc T_NEW
%token	 T_IF
%left	 T_ELSE
%left	 T_EVAL T_INCLUDE

%token T_FUNCTION T_FOR T_WHILE T_DO T_SWITCH T_MONITOR T_HANDLE
%token T_CASE T_DEFAULT T_USES T_GLOBAL T_RETURN T_FINAL T_NAMESPACE
%token T_CLASS T_EXTENDS T_BREAK T_CONTINUE T_STATIC T_MULTIPLE_ARGS
%token T_MODIFIES T_NATIVE
%token T_MODULE_INIT T_MODULE_DEINIT T_MODULE_HEADER T_MODULE_FLAGS
%token T_SLICE T_RARROW T_ATOMIC T_MODULE_REGISTER T_MODULE_UNREGISTER
%token T_FOREACH T_ARGS T_PRIVATE T_PROTECTED T_PUBLIC T_ABSTRACT
%token T_CLOSURE T_USING T_DELIVER T_RECIPIENT T_PROTOCOL T_IMPLEMENTS
%token T_SELF T_SUPER T_CONSTRUCTOR T_DESTRUCTOR
%token T_VAR_NUMBER T_VAR_STRING T_VAR_VOID T_VAR_OBJECT T_VAR_ARRAY T_VAR_BOOLEAN T_VAR_NUMBER_LNG T_VAR_NUMBER_DBL
%token T_DIRECTIVE T_RAISE T_CURRENT_LINE T_CURRENT_FILE T_EXPANSION T_FUNCTION_JUMP
%token T_RET_UNDEF

%token <text>	 T_QSTRING T_LABEL T_OBJPROPERTY T_NATIVE_CODE_BLOCK T_DOT_LABEL T_DIRECTIVE_LABEL
%token <inumber> T_NNUMBER T_TRUE T_FALSE
%token <fnumber> T_RNUMBER

%pure-parser
%expect 9

%%

script:
	script_block_list				{
										ferite_do_function_header( ferite_compiler_entry_function(ferite_scanner_file), FE_FALSE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE, FE_FALSE );
										if( ferite_do_function_start() ) {
											yyclearin;
										}
										ferite_var_array_count = 0;
									}
	script_body						{	ferite_do_script_finish(); }
	;
	
script_body:
	T_FUNCTION_JUMP					{	ferite_do_function_footer(); }
|	variable_block
	statement_block					{
										ferite_temp_variable = ferite_create_void_variable( NULL, "forced-function-return", FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_function_footer();
									}
	;

script_block_list:
	/* empty */
|	non_empty_script_block_list
	;

non_empty_script_block_list:
	script_block
|	non_empty_script_block_list script_block
	;

script_block:
	uses_list
|	global_decl
|	protocol_def
|	class_def
|	namespace_def
|	builder_item
|	block_item_modifier_list function
	;

builder_item:
	T_MODULE_INIT					 { ferite_scanner_go_native(); }
	T_NATIVE_CODE_BLOCK			   { ferite_do_create_builder_variable( "'module-init", $3 ); ffree_ngc( $3 ); }
|	T_MODULE_DEINIT				   { ferite_scanner_go_native(); }
	T_NATIVE_CODE_BLOCK			   { ferite_do_create_builder_variable( "'module-deinit", $3 ); ffree_ngc( $3 ); }
|	T_MODULE_REGISTER				 { ferite_scanner_go_native(); }
	T_NATIVE_CODE_BLOCK			   { ferite_do_create_builder_variable( "'module-register", $3 ); ffree_ngc( $3 ); }
|	T_MODULE_UNREGISTER			   { ferite_scanner_go_native(); }
	T_NATIVE_CODE_BLOCK			   { ferite_do_create_builder_variable( "'module-unregister", $3 ); ffree_ngc( $3 ); }
|	T_MODULE_HEADER				   { ferite_scanner_go_native(); }
	T_NATIVE_CODE_BLOCK			   { ferite_do_create_builder_variable( "'module-header", $3 ); ffree_ngc( $3 ); }
|	T_MODULE_FLAGS					{ ferite_scanner_go_native(); }
	T_NATIVE_CODE_BLOCK			   { ferite_do_create_builder_variable( "'module-flags", $3 ); ffree_ngc( $3 ); }
	;

uses_list:
	T_USES module_list ';'
	;

module_item:
	T_QSTRING						 { ferite_do_uses( $1 ); ffree_ngc($1); FECT; }
	;

module_list:
	module_item
|	module_list ',' module_item
	;

directive_list:
	 /* empty */
|	non_empty_directive_list
	 ;

directive_item:
	 T_QSTRING			{ FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
												 										 "", $1, 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item );
							  ffree_ngc( $1 ); FECT; }
|	T_LABEL			  { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																					  "", $1, 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item );
						  ffree_ngc( $1 ); FECT; }
|	T_RNUMBER			{ FeriteVariable *item = ferite_create_number_double_variable( CURRENT_SCRIPT, "", $1, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_NNUMBER			{ FeriteVariable *item = ferite_create_number_double_variable( CURRENT_SCRIPT, "", $1, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_VAR_NUMBER		 { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "number", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_VAR_BOOLEAN		{ FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "boolean", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_VAR_STRING		 { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "string", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_VAR_VOID		   { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "void", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_VAR_OBJECT		 { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "object", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_VAR_ARRAY		  { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "array", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_CLASS			  { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "class", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_NAMESPACE		  { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																					 "", "namespace", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_TRUE				{ FeriteVariable *item = ferite_create_boolean_variable( CURRENT_SCRIPT, "", FE_TRUE, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
|	T_FALSE				{ FeriteVariable *item = ferite_create_boolean_variable( CURRENT_SCRIPT, "", FE_FALSE, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); }
	 ;

non_empty_directive_list:
	 directive_item
|	non_empty_directive_list ',' directive_item
|	non_empty_directive_list	 directive_item
	 ;

directive_decl:
	directive_decl_with_out_semi_colon ';'
	;
	
directive_decl_with_out_semi_colon:
	'[' T_LABEL				{ 
								ferite_directive_parameters = ferite_create_stack( CURRENT_SCRIPT, 10 ); 
								ferite_scanner_start_directive_list();
							}
	directive_list ']'      {
								ferite_scanner_stop_directive_list();
								if( ferite_expansion_hash && ferite_hash_get( FE_NoScript, ferite_expansion_hash, $2 ) ) {
									char *body = NULL;
									FeriteIterator *iter = ferite_create_iterator( FE_NoScript );
									FeriteHashBucket *bucket = NULL;

									ferite_expansion_map_hash = ferite_hash_get( FE_NoScript, ferite_expansion_hash, $2 );
									printf("Got expansion call: %s\n", $2);
									body = ferite_hash_get( FE_NoScript, ferite_expansion_map_hash, "!_BODY_");
									printf( "With body: %s\n", body );

									while( (bucket = ferite_hash_walk(FE_NoScript, ferite_expansion_map_hash, iter)) != NULL ) {
										long offset = (long)(bucket->data);
										if( strcmp( bucket->id, "!_BODY_" ) ) { /* No body */
											FeriteString *var = ferite_variable_to_str( FE_NoScript, ferite_directive_parameters->stack[offset+1], FE_FALSE );
											printf( "bucket: %s -> %ld\n", bucket->id, offset );
											printf( "replacement: %s\n", var->data );
											body = ferite_replace_string( body, bucket->id, var->data );
										}
									}
									printf( "new body: %s\n", body );

									/* Now we want to parse this code in the current context */
									ferite_scanner_parse_macro(body);
								} else {
									ferite_do_add_directive( $2, ferite_directive_parameters );
									ferite_directive_parameters = NULL;
									ffree_ngc($2); FECT;
								}
							 }
	 ;

expansion_decl:
	T_EXPANSION directive_expansion_capture_mask { ferite_scanner_go_native(); }
	T_NATIVE_CODE_BLOCK  { printf("Code block: '%s'\n", $4);
		ferite_hash_add( FE_NoScript, ferite_expansion_map_hash, "!_BODY_", $4 );
	}
	;

directive_expansion_capture_mask_arguments:
	/* nothing */
|	non_empty_directive_mask_list
	;

non_empty_directive_mask_list_item:
	T_LABEL { printf("Label: %s [%ld]\n", $1, ferite_expansion_map_offset ); ferite_expansion_map_offset++; }
|	T_DIRECTIVE_LABEL {
		printf("DirectiveLabel: %s [%ld]\n", $1, ferite_expansion_map_offset );
		ferite_hash_add( FE_NoScript, ferite_expansion_map_hash, $1, (void*)ferite_expansion_map_offset );
		ferite_expansion_map_offset++;
	}
	;

non_empty_directive_mask_list:
	non_empty_directive_mask_list_item
|	non_empty_directive_mask_list non_empty_directive_mask_list_item
	;

directive_expansion_capture_mask:
	 '['
	 		T_LABEL {
			printf("expansion name: %s\n", $2);
			if( ferite_expansion_hash == NULL ) {
				ferite_expansion_hash = ferite_create_hash( FE_NoScript, 10 );
			}
			ferite_expansion_map_offset = 0;
			ferite_expansion_map_hash = ferite_create_hash( FE_NoScript, 10 );
			ferite_hash_add( FE_NoScript, ferite_expansion_hash, $2, ferite_expansion_map_hash );
		}
		directive_expansion_capture_mask_arguments
	 ']'
	 ;

global_decl:
	T_GLOBAL						{ FUD(( "going into global block\n" )); ferite_var_is_global = FE_TRUE; }
	'{' namespace_variable_block '}'
									{ ferite_var_is_global = FE_FALSE; FUD(( "leaving global memory block\n" )); }
	;

var_type:
	T_VAR_NUMBER     { strcpy( ferite_hint_typestring, "L" ); ferite_last_type = F_VAR_NUM; }
|	T_VAR_NUMBER_LNG { strcpy( ferite_hint_typestring, "L" ); ferite_last_type = F_VAR_LONG; }
|	T_VAR_NUMBER_DBL { strcpy( ferite_hint_typestring, "D" ); ferite_last_type = F_VAR_DOUBLE; }
|	T_VAR_STRING     { strcpy( ferite_hint_typestring, "S" ); ferite_last_type = F_VAR_STR; }
|	T_VAR_VOID	     { strcpy( ferite_hint_typestring, "V" ); ferite_last_type = F_VAR_VOID; }
|	T_VAR_OBJECT     { strcpy( ferite_hint_typestring, "O" ); ferite_last_type = F_VAR_OBJ; }
|	T_VAR_ARRAY      { strcpy( ferite_hint_typestring, "A" ); ferite_last_type = F_VAR_UARRAY; }
|	T_VAR_BOOLEAN    { strcpy( ferite_hint_typestring, "B" ); ferite_last_type = F_VAR_BOOL; }
	;

param_var_type:
	var_type
|	T_CLASS	      { strcpy( ferite_hint_typestring, "C" ); ferite_last_type = F_VAR_CLASS; }
|	T_NAMESPACE   { strcpy( ferite_hint_typestring, "N" ); ferite_last_type = F_VAR_NS; }
	;

optional_function_token:
	/* empty */
|	T_FUNCTION
	;

function_name:
	T_FUNCTION T_LABEL			   { ferite_last_function = $2; }
|	T_DIRECTIVE T_LABEL			  { ferite_last_function = $2; ferite_function_is_directive = FE_TRUE; }
|	optional_function_token
	T_CONSTRUCTOR					{ ferite_last_function = fstrdup( "constructor" ); }
|	optional_function_token
	T_DESTRUCTOR					 { ferite_last_function = fstrdup( "destructor" ); }
	;

function_header_prefix:
	function_name		 {
										if( ferite_var_is_final )
										  ferite_warning( CURRENT_SCRIPT, 
												"Keyword 'final' can not be applied to functions ('%s' on line %d in %s)\n", 
												ferite_last_function, 
												ferite_scanner_lineno, 
												ferite_scanner_file );
										ferite_do_function_header( 
											ferite_last_function, 
											ferite_var_is_static, 
											ferite_var_is_native, 
											ferite_var_is_atomic, 
											ferite_item_state, 
											ferite_var_is_final, 
											ferite_function_is_directive 
										);
										ffree_ngc( ferite_last_function ); FECT;
										ferite_var_is_params = FE_TRUE;
										ferite_var_is_static = FE_FALSE;
										ferite_var_is_atomic = FE_FALSE;
										ferite_var_array_count = FE_FALSE;
										ferite_var_is_final = FE_FALSE;
										ferite_item_state = FE_ITEM_IS_PRIVATE;
										ferite_function_is_directive = FE_FALSE;
									}
	 ;

function_return_type:
	param_var_type					{
										ferite_return_type_was_provided = FE_TRUE;
										ferite_scanner_stop_typing_label();
										ferite_seeking_return_type_fix = FE_TRUE;
										ferite_hint_depth = 0;
									}
	optional_variable_type_hint		{ ferite_seeking_return_type_fix = FE_FALSE; }
|	T_DOT_LABEL						{
										ferite_scanner_stop_typing_label();
										ferite_return_type_was_provided = FE_TRUE;
										strcpy(ferite_hint_typestring, "O:");
										strcat(ferite_hint_typestring, $1);
										strcat(ferite_hint_typestring, ";");
										ffree_ngc($1); FECT;
									}
|	T_RET_UNDEF						{ 
										strcpy(ferite_hint_typestring, "U");
										ferite_scanner_stop_typing_label();
										ferite_return_type_was_provided = FE_TRUE;
									}
	;

function_optional_return_type:
	':' 							{ ferite_scanner_want_typing_label(); }
	function_return_type			{ 
										ferite_do_function_return_type(ferite_hint_typestring);
									}
|	/* empty */						{ 
										ferite_return_type_was_provided = FE_FALSE;
										ferite_do_function_return_type("V");
										
										if( ferite_var_is_native == FE_TRUE ) {
											yyclearin; 
											ferite_lexer_fix_native_blocks(); 
										}
										
										if( ferite_var_is_native == FE_TRUE ) {
											if( !(strcmp(ferite_current_compile->function->name,"constructor") == 0 ||
													strcmp(ferite_current_compile->function->name,"destructor") == 0 ||
													ferite_function_is_directive == FE_TRUE )) {		
												void *path = ferite_compiler_build_current_path_wannotation_wfunction(FE_FALSE, FE_TRUE);
												if( ferite_is_strict ) {
													ferite_error( CURRENT_SCRIPT, 0, "You must specify a return type for native function: %s\n", path );
													ffree_ngc(path);
													feperror( NULL );
												} else {
													if( ferite_show_partial_implementation ) {
														ferite_warning( CURRENT_SCRIPT,  "You should specify a return type for native function: %s\n", path );
													}
													ffree_ngc(path);
												}
											}
										}
									}
	;

function_parameter_declaration:
	 '(' decl_parameter_list ')'	{
										FUD(("PARSER: END FUNCTION PARAMETER DECLARATION\n"));
										FUD(("PARSER: BEGIN FUNCTION BODY\n"));
									}
|	/* empty */
	 ;

function_body:
	'{'
		variable_block
		statement_block 
	'}'								{
										FUD(("PARSER: END FUNCTION BODY\n"));
										ferite_temp_variable = ferite_create_void_variable( NULL, "forced-function-return", FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(voidptr)		\n"));
										ferite_do_function_footer();
									}
|	T_NATIVE_CODE_BLOCK				{
										ferite_do_function_native_block( $1,
												ferite_current_native_block_file,
												ferite_current_native_block_line );
									}
|	statement						{

										FUD(("PARSER: END FUNCTION BODY\n"));
										ferite_temp_variable = ferite_create_void_variable( NULL, "forced-function-return", FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(voidptr)		\n"));
										ferite_do_function_footer();
									}
|	T_FUNCTION_JUMP					{	ferite_do_function_footer(); }
	 ;

function:
	function_header_prefix
	function_parameter_declaration  
	function_optional_return_type
									{
										ferite_var_is_params = FE_FALSE;
										if( ferite_do_function_start() ) {
											yyclearin;
										}
										if( ferite_var_is_native == FE_TRUE ) {
											ferite_scanner_go_native();
											ferite_var_is_native = FE_FALSE;
										}
									}
	 function_body
	 ;

parameter_byref:
	 /* No by-ref char */
|	T_BINARY_AND					{ ferite_var_pass_type = FE_BY_REF; }
	 ;

parameter_define_item:
	param_var_type					{ ferite_hint_depth = 0; }
	optional_variable_type_hint
	parameter_byref 
	T_LABEL							{
										FUD(("PARSER: Parameter Declared		 %s (%d)\n", $5, ferite_last_type));
										ferite_do_add_variable_to_paramlist( $5, ferite_last_type, ferite_hint_current_type, ferite_var_pass_type );
										ffree_ngc( $5 ); FECT;
										ferite_var_pass_type = FE_BY_VALUE;
									}
	 ;

parameter_multiple_args:
	 T_MULTIPLE_ARGS				  {
										FUD(("PARSER: Parameter Declared - Multiple Args\n"));
										ferite_do_add_variable_to_paramlist( "...", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_VALUE );
									}
	 ;

decl_parameter_list:
	 /* none */
|	non_empty_decl_parameter_list
|	non_empty_decl_parameter_list ',' parameter_multiple_args
|	parameter_multiple_args
	 ;

non_empty_decl_parameter_list:
	parameter_define_item
|	non_empty_decl_parameter_list ',' parameter_define_item
	;

namespace_header:
	T_NAMESPACE T_LABEL			   { ferite_do_namespace_header( $2 ); ffree_ngc( $2 ); FECT; }
|	T_NAMESPACE T_MODIFIES T_LABEL	{ ferite_do_namespace_extends( $3 ); ffree_ngc( $3 );  FECT; }
	;

namespace_block_list:
	/* empty */
|	non_empty_namespace_block_list
	;

non_empty_namespace_block_list:
	namespace_block
|	non_empty_namespace_block_list namespace_block
	;

namespace_block:
	block_item_modifier_list namespace_multiple_variable_item ';'
|	block_item_modifier_list function
|	protocol_def
|	class_def
|	namespace_def
|	directive_decl
	;

namespace_def:
	namespace_header
	'{'  namespace_block_list '}'	 { ferite_do_namespace_footer(); }
	;

class_block_list:
	/* empty */
|	non_empty_class_block_list
	;

non_empty_class_block_list:
	class_block
|	non_empty_class_block_list class_block
	;

class_block:
	{ ferite_item_state = -1; }
	class_block_real
	;

class_block_real:
	block_item_modifier_list namespace_multiple_variable_item ';'
|	block_item_modifier_list function
|	directive_decl
|	expansion_decl
	;

class_header:
	T_LABEL							{
										FUD(("CLASS START\n" ));
										ferite_do_class_header( $1, NULL, ferite_class_state );
										ferite_class_state = 0;
										ffree_ngc( $1 ); FECT;
									}
|	T_LABEL T_EXTENDS				{
										ferite_scanner_want_dot_label();
									}
	T_DOT_LABEL						{
										FUD(("CLASS START\n" ));
										ferite_scanner_stop_dot_label();
										ferite_do_class_header( $1, $4, ferite_class_state );
										ferite_class_state = 0;
										ffree_ngc( $1 ); FECT;
										ffree_ngc( $4 ); FECT;
									}
	;

class_modifies_header:
	T_MODIFIES						{
										ferite_scanner_want_dot_label();
									}
	T_DOT_LABEL					   {
										FUD(("CLASS START\n" ));
										ferite_scanner_stop_dot_label();
										ferite_do_class_header( NULL, $3, ferite_class_state );
										ferite_class_state = 0;
										ffree_ngc( $3 ); FECT;
									}
	;

implements_item:
									{
										ferite_scanner_want_dot_label();
									}
	T_DOT_LABEL					 {
										ferite_scanner_stop_dot_label();
										ferite_do_class_implements( $2 );
										ffree_ngc( $2 ); FECT;
									}
	;

implements_list:
	implements_item
|	implements_list ',' implements_item
	;

class_implements:
	/* empty - no implements */
|	T_IMPLEMENTS implements_list
	;

standard_class:
	 class_header
	 class_implements
	 '{' class_block_list '}'		 {
										FUD(("CLASS END\n" ));
										ferite_do_class_footer();
									}
	 ;

modifies_class:
	 class_modifies_header
	 '{' class_block_list '}'		 {
										ferite_do_class_footer();
									}
	 ;

class_def:
	class_modifier_list
	T_CLASS
	class_body
	;

class_body:
	standard_class
|	modifies_class
	;

protocol_header:
	T_PROTOCOL T_LABEL {
							ferite_do_class_header( $2, NULL, FE_ITEM_IS_PROTOCOL );
							ferite_class_state = 0;
							ffree_ngc( $2 ); FECT;
						}
	;

protocol_item:
	block_item_modifier_list function
	;

non_empty_protocol_block_list:
	protocol_item
|	non_empty_protocol_block_list protocol_item
	;

protocol_block_list_real:
	/* empty */
|	non_empty_protocol_block_list
	;

protocol_block_list:
	{ ferite_class_state = -1; }
	protocol_block_list_real;

protocol_def:
	protocol_header
	'{' protocol_block_list '}'	 {
										ferite_do_class_footer();
									}
	;


variable_define_item:
	 T_LABEL						  {
										FUD(("PARSER: Variable Declared		 %s(%d)\n", $1, ferite_last_type));
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", $1, ferite_scanner_lineno, ferite_scanner_file );
										ferite_do_add_variable( $1, ferite_last_type, ferite_hint_current_type, ferite_var_is_global, ferite_var_is_final, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );

										ferite_do_variable_push( $1 );
										ferite_do_variable_reset( ferite_last_type );

										ffree_ngc( $1 ); FECT;
									}
|	T_LABEL T_ASSIGN				 {
										FUD(("PARSER: Variable Declared		 %s(%d)\n", $1, ferite_last_type));
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", $1, ferite_scanner_lineno, ferite_scanner_file );
										ferite_do_add_variable( $1, ferite_last_type, ferite_hint_current_type, FE_FALSE, ferite_var_is_final, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );

										ferite_do_variable_push( $1 );
										ferite_do_variable_reset( ferite_last_type );

										ferite_do_variable_push( $1 );
										ffree_ngc( $1 ); FECT;
									}
	 expression					   {
										ferite_do_binary_op( FERITE_OPCODE_assign );
										FUD(("PARSER: op->assign	   pop,pop\n"));
										ferite_do_pop();
									}
	 ;

namespace_natural_number:
	T_MINUS T_NNUMBER				{   ferite_namespace_naturaln = 0 - $2; }
|	T_PLUS  T_NNUMBER				{   ferite_namespace_naturaln = $2; }
|	T_NNUMBER						{   ferite_namespace_naturaln = $1; }
	 ;

namespace_variable_define_item:
	 T_LABEL						  {
										FUD(("PARSER: Variable Declared		 %s(%d)\n", $1, ferite_last_type));
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", $1, ferite_scanner_lineno, ferite_scanner_file );
										ferite_do_add_variable( $1, ferite_last_type, ferite_hint_current_type, ferite_var_is_global, ferite_var_is_final, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
										ffree_ngc( $1 ); FECT;
									}
|	T_LABEL T_ASSIGN namespace_natural_number   {
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", $1, ferite_scanner_lineno, ferite_scanner_file );
										if( ferite_last_type == F_VAR_NUM )
										{
											FUD(("PARSER: Variable Declared		 %s(%d)=%d\n", $1, ferite_last_type, ferite_namespace_naturaln ));
											ferite_temp_variable = ferite_create_number_long_variable( NULL, $1, ferite_namespace_naturaln, FE_ALLOC );
											if( ferite_var_is_final )
											  MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
											ferite_do_add_variable_with_value( $1, ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
											ffree_ngc( $1 ); FECT;
										}
										else
										{
											ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the number %d.\n", $1, ferite_variable_id_to_str( NULL,ferite_last_type), ferite_namespace_naturaln );
											ffree_ngc( $1 ); FECT;
											feperror( NULL );
										}
									}
|	T_LABEL T_ASSIGN T_RNUMBER	   {
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", $1, ferite_scanner_lineno, ferite_scanner_file );
										if( ferite_last_type == F_VAR_NUM )
										{
											FUD(("PARSER: Variable Declared		 %s(%d)=%f\n", $1, ferite_last_type, $3));
											ferite_temp_variable = ferite_create_number_double_variable( NULL, $1, $3, FE_ALLOC );
											if( ferite_var_is_final )
											  MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
											ferite_do_add_variable_with_value( $1, ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
											ffree_ngc( $1 ); FECT;
										}
										else
										{
											ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the number %f.\n", $1, ferite_variable_id_to_str( NULL,ferite_last_type), $3 );
											ffree_ngc( $1 ); FECT;
											feperror( NULL );
										}
									}
|	T_LABEL T_ASSIGN T_QSTRING	   {
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", $1, ferite_scanner_lineno, ferite_scanner_file );
										if( ferite_last_type == F_VAR_STR )
										{
											FUD(("PARSER: Variable Declared		 %s(%d)='%s'\n", $1, ferite_last_type, $3));
											ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, $1, $3, strlen($3), FE_CHARSET_DEFAULT, FE_ALLOC );
											if( ferite_var_is_final )
											  MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
											ferite_do_add_variable_with_value( $1, ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
											ffree_ngc( $1 ); ffree_ngc( $3 ); FECT;
										}
										else
										{
											ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the string '%s'.\n", $1, ferite_variable_id_to_str( NULL,ferite_last_type), $3 );
											ffree_ngc( $1 );
											ffree_ngc( $3 );
											FECT;
											feperror( NULL );
										}
									}
|	T_LABEL T_ASSIGN T_TRUE		  {
									   if( ferite_var_is_native )
										 ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", $1, ferite_scanner_lineno, ferite_scanner_file );
									   if( ferite_last_type == F_VAR_BOOL )
									   {
										   FUD(("PARSER: Variable Declared		 %s(%d)=%d\n", $1, ferite_last_type, $3));
										   ferite_temp_variable = ferite_create_boolean_variable( NULL, $1, FE_TRUE, FE_ALLOC );
										   if( ferite_var_is_final )
											 MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
										   ferite_do_add_variable_with_value( $1, ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
										   ffree_ngc( $1 ); FECT;
									   }
									   else
									   {
										   ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the number %f.\n", $1, ferite_variable_id_to_str( NULL,ferite_last_type), $3 );
										   ffree_ngc( $1 ); FECT;
										   feperror( NULL );
									   }
								   }
|	T_LABEL T_ASSIGN T_FALSE		{
									   if( ferite_var_is_native )
										 ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", $1, ferite_scanner_lineno, ferite_scanner_file );
									   if( ferite_last_type == F_VAR_BOOL )
									   {
										   FUD(("PARSER: Variable Declared		 %s(%d)=%d\n", $1, ferite_last_type, $3));
										   ferite_temp_variable = ferite_create_boolean_variable( NULL, $1, FE_FALSE, FE_ALLOC );
										   if( ferite_var_is_final )
											 MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
										   ferite_do_add_variable_with_value( $1, ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
										   ffree_ngc( $1 ); FECT;
									   }
									   else
									   {
										   ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the number %f.\n", $1, ferite_variable_id_to_str( NULL,ferite_last_type), $3 );
										   ffree_ngc( $1 ); FECT;
										   feperror( NULL );
									   }
								   }

;

variable_define_list:
	 variable_define_item
|	variable_define_list ',' variable_define_item
	 ;

namespace_variable_define_list:
	 namespace_variable_define_item
|	namespace_variable_define_list ',' namespace_variable_define_item
	 ;

class_modifier:
	T_FINAL						  {
									  if( ferite_class_state > 0 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "The class already has state - you can't make it final\n" );
										  feperror( NULL );
									  }
									  else
										ferite_class_state = FE_ITEM_IS_FINAL;
									}
|	T_ABSTRACT					   {
									  if( ferite_class_state > 0 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "The class already has state - you can't make it abstract\n" );
										  feperror( NULL );
									  }
									  else
										ferite_class_state = FE_ITEM_IS_ABSTRACT;
									}
	 ;

class_modifier_list:
	 /* Empty */
|	class_modifier
	 ;

item_modifier:
	 T_FINAL						  {
									  if( ferite_var_is_final )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already final - you can't make it final again\n" );
										  feperror( NULL );
									  }
									  else
										ferite_var_is_final = FE_TRUE;
									}
|	T_STATIC						 {
									  if( ferite_var_is_static )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already static - you can't make it static again\n" );
										  feperror( NULL );
									  }
									  else
										ferite_var_is_static = FE_TRUE;
									}
|	T_ATOMIC						 {
									  if( ferite_var_is_atomic )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already atomic - you can't make it atomic again\n" );
										  feperror( NULL );
									  }
									  else
										ferite_var_is_atomic = FE_TRUE;
									}
|	T_NATIVE						 {
									  if( ferite_var_is_native )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already native - you can't make it native again\n" );
										  feperror( NULL );
									  }
									  else
										ferite_var_is_native = FE_TRUE;
									}
|	T_PRIVATE						{
									  if( ferite_item_state > -1 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already %s - you can't make it private\n", ferite_state_to_str( ferite_item_state ) );
										  feperror( NULL );
									  }
									  else
										ferite_item_state = FE_ITEM_IS_PRIVATE;
									}
|	T_PROTECTED					  {
									  if( ferite_item_state > -1 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already %s - you can't make it protected\n", ferite_state_to_str( ferite_item_state ) );
										  feperror( NULL );
									  }
									  else
										ferite_item_state = FE_ITEM_IS_PROTECTED;
									}
|	T_PUBLIC						 {
									  if( ferite_item_state > -1 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already %s - you can't make it public\n", ferite_state_to_str( ferite_item_state ) );
										  feperror( NULL );
									  }
									  else
										ferite_item_state = FE_ITEM_IS_PUBLIC;
									}
	 ;

block_item_modifier_list:
	 /* nowt */
|	non_empty_block_item_modifier_list
	 ;

non_empty_block_item_modifier_list:
	 item_modifier
|	non_empty_block_item_modifier_list item_modifier
	 ;

variable_block:
	 non_empty_variable_block
|	/* empty */
	 ;

non_empty_variable_block:
	 multiple_variable_item ';'
|	non_empty_variable_block multiple_variable_item ';'
	 ;

final_modifier:
	 T_FINAL						{
									  if( ferite_var_is_final )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already final - you can't make it final again\n" );
										  feperror( NULL );
									  }
									  else
										  ferite_var_is_final = FE_TRUE;
									}
|	/* nothing */
	 ;

optional_variable_type_hint_t:
	T_DOT_LABEL      {
						if( ferite_hint_typestring[strlen(ferite_hint_typestring) - 1] == 'A' ) {
							strcat( ferite_hint_typestring, "O" );
						}
						strcat( ferite_hint_typestring, ":" );
						strcat( ferite_hint_typestring, $1 );
						strcat( ferite_hint_typestring, ";" );
						ffree_ngc($1); 
						FECT; 
					 }
|	T_QSTRING        { ferite_error(CURRENT_SCRIPT, 0, "Quoted string hint type not supported\n" ); ffree_ngc($1); FECT; feperror( NULL ); }
|	T_RNUMBER        { ferite_error(CURRENT_SCRIPT, 0, "Floating point hint type not supported\n" ); feperror( NULL ); }
|	T_NNUMBER        { ferite_error(CURRENT_SCRIPT, 0, "Natural nunber hint type not supported\n" ); feperror( NULL ); }
|	T_VAR_NUMBER     { strcat( ferite_hint_typestring, "L" ); }
|	T_VAR_NUMBER_LNG { strcat( ferite_hint_typestring, "L" ); }
|	T_VAR_NUMBER_DBL { strcat( ferite_hint_typestring, "D" ); }
|	T_VAR_STRING     { strcat( ferite_hint_typestring, "S" ); }
|	T_VAR_VOID	     { strcat( ferite_hint_typestring, "V" ); }
|	T_VAR_OBJECT     { strcat( ferite_hint_typestring, "O" ); }
|	T_VAR_ARRAY      { strcat( ferite_hint_typestring, "A" ); }
|	T_VAR_BOOLEAN    { strcat( ferite_hint_typestring, "B" ); }
|	T_CLASS          { strcat( ferite_hint_typestring, "C" ); }
|	T_NAMESPACE      { strcat( ferite_hint_typestring, "N" ); }
	;

optional_variable_type_hint:
	/* empty */						{
										if( ferite_seeking_return_type_fix && ferite_hint_depth == 0 ) {
											yyclearin; 
											ferite_lexer_fix_native_blocks(); 
										}
									}
|	T_LESSTHAN						{
										ferite_hint_depth++;
										ferite_scanner_want_end_hint(); 
										ferite_scanner_want_typing_label();
									}
	optional_variable_type_hint_t	{ ferite_scanner_stop_typing_label(); }
	optional_variable_type_hint
	T_GRTRTHAN						{ 
										ferite_scanner_stop_end_hint();
										ferite_hint_depth--;
									}
	;

multiple_variable_item:
	final_modifier var_type			{
										FUD(("PARSER: Setting current type to %d\n", ferite_last_type ));
										ferite_hint_depth = 0;
									}
	optional_variable_type_hint		{
										ferite_hint_current_type = ferite_subtype_link( CURRENT_SCRIPT, ferite_hint_typestring );
										if( ferite_hint_current_type == NULL && CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
											feperror(NULL);
										}
									}
	variable_define_list			{ 
										ferite_var_is_final = FE_FALSE; 
										ferite_var_is_static = FE_FALSE; 
										ferite_var_is_atomic = FE_FALSE; 
										ferite_var_is_native = FE_FALSE; 
									}
	;

namespace_multiple_variable_item:
	var_type						{
										FUD(("PARSER: Setting current type to %d\n", ferite_last_type ));
										ferite_hint_depth = 0;
									}
	optional_variable_type_hint	   	{
										ferite_hint_current_type = ferite_subtype_link( CURRENT_SCRIPT, ferite_hint_typestring );
										if( ferite_hint_current_type == NULL && CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
											feperror(NULL);
										}
									}
	namespace_variable_define_list  { ferite_var_is_final = FE_FALSE; ferite_var_is_static = FE_FALSE; ferite_var_is_atomic = FE_FALSE; ferite_var_is_native = FE_FALSE; }
	;

namespace_variable_block:
	/* empty */
|	non_empty_namespace_variable_block
	;

non_empty_namespace_variable_block:
	block_item_modifier_list namespace_multiple_variable_item ';'
|	non_empty_namespace_variable_block
	block_item_modifier_list namespace_multiple_variable_item ';'
	;

nested_block:
	'{'			  { ferite_do_begin_block(); }
		variable_block
		statement_block
	'}'			  { ferite_do_end_block(); }
	;
non_empty_nested_block:
	'{'			  { ferite_do_begin_block(); }
		variable_block
		error_empty_statement_block
	'}'			  { ferite_do_end_block(); }
	;

block:
	statement
|	nested_block
	;

case_block:
	statement_block
	;

statement_block:
	/* empty */
|	non_empty_statement_block
	;
error_empty_statement_block:
	/* empty */ 			{
								ferite_error( CURRENT_SCRIPT, 0, "You are not allowed to have empty handle blocks.\n" );
	  							feperror( NULL );
							}
|	non_empty_error_statement_block
	;
non_empty_error_statement_block:
	non_empty_error_statement_block_item
|	non_empty_error_statement_block non_empty_error_statement_block_item
	;
non_empty_error_statement_block_item:
	statement_without_semicolon
|	nested_block
	;
	
non_empty_statement_block_item:
	 statement
|	nested_block
	 ;

non_empty_statement_block:
	 non_empty_statement_block_item
|	non_empty_statement_block non_empty_statement_block_item
	 ;

statement_without_semicolon:
	expression ';'				   { ferite_do_pop(); /* clear up the stupid stack ie wham = 2 + 3 leaves a variable on the stack*/ }
|	select_statement
|	itterative_statement
|	return_statement ';'
|	T_BREAK ';'					  { ferite_do_break(); }
|	T_CONTINUE ';'				   { ferite_do_continue(); }
|	T_RAISE expression ';'		   { ferite_do_raise(); }
	;
	
statement:
	statement_without_semicolon
|	';'
	 ;

return_statement:
	 T_RETURN						 {
										ferite_temp_variable = ferite_create_void_variable( NULL, "compiled-in-return-value", FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(voidptr)		\n"));
										ferite_do_exit();
									}
|	T_RETURN expression			   {  ferite_do_exit(); }
	;

optional_handle_variable:
	/* empty */
|	'(' T_LABEL ')' {
						ferite_do_add_variable( $2, F_VAR_OBJ, ferite_subtype_link(CURRENT_SCRIPT, "O"), FE_FALSE, FE_TRUE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );
						ferite_do_variable_push( $2 );
						ferite_do_variable_push( "err" );
						ferite_do_binary_op( FERITE_OPCODE_assign );
					}
	;
	
monitor_block:
	T_MONITOR                         { ferite_do_monitor_block(); }
	nested_block				      { ferite_do_before_handle_block(); }
	T_HANDLE						  { ferite_do_begin_block(); }
	optional_handle_variable
	non_empty_nested_block            { ferite_do_end_block() ; }
	fix_else_block
	;

select_statement:
	T_IF '(' expression ')'		  { ferite_do_if_statement(); }
	block else_block
|	monitor_block
|	switch_statement
	 ;

else_block:
	 /* empty */					  { ferite_do_after_then_statement(); }
|	T_ELSE						   { ferite_do_after_then_before_else_statement(); }
	 block {  ferite_do_after_else_statement(); }
	 ;

switch_statement:
	 T_SWITCH						 {
										ferite_do_pre_switch();
									}
	 '(' expression ')'
	 '{'
	   switch_case_list
	 '}'							  {
										ferite_do_post_switch();
									}
	 ;

switch_case_list:
	 switch_cases_list
	 default_item
	 ;

switch_cases_list:
	 /* empty */
|	non_empty_switch_cases_list
	 ;

non_empty_switch_cases_list:
	 switch_case_item
|	non_empty_switch_cases_list switch_case_item
	 ;


switch_case_item:
	 T_CASE
	 expression					   {
										ferite_do_case_block_start();
									}
	 ':' case_block				   {
										ferite_do_case_block_end();
									}
	 ;

default_item:
	 /* nuttin */
|	T_DEFAULT ':'					{
										ferite_do_default_block_start();
									}
	 statement_block
	 ;

fix_else_block:
	 /* empty */					  { ferite_do_after_fix_block(); }
|	T_ELSE						   { ferite_do_after_fix_before_else_block(); }
	 block							{ ferite_do_after_fix_else_statement(); }
	 ;

for_expression:
	 /* nothing */					{
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "forconst", 1, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: for truth value\n"));
									}
|	expression
	 ;

for_init_expression:
	 for_expression				   { ferite_do_pop(); }
|	multiple_variable_item
	 ;

foreach_start:
	 T_FOREACH						{
										ferite_error( CURRENT_SCRIPT, 0, "'foreach' has been removed from ferite for more elegant methods.\n" );
										ferite_error( CURRENT_SCRIPT, 0, "Please console the documentation for the Array module.\n" );
										feperror( NULL );
									}
	 ;

itterative_statement:
	 T_FOR '('						{
									ferite_do_begin_block();
										FUD(("PARSER: [FOR] Begin Initial Expression\n"));
									}
	  for_init_expression		   {
										FUD(("PARSER: [FOR] End Initial Expression\n"));
										ferite_do_for_loop_start();
									}
		 ';'						{ FUD(("PARSER: [FOR] Begin Test Expression\n")); }
	  for_expression				{ FUD(("PARSER: [FOR] End Test Expression\n")); }
		 ';'						{
										FUD(("PARSER: [FOR] Begin Itterator Expression\n"));
										ferite_do_for_loop_itterate();
									}
	  for_expression				{
										FUD(("PARSER: [FOR] End Itterator Expression\n"));
										ferite_do_pop();
										ferite_do_for_loop_block();
									}
		 ')'
	 block							{
										FUD(("PARSER: [FOR] Jump back to test expression\n"));
										ferite_do_for_loop_end();
					ferite_do_end_block();
									}

|	T_WHILE						  { ferite_do_while_begin(); }
		 '(' expression ')'		 { ferite_do_while_after_expr(); }
		 block					  { ferite_do_while_end(); }
|	T_DO							 { ferite_do_do_start(); }
	 block
	 T_WHILE '(' expression ')' ';'   { ferite_do_do_end(); }

|	foreach_start
	 ;

expression:
	cvar
|	cval
|	expression_without_var
	;

cval:
	 T_QSTRING						{
										ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, "strconst", $1, 0, FE_CHARSET_DEFAULT, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(str)		\"%s\"\n", $1));
										ffree_ngc( $1 ); FECT;
									}
|	T_TRUE						   {
										ferite_temp_variable = ferite_create_boolean_variable( NULL, "true", FE_TRUE, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(str)		\"true\"\n"));
									}
|	T_FALSE						  {
										ferite_temp_variable = ferite_create_boolean_variable( NULL, "false", FE_FALSE, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(str)		\"false\"\n"));
									}
|	T_NNUMBER						{
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "longconst", $1, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(int)		%d\n", $1));
									}
|	T_RNUMBER						{
										ferite_temp_variable = ferite_create_number_double_variable( NULL, "doubleconst", $1, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(float)	  %f\n", $1));
									}
|	T_CURRENT_LINE				   {
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "__LINE__", ferite_scanner_lineno, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
									}
|	T_CURRENT_FILE				   {
										ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, "__FILE__", ferite_scanner_file, 0, FE_CHARSET_DEFAULT, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
									}
	 ;

cvar:
	T_LABEL						   {
										ferite_do_variable_push( $1 );
										FUD(("PARSER: push(cvar)	   %s\n", $1));
										ffree_ngc( $1 ); FECT;
									}
|	T_SELF						   {   ferite_do_variable_push( "self" ); ferite_objcall_is_self = FE_TRUE; }
|	T_SUPER						  {   ferite_do_variable_push( "super" ); ferite_objcall_is_super = FE_TRUE; }
|	cvar '[' expression T_SLICE expression ']'
									{
										FUD(("PARSER: op->array_slice\n"));
										ferite_do_many_op( FERITE_OPCODE_array_slice,3);
									}
|	cvar '[' T_SLICE				 {
										FUD(("PARSER: op->array_slice\n"));
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "slice_first",0, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
									}
	expression ']'					{ ferite_do_many_op(FERITE_OPCODE_array_slice,3); }
|	cvar '[' expression T_SLICE ']'   {
										ferite_temp_variable = ferite_create_void_variable( NULL, "sliceconst", FE_STATIC );
										MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_many_op(FERITE_OPCODE_array_slice,3);
									}
|	cvar '[' T_SLICE ']'			  {
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "slice_first",0, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_temp_variable = ferite_create_void_variable( NULL, "sliceconst", FE_STATIC );
										MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_many_op(FERITE_OPCODE_array_slice,3);
									}
|	cvar '[' expression ']'		   {
										FUD(("PARSER: op->array_index"));
										ferite_do_binary_op(FERITE_OPCODE_array_index);
									}
|	cvar '[' ']'					 {   /* that dash cunning end of array index type thingy.... */
										/* it wasnt cunning enough because it broke with the introduction of negative indices ;) - ntropy*/
										ferite_temp_variable = ferite_create_void_variable( NULL, "void-placeholder", FE_STATIC );
										MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(int)		%d\n", FE_ARRAY_ADD_AT_END ));
										FUD(("PARSER: op->array_index\n"));
										ferite_do_binary_op( FERITE_OPCODE_array_index );
									}
|	cvar object_list
|	cval object_list
|								   { ferite_do_self_push(); ferite_objcall_is_self = FE_TRUE; }
	 object_list
	 ;

object_list:
	 object_property
|	object_list object_property
	 ;

object_no_call_list:
	 object_property_no_call
|	object_no_call_list object_property_no_call
	 ;

nvar:
	 T_LABEL						  {
										ferite_do_variable_push( $1 );
										FUD(("PARSER: push(cvar)	   %s\n", $1));
										ffree_ngc( $1 ); FECT;
									}
|	nvar object_no_call_list
	 ;

object_property_no_call:
	 T_OBJOP T_OBJPROPERTY			{
										ferite_do_variable_pushattr( $2, ferite_objcall_is_self, ferite_objcall_is_super );
										if( ferite_objcall_is_self || ferite_objcall_is_super ) {
											ferite_objcall_is_self = FE_FALSE;
											ferite_objcall_is_super = FE_FALSE;
										}
										FUD(("PARSER: push(object property)	   %s\n", $2));
										ffree_ngc( $2 ); FECT;
									}
	 ;

object_property:
	 object_property_no_call
|	T_OBJOP T_OBJPROPERTY
	'(' parameter_list ')'
	optional_function_using_clause	{
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										FUD(("PARSER: op->c_obj_func	   %s\n", $2 ));
										ferite_do_object_function_call( $2, ferite_current_arg_count, ferite_objcall_is_self, ferite_objcall_is_super );
										if( ferite_objcall_is_self || ferite_objcall_is_super ) {
											ferite_objcall_is_self = FE_FALSE;
											ferite_objcall_is_super = FE_FALSE;
										}
										ffree_ngc( $2 ); FECT;
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
									}
	 ;

closure_argument:
	T_LABEL						  { ferite_do_add_variable_to_paramlist( $1, F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_VALUE ); ffree_ngc( $1 ); FECT; }
|	parameter_define_item
|	parameter_multiple_args
	 ;

non_empty_closure_arguments:
	 closure_argument
|	non_empty_closure_arguments ',' closure_argument
	 ;

closure_arguments:
	 /* Empty List */
|	'(' non_empty_closure_arguments ')'
	 ;

closure_body:
									{ ferite_do_closure_start(); }
	 closure_arguments				{ ferite_do_closure_end_of_args(); }
	 nested_block					 { ferite_do_closure_end(); }
	 ;

closure_expression:
	 T_CLOSURE closure_body
	 ;

expression_without_var:
	 expression T_PLUS expression	 {
										 ferite_do_binary_op( FERITE_OPCODE_add );
										 FUD(("PARSER: op->add		  pop,pop\n"));
									}
|	expression T_MINUS expression	{
										 ferite_do_binary_op( FERITE_OPCODE_minus );
										 FUD(("PARSER: op->minus		pop,pop\n"));
									}
|	expression T_MULT expression	 {
										 ferite_do_binary_op( FERITE_OPCODE_mult );
										 FUD(("PARSER: op->mult		 pop,pop\n"));
									}
|	expression T_DIVIDE expression   {
										 ferite_do_binary_op( FERITE_OPCODE_divide );
										 FUD(("PARSER: op->divide	   pop,pop\n"));
									}
|	expression T_MODULUS expression  {
										 ferite_do_binary_op( FERITE_OPCODE_modulus );
										 FUD(("PARSER: op->modulus	  pop,pop\n"));
									}
|	expression T_EQUALS expression   {
										 ferite_do_binary_op( FERITE_OPCODE_equals );
										 FUD(("PARSER: op->equals	   pop,pop\n"));
									}
|	expression T_LESSTHAN_EQUALS expression
									{
										 ferite_do_binary_op( FERITE_OPCODE_less_than_equals );
										 FUD(("PARSER: op->less_than_eq pop,pop\n"));
									}
|	expression T_GRTRTHAN_EQUALS expression
									{
										 ferite_do_binary_op( FERITE_OPCODE_greater_than_equals );
										 FUD(("PARSER: op->grtr_then_eq pop,pop\n"));
									}
|	expression T_LESSTHAN expression {
										 ferite_do_binary_op( FERITE_OPCODE_less_than );
										 FUD(("PARSER: op->less_than	pop,pop\n"));
									}
|	expression T_GRTRTHAN expression {
										 ferite_do_binary_op( FERITE_OPCODE_greater_than );
										 FUD(("PARSER: op->grtr_then	pop,pop\n"));
									}
|	expression T_NOTEQUAL expression {
										 ferite_do_binary_op( FERITE_OPCODE_notequals );
										 FUD(("PARSER: op->not_equal	pop,pop\n"));
									}
|	expression T_LEFT_SHIFT expression
									{
										 ferite_do_binary_op( FERITE_OPCODE_left_shift );
										 FUD(("PARSER: op->left_shift	pop,pop\n"));
									}
|	expression T_RIGHT_SHIFT expression
									{
										 ferite_do_binary_op( FERITE_OPCODE_right_shift );
										 FUD(("PARSER: op->right_shift	pop,pop\n"));
									}
|	expression T_BINARY_OR expression
									{
										 ferite_do_binary_op( FERITE_OPCODE_binary_or );
										 FUD(("PARSER: op->binary_or	pop,pop\n"));
									}
|	expression T_BINARY_AND expression
									{
										 ferite_do_binary_op( FERITE_OPCODE_binary_and );
										 FUD(("PARSER: op->binary_and   pop,pop\n"));
									}
|	expression T_BINARY_XOR expression
									{
										 ferite_do_binary_op( FERITE_OPCODE_binary_xor );
										 FUD(("PARSER: op->binary_xor	pop,pop\n"));
									}
|	cvar T_ASSIGN					{ ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_assign, FERITE_OPCODE_assign ); }
|	cvar T_PLUS_ASSIGN			    { ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_add_assign, FERITE_OPCODE_add ); }
|	cvar T_MINUS_ASSIGN			    { ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_minus_assign, FERITE_OPCODE_minus ); }
|	cvar T_MULT_ASSIGN			    { ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_mult_assign, FERITE_OPCODE_mult ); }
|	cvar T_DIVIDE_ASSIGN			{ ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_divide_assign, FERITE_OPCODE_divide ); }
|	cvar T_LEFT_SHIFT_ASSIGN		{ ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_left_shift_assign, FERITE_OPCODE_left_shift ); }
|	cvar T_RIGHT_SHIFT_ASSIGN		{ ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_right_shift_assign, FERITE_OPCODE_right_shift ); }
|	cvar T_BINARY_AND_ASSIGN		{ ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_binary_and_assign, FERITE_OPCODE_binary_and ); }
|	cvar T_BINARY_OR_ASSIGN		    { ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_binary_or_assign, FERITE_OPCODE_binary_or ); }
|	cvar T_BINARY_XOR_ASSIGN		{ ASSIGNMENT_SETUP() } expression { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_binary_xor_assign, FERITE_OPCODE_binary_xor ); }
|	T_NEW nvar
	 '(' parameter_list ')'		   {
										 int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										 ferite_do_new_object( ferite_current_arg_count );
										 if( value != NULL )
										 {
											 ferite_current_arg_count = *value;
											 ffree_ngc( value );
										 }
										 FUD(("PARSER: new object\n" ));
									}
|	T_PLUS expression %prec T_UPLUS  {
										 FUD(("PARSER: unary plus\n"));
										 ferite_do_unary_op( FERITE_OPCODE_positive_var );
									}
|	T_MINUS expression %prec T_UMINUS
									{
										 FUD(("PARSER: unary minus\n"));
										 ferite_do_unary_op( FERITE_OPCODE_negative_var );
									}
|	cvar T_INCR					  {
										 FUD(("PARSER: right increment\n"));
										 ferite_do_unary_op( FERITE_OPCODE_right_incr );
									}
|	T_INCR cvar					  {
										 FUD(("PARSER: left increment\n"));
										 ferite_do_unary_op( FERITE_OPCODE_left_incr );
									}
|	cvar T_DECR					  {
										 FUD(("PARSER: right decrement\n"));
										 ferite_do_unary_op( FERITE_OPCODE_right_decr );
									}
|	T_DECR cvar					  {
										 FUD(("PARSER: left decrement\n"));
										 ferite_do_unary_op( FERITE_OPCODE_left_decr );
									}
|	T_NOT_OPERATOR expression		{
										 ferite_do_unary_op( FERITE_OPCODE_not_op );
										 FUD(("PARSER: op->not_op		   pop\n" ));
									}
|	T_EVAL '(' expression ')'		{
										 ferite_do_unary_op( FERITE_OPCODE_eval );
										 FUD(("PARSER: op->eval\n"));
									}
|	T_INCLUDE '(' expression ')'	 {
										 ferite_do_unary_op( FERITE_OPCODE_include );
										 FUD(("PARSER: op->include\n"));
									}
|	expression T_ISA var_type		{
										 ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, "strconst", ferite_variable_id_to_str( NULL,ferite_last_type), 0, FE_CHARSET_DEFAULT, FE_STATIC );
										 MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										 ferite_do_push( ferite_temp_variable );
										 ferite_do_binary_op( FERITE_OPCODE_isa );
										 FUD(("PARSER: op->include\n"));
									}
|	expression T_INSTANCEOF nvar	 {
										 ferite_do_binary_op( FERITE_OPCODE_instanceof );
										 FUD(("PARSER: op->include\n"));
									}
|	T_ARGS '(' ')'				   { ferite_do_get_args(); }
|	T_RECIPIENT '(' ')'			  { ferite_do_get_yield_block(); }
|	question_colon_expr
|	array_in_script
|	array_in_script object_list
|	function_call
|	function_call object_list
|	logical_operators
|	closure_expression
|	'(' expression ')'
|	'(' expression ')' object_list
	 ;

question_colon_expr:
	'(' expression '?'				{ ferite_do_if_statement(); }
	  expression ':'				{ ferite_do_after_then_before_else_statement(); }
	  expression ')'				{ ferite_do_after_else_statement(); }
	;

optional_function_using_clause:
	 /* Empty */
|	T_USING closure_body			   { ferite_do_yield_block( FE_FALSE ); }
|	T_USING T_RECIPIENT '(' ')'	   { ferite_do_yield_block( FE_TRUE ); }
	 ;

function_call:
	 T_LABEL						 { ferite_do_push_main_namespace(); }
	 '(' parameter_list ')'
	 optional_function_using_clause	{
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										FUD(("PARSER: op->c_func	   %s\n", $1 ));
										ferite_do_function_call( $1, ferite_current_arg_count );
										ffree_ngc( $1 ); FECT;
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
									}
|	T_SUPER						  { ferite_do_super_push(); }
	 '(' parameter_list ')'
	 optional_function_using_clause	{
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										ferite_do_object_function_call( "constructor", ferite_current_arg_count, FE_FALSE, FE_TRUE );
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
									}
|	T_SELF						   { ferite_do_self_push(); }
	 '(' parameter_list ')'
	 optional_function_using_clause	{
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										ferite_do_object_function_call( "constructor", ferite_current_arg_count, FE_TRUE, FE_FALSE );
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
									}
|	T_DELIVER
	 '(' parameter_list ')'		   {
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										FUD(("PARSER: op->yield\n"));
										ferite_do_yield_call( ferite_current_arg_count );
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
						}
	 ;

parameter_list:
	 /* empty */					  {
										int *value = fmalloc_ngc(sizeof(int));
										*value = ferite_current_arg_count;
										ferite_stack_push( FE_NoScript, ferite_argcount_stack, value );
										ferite_current_arg_count = 0;
									}
|								   {
										int *value = fmalloc_ngc(sizeof(int));
										*value = ferite_current_arg_count;
										ferite_stack_push( FE_NoScript, ferite_argcount_stack, value );
										ferite_current_arg_count = 0;
									}
	 non_empty_function_call_parameter_list
	 ;

non_empty_function_call_parameter_list:
	 function_call_parameter
|	non_empty_function_call_parameter_list ',' function_call_parameter
	 ;

function_call_parameter:
	 { ferite_current_arg_count++ } expression
	 ;

array_in_script:
	'['							   {
										char *buf = fmalloc_ngc(256);
										ferite_var_array_count++;
										snprintf( buf, 256, "!__array%d__", ferite_var_array_count );
										ferite_do_add_variable( buf, F_VAR_UARRAY, ferite_subtype_link(CURRENT_SCRIPT, "A"), FE_FALSE, FE_FALSE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );
										ferite_stack_push( FE_NoScript, ferite_compiled_arrays_stack, buf );
										FUD(("PARSER: push			 array_start\n"));

										ferite_do_variable_push( buf );
										ferite_do_unary_op( FERITE_OPCODE_array_clear );
										ferite_do_pop();
									}
	array_elements_list ']'		   {
										char *buf = ferite_stack_pop( FE_NoScript, ferite_compiled_arrays_stack );
										ferite_do_variable_push( buf );
										ffree_ngc( buf );
									}
	;

array_elements_list:
	/* empty */
|	non_empty_array_elements_list
	;

non_empty_array_elements_list:
	array_element
|	non_empty_array_elements_list ',' array_element
	;

array_element:
	 expression
	 array_element_post			   {
										ferite_do_variable_push( ferite_stack_top( FE_NoScript, ferite_compiled_arrays_stack ) );
										ferite_do_swap_top(0);
										ferite_do_swap_top(1);
										ferite_do_swap_top(0);
										ferite_do_many_op( FERITE_OPCODE_array_index_assign, 3 );
										ferite_do_pop();
									}
array_element_post:
	 /* empty */					  {
										ferite_temp_variable = ferite_create_void_variable( NULL, "void-placeholder", FE_STATIC );
										MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
									}
|	T_RARROW expression			  {   ferite_do_swap_top(0); }
	 ;

logical_operators:
	 expression T_LOGICAL_AND		 { ferite_do_if_statement(); }
	 expression					   {
										ferite_do_after_then_before_else_statement();
										/* we have to push a value onto the stack, otherwise we end up with a smashed stack */
										ferite_temp_variable = ferite_create_boolean_variable( NULL, "false", FE_FALSE, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_after_else_statement();
								   }
|	expression T_LOGICAL_OR		 { ferite_do_not_if_statement(); }
	 expression					  {
										ferite_do_after_then_before_else_statement();
										/* we have to push a value onto the stack, otherwise we end up with a smashed stack */
										ferite_temp_variable = ferite_create_boolean_variable( NULL, "true", FE_TRUE, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_after_else_statement();
								   }
	 ;

%%

int ferite_parse()
{
	 int parser_return = 0;

	 FE_ENTER_FUNCTION;
	 parser_return = fepparse();
	 FE_LEAVE_FUNCTION( parser_return );
}

