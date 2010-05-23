/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse fepparse
#define yylex   feplex
#define yyerror feperror
#define yylval  feplval
#define yychar  fepchar
#define yydebug fepdebug
#define yynerrs fepnerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_BINARY_XOR_ASSIGN = 258,
     T_BINARY_OR_ASSIGN = 259,
     T_BINARY_AND_ASSIGN = 260,
     T_RIGHT_SHIFT_ASSIGN = 261,
     T_LEFT_SHIFT_ASSIGN = 262,
     T_DIVIDE_ASSIGN = 263,
     T_MULT_ASSIGN = 264,
     T_MINUS_ASSIGN = 265,
     T_PLUS_ASSIGN = 266,
     T_ASSIGN = 267,
     T_LOGICAL_OR = 268,
     T_RARROW = 269,
     T_LOGICAL_AND = 270,
     T_BINARY_OR = 271,
     T_BINARY_AND = 272,
     T_BINARY_XOR = 273,
     T_NOTEQUAL = 274,
     T_GRTRTHAN = 275,
     T_LESSTHAN = 276,
     T_GRTRTHAN_EQUALS = 277,
     T_LESSTHAN_EQUALS = 278,
     T_EQUALS = 279,
     T_INSTANCEOF = 280,
     T_ISA = 281,
     T_RIGHT_SHIFT = 282,
     T_LEFT_SHIFT = 283,
     T_MINUS = 284,
     T_PLUS = 285,
     T_MODULUS = 286,
     T_DIVIDE = 287,
     T_MULT = 288,
     T_NOT_OPERATOR = 289,
     T_DECR = 290,
     T_INCR = 291,
     T_UPLUS = 292,
     T_UMINUS = 293,
     T_OBJOP = 294,
     T_NEW = 295,
     T_IF = 296,
     T_ELSE = 297,
     T_INCLUDE = 298,
     T_EVAL = 299,
     T_FUNCTION = 300,
     T_FOR = 301,
     T_WHILE = 302,
     T_DO = 303,
     T_SWITCH = 304,
     T_MONITOR = 305,
     T_HANDLE = 306,
     T_CASE = 307,
     T_DEFAULT = 308,
     T_USES = 309,
     T_GLOBAL = 310,
     T_RETURN = 311,
     T_FINAL = 312,
     T_NAMESPACE = 313,
     T_CLASS = 314,
     T_EXTENDS = 315,
     T_BREAK = 316,
     T_CONTINUE = 317,
     T_STATIC = 318,
     T_MULTIPLE_ARGS = 319,
     T_MODIFIES = 320,
     T_NATIVE = 321,
     T_MODULE_INIT = 322,
     T_MODULE_DEINIT = 323,
     T_MODULE_HEADER = 324,
     T_MODULE_FLAGS = 325,
     T_SLICE = 326,
     T_ATOMIC = 327,
     T_MODULE_REGISTER = 328,
     T_MODULE_UNREGISTER = 329,
     T_FOREACH = 330,
     T_ARGS = 331,
     T_PRIVATE = 332,
     T_PROTECTED = 333,
     T_PUBLIC = 334,
     T_ABSTRACT = 335,
     T_CLOSURE = 336,
     T_USING = 337,
     T_DELIVER = 338,
     T_RECIPIENT = 339,
     T_PROTOCOL = 340,
     T_IMPLEMENTS = 341,
     T_SELF = 342,
     T_SUPER = 343,
     T_CONSTRUCTOR = 344,
     T_DESTRUCTOR = 345,
     T_VAR_NUMBER = 346,
     T_VAR_STRING = 347,
     T_VAR_VOID = 348,
     T_VAR_OBJECT = 349,
     T_VAR_ARRAY = 350,
     T_VAR_BOOLEAN = 351,
     T_VAR_NUMBER_LNG = 352,
     T_VAR_NUMBER_DBL = 353,
     T_DIRECTIVE = 354,
     T_RAISE = 355,
     T_CURRENT_LINE = 356,
     T_CURRENT_FILE = 357,
     T_EXPANSION = 358,
     T_FUNCTION_JUMP = 359,
     T_RET_UNDEF = 360,
     T_QSTRING = 361,
     T_LABEL = 362,
     T_OBJPROPERTY = 363,
     T_NATIVE_CODE_BLOCK = 364,
     T_DOT_LABEL = 365,
     T_DIRECTIVE_LABEL = 366,
     T_NNUMBER = 367,
     T_TRUE = 368,
     T_FALSE = 369,
     T_RNUMBER = 370
   };
#endif
/* Tokens.  */
#define T_BINARY_XOR_ASSIGN 258
#define T_BINARY_OR_ASSIGN 259
#define T_BINARY_AND_ASSIGN 260
#define T_RIGHT_SHIFT_ASSIGN 261
#define T_LEFT_SHIFT_ASSIGN 262
#define T_DIVIDE_ASSIGN 263
#define T_MULT_ASSIGN 264
#define T_MINUS_ASSIGN 265
#define T_PLUS_ASSIGN 266
#define T_ASSIGN 267
#define T_LOGICAL_OR 268
#define T_RARROW 269
#define T_LOGICAL_AND 270
#define T_BINARY_OR 271
#define T_BINARY_AND 272
#define T_BINARY_XOR 273
#define T_NOTEQUAL 274
#define T_GRTRTHAN 275
#define T_LESSTHAN 276
#define T_GRTRTHAN_EQUALS 277
#define T_LESSTHAN_EQUALS 278
#define T_EQUALS 279
#define T_INSTANCEOF 280
#define T_ISA 281
#define T_RIGHT_SHIFT 282
#define T_LEFT_SHIFT 283
#define T_MINUS 284
#define T_PLUS 285
#define T_MODULUS 286
#define T_DIVIDE 287
#define T_MULT 288
#define T_NOT_OPERATOR 289
#define T_DECR 290
#define T_INCR 291
#define T_UPLUS 292
#define T_UMINUS 293
#define T_OBJOP 294
#define T_NEW 295
#define T_IF 296
#define T_ELSE 297
#define T_INCLUDE 298
#define T_EVAL 299
#define T_FUNCTION 300
#define T_FOR 301
#define T_WHILE 302
#define T_DO 303
#define T_SWITCH 304
#define T_MONITOR 305
#define T_HANDLE 306
#define T_CASE 307
#define T_DEFAULT 308
#define T_USES 309
#define T_GLOBAL 310
#define T_RETURN 311
#define T_FINAL 312
#define T_NAMESPACE 313
#define T_CLASS 314
#define T_EXTENDS 315
#define T_BREAK 316
#define T_CONTINUE 317
#define T_STATIC 318
#define T_MULTIPLE_ARGS 319
#define T_MODIFIES 320
#define T_NATIVE 321
#define T_MODULE_INIT 322
#define T_MODULE_DEINIT 323
#define T_MODULE_HEADER 324
#define T_MODULE_FLAGS 325
#define T_SLICE 326
#define T_ATOMIC 327
#define T_MODULE_REGISTER 328
#define T_MODULE_UNREGISTER 329
#define T_FOREACH 330
#define T_ARGS 331
#define T_PRIVATE 332
#define T_PROTECTED 333
#define T_PUBLIC 334
#define T_ABSTRACT 335
#define T_CLOSURE 336
#define T_USING 337
#define T_DELIVER 338
#define T_RECIPIENT 339
#define T_PROTOCOL 340
#define T_IMPLEMENTS 341
#define T_SELF 342
#define T_SUPER 343
#define T_CONSTRUCTOR 344
#define T_DESTRUCTOR 345
#define T_VAR_NUMBER 346
#define T_VAR_STRING 347
#define T_VAR_VOID 348
#define T_VAR_OBJECT 349
#define T_VAR_ARRAY 350
#define T_VAR_BOOLEAN 351
#define T_VAR_NUMBER_LNG 352
#define T_VAR_NUMBER_DBL 353
#define T_DIRECTIVE 354
#define T_RAISE 355
#define T_CURRENT_LINE 356
#define T_CURRENT_FILE 357
#define T_EXPANSION 358
#define T_FUNCTION_JUMP 359
#define T_RET_UNDEF 360
#define T_QSTRING 361
#define T_LABEL 362
#define T_OBJPROPERTY 363
#define T_NATIVE_CODE_BLOCK 364
#define T_DOT_LABEL 365
#define T_DIRECTIVE_LABEL 366
#define T_NNUMBER 367
#define T_TRUE 368
#define T_FALSE 369
#define T_RNUMBER 370




