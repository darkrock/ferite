%{
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

/*
 * This is the scanner for the ferite programming language written by Chris Ross
 */

%}

%x S_STRING
%x S_COMMENT
%x S_CPPCOMMENT
%x S_SCRIPTING
%x S_PROPERTY
%x S_LOOKING_FOR_REGEX
%x S_QUOTE_STRING
%x S_STRING_VARIABLE
%x S_STRING_VARIABLE_PART_TWO
%x S_STRING_VARIABLE_PART_THREE
%x S_NATIVE_CODE
%x S_WANT_DOT_LABEL
%x S_STRING_EXPR
%x S_DIRECTIVE_EXPR
%x S_DIRECTIVE_EXPR_CONTENTS
%x S_KRIS_KROSSLL_MAKE_YOU_JUMP_JUMP
%x S_TYPING
%x S_GRTRTHAN

%option stack
%option noyywrap

%{

#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

typedef struct _ferite_lex_state {
	YY_BUFFER_STATE buffer_state;
	int state;
	FILE *in;
	int lineno;
	char *filename;
	int is_macro;
} FeriteLexState;

#include <stdio.h>
#include <setjmp.h>
#include "ferite.h"
#include "ferite/fparser.h"

#ifdef WIN32
# include <io.h>
# define  read _read
#endif

#define YY_NO_TOP_STATE

#define YY_DECL int yylex(YYSTYPE *feplval)

#define SCANNER_DEBUG
#ifndef SCANNER_DEBUG
# undef  D
# define FUD( var )
#endif

	extern jmp_buf ferite_compiler_jmpback;
	extern int ferite_compile_error;
	int   ferite_scanner_lineno = 1;
	int   ferite_i;

#define BUFFER_LENGTH   32768

	char  ferite_cstring_buf[BUFFER_LENGTH]; /* should be big enough */
	char *ferite_cstrptr;
	FeriteBuffer *ferite_cstring_buffer = NULL;
	int   ferite_scanner_buffer_counter = 0;
	int   ferite_scanner_in_expression = FE_FALSE;

	char  ferite_variablenamebuf[4096];
	char  ferite_regex_buf[4096];
	char *ferite_rgxptr;
	char *ferite_last_token_alloc = NULL;

	extern FeriteStack *ferite_compiled_arrays_stack;
	extern FeriteCompileRecord *ferite_current_compile;

	YY_BUFFER_STATE ferite_fp_state;

	char *ferite_scanner_file = NULL;

	FeriteStack *ferite_scanner_stack = NULL;
	FeriteLexState *ferite_save_state;

	/*****
	 ***** NATIVE CODE HANDLING STUFF
	 *****/
	int ferite_native_code_bracket_depth = 0;
	int ferite_native_code_has_started = 0;
	char ferite_current_native_block_file[4096];
	int  ferite_current_native_block_line;

	/* For caching */
	FeriteStack *ferite_compiled_lengths_stack = NULL;


	void fepwarning( char *message );

%}

NNUM		  (([0]{1})|([1-9]+[0-9]*))
RNUM		  (([0-9]*[\.][0-9]+)|([0-9]+[\.][0-9]+))
HNUM		  "0x"[0-9a-fA-F]+
ONUM		  "0"[0-7]+
BNUM		  "0b"[0-1]+
LABEL		 [a-zA-Z_][a-zA-Z0-9_]*
ALPHA		 [a-zA-Z]*
DOTLABEL	  [a-zA-Z_][a-zA-Z0-9_]*([\.][a-zA-Z_][a-zA-Z0-9_]*)*

%%

<S_SCRIPTING>"/*"		{ yy_push_state( S_COMMENT ); }
<S_COMMENT>"/*"		  { yy_push_state( S_COMMENT ); }
<S_COMMENT>[^*/\n]*	  /* eat up everything thats not a '*' */
<S_COMMENT>"*"+[^*/\n]*  /* eat up '*' not followed by '/'s */
<S_COMMENT>"/"+[^*]	  /* eat up '/' not followed by '*'s */
<S_COMMENT>\n			ferite_scanner_lineno++;
<S_COMMENT>"*"+"/"	   { yy_pop_state(); }

<S_SCRIPTING>"//"		yy_push_state( S_CPPCOMMENT );
<S_CPPCOMMENT>[^\n]*	 /* eat up everything thats not a newline */
<S_CPPCOMMENT>\n		 {
						   ferite_scanner_lineno++;
						   yy_pop_state();
						 }

<S_SCRIPTING,S_DIRECTIVE_EXPR,S_TYPING>\"		  {
						   ferite_cstrptr = ferite_cstring_buf;
						   yy_push_state( S_STRING );
			 }
<S_STRING>\${LABEL}	  {
							*ferite_cstrptr = '\0';
							feplval->text = fstrdup( ferite_cstring_buf );
							ferite_last_token_alloc = feplval->text;
							/* now we pop a sorting out var name */
							memset( ferite_variablenamebuf, '\0', strlen( feptext ) + 1 );
							strncpy( ferite_variablenamebuf, feptext+1, strlen( feptext+1 ) );
							yy_push_state( S_STRING_VARIABLE );
							return T_QSTRING;
						 }
<S_STRING_VARIABLE>.	 {
							unput( feptext[0] );
							yy_pop_state();
							yy_push_state( S_STRING_VARIABLE_PART_TWO );
							return T_PLUS;
						 }
<S_STRING_VARIABLE_PART_TWO>. {
							unput( feptext[0] );
							feplval->text = fstrdup( ferite_variablenamebuf );
							ferite_last_token_alloc = feplval->text;
							yy_pop_state();
							yy_push_state( S_STRING_VARIABLE_PART_THREE );
							ferite_cstrptr = ferite_cstring_buf;
							*ferite_cstrptr = '\0';
							return T_LABEL;
						 }
<S_STRING_VARIABLE_PART_THREE>. {
							unput( feptext[0] );
							yy_pop_state();
							return T_PLUS;
						 }

<S_STRING>"${"		   {
							yy_push_state( S_STRING_EXPR );
							*ferite_cstrptr = '\0';
							feplval->text = fstrdup( ferite_cstring_buf );
							ferite_last_token_alloc = feplval->text;
							return T_QSTRING;
						 }
<S_STRING_EXPR>.		 {
							ferite_scanner_in_expression = FE_TRUE;
							unput( feptext[0] );
							yy_pop_state();
							yy_push_state( S_SCRIPTING );
							return T_PLUS;
						}
<S_SCRIPTING>"}"		 {
							if( ferite_scanner_in_expression )
							{
									ferite_cstrptr = ferite_cstring_buf;
									*ferite_cstrptr = '\0';
									ferite_scanner_in_expression = FE_FALSE;
									yy_pop_state();
									return T_PLUS;
							}
							else
							{
									return '}';
							}
						 }

<S_STRING>\"			 {
							/* we have a closing quote lets clean up */
							yy_pop_state();
							*ferite_cstrptr = '\0';
							FUD(( "Found String: %s\n", ferite_cstring_buf ));
							feplval->text = fstrdup( ferite_cstring_buf );
							ferite_last_token_alloc = feplval->text;
							return T_QSTRING;
						 }
<S_STRING>\n			 {
							*ferite_cstrptr++ = '\n';
							ferite_scanner_lineno++;
						 }
<S_STRING>\\[0-7]{1,3}   {
							/* octal escape sequence */
							unsigned int result;
							sscanf( feptext+1, "%o", &result );
							if( result > 0xff )
							   printf( "Error: Octal constant (%s) is out-of-bounds\n", feptext );
							*ferite_cstrptr++ = result;
						 }
<S_STRING>\\x[0-9a-fA-F]{2} {
							/* hex escape sequence */
							unsigned int result = strtol( feptext+2, NULL, 16 );
							if( result > 0xff )
							   printf( "Error: Hexadecimal constant (%s) is out-of-bounds\n", feptext );
							*ferite_cstrptr++ = result;
						 }
<S_STRING>\\b[0-1]{8}	{
							/* binary escape sequence */
							unsigned int result = strtol( feptext+2, NULL, 2 );
							if( result > 0xff )
							   printf( "Error: Binary constant (%s) is out-of-bounds\n", feptext );
							*ferite_cstrptr++ = result;
						 }
<S_STRING>\\d[0-9]{1,3}  {
							/* decimal escape sequence */
							unsigned int result = strtol( feptext+2, NULL, 10 );
							if( result > 0xff )
							   printf( "Error: Decinmal constant (%s) is out-of-bounds\n", feptext );
							*ferite_cstrptr++ = result;
						 }
<S_STRING>\\n			*ferite_cstrptr++ = '\n';
<S_STRING>\\t			*ferite_cstrptr++ = '\t';
<S_STRING>\\r			*ferite_cstrptr++ = '\r';
<S_STRING>\\f			*ferite_cstrptr++ = '\f';
<S_STRING>\\a			*ferite_cstrptr++ = '\a';
<S_STRING>\\\"		   *ferite_cstrptr++ = '\"';
<S_STRING>\\(.|\n)	   *ferite_cstrptr++ = feptext[1];
<S_STRING>[^\\\n\"]	  *ferite_cstrptr++ = feptext[0];

<S_SCRIPTING,S_DIRECTIVE_EXPR,S_TYPING>'		   {
							ferite_cstrptr = ferite_cstring_buf;
							yy_push_state( S_QUOTE_STRING );
						 }
<S_QUOTE_STRING>\n	   {
							*ferite_cstrptr++ = '\n';
							ferite_scanner_lineno++;
						 }