/* Copy the first part of user declarations.  */
#line 1 "ferite_parser.y"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 133 "ferite_parser.y"
{
	char   *text;
	int	    inumber;
	double  fnumber;
}
/* Line 193 of yacc.c.  */
#line 472 "ferite_parser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 485 "ferite_parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  49
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1746

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  126
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  436
/* YYNRULES -- Number of states.  */
#define YYNSTATES  648

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   370

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     123,   124,     2,     2,     3,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   122,   117,
       2,     2,     2,   125,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   118,     2,   119,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   120,     2,   121,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     8,    10,    13,    14,    16,    18,
      21,    23,    25,    27,    29,    31,    33,    36,    37,    41,
      42,    46,    47,    51,    52,    56,    57,    61,    62,    66,
      70,    72,    74,    78,    79,    81,    83,    85,    87,    89,
      91,    93,    95,    97,    99,   101,   103,   105,   107,   109,
     111,   115,   118,   121,   122,   128,   129,   134,   135,   137,
     139,   141,   143,   146,   147,   153,   154,   160,   162,   164,
     166,   168,   170,   172,   174,   176,   178,   180,   182,   183,
     185,   188,   191,   194,   197,   199,   200,   204,   206,   208,
     209,   213,   214,   218,   219,   224,   226,   228,   230,   231,
     237,   238,   240,   241,   247,   249,   250,   252,   256,   258,
     260,   264,   267,   271,   272,   274,   276,   279,   283,   286,
     288,   290,   292,   294,   299,   300,   302,   304,   307,   308,
     311,   315,   318,   320,   322,   324,   325,   330,   331,   335,
     336,   339,   341,   345,   346,   349,   355,   360,   364,   366,
     368,   371,   374,   376,   379,   380,   382,   383,   386,   391,
     393,   394,   399,   402,   405,   407,   409,   413,   417,   421,
     425,   429,   431,   435,   437,   441,   443,   445,   446,   448,
     450,   452,   454,   456,   458,   460,   462,   463,   465,   467,
     470,   472,   473,   476,   480,   482,   483,   485,   487,   489,
     491,   493,   495,   497,   499,   501,   503,   505,   507,   509,
     511,   512,   513,   514,   521,   522,   523,   530,   531,   532,
     538,   539,   541,   545,   550,   551,   557,   558,   564,   566,
     568,   570,   571,   573,   574,   576,   578,   581,   583,   585,
     587,   589,   591,   594,   597,   599,   601,   604,   607,   610,
     614,   616,   618,   620,   623,   624,   628,   629,   630,   631,
     632,   643,   644,   652,   654,   656,   657,   658,   662,   663,
     672,   675,   676,   678,   680,   683,   684,   690,   691,   692,
     697,   698,   699,   703,   704,   706,   708,   710,   712,   713,
     714,   715,   716,   717,   718,   734,   735,   736,   744,   745,
     754,   756,   758,   760,   762,   764,   766,   768,   770,   772,
     774,   776,   778,   780,   782,   789,   790,   797,   803,   808,
     813,   817,   820,   823,   824,   827,   829,   832,   834,   837,
     839,   842,   845,   847,   854,   856,   858,   860,   862,   866,
     867,   871,   872,   873,   878,   881,   885,   889,   893,   897,
     901,   905,   909,   913,   917,   921,   925,   929,   933,   937,
     941,   945,   946,   951,   952,   957,   958,   963,   964,   969,
     970,   975,   976,   981,   982,   987,   988,   993,   994,   999,
    1000,  1005,  1011,  1014,  1017,  1020,  1023,  1026,  1029,  1032,
    1037,  1042,  1046,  1050,  1054,  1058,  1060,  1062,  1065,  1067,
    1070,  1072,  1074,  1078,  1083,  1084,  1085,  1095,  1096,  1099,
    1104,  1105,  1112,  1113,  1120,  1121,  1128,  1133,  1134,  1135,
    1138,  1140,  1144,  1145,  1148,  1149,  1154,  1155,  1157,  1159,
    1163,  1166,  1167,  1170,  1171,  1176,  1177
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     127,     0,    -1,    -1,   130,   128,   129,    -1,   105,    -1,
     217,   238,    -1,    -1,   131,    -1,   132,    -1,   131,   132,
      -1,   140,    -1,   156,    -1,   205,    -1,   197,    -1,   181,
      -1,   133,    -1,   215,   169,    -1,    -1,    68,   134,   110,
      -1,    -1,    69,   135,   110,    -1,    -1,    74,   136,   110,
      -1,    -1,    75,   137,   110,    -1,    -1,    70,   138,   110,
      -1,    -1,    71,   139,   110,    -1,    55,   142,   117,    -1,
     107,    -1,   141,    -1,   142,     3,   141,    -1,    -1,   145,
      -1,   107,    -1,   108,    -1,   116,    -1,   113,    -1,    92,
      -1,    97,    -1,    93,    -1,    94,    -1,    95,    -1,    96,
      -1,    60,    -1,    59,    -1,   114,    -1,   115,    -1,   144,
      -1,   145,     3,   144,    -1,   145,   144,    -1,   147,   117,
      -1,    -1,   118,   108,   148,   143,   119,    -1,    -1,   104,
     154,   150,   110,    -1,    -1,   153,    -1,   108,    -1,   112,
      -1,   152,    -1,   153,   152,    -1,    -1,   118,   108,   155,
     151,   119,    -1,    -1,    56,   157,   120,   230,   121,    -1,
      92,    -1,    98,    -1,    99,    -1,    93,    -1,    94,    -1,
      95,    -1,    96,    -1,    97,    -1,   158,    -1,    60,    -1,
      59,    -1,    -1,    46,    -1,    46,   108,    -1,   100,   108,
      -1,   160,    90,    -1,   160,    91,    -1,   161,    -1,    -1,
     159,   164,   221,    -1,   111,    -1,   106,    -1,    -1,   122,
     166,   163,    -1,    -1,   123,   175,   124,    -1,    -1,   120,
     217,   238,   121,    -1,   110,    -1,   245,    -1,   105,    -1,
      -1,   162,   167,   165,   170,   168,    -1,    -1,    18,    -1,
      -1,   159,   173,   221,   171,   108,    -1,    65,    -1,    -1,
     176,    -1,   176,     3,   174,    -1,   174,    -1,   172,    -1,
     176,     3,   172,    -1,    59,   108,    -1,    59,    66,   108,
      -1,    -1,   179,    -1,   180,    -1,   179,   180,    -1,   215,
     227,   117,    -1,   215,   169,    -1,   205,    -1,   197,    -1,
     181,    -1,   146,    -1,   177,   120,   178,   121,    -1,    -1,
     183,    -1,   184,    -1,   183,   184,    -1,    -1,   185,   186,
      -1,   215,   227,   117,    -1,   215,   169,    -1,   146,    -1,
     149,    -1,   108,    -1,    -1,   108,    61,   188,   111,    -1,
      -1,    66,   190,   111,    -1,    -1,   192,   111,    -1,   191,
      -1,   193,     3,   191,    -1,    -1,    87,   193,    -1,   187,
     194,   120,   182,   121,    -1,   189,   120,   182,   121,    -1,
     213,    60,   198,    -1,   195,    -1,   196,    -1,    86,   108,
      -1,   215,   169,    -1,   200,    -1,   201,   200,    -1,    -1,
     201,    -1,    -1,   204,   202,    -1,   199,   120,   203,   121,
      -1,   108,    -1,    -1,   108,    13,   207,   281,    -1,    30,
     113,    -1,    31,   113,    -1,   113,    -1,   108,    -1,   108,
      13,   208,    -1,   108,    13,   116,    -1,   108,    13,   107,
      -1,   108,    13,   114,    -1,   108,    13,   115,    -1,   206,
      -1,   210,     3,   206,    -1,   209,    -1,   211,     3,   209,
      -1,    58,    -1,    81,    -1,    -1,   212,    -1,    58,    -1,
      64,    -1,    73,    -1,    67,    -1,    78,    -1,    79,    -1,
      80,    -1,    -1,   216,    -1,   214,    -1,   216,   214,    -1,
     218,    -1,    -1,   224,   117,    -1,   218,   224,   117,    -1,
      58,    -1,    -1,   111,    -1,   107,    -1,   116,    -1,   113,
      -1,    92,    -1,    98,    -1,    99,    -1,    93,    -1,    94,
      -1,    95,    -1,    96,    -1,    97,    -1,    60,    -1,    59,
      -1,    -1,    -1,    -1,    22,   222,   220,   223,   221,    21,
      -1,    -1,    -1,   219,   158,   225,   221,   226,   210,    -1,
      -1,    -1,   158,   228,   221,   229,   211,    -1,    -1,   231,
      -1,   215,   227,   117,    -1,   231,   215,   227,   117,    -1,
      -1,   120,   233,   217,   238,   121,    -1,    -1,   120,   235,
     217,   239,   121,    -1,   245,    -1,   232,    -1,   238,    -1,
      -1,   243,    -1,    -1,   240,    -1,   241,    -1,   240,   241,
      -1,   244,    -1,   232,    -1,   245,    -1,   232,    -1,   242,
      -1,   243,   242,    -1,   281,   117,    -1,   253,    -1,   271,
      -1,   246,   117,    -1,    62,   117,    -1,    63,   117,    -1,
     101,   281,   117,    -1,   244,    -1,   117,    -1,    57,    -1,
      57,   281,    -1,    -1,   123,   108,   124,    -1,    -1,    -1,
      -1,    -1,    51,   249,   232,   250,    52,   251,   247,   234,
     252,   266,    -1,    -1,    42,   123,   281,   124,   254,   236,
     255,    -1,   248,    -1,   257,    -1,    -1,    -1,    43,   256,
     236,    -1,    -1,    50,   258,   123,   281,   124,   120,   259,
     121,    -1,   260,   264,    -1,    -1,   261,    -1,   262,    -1,
     261,   262,    -1,    -1,    53,   281,   263,   122,   237,    -1,
      -1,    -1,    54,   122,   265,   238,    -1,    -1,    -1,    43,
     267,   236,    -1,    -1,   281,    -1,   268,    -1,   224,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,   123,
     272,   269,   273,   117,   274,   268,   275,   117,   276,   268,
     277,   124,   236,    -1,    -1,    -1,    48,   278,   123,   281,
     124,   279,   236,    -1,    -1,    49,   280,   236,    48,   123,
     281,   124,   117,    -1,   270,    -1,   283,    -1,   282,    -1,
     298,    -1,   107,    -1,   114,    -1,   115,    -1,   113,    -1,
     116,    -1,   102,    -1,   103,    -1,   108,    -1,    88,    -1,
      89,    -1,   283,   118,   281,    72,   281,   119,    -1,    -1,
     283,   118,    72,   284,   281,   119,    -1,   283,   118,   281,
      72,   119,    -1,   283,   118,    72,   119,    -1,   283,   118,
     281,   119,    -1,   283,   118,   119,    -1,   283,   286,    -1,
     282,   286,    -1,    -1,   285,   286,    -1,   290,    -1,   286,
     290,    -1,   289,    -1,   287,   289,    -1,   108,    -1,   288,
     287,    -1,    40,   109,    -1,   289,    -1,    40,   109,   123,
     317,   124,   312,    -1,   108,    -1,   172,    -1,   174,    -1,
     291,    -1,   292,     3,   291,    -1,    -1,   123,   292,   124,
      -1,    -1,    -1,   295,   293,   296,   232,    -1,    82,   294,
      -1,   281,    31,   281,    -1,   281,    30,   281,    -1,   281,
      34,   281,    -1,   281,    33,   281,    -1,   281,    32,   281,
      -1,   281,    25,   281,    -1,   281,    24,   281,    -1,   281,
      23,   281,    -1,   281,    22,   281,    -1,   281,    21,   281,
      -1,   281,    20,   281,    -1,   281,    29,   281,    -1,   281,
      28,   281,    -1,   281,    17,   281,    -1,   281,    18,   281,
      -1,   281,    19,   281,    -1,    -1,   283,    13,   299,   281,
      -1,    -1,   283,    12,   300,   281,    -1,    -1,   283,    11,
     301,   281,    -1,    -1,   283,    10,   302,   281,    -1,    -1,
     283,     9,   303,   281,    -1,    -1,   283,     8,   304,   281,
      -1,    -1,   283,     7,   305,   281,    -1,    -1,   283,     6,
     306,   281,    -1,    -1,   283,     5,   307,   281,    -1,    -1,
     283,     4,   308,   281,    -1,    41,   288,   123,   317,   124,
      -1,    31,   281,    -1,    30,   281,    -1,   283,    37,    -1,
      37,   283,    -1,   283,    36,    -1,    36,   283,    -1,    35,
     281,    -1,    45,   123,   281,   124,    -1,    44,   123,   281,
     124,    -1,   281,    27,   158,    -1,   281,    26,   288,    -1,
      77,   123,   124,    -1,    85,   123,   124,    -1,   309,    -1,
     322,    -1,   322,   286,    -1,   313,    -1,   313,   286,    -1,
     328,    -1,   297,    -1,   123,   281,   124,    -1,   123,   281,
     124,   286,    -1,    -1,    -1,   123,   281,   125,   310,   281,
     122,   311,   281,   124,    -1,    -1,    83,   294,    -1,    83,
      85,   123,   124,    -1,    -1,   108,   314,   123,   317,   124,
     312,    -1,    -1,    89,   315,   123,   317,   124,   312,    -1,
      -1,    88,   316,   123,   317,   124,   312,    -1,    84,   123,
     317,   124,    -1,    -1,    -1,   318,   319,    -1,   320,    -1,
     319,     3,   320,    -1,    -1,   321,   281,    -1,    -1,   118,
     323,   324,   119,    -1,    -1,   325,    -1,   326,    -1,   325,
       3,   326,    -1,   281,   327,    -1,    -1,    15,   281,    -1,
      -1,   281,    16,   329,   281,    -1,    -1,   281,    14,   330,
     281,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   182,   182,   182,   193,   194,   203,   205,   209,   210,
     214,   215,   216,   217,   218,   219,   220,   224,   224,   226,
     226,   228,   228,   230,   230,   232,   232,   234,   234,   239,
     243,   247,   248,   251,   253,   257,   261,   265,   267,   269,
     272,   275,   278,   281,   284,   287,   290,   293,   295,   300,
     301,   302,   306,   310,   310,   348,   348,   354,   356,   360,
     361,   369,   370,   375,   374,   389,   389,   395,   396,   397,
     398,   399,   400,   401,   402,   406,   407,   408,   411,   413,
     417,   418,   419,   421,   426,   454,   454,   461,   469,   477,
     477,   481,   511,   515,   519,   530,   535,   544,   551,   548,
     564,   566,   570,   570,   582,   588,   590,   591,   592,   596,
     597,   601,   602,   605,   607,   611,   612,   616,   617,   618,
     619,   620,   621,   625,   629,   631,   635,   636,   640,   640,
     645,   646,   647,   648,   652,   658,   658,   672,   672,   685,
     685,   696,   697,   700,   702,   706,   715,   722,   728,   729,
     733,   741,   745,   746,   749,   751,   755,   755,   759,   767,
     778,   778,   798,   799,   800,   804,   811,   831,   851,   873,
     893,   917,   918,   922,   923,   927,   936,   947,   949,   953,
     962,   971,   980,   989,   998,  1007,  1018,  1020,  1024,  1025,
    1029,  1030,  1034,  1035,  1039,  1048,  1052,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1078,  1084,  1089,  1084,  1098,  1102,  1098,  1117,  1121,  1117,
    1130,  1132,  1136,  1137,  1142,  1142,  1148,  1148,  1155,  1156,
    1160,  1163,  1165,  1168,  1172,  1175,  1176,  1179,  1180,  1184,
    1185,  1189,  1190,  1194,  1195,  1196,  1197,  1198,  1199,  1200,
    1204,  1205,  1209,  1216,  1219,  1221,  1230,  1231,  1232,  1234,
    1230,  1239,  1239,  1241,  1242,  1246,  1247,  1247,  1252,  1252,
    1264,  1268,  1270,  1274,  1275,  1281,  1280,  1289,  1291,  1291,
    1298,  1299,  1299,  1304,  1310,  1314,  1315,  1319,  1327,  1331,
    1335,  1336,  1337,  1341,  1327,  1353,  1354,  1353,  1356,  1356,
    1360,  1364,  1365,  1366,  1370,  1378,  1385,  1392,  1399,  1406,
    1412,  1421,  1426,  1427,  1428,  1433,  1433,  1440,  1447,  1457,
    1461,  1471,  1472,  1473,  1473,  1478,  1479,  1483,  1484,  1488,
    1493,  1497,  1509,  1510,  1530,  1531,  1532,  1536,  1537,  1540,
    1542,  1546,  1547,  1546,  1552,  1556,  1560,  1564,  1568,  1572,
    1576,  1580,  1585,  1590,  1594,  1598,  1602,  1607,  1612,  1617,
    1622,  1627,  1627,  1628,  1628,  1629,  1629,  1630,  1630,  1631,
    1631,  1632,  1632,  1633,  1633,  1634,  1634,  1635,  1635,  1636,
    1636,  1637,  1648,  1652,  1657,  1661,  1665,  1669,  1673,  1677,
    1681,  1685,  1692,  1696,  1697,  1698,  1699,  1700,  1701,  1702,
    1703,  1704,  1705,  1706,  1710,  1711,  1710,  1715,  1717,  1718,
    1722,  1722,  1735,  1735,  1746,  1746,  1757,  1771,  1777,  1777,
    1787,  1788,  1792,  1792,  1796,  1796,  1815,  1817,  1821,  1822,
    1826,  1836,  1842,  1846,  1846,  1855,  1855
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "','", "T_BINARY_XOR_ASSIGN",
  "T_BINARY_OR_ASSIGN", "T_BINARY_AND_ASSIGN", "T_RIGHT_SHIFT_ASSIGN",
  "T_LEFT_SHIFT_ASSIGN", "T_DIVIDE_ASSIGN", "T_MULT_ASSIGN",
  "T_MINUS_ASSIGN", "T_PLUS_ASSIGN", "T_ASSIGN", "T_LOGICAL_OR",
  "T_RARROW", "T_LOGICAL_AND", "T_BINARY_OR", "T_BINARY_AND",
  "T_BINARY_XOR", "T_NOTEQUAL", "T_GRTRTHAN", "T_LESSTHAN",
  "T_GRTRTHAN_EQUALS", "T_LESSTHAN_EQUALS", "T_EQUALS", "T_INSTANCEOF",
  "T_ISA", "T_RIGHT_SHIFT", "T_LEFT_SHIFT", "T_MINUS", "T_PLUS",
  "T_MODULUS", "T_DIVIDE", "T_MULT", "T_NOT_OPERATOR", "T_DECR", "T_INCR",
  "T_UPLUS", "T_UMINUS", "T_OBJOP", "T_NEW", "T_IF", "T_ELSE", "T_INCLUDE",
  "T_EVAL", "T_FUNCTION", "T_FOR", "T_WHILE", "T_DO", "T_SWITCH",
  "T_MONITOR", "T_HANDLE", "T_CASE", "T_DEFAULT", "T_USES", "T_GLOBAL",
  "T_RETURN", "T_FINAL", "T_NAMESPACE", "T_CLASS", "T_EXTENDS", "T_BREAK",
  "T_CONTINUE", "T_STATIC", "T_MULTIPLE_ARGS", "T_MODIFIES", "T_NATIVE",
  "T_MODULE_INIT", "T_MODULE_DEINIT", "T_MODULE_HEADER", "T_MODULE_FLAGS",
  "T_SLICE", "T_ATOMIC", "T_MODULE_REGISTER", "T_MODULE_UNREGISTER",
  "T_FOREACH", "T_ARGS", "T_PRIVATE", "T_PROTECTED", "T_PUBLIC",
  "T_ABSTRACT", "T_CLOSURE", "T_USING", "T_DELIVER", "T_RECIPIENT",
  "T_PROTOCOL", "T_IMPLEMENTS", "T_SELF", "T_SUPER", "T_CONSTRUCTOR",
  "T_DESTRUCTOR", "T_VAR_NUMBER", "T_VAR_STRING", "T_VAR_VOID",
  "T_VAR_OBJECT", "T_VAR_ARRAY", "T_VAR_BOOLEAN", "T_VAR_NUMBER_LNG",
  "T_VAR_NUMBER_DBL", "T_DIRECTIVE", "T_RAISE", "T_CURRENT_LINE",
  "T_CURRENT_FILE", "T_EXPANSION", "T_FUNCTION_JUMP", "T_RET_UNDEF",
  "T_QSTRING", "T_LABEL", "T_OBJPROPERTY", "T_NATIVE_CODE_BLOCK",
  "T_DOT_LABEL", "T_DIRECTIVE_LABEL", "T_NNUMBER", "T_TRUE", "T_FALSE",
  "T_RNUMBER", "';'", "'['", "']'", "'{'", "'}'", "':'", "'('", "')'",
  "'?'", "$accept", "script", "@1", "script_body", "script_block_list",
  "non_empty_script_block_list", "script_block", "builder_item", "@2",
  "@3", "@4", "@5", "@6", "@7", "uses_list", "module_item", "module_list",
  "directive_list", "directive_item", "non_empty_directive_list",
  "directive_decl", "directive_decl_with_out_semi_colon", "@8",
  "expansion_decl", "@9", "directive_expansion_capture_mask_arguments",
  "non_empty_directive_mask_list_item", "non_empty_directive_mask_list",
  "directive_expansion_capture_mask", "@10", "global_decl", "@11",
  "var_type", "param_var_type", "optional_function_token", "function_name",
  "function_header_prefix", "function_return_type", "@12",
  "function_optional_return_type", "@13", "function_parameter_declaration",
  "function_body", "function", "@14", "parameter_byref",
  "parameter_define_item", "@15", "parameter_multiple_args",
  "decl_parameter_list", "non_empty_decl_parameter_list",
  "namespace_header", "namespace_block_list",
  "non_empty_namespace_block_list", "namespace_block", "namespace_def",
  "class_block_list", "non_empty_class_block_list", "class_block", "@16",
  "class_block_real", "class_header", "@17", "class_modifies_header",
  "@18", "implements_item", "@19", "implements_list", "class_implements",
  "standard_class", "modifies_class", "class_def", "class_body",
  "protocol_header", "protocol_item", "non_empty_protocol_block_list",
  "protocol_block_list_real", "protocol_block_list", "@20", "protocol_def",
  "variable_define_item", "@21", "namespace_natural_number",
  "namespace_variable_define_item", "variable_define_list",
  "namespace_variable_define_list", "class_modifier",
  "class_modifier_list", "item_modifier", "block_item_modifier_list",
  "non_empty_block_item_modifier_list", "variable_block",
  "non_empty_variable_block", "final_modifier",
  "optional_variable_type_hint_t", "optional_variable_type_hint", "@22",
  "@23", "multiple_variable_item", "@24", "@25",
  "namespace_multiple_variable_item", "@26", "@27",
  "namespace_variable_block", "non_empty_namespace_variable_block",
  "nested_block", "@28", "non_empty_nested_block", "@29", "block",
  "case_block", "statement_block", "error_empty_statement_block",
  "non_empty_error_statement_block",
  "non_empty_error_statement_block_item", "non_empty_statement_block_item",
  "non_empty_statement_block", "statement_without_semicolon", "statement",
  "return_statement", "optional_handle_variable", "monitor_block", "@30",
  "@31", "@32", "@33", "select_statement", "@34", "else_block", "@35",
  "switch_statement", "@36", "switch_case_list", "switch_cases_list",
  "non_empty_switch_cases_list", "switch_case_item", "@37", "default_item",
  "@38", "fix_else_block", "@39", "for_expression", "for_init_expression",
  "foreach_start", "itterative_statement", "@40", "@41", "@42", "@43",
  "@44", "@45", "@46", "@47", "@48", "expression", "cval", "cvar", "@49",
  "@50", "object_list", "object_no_call_list", "nvar",
  "object_property_no_call", "object_property", "closure_argument",
  "non_empty_closure_arguments", "closure_arguments", "closure_body",
  "@51", "@52", "closure_expression", "expression_without_var", "@53",
  "@54", "@55", "@56", "@57", "@58", "@59", "@60", "@61", "@62",
  "question_colon_expr", "@63", "@64", "optional_function_using_clause",
  "function_call", "@65", "@66", "@67", "parameter_list", "@68",
  "non_empty_function_call_parameter_list", "function_call_parameter",
  "@69", "array_in_script", "@70", "array_elements_list",
  "non_empty_array_elements_list", "array_element", "array_element_post",
  "logical_operators", "@71", "@72", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    44,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,    59,    91,    93,
     123,   125,    58,    40,    41,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   126,   128,   127,   129,   129,   130,   130,   131,   131,
     132,   132,   132,   132,   132,   132,   132,   134,   133,   135,
     133,   136,   133,   137,   133,   138,   133,   139,   133,   140,
     141,   142,   142,   143,   143,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   145,
     145,   145,   146,   148,   147,   150,   149,   151,   151,   152,
     152,   153,   153,   155,   154,   157,   156,   158,   158,   158,
     158,   158,   158,   158,   158,   159,   159,   159,   160,   160,
     161,   161,   161,   161,   162,   164,   163,   163,   163,   166,
     165,   165,   167,   167,   168,   168,   168,   168,   170,   169,
     171,   171,   173,   172,   174,   175,   175,   175,   175,   176,
     176,   177,   177,   178,   178,   179,   179,   180,   180,   180,
     180,   180,   180,   181,   182,   182,   183,   183,   185,   184,
     186,   186,   186,   186,   187,   188,   187,   190,   189,   192,
     191,   193,   193,   194,   194,   195,   196,   197,   198,   198,
     199,   200,   201,   201,   202,   202,   204,   203,   205,   206,
     207,   206,   208,   208,   208,   209,   209,   209,   209,   209,
     209,   210,   210,   211,   211,   212,   212,   213,   213,   214,
     214,   214,   214,   214,   214,   214,   215,   215,   216,   216,
     217,   217,   218,   218,   219,   219,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     221,   222,   223,   221,   225,   226,   224,   228,   229,   227,
     230,   230,   231,   231,   233,   232,   235,   234,   236,   236,
     237,   238,   238,   239,   239,   240,   240,   241,   241,   242,
     242,   243,   243,   244,   244,   244,   244,   244,   244,   244,
     245,   245,   246,   246,   247,   247,   249,   250,   251,   252,
     248,   254,   253,   253,   253,   255,   256,   255,   258,   257,
     259,   260,   260,   261,   261,   263,   262,   264,   265,   264,
     266,   267,   266,   268,   268,   269,   269,   270,   272,   273,
     274,   275,   276,   277,   271,   278,   279,   271,   280,   271,
     271,   281,   281,   281,   282,   282,   282,   282,   282,   282,
     282,   283,   283,   283,   283,   284,   283,   283,   283,   283,
     283,   283,   283,   285,   283,   286,   286,   287,   287,   288,
     288,   289,   290,   290,   291,   291,   291,   292,   292,   293,
     293,   295,   296,   294,   297,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   299,   298,   300,   298,   301,   298,   302,   298,   303,
     298,   304,   298,   305,   298,   306,   298,   307,   298,   308,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   310,   311,   309,   312,   312,   312,
     314,   313,   315,   313,   316,   313,   313,   317,   318,   317,
     319,   319,   321,   320,   323,   322,   324,   324,   325,   325,
     326,   327,   327,   329,   328,   330,   328
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     1,     2,     0,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     2,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     3,
       1,     1,     3,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     2,     0,     5,     0,     4,     0,     1,     1,
       1,     1,     2,     0,     5,     0,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       2,     2,     2,     2,     1,     0,     3,     1,     1,     0,
       3,     0,     3,     0,     4,     1,     1,     1,     0,     5,
       0,     1,     0,     5,     1,     0,     1,     3,     1,     1,
       3,     2,     3,     0,     1,     1,     2,     3,     2,     1,
       1,     1,     1,     4,     0,     1,     1,     2,     0,     2,
       3,     2,     1,     1,     1,     0,     4,     0,     3,     0,
       2,     1,     3,     0,     2,     5,     4,     3,     1,     1,
       2,     2,     1,     2,     0,     1,     0,     2,     4,     1,
       0,     4,     2,     2,     1,     1,     3,     3,     3,     3,
       3,     1,     3,     1,     3,     1,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     0,     2,     3,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     0,     0,     6,     0,     0,     6,     0,     0,     5,
       0,     1,     3,     4,     0,     5,     0,     5,     1,     1,
       1,     0,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     2,     2,     2,     3,
       1,     1,     1,     2,     0,     3,     0,     0,     0,     0,
      10,     0,     7,     1,     1,     0,     0,     3,     0,     8,
       2,     0,     1,     1,     2,     0,     5,     0,     0,     4,
       0,     0,     3,     0,     1,     1,     1,     1,     0,     0,
       0,     0,     0,     0,    15,     0,     0,     7,     0,     8,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,     0,     6,     5,     4,     4,
       3,     2,     2,     0,     2,     1,     2,     1,     2,     1,
       2,     2,     1,     6,     1,     1,     1,     1,     3,     0,
       3,     0,     0,     4,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     5,     2,     2,     2,     2,     2,     2,     2,     4,
       4,     3,     3,     3,     3,     1,     1,     2,     1,     2,
       1,     1,     3,     4,     0,     0,     9,     0,     2,     4,
       0,     6,     0,     6,     0,     6,     4,     0,     0,     2,
       1,     3,     0,     2,     0,     4,     0,     1,     1,     3,
       2,     0,     2,     0,     4,     0,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       6,     0,    65,   179,     0,   180,   182,    17,    19,    25,
      27,   181,    21,    23,   183,   184,   185,   176,     0,     0,
       2,     7,     8,    15,    10,    11,     0,    14,    13,     0,
      12,   178,     0,   188,    78,   187,    30,    31,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,   150,     1,
     191,     9,   186,   156,     0,    79,     0,     0,    84,    93,
      16,   179,   189,     0,    29,   186,   112,    18,    20,    26,
      28,    22,    24,   194,     4,     3,   231,   190,     0,     0,
       0,   122,     0,     0,   186,   115,   121,   120,   119,    78,
       0,   186,   137,   134,   143,     0,   148,   149,   147,    80,
      81,    82,    83,   105,    91,    32,     0,     0,   186,   323,
     323,   323,   323,   323,     0,     0,     0,     0,     0,   295,
     298,   268,   256,   252,     0,     0,   287,     0,   341,     0,
       0,   312,   313,   323,   309,   310,   304,   311,   307,   305,
     306,   308,   251,   424,   224,   323,   240,     5,   241,   232,
     250,   239,     0,   263,   244,   264,   300,   245,     0,   302,
     301,     0,   401,   303,   395,   398,   396,   400,     0,    67,
      70,    71,    72,    73,    74,    68,    69,   214,   192,    53,
      52,   123,   116,   217,   118,     0,   158,   152,   186,   157,
      78,     0,   135,   139,     0,   128,    77,    76,   104,    75,
     102,   109,   108,     0,   106,    89,    98,     0,    66,     0,
     383,   382,   388,   312,   313,   311,     0,   387,   385,   329,
       0,   323,   323,   323,   288,     0,   323,     0,     0,   253,
     247,   248,     0,   344,   339,   418,     0,     0,     0,     0,
       0,   323,   191,     0,   242,   246,   435,   433,   323,   323,
     323,   323,   323,   323,   323,   323,   323,     0,     0,   323,
     323,   323,   323,   323,   323,   323,   243,     0,   322,   332,
     325,   379,   377,   375,   373,   371,   369,   367,   365,   363,
     361,   386,   384,   323,   321,   324,   399,   397,   193,   210,
      33,   210,   117,   153,   151,   138,     0,   141,     0,   144,
     128,     0,   128,   126,   186,   210,    92,     0,     0,   323,
     222,     0,     0,   418,   330,   327,     0,     0,     0,   195,
     323,   229,     0,   228,   323,   257,   393,     0,   342,     0,
     422,   394,   418,   418,   249,   418,   431,     0,   427,   428,
     231,   402,   404,   323,   323,   358,   359,   360,   355,   354,
     353,   352,   351,   350,   392,   391,   357,   356,   346,   345,
     349,   348,   347,   331,   326,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   315,   320,     0,   211,   215,
      46,    45,    39,    41,    42,    43,    44,    40,    35,    36,
      38,    47,    48,    37,     0,    49,    34,   218,   136,   140,
     139,     0,   146,   127,     0,   132,   133,   129,    78,   100,
     110,   107,    88,    87,    85,    90,    97,    95,   191,    99,
      96,   223,   331,     0,   328,   261,   390,   389,   286,   285,
     289,   284,     0,     0,     0,     0,   334,   335,   336,   337,
       0,     0,   416,   419,   420,   323,     0,     0,     0,   323,
     430,   425,   323,     0,   403,   323,   436,   434,   418,   380,
     378,   376,   374,   372,   370,   368,   366,   364,   362,   318,
     323,   323,   319,     0,     0,    54,     0,    51,     0,   142,
     145,     0,    55,   131,     0,   101,     0,   210,   231,   381,
     323,     0,   296,   323,     0,   258,     0,   340,   343,   422,
     423,   407,   407,   407,   432,   429,   225,     0,     0,     0,
     317,     0,   209,   208,   200,   203,   204,   205,   206,   207,
     201,   202,   197,   196,   199,   198,   212,   159,   171,   216,
      50,   165,   173,   219,    63,     0,   130,   103,    86,     0,
     265,   290,   323,     0,   271,   254,   338,   421,   341,   415,
     413,   411,   405,   407,   316,   314,   210,   160,     0,     0,
       0,    57,    56,    94,   266,   262,   283,   297,     0,   323,
       0,   277,   272,   273,     0,     0,     0,   408,   323,   333,
       0,   323,   172,     0,     0,   168,   164,   169,   170,   167,
     166,   174,    59,    60,     0,    61,    58,   323,   291,   299,
     275,   269,     0,   270,   274,     0,   226,   259,     0,     0,
     213,   161,   162,   163,    64,    62,   267,     0,     0,   278,
     255,   191,   280,   409,   406,   292,   231,   231,   233,   281,
     260,   283,   276,   230,   279,   238,     0,   234,   235,   237,
     323,   293,   227,   236,   282,     0,   323,   294
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    19,    50,    75,    20,    21,    22,    23,    42,    43,
      46,    47,    44,    45,    24,    37,    38,   394,   395,   396,
      81,    82,   290,   406,   535,   594,   595,   596,   482,   561,
      25,    39,   199,   200,    57,    58,    59,   415,   487,   206,
     308,   104,   419,    60,   309,   486,   437,   305,   438,   203,
     204,    26,    83,    84,    85,    27,   301,   302,   303,   304,
     407,    94,   296,    95,   191,   297,   298,   299,   194,    96,
      97,    28,    98,    29,   187,   188,   189,    90,    91,    30,
     528,   581,   590,   532,   529,   533,    31,    32,    33,    34,
      35,    76,    77,    78,   526,   379,   473,   556,    79,   289,
     474,   185,   291,   478,   107,   108,   146,   242,   607,   621,
     322,   632,   147,   636,   637,   638,   148,   149,   150,   151,
     152,   575,   153,   228,   435,   545,   622,   154,   490,   565,
     597,   155,   227,   570,   571,   572,   573,   618,   603,   627,
     630,   640,   429,   430,   156,   157,   319,   491,   566,   617,
     631,   645,   225,   542,   226,   158,   159,   160,   470,   161,
     284,   314,   220,   269,   270,   439,   440,   328,   233,   234,
     441,   162,   163,   374,   373,   372,   371,   370,   369,   368,
     367,   366,   365,   164,   455,   578,   549,   165,   240,   238,
     237,   329,   330,   443,   444,   445,   166,   241,   337,   338,
     339,   450,   167,   344,   343
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -606
static const yytype_int16 yypact[] =
{
    1519,   -66,  -606,   -12,   -29,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,   -44,    81,
    -606,  1519,  -606,  -606,  -606,  -606,   -35,  -606,  -606,   -22,
    -606,  -606,    51,  -606,   -17,   175,  -606,  -606,    14,     5,
      18,  -606,    17,    20,    24,    26,    27,    38,  -606,  -606,
     354,  -606,   213,  -606,   -28,    50,    53,   -40,  -606,    12,
    -606,  -606,  -606,   -66,  -606,   243,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,  -606,   875,   435,   620,    11,
      55,  -606,    56,    54,   290,  -606,  -606,  -606,  -606,    -4,
      57,   280,  -606,   115,    90,    59,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,   663,    58,  -606,   620,    60,   342,  1428,
    1428,  1428,   735,   735,    76,    63,    68,    77,    78,  -606,
    -606,  -606,  -606,  1260,    71,    85,  -606,    80,  -606,    82,
      83,    86,    89,  1428,  -606,  -606,  -606,    91,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,  1428,  -606,  -606,  -606,   875,
    -606,  -606,    87,  -606,  -606,  -606,  -606,  -606,   757,   176,
      65,   176,  -606,  -606,  -606,   176,   176,  -606,   103,  -606,
    -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,   104,  -606,  -606,   863,  -606,
     -17,   117,  -606,  -606,   110,   111,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,   107,   233,  -606,  -606,   123,  -606,   620,
    -606,  -606,  -606,  -606,  -606,  -606,   176,   -18,   -18,  -606,
     -20,  1428,  1428,  1428,  -606,   118,   966,   124,   129,  1627,
    -606,  -606,   126,  -606,   128,   142,   145,   147,   151,   779,
     152,  1316,   435,   411,  -606,  -606,  -606,  -606,  1428,  1428,
    1428,  1428,  1428,  1428,  1428,  1428,  1428,    76,   620,  1428,
    1428,  1428,  1428,  1428,  1428,  1428,  -606,   170,   176,  -606,
    -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  1204,   176,   176,   176,   176,  -606,   259,
    1102,   259,  -606,  -606,  -606,  -606,   171,  -606,   172,   281,
     111,   164,   166,  -606,   954,   259,  -606,   663,   296,   784,
    -606,   173,   179,   142,   249,  -606,   457,   478,   523,  1148,
    1428,  -606,   250,  -606,  1428,  -606,  -606,   100,  -606,   178,
    -606,  -606,   142,   142,  -606,   142,  1606,   181,   294,  -606,
     875,   176,  -606,  1428,  1428,  1681,  1697,  1712,   487,   487,
     487,   487,   487,   487,   249,  -606,   137,   137,    88,    88,
    -606,  -606,  -606,   180,  -606,  1428,  1428,  1428,  1428,  1428,
    1428,  1428,  1428,  1428,  1428,   185,  -606,   653,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,   186,  -606,   130,  -606,  -606,  -606,
    -606,   187,  -606,  -606,   188,  -606,  -606,  -606,    -4,   291,
    -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,   435,  -606,
    -606,  -606,  -606,   189,  -606,  -606,  -606,  -606,  -606,  -606,
    -606,  1627,   544,   192,   566,   260,  -606,  -606,  -606,  -606,
      25,   129,  -606,   308,  -606,  1428,   193,   194,   200,  1428,
    -606,  -606,  1428,   204,   176,  1428,  1646,  1664,   142,  1627,
    1627,  1627,  1627,  1627,  1627,  1627,  1627,  1627,  1627,  -606,
    1428,  1372,  -606,  1460,   218,  -606,  1102,  -606,   220,  -606,
    -606,   221,  -606,  -606,   215,  -606,   222,   259,   875,  -606,
     966,   216,  -606,  1428,   219,  -606,   100,  -606,  -606,  -606,
    1627,   252,   252,   252,  1627,  -606,  -606,   632,   217,   675,
    -606,   717,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,  -606,  -606,   329,  -606,   348,
    -606,   332,  -606,   349,  -606,   255,  -606,  -606,  -606,   245,
     324,  -606,   966,   587,   319,   254,  -606,  -606,   288,  -606,
    -606,  -606,  -606,   252,  -606,  -606,   259,  -606,   218,     3,
     220,   -51,  -606,  -606,  -606,  -606,  1260,  -606,   261,  1428,
     258,   326,   319,  -606,   273,   262,   263,  -606,  1428,  -606,
     362,  1428,  -606,   274,   283,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,   278,  -606,   -51,   966,  -606,  -606,
    1627,  -606,   276,  -606,  -606,   275,  -606,  -606,   279,   608,
    -606,  1627,  -606,  -606,  -606,  -606,  -606,   287,   297,  -606,
    -606,   435,   367,  -606,  -606,  -606,   875,   875,  1057,  -606,
    -606,  1260,  -606,  -606,  -606,  -606,   292,  1057,  -606,  -606,
     966,  -606,  -606,  -606,  -606,   302,   966,  -606
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -606,  -606,  -606,  -606,  -606,  -606,   395,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,   391,  -606,  -606,  -377,  -606,
     154,  -606,  -606,  -606,  -606,  -606,  -141,  -606,  -606,  -606,
    -606,  -606,   -71,   148,  -606,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,   -81,  -606,  -606,   -90,  -606,   -88,  -606,
    -606,  -606,  -606,  -606,   377,   -21,   162,  -606,   163,  -606,
    -606,  -606,  -606,  -606,  -606,    64,  -606,  -606,  -606,  -606,
    -606,     0,  -606,  -606,   282,  -606,  -606,  -606,  -606,    45,
     -92,  -606,  -606,   -93,  -606,  -606,  -606,  -606,   433,   -26,
    -606,  -236,  -606,  -606,  -606,  -280,  -606,  -606,   -67,  -606,
    -606,   -94,  -606,  -606,  -606,  -606,  -223,  -606,  -606,  -606,
    -474,  -606,  -331,  -606,  -606,  -114,   375,  -606,  -605,  -222,
    -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,  -606,   -47,  -606,  -606,  -606,
    -606,  -606,  -545,  -606,  -606,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,  -109,   -46,    -5,  -606,  -606,
    -112,  -606,   269,  -190,  -241,    42,  -606,  -606,    31,  -606,
    -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,  -606,
    -606,  -606,  -606,  -606,  -606,  -606,  -447,  -606,  -606,  -606,
    -606,  -273,  -606,  -606,   144,  -606,  -606,  -606,  -606,  -606,
     236,  -606,  -606,  -606,  -606
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -427
static const yytype_int16 yytable[] =
{
     210,   211,   212,   321,   323,   325,   340,   177,   184,   453,
     168,   397,   207,   201,   229,   202,   540,    63,   183,   477,
     312,   598,   267,   639,   239,   409,    89,   364,   496,    55,
     315,    86,   639,   583,   584,   183,   243,    40,    92,   106,
     423,    36,    55,   364,   364,   364,   364,   268,  -175,   285,
     101,   102,    87,   286,   287,   550,   551,   592,    89,   446,
     447,   593,   448,    86,    48,   190,   216,   216,   567,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,    41,
      93,    49,   209,    56,    87,    52,   641,   420,   169,   170,
     171,   172,   173,   174,   175,   176,    56,    88,    53,   530,
     283,   281,   282,   313,   268,   267,   579,   217,   218,   294,
     585,    54,   316,   317,   318,   311,   586,   587,   588,   589,
     263,   264,   265,   616,   424,    65,    66,    67,   178,    88,
      68,    64,   336,   476,    69,   103,    70,    71,   183,   345,
     346,   347,   348,   349,   350,   351,   352,   353,    72,   497,
     356,   357,   358,   359,   360,   361,   362,   539,    99,   196,
     197,   100,   190,   179,   315,   198,   644,   261,   262,   263,
     264,   265,   647,   180,   377,   181,   192,   193,   186,   195,
     205,   208,   488,   283,   219,   508,   221,   355,   230,   380,
     381,   222,   169,   170,   171,   172,   173,   174,   175,   176,
     223,   224,   231,   232,   245,   235,   236,   538,   436,  -414,
     431,   432,  -412,   364,  -410,   434,   267,   410,   498,   411,
     288,   292,   382,   383,   384,   385,   386,   387,   295,   454,
     300,   306,  -124,    61,   456,   457,   307,   388,   389,     5,
     310,   320,     6,   390,   391,   392,   393,   324,    11,   144,
     326,   327,   428,    14,    15,    16,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,  -417,   321,   323,   331,
     332,     3,     4,  -177,   333,   335,   580,     5,   408,   363,
       6,   378,   398,   399,   400,   402,    11,  -125,   422,   312,
     421,    14,    15,    16,    17,   633,   634,   452,   433,    18,
     451,    61,   442,   458,   469,   475,   481,     5,   480,   485,
       6,   499,   495,   489,   484,   493,    11,   501,   502,   321,
     323,    14,    15,    16,   503,   506,   527,   483,   531,   534,
     537,    80,   536,   541,  -113,   548,   500,   183,    61,   544,
     504,   553,   557,   336,     5,   559,   507,     6,     3,     4,
    -177,   558,   560,    11,     5,   196,   197,     6,    14,    15,
      16,   509,   511,    11,  -220,   562,   563,   564,    14,    15,
      16,    17,   569,   576,   321,   323,    18,   574,   599,   601,
     602,   605,   606,   610,   543,   628,   608,   612,   169,   170,
     171,   172,   173,   174,   175,   176,   613,   614,   619,   620,
      61,  -154,   412,   623,   625,   635,     5,   413,    80,     6,
     629,  -114,    73,   642,   635,    11,    51,   321,   323,   626,
      14,    15,    16,   321,   323,   246,   646,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,  -195,  -195,  -195,  -195,
    -195,  -195,  -195,  -195,   105,   615,   414,   431,   405,    74,
     600,   182,   401,  -221,   479,   403,   582,   591,    62,   609,
     293,   246,   611,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   246,    73,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   431,   643,   244,   604,   354,  -195,  -195,  -195,
    -195,  -195,  -195,  -195,  -195,   341,   342,   246,   546,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   246,     0,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   577,
     246,   425,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   246,   426,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   246,     0,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   547,     0,     0,   246,   427,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   246,   492,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   505,   246,
     494,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
       0,   568,   169,   170,   171,   172,   173,   174,   175,   176,
       0,     0,   196,   197,     0,   471,     0,     0,   198,     0,
       0,   246,   624,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,     0,     0,   552,   169,   170,   171,   172,   173,
     174,   175,   176,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   472,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,     0,   246,   554,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   109,   110,     0,     0,     0,   111,
     112,   113,     0,   213,   214,   114,   115,     0,   116,   117,
       0,   118,   119,   120,   121,   122,   555,   134,   135,     0,
       0,   123,   136,   215,     0,     0,   124,   125,   138,   139,
     140,   141,     0,     0,     0,     0,     0,     0,     0,     0,
     126,   127,     0,     0,     0,     0,   128,     0,   129,   130,
       0,     0,   131,   132,   266,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   133,   134,   135,     0,   416,
       0,   136,   137,     0,   417,     0,   334,   138,   139,   140,
     141,   142,   143,     0,   418,   109,   110,   145,     0,     0,
     111,   112,   113,     0,     0,  -323,   114,   115,     0,   116,
     117,    61,   118,   119,   120,   121,   122,     5,     0,     0,
       6,     0,   123,     0,     0,     0,    11,   124,   125,     0,
       0,    14,    15,    16,     0,     0,     0,     0,     0,     0,
       0,   126,   127,     0,     0,     0,     0,   128,     0,   129,
     130,     0,     0,   131,   132,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   133,   134,   135,     0,
       0,     0,   136,   137,  -155,     0,     0,     0,   138,   139,
     140,   141,   142,   143,     0,   144,   109,   110,   145,     0,
       0,   111,   112,   113,     0,     0,     0,   114,   115,     0,
     116,   117,    61,   118,   119,   120,   121,   122,     5,     0,
       0,     6,     0,   123,     0,     0,     0,    11,   124,   125,
       0,     0,    14,    15,    16,     0,     0,     0,     0,     0,
       0,     0,   126,   127,     0,     0,     0,     0,   128,     0,
     129,   130,     0,     0,   131,   132,     0,     0,   404,     0,
       0,     0,     0,     0,     0,     0,     0,   133,   134,   135,
       0,     0,    80,   136,   137,     0,     0,     0,     0,   138,
     139,   140,   141,   142,   143,     0,   144,   109,   110,   145,
       0,     0,   111,   112,   113,     0,     0,  -323,   114,   115,
       0,   116,   117,     0,   118,   119,   120,   121,   122,     0,
       0,     0,     0,     0,   123,     0,     0,     0,     0,   124,
     125,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   126,   127,     0,     0,     0,     0,   128,
       0,   129,   130,     0,     0,   131,   132,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   133,   134,
     135,   380,   381,     0,   136,   137,     0,     0,     0,     0,
     138,   139,   140,   141,     0,   143,     0,   144,   109,   110,
     145,     0,     0,   111,   112,   113,     0,     0,  -323,   114,
       0,     0,   116,   117,   382,   383,   384,   385,   386,   387,
       0,     0,     0,     0,     0,     0,    73,     0,     0,   388,
     389,     0,     0,     0,     0,   390,   391,   392,   393,     0,
       0,     0,     0,     0,     0,   127,     0,     0,     0,     0,
     128,     0,   129,   130,   109,   110,   131,   132,     0,   111,
     112,   113,     0,     0,     0,   114,     0,     0,   116,   117,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,   141,  -283,   143,     0,     0,     0,
       0,   145,     0,     0,     0,     0,   375,     0,     0,     0,
       0,   127,     0,     0,     0,     0,   128,     0,   129,   130,
     109,   110,   131,   132,     0,   111,   112,   113,     0,     0,
    -323,   114,     0,     0,   116,   117,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   139,   140,
     141,     0,   143,   376,     0,     0,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   127,     0,     0,
       0,     0,   128,     0,   129,   130,   109,   110,   131,   132,
       0,   111,   112,   113,     0,     0,     0,   114,     0,     0,
     116,   117,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,   141,     0,   143,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,     0,     0,     0,     0,   128,     0,
     129,   130,   109,   110,   131,   132,     0,   111,   112,   113,
       0,     0,     0,   114,     0,     0,   116,   117,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     139,   140,   141,     0,   143,  -426,     0,     0,     0,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   127,
       0,     0,     0,     0,   128,     0,   129,   130,   109,   110,
     131,   132,     0,   111,   112,   113,     0,     0,     0,   114,
       0,     0,   116,   117,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,   141,     0,
     143,   510,     0,     0,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   127,     0,     0,     0,     0,
     128,     0,   129,   130,     0,     0,   131,   132,     0,   512,
     513,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,   141,     0,   143,     0,     0,     0,
       0,   145,   514,   515,   516,   517,   518,   519,   520,   521,
       0,     0,     0,     0,     0,  -186,     0,   522,     0,     0,
       0,   523,     0,   524,     1,     2,   525,     3,     4,  -177,
       0,     0,     0,     5,     0,     0,     6,     7,     8,     9,
      10,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,     0,     0,     0,     0,    18,     0,     0,     0,  -186,
    -186,     0,     0,     0,     0,     0,     0,     0,     0,  -186,
     246,   449,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   246,     0,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265
};

static const yytype_int16 yycheck[] =
{
     109,   110,   111,   226,   226,   228,   242,    78,    89,   340,
      77,   291,   106,   103,   123,   103,   490,     3,    89,   396,
      40,   566,    40,   628,   133,   305,    52,   268,     3,    46,
     220,    52,   637,    30,    31,   106,   145,    66,    66,    65,
     313,   107,    46,   284,   285,   286,   287,   159,    60,   161,
      90,    91,    52,   165,   166,   502,   503,   108,    84,   332,
     333,   112,   335,    84,   108,    91,   112,   113,   542,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,   108,
     108,     0,   108,   100,    84,   120,   631,   309,    92,    93,
      94,    95,    96,    97,    98,    99,   100,    52,   120,   476,
     118,    36,    37,   123,   216,    40,   553,   112,   113,   190,
     107,    60,   221,   222,   223,   209,   113,   114,   115,   116,
      32,    33,    34,   597,   314,   120,   108,   110,   117,    84,
     110,   117,   241,     3,   110,   123,   110,   110,   209,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   110,   124,
     259,   260,   261,   262,   263,   264,   265,   488,   108,    59,
      60,   108,   188,   108,   354,    65,   640,    30,    31,    32,
      33,    34,   646,   117,   283,   121,    61,    87,   121,   120,
     122,   121,   418,   118,   108,   458,   123,   258,   117,    59,
      60,   123,    92,    93,    94,    95,    96,    97,    98,    99,
     123,   123,   117,   123,   117,   123,   123,   487,   108,   123,
     319,   320,   123,   454,   123,   324,    40,   307,   441,   307,
     117,   117,    92,    93,    94,    95,    96,    97,   111,   341,
     120,   124,   121,    58,   343,   344,     3,   107,   108,    64,
     117,   123,    67,   113,   114,   115,   116,   123,    73,   120,
     124,   123,   319,    78,    79,    80,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   124,   490,   490,   124,
     123,    58,    59,    60,   123,   123,   556,    64,   304,   109,
      67,    22,   111,   111,     3,   121,    73,   121,   109,    40,
     117,    78,    79,    80,    81,   626,   627,     3,    48,    86,
     119,    58,   124,   123,   119,   119,   118,    64,   121,    18,
      67,     3,    52,   124,   408,   123,    73,   124,   124,   542,
     542,    78,    79,    80,   124,   121,   108,   408,   108,   108,
     108,   118,   117,   117,   121,    83,   445,   408,    58,   120,
     449,   124,    13,   452,    64,    13,   455,    67,    58,    59,
      60,     3,     3,    73,    64,    59,    60,    67,    78,    79,
      80,   470,   471,    73,   121,   110,   121,    43,    78,    79,
      80,    81,    53,    85,   597,   597,    86,   123,   117,   121,
      54,   108,   120,    21,   493,   621,   123,   113,    92,    93,
      94,    95,    96,    97,    98,    99,   113,   119,   122,   124,
      58,   121,   106,   124,   117,   628,    64,   111,   118,    67,
      43,   121,    58,   121,   637,    73,    21,   640,   640,   122,
      78,    79,    80,   646,   646,    14,   124,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    92,    93,    94,    95,
      96,    97,    98,    99,    63,   596,   308,   566,   304,   105,
     569,    84,   300,   121,   400,   302,   558,   560,    35,   578,
     188,    14,   581,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    14,    58,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    26,    27,    28,    29,    30,    31,    32,
      33,    34,   631,   637,   149,   572,   257,    92,    93,    94,
      95,    96,    97,    98,    99,   124,   125,    14,   496,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,   548,
      14,   124,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    14,   124,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,   499,    -1,    -1,    14,   124,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    14,   124,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,   452,    14,
     124,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,   124,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,    59,    60,    -1,    72,    -1,    -1,    65,    -1,
      -1,    14,   124,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,   122,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    14,   119,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    14,   119,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    30,    31,    -1,    -1,    -1,    35,
      36,    37,    -1,    88,    89,    41,    42,    -1,    44,    45,
      -1,    47,    48,    49,    50,    51,   119,   102,   103,    -1,
      -1,    57,   107,   108,    -1,    -1,    62,    63,   113,   114,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    77,    -1,    -1,    -1,    -1,    82,    -1,    84,    85,
      -1,    -1,    88,    89,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,   103,    -1,   105,
      -1,   107,   108,    -1,   110,    -1,   117,   113,   114,   115,
     116,   117,   118,    -1,   120,    30,    31,   123,    -1,    -1,
      35,    36,    37,    -1,    -1,    40,    41,    42,    -1,    44,
      45,    58,    47,    48,    49,    50,    51,    64,    -1,    -1,
      67,    -1,    57,    -1,    -1,    -1,    73,    62,    63,    -1,
      -1,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    -1,    -1,    -1,    -1,    82,    -1,    84,
      85,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,   107,   108,   121,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,   120,    30,    31,   123,    -1,
      -1,    35,    36,    37,    -1,    -1,    -1,    41,    42,    -1,
      44,    45,    58,    47,    48,    49,    50,    51,    64,    -1,
      -1,    67,    -1,    57,    -1,    -1,    -1,    73,    62,    63,
      -1,    -1,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    82,    -1,
      84,    85,    -1,    -1,    88,    89,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
      -1,    -1,   118,   107,   108,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   118,    -1,   120,    30,    31,   123,
      -1,    -1,    35,    36,    37,    -1,    -1,    40,    41,    42,
      -1,    44,    45,    -1,    47,    48,    49,    50,    51,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    82,
      -1,    84,    85,    -1,    -1,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
     103,    59,    60,    -1,   107,   108,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,    -1,   118,    -1,   120,    30,    31,
     123,    -1,    -1,    35,    36,    37,    -1,    -1,    40,    41,
      -1,    -1,    44,    45,    92,    93,    94,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,   107,
     108,    -1,    -1,    -1,    -1,   113,   114,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      82,    -1,    84,    85,    30,    31,    88,    89,    -1,    35,
      36,    37,    -1,    -1,    -1,    41,    -1,    -1,    44,    45,
     102,   103,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    82,    -1,    84,    85,
      30,    31,    88,    89,    -1,    35,    36,    37,    -1,    -1,
      40,    41,    -1,    -1,    44,    45,   102,   103,    -1,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,    -1,   118,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    82,    -1,    84,    85,    30,    31,    88,    89,
      -1,    35,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,
      44,    45,   102,   103,    -1,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,    -1,   118,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    82,    -1,
      84,    85,    30,    31,    88,    89,    -1,    35,    36,    37,
      -1,    -1,    -1,    41,    -1,    -1,    44,    45,   102,   103,
      -1,    -1,    -1,   107,   108,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,    -1,   118,   119,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    82,    -1,    84,    85,    30,    31,
      88,    89,    -1,    35,    36,    37,    -1,    -1,    -1,    41,
      -1,    -1,    44,    45,   102,   103,    -1,    -1,    -1,   107,
     108,    -1,    -1,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      82,    -1,    84,    85,    -1,    -1,    88,    89,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,   103,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,    -1,   118,    -1,    -1,    -1,
      -1,   123,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,    -1,    46,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,    55,    56,   116,    58,    59,    60,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    79,    80,
      81,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    55,    56,    58,    59,    64,    67,    68,    69,    70,
      71,    73,    74,    75,    78,    79,    80,    81,    86,   127,
     130,   131,   132,   133,   140,   156,   177,   181,   197,   199,
     205,   212,   213,   214,   215,   216,   107,   141,   142,   157,
      66,   108,   134,   135,   138,   139,   136,   137,   108,     0,
     128,   132,   120,   120,    60,    46,   100,   160,   161,   162,
     169,    58,   214,     3,   117,   120,   108,   110,   110,   110,
     110,   110,   110,    58,   105,   129,   217,   218,   219,   224,
     118,   146,   147,   178,   179,   180,   181,   197,   205,   215,
     203,   204,    66,   108,   187,   189,   195,   196,   198,   108,
     108,    90,    91,   123,   167,   141,   215,   230,   231,    30,
      31,    35,    36,    37,    41,    42,    44,    45,    47,    48,
      49,    50,    51,    57,    62,    63,    76,    77,    82,    84,
      85,    88,    89,   101,   102,   103,   107,   108,   113,   114,
     115,   116,   117,   118,   120,   123,   232,   238,   242,   243,
     244,   245,   246,   248,   253,   257,   270,   271,   281,   282,
     283,   285,   297,   298,   309,   313,   322,   328,   224,    92,
      93,    94,    95,    96,    97,    98,    99,   158,   117,   108,
     117,   121,   180,   158,   169,   227,   121,   200,   201,   202,
     215,   190,    61,    87,   194,   120,    59,    60,    65,   158,
     159,   172,   174,   175,   176,   122,   165,   227,   121,   215,
     281,   281,   281,    88,    89,   108,   282,   283,   283,   108,
     288,   123,   123,   123,   123,   278,   280,   258,   249,   281,
     117,   117,   123,   294,   295,   123,   123,   316,   315,   281,
     314,   323,   233,   281,   242,   117,    14,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,   117,    40,   286,   289,
     290,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    36,    37,   118,   286,   286,   286,   286,   117,   225,
     148,   228,   117,   200,   169,   111,   188,   191,   192,   193,
     120,   182,   183,   184,   185,   173,   124,     3,   166,   170,
     117,   227,    40,   123,   287,   289,   281,   281,   281,   272,
     123,   232,   236,   245,   123,   232,   124,   123,   293,   317,
     318,   124,   123,   123,   117,   123,   281,   324,   325,   326,
     217,   124,   125,   330,   329,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   288,   158,   281,   281,   281,   281,
     281,   281,   281,   109,   290,   308,   307,   306,   305,   304,
     303,   302,   301,   300,   299,    72,   119,   281,    22,   221,
      59,    60,    92,    93,    94,    95,    96,    97,   107,   108,
     113,   114,   115,   116,   143,   144,   145,   221,   111,   111,
       3,   182,   121,   184,   104,   146,   149,   186,   215,   221,
     172,   174,   106,   111,   159,   163,   105,   110,   120,   168,
     245,   117,   109,   317,   289,   124,   124,   124,   224,   268,
     269,   281,   281,    48,   281,   250,   108,   172,   174,   291,
     292,   296,   124,   319,   320,   321,   317,   317,   317,    15,
     327,   119,     3,   238,   286,   310,   281,   281,   123,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   119,
     284,    72,   119,   222,   226,   119,     3,   144,   229,   191,
     121,   118,   154,   169,   227,    18,   171,   164,   217,   124,
     254,   273,   124,   123,   124,    52,     3,   124,   232,     3,
     281,   124,   124,   124,   281,   326,   121,   281,   317,   281,
     119,   281,    59,    60,    92,    93,    94,    95,    96,    97,
      98,    99,   107,   111,   113,   116,   220,   108,   206,   210,
     144,   108,   209,   211,   108,   150,   117,   108,   221,   238,
     236,   117,   279,   281,   120,   251,   291,   320,    83,   312,
     312,   312,   122,   124,   119,   119,   223,    13,     3,    13,
       3,   155,   110,   121,    43,   255,   274,   236,   124,    53,
     259,   260,   261,   262,   123,   247,    85,   294,   311,   312,
     221,   207,   206,    30,    31,   107,   113,   114,   115,   116,
     208,   209,   108,   112,   151,   152,   153,   256,   268,   117,
     281,   121,    54,   264,   262,   108,   120,   234,   123,   281,
      21,   281,   113,   113,   119,   152,   236,   275,   263,   122,
     124,   235,   252,   124,   124,   117,   122,   265,   217,    43,
     266,   276,   237,   238,   238,   232,   239,   240,   241,   244,
     267,   268,   121,   241,   236,   277,   124,   236
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 182 "ferite_parser.y"
    {
										ferite_do_function_header( ferite_compiler_entry_function(ferite_scanner_file), FE_FALSE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE, FE_FALSE );
										if( ferite_do_function_start() ) {
											yyclearin;
										}
										ferite_var_array_count = 0;
									;}
    break;

  case 3:
#line 189 "ferite_parser.y"
    {	ferite_do_script_finish(); ;}
    break;

  case 4:
#line 193 "ferite_parser.y"
    {	ferite_do_function_footer(); ;}
    break;

  case 5:
#line 195 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_void_variable( NULL, "forced-function-return", FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_function_footer();
									;}
    break;

  case 17:
#line 224 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 18:
#line 225 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-init", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 19:
#line 226 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 20:
#line 227 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-deinit", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 21:
#line 228 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 22:
#line 229 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-register", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 23:
#line 230 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 24:
#line 231 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-unregister", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 25:
#line 232 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 26:
#line 233 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-header", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 27:
#line 234 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 28:
#line 235 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-flags", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 30:
#line 243 "ferite_parser.y"
    { ferite_do_uses( (yyvsp[(1) - (1)].text) ); ffree_ngc((yyvsp[(1) - (1)].text)); FECT; ;}
    break;

  case 35:
#line 257 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
												 										 "", (yyvsp[(1) - (1)].text), 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item );
							  ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT; ;}
    break;

  case 36:
#line 261 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																					  "", (yyvsp[(1) - (1)].text), 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item );
						  ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT; ;}
    break;

  case 37:
#line 265 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_number_double_variable( CURRENT_SCRIPT, "", (yyvsp[(1) - (1)].fnumber), FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 38:
#line 267 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_number_double_variable( CURRENT_SCRIPT, "", (yyvsp[(1) - (1)].inumber), FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 39:
#line 269 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "number", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 40:
#line 272 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "boolean", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 41:
#line 275 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "string", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 42:
#line 278 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "void", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 43:
#line 281 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "object", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 44:
#line 284 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "array", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 45:
#line 287 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																						 "", "class", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 46:
#line 290 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
																					 "", "namespace", 0, FE_CHARSET_DEFAULT, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 47:
#line 293 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_boolean_variable( CURRENT_SCRIPT, "", FE_TRUE, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 48:
#line 295 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_boolean_variable( CURRENT_SCRIPT, "", FE_FALSE, FE_STATIC );
						  ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 53:
#line 310 "ferite_parser.y"
    { 
								ferite_directive_parameters = ferite_create_stack( CURRENT_SCRIPT, 10 ); 
								ferite_scanner_start_directive_list();
							;}
    break;

  case 54:
#line 314 "ferite_parser.y"
    {
								ferite_scanner_stop_directive_list();
								if( ferite_expansion_hash && ferite_hash_get( FE_NoScript, ferite_expansion_hash, (yyvsp[(2) - (5)].text) ) ) {
									char *body = NULL;
									FeriteIterator *iter = ferite_create_iterator( FE_NoScript );
									FeriteHashBucket *bucket = NULL;

									ferite_expansion_map_hash = ferite_hash_get( FE_NoScript, ferite_expansion_hash, (yyvsp[(2) - (5)].text) );
									printf("Got expansion call: %s\n", (yyvsp[(2) - (5)].text));
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
									ferite_do_add_directive( (yyvsp[(2) - (5)].text), ferite_directive_parameters );
									ferite_directive_parameters = NULL;
									ffree_ngc((yyvsp[(2) - (5)].text)); FECT;
								}
							 ;}
    break;

  case 55:
#line 348 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 56:
#line 349 "ferite_parser.y"
    { printf("Code block: '%s'\n", (yyvsp[(4) - (4)].text));
		ferite_hash_add( FE_NoScript, ferite_expansion_map_hash, "!_BODY_", (yyvsp[(4) - (4)].text) );
	;}
    break;

  case 59:
#line 360 "ferite_parser.y"
    { printf("Label: %s [%ld]\n", (yyvsp[(1) - (1)].text), ferite_expansion_map_offset ); ferite_expansion_map_offset++; ;}
    break;

  case 60:
#line 361 "ferite_parser.y"
    {
		printf("DirectiveLabel: %s [%ld]\n", (yyvsp[(1) - (1)].text), ferite_expansion_map_offset );
		ferite_hash_add( FE_NoScript, ferite_expansion_map_hash, (yyvsp[(1) - (1)].text), (void*)ferite_expansion_map_offset );
		ferite_expansion_map_offset++;
	;}
    break;

  case 63:
#line 375 "ferite_parser.y"
    {
			printf("expansion name: %s\n", (yyvsp[(2) - (2)].text));
			if( ferite_expansion_hash == NULL ) {
				ferite_expansion_hash = ferite_create_hash( FE_NoScript, 10 );
			}
			ferite_expansion_map_offset = 0;
			ferite_expansion_map_hash = ferite_create_hash( FE_NoScript, 10 );
			ferite_hash_add( FE_NoScript, ferite_expansion_hash, (yyvsp[(2) - (2)].text), ferite_expansion_map_hash );
		;}
    break;

  case 65:
#line 389 "ferite_parser.y"
    { FUD(( "going into global block\n" )); ferite_var_is_global = FE_TRUE; ;}
    break;

  case 66:
#line 391 "ferite_parser.y"
    { ferite_var_is_global = FE_FALSE; FUD(( "leaving global memory block\n" )); ;}
    break;

  case 67:
#line 395 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "L" ); ferite_last_type = F_VAR_NUM; ;}
    break;

  case 68:
#line 396 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "L" ); ferite_last_type = F_VAR_LONG; ;}
    break;

  case 69:
#line 397 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "D" ); ferite_last_type = F_VAR_DOUBLE; ;}
    break;

  case 70:
#line 398 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "S" ); ferite_last_type = F_VAR_STR; ;}
    break;

  case 71:
#line 399 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "V" ); ferite_last_type = F_VAR_VOID; ;}
    break;

  case 72:
#line 400 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "O" ); ferite_last_type = F_VAR_OBJ; ;}
    break;

  case 73:
#line 401 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "A" ); ferite_last_type = F_VAR_UARRAY; ;}
    break;

  case 74:
#line 402 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "B" ); ferite_last_type = F_VAR_BOOL; ;}
    break;

  case 76:
#line 407 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "C" ); ferite_last_type = F_VAR_CLASS; ;}
    break;

  case 77:
#line 408 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "N" ); ferite_last_type = F_VAR_NS; ;}
    break;

  case 80:
#line 417 "ferite_parser.y"
    { ferite_last_function = (yyvsp[(2) - (2)].text); ;}
    break;

  case 81:
#line 418 "ferite_parser.y"
    { ferite_last_function = (yyvsp[(2) - (2)].text); ferite_function_is_directive = FE_TRUE; ;}
    break;

  case 82:
#line 420 "ferite_parser.y"
    { ferite_last_function = fstrdup( "constructor" ); ;}
    break;

  case 83:
#line 422 "ferite_parser.y"
    { ferite_last_function = fstrdup( "destructor" ); ;}
    break;

  case 84:
#line 426 "ferite_parser.y"
    {
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
									;}
    break;

  case 85:
#line 454 "ferite_parser.y"
    {
										ferite_return_type_was_provided = FE_TRUE;
										ferite_scanner_stop_typing_label();
										ferite_seeking_return_type_fix = FE_TRUE;
										ferite_hint_depth = 0;
									;}
    break;

  case 86:
#line 460 "ferite_parser.y"
    { ferite_seeking_return_type_fix = FE_FALSE; ;}
    break;

  case 87:
#line 461 "ferite_parser.y"
    {
										ferite_scanner_stop_typing_label();
										ferite_return_type_was_provided = FE_TRUE;
										strcpy(ferite_hint_typestring, "O:");
										strcat(ferite_hint_typestring, (yyvsp[(1) - (1)].text));
										strcat(ferite_hint_typestring, ";");
										ffree_ngc((yyvsp[(1) - (1)].text)); FECT;
									;}
    break;

  case 88:
#line 469 "ferite_parser.y"
    { 
										strcpy(ferite_hint_typestring, "U");
										ferite_scanner_stop_typing_label();
										ferite_return_type_was_provided = FE_TRUE;
									;}
    break;

  case 89:
#line 477 "ferite_parser.y"
    { ferite_scanner_want_typing_label(); ;}
    break;

  case 90:
#line 478 "ferite_parser.y"
    { 
										ferite_do_function_return_type(ferite_hint_typestring);
									;}
    break;

  case 91:
#line 481 "ferite_parser.y"
    { 
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
									;}
    break;

  case 92:
#line 511 "ferite_parser.y"
    {
										FUD(("PARSER: END FUNCTION PARAMETER DECLARATION\n"));
										FUD(("PARSER: BEGIN FUNCTION BODY\n"));
									;}
    break;

  case 94:
#line 522 "ferite_parser.y"
    {
										FUD(("PARSER: END FUNCTION BODY\n"));
										ferite_temp_variable = ferite_create_void_variable( NULL, "forced-function-return", FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(voidptr)		\n"));
										ferite_do_function_footer();
									;}
    break;

  case 95:
#line 530 "ferite_parser.y"
    {
										ferite_do_function_native_block( (yyvsp[(1) - (1)].text),
												ferite_current_native_block_file,
												ferite_current_native_block_line );
									;}
    break;

  case 96:
#line 535 "ferite_parser.y"
    {

										FUD(("PARSER: END FUNCTION BODY\n"));
										ferite_temp_variable = ferite_create_void_variable( NULL, "forced-function-return", FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(voidptr)		\n"));
										ferite_do_function_footer();
									;}
    break;

  case 97:
#line 544 "ferite_parser.y"
    {	ferite_do_function_footer(); ;}
    break;

  case 98:
#line 551 "ferite_parser.y"
    {
										ferite_var_is_params = FE_FALSE;
										if( ferite_do_function_start() ) {
											yyclearin;
										}
										if( ferite_var_is_native == FE_TRUE ) {
											ferite_scanner_go_native();
											ferite_var_is_native = FE_FALSE;
										}
									;}
    break;

  case 101:
#line 566 "ferite_parser.y"
    { ferite_var_pass_type = FE_BY_REF; ;}
    break;

  case 102:
#line 570 "ferite_parser.y"
    { ferite_hint_depth = 0; ;}
    break;

  case 103:
#line 573 "ferite_parser.y"
    {
										FUD(("PARSER: Parameter Declared		 %s (%d)\n", (yyvsp[(5) - (5)].text), ferite_last_type));
										ferite_do_add_variable_to_paramlist( (yyvsp[(5) - (5)].text), ferite_last_type, ferite_hint_current_type, ferite_var_pass_type );
										ffree_ngc( (yyvsp[(5) - (5)].text) ); FECT;
										ferite_var_pass_type = FE_BY_VALUE;
									;}
    break;

  case 104:
#line 582 "ferite_parser.y"
    {
										FUD(("PARSER: Parameter Declared - Multiple Args\n"));
										ferite_do_add_variable_to_paramlist( "...", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_VALUE );
									;}
    break;

  case 111:
#line 601 "ferite_parser.y"
    { ferite_do_namespace_header( (yyvsp[(2) - (2)].text) ); ffree_ngc( (yyvsp[(2) - (2)].text) ); FECT; ;}
    break;

  case 112:
#line 602 "ferite_parser.y"
    { ferite_do_namespace_extends( (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) );  FECT; ;}
    break;

  case 123:
#line 626 "ferite_parser.y"
    { ferite_do_namespace_footer(); ;}
    break;

  case 128:
#line 640 "ferite_parser.y"
    { ferite_item_state = -1; ;}
    break;

  case 134:
#line 652 "ferite_parser.y"
    {
										FUD(("CLASS START\n" ));
										ferite_do_class_header( (yyvsp[(1) - (1)].text), NULL, ferite_class_state );
										ferite_class_state = 0;
										ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
									;}
    break;

  case 135:
#line 658 "ferite_parser.y"
    {
										ferite_scanner_want_dot_label();
									;}
    break;

  case 136:
#line 661 "ferite_parser.y"
    {
										FUD(("CLASS START\n" ));
										ferite_scanner_stop_dot_label();
										ferite_do_class_header( (yyvsp[(1) - (4)].text), (yyvsp[(4) - (4)].text), ferite_class_state );
										ferite_class_state = 0;
										ffree_ngc( (yyvsp[(1) - (4)].text) ); FECT;
										ffree_ngc( (yyvsp[(4) - (4)].text) ); FECT;
									;}
    break;

  case 137:
#line 672 "ferite_parser.y"
    {
										ferite_scanner_want_dot_label();
									;}
    break;

  case 138:
#line 675 "ferite_parser.y"
    {
										FUD(("CLASS START\n" ));
										ferite_scanner_stop_dot_label();
										ferite_do_class_header( NULL, (yyvsp[(3) - (3)].text), ferite_class_state );
										ferite_class_state = 0;
										ffree_ngc( (yyvsp[(3) - (3)].text) ); FECT;
									;}
    break;

  case 139:
#line 685 "ferite_parser.y"
    {
										ferite_scanner_want_dot_label();
									;}
    break;

  case 140:
#line 688 "ferite_parser.y"
    {
										ferite_scanner_stop_dot_label();
										ferite_do_class_implements( (yyvsp[(2) - (2)].text) );
										ffree_ngc( (yyvsp[(2) - (2)].text) ); FECT;
									;}
    break;

  case 145:
#line 708 "ferite_parser.y"
    {
										FUD(("CLASS END\n" ));
										ferite_do_class_footer();
									;}
    break;

  case 146:
#line 716 "ferite_parser.y"
    {
										ferite_do_class_footer();
									;}
    break;

  case 150:
#line 733 "ferite_parser.y"
    {
							ferite_do_class_header( (yyvsp[(2) - (2)].text), NULL, FE_ITEM_IS_PROTOCOL );
							ferite_class_state = 0;
							ffree_ngc( (yyvsp[(2) - (2)].text) ); FECT;
						;}
    break;

  case 156:
#line 755 "ferite_parser.y"
    { ferite_class_state = -1; ;}
    break;

  case 158:
#line 760 "ferite_parser.y"
    {
										ferite_do_class_footer();
									;}
    break;

  case 159:
#line 767 "ferite_parser.y"
    {
										FUD(("PARSER: Variable Declared		 %s(%d)\n", (yyvsp[(1) - (1)].text), ferite_last_type));
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (1)].text), ferite_scanner_lineno, ferite_scanner_file );
										ferite_do_add_variable( (yyvsp[(1) - (1)].text), ferite_last_type, ferite_hint_current_type, ferite_var_is_global, ferite_var_is_final, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );

										ferite_do_variable_push( (yyvsp[(1) - (1)].text) );
										ferite_do_variable_reset( ferite_last_type );

										ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
									;}
    break;

  case 160:
#line 778 "ferite_parser.y"
    {
										FUD(("PARSER: Variable Declared		 %s(%d)\n", (yyvsp[(1) - (2)].text), ferite_last_type));
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (2)].text), ferite_scanner_lineno, ferite_scanner_file );
										ferite_do_add_variable( (yyvsp[(1) - (2)].text), ferite_last_type, ferite_hint_current_type, FE_FALSE, ferite_var_is_final, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );

										ferite_do_variable_push( (yyvsp[(1) - (2)].text) );
										ferite_do_variable_reset( ferite_last_type );

										ferite_do_variable_push( (yyvsp[(1) - (2)].text) );
										ffree_ngc( (yyvsp[(1) - (2)].text) ); FECT;
									;}
    break;

  case 161:
#line 790 "ferite_parser.y"
    {
										ferite_do_binary_op( FERITE_OPCODE_assign );
										FUD(("PARSER: op->assign	   pop,pop\n"));
										ferite_do_pop();
									;}
    break;

  case 162:
#line 798 "ferite_parser.y"
    {   ferite_namespace_naturaln = 0 - (yyvsp[(2) - (2)].inumber); ;}
    break;

  case 163:
#line 799 "ferite_parser.y"
    {   ferite_namespace_naturaln = (yyvsp[(2) - (2)].inumber); ;}
    break;

  case 164:
#line 800 "ferite_parser.y"
    {   ferite_namespace_naturaln = (yyvsp[(1) - (1)].inumber); ;}
    break;

  case 165:
#line 804 "ferite_parser.y"
    {
										FUD(("PARSER: Variable Declared		 %s(%d)\n", (yyvsp[(1) - (1)].text), ferite_last_type));
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (1)].text), ferite_scanner_lineno, ferite_scanner_file );
										ferite_do_add_variable( (yyvsp[(1) - (1)].text), ferite_last_type, ferite_hint_current_type, ferite_var_is_global, ferite_var_is_final, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
										ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
									;}
    break;

  case 166:
#line 811 "ferite_parser.y"
    {
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
										if( ferite_last_type == F_VAR_NUM )
										{
											FUD(("PARSER: Variable Declared		 %s(%d)=%d\n", (yyvsp[(1) - (3)].text), ferite_last_type, ferite_namespace_naturaln ));
											ferite_temp_variable = ferite_create_number_long_variable( NULL, (yyvsp[(1) - (3)].text), ferite_namespace_naturaln, FE_ALLOC );
											if( ferite_var_is_final )
											  MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
											ferite_do_add_variable_with_value( (yyvsp[(1) - (3)].text), ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
											ffree_ngc( (yyvsp[(1) - (3)].text) ); FECT;
										}
										else
										{
											ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the number %d.\n", (yyvsp[(1) - (3)].text), ferite_variable_id_to_str( NULL,ferite_last_type), ferite_namespace_naturaln );
											ffree_ngc( (yyvsp[(1) - (3)].text) ); FECT;
											feperror( NULL );
										}
									;}
    break;

  case 167:
#line 831 "ferite_parser.y"
    {
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
										if( ferite_last_type == F_VAR_NUM )
										{
											FUD(("PARSER: Variable Declared		 %s(%d)=%f\n", (yyvsp[(1) - (3)].text), ferite_last_type, (yyvsp[(3) - (3)].fnumber)));
											ferite_temp_variable = ferite_create_number_double_variable( NULL, (yyvsp[(1) - (3)].text), (yyvsp[(3) - (3)].fnumber), FE_ALLOC );
											if( ferite_var_is_final )
											  MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
											ferite_do_add_variable_with_value( (yyvsp[(1) - (3)].text), ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
											ffree_ngc( (yyvsp[(1) - (3)].text) ); FECT;
										}
										else
										{
											ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the number %f.\n", (yyvsp[(1) - (3)].text), ferite_variable_id_to_str( NULL,ferite_last_type), (yyvsp[(3) - (3)].fnumber) );
											ffree_ngc( (yyvsp[(1) - (3)].text) ); FECT;
											feperror( NULL );
										}
									;}
    break;

  case 168:
#line 851 "ferite_parser.y"
    {
										if( ferite_var_is_native )
										  ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
										if( ferite_last_type == F_VAR_STR )
										{
											FUD(("PARSER: Variable Declared		 %s(%d)='%s'\n", (yyvsp[(1) - (3)].text), ferite_last_type, (yyvsp[(3) - (3)].text)));
											ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, (yyvsp[(1) - (3)].text), (yyvsp[(3) - (3)].text), strlen((yyvsp[(3) - (3)].text)), FE_CHARSET_DEFAULT, FE_ALLOC );
											if( ferite_var_is_final )
											  MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
											ferite_do_add_variable_with_value( (yyvsp[(1) - (3)].text), ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
											ffree_ngc( (yyvsp[(1) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); FECT;
										}
										else
										{
											ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the string '%s'.\n", (yyvsp[(1) - (3)].text), ferite_variable_id_to_str( NULL,ferite_last_type), (yyvsp[(3) - (3)].text) );
											ffree_ngc( (yyvsp[(1) - (3)].text) );
											ffree_ngc( (yyvsp[(3) - (3)].text) );
											FECT;
											feperror( NULL );
										}
									;}
    break;

  case 169:
#line 873 "ferite_parser.y"
    {
									   if( ferite_var_is_native )
										 ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
									   if( ferite_last_type == F_VAR_BOOL )
									   {
										   FUD(("PARSER: Variable Declared		 %s(%d)=%d\n", (yyvsp[(1) - (3)].text), ferite_last_type, (yyvsp[(3) - (3)].inumber)));
										   ferite_temp_variable = ferite_create_boolean_variable( NULL, (yyvsp[(1) - (3)].text), FE_TRUE, FE_ALLOC );
										   if( ferite_var_is_final )
											 MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
										   ferite_do_add_variable_with_value( (yyvsp[(1) - (3)].text), ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
										   ffree_ngc( (yyvsp[(1) - (3)].text) ); FECT;
									   }
									   else
									   {
										   ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the number %f.\n", (yyvsp[(1) - (3)].text), ferite_variable_id_to_str( NULL,ferite_last_type), (yyvsp[(3) - (3)].inumber) );
										   ffree_ngc( (yyvsp[(1) - (3)].text) ); FECT;
										   feperror( NULL );
									   }
								   ;}
    break;

  case 170:
#line 893 "ferite_parser.y"
    {
									   if( ferite_var_is_native )
										 ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
									   if( ferite_last_type == F_VAR_BOOL )
									   {
										   FUD(("PARSER: Variable Declared		 %s(%d)=%d\n", (yyvsp[(1) - (3)].text), ferite_last_type, (yyvsp[(3) - (3)].inumber)));
										   ferite_temp_variable = ferite_create_boolean_variable( NULL, (yyvsp[(1) - (3)].text), FE_FALSE, FE_ALLOC );
										   if( ferite_var_is_final )
											 MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
											ferite_temp_variable->subtype = ferite_hint_current_type;
										   ferite_do_add_variable_with_value( (yyvsp[(1) - (3)].text), ferite_temp_variable, ferite_var_is_global, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
										   ffree_ngc( (yyvsp[(1) - (3)].text) ); FECT;
									   }
									   else
									   {
										   ferite_error( CURRENT_SCRIPT, 0, "Can't initialise variable '%s' of type '%s' with the number %f.\n", (yyvsp[(1) - (3)].text), ferite_variable_id_to_str( NULL,ferite_last_type), (yyvsp[(3) - (3)].inumber) );
										   ffree_ngc( (yyvsp[(1) - (3)].text) ); FECT;
										   feperror( NULL );
									   }
								   ;}
    break;

  case 175:
#line 927 "ferite_parser.y"
    {
									  if( ferite_class_state > 0 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "The class already has state - you can't make it final\n" );
										  feperror( NULL );
									  }
									  else
										ferite_class_state = FE_ITEM_IS_FINAL;
									;}
    break;

  case 176:
#line 936 "ferite_parser.y"
    {
									  if( ferite_class_state > 0 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "The class already has state - you can't make it abstract\n" );
										  feperror( NULL );
									  }
									  else
										ferite_class_state = FE_ITEM_IS_ABSTRACT;
									;}
    break;

  case 179:
#line 953 "ferite_parser.y"
    {
									  if( ferite_var_is_final )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already final - you can't make it final again\n" );
										  feperror( NULL );
									  }
									  else
										ferite_var_is_final = FE_TRUE;
									;}
    break;

  case 180:
#line 962 "ferite_parser.y"
    {
									  if( ferite_var_is_static )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already static - you can't make it static again\n" );
										  feperror( NULL );
									  }
									  else
										ferite_var_is_static = FE_TRUE;
									;}
    break;

  case 181:
#line 971 "ferite_parser.y"
    {
									  if( ferite_var_is_atomic )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already atomic - you can't make it atomic again\n" );
										  feperror( NULL );
									  }
									  else
										ferite_var_is_atomic = FE_TRUE;
									;}
    break;

  case 182:
#line 980 "ferite_parser.y"
    {
									  if( ferite_var_is_native )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already native - you can't make it native again\n" );
										  feperror( NULL );
									  }
									  else
										ferite_var_is_native = FE_TRUE;
									;}
    break;

  case 183:
#line 989 "ferite_parser.y"
    {
									  if( ferite_item_state > -1 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already %s - you can't make it private\n", ferite_state_to_str( ferite_item_state ) );
										  feperror( NULL );
									  }
									  else
										ferite_item_state = FE_ITEM_IS_PRIVATE;
									;}
    break;

  case 184:
#line 998 "ferite_parser.y"
    {
									  if( ferite_item_state > -1 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already %s - you can't make it protected\n", ferite_state_to_str( ferite_item_state ) );
										  feperror( NULL );
									  }
									  else
										ferite_item_state = FE_ITEM_IS_PROTECTED;
									;}
    break;

  case 185:
#line 1007 "ferite_parser.y"
    {
									  if( ferite_item_state > -1 )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already %s - you can't make it public\n", ferite_state_to_str( ferite_item_state ) );
										  feperror( NULL );
									  }
									  else
										ferite_item_state = FE_ITEM_IS_PUBLIC;
									;}
    break;

  case 194:
#line 1039 "ferite_parser.y"
    {
									  if( ferite_var_is_final )
									  {
										  ferite_error( CURRENT_SCRIPT, 0, "Item is already final - you can't make it final again\n" );
										  feperror( NULL );
									  }
									  else
										  ferite_var_is_final = FE_TRUE;
									;}
    break;

  case 196:
#line 1052 "ferite_parser.y"
    {
						if( ferite_hint_typestring[strlen(ferite_hint_typestring) - 1] == 'A' ) {
							strcat( ferite_hint_typestring, "O" );
						}
						strcat( ferite_hint_typestring, ":" );
						strcat( ferite_hint_typestring, (yyvsp[(1) - (1)].text) );
						strcat( ferite_hint_typestring, ";" );
						ffree_ngc((yyvsp[(1) - (1)].text)); 
						FECT; 
					 ;}
    break;

  case 197:
#line 1062 "ferite_parser.y"
    { ferite_error(CURRENT_SCRIPT, 0, "Quoted string hint type not supported\n" ); ffree_ngc((yyvsp[(1) - (1)].text)); FECT; feperror( NULL ); ;}
    break;

  case 198:
#line 1063 "ferite_parser.y"
    { ferite_error(CURRENT_SCRIPT, 0, "Floating point hint type not supported\n" ); feperror( NULL ); ;}
    break;

  case 199:
#line 1064 "ferite_parser.y"
    { ferite_error(CURRENT_SCRIPT, 0, "Natural nunber hint type not supported\n" ); feperror( NULL ); ;}
    break;

  case 200:
#line 1065 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "L" ); ;}
    break;

  case 201:
#line 1066 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "L" ); ;}
    break;

  case 202:
#line 1067 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "D" ); ;}
    break;

  case 203:
#line 1068 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "S" ); ;}
    break;

  case 204:
#line 1069 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "V" ); ;}
    break;

  case 205:
#line 1070 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "O" ); ;}
    break;

  case 206:
#line 1071 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "A" ); ;}
    break;

  case 207:
#line 1072 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "B" ); ;}
    break;

  case 208:
#line 1073 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "C" ); ;}
    break;

  case 209:
#line 1074 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "N" ); ;}
    break;

  case 210:
#line 1078 "ferite_parser.y"
    {
										if( ferite_seeking_return_type_fix && ferite_hint_depth == 0 ) {
											yyclearin; 
											ferite_lexer_fix_native_blocks(); 
										}
									;}
    break;

  case 211:
#line 1084 "ferite_parser.y"
    {
										ferite_hint_depth++;
										ferite_scanner_want_end_hint(); 
										ferite_scanner_want_typing_label();
									;}
    break;

  case 212:
#line 1089 "ferite_parser.y"
    { ferite_scanner_stop_typing_label(); ;}
    break;

  case 213:
#line 1091 "ferite_parser.y"
    { 
										ferite_scanner_stop_end_hint();
										ferite_hint_depth--;
									;}
    break;

  case 214:
#line 1098 "ferite_parser.y"
    {
										FUD(("PARSER: Setting current type to %d\n", ferite_last_type ));
										ferite_hint_depth = 0;
									;}
    break;

  case 215:
#line 1102 "ferite_parser.y"
    {
										ferite_hint_current_type = ferite_subtype_link( CURRENT_SCRIPT, ferite_hint_typestring );
										if( ferite_hint_current_type == NULL && CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
											feperror(NULL);
										}
									;}
    break;

  case 216:
#line 1108 "ferite_parser.y"
    { 
										ferite_var_is_final = FE_FALSE; 
										ferite_var_is_static = FE_FALSE; 
										ferite_var_is_atomic = FE_FALSE; 
										ferite_var_is_native = FE_FALSE; 
									;}
    break;

  case 217:
#line 1117 "ferite_parser.y"
    {
										FUD(("PARSER: Setting current type to %d\n", ferite_last_type ));
										ferite_hint_depth = 0;
									;}
    break;

  case 218:
#line 1121 "ferite_parser.y"
    {
										ferite_hint_current_type = ferite_subtype_link( CURRENT_SCRIPT, ferite_hint_typestring );
										if( ferite_hint_current_type == NULL && CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
											feperror(NULL);
										}
									;}
    break;

  case 219:
#line 1127 "ferite_parser.y"
    { ferite_var_is_final = FE_FALSE; ferite_var_is_static = FE_FALSE; ferite_var_is_atomic = FE_FALSE; ferite_var_is_native = FE_FALSE; ;}
    break;

  case 224:
#line 1142 "ferite_parser.y"
    { ferite_do_begin_block(); ;}
    break;

  case 225:
#line 1145 "ferite_parser.y"
    { ferite_do_end_block(); ;}
    break;

  case 226:
#line 1148 "ferite_parser.y"
    { ferite_do_begin_block(); ;}
    break;

  case 227:
#line 1151 "ferite_parser.y"
    { ferite_do_end_block(); ;}
    break;

  case 233:
#line 1168 "ferite_parser.y"
    {
								ferite_error( CURRENT_SCRIPT, 0, "You are not allowed to have empty handle blocks.\n" );
	  							feperror( NULL );
							;}
    break;

  case 243:
#line 1194 "ferite_parser.y"
    { ferite_do_pop(); /* clear up the stupid stack ie wham = 2 + 3 leaves a variable on the stack*/ ;}
    break;

  case 247:
#line 1198 "ferite_parser.y"
    { ferite_do_break(); ;}
    break;

  case 248:
#line 1199 "ferite_parser.y"
    { ferite_do_continue(); ;}
    break;

  case 249:
#line 1200 "ferite_parser.y"
    { ferite_do_raise(); ;}
    break;

  case 252:
#line 1209 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_void_variable( NULL, "compiled-in-return-value", FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(voidptr)		\n"));
										ferite_do_exit();
									;}
    break;

  case 253:
#line 1216 "ferite_parser.y"
    {  ferite_do_exit(); ;}
    break;

  case 255:
#line 1221 "ferite_parser.y"
    {
						ferite_do_add_variable( (yyvsp[(2) - (3)].text), F_VAR_OBJ, ferite_subtype_link(CURRENT_SCRIPT, "O"), FE_FALSE, FE_TRUE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );
						ferite_do_variable_push( (yyvsp[(2) - (3)].text) );
						ferite_do_variable_push( "err" );
						ferite_do_binary_op( FERITE_OPCODE_assign );
					;}
    break;

  case 256:
#line 1230 "ferite_parser.y"
    { ferite_do_monitor_block(); ;}
    break;

  case 257:
#line 1231 "ferite_parser.y"
    { ferite_do_before_handle_block(); ;}
    break;

  case 258:
#line 1232 "ferite_parser.y"
    { ferite_do_begin_block(); ;}
    break;

  case 259:
#line 1234 "ferite_parser.y"
    { ferite_do_end_block() ; ;}
    break;

  case 261:
#line 1239 "ferite_parser.y"
    { ferite_do_if_statement(); ;}
    break;

  case 265:
#line 1246 "ferite_parser.y"
    { ferite_do_after_then_statement(); ;}
    break;

  case 266:
#line 1247 "ferite_parser.y"
    { ferite_do_after_then_before_else_statement(); ;}
    break;

  case 267:
#line 1248 "ferite_parser.y"
    {  ferite_do_after_else_statement(); ;}
    break;

  case 268:
#line 1252 "ferite_parser.y"
    {
										ferite_do_pre_switch();
									;}
    break;

  case 269:
#line 1258 "ferite_parser.y"
    {
										ferite_do_post_switch();
									;}
    break;

  case 275:
#line 1281 "ferite_parser.y"
    {
										ferite_do_case_block_start();
									;}
    break;

  case 276:
#line 1284 "ferite_parser.y"
    {
										ferite_do_case_block_end();
									;}
    break;

  case 278:
#line 1291 "ferite_parser.y"
    {
										ferite_do_default_block_start();
									;}
    break;

  case 280:
#line 1298 "ferite_parser.y"
    { ferite_do_after_fix_block(); ;}
    break;

  case 281:
#line 1299 "ferite_parser.y"
    { ferite_do_after_fix_before_else_block(); ;}
    break;

  case 282:
#line 1300 "ferite_parser.y"
    { ferite_do_after_fix_else_statement(); ;}
    break;

  case 283:
#line 1304 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "forconst", 1, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: for truth value\n"));
									;}
    break;

  case 285:
#line 1314 "ferite_parser.y"
    { ferite_do_pop(); ;}
    break;

  case 287:
#line 1319 "ferite_parser.y"
    {
										ferite_error( CURRENT_SCRIPT, 0, "'foreach' has been removed from ferite for more elegant methods.\n" );
										ferite_error( CURRENT_SCRIPT, 0, "Please console the documentation for the Array module.\n" );
										feperror( NULL );
									;}
    break;

  case 288:
#line 1327 "ferite_parser.y"
    {
									ferite_do_begin_block();
										FUD(("PARSER: [FOR] Begin Initial Expression\n"));
									;}
    break;

  case 289:
#line 1331 "ferite_parser.y"
    {
										FUD(("PARSER: [FOR] End Initial Expression\n"));
										ferite_do_for_loop_start();
									;}
    break;

  case 290:
#line 1335 "ferite_parser.y"
    { FUD(("PARSER: [FOR] Begin Test Expression\n")); ;}
    break;

  case 291:
#line 1336 "ferite_parser.y"
    { FUD(("PARSER: [FOR] End Test Expression\n")); ;}
    break;

  case 292:
#line 1337 "ferite_parser.y"
    {
										FUD(("PARSER: [FOR] Begin Itterator Expression\n"));
										ferite_do_for_loop_itterate();
									;}
    break;

  case 293:
#line 1341 "ferite_parser.y"
    {
										FUD(("PARSER: [FOR] End Itterator Expression\n"));
										ferite_do_pop();
										ferite_do_for_loop_block();
									;}
    break;

  case 294:
#line 1347 "ferite_parser.y"
    {
										FUD(("PARSER: [FOR] Jump back to test expression\n"));
										ferite_do_for_loop_end();
					ferite_do_end_block();
									;}
    break;

  case 295:
#line 1353 "ferite_parser.y"
    { ferite_do_while_begin(); ;}
    break;

  case 296:
#line 1354 "ferite_parser.y"
    { ferite_do_while_after_expr(); ;}
    break;

  case 297:
#line 1355 "ferite_parser.y"
    { ferite_do_while_end(); ;}
    break;

  case 298:
#line 1356 "ferite_parser.y"
    { ferite_do_do_start(); ;}
    break;

  case 299:
#line 1358 "ferite_parser.y"
    { ferite_do_do_end(); ;}
    break;

  case 304:
#line 1370 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, "strconst", (yyvsp[(1) - (1)].text), 0, FE_CHARSET_DEFAULT, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(str)		\"%s\"\n", (yyvsp[(1) - (1)].text)));
										ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
									;}
    break;

  case 305:
#line 1378 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_boolean_variable( NULL, "true", FE_TRUE, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(str)		\"true\"\n"));
									;}
    break;

  case 306:
#line 1385 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_boolean_variable( NULL, "false", FE_FALSE, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(str)		\"false\"\n"));
									;}
    break;

  case 307:
#line 1392 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "longconst", (yyvsp[(1) - (1)].inumber), FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(int)		%d\n", (yyvsp[(1) - (1)].inumber)));
									;}
    break;

  case 308:
#line 1399 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_number_double_variable( NULL, "doubleconst", (yyvsp[(1) - (1)].fnumber), FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(float)	  %f\n", (yyvsp[(1) - (1)].fnumber)));
									;}
    break;

  case 309:
#line 1406 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "__LINE__", ferite_scanner_lineno, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
									;}
    break;

  case 310:
#line 1412 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, "__FILE__", ferite_scanner_file, 0, FE_CHARSET_DEFAULT, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
									;}
    break;

  case 311:
#line 1421 "ferite_parser.y"
    {
										ferite_do_variable_push( (yyvsp[(1) - (1)].text) );
										FUD(("PARSER: push(cvar)	   %s\n", (yyvsp[(1) - (1)].text)));
										ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
									;}
    break;

  case 312:
#line 1426 "ferite_parser.y"
    {   ferite_do_variable_push( "self" ); ferite_objcall_is_self = FE_TRUE; ;}
    break;

  case 313:
#line 1427 "ferite_parser.y"
    {   ferite_do_variable_push( "super" ); ferite_objcall_is_super = FE_TRUE; ;}
    break;

  case 314:
#line 1429 "ferite_parser.y"
    {
										FUD(("PARSER: op->array_slice\n"));
										ferite_do_many_op( FERITE_OPCODE_array_slice,3);
									;}
    break;

  case 315:
#line 1433 "ferite_parser.y"
    {
										FUD(("PARSER: op->array_slice\n"));
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "slice_first",0, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
									;}
    break;

  case 316:
#line 1439 "ferite_parser.y"
    { ferite_do_many_op(FERITE_OPCODE_array_slice,3); ;}
    break;

  case 317:
#line 1440 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_void_variable( NULL, "sliceconst", FE_STATIC );
										MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_many_op(FERITE_OPCODE_array_slice,3);
									;}
    break;

  case 318:
#line 1447 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_number_long_variable( NULL, "slice_first",0, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_temp_variable = ferite_create_void_variable( NULL, "sliceconst", FE_STATIC );
										MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_many_op(FERITE_OPCODE_array_slice,3);
									;}
    break;

  case 319:
#line 1457 "ferite_parser.y"
    {
										FUD(("PARSER: op->array_index"));
										ferite_do_binary_op(FERITE_OPCODE_array_index);
									;}
    break;

  case 320:
#line 1461 "ferite_parser.y"
    {   /* that dash cunning end of array index type thingy.... */
										/* it wasnt cunning enough because it broke with the introduction of negative indices ;) - ntropy*/
										ferite_temp_variable = ferite_create_void_variable( NULL, "void-placeholder", FE_STATIC );
										MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										FUD(("PARSER: push(int)		%d\n", FE_ARRAY_ADD_AT_END ));
										FUD(("PARSER: op->array_index\n"));
										ferite_do_binary_op( FERITE_OPCODE_array_index );
									;}
    break;

  case 323:
#line 1473 "ferite_parser.y"
    { ferite_do_self_push(); ferite_objcall_is_self = FE_TRUE; ;}
    break;

  case 329:
#line 1488 "ferite_parser.y"
    {
										ferite_do_variable_push( (yyvsp[(1) - (1)].text) );
										FUD(("PARSER: push(cvar)	   %s\n", (yyvsp[(1) - (1)].text)));
										ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
									;}
    break;

  case 331:
#line 1497 "ferite_parser.y"
    {
										ferite_do_variable_pushattr( (yyvsp[(2) - (2)].text), ferite_objcall_is_self, ferite_objcall_is_super );
										if( ferite_objcall_is_self || ferite_objcall_is_super ) {
											ferite_objcall_is_self = FE_FALSE;
											ferite_objcall_is_super = FE_FALSE;
										}
										FUD(("PARSER: push(object property)	   %s\n", (yyvsp[(2) - (2)].text)));
										ffree_ngc( (yyvsp[(2) - (2)].text) ); FECT;
									;}
    break;

  case 333:
#line 1512 "ferite_parser.y"
    {
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										FUD(("PARSER: op->c_obj_func	   %s\n", (yyvsp[(2) - (6)].text) ));
										ferite_do_object_function_call( (yyvsp[(2) - (6)].text), ferite_current_arg_count, ferite_objcall_is_self, ferite_objcall_is_super );
										if( ferite_objcall_is_self || ferite_objcall_is_super ) {
											ferite_objcall_is_self = FE_FALSE;
											ferite_objcall_is_super = FE_FALSE;
										}
										ffree_ngc( (yyvsp[(2) - (6)].text) ); FECT;
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
									;}
    break;

  case 334:
#line 1530 "ferite_parser.y"
    { ferite_do_add_variable_to_paramlist( (yyvsp[(1) - (1)].text), F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_VALUE ); ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT; ;}
    break;

  case 341:
#line 1546 "ferite_parser.y"
    { ferite_do_closure_start(); ;}
    break;

  case 342:
#line 1547 "ferite_parser.y"
    { ferite_do_closure_end_of_args(); ;}
    break;

  case 343:
#line 1548 "ferite_parser.y"
    { ferite_do_closure_end(); ;}
    break;

  case 345:
#line 1556 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_add );
										 FUD(("PARSER: op->add		  pop,pop\n"));
									;}
    break;

  case 346:
#line 1560 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_minus );
										 FUD(("PARSER: op->minus		pop,pop\n"));
									;}
    break;

  case 347:
#line 1564 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_mult );
										 FUD(("PARSER: op->mult		 pop,pop\n"));
									;}
    break;

  case 348:
#line 1568 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_divide );
										 FUD(("PARSER: op->divide	   pop,pop\n"));
									;}
    break;

  case 349:
#line 1572 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_modulus );
										 FUD(("PARSER: op->modulus	  pop,pop\n"));
									;}
    break;

  case 350:
#line 1576 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_equals );
										 FUD(("PARSER: op->equals	   pop,pop\n"));
									;}
    break;

  case 351:
#line 1581 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_less_than_equals );
										 FUD(("PARSER: op->less_than_eq pop,pop\n"));
									;}
    break;

  case 352:
#line 1586 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_greater_than_equals );
										 FUD(("PARSER: op->grtr_then_eq pop,pop\n"));
									;}
    break;

  case 353:
#line 1590 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_less_than );
										 FUD(("PARSER: op->less_than	pop,pop\n"));
									;}
    break;

  case 354:
#line 1594 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_greater_than );
										 FUD(("PARSER: op->grtr_then	pop,pop\n"));
									;}
    break;

  case 355:
#line 1598 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_notequals );
										 FUD(("PARSER: op->not_equal	pop,pop\n"));
									;}
    break;

  case 356:
#line 1603 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_left_shift );
										 FUD(("PARSER: op->left_shift	pop,pop\n"));
									;}
    break;

  case 357:
#line 1608 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_right_shift );
										 FUD(("PARSER: op->right_shift	pop,pop\n"));
									;}
    break;

  case 358:
#line 1613 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_binary_or );
										 FUD(("PARSER: op->binary_or	pop,pop\n"));
									;}
    break;

  case 359:
#line 1618 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_binary_and );
										 FUD(("PARSER: op->binary_and   pop,pop\n"));
									;}
    break;

  case 360:
#line 1623 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_binary_xor );
										 FUD(("PARSER: op->binary_xor	pop,pop\n"));
									;}
    break;

  case 361:
#line 1627 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 362:
#line 1627 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_assign, FERITE_OPCODE_assign ); ;}
    break;

  case 363:
#line 1628 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 364:
#line 1628 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_add_assign, FERITE_OPCODE_add ); ;}
    break;

  case 365:
#line 1629 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 366:
#line 1629 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_minus_assign, FERITE_OPCODE_minus ); ;}
    break;

  case 367:
#line 1630 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 368:
#line 1630 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_mult_assign, FERITE_OPCODE_mult ); ;}
    break;

  case 369:
#line 1631 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 370:
#line 1631 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_divide_assign, FERITE_OPCODE_divide ); ;}
    break;

  case 371:
#line 1632 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 372:
#line 1632 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_left_shift_assign, FERITE_OPCODE_left_shift ); ;}
    break;

  case 373:
#line 1633 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 374:
#line 1633 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_right_shift_assign, FERITE_OPCODE_right_shift ); ;}
    break;

  case 375:
#line 1634 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 376:
#line 1634 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_binary_and_assign, FERITE_OPCODE_binary_and ); ;}
    break;

  case 377:
#line 1635 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 378:
#line 1635 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_binary_or_assign, FERITE_OPCODE_binary_or ); ;}
    break;

  case 379:
#line 1636 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 380:
#line 1636 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_binary_xor_assign, FERITE_OPCODE_binary_xor ); ;}
    break;

  case 381:
#line 1638 "ferite_parser.y"
    {
										 int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										 ferite_do_new_object( ferite_current_arg_count );
										 if( value != NULL )
										 {
											 ferite_current_arg_count = *value;
											 ffree_ngc( value );
										 }
										 FUD(("PARSER: new object\n" ));
									;}
    break;

  case 382:
#line 1648 "ferite_parser.y"
    {
										 FUD(("PARSER: unary plus\n"));
										 ferite_do_unary_op( FERITE_OPCODE_positive_var );
									;}
    break;

  case 383:
#line 1653 "ferite_parser.y"
    {
										 FUD(("PARSER: unary minus\n"));
										 ferite_do_unary_op( FERITE_OPCODE_negative_var );
									;}
    break;

  case 384:
#line 1657 "ferite_parser.y"
    {
										 FUD(("PARSER: right increment\n"));
										 ferite_do_unary_op( FERITE_OPCODE_right_incr );
									;}
    break;

  case 385:
#line 1661 "ferite_parser.y"
    {
										 FUD(("PARSER: left increment\n"));
										 ferite_do_unary_op( FERITE_OPCODE_left_incr );
									;}
    break;

  case 386:
#line 1665 "ferite_parser.y"
    {
										 FUD(("PARSER: right decrement\n"));
										 ferite_do_unary_op( FERITE_OPCODE_right_decr );
									;}
    break;

  case 387:
#line 1669 "ferite_parser.y"
    {
										 FUD(("PARSER: left decrement\n"));
										 ferite_do_unary_op( FERITE_OPCODE_left_decr );
									;}
    break;

  case 388:
#line 1673 "ferite_parser.y"
    {
										 ferite_do_unary_op( FERITE_OPCODE_not_op );
										 FUD(("PARSER: op->not_op		   pop\n" ));
									;}
    break;

  case 389:
#line 1677 "ferite_parser.y"
    {
										 ferite_do_unary_op( FERITE_OPCODE_eval );
										 FUD(("PARSER: op->eval\n"));
									;}
    break;

  case 390:
#line 1681 "ferite_parser.y"
    {
										 ferite_do_unary_op( FERITE_OPCODE_include );
										 FUD(("PARSER: op->include\n"));
									;}
    break;

  case 391:
#line 1685 "ferite_parser.y"
    {
										 ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, "strconst", ferite_variable_id_to_str( NULL,ferite_last_type), 0, FE_CHARSET_DEFAULT, FE_STATIC );
										 MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										 ferite_do_push( ferite_temp_variable );
										 ferite_do_binary_op( FERITE_OPCODE_isa );
										 FUD(("PARSER: op->include\n"));
									;}
    break;

  case 392:
#line 1692 "ferite_parser.y"
    {
										 ferite_do_binary_op( FERITE_OPCODE_instanceof );
										 FUD(("PARSER: op->include\n"));
									;}
    break;

  case 393:
#line 1696 "ferite_parser.y"
    { ferite_do_get_args(); ;}
    break;

  case 394:
#line 1697 "ferite_parser.y"
    { ferite_do_get_yield_block(); ;}
    break;

  case 404:
#line 1710 "ferite_parser.y"
    { ferite_do_if_statement(); ;}
    break;

  case 405:
#line 1711 "ferite_parser.y"
    { ferite_do_after_then_before_else_statement(); ;}
    break;

  case 406:
#line 1712 "ferite_parser.y"
    { ferite_do_after_else_statement(); ;}
    break;

  case 408:
#line 1717 "ferite_parser.y"
    { ferite_do_yield_block( FE_FALSE ); ;}
    break;

  case 409:
#line 1718 "ferite_parser.y"
    { ferite_do_yield_block( FE_TRUE ); ;}
    break;

  case 410:
#line 1722 "ferite_parser.y"
    { ferite_do_push_main_namespace(); ;}
    break;

  case 411:
#line 1724 "ferite_parser.y"
    {
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										FUD(("PARSER: op->c_func	   %s\n", (yyvsp[(1) - (6)].text) ));
										ferite_do_function_call( (yyvsp[(1) - (6)].text), ferite_current_arg_count );
										ffree_ngc( (yyvsp[(1) - (6)].text) ); FECT;
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
									;}
    break;

  case 412:
#line 1735 "ferite_parser.y"
    { ferite_do_super_push(); ;}
    break;

  case 413:
#line 1737 "ferite_parser.y"
    {
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										ferite_do_object_function_call( "constructor", ferite_current_arg_count, FE_FALSE, FE_TRUE );
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
									;}
    break;

  case 414:
#line 1746 "ferite_parser.y"
    { ferite_do_self_push(); ;}
    break;

  case 415:
#line 1748 "ferite_parser.y"
    {
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										ferite_do_object_function_call( "constructor", ferite_current_arg_count, FE_TRUE, FE_FALSE );
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
									;}
    break;

  case 416:
#line 1758 "ferite_parser.y"
    {
										int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
										FUD(("PARSER: op->yield\n"));
										ferite_do_yield_call( ferite_current_arg_count );
										if( value != NULL )
										{
											ferite_current_arg_count = *value;
											ffree_ngc( value );
										}
						;}
    break;

  case 417:
#line 1771 "ferite_parser.y"
    {
										int *value = fmalloc_ngc(sizeof(int));
										*value = ferite_current_arg_count;
										ferite_stack_push( FE_NoScript, ferite_argcount_stack, value );
										ferite_current_arg_count = 0;
									;}
    break;

  case 418:
#line 1777 "ferite_parser.y"
    {
										int *value = fmalloc_ngc(sizeof(int));
										*value = ferite_current_arg_count;
										ferite_stack_push( FE_NoScript, ferite_argcount_stack, value );
										ferite_current_arg_count = 0;
									;}
    break;

  case 422:
#line 1792 "ferite_parser.y"
    { ferite_current_arg_count++ ;}
    break;

  case 424:
#line 1796 "ferite_parser.y"
    {
										char *buf = fmalloc_ngc(256);
										ferite_var_array_count++;
										snprintf( buf, 256, "!__array%d__", ferite_var_array_count );
										ferite_do_add_variable( buf, F_VAR_UARRAY, ferite_subtype_link(CURRENT_SCRIPT, "A"), FE_FALSE, FE_FALSE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );
										ferite_stack_push( FE_NoScript, ferite_compiled_arrays_stack, buf );
										FUD(("PARSER: push			 array_start\n"));

										ferite_do_variable_push( buf );
										ferite_do_unary_op( FERITE_OPCODE_array_clear );
										ferite_do_pop();
									;}
    break;

  case 425:
#line 1808 "ferite_parser.y"
    {
										char *buf = ferite_stack_pop( FE_NoScript, ferite_compiled_arrays_stack );
										ferite_do_variable_push( buf );
										ffree_ngc( buf );
									;}
    break;

  case 430:
#line 1827 "ferite_parser.y"
    {
										ferite_do_variable_push( ferite_stack_top( FE_NoScript, ferite_compiled_arrays_stack ) );
										ferite_do_swap_top(0);
										ferite_do_swap_top(1);
										ferite_do_swap_top(0);
										ferite_do_many_op( FERITE_OPCODE_array_index_assign, 3 );
										ferite_do_pop();
									;}
    break;

  case 431:
#line 1836 "ferite_parser.y"
    {
										ferite_temp_variable = ferite_create_void_variable( NULL, "void-placeholder", FE_STATIC );
										MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
									;}
    break;

  case 432:
#line 1842 "ferite_parser.y"
    {   ferite_do_swap_top(0); ;}
    break;

  case 433:
#line 1846 "ferite_parser.y"
    { ferite_do_if_statement(); ;}
    break;

  case 434:
#line 1847 "ferite_parser.y"
    {
										ferite_do_after_then_before_else_statement();
										/* we have to push a value onto the stack, otherwise we end up with a smashed stack */
										ferite_temp_variable = ferite_create_boolean_variable( NULL, "false", FE_FALSE, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_after_else_statement();
								   ;}
    break;

  case 435:
#line 1855 "ferite_parser.y"
    { ferite_do_not_if_statement(); ;}
    break;

  case 436:
#line 1856 "ferite_parser.y"
    {
										ferite_do_after_then_before_else_statement();
										/* we have to push a value onto the stack, otherwise we end up with a smashed stack */
										ferite_temp_variable = ferite_create_boolean_variable( NULL, "true", FE_TRUE, FE_STATIC );
										MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
										ferite_do_push( ferite_temp_variable );
										ferite_do_after_else_statement();
								   ;}
    break;


/* Line 1267 of yacc.c.  */
#line 4832 "ferite_parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1866 "ferite_parser.y"


int ferite_parse()
{
	 int parser_return = 0;

	 FE_ENTER_FUNCTION;
	 parser_return = fepparse();
	 FE_LEAVE_FUNCTION( parser_return );
}