<S_QUOTE_STRING>\\\'	 *ferite_cstrptr++ = '\'';
<S_QUOTE_STRING>[^\n']   *ferite_cstrptr++ = feptext[0];
<S_QUOTE_STRING>'		{
							/* we have a closing quote lets clean up "*/
							 yy_pop_state();
							*ferite_cstrptr = '\0';
							FUD(( "Found String: %s\n", ferite_cstring_buf ));
							feplval->text = fstrdup( ferite_cstring_buf );
							ferite_last_token_alloc = feplval->text;
							return T_QSTRING;
						}

<S_NATIVE_CODE>.		{
							if( ferite_scanner_block_read( feptext[0], '{', '}', ';', T_NATIVE_CODE_BLOCK, feplval ) == T_NATIVE_CODE_BLOCK )
								return T_NATIVE_CODE_BLOCK;
						}
<S_NATIVE_CODE>\n		{
							ferite_scanner_lineno++;
							ferite_scanner_block_read( feptext[0], '{', '}', ';', T_NATIVE_CODE_BLOCK, feplval );
						}

<S_SCRIPTING>"__LINE__"  { return T_CURRENT_LINE; }
<S_SCRIPTING>"__FILE__"  { return T_CURRENT_FILE; }
<S_SCRIPTING>"self"	  { return T_SELF; }
<S_SCRIPTING>"super"	 { return T_SUPER; }
<S_SCRIPTING>"constructor" { return T_CONSTRUCTOR; }
<S_SCRIPTING>"destructor" { return T_DESTRUCTOR; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"class"	 { return T_CLASS; }
<S_SCRIPTING>"extends"   { return T_EXTENDS; }
<S_SCRIPTING>"function"  { return T_FUNCTION; }
<S_SCRIPTING>"native"	{ return T_NATIVE; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"namespace" { return T_NAMESPACE; }
<S_SCRIPTING>"if"		{ return T_IF; }
<S_SCRIPTING>"else"	  { return T_ELSE; }
<S_SCRIPTING>"for"	   { return T_FOR; }
<S_SCRIPTING>"foreach"   { return T_FOREACH; }
<S_SCRIPTING>"while"	 { return T_WHILE; }
<S_SCRIPTING>"do"		{ return T_DO; }
<S_SCRIPTING>"switch"	{ return T_SWITCH; }
<S_SCRIPTING>"case"	  { return T_CASE; }
<S_SCRIPTING>"default"   { return T_DEFAULT; }
<S_SCRIPTING>"uses"	  { return T_USES; }
<S_SCRIPTING>"global"	{ return T_GLOBAL; }
<S_SCRIPTING>"return"	{ return T_RETURN; }
<S_SCRIPTING>"new"	   { return T_NEW; }
<S_SCRIPTING>"final"	 { return T_FINAL; }
<S_SCRIPTING>"raise"	 { return T_RAISE; }
<S_SCRIPTING>"iferr"	 {
							fepwarning( "The keyword 'iferr' has been deprecated in favour of 'monitor', please update your code!");
							return T_MONITOR;
						 }
<S_SCRIPTING>"fix"	   {
							fepwarning( "The keyword 'fix' has been deprecated in favour of 'handle', please update your code!");
							return T_HANDLE;
						 }
<S_SCRIPTING>"monitor"   { return T_MONITOR; }
<S_SCRIPTING>"handle"	{ return T_HANDLE; }
<S_SCRIPTING>"break"	 { return T_BREAK; }
<S_SCRIPTING>"continue"  { return T_CONTINUE; }
<S_SCRIPTING>"static"	{ return T_STATIC; }
<S_SCRIPTING,S_DIRECTIVE_EXPR>"true"	  { return T_TRUE; }
<S_SCRIPTING,S_DIRECTIVE_EXPR>"false"	 { return T_FALSE; }
<S_SCRIPTING>"modifies"  { return T_MODIFIES; }
<S_SCRIPTING>"atomic"	{ return T_ATOMIC; }
<S_SCRIPTING>"eval"	  { return T_EVAL; }
<S_SCRIPTING>"include"   { return T_INCLUDE; }
<S_SCRIPTING>"instanceof" { return T_INSTANCEOF; }
<S_SCRIPTING>"isa"	   { return T_ISA; }
<S_SCRIPTING>"arguments" { return T_ARGS; }
<S_SCRIPTING>"private"   { return T_PRIVATE; }
<S_SCRIPTING>"protected" { return T_PROTECTED; }
<S_SCRIPTING>"public"	{ return T_PUBLIC; }
<S_SCRIPTING>"abstract"  { return T_ABSTRACT; }
<S_SCRIPTING>"closure"   { return T_CLOSURE; }
<S_SCRIPTING>"using"	 { return T_USING; }
<S_SCRIPTING>"deliver"   { return T_DELIVER; }
<S_SCRIPTING>"recipient" { return T_RECIPIENT; }
<S_SCRIPTING>"protocol"   { return T_PROTOCOL; }
<S_SCRIPTING>"implements" { return T_IMPLEMENTS; }
<S_SCRIPTING>"directive" { return T_DIRECTIVE; }
<S_SCRIPTING>"expansion" { return T_EXPANSION; }

<S_SCRIPTING>"module-init"   { return T_MODULE_INIT; }
<S_SCRIPTING>"module-deinit" { return T_MODULE_DEINIT; }
<S_SCRIPTING>"module-register"   { return T_MODULE_REGISTER; }
<S_SCRIPTING>"module-unregister" { return T_MODULE_UNREGISTER; }
<S_SCRIPTING>"module-header" { return T_MODULE_HEADER; }
<S_SCRIPTING>"module-flags"  { return T_MODULE_FLAGS; }

<S_SCRIPTING>".."		{ return T_SLICE; }
<S_SCRIPTING>"..."	   { return T_MULTIPLE_ARGS; }
<S_SCRIPTING>"=>"		{ return T_RARROW; }
<S_SCRIPTING>"="		 { return T_ASSIGN; }
<S_SCRIPTING>"+="		{ return T_PLUS_ASSIGN; }
<S_SCRIPTING>"-="		{ return T_MINUS_ASSIGN; }
<S_SCRIPTING>"*="		{ return T_MULT_ASSIGN; }
<S_SCRIPTING>"/="		{ return T_DIVIDE_ASSIGN; }
<S_SCRIPTING>"=="		{ return T_EQUALS; }
<S_SCRIPTING>"<="		{ return T_LESSTHAN_EQUALS; }
<S_SCRIPTING>">="		{ return T_GRTRTHAN_EQUALS; }
<S_SCRIPTING,S_GRTRTHAN>"<" { return T_LESSTHAN; }
<S_SCRIPTING,S_GRTRTHAN>">" { return T_GRTRTHAN; }
<S_SCRIPTING>"!="		{ return T_NOTEQUAL; }
<S_SCRIPTING>"++"		{ return T_INCR; }
<S_SCRIPTING>"--"		{ return T_DECR; }
<S_SCRIPTING>"+"		 { return T_PLUS; }
<S_SCRIPTING>"-"		 { return T_MINUS; }
<S_SCRIPTING>"*"		 { return T_MULT; }
<S_SCRIPTING>"/"		 { return T_DIVIDE; }
<S_SCRIPTING>"%"		 { return T_MODULUS; }
<S_SCRIPTING>"|"		 { return T_BINARY_OR; }
<S_SCRIPTING>"||"		{ return T_LOGICAL_OR; }
<S_SCRIPTING>"or"		{ return T_LOGICAL_OR; }
<S_SCRIPTING>"&"		 { return T_BINARY_AND; }
<S_SCRIPTING>"&&"		{ return T_LOGICAL_AND; }
<S_SCRIPTING>"and"	   { return T_LOGICAL_AND; }
<S_SCRIPTING>"<<"		{ return T_LEFT_SHIFT; }
<S_SCRIPTING>">>"		{ return T_RIGHT_SHIFT; }
<S_SCRIPTING>"<<="	   { return T_LEFT_SHIFT_ASSIGN; }
<S_SCRIPTING>">>="	   { return T_RIGHT_SHIFT_ASSIGN; }
<S_SCRIPTING>"&="		{ return T_BINARY_AND_ASSIGN; }
<S_SCRIPTING>"|="		{ return T_BINARY_OR_ASSIGN; }
<S_SCRIPTING>"^"		 { return T_BINARY_XOR; }
<S_SCRIPTING>"^="		{ return T_BINARY_XOR_ASSIGN; }
<S_SCRIPTING>"!"		 { return T_NOT_OPERATOR; }
<S_SCRIPTING>"not"	   { return T_NOT_OPERATOR; }

<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"number"	 { return T_VAR_NUMBER; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"integer" { return T_VAR_NUMBER_LNG; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"long"	 { return T_VAR_NUMBER_LNG; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"float"	 { return T_VAR_NUMBER_DBL; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"double"	 { printf("double\n"); return T_VAR_NUMBER_DBL; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"string"	 { return T_VAR_STRING; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"void"	 { return T_VAR_VOID; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"object"	 { return T_VAR_OBJECT; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"array"	 { return T_VAR_ARRAY; }
<S_TYPING,S_SCRIPTING,S_DIRECTIVE_EXPR>"boolean" { return T_VAR_BOOLEAN; }
<S_TYPING,S_SCRIPTING>"undefined"				 { return T_RET_UNDEF; }

<S_SCRIPTING,S_PROPERTY,S_DIRECTIVE_EXPR>[\n]		{
						   ferite_scanner_lineno++;
						 }

<S_SCRIPTING,S_WANT_DOT_LABEL,S_DIRECTIVE_EXPR,S_PROPERTY,S_TYPING>[ \t]  {
							/* eat whitespace */
							FUD(( "State: %d\n", YY_START ));
							#ifdef SCANNER_DEBUG
							  if( YY_START == S_LOOKING_FOR_REGEX )
								 FUD(( "Looking for regex\n" ));
							#endif
						 }

<S_SCRIPTING>"."		 {
							FUD(("SCANNER: Found Object OP (.)\n" ));
							yy_push_state( S_PROPERTY );
							return T_OBJOP;
						 }

<S_PROPERTY>{LABEL}	  {
							FUD(("SCANNER: Object part call\n"));
							feplval->text = fstrdup( feptext );
							FUD(( "SCANNER: returning object part: \"%s\"\n", feplval->text ));
							yy_pop_state();
							return T_OBJPROPERTY;
						 }

<S_SCRIPTING,S_DIRECTIVE_EXPR>{LABEL}	 {
							FUD(( "SCANNER: Token: %s\n", feptext ));
							feplval->text = fstrdup( feptext );
							ferite_last_token_alloc = feplval->text;
							return T_LABEL;
						 }
<S_SCRIPTING>"#"{LABEL}  {
							feplval->text = fstrdup( feptext );
							ferite_last_token_alloc = feplval->text;
							return T_DIRECTIVE_LABEL;
						 }

<S_WANT_DOT_LABEL,S_TYPING>{DOTLABEL}  {
							feplval->text = fstrdup( feptext );
							ferite_last_token_alloc = feplval->text;
							return T_DOT_LABEL;
						 }

<S_SCRIPTING,S_DIRECTIVE_EXPR,S_TYPING>{NNUM}	  {
							feplval->inumber = atoi( feptext );
							FUD(( "SCANNER: Natural number: %d\n", feplval->inumber ));
							return T_NNUMBER;
						 }

<S_SCRIPTING,S_DIRECTIVE_EXPR,S_TYPING>{RNUM}	  {
							feplval->fnumber = atof( feptext );
							FUD(( "SCANNER: Real number: %f\n", feplval->fnumber ));
							return T_RNUMBER;
						 }

<S_SCRIPTING,S_DIRECTIVE_EXPR,S_TYPING>{HNUM}		 {
							FUD(( "SCANNER: Found hexadecimal number: %s (%d)\n", feptext, (int)strtol( feptext, NULL, 16 ) ));
							feplval->inumber = strtol( feptext, NULL, 16 );
							return T_NNUMBER;
						 }

<S_SCRIPTING,S_DIRECTIVE_EXPR,S_TYPING>{ONUM}		 {
							FUD(( "SCANNER: Found Octal number: %s (%d)\n", feptext, (int)strtol( feptext, NULL, 8 ) ));
							feplval->inumber = strtol( feptext, NULL, 8 );
							return T_NNUMBER;
						 }

<S_SCRIPTING,S_DIRECTIVE_EXPR,S_TYPING>{BNUM}		 {
							FUD(( "SCANNER: Found hexadecimal number: %s (%d)\n", feptext, (int)strtol( feptext+2, NULL, 2 ) ));
							feplval->inumber = strtol( feptext+2, NULL, 2 );
							return T_NNUMBER;
						 }

<S_SCRIPTING,S_STRING,S_DIRECTIVE_EXPR,S_TYPING>\r

<S_DIRECTIVE_EXPR>\(			{
									yy_push_state( S_DIRECTIVE_EXPR_CONTENTS );
									unput( feptext[0] );
						 		}
<S_DIRECTIVE_EXPR_CONTENTS>.	{
									if( ferite_scanner_block_read( feptext[0], '(', ')', 0, T_QSTRING, feplval ) == T_QSTRING )
										return T_QSTRING;
								}
<S_DIRECTIVE_EXPR_CONTENTS>\n	{
									ferite_scanner_lineno++;
									ferite_scanner_block_read( feptext[0], '(', ')', 0, T_QSTRING, feplval );
								}

<S_SCRIPTING,S_PROPERTY,S_LOOKING_FOR_REGEX,S_DIRECTIVE_EXPR>.		   {
							FUD(( "Unknown character in source: %s:%d '%c' '%d'\n", ferite_scanner_file, ferite_scanner_lineno, feptext[0], feptext[0] ));
							if( (int)feptext[0] >= 0 )
								return feptext[0];
						}
						
<S_KRIS_KROSSLL_MAKE_YOU_JUMP_JUMP>; {
							while( YYSTATE != S_SCRIPTING ) {
								/* This is to make sure that what ever state we get into as a result of the function start, we roll out of it */
								yy_pop_state();
							}
							return T_FUNCTION_JUMP;
						}
<<EOF>> {
			/*if( !ferite_scanner_end_macro() ) {*/
				yyterminate();
			/*}*/
		}
%%

/* This is an error function used by yacc, and must be defined */
void feperror( char *message )
{
   ferite_error( ferite_current_compile->script, 0, "Parse Error: on line %d in \"%s\" [%d]\n", ferite_scanner_lineno, ferite_scanner_file, YYSTATE );
	if( message != NULL )
		ferite_error( ferite_current_compile->script, 0, "   (%s)\n", message );
	ferite_compile_error = 1;
   longjmp( ferite_compiler_jmpback, 1 );
}
void fepwarning( char *message )
{
	ferite_warning( ferite_current_compile->script, "Parse Warning: on line %d in \"%s\"\n", ferite_scanner_lineno, ferite_scanner_file );
	if( message != NULL )
		ferite_warning( ferite_current_compile->script, "   (%s)\n", message );
}

void ferite_set_filename( char *name )
{
	FE_ENTER_FUNCTION;
	ferite_scanner_file = fstrdup( name );
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_parser_script_normalise( char *str )
{
	int current = 0;
	int insertpos = 0;
	int in_block = FE_FALSE;
	char block_escape = 0;
	
	while( str[current] != '\0' )
	{
		if( str[current] == '\r' )
		{
			if( str[current+1] == '\n' )
			{
				/* We are on windows or dealing with a file from windows */
				current++; /* Bump on one more character to the LF and then... */
				str[insertpos++] = str[current++]; /* ...copy it over */
			} else {
				/* We are on macos or dealing with a file from macos */
				current++;
				str[insertpos++] = '\n'; /* simply replace it with a LF */
			}
		}
		else {
			char c = str[current];
			char next = str[current+1];
			int record_c = FE_FALSE;
			
			if( !in_block ) {
				if( c == '"' ) {
					record_c = FE_TRUE;
					in_block = FE_TRUE;
					block_escape = '"';
				} else if( c == '\'' ) {
					record_c = FE_TRUE;
					in_block = FE_TRUE;
					block_escape = '\'';
				} else if( (c == ' ' || c == '\t') && (next == ' ' || next == '\t') ) {
					record_c = FE_FALSE;
				} else {
					record_c = FE_TRUE;
				}
			} else {
				record_c = FE_TRUE;
				if( c == block_escape && str[current - 1] != '\\' ) {
					in_block = FE_FALSE;
				}
			}
			if( record_c ) {
				str[insertpos++] = str[current++];
			} else {
				current++;
			}
		}
	}
	str[insertpos] = '\0';
}
void ferite_parser_script_agressive_normalise( char *str )
{
	int current = 0;
	int insertpos = 0;
	int in_block = FE_FALSE;
	char block_escape = 0;
	
	while( str[current] != '\0' )
	{
		if( str[current] == '\r' ) {
			if( str[current+1] == '\n' ) {
				/* We are on windows or dealing with a file from windows */
				current += 2;
			} else {
				/* We are on macos or dealing with a file from macos */
				current++;
			}
		}
		else if( str[current] == '\n' ) {
			current++;
		}
		else {
			char c = str[current];
			char next = str[current+1];
			int record_c = FE_FALSE;
			int trade_tabs = FE_FALSE;
			
			if( !in_block ) {
				if( c == '"' ) {
					record_c = FE_TRUE;
					in_block = FE_TRUE;
					block_escape = '"';
				} else if( c == '\'' ) {
					record_c = FE_TRUE;
					in_block = FE_TRUE;
					block_escape = '\'';
				} else if( (c == ' ' || c == '\t') && (next == ' ' || next == '\t') ) {
					record_c = FE_FALSE;
				} else {
					record_c = FE_TRUE;
				}
				trade_tabs = FE_TRUE;
			} else {
				record_c = FE_TRUE;
				if( c == block_escape && str[current - 1] != '\\' ) {
					in_block = FE_FALSE;
				}
			}
			if( record_c ) {
				if( str[current] == '/' ) {
					current++;
					if( str[current] == '/' ) {
						while( str[current] != '\0' && str[current++] != '\n' )
							;
					} else if( str[current] == '*' ) { 
						char previous_character = 0;
						current++;
						while( str[current] != '\0' && str[current] != '/' && previous_character != '*' ) { \
							previous_character = str[current++];
						}
						current++;
					} else {
						str[insertpos++] = '/';
						str[insertpos++] = str[current++];
					}
				} else {
					str[insertpos++] = (str[current] == '\t' && trade_tabs ? ' ' : str[current]);
					current++;
				}
			} else {
				current++;
			}
		}
	}
	str[insertpos] = '\0';
}
void ferite_prepare_parser_cr( char *script, int strip )
{
	FE_ENTER_FUNCTION;
	if( strip ) {
		ferite_parser_script_normalise( script );
	}
	ferite_fp_state = yy_scan_string( script );
	ferite_scanner_in_expression = FE_FALSE;
	ferite_last_token_alloc = NULL;
	BEGIN( S_SCRIPTING );
	FE_LEAVE_FUNCTION(NOWT);
}
void ferite_prepare_parser( char *script ) {
	ferite_prepare_parser_cr( script, FE_TRUE );
}

void ferite_scanner_parse_macro( char *m )
{
	FE_ENTER_FUNCTION;
	ferite_save_lexer();
	ferite_save_state->is_macro = FE_TRUE;

	ferite_fp_state = yy_scan_string( m );
	ferite_scanner_in_expression = FE_FALSE;
	ferite_last_token_alloc = NULL;
	ferite_set_filename( "<macro-expansion>" );
	FE_LEAVE_FUNCTION(NOWT);
}
int ferite_scanner_end_macro()
{
	FE_ENTER_FUNCTION;
	if( ferite_scanner_stack->stack_ptr > 0 ) {
		ferite_save_state = ferite_stack_top( FE_NoScript, ferite_scanner_stack );
		if( ferite_save_state->is_macro ) {
			printf("Checking that we are at the end of a macro expansion\n");
			ferite_restore_lexer();
			FE_LEAVE_FUNCTION(FE_TRUE);
		}
	}
	FE_LEAVE_FUNCTION(FE_FALSE);
}
void ferite_scanner_start_directive_list()
{
	FE_ENTER_FUNCTION;
	yy_push_state( S_DIRECTIVE_EXPR );
	FE_LEAVE_FUNCTION(NOWT);
}
void ferite_scanner_stop_directive_list()
{
	FE_ENTER_FUNCTION;
	yy_pop_state();
	FE_LEAVE_FUNCTION(NOWT);
}
void ferite_scanner_go_native()
{
	FE_ENTER_FUNCTION;
	if( ferite_scanner_file != NULL )
		strcpy( ferite_current_native_block_file, ferite_scanner_file );
	ferite_current_native_block_line = ferite_scanner_lineno;
	if( ferite_cstring_buffer != NULL ) {
		ferite_buffer_delete( FE_NoScript, ferite_cstring_buffer );
		ferite_cstring_buffer = NULL;
	}
	yy_push_state( S_NATIVE_CODE );
	FE_LEAVE_FUNCTION(NOWT);
}

int  ferite_scanner_block_read( char current_character, char lhs, char rhs, char early_stop, int return_token, void *_feplval )
{
	YYSTYPE *feplval = _feplval;
	
	FE_ENTER_FUNCTION;
	
	if( ferite_cstring_buffer == NULL ) { /* Set things up */
		ferite_cstrptr = ferite_cstring_buf;
		ferite_native_code_bracket_depth = 0;
		ferite_native_code_has_started = 0;
		ferite_scanner_buffer_counter = 0;
		ferite_cstring_buffer = ferite_buffer_new( FE_NoScript, 0 );
	}
	
	if( ferite_scanner_buffer_counter == (BUFFER_LENGTH-1) )
	{
		 *ferite_cstrptr = '\0';
		ferite_buffer_add_str( FE_NoScript, ferite_cstring_buffer, ferite_cstring_buf );
		ferite_cstrptr = ferite_cstring_buf;
		ferite_scanner_buffer_counter = 0;
	}

	*ferite_cstrptr++ = current_character;
	ferite_scanner_buffer_counter++;

	if( current_character == lhs )
	{
		ferite_native_code_bracket_depth++;
		if( ferite_native_code_has_started == FE_FALSE )
		{
			ferite_native_code_has_started = FE_TRUE;
			*ferite_cstrptr--;
		}
	}
	else if( current_character == rhs )
		ferite_native_code_bracket_depth--;

	if( early_stop && current_character == early_stop && !ferite_native_code_has_started )
	{
		feplval->text = fcalloc_ngc(4, sizeof(char));
		sprintf( feplval->text, "%c%c", lhs, rhs );
		yy_pop_state();
		FE_LEAVE_FUNCTION(return_token);
	}
	else if( ferite_native_code_bracket_depth == 0 && ferite_native_code_has_started )
	{
		*ferite_cstrptr--;
		*ferite_cstrptr = '\0'; /* we always terminate in this case */
		ferite_buffer_add_str( FE_NoScript, ferite_cstring_buffer, ferite_cstring_buf );

		feplval->text = ferite_buffer_get( FE_NoScript, ferite_cstring_buffer, NULL );
		ferite_buffer_delete( FE_NoScript, ferite_cstring_buffer );
		ferite_cstring_buffer = NULL;

		yy_pop_state();
		FE_LEAVE_FUNCTION(return_token);
	}
	FE_LEAVE_FUNCTION(0)
}
void ferite_scanner_want_dot_label()
{
	FE_ENTER_FUNCTION;
	yy_push_state( S_WANT_DOT_LABEL );
	FE_LEAVE_FUNCTION(NOWT);
}

void ferite_scanner_stop_dot_label()
{
	FE_ENTER_FUNCTION;
	yy_pop_state();
	FE_LEAVE_FUNCTION(NOWT);
}
void ferite_scanner_want_typing_label()
{
	FE_ENTER_FUNCTION;
	yy_push_state( S_TYPING );
	FE_LEAVE_FUNCTION(NOWT);
}

void ferite_scanner_stop_typing_label()
{
	FE_ENTER_FUNCTION;
	yy_pop_state();
	FE_LEAVE_FUNCTION(NOWT);
}
void ferite_scanner_want_end_hint()
{
	FE_ENTER_FUNCTION;
	yy_push_state( S_GRTRTHAN );
	FE_LEAVE_FUNCTION(NOWT);
}

void ferite_scanner_stop_end_hint()
{
	FE_ENTER_FUNCTION;
	yy_pop_state();
	FE_LEAVE_FUNCTION(NOWT);
}

void ferite_clean_parser()
{
	FE_ENTER_FUNCTION;

	if( ferite_scanner_stack != NULL && ferite_scanner_stack->stack_ptr == 0 ) {
		FUD(( "Cleaning Parser: %p::%d depth\n", ferite_scanner_stack, ferite_scanner_stack->stack_ptr ));
		FUD(( "Cleaning up for good.\n" ));
		ferite_delete_stack( NULL, ferite_scanner_stack );
		ferite_scanner_stack = NULL;
	}
	if( ferite_scanner_file )
		ffree_ngc( ferite_scanner_file );
	ferite_scanner_lineno = 1;
	if( ferite_last_token_alloc != NULL ) {
		ffree_ngc( ferite_last_token_alloc );
		ferite_last_token_alloc = NULL;
	}
	if( ferite_cstring_buffer != NULL ) {
		ferite_buffer_delete( FE_NoScript, ferite_cstring_buffer );
		ferite_cstring_buffer = NULL;
	}
	if( ferite_compiled_lengths_stack ) {
		ferite_delete_stack( NULL, ferite_compiled_lengths_stack );
		ferite_compiled_lengths_stack = NULL;
	}
	yy_delete_buffer(YY_CURRENT_BUFFER);
	FE_LEAVE_FUNCTION( NOWT );
}

char *last_yytext_ptr = NULL;

void ferite_save_lexer()
{
	FE_ENTER_FUNCTION;

	if(	ferite_scanner_stack ==	NULL )
		ferite_scanner_stack = ferite_create_stack( NULL, FE_SCANNER_STACK_SIZE	);

	ferite_save_state =	fmalloc_ngc( sizeof( FeriteLexState	) );
	ferite_save_state->buffer_state = YY_CURRENT_BUFFER;
	ferite_save_state->in = yyin;
	ferite_save_state->state = YYSTATE;
	ferite_save_state->lineno =	ferite_scanner_lineno;
	ferite_save_state->filename	= ferite_scanner_file;
	ferite_save_state->is_macro = FE_FALSE;

	ferite_stack_push( FE_NoScript, ferite_scanner_stack, ferite_save_state );

	ferite_scanner_lineno = 1;
	last_yytext_ptr = NULL;
	FE_LEAVE_FUNCTION( NOWT );
}

void ferite_restore_lexer()
{
	YY_BUFFER_STATE original_buffer_state;

	FE_ENTER_FUNCTION;
	original_buffer_state = YY_CURRENT_BUFFER;
	ferite_save_state = ferite_stack_pop( FE_NoScript, ferite_scanner_stack );
	yy_switch_to_buffer( ferite_save_state->buffer_state );
	yy_flush_buffer( original_buffer_state );
	yy_delete_buffer( original_buffer_state );
	yyin = ferite_save_state->in;
	BEGIN( ferite_save_state->state );
	ferite_scanner_lineno = ferite_save_state->lineno;

	if( ferite_scanner_file )
		ffree_ngc( ferite_scanner_file );
	ferite_scanner_file = ferite_save_state->filename;
	ffree_ngc( ferite_save_state );
	last_yytext_ptr = NULL;
	FE_LEAVE_FUNCTION( NOWT );
}

size_t ferite_lexer_offset( char *name, int start ) {
	YY_BUFFER_STATE buffer_state = NULL;
	size_t delta = 0;
	
	FE_ENTER_FUNCTION;
	
	if( !ferite_compiled_lengths_stack )
		ferite_compiled_lengths_stack = ferite_create_stack( NULL, 10 );
		
	buffer_state = YY_CURRENT_BUFFER;

	if( !start ) {
		last_yytext_ptr = ferite_stack_pop( NULL, ferite_compiled_lengths_stack);
		delta = ((size_t)(yytext_ptr)) - ((size_t)(last_yytext_ptr));
		FUD(("<-- %s: Length %d [%c -> %c:%d]\n", name, (int)delta, last_yytext_ptr[0], yytext_ptr[0], yytext_ptr[0]));
		if( yytext_ptr[0] ) {
			delta++;
		}
	} else {
		FUD(("--> %s\n", name));
		ferite_stack_push( NULL, ferite_compiled_lengths_stack, yytext_ptr );
	}
	
	FE_LEAVE_FUNCTION( delta );
}

void ferite_lexer_jump( size_t amount ) {
	if( amount > 0 ) {
		yyless(amount);
	}
	unput(';');
	yy_push_state(S_KRIS_KROSSLL_MAKE_YOU_JUMP_JUMP);
}

void ferite_lexer_fix_native_blocks() {
	unput( '{' );
}
