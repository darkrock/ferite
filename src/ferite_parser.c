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
     T_VAR_AUTO = 354,
     T_DIRECTIVE = 355,
     T_RAISE = 356,
     T_CURRENT_LINE = 357,
     T_CURRENT_FILE = 358,
     T_EXPANSION = 359,
     T_FUNCTION_JUMP = 360,
     T_RET_UNDEF = 361,
     T_QSTRING = 362,
     T_LABEL = 363,
     T_OBJPROPERTY = 364,
     T_NATIVE_CODE_BLOCK = 365,
     T_DOT_LABEL = 366,
     T_DIRECTIVE_LABEL = 367,
     T_NNUMBER = 368,
     T_TRUE = 369,
     T_FALSE = 370,
     T_RNUMBER = 371
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
#define T_VAR_AUTO 354
#define T_DIRECTIVE 355
#define T_RAISE 356
#define T_CURRENT_LINE 357
#define T_CURRENT_FILE 358
#define T_EXPANSION 359
#define T_FUNCTION_JUMP 360
#define T_RET_UNDEF 361
#define T_QSTRING 362
#define T_LABEL 363
#define T_OBJPROPERTY 364
#define T_NATIVE_CODE_BLOCK 365
#define T_DOT_LABEL 366
#define T_DIRECTIVE_LABEL 367
#define T_NNUMBER 368
#define T_TRUE 369
#define T_FALSE 370
#define T_RNUMBER 371




/* Copy the first part of user declarations.  */
#line 1 "ferite_parser.y"

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
extern int        ferite_scanner_lineno;
extern char       *ferite_scanner_file;
extern int        ferite_compile_error;
extern char       *ferite_last_token_alloc;
extern FeriteCompileRecord *ferite_current_compile;

extern FeriteStack *ferite_compiled_arrays_stack;
extern FeriteStack *ferite_argcount_stack;

char           *ferite_last_function = NULL;
int                 ferite_last_type = 0;
FeriteString   *ferite_last_clamp = NULL;
FeriteVariable *ferite_temp_variable = NULL;
char            ferite_fetypebuf[1024];
char            ferite_fenamebuf[1024];
int                 ferite_var_is_global = FE_FALSE;
int                 ferite_var_is_params = FE_FALSE;
int                 ferite_var_is_final = FE_FALSE;
int                 ferite_var_is_static = FE_FALSE;
int                 ferite_var_is_atomic = FE_FALSE;
int                 ferite_var_is_native = FE_FALSE;
int                 ferite_var_pass_type = FE_BY_VALUE;
int             ferite_var_type_is_deprecated = FE_FALSE;
int                 ferite_item_state = FE_ITEM_IS_PRIVATE;
int                 ferite_class_state = 0;
int                 ferite_current_arg_count = 0;
int                 ferite_var_array_count = 0;
long             ferite_namespace_naturaln = 0;
int                 ferite_function_is_directive = 0;
FeriteStack       *ferite_directive_parameters = NULL;
FeriteHash       *ferite_expansion_hash = NULL;
FeriteHash       *ferite_expansion_map_hash = NULL;
long             ferite_expansion_map_offset = 0;
int                 ferite_objcall_is_self = FE_FALSE;
int                 ferite_objcall_is_super = FE_FALSE;
int                ferite_function_return_type = F_VAR_VOID;
FeriteOp       *ferite_assign_op_to_change[128];
int                 ferite_assign_is_array[128];
int                 ferite_assign_loc = 0;
int                ferite_return_type_was_provided = FE_FALSE;
int                ferite_seeking_return_type_fix = FE_FALSE;
int                ferite_hint_depth = 0;
char            ferite_hint_typestring[512];
FeriteVariableSubType *ferite_hint_current_type = NULL;

int  feplex();
void feperror( char *message );
void ferite_scanner_go_native();

void ferite_lexer_fix_native_blocks();
void ferite_scanner_want_end_hint();
void ferite_scanner_stop_end_hint();

#define FECT    ferite_last_token_alloc = NULL

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
#line 134 "ferite_parser.y"
{
    char   *text;
    long    inumber;
    double  fnumber;
}
/* Line 193 of yacc.c.  */
#line 475 "ferite_parser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 488 "ferite_parser.tab.c"

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
#define YYLAST   1759

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  439
/* YYNRULES -- Number of states.  */
#define YYNSTATES  651

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   371

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     124,   125,     2,     2,     3,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   123,   118,
       2,     2,     2,   126,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   119,     2,   120,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   121,     2,   122,     2,     2,     2,     2,
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
     116,   117
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
     111,   113,   117,   120,   123,   124,   130,   131,   136,   137,
     139,   141,   143,   145,   148,   149,   155,   156,   162,   164,
     166,   168,   170,   172,   174,   176,   178,   180,   182,   184,
     186,   187,   189,   192,   195,   198,   201,   203,   204,   208,
     210,   212,   213,   217,   218,   222,   223,   228,   230,   232,
     234,   235,   241,   242,   244,   245,   251,   253,   254,   256,
     260,   262,   264,   268,   271,   275,   276,   278,   280,   283,
     287,   290,   292,   294,   296,   298,   303,   304,   306,   308,
     311,   312,   315,   319,   322,   324,   326,   328,   329,   334,
     335,   339,   340,   343,   345,   349,   350,   353,   359,   364,
     368,   370,   372,   375,   378,   380,   383,   384,   386,   387,
     390,   395,   397,   398,   403,   406,   409,   411,   413,   417,
     421,   425,   429,   433,   435,   439,   441,   445,   447,   449,
     450,   452,   454,   456,   458,   460,   462,   464,   466,   467,
     469,   471,   474,   476,   477,   480,   484,   486,   487,   489,
     491,   493,   495,   497,   499,   501,   503,   505,   507,   509,
     511,   513,   515,   517,   518,   519,   520,   527,   528,   529,
     536,   537,   538,   544,   545,   547,   551,   556,   557,   563,
     564,   570,   572,   574,   576,   577,   579,   580,   582,   584,
     587,   589,   591,   593,   595,   597,   600,   603,   605,   607,
     610,   613,   616,   620,   622,   624,   626,   629,   630,   634,
     635,   636,   637,   638,   649,   650,   658,   660,   662,   663,
     664,   668,   669,   678,   681,   682,   684,   686,   689,   690,
     696,   697,   698,   703,   704,   705,   709,   710,   712,   714,
     716,   718,   719,   720,   721,   722,   723,   724,   740,   741,
     742,   750,   751,   760,   762,   764,   766,   768,   770,   772,
     774,   776,   778,   780,   782,   784,   786,   788,   795,   796,
     803,   809,   814,   819,   823,   826,   829,   830,   833,   835,
     838,   840,   843,   845,   848,   851,   853,   860,   862,   864,
     866,   868,   872,   873,   877,   878,   879,   884,   887,   891,
     895,   899,   903,   907,   911,   915,   919,   923,   927,   931,
     935,   939,   943,   947,   951,   952,   957,   958,   963,   964,
     969,   970,   975,   976,   981,   982,   987,   988,   993,   994,
     999,  1000,  1005,  1006,  1011,  1017,  1020,  1023,  1026,  1029,
    1032,  1035,  1038,  1043,  1048,  1052,  1056,  1060,  1064,  1066,
    1068,  1071,  1073,  1076,  1078,  1080,  1084,  1089,  1090,  1091,
    1101,  1102,  1105,  1110,  1111,  1118,  1119,  1126,  1127,  1134,
    1139,  1140,  1141,  1144,  1146,  1150,  1151,  1154,  1155,  1160,
    1161,  1163,  1165,  1169,  1172,  1173,  1176,  1177,  1182,  1183
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     128,     0,    -1,    -1,   131,   129,   130,    -1,   106,    -1,
     218,   239,    -1,    -1,   132,    -1,   133,    -1,   132,   133,
      -1,   141,    -1,   157,    -1,   206,    -1,   198,    -1,   182,
      -1,   134,    -1,   216,   170,    -1,    -1,    68,   135,   111,
      -1,    -1,    69,   136,   111,    -1,    -1,    74,   137,   111,
      -1,    -1,    75,   138,   111,    -1,    -1,    70,   139,   111,
      -1,    -1,    71,   140,   111,    -1,    55,   143,   118,    -1,
     108,    -1,   142,    -1,   143,     3,   142,    -1,    -1,   146,
      -1,   108,    -1,   109,    -1,   117,    -1,   114,    -1,    92,
      -1,    97,    -1,    93,    -1,    94,    -1,   100,    -1,    95,
      -1,    96,    -1,    60,    -1,    59,    -1,   115,    -1,   116,
      -1,   145,    -1,   146,     3,   145,    -1,   146,   145,    -1,
     148,   118,    -1,    -1,   119,   109,   149,   144,   120,    -1,
      -1,   105,   155,   151,   111,    -1,    -1,   154,    -1,   109,
      -1,   113,    -1,   153,    -1,   154,   153,    -1,    -1,   119,
     109,   156,   152,   120,    -1,    -1,    56,   158,   121,   231,
     122,    -1,    92,    -1,    98,    -1,    99,    -1,    93,    -1,
      94,    -1,   100,    -1,    95,    -1,    96,    -1,    97,    -1,
     159,    -1,    60,    -1,    59,    -1,    -1,    46,    -1,    46,
     109,    -1,   101,   109,    -1,   161,    90,    -1,   161,    91,
      -1,   162,    -1,    -1,   160,   165,   222,    -1,   112,    -1,
     107,    -1,    -1,   123,   167,   164,    -1,    -1,   124,   176,
     125,    -1,    -1,   121,   218,   239,   122,    -1,   111,    -1,
     246,    -1,   106,    -1,    -1,   163,   168,   166,   171,   169,
      -1,    -1,    18,    -1,    -1,   160,   174,   222,   172,   109,
      -1,    65,    -1,    -1,   177,    -1,   177,     3,   175,    -1,
     175,    -1,   173,    -1,   177,     3,   173,    -1,    59,   109,
      -1,    59,    66,   109,    -1,    -1,   180,    -1,   181,    -1,
     180,   181,    -1,   216,   228,   118,    -1,   216,   170,    -1,
     206,    -1,   198,    -1,   182,    -1,   147,    -1,   178,   121,
     179,   122,    -1,    -1,   184,    -1,   185,    -1,   184,   185,
      -1,    -1,   186,   187,    -1,   216,   228,   118,    -1,   216,
     170,    -1,   147,    -1,   150,    -1,   109,    -1,    -1,   109,
      61,   189,   112,    -1,    -1,    66,   191,   112,    -1,    -1,
     193,   112,    -1,   192,    -1,   194,     3,   192,    -1,    -1,
      87,   194,    -1,   188,   195,   121,   183,   122,    -1,   190,
     121,   183,   122,    -1,   214,    60,   199,    -1,   196,    -1,
     197,    -1,    86,   109,    -1,   216,   170,    -1,   201,    -1,
     202,   201,    -1,    -1,   202,    -1,    -1,   205,   203,    -1,
     200,   121,   204,   122,    -1,   109,    -1,    -1,   109,    13,
     208,   282,    -1,    30,   114,    -1,    31,   114,    -1,   114,
      -1,   109,    -1,   109,    13,   209,    -1,   109,    13,   117,
      -1,   109,    13,   108,    -1,   109,    13,   115,    -1,   109,
      13,   116,    -1,   207,    -1,   211,     3,   207,    -1,   210,
      -1,   212,     3,   210,    -1,    58,    -1,    81,    -1,    -1,
     213,    -1,    58,    -1,    64,    -1,    73,    -1,    67,    -1,
      78,    -1,    79,    -1,    80,    -1,    -1,   217,    -1,   215,
      -1,   217,   215,    -1,   219,    -1,    -1,   225,   118,    -1,
     219,   225,   118,    -1,    58,    -1,    -1,   112,    -1,   108,
      -1,   117,    -1,   114,    -1,    92,    -1,    98,    -1,    99,
      -1,    93,    -1,    94,    -1,   100,    -1,    95,    -1,    96,
      -1,    97,    -1,    60,    -1,    59,    -1,    -1,    -1,    -1,
      22,   223,   221,   224,   222,    21,    -1,    -1,    -1,   220,
     159,   226,   222,   227,   211,    -1,    -1,    -1,   159,   229,
     222,   230,   212,    -1,    -1,   232,    -1,   216,   228,   118,
      -1,   232,   216,   228,   118,    -1,    -1,   121,   234,   218,
     239,   122,    -1,    -1,   121,   236,   218,   240,   122,    -1,
     246,    -1,   233,    -1,   239,    -1,    -1,   244,    -1,    -1,
     241,    -1,   242,    -1,   241,   242,    -1,   245,    -1,   233,
      -1,   246,    -1,   233,    -1,   243,    -1,   244,   243,    -1,
     282,   118,    -1,   254,    -1,   272,    -1,   247,   118,    -1,
      62,   118,    -1,    63,   118,    -1,   102,   282,   118,    -1,
     245,    -1,   118,    -1,    57,    -1,    57,   282,    -1,    -1,
     124,   109,   125,    -1,    -1,    -1,    -1,    -1,    51,   250,
     233,   251,    52,   252,   248,   235,   253,   267,    -1,    -1,
      42,   124,   282,   125,   255,   237,   256,    -1,   249,    -1,
     258,    -1,    -1,    -1,    43,   257,   237,    -1,    -1,    50,
     259,   124,   282,   125,   121,   260,   122,    -1,   261,   265,
      -1,    -1,   262,    -1,   263,    -1,   262,   263,    -1,    -1,
      53,   282,   264,   123,   238,    -1,    -1,    -1,    54,   123,
     266,   239,    -1,    -1,    -1,    43,   268,   237,    -1,    -1,
     282,    -1,   269,    -1,   225,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,   124,   273,   270,   274,   118,
     275,   269,   276,   118,   277,   269,   278,   125,   237,    -1,
      -1,    -1,    48,   279,   124,   282,   125,   280,   237,    -1,
      -1,    49,   281,   237,    48,   124,   282,   125,   118,    -1,
     271,    -1,   284,    -1,   283,    -1,   299,    -1,   108,    -1,
     115,    -1,   116,    -1,   114,    -1,   117,    -1,   103,    -1,
     104,    -1,   109,    -1,    88,    -1,    89,    -1,   284,   119,
     282,    72,   282,   120,    -1,    -1,   284,   119,    72,   285,
     282,   120,    -1,   284,   119,   282,    72,   120,    -1,   284,
     119,    72,   120,    -1,   284,   119,   282,   120,    -1,   284,
     119,   120,    -1,   284,   287,    -1,   283,   287,    -1,    -1,
     286,   287,    -1,   291,    -1,   287,   291,    -1,   290,    -1,
     288,   290,    -1,   109,    -1,   289,   288,    -1,    40,   110,
      -1,   290,    -1,    40,   110,   124,   318,   125,   313,    -1,
     109,    -1,   173,    -1,   175,    -1,   292,    -1,   293,     3,
     292,    -1,    -1,   124,   293,   125,    -1,    -1,    -1,   296,
     294,   297,   233,    -1,    82,   295,    -1,   282,    31,   282,
      -1,   282,    30,   282,    -1,   282,    34,   282,    -1,   282,
      33,   282,    -1,   282,    32,   282,    -1,   282,    25,   282,
      -1,   282,    24,   282,    -1,   282,    23,   282,    -1,   282,
      22,   282,    -1,   282,    21,   282,    -1,   282,    20,   282,
      -1,   282,    29,   282,    -1,   282,    28,   282,    -1,   282,
      17,   282,    -1,   282,    18,   282,    -1,   282,    19,   282,
      -1,    -1,   284,    13,   300,   282,    -1,    -1,   284,    12,
     301,   282,    -1,    -1,   284,    11,   302,   282,    -1,    -1,
     284,    10,   303,   282,    -1,    -1,   284,     9,   304,   282,
      -1,    -1,   284,     8,   305,   282,    -1,    -1,   284,     7,
     306,   282,    -1,    -1,   284,     6,   307,   282,    -1,    -1,
     284,     5,   308,   282,    -1,    -1,   284,     4,   309,   282,
      -1,    41,   289,   124,   318,   125,    -1,    31,   282,    -1,
      30,   282,    -1,   284,    37,    -1,    37,   284,    -1,   284,
      36,    -1,    36,   284,    -1,    35,   282,    -1,    45,   124,
     282,   125,    -1,    44,   124,   282,   125,    -1,   282,    27,
     159,    -1,   282,    26,   289,    -1,    77,   124,   125,    -1,
      85,   124,   125,    -1,   310,    -1,   323,    -1,   323,   287,
      -1,   314,    -1,   314,   287,    -1,   329,    -1,   298,    -1,
     124,   282,   125,    -1,   124,   282,   125,   287,    -1,    -1,
      -1,   124,   282,   126,   311,   282,   123,   312,   282,   125,
      -1,    -1,    83,   295,    -1,    83,    85,   124,   125,    -1,
      -1,   109,   315,   124,   318,   125,   313,    -1,    -1,    89,
     316,   124,   318,   125,   313,    -1,    -1,    88,   317,   124,
     318,   125,   313,    -1,    84,   124,   318,   125,    -1,    -1,
      -1,   319,   320,    -1,   321,    -1,   320,     3,   321,    -1,
      -1,   322,   282,    -1,    -1,   119,   324,   325,   120,    -1,
      -1,   326,    -1,   327,    -1,   326,     3,   327,    -1,   282,
     328,    -1,    -1,    15,   282,    -1,    -1,   282,    16,   330,
     282,    -1,    -1,   282,    14,   331,   282,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   183,   183,   183,   194,   195,   204,   206,   210,   211,
     215,   216,   217,   218,   219,   220,   221,   225,   225,   227,
     227,   229,   229,   231,   231,   233,   233,   235,   235,   240,
     244,   248,   249,   252,   254,   258,   262,   266,   268,   270,
     273,   276,   279,   282,   285,   288,   291,   294,   297,   299,
     304,   305,   306,   310,   314,   314,   352,   352,   358,   360,
     364,   365,   373,   374,   379,   378,   393,   393,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   411,   412,   413,
     416,   418,   422,   423,   424,   426,   431,   460,   460,   467,
     475,   483,   483,   487,   517,   521,   525,   536,   541,   550,
     557,   554,   570,   572,   576,   576,   602,   608,   610,   611,
     612,   616,   617,   621,   622,   625,   627,   631,   632,   636,
     637,   638,   639,   640,   641,   645,   649,   651,   655,   656,
     660,   660,   665,   666,   667,   668,   672,   678,   678,   692,
     692,   705,   705,   716,   717,   720,   722,   726,   735,   742,
     748,   749,   753,   761,   765,   766,   769,   771,   775,   775,
     779,   787,   798,   798,   818,   819,   820,   824,   831,   851,
     871,   893,   913,   937,   938,   942,   943,   947,   956,   967,
     969,   973,   982,   991,  1000,  1009,  1018,  1027,  1038,  1040,
    1044,  1045,  1049,  1050,  1054,  1055,  1059,  1068,  1072,  1082,
    1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,
    1093,  1094,  1095,  1099,  1105,  1110,  1105,  1119,  1131,  1119,
    1146,  1159,  1146,  1168,  1170,  1174,  1175,  1180,  1180,  1186,
    1186,  1193,  1194,  1198,  1201,  1203,  1206,  1210,  1213,  1214,
    1217,  1218,  1222,  1223,  1227,  1228,  1232,  1233,  1234,  1235,
    1236,  1237,  1238,  1242,  1243,  1247,  1254,  1257,  1259,  1268,
    1269,  1270,  1272,  1268,  1277,  1277,  1279,  1280,  1284,  1285,
    1285,  1290,  1290,  1302,  1306,  1308,  1312,  1313,  1319,  1318,
    1327,  1329,  1329,  1336,  1337,  1337,  1342,  1348,  1352,  1353,
    1357,  1365,  1369,  1373,  1374,  1375,  1379,  1365,  1391,  1392,
    1391,  1394,  1394,  1398,  1402,  1403,  1404,  1408,  1416,  1423,
    1430,  1437,  1444,  1450,  1459,  1464,  1465,  1466,  1471,  1471,
    1478,  1485,  1495,  1499,  1509,  1510,  1511,  1511,  1516,  1517,
    1521,  1522,  1526,  1531,  1535,  1547,  1548,  1568,  1569,  1570,
    1574,  1575,  1578,  1580,  1584,  1585,  1584,  1590,  1594,  1598,
    1602,  1606,  1610,  1614,  1618,  1623,  1628,  1632,  1636,  1640,
    1645,  1650,  1655,  1660,  1665,  1665,  1666,  1666,  1667,  1667,
    1668,  1668,  1669,  1669,  1670,  1670,  1671,  1671,  1672,  1672,
    1673,  1673,  1674,  1674,  1675,  1686,  1690,  1695,  1699,  1703,
    1707,  1711,  1715,  1719,  1723,  1730,  1734,  1735,  1736,  1737,
    1738,  1739,  1740,  1741,  1742,  1743,  1744,  1748,  1749,  1748,
    1753,  1755,  1756,  1760,  1760,  1773,  1773,  1784,  1784,  1795,
    1809,  1815,  1815,  1825,  1826,  1830,  1830,  1834,  1834,  1853,
    1855,  1859,  1860,  1864,  1874,  1880,  1884,  1884,  1893,  1893
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
  "T_VAR_NUMBER_DBL", "T_VAR_AUTO", "T_DIRECTIVE", "T_RAISE",
  "T_CURRENT_LINE", "T_CURRENT_FILE", "T_EXPANSION", "T_FUNCTION_JUMP",
  "T_RET_UNDEF", "T_QSTRING", "T_LABEL", "T_OBJPROPERTY",
  "T_NATIVE_CODE_BLOCK", "T_DOT_LABEL", "T_DIRECTIVE_LABEL", "T_NNUMBER",
  "T_TRUE", "T_FALSE", "T_RNUMBER", "';'", "'['", "']'", "'{'", "'}'",
  "':'", "'('", "')'", "'?'", "$accept", "script", "@1", "script_body",
  "script_block_list", "non_empty_script_block_list", "script_block",
  "builder_item", "@2", "@3", "@4", "@5", "@6", "@7", "uses_list",
  "module_item", "module_list", "directive_list", "directive_item",
  "non_empty_directive_list", "directive_decl",
  "directive_decl_with_out_semi_colon", "@8", "expansion_decl", "@9",
  "directive_expansion_capture_mask_arguments",
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
     364,   365,   366,   367,   368,   369,   370,   371,    59,    91,
      93,   123,   125,    58,    40,    41,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   127,   129,   128,   130,   130,   131,   131,   132,   132,
     133,   133,   133,   133,   133,   133,   133,   135,   134,   136,
     134,   137,   134,   138,   134,   139,   134,   140,   134,   141,
     142,   143,   143,   144,   144,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     146,   146,   146,   147,   149,   148,   151,   150,   152,   152,
     153,   153,   154,   154,   156,   155,   158,   157,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   160,   160,   160,
     161,   161,   162,   162,   162,   162,   163,   165,   164,   164,
     164,   167,   166,   166,   168,   168,   169,   169,   169,   169,
     171,   170,   172,   172,   174,   173,   175,   176,   176,   176,
     176,   177,   177,   178,   178,   179,   179,   180,   180,   181,
     181,   181,   181,   181,   181,   182,   183,   183,   184,   184,
     186,   185,   187,   187,   187,   187,   188,   189,   188,   191,
     190,   193,   192,   194,   194,   195,   195,   196,   197,   198,
     199,   199,   200,   201,   202,   202,   203,   203,   205,   204,
     206,   207,   208,   207,   209,   209,   209,   210,   210,   210,
     210,   210,   210,   211,   211,   212,   212,   213,   213,   214,
     214,   215,   215,   215,   215,   215,   215,   215,   216,   216,
     217,   217,   218,   218,   219,   219,   220,   220,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   222,   223,   224,   222,   226,   227,   225,
     229,   230,   228,   231,   231,   232,   232,   234,   233,   236,
     235,   237,   237,   238,   239,   239,   240,   240,   241,   241,
     242,   242,   243,   243,   244,   244,   245,   245,   245,   245,
     245,   245,   245,   246,   246,   247,   247,   248,   248,   250,
     251,   252,   253,   249,   255,   254,   254,   254,   256,   257,
     256,   259,   258,   260,   261,   261,   262,   262,   264,   263,
     265,   266,   265,   267,   268,   267,   269,   269,   270,   270,
     271,   273,   274,   275,   276,   277,   278,   272,   279,   280,
     272,   281,   272,   272,   282,   282,   282,   283,   283,   283,
     283,   283,   283,   283,   284,   284,   284,   284,   285,   284,
     284,   284,   284,   284,   284,   284,   286,   284,   287,   287,
     288,   288,   289,   289,   290,   291,   291,   292,   292,   292,
     293,   293,   294,   294,   296,   297,   295,   298,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   300,   299,   301,   299,   302,   299,
     303,   299,   304,   299,   305,   299,   306,   299,   307,   299,
     308,   299,   309,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   311,   312,   310,
     313,   313,   313,   315,   314,   316,   314,   317,   314,   314,
     318,   319,   318,   320,   320,   322,   321,   324,   323,   325,
     325,   326,   326,   327,   328,   328,   330,   329,   331,   329
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     1,     2,     0,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     2,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     3,
       1,     1,     3,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     2,     2,     0,     5,     0,     4,     0,     1,
       1,     1,     1,     2,     0,     5,     0,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     2,     2,     2,     2,     1,     0,     3,     1,
       1,     0,     3,     0,     3,     0,     4,     1,     1,     1,
       0,     5,     0,     1,     0,     5,     1,     0,     1,     3,
       1,     1,     3,     2,     3,     0,     1,     1,     2,     3,
       2,     1,     1,     1,     1,     4,     0,     1,     1,     2,
       0,     2,     3,     2,     1,     1,     1,     0,     4,     0,
       3,     0,     2,     1,     3,     0,     2,     5,     4,     3,
       1,     1,     2,     2,     1,     2,     0,     1,     0,     2,
       4,     1,     0,     4,     2,     2,     1,     1,     3,     3,
       3,     3,     3,     1,     3,     1,     3,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     2,     1,     0,     2,     3,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     0,     0,     6,     0,     0,     6,
       0,     0,     5,     0,     1,     3,     4,     0,     5,     0,
       5,     1,     1,     1,     0,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     2,
       2,     2,     3,     1,     1,     1,     2,     0,     3,     0,
       0,     0,     0,    10,     0,     7,     1,     1,     0,     0,
       3,     0,     8,     2,     0,     1,     1,     2,     0,     5,
       0,     0,     4,     0,     0,     3,     0,     1,     1,     1,
       1,     0,     0,     0,     0,     0,     0,    15,     0,     0,
       7,     0,     8,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     0,     6,
       5,     4,     4,     3,     2,     2,     0,     2,     1,     2,
       1,     2,     1,     2,     2,     1,     6,     1,     1,     1,
       1,     3,     0,     3,     0,     0,     4,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     5,     2,     2,     2,     2,     2,
       2,     2,     4,     4,     3,     3,     3,     3,     1,     1,
       2,     1,     2,     1,     1,     3,     4,     0,     0,     9,
       0,     2,     4,     0,     6,     0,     6,     0,     6,     4,
       0,     0,     2,     1,     3,     0,     2,     0,     4,     0,
       1,     1,     3,     2,     0,     2,     0,     4,     0,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       6,     0,    66,   181,     0,   182,   184,    17,    19,    25,
      27,   183,    21,    23,   185,   186,   187,   178,     0,     0,
       2,     7,     8,    15,    10,    11,     0,    14,    13,     0,
      12,   180,     0,   190,    80,   189,    30,    31,     0,     0,
       0,   113,     0,     0,     0,     0,     0,     0,   152,     1,
     193,     9,   188,   158,     0,    81,     0,     0,    86,    95,
      16,   181,   191,     0,    29,   188,   114,    18,    20,    26,
      28,    22,    24,   196,     4,     3,   234,   192,     0,     0,
       0,   124,     0,     0,   188,   117,   123,   122,   121,    80,
       0,   188,   139,   136,   145,     0,   150,   151,   149,    82,
      83,    84,    85,   107,    93,    32,     0,     0,   188,   326,
     326,   326,   326,   326,     0,     0,     0,     0,     0,   298,
     301,   271,   259,   255,     0,     0,   290,     0,   344,     0,
       0,   315,   316,   326,   312,   313,   307,   314,   310,   308,
     309,   311,   254,   427,   227,   326,   243,     5,   244,   235,
     253,   242,     0,   266,   247,   267,   303,   248,     0,   305,
     304,     0,   404,   306,   398,   401,   399,   403,     0,    68,
      71,    72,    74,    75,    76,    69,    70,    73,   217,   194,
      54,    53,   125,   118,   220,   120,     0,   160,   154,   188,
     159,    80,     0,   137,   141,     0,   130,    79,    78,   106,
      77,   104,   111,   110,     0,   108,    91,   100,     0,    67,
       0,   386,   385,   391,   315,   316,   314,     0,   390,   388,
     332,     0,   326,   326,   326,   291,     0,   326,     0,     0,
     256,   250,   251,     0,   347,   342,   421,     0,     0,     0,
       0,     0,   326,   193,     0,   245,   249,   438,   436,   326,
     326,   326,   326,   326,   326,   326,   326,   326,     0,     0,
     326,   326,   326,   326,   326,   326,   326,   246,     0,   325,
     335,   328,   382,   380,   378,   376,   374,   372,   370,   368,
     366,   364,   389,   387,   326,   324,   327,   402,   400,   195,
     213,    33,   213,   119,   155,   153,   140,     0,   143,     0,
     146,   130,     0,   130,   128,   188,   213,    94,     0,     0,
     326,   225,     0,     0,   421,   333,   330,     0,     0,     0,
     197,   326,   232,     0,   231,   326,   260,   396,     0,   345,
       0,   425,   397,   421,   421,   252,   421,   434,     0,   430,
     431,   234,   405,   407,   326,   326,   361,   362,   363,   358,
     357,   356,   355,   354,   353,   395,   394,   360,   359,   349,
     348,   352,   351,   350,   334,   329,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   318,   323,     0,   214,
     218,    47,    46,    39,    41,    42,    44,    45,    40,    43,
      35,    36,    38,    48,    49,    37,     0,    50,    34,   221,
     138,   142,   141,     0,   148,   129,     0,   134,   135,   131,
      80,   102,   112,   109,    90,    89,    87,    92,    99,    97,
     193,   101,    98,   226,   334,     0,   331,   264,   393,   392,
     289,   288,   292,   287,     0,     0,     0,     0,   337,   338,
     339,   340,     0,     0,   419,   422,   423,   326,     0,     0,
       0,   326,   433,   428,   326,     0,   406,   326,   439,   437,
     421,   383,   381,   379,   377,   375,   373,   371,   369,   367,
     365,   321,   326,   326,   322,     0,     0,    55,     0,    52,
       0,   144,   147,     0,    56,   133,     0,   103,     0,   213,
     234,   384,   326,     0,   299,   326,     0,   261,     0,   343,
     346,   425,   426,   410,   410,   410,   435,   432,   228,     0,
       0,     0,   320,     0,   212,   211,   202,   205,   206,   208,
     209,   210,   203,   204,   207,   199,   198,   201,   200,   215,
     161,   173,   219,    51,   167,   175,   222,    64,     0,   132,
     105,    88,     0,   268,   293,   326,     0,   274,   257,   341,
     424,   344,   418,   416,   414,   408,   410,   319,   317,   213,
     162,     0,     0,     0,    58,    57,    96,   269,   265,   286,
     300,     0,   326,     0,   280,   275,   276,     0,     0,     0,
     411,   326,   336,     0,   326,   174,     0,     0,   170,   166,
     171,   172,   169,   168,   176,    60,    61,     0,    62,    59,
     326,   294,   302,   278,   272,     0,   273,   277,     0,   229,
     262,     0,     0,   216,   163,   164,   165,    65,    63,   270,
       0,     0,   281,   258,   193,   283,   412,   409,   295,   234,
     234,   236,   284,   263,   286,   279,   233,   282,   241,     0,
     237,   238,   240,   326,   296,   230,   239,   285,     0,   326,
     297
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    19,    50,    75,    20,    21,    22,    23,    42,    43,
      46,    47,    44,    45,    24,    37,    38,   396,   397,   398,
      81,    82,   291,   408,   538,   597,   598,   599,   484,   564,
      25,    39,   200,   201,    57,    58,    59,   417,   489,   207,
     309,   104,   421,    60,   310,   488,   439,   306,   440,   204,
     205,    26,    83,    84,    85,    27,   302,   303,   304,   305,
     409,    94,   297,    95,   192,   298,   299,   300,   195,    96,
      97,    28,    98,    29,   188,   189,   190,    90,    91,    30,
     531,   584,   593,   535,   532,   536,    31,    32,    33,    34,
      35,    76,    77,    78,   529,   380,   475,   559,    79,   290,
     476,   186,   292,   480,   107,   108,   146,   243,   610,   624,
     323,   635,   147,   639,   640,   641,   148,   149,   150,   151,
     152,   578,   153,   229,   437,   548,   625,   154,   492,   568,
     600,   155,   228,   573,   574,   575,   576,   621,   606,   630,
     633,   643,   431,   432,   156,   157,   320,   493,   569,   620,
     634,   648,   226,   545,   227,   158,   159,   160,   472,   161,
     285,   315,   221,   270,   271,   441,   442,   329,   234,   235,
     443,   162,   163,   375,   374,   373,   372,   371,   370,   369,
     368,   367,   366,   164,   457,   581,   552,   165,   241,   239,
     238,   330,   331,   445,   446,   447,   166,   242,   338,   339,
     340,   452,   167,   345,   344
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -591
static const yytype_int16 yypact[] =
{
    1531,   -86,  -591,   -30,   -28,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,   -58,    57,
    -591,  1531,  -591,  -591,  -591,  -591,   -60,  -591,  -591,   -53,
    -591,  -591,    26,  -591,   -17,   223,  -591,  -591,    14,    -9,
      -8,  -591,    10,    12,    16,    18,    20,    24,  -591,  -591,
     668,  -591,   213,  -591,   -24,    27,    28,   -27,  -591,    25,
    -591,  -591,  -591,   -86,  -591,   120,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,   882,    -3,   424,    32,
      29,  -591,    43,    42,   247,  -591,  -591,  -591,  -591,   357,
      44,   273,  -591,   104,    87,    55,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,   789,    54,  -591,   424,    58,   292,  1438,
    1438,  1438,   865,   865,    70,    65,    68,    71,    72,  -591,
    -591,  -591,  -591,  1270,    63,    64,  -591,    73,  -591,    77,
      78,    79,    80,  1438,  -591,  -591,  -591,    81,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  1438,  -591,  -591,  -591,   882,
    -591,  -591,    88,  -591,  -591,  -591,  -591,  -591,   762,   154,
      66,   154,  -591,  -591,  -591,   154,   154,  -591,    89,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,    91,  -591,  -591,   326,
    -591,   -17,    98,  -591,  -591,    92,    93,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,    96,   214,  -591,  -591,   111,  -591,
     424,  -591,  -591,  -591,  -591,  -591,  -591,   154,   -19,   -19,
    -591,   -20,  1438,  1438,  1438,  -591,   108,   974,   109,   101,
    1640,  -591,  -591,   112,  -591,   110,   113,   116,   119,   125,
     785,   126,  1326,    -3,   413,  -591,  -591,  -591,  -591,  1438,
    1438,  1438,  1438,  1438,  1438,  1438,  1438,  1438,    70,   424,
    1438,  1438,  1438,  1438,  1438,  1438,  1438,  -591,   134,   154,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  1214,   154,   154,   154,   154,  -591,
     229,  1112,   229,  -591,  -591,  -591,  -591,   140,  -591,   142,
     252,    93,   145,   148,  -591,   870,   229,  -591,   789,   764,
     790,  -591,   156,   146,   113,   235,  -591,   460,   481,   526,
    1158,  1438,  -591,   228,  -591,  1438,  -591,  -591,   948,  -591,
     157,  -591,  -591,   113,   113,  -591,   113,  1619,   163,   281,
    -591,   882,   154,  -591,  1438,  1438,  1694,  1710,  1725,   500,
     500,   500,   500,   500,   500,   235,  -591,   138,   138,    75,
      75,  -591,  -591,  -591,   161,  -591,  1438,  1438,  1438,  1438,
    1438,  1438,  1438,  1438,  1438,  1438,   168,  -591,   657,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,   169,  -591,   131,  -591,
    -591,  -591,  -591,   173,  -591,  -591,   181,  -591,  -591,  -591,
     357,   286,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
      -3,  -591,  -591,  -591,  -591,   183,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  1640,   547,   185,   570,   258,  -591,  -591,
    -591,  -591,    34,   101,  -591,   309,  -591,  1438,   188,   190,
     192,  1438,  -591,  -591,  1438,   196,   154,  1438,  1659,  1677,
     113,  1640,  1640,  1640,  1640,  1640,  1640,  1640,  1640,  1640,
    1640,  -591,  1438,  1382,  -591,  1471,   210,  -591,  1112,  -591,
     215,  -591,  -591,   221,  -591,  -591,   205,  -591,   225,   229,
     882,  -591,   974,   218,  -591,  1438,   220,  -591,   948,  -591,
    -591,  -591,  1640,   260,   260,   260,  1640,  -591,  -591,   636,
     219,   680,  -591,   722,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
     334,  -591,   346,  -591,   341,  -591,   352,  -591,   246,  -591,
    -591,  -591,   236,   317,  -591,   974,   591,   308,   238,  -591,
    -591,   282,  -591,  -591,  -591,  -591,   260,  -591,  -591,   229,
    -591,   210,     3,   215,   -57,  -591,  -591,  -591,  -591,  1270,
    -591,   250,  1438,   251,   320,   308,  -591,   266,   257,   255,
    -591,  1438,  -591,   359,  1438,  -591,   267,   268,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,   263,  -591,   -57,
     974,  -591,  -591,  1640,  -591,   262,  -591,  -591,   264,  -591,
    -591,   269,   612,  -591,  1640,  -591,  -591,  -591,  -591,  -591,
     270,   274,  -591,  -591,    -3,   348,  -591,  -591,  -591,   882,
     882,  1066,  -591,  -591,  1270,  -591,  -591,  -591,  -591,   276,
    1066,  -591,  -591,   974,  -591,  -591,  -591,  -591,   271,   974,
    -591
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -591,  -591,  -591,  -591,  -591,  -591,   371,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,   337,  -591,  -591,  -379,  -591,
      97,  -591,  -591,  -591,  -591,  -591,  -198,  -591,  -591,  -591,
    -591,  -591,   -71,    99,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,   -81,  -591,  -591,   -90,  -591,   -88,  -591,
    -591,  -591,  -591,  -591,   325,    -4,   114,  -591,   107,  -591,
    -591,  -591,  -591,  -591,  -591,     9,  -591,  -591,  -591,  -591,
    -591,    46,  -591,  -591,   224,  -591,  -591,  -591,  -591,    76,
    -149,  -591,  -591,  -146,  -591,  -591,  -591,  -591,   383,   -26,
    -591,  -237,  -591,  -591,  -591,  -281,  -591,  -591,   -67,  -591,
    -591,   -94,  -591,  -591,  -591,  -591,  -224,  -591,  -591,  -591,
    -476,  -591,  -332,  -591,  -591,  -219,   275,  -591,  -590,  -223,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -153,  -591,  -591,  -591,
    -591,  -591,  -546,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -109,   -46,    13,  -591,  -591,
    -112,  -591,   165,  -193,  -242,   -70,  -591,  -591,   -92,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -473,  -591,  -591,  -591,
    -591,  -274,  -591,  -591,   -40,  -591,  -591,  -591,  -591,  -591,
       8,  -591,  -591,  -591,  -591
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -430
static const yytype_int16 yytable[] =
{
     211,   212,   213,   322,   324,   326,   341,   178,   185,   455,
     168,   399,   208,   202,   230,   203,   543,    63,   184,   479,
     313,   268,    36,   601,   240,   411,    89,   365,   316,    55,
    -177,   553,   554,   586,   587,   184,   244,   498,    40,   106,
     425,   642,    92,   365,   365,   365,   365,   269,    86,   286,
     642,    48,   595,   287,   288,    73,   596,    49,    89,   448,
     449,    52,   450,   101,   102,   191,   217,   217,    53,   570,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
      86,    41,   210,   582,    56,    93,    54,   422,   644,  -197,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,    87,   533,
     284,    66,   282,   283,   314,   269,   268,   264,   265,   266,
     295,   588,    65,   317,   318,   319,   312,   589,   590,   591,
     592,    67,   426,    68,   619,   218,   219,    69,    88,    70,
      87,    71,    64,   337,   478,    72,    99,   100,   180,   184,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   103,
     179,   357,   358,   359,   360,   361,   362,   363,   542,   499,
      88,   181,   316,   191,   182,   193,   187,   647,   262,   263,
     264,   265,   266,   650,   194,   378,   196,   206,    61,   220,
     209,   231,   232,   490,     5,   284,   510,     6,   356,   222,
     381,   382,   223,    11,   268,   224,   225,   233,    14,    15,
      16,   236,   237,  -417,  -415,  -413,   246,   289,   541,   293,
     296,   433,   434,   301,   365,  -126,   436,   308,   412,   500,
     413,   307,   144,   383,   384,   385,   386,   387,   388,   311,
     456,   389,   321,   325,   328,   458,   459,   327,  -420,   390,
     391,   332,  -223,   333,   364,   392,   393,   394,   395,   334,
     336,   379,   400,   430,   401,   402,   424,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   404,   322,   324,
    -127,     3,     4,  -179,   423,   313,   435,     5,   583,   410,
       6,    61,   444,   453,   454,   460,    11,     5,   471,   477,
       6,    14,    15,    16,    17,   482,    11,   636,   637,    18,
     483,    14,    15,    16,   487,     3,     4,  -179,   491,   495,
     497,     5,   501,   503,     6,   504,   486,   505,   508,   530,
      11,   322,   324,   539,   534,    14,    15,    16,    17,   485,
     537,    61,    80,    18,   540,  -115,   544,     5,   502,   184,
       6,   547,   506,   551,   556,   337,    11,   560,   509,   561,
      61,    14,    15,    16,   562,   563,     5,   565,   566,     6,
     567,   572,   577,   511,   513,    11,    80,   579,   602,  -116,
      14,    15,    16,   604,   605,   608,   322,   324,   609,   611,
     613,   615,   616,   617,    61,   622,   546,   631,   628,   623,
       5,   632,    51,     6,   626,  -156,   649,   629,   645,    11,
     105,   618,   407,    55,    14,    15,    16,   638,   416,   183,
     405,   481,   585,   294,  -224,   403,   638,   594,    62,   322,
     324,   646,   607,   355,   245,   322,   324,   247,   549,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,  -157,   169,
     170,   171,   172,   173,   174,   175,   176,   177,    56,   580,
     433,   550,   507,   603,     0,     0,     0,     0,     0,     0,
       0,     0,   612,     0,   247,   614,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   247,     0,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   433,   258,   259,   260,   261,
     262,   263,   264,   265,   266,     0,     0,     0,   342,   343,
     247,     0,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   247,     0,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,     0,   247,   427,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   247,   428,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   247,     0,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,     0,     0,     0,
     247,   429,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   247,   494,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,     0,   247,   496,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,     0,   571,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,     0,   473,
       0,     0,     0,     0,     0,     0,   247,   627,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,     0,     0,   555,
    -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,     0,
       0,     0,     0,     0,    74,     0,   247,   474,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,     0,     0,   247,
     557,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     109,   110,     0,   197,   198,   111,   112,   113,     0,     0,
       0,   114,   115,     0,   116,   117,     0,   118,   119,   120,
     121,   122,   558,     0,     0,     0,     0,   123,   197,   198,
       0,     0,   124,   125,   199,     0,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,   126,   127,     0,     0,
       0,   414,   128,     0,   129,   130,   415,     0,   131,   132,
     267,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,   133,   134,   135,     0,   418,     0,   136,   137,
       0,   419,     0,   335,   138,   139,   140,   141,   142,   143,
       0,   420,   109,   110,   145,     0,     0,   111,   112,   113,
       0,     0,  -326,   114,   115,     0,   116,   117,    61,   118,
     119,   120,   121,   122,     5,     0,     0,     6,     0,   123,
       0,     0,     0,    11,   124,   125,     0,     0,    14,    15,
      16,     0,     0,   214,   215,     0,     0,     0,   126,   127,
       0,     0,     0,     0,   128,     0,   129,   130,   134,   135,
     131,   132,     0,   136,   216,   406,     0,     0,     0,   138,
     139,   140,   141,     0,   133,   134,   135,     0,     0,    80,
     136,   137,     0,     0,     0,     0,   138,   139,   140,   141,
     142,   143,     0,   144,   109,   110,   145,   197,   198,   111,
     112,   113,     0,   199,     0,   114,   115,     0,   116,   117,
       0,   118,   119,   120,   121,   122,     0,     0,     0,     0,
       0,   123,     0,     0,     0,     0,   124,   125,     0,     0,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     126,   127,     0,     0,     0,     0,   128,   438,   129,   130,
       0,     0,   131,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,   141,   142,   143,     0,   144,   109,   110,   145,     0,
       0,   111,   112,   113,     0,     0,  -326,   114,   115,     0,
     116,   117,     0,   118,   119,   120,   121,   122,     0,     0,
       0,     0,     0,   123,     0,     0,     0,     0,   124,   125,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   126,   127,     0,     0,     0,     0,   128,     0,
     129,   130,     0,     0,   131,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   133,   134,
     135,   381,   382,     0,   136,   137,     0,     0,     0,     0,
     138,   139,   140,   141,     0,   143,     0,   144,   109,   110,
     145,     0,     0,   111,   112,   113,     0,     0,  -326,   114,
       0,     0,   116,   117,   383,   384,   385,   386,   387,   388,
       0,     0,   389,     0,     0,     0,    73,     0,     0,     0,
     390,   391,     0,     0,     0,     0,   392,   393,   394,   395,
       0,     0,     0,     0,     0,   127,     0,     0,     0,     0,
     128,     0,   129,   130,   109,   110,   131,   132,     0,   111,
     112,   113,     0,     0,     0,   114,     0,     0,   116,   117,
       0,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,   141,  -286,   143,     0,     0,
       0,     0,   145,     0,     0,     0,   376,     0,     0,     0,
       0,   127,     0,     0,     0,     0,   128,     0,   129,   130,
     109,   110,   131,   132,     0,   111,   112,   113,     0,     0,
    -326,   114,     0,     0,   116,   117,     0,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,   141,     0,   143,   377,     0,     0,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,   127,     0,     0,
       0,     0,   128,     0,   129,   130,   109,   110,   131,   132,
       0,   111,   112,   113,     0,     0,     0,   114,     0,     0,
     116,   117,     0,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,   141,     0,   143,
       0,     0,     0,     0,   145,     0,     0,     0,     0,     0,
       0,     0,     0,   127,     0,     0,     0,     0,   128,     0,
     129,   130,   109,   110,   131,   132,     0,   111,   112,   113,
       0,     0,     0,   114,     0,     0,   116,   117,     0,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   139,   140,   141,     0,   143,  -429,     0,     0,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,   127,
       0,     0,     0,     0,   128,     0,   129,   130,   109,   110,
     131,   132,     0,   111,   112,   113,     0,     0,     0,   114,
       0,     0,   116,   117,     0,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,   141,
       0,   143,   512,     0,     0,     0,   145,     0,     0,     0,
       0,     0,     0,     0,     0,   127,     0,     0,     0,     0,
     128,     0,   129,   130,     0,     0,   131,   132,     0,     0,
     514,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,   141,     0,   143,     0,     0,
       0,     0,   145,   516,   517,   518,   519,   520,   521,   522,
     523,   524,     0,     0,     0,     0,     0,  -188,     0,   525,
       0,     0,     0,   526,     0,   527,     1,     2,   528,     3,
       4,  -179,     0,     0,     0,     5,     0,     0,     6,     7,
       8,     9,    10,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,     0,     0,     0,     0,    18,     0,     0,
       0,  -188,  -188,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -188,   247,   451,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   247,     0,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266
};

static const yytype_int16 yycheck[] =
{
     109,   110,   111,   227,   227,   229,   243,    78,    89,   341,
      77,   292,   106,   103,   123,   103,   492,     3,    89,   398,
      40,    40,   108,   569,   133,   306,    52,   269,   221,    46,
      60,   504,   505,    30,    31,   106,   145,     3,    66,    65,
     314,   631,    66,   285,   286,   287,   288,   159,    52,   161,
     640,   109,   109,   165,   166,    58,   113,     0,    84,   333,
     334,   121,   336,    90,    91,    91,   112,   113,   121,   545,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      84,   109,   108,   556,   101,   109,    60,   310,   634,    92,
      93,    94,    95,    96,    97,    98,    99,   100,    52,   478,
     119,   109,    36,    37,   124,   217,    40,    32,    33,    34,
     191,   108,   121,   222,   223,   224,   210,   114,   115,   116,
     117,   111,   315,   111,   600,   112,   113,   111,    52,   111,
      84,   111,   118,   242,     3,   111,   109,   109,   109,   210,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   124,
     118,   260,   261,   262,   263,   264,   265,   266,   490,   125,
      84,   118,   355,   189,   122,    61,   122,   643,    30,    31,
      32,    33,    34,   649,    87,   284,   121,   123,    58,   109,
     122,   118,   118,   420,    64,   119,   460,    67,   259,   124,
      59,    60,   124,    73,    40,   124,   124,   124,    78,    79,
      80,   124,   124,   124,   124,   124,   118,   118,   489,   118,
     112,   320,   321,   121,   456,   122,   325,     3,   308,   443,
     308,   125,   121,    92,    93,    94,    95,    96,    97,   118,
     342,   100,   124,   124,   124,   344,   345,   125,   125,   108,
     109,   125,   122,   124,   110,   114,   115,   116,   117,   124,
     124,    22,   112,   320,   112,     3,   110,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   122,   492,   492,
     122,    58,    59,    60,   118,    40,    48,    64,   559,   305,
      67,    58,   125,   120,     3,   124,    73,    64,   120,   120,
      67,    78,    79,    80,    81,   122,    73,   629,   630,    86,
     119,    78,    79,    80,    18,    58,    59,    60,   125,   124,
      52,    64,     3,   125,    67,   125,   410,   125,   122,   109,
      73,   545,   545,   118,   109,    78,    79,    80,    81,   410,
     109,    58,   119,    86,   109,   122,   118,    64,   447,   410,
      67,   121,   451,    83,   125,   454,    73,    13,   457,     3,
      58,    78,    79,    80,    13,     3,    64,   111,   122,    67,
      43,    53,   124,   472,   473,    73,   119,    85,   118,   122,
      78,    79,    80,   122,    54,   109,   600,   600,   121,   124,
      21,   114,   114,   120,    58,   123,   495,   624,   118,   125,
      64,    43,    21,    67,   125,   122,   125,   123,   122,    73,
      63,   599,   305,    46,    78,    79,    80,   631,   309,    84,
     303,   402,   561,   189,   122,   301,   640,   563,    35,   643,
     643,   640,   575,   258,   149,   649,   649,    14,   498,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,   122,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   551,
     569,   501,   454,   572,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   581,    -1,    14,   584,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   634,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,   125,   126,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    14,   125,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    14,   125,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      14,   125,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    14,   125,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    14,   125,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    14,   125,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,   123,
      92,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    14,   120,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    14,
     120,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      30,    31,    -1,    59,    60,    35,    36,    37,    -1,    -1,
      -1,    41,    42,    -1,    44,    45,    -1,    47,    48,    49,
      50,    51,   120,    -1,    -1,    -1,    -1,    57,    59,    60,
      -1,    -1,    62,    63,    65,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,    76,    77,    -1,    -1,
      -1,   107,    82,    -1,    84,    85,   112,    -1,    88,    89,
     118,    92,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,    -1,   102,   103,   104,    -1,   106,    -1,   108,   109,
      -1,   111,    -1,   118,   114,   115,   116,   117,   118,   119,
      -1,   121,    30,    31,   124,    -1,    -1,    35,    36,    37,
      -1,    -1,    40,    41,    42,    -1,    44,    45,    58,    47,
      48,    49,    50,    51,    64,    -1,    -1,    67,    -1,    57,
      -1,    -1,    -1,    73,    62,    63,    -1,    -1,    78,    79,
      80,    -1,    -1,    88,    89,    -1,    -1,    -1,    76,    77,
      -1,    -1,    -1,    -1,    82,    -1,    84,    85,   103,   104,
      88,    89,    -1,   108,   109,   105,    -1,    -1,    -1,   114,
     115,   116,   117,    -1,   102,   103,   104,    -1,    -1,   119,
     108,   109,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
     118,   119,    -1,   121,    30,    31,   124,    59,    60,    35,
      36,    37,    -1,    65,    -1,    41,    42,    -1,    44,    45,
      -1,    47,    48,    49,    50,    51,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    62,    63,    -1,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
      76,    77,    -1,    -1,    -1,    -1,    82,   109,    84,    85,
      -1,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,   119,    -1,   121,    30,    31,   124,    -1,
      -1,    35,    36,    37,    -1,    -1,    40,    41,    42,    -1,
      44,    45,    -1,    47,    48,    49,    50,    51,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    82,    -1,
      84,    85,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,    59,    60,    -1,   108,   109,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,    -1,   119,    -1,   121,    30,    31,
     124,    -1,    -1,    35,    36,    37,    -1,    -1,    40,    41,
      -1,    -1,    44,    45,    92,    93,    94,    95,    96,    97,
      -1,    -1,   100,    -1,    -1,    -1,    58,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      82,    -1,    84,    85,    30,    31,    88,    89,    -1,    35,
      36,    37,    -1,    -1,    -1,    41,    -1,    -1,    44,    45,
      -1,   103,   104,    -1,    -1,    -1,   108,   109,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   119,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    82,    -1,    84,    85,
      30,    31,    88,    89,    -1,    35,    36,    37,    -1,    -1,
      40,    41,    -1,    -1,    44,    45,    -1,   103,   104,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,    -1,   119,   120,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    82,    -1,    84,    85,    30,    31,    88,    89,
      -1,    35,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,
      44,    45,    -1,   103,   104,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,   114,   115,   116,   117,    -1,   119,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    82,    -1,
      84,    85,    30,    31,    88,    89,    -1,    35,    36,    37,
      -1,    -1,    -1,    41,    -1,    -1,    44,    45,    -1,   103,
     104,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,    -1,   119,   120,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    82,    -1,    84,    85,    30,    31,
      88,    89,    -1,    35,    36,    37,    -1,    -1,    -1,    41,
      -1,    -1,    44,    45,    -1,   103,   104,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,   114,   115,   116,   117,
      -1,   119,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      82,    -1,    84,    85,    -1,    -1,    88,    89,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,   108,   109,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,    -1,   119,    -1,    -1,
      -1,    -1,   124,    92,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,    -1,    -1,    -1,    -1,    46,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    55,    56,   117,    58,
      59,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    55,    56,    58,    59,    64,    67,    68,    69,    70,
      71,    73,    74,    75,    78,    79,    80,    81,    86,   128,
     131,   132,   133,   134,   141,   157,   178,   182,   198,   200,
     206,   213,   214,   215,   216,   217,   108,   142,   143,   158,
      66,   109,   135,   136,   139,   140,   137,   138,   109,     0,
     129,   133,   121,   121,    60,    46,   101,   161,   162,   163,
     170,    58,   215,     3,   118,   121,   109,   111,   111,   111,
     111,   111,   111,    58,   106,   130,   218,   219,   220,   225,
     119,   147,   148,   179,   180,   181,   182,   198,   206,   216,
     204,   205,    66,   109,   188,   190,   196,   197,   199,   109,
     109,    90,    91,   124,   168,   142,   216,   231,   232,    30,
      31,    35,    36,    37,    41,    42,    44,    45,    47,    48,
      49,    50,    51,    57,    62,    63,    76,    77,    82,    84,
      85,    88,    89,   102,   103,   104,   108,   109,   114,   115,
     116,   117,   118,   119,   121,   124,   233,   239,   243,   244,
     245,   246,   247,   249,   254,   258,   271,   272,   282,   283,
     284,   286,   298,   299,   310,   314,   323,   329,   225,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   159,   118,
     109,   118,   122,   181,   159,   170,   228,   122,   201,   202,
     203,   216,   191,    61,    87,   195,   121,    59,    60,    65,
     159,   160,   173,   175,   176,   177,   123,   166,   228,   122,
     216,   282,   282,   282,    88,    89,   109,   283,   284,   284,
     109,   289,   124,   124,   124,   124,   279,   281,   259,   250,
     282,   118,   118,   124,   295,   296,   124,   124,   317,   316,
     282,   315,   324,   234,   282,   243,   118,    14,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,   118,    40,   287,
     290,   291,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    36,    37,   119,   287,   287,   287,   287,   118,
     226,   149,   229,   118,   201,   170,   112,   189,   192,   193,
     194,   121,   183,   184,   185,   186,   174,   125,     3,   167,
     171,   118,   228,    40,   124,   288,   290,   282,   282,   282,
     273,   124,   233,   237,   246,   124,   233,   125,   124,   294,
     318,   319,   125,   124,   124,   118,   124,   282,   325,   326,
     327,   218,   125,   126,   331,   330,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   289,   159,   282,   282,   282,
     282,   282,   282,   282,   110,   291,   309,   308,   307,   306,
     305,   304,   303,   302,   301,   300,    72,   120,   282,    22,
     222,    59,    60,    92,    93,    94,    95,    96,    97,   100,
     108,   109,   114,   115,   116,   117,   144,   145,   146,   222,
     112,   112,     3,   183,   122,   185,   105,   147,   150,   187,
     216,   222,   173,   175,   107,   112,   160,   164,   106,   111,
     121,   169,   246,   118,   110,   318,   290,   125,   125,   125,
     225,   269,   270,   282,   282,    48,   282,   251,   109,   173,
     175,   292,   293,   297,   125,   320,   321,   322,   318,   318,
     318,    15,   328,   120,     3,   239,   287,   311,   282,   282,
     124,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   120,   285,    72,   120,   223,   227,   120,     3,   145,
     230,   192,   122,   119,   155,   170,   228,    18,   172,   165,
     218,   125,   255,   274,   125,   124,   125,    52,     3,   125,
     233,     3,   282,   125,   125,   125,   282,   327,   122,   282,
     318,   282,   120,   282,    59,    60,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   108,   112,   114,   117,   221,
     109,   207,   211,   145,   109,   210,   212,   109,   151,   118,
     109,   222,   239,   237,   118,   280,   282,   121,   252,   292,
     321,    83,   313,   313,   313,   123,   125,   120,   120,   224,
      13,     3,    13,     3,   156,   111,   122,    43,   256,   275,
     237,   125,    53,   260,   261,   262,   263,   124,   248,    85,
     295,   312,   313,   222,   208,   207,    30,    31,   108,   114,
     115,   116,   117,   209,   210,   109,   113,   152,   153,   154,
     257,   269,   118,   282,   122,    54,   265,   263,   109,   121,
     235,   124,   282,    21,   282,   114,   114,   120,   153,   237,
     276,   264,   123,   125,   236,   253,   125,   125,   118,   123,
     266,   218,    43,   267,   277,   238,   239,   239,   233,   240,
     241,   242,   245,   268,   269,   122,   242,   237,   278,   125,
     237
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
#line 183 "ferite_parser.y"
    {
                                        ferite_do_function_header( ferite_compiler_entry_function(ferite_scanner_file), FE_FALSE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE, FE_FALSE );
                                        if( ferite_do_function_start() ) {
                                            yyclearin;
                                        }
                                        ferite_var_array_count = 0;
                                    ;}
    break;

  case 3:
#line 190 "ferite_parser.y"
    {    ferite_do_script_finish(); ;}
    break;

  case 4:
#line 194 "ferite_parser.y"
    {    ferite_do_function_footer(); ;}
    break;

  case 5:
#line 196 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_void_variable( NULL, "forced-function-return", FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        ferite_do_function_footer();
                                    ;}
    break;

  case 17:
#line 225 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 18:
#line 226 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-init", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 19:
#line 227 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 20:
#line 228 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-deinit", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 21:
#line 229 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 22:
#line 230 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-register", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 23:
#line 231 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 24:
#line 232 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-unregister", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 25:
#line 233 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 26:
#line 234 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-header", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 27:
#line 235 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 28:
#line 236 "ferite_parser.y"
    { ferite_do_create_builder_variable( "'module-flags", (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) ); ;}
    break;

  case 30:
#line 244 "ferite_parser.y"
    { ferite_do_uses( (yyvsp[(1) - (1)].text) ); ffree_ngc((yyvsp[(1) - (1)].text)); FECT; ;}
    break;

  case 35:
#line 258 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                          "", (yyvsp[(1) - (1)].text), 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item );
                              ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT; ;}
    break;

  case 36:
#line 262 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                      "", (yyvsp[(1) - (1)].text), 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item );
                          ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT; ;}
    break;

  case 37:
#line 266 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_number_double_variable( CURRENT_SCRIPT, "", (yyvsp[(1) - (1)].fnumber), FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 38:
#line 268 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_number_double_variable( CURRENT_SCRIPT, "", (yyvsp[(1) - (1)].inumber), FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 39:
#line 270 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                         "", "number", 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 40:
#line 273 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                         "", "boolean", 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 41:
#line 276 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                         "", "string", 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 42:
#line 279 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                         "", "void", 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 43:
#line 282 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                         "", "void", 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 44:
#line 285 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                         "", "object", 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 45:
#line 288 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                         "", "array", 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 46:
#line 291 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                         "", "class", 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 47:
#line 294 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_string_variable_from_ptr( CURRENT_SCRIPT,
                                                                                     "", "namespace", 0, FE_CHARSET_DEFAULT, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 48:
#line 297 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_boolean_variable( CURRENT_SCRIPT, "", FE_TRUE, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 49:
#line 299 "ferite_parser.y"
    { FeriteVariable *item = ferite_create_boolean_variable( CURRENT_SCRIPT, "", FE_FALSE, FE_STATIC );
                          ferite_stack_push( FE_NoScript, ferite_directive_parameters, item ); ;}
    break;

  case 54:
#line 314 "ferite_parser.y"
    { 
                                ferite_directive_parameters = ferite_create_stack( CURRENT_SCRIPT, 10 ); 
                                ferite_scanner_start_directive_list();
                            ;}
    break;

  case 55:
#line 318 "ferite_parser.y"
    {
                                ferite_scanner_stop_directive_list();
                                if( ferite_expansion_hash && ferite_hash_get( FE_NoScript, ferite_expansion_hash, (yyvsp[(2) - (5)].text) ) ) {
                                    char *body = NULL;
                                    FeriteIterator *iter = ferite_create_iterator( FE_NoScript );
                                    FeriteHashBucket *bucket = NULL;

                                    ferite_expansion_map_hash = ferite_hash_get( FE_NoScript, ferite_expansion_hash, (yyvsp[(2) - (5)].text) );
                                    FUD(("Got expansion call: %s\n", (yyvsp[(2) - (5)].text)));
                                    body = ferite_hash_get( FE_NoScript, ferite_expansion_map_hash, "!_BODY_");
                                    FUD(( "With body: %s\n", body ));

                                    while( (bucket = ferite_hash_walk(FE_NoScript, ferite_expansion_map_hash, iter)) != NULL ) {
                                        long offset = (long)(bucket->data);
                                        if( strcmp( bucket->id, "!_BODY_" ) ) { /* No body */
                                            FeriteString *var = ferite_variable_to_str( FE_NoScript, ferite_directive_parameters->stack[offset+1], FE_FALSE );
                                            FUD(( "bucket: %s -> %ld\n", bucket->id, offset ));
                                            FUD(( "replacement: %s\n", var->data ));
                                            body = ferite_replace_string( body, bucket->id, var->data );
                                        }
                                    }
                                    FUD(( "new body: %s\n", body ));

                                    /* Now we want to parse this code in the current context */
                                    ferite_scanner_parse_macro(body);
                                } else {
                                    ferite_do_add_directive( (yyvsp[(2) - (5)].text), ferite_directive_parameters );
                                    ferite_directive_parameters = NULL;
                                    ffree_ngc((yyvsp[(2) - (5)].text)); FECT;
                                }
                             ;}
    break;

  case 56:
#line 352 "ferite_parser.y"
    { ferite_scanner_go_native(); ;}
    break;

  case 57:
#line 353 "ferite_parser.y"
    { FUD(("Code block: '%s'\n", (yyvsp[(4) - (4)].text)));
        ferite_hash_add( FE_NoScript, ferite_expansion_map_hash, "!_BODY_", (yyvsp[(4) - (4)].text) );
    ;}
    break;

  case 60:
#line 364 "ferite_parser.y"
    { FUD(("Label: %s [%ld]\n", (yyvsp[(1) - (1)].text), ferite_expansion_map_offset )); ferite_expansion_map_offset++; ;}
    break;

  case 61:
#line 365 "ferite_parser.y"
    {
        FUD(("DirectiveLabel: %s [%ld]\n", (yyvsp[(1) - (1)].text), ferite_expansion_map_offset));
        ferite_hash_add( FE_NoScript, ferite_expansion_map_hash, (yyvsp[(1) - (1)].text), (void*)ferite_expansion_map_offset );
        ferite_expansion_map_offset++;
    ;}
    break;

  case 64:
#line 379 "ferite_parser.y"
    {
            FUD(("expansion name: %s\n", (yyvsp[(2) - (2)].text)));
            if( ferite_expansion_hash == NULL ) {
                ferite_expansion_hash = ferite_create_hash( FE_NoScript, 10 );
            }
            ferite_expansion_map_offset = 0;
            ferite_expansion_map_hash = ferite_create_hash( FE_NoScript, 10 );
            ferite_hash_add( FE_NoScript, ferite_expansion_hash, (yyvsp[(2) - (2)].text), ferite_expansion_map_hash );
        ;}
    break;

  case 66:
#line 393 "ferite_parser.y"
    { FUD(( "going into global block\n" )); ferite_var_is_global = FE_TRUE; ;}
    break;

  case 67:
#line 395 "ferite_parser.y"
    { ferite_var_is_global = FE_FALSE; FUD(( "leaving global memory block\n" )); ;}
    break;

  case 68:
#line 399 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "L" ); ferite_last_type = F_VAR_NUM;    ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 69:
#line 400 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "L" ); ferite_last_type = F_VAR_LONG;   ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 70:
#line 401 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "D" ); ferite_last_type = F_VAR_DOUBLE; ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 71:
#line 402 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "S" ); ferite_last_type = F_VAR_STR;    ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 72:
#line 403 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "V" ); ferite_last_type = F_VAR_VOID;   ferite_var_type_is_deprecated = FE_TRUE; ;}
    break;

  case 73:
#line 404 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "V" ); ferite_last_type = F_VAR_VOID;   ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 74:
#line 405 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "O" ); ferite_last_type = F_VAR_OBJ;    ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 75:
#line 406 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "A" ); ferite_last_type = F_VAR_UARRAY; ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 76:
#line 407 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "B" ); ferite_last_type = F_VAR_BOOL;   ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 78:
#line 412 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "C" ); ferite_last_type = F_VAR_CLASS; ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 79:
#line 413 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "N" ); ferite_last_type = F_VAR_NS;    ferite_var_type_is_deprecated = FE_FALSE; ;}
    break;

  case 82:
#line 422 "ferite_parser.y"
    { ferite_last_function = (yyvsp[(2) - (2)].text); ;}
    break;

  case 83:
#line 423 "ferite_parser.y"
    { ferite_last_function = (yyvsp[(2) - (2)].text); ferite_function_is_directive = FE_TRUE; ;}
    break;

  case 84:
#line 425 "ferite_parser.y"
    { ferite_last_function = fstrdup( "constructor" ); ;}
    break;

  case 85:
#line 427 "ferite_parser.y"
    { ferite_last_function = fstrdup( "destructor" ); ;}
    break;

  case 86:
#line 431 "ferite_parser.y"
    {
                                        if( ferite_var_is_final ) {
                                            ferite_warning( CURRENT_SCRIPT,
                                                "Keyword 'final' can not be applied to functions ('%s' on line %d in %s)\n",
                                                ferite_last_function,
                                                ferite_scanner_lineno,
                                                ferite_scanner_file );
                                        }
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

  case 87:
#line 460 "ferite_parser.y"
    {
                                        ferite_return_type_was_provided = FE_TRUE;
                                        ferite_scanner_stop_typing_label();
                                        ferite_seeking_return_type_fix = FE_TRUE;
                                        ferite_hint_depth = 0;
                                    ;}
    break;

  case 88:
#line 466 "ferite_parser.y"
    { ferite_seeking_return_type_fix = FE_FALSE; ;}
    break;

  case 89:
#line 467 "ferite_parser.y"
    {
                                        ferite_scanner_stop_typing_label();
                                        ferite_return_type_was_provided = FE_TRUE;
                                        strcpy(ferite_hint_typestring, "O:");
                                        strcat(ferite_hint_typestring, (yyvsp[(1) - (1)].text));
                                        strcat(ferite_hint_typestring, ";");
                                        ffree_ngc((yyvsp[(1) - (1)].text)); FECT;
                                    ;}
    break;

  case 90:
#line 475 "ferite_parser.y"
    { 
                                        strcpy(ferite_hint_typestring, "U");
                                        ferite_scanner_stop_typing_label();
                                        ferite_return_type_was_provided = FE_TRUE;
                                    ;}
    break;

  case 91:
#line 483 "ferite_parser.y"
    { ferite_scanner_want_typing_label(); ;}
    break;

  case 92:
#line 484 "ferite_parser.y"
    { 
                                        ferite_do_function_return_type(ferite_hint_typestring);
                                    ;}
    break;

  case 93:
#line 487 "ferite_parser.y"
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

  case 94:
#line 517 "ferite_parser.y"
    {
                                        FUD(("PARSER: END FUNCTION PARAMETER DECLARATION\n"));
                                        FUD(("PARSER: BEGIN FUNCTION BODY\n"));
                                    ;}
    break;

  case 96:
#line 528 "ferite_parser.y"
    {
                                        FUD(("PARSER: END FUNCTION BODY\n"));
                                        ferite_temp_variable = ferite_create_void_variable( NULL, "forced-function-return", FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: push(voidptr)        \n"));
                                        ferite_do_function_footer();
                                    ;}
    break;

  case 97:
#line 536 "ferite_parser.y"
    {
                                        ferite_do_function_native_block( (yyvsp[(1) - (1)].text),
                                                ferite_current_native_block_file,
                                                ferite_current_native_block_line );
                                    ;}
    break;

  case 98:
#line 541 "ferite_parser.y"
    {

                                        FUD(("PARSER: END FUNCTION BODY\n"));
                                        ferite_temp_variable = ferite_create_void_variable( NULL, "forced-function-return", FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: push(voidptr)        \n"));
                                        ferite_do_function_footer();
                                    ;}
    break;

  case 99:
#line 550 "ferite_parser.y"
    {    ferite_do_function_footer(); ;}
    break;

  case 100:
#line 557 "ferite_parser.y"
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

  case 103:
#line 572 "ferite_parser.y"
    { ferite_var_pass_type = FE_BY_REF; ;}
    break;

  case 104:
#line 576 "ferite_parser.y"
    {
                                        ferite_hint_depth = 0;

                                        if( ferite_var_type_is_deprecated ) {
                                            ferite_warning( CURRENT_SCRIPT,
                                                            "Keyword 'void' can not be applied to a function's parameter type - use 'auto' for polymorphic typing ('%s' on line %d in %s)\n",
                                                            ferite_last_function,
                                                            ferite_scanner_lineno,
                                                            ferite_scanner_file );
                                        }
                                    ;}
    break;

  case 105:
#line 589 "ferite_parser.y"
    {
                                        ferite_hint_current_type = ferite_subtype_link( CURRENT_SCRIPT, ferite_hint_typestring );
                                        if( ferite_hint_current_type == NULL && CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
                                            feperror(NULL);
                                        }
                                        FUD(("PARSER: Parameter Declared         %s (%d)\n", (yyvsp[(5) - (5)].text), ferite_last_type));
                                        ferite_do_add_variable_to_paramlist( (yyvsp[(5) - (5)].text), ferite_last_type, ferite_hint_current_type, ferite_var_pass_type );
                                        ffree_ngc( (yyvsp[(5) - (5)].text) ); FECT;
                                        ferite_var_pass_type = FE_BY_VALUE;
                                    ;}
    break;

  case 106:
#line 602 "ferite_parser.y"
    {
                                        FUD(("PARSER: Parameter Declared - Multiple Args\n"));
                                        ferite_do_add_variable_to_paramlist( "...", F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_VALUE );
                                    ;}
    break;

  case 113:
#line 621 "ferite_parser.y"
    { ferite_do_namespace_header( (yyvsp[(2) - (2)].text) ); ffree_ngc( (yyvsp[(2) - (2)].text) ); FECT; ;}
    break;

  case 114:
#line 622 "ferite_parser.y"
    { ferite_do_namespace_extends( (yyvsp[(3) - (3)].text) ); ffree_ngc( (yyvsp[(3) - (3)].text) );  FECT; ;}
    break;

  case 125:
#line 646 "ferite_parser.y"
    { ferite_do_namespace_footer(); ;}
    break;

  case 130:
#line 660 "ferite_parser.y"
    { ferite_item_state = -1; ;}
    break;

  case 136:
#line 672 "ferite_parser.y"
    {
                                        FUD(("CLASS START\n" ));
                                        ferite_do_class_header( (yyvsp[(1) - (1)].text), NULL, ferite_class_state );
                                        ferite_class_state = 0;
                                        ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
                                    ;}
    break;

  case 137:
#line 678 "ferite_parser.y"
    {
                                        ferite_scanner_want_dot_label();
                                    ;}
    break;

  case 138:
#line 681 "ferite_parser.y"
    {
                                        FUD(("CLASS START\n" ));
                                        ferite_scanner_stop_dot_label();
                                        ferite_do_class_header( (yyvsp[(1) - (4)].text), (yyvsp[(4) - (4)].text), ferite_class_state );
                                        ferite_class_state = 0;
                                        ffree_ngc( (yyvsp[(1) - (4)].text) ); FECT;
                                        ffree_ngc( (yyvsp[(4) - (4)].text) ); FECT;
                                    ;}
    break;

  case 139:
#line 692 "ferite_parser.y"
    {
                                        ferite_scanner_want_dot_label();
                                    ;}
    break;

  case 140:
#line 695 "ferite_parser.y"
    {
                                        FUD(("CLASS START\n" ));
                                        ferite_scanner_stop_dot_label();
                                        ferite_do_class_header( NULL, (yyvsp[(3) - (3)].text), ferite_class_state );
                                        ferite_class_state = 0;
                                        ffree_ngc( (yyvsp[(3) - (3)].text) ); FECT;
                                    ;}
    break;

  case 141:
#line 705 "ferite_parser.y"
    {
                                        ferite_scanner_want_dot_label();
                                    ;}
    break;

  case 142:
#line 708 "ferite_parser.y"
    {
                                        ferite_scanner_stop_dot_label();
                                        ferite_do_class_implements( (yyvsp[(2) - (2)].text) );
                                        ffree_ngc( (yyvsp[(2) - (2)].text) ); FECT;
                                    ;}
    break;

  case 147:
#line 728 "ferite_parser.y"
    {
                                        FUD(("CLASS END\n" ));
                                        ferite_do_class_footer();
                                    ;}
    break;

  case 148:
#line 736 "ferite_parser.y"
    {
                                        ferite_do_class_footer();
                                    ;}
    break;

  case 152:
#line 753 "ferite_parser.y"
    {
                            ferite_do_class_header( (yyvsp[(2) - (2)].text), NULL, FE_ITEM_IS_PROTOCOL );
                            ferite_class_state = 0;
                            ffree_ngc( (yyvsp[(2) - (2)].text) ); FECT;
                        ;}
    break;

  case 158:
#line 775 "ferite_parser.y"
    { ferite_class_state = -1; ;}
    break;

  case 160:
#line 780 "ferite_parser.y"
    {
                                        ferite_do_class_footer();
                                    ;}
    break;

  case 161:
#line 787 "ferite_parser.y"
    {
                                        FUD(("PARSER: Variable Declared         %s(%d)\n", (yyvsp[(1) - (1)].text), ferite_last_type));
                                        if( ferite_var_is_native )
                                          ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (1)].text), ferite_scanner_lineno, ferite_scanner_file );
                                        ferite_do_add_variable( (yyvsp[(1) - (1)].text), ferite_last_type, ferite_hint_current_type, ferite_var_is_global, ferite_var_is_final, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );

                                        ferite_do_variable_push( (yyvsp[(1) - (1)].text) );
                                        ferite_do_variable_reset( ferite_last_type );

                                        ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
                                    ;}
    break;

  case 162:
#line 798 "ferite_parser.y"
    {
                                        FUD(("PARSER: Variable Declared         %s(%d)\n", (yyvsp[(1) - (2)].text), ferite_last_type));
                                        if( ferite_var_is_native )
                                          ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (2)].text), ferite_scanner_lineno, ferite_scanner_file );
                                        ferite_do_add_variable( (yyvsp[(1) - (2)].text), ferite_last_type, ferite_hint_current_type, FE_FALSE, ferite_var_is_final, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );

                                        ferite_do_variable_push( (yyvsp[(1) - (2)].text) );
                                        ferite_do_variable_reset( ferite_last_type );

                                        ferite_do_variable_push( (yyvsp[(1) - (2)].text) );
                                        ffree_ngc( (yyvsp[(1) - (2)].text) ); FECT;
                                    ;}
    break;

  case 163:
#line 810 "ferite_parser.y"
    {
                                        ferite_do_binary_op( FERITE_OPCODE_assign );
                                        FUD(("PARSER: op->assign       pop,pop\n"));
                                        ferite_do_pop();
                                    ;}
    break;

  case 164:
#line 818 "ferite_parser.y"
    {   ferite_namespace_naturaln = 0 - (yyvsp[(2) - (2)].inumber); ;}
    break;

  case 165:
#line 819 "ferite_parser.y"
    {   ferite_namespace_naturaln = (yyvsp[(2) - (2)].inumber); ;}
    break;

  case 166:
#line 820 "ferite_parser.y"
    {   ferite_namespace_naturaln = (yyvsp[(1) - (1)].inumber); ;}
    break;

  case 167:
#line 824 "ferite_parser.y"
    {
                                        FUD(("PARSER: Variable Declared         %s(%d)\n", (yyvsp[(1) - (1)].text), ferite_last_type));
                                        if( ferite_var_is_native )
                                          ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (1)].text), ferite_scanner_lineno, ferite_scanner_file );
                                        ferite_do_add_variable( (yyvsp[(1) - (1)].text), ferite_last_type, ferite_hint_current_type, ferite_var_is_global, ferite_var_is_final, ferite_var_is_static, ferite_var_is_atomic, ferite_item_state, FE_FALSE );
                                        ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
                                    ;}
    break;

  case 168:
#line 831 "ferite_parser.y"
    {
                                        if( ferite_var_is_native )
                                          ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
                                        if( ferite_last_type == F_VAR_NUM )
                                        {
                                            FUD(("PARSER: Variable Declared         %s(%d)=%ld\n", (yyvsp[(1) - (3)].text), ferite_last_type, ferite_namespace_naturaln ));
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

  case 169:
#line 851 "ferite_parser.y"
    {
                                        if( ferite_var_is_native )
                                          ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
                                        if( ferite_last_type == F_VAR_NUM )
                                        {
                                            FUD(("PARSER: Variable Declared         %s(%d)=%f\n", (yyvsp[(1) - (3)].text), ferite_last_type, (yyvsp[(3) - (3)].fnumber)));
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

  case 170:
#line 871 "ferite_parser.y"
    {
                                        if( ferite_var_is_native )
                                          ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
                                        if( ferite_last_type == F_VAR_STR )
                                        {
                                            FUD(("PARSER: Variable Declared         %s(%d)='%s'\n", (yyvsp[(1) - (3)].text), ferite_last_type, (yyvsp[(3) - (3)].text)));
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

  case 171:
#line 893 "ferite_parser.y"
    {
                                       if( ferite_var_is_native )
                                         ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
                                       if( ferite_last_type == F_VAR_BOOL )
                                       {
                                           FUD(("PARSER: Variable Declared         %s(%d)=%ld\n", (yyvsp[(1) - (3)].text), ferite_last_type, (yyvsp[(3) - (3)].inumber)));
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

  case 172:
#line 913 "ferite_parser.y"
    {
                                       if( ferite_var_is_native )
                                         ferite_warning( CURRENT_SCRIPT, "Keyword 'native' can not be applied to variables ('%s' on line %d in %s)\n", (yyvsp[(1) - (3)].text), ferite_scanner_lineno, ferite_scanner_file );
                                       if( ferite_last_type == F_VAR_BOOL )
                                       {
                                           FUD(("PARSER: Variable Declared         %s(%d)=%ld\n", (yyvsp[(1) - (3)].text), ferite_last_type, (yyvsp[(3) - (3)].inumber)));
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

  case 177:
#line 947 "ferite_parser.y"
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

  case 178:
#line 956 "ferite_parser.y"
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

  case 181:
#line 973 "ferite_parser.y"
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

  case 182:
#line 982 "ferite_parser.y"
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

  case 183:
#line 991 "ferite_parser.y"
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

  case 184:
#line 1000 "ferite_parser.y"
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

  case 185:
#line 1009 "ferite_parser.y"
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

  case 186:
#line 1018 "ferite_parser.y"
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

  case 187:
#line 1027 "ferite_parser.y"
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

  case 196:
#line 1059 "ferite_parser.y"
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

  case 198:
#line 1072 "ferite_parser.y"
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

  case 199:
#line 1082 "ferite_parser.y"
    { ferite_error(CURRENT_SCRIPT, 0, "Quoted string hint type not supported\n" ); ffree_ngc((yyvsp[(1) - (1)].text)); FECT; feperror( NULL ); ;}
    break;

  case 200:
#line 1083 "ferite_parser.y"
    { ferite_error(CURRENT_SCRIPT, 0, "Floating point hint type not supported\n" ); feperror( NULL ); ;}
    break;

  case 201:
#line 1084 "ferite_parser.y"
    { ferite_error(CURRENT_SCRIPT, 0, "Natural nunber hint type not supported\n" ); feperror( NULL ); ;}
    break;

  case 202:
#line 1085 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "L" ); ;}
    break;

  case 203:
#line 1086 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "L" ); ;}
    break;

  case 204:
#line 1087 "ferite_parser.y"
    { strcpy( ferite_hint_typestring, "D" ); ;}
    break;

  case 205:
#line 1088 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "S" ); ;}
    break;

  case 206:
#line 1089 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "V" ); ;}
    break;

  case 207:
#line 1090 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "V" ); ;}
    break;

  case 208:
#line 1091 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "O" ); ;}
    break;

  case 209:
#line 1092 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "A" ); ;}
    break;

  case 210:
#line 1093 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "B" ); ;}
    break;

  case 211:
#line 1094 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "C" ); ;}
    break;

  case 212:
#line 1095 "ferite_parser.y"
    { strcat( ferite_hint_typestring, "N" ); ;}
    break;

  case 213:
#line 1099 "ferite_parser.y"
    {
                                        if( ferite_seeking_return_type_fix && ferite_hint_depth == 0 ) {
                                            yyclearin; 
                                            ferite_lexer_fix_native_blocks(); 
                                        }
                                    ;}
    break;

  case 214:
#line 1105 "ferite_parser.y"
    {
                                        ferite_hint_depth++;
                                        ferite_scanner_want_end_hint(); 
                                        ferite_scanner_want_typing_label();
                                    ;}
    break;

  case 215:
#line 1110 "ferite_parser.y"
    { ferite_scanner_stop_typing_label(); ;}
    break;

  case 216:
#line 1112 "ferite_parser.y"
    { 
                                        ferite_scanner_stop_end_hint();
                                        ferite_hint_depth--;
                                    ;}
    break;

  case 217:
#line 1119 "ferite_parser.y"
    {
                                        FUD(("PARSER: Setting current type to %d\n", ferite_last_type ));
                                        ferite_hint_depth = 0;

                                        if( ferite_var_type_is_deprecated ) {
                                            ferite_warning( CURRENT_SCRIPT,
                                                            "Keyword 'void' can not be applied to a function's parameter type - use 'auto' for polymorphic typing ('%s' on line %d in %s)\n",
                                                            ferite_last_function,
                                                            ferite_scanner_lineno,
                                                            ferite_scanner_file );
                                        }
                                    ;}
    break;

  case 218:
#line 1131 "ferite_parser.y"
    {
                                        ferite_hint_current_type = ferite_subtype_link( CURRENT_SCRIPT, ferite_hint_typestring );
                                        if( ferite_hint_current_type == NULL && CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
                                            feperror(NULL);
                                        }
                                    ;}
    break;

  case 219:
#line 1137 "ferite_parser.y"
    { 
                                        ferite_var_is_final = FE_FALSE; 
                                        ferite_var_is_static = FE_FALSE; 
                                        ferite_var_is_atomic = FE_FALSE; 
                                        ferite_var_is_native = FE_FALSE; 
                                    ;}
    break;

  case 220:
#line 1146 "ferite_parser.y"
    {
                                        FUD(("PARSER: Setting current type to %d\n", ferite_last_type ));
                                        ferite_hint_depth = 0;

                                        if( ferite_var_type_is_deprecated ) {
                                            ferite_warning( CURRENT_SCRIPT,
                                                            "Keyword 'void' can not be applied to a function's parameter type - use 'auto' for polymorphic typing ('%s' on line %d in %s)\n",
                                                            ferite_last_function,
                                                            ferite_scanner_lineno,
                                                            ferite_scanner_file );
                                        }

                                    ;}
    break;

  case 221:
#line 1159 "ferite_parser.y"
    {
                                        ferite_hint_current_type = ferite_subtype_link( CURRENT_SCRIPT, ferite_hint_typestring );
                                        if( ferite_hint_current_type == NULL && CURRENT_SCRIPT->error_state == FE_ERROR_THROWN ) {
                                            feperror(NULL);
                                        }
                                    ;}
    break;

  case 222:
#line 1165 "ferite_parser.y"
    { ferite_var_is_final = FE_FALSE; ferite_var_is_static = FE_FALSE; ferite_var_is_atomic = FE_FALSE; ferite_var_is_native = FE_FALSE; ;}
    break;

  case 227:
#line 1180 "ferite_parser.y"
    { ferite_do_begin_block(); ;}
    break;

  case 228:
#line 1183 "ferite_parser.y"
    { ferite_do_end_block(); ;}
    break;

  case 229:
#line 1186 "ferite_parser.y"
    { ferite_do_begin_block(); ;}
    break;

  case 230:
#line 1189 "ferite_parser.y"
    { ferite_do_end_block(); ;}
    break;

  case 236:
#line 1206 "ferite_parser.y"
    {
                                ferite_error( CURRENT_SCRIPT, 0, "You are not allowed to have empty handle blocks.\n" );
                                  feperror( NULL );
                            ;}
    break;

  case 246:
#line 1232 "ferite_parser.y"
    { ferite_do_pop(); /* clear up the stupid stack ie wham = 2 + 3 leaves a variable on the stack*/ ;}
    break;

  case 250:
#line 1236 "ferite_parser.y"
    { ferite_do_break(); ;}
    break;

  case 251:
#line 1237 "ferite_parser.y"
    { ferite_do_continue(); ;}
    break;

  case 252:
#line 1238 "ferite_parser.y"
    { ferite_do_raise(); ;}
    break;

  case 255:
#line 1247 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_void_variable( NULL, "compiled-in-return-value", FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: push(voidptr)        \n"));
                                        ferite_do_exit();
                                    ;}
    break;

  case 256:
#line 1254 "ferite_parser.y"
    {  ferite_do_exit(); ;}
    break;

  case 258:
#line 1259 "ferite_parser.y"
    {
                        ferite_do_add_variable( (yyvsp[(2) - (3)].text), F_VAR_OBJ, ferite_subtype_link(CURRENT_SCRIPT, "O"), FE_FALSE, FE_TRUE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );
                        ferite_do_variable_push( (yyvsp[(2) - (3)].text) );
                        ferite_do_variable_push( "err" );
                        ferite_do_binary_op( FERITE_OPCODE_assign );
                    ;}
    break;

  case 259:
#line 1268 "ferite_parser.y"
    { ferite_do_monitor_block(); ;}
    break;

  case 260:
#line 1269 "ferite_parser.y"
    { ferite_do_before_handle_block(); ;}
    break;

  case 261:
#line 1270 "ferite_parser.y"
    { ferite_do_begin_block(); ;}
    break;

  case 262:
#line 1272 "ferite_parser.y"
    { ferite_do_end_block() ; ;}
    break;

  case 264:
#line 1277 "ferite_parser.y"
    { ferite_do_if_statement(); ;}
    break;

  case 268:
#line 1284 "ferite_parser.y"
    { ferite_do_after_then_statement(); ;}
    break;

  case 269:
#line 1285 "ferite_parser.y"
    { ferite_do_after_then_before_else_statement(); ;}
    break;

  case 270:
#line 1286 "ferite_parser.y"
    {  ferite_do_after_else_statement(); ;}
    break;

  case 271:
#line 1290 "ferite_parser.y"
    {
                                        ferite_do_pre_switch();
                                    ;}
    break;

  case 272:
#line 1296 "ferite_parser.y"
    {
                                        ferite_do_post_switch();
                                    ;}
    break;

  case 278:
#line 1319 "ferite_parser.y"
    {
                                        ferite_do_case_block_start();
                                    ;}
    break;

  case 279:
#line 1322 "ferite_parser.y"
    {
                                        ferite_do_case_block_end();
                                    ;}
    break;

  case 281:
#line 1329 "ferite_parser.y"
    {
                                        ferite_do_default_block_start();
                                    ;}
    break;

  case 283:
#line 1336 "ferite_parser.y"
    { ferite_do_after_fix_block(); ;}
    break;

  case 284:
#line 1337 "ferite_parser.y"
    { ferite_do_after_fix_before_else_block(); ;}
    break;

  case 285:
#line 1338 "ferite_parser.y"
    { ferite_do_after_fix_else_statement(); ;}
    break;

  case 286:
#line 1342 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_number_long_variable( NULL, "forconst", 1, FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: for truth value\n"));
                                    ;}
    break;

  case 288:
#line 1352 "ferite_parser.y"
    { ferite_do_pop(); ;}
    break;

  case 290:
#line 1357 "ferite_parser.y"
    {
                                        ferite_error( CURRENT_SCRIPT, 0, "'foreach' has been removed from ferite for more elegant methods.\n" );
                                        ferite_error( CURRENT_SCRIPT, 0, "Please console the documentation for the Array module.\n" );
                                        feperror( NULL );
                                    ;}
    break;

  case 291:
#line 1365 "ferite_parser.y"
    {
                                    ferite_do_begin_block();
                                        FUD(("PARSER: [FOR] Begin Initial Expression\n"));
                                    ;}
    break;

  case 292:
#line 1369 "ferite_parser.y"
    {
                                        FUD(("PARSER: [FOR] End Initial Expression\n"));
                                        ferite_do_for_loop_start();
                                    ;}
    break;

  case 293:
#line 1373 "ferite_parser.y"
    { FUD(("PARSER: [FOR] Begin Test Expression\n")); ;}
    break;

  case 294:
#line 1374 "ferite_parser.y"
    { FUD(("PARSER: [FOR] End Test Expression\n")); ;}
    break;

  case 295:
#line 1375 "ferite_parser.y"
    {
                                        FUD(("PARSER: [FOR] Begin Itterator Expression\n"));
                                        ferite_do_for_loop_itterate();
                                    ;}
    break;

  case 296:
#line 1379 "ferite_parser.y"
    {
                                        FUD(("PARSER: [FOR] End Itterator Expression\n"));
                                        ferite_do_pop();
                                        ferite_do_for_loop_block();
                                    ;}
    break;

  case 297:
#line 1385 "ferite_parser.y"
    {
                                        FUD(("PARSER: [FOR] Jump back to test expression\n"));
                                        ferite_do_for_loop_end();
                    ferite_do_end_block();
                                    ;}
    break;

  case 298:
#line 1391 "ferite_parser.y"
    { ferite_do_while_begin(); ;}
    break;

  case 299:
#line 1392 "ferite_parser.y"
    { ferite_do_while_after_expr(); ;}
    break;

  case 300:
#line 1393 "ferite_parser.y"
    { ferite_do_while_end(); ;}
    break;

  case 301:
#line 1394 "ferite_parser.y"
    { ferite_do_do_start(); ;}
    break;

  case 302:
#line 1396 "ferite_parser.y"
    { ferite_do_do_end(); ;}
    break;

  case 307:
#line 1408 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, "strconst", (yyvsp[(1) - (1)].text), 0, FE_CHARSET_DEFAULT, FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: push(str)        \"%s\"\n", (yyvsp[(1) - (1)].text)));
                                        ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
                                    ;}
    break;

  case 308:
#line 1416 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_boolean_variable( NULL, "true", FE_TRUE, FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: push(str)        \"true\"\n"));
                                    ;}
    break;

  case 309:
#line 1423 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_boolean_variable( NULL, "false", FE_FALSE, FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: push(str)        \"false\"\n"));
                                    ;}
    break;

  case 310:
#line 1430 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_number_long_variable( NULL, "longconst", (yyvsp[(1) - (1)].inumber), FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: push(int)        %ld\n", (yyvsp[(1) - (1)].inumber)));
                                    ;}
    break;

  case 311:
#line 1437 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_number_double_variable( NULL, "doubleconst", (yyvsp[(1) - (1)].fnumber), FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: push(float)      %f\n", (yyvsp[(1) - (1)].fnumber)));
                                    ;}
    break;

  case 312:
#line 1444 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_number_long_variable( NULL, "__LINE__", ferite_scanner_lineno, FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                    ;}
    break;

  case 313:
#line 1450 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, "__FILE__", ferite_scanner_file, 0, FE_CHARSET_DEFAULT, FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        MARK_VARIABLE_AS_FINALSET( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                    ;}
    break;

  case 314:
#line 1459 "ferite_parser.y"
    {
                                        ferite_do_variable_push( (yyvsp[(1) - (1)].text) );
                                        FUD(("PARSER: push(cvar)       %s\n", (yyvsp[(1) - (1)].text)));
                                        ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
                                    ;}
    break;

  case 315:
#line 1464 "ferite_parser.y"
    {   ferite_do_variable_push( "self" ); ferite_objcall_is_self = FE_TRUE; ;}
    break;

  case 316:
#line 1465 "ferite_parser.y"
    {   ferite_do_variable_push( "super" ); ferite_objcall_is_super = FE_TRUE; ;}
    break;

  case 317:
#line 1467 "ferite_parser.y"
    {
                                        FUD(("PARSER: op->array_slice\n"));
                                        ferite_do_many_op( FERITE_OPCODE_array_slice,3);
                                    ;}
    break;

  case 318:
#line 1471 "ferite_parser.y"
    {
                                        FUD(("PARSER: op->array_slice\n"));
                                        ferite_temp_variable = ferite_create_number_long_variable( NULL, "slice_first",0, FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                    ;}
    break;

  case 319:
#line 1477 "ferite_parser.y"
    { ferite_do_many_op(FERITE_OPCODE_array_slice,3); ;}
    break;

  case 320:
#line 1478 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_void_variable( NULL, "sliceconst", FE_STATIC );
                                        MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        ferite_do_many_op(FERITE_OPCODE_array_slice,3);
                                    ;}
    break;

  case 321:
#line 1485 "ferite_parser.y"
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

  case 322:
#line 1495 "ferite_parser.y"
    {
                                        FUD(("PARSER: op->array_index"));
                                        ferite_do_binary_op(FERITE_OPCODE_array_index);
                                    ;}
    break;

  case 323:
#line 1499 "ferite_parser.y"
    {   /* that dash cunning end of array index type thingy.... */
                                        /* it wasnt cunning enough because it broke with the introduction of negative indices ;) - ntropy*/
                                        ferite_temp_variable = ferite_create_void_variable( NULL, "void-placeholder", FE_STATIC );
                                        MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        FUD(("PARSER: push(int)        %d\n", FE_ARRAY_ADD_AT_END ));
                                        FUD(("PARSER: op->array_index\n"));
                                        ferite_do_binary_op( FERITE_OPCODE_array_index );
                                    ;}
    break;

  case 326:
#line 1511 "ferite_parser.y"
    { ferite_do_self_push(); ferite_objcall_is_self = FE_TRUE; ;}
    break;

  case 332:
#line 1526 "ferite_parser.y"
    {
                                        ferite_do_variable_push( (yyvsp[(1) - (1)].text) );
                                        FUD(("PARSER: push(cvar)       %s\n", (yyvsp[(1) - (1)].text)));
                                        ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT;
                                    ;}
    break;

  case 334:
#line 1535 "ferite_parser.y"
    {
                                        ferite_do_variable_pushattr( (yyvsp[(2) - (2)].text), ferite_objcall_is_self, ferite_objcall_is_super );
                                        if( ferite_objcall_is_self || ferite_objcall_is_super ) {
                                            ferite_objcall_is_self = FE_FALSE;
                                            ferite_objcall_is_super = FE_FALSE;
                                        }
                                        FUD(("PARSER: push(object property)       %s\n", (yyvsp[(2) - (2)].text)));
                                        ffree_ngc( (yyvsp[(2) - (2)].text) ); FECT;
                                    ;}
    break;

  case 336:
#line 1550 "ferite_parser.y"
    {
                                        int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
                                        FUD(("PARSER: op->c_obj_func       %s\n", (yyvsp[(2) - (6)].text) ));
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

  case 337:
#line 1568 "ferite_parser.y"
    { ferite_do_add_variable_to_paramlist( (yyvsp[(1) - (1)].text), F_VAR_VOID, ferite_subtype_link(CURRENT_SCRIPT, "V"), FE_BY_VALUE ); ffree_ngc( (yyvsp[(1) - (1)].text) ); FECT; ;}
    break;

  case 344:
#line 1584 "ferite_parser.y"
    { ferite_do_closure_start(); ;}
    break;

  case 345:
#line 1585 "ferite_parser.y"
    { ferite_do_closure_end_of_args(); ;}
    break;

  case 346:
#line 1586 "ferite_parser.y"
    { ferite_do_closure_end(); ;}
    break;

  case 348:
#line 1594 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_add );
                                         FUD(("PARSER: op->add          pop,pop\n"));
                                    ;}
    break;

  case 349:
#line 1598 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_minus );
                                         FUD(("PARSER: op->minus        pop,pop\n"));
                                    ;}
    break;

  case 350:
#line 1602 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_mult );
                                         FUD(("PARSER: op->mult         pop,pop\n"));
                                    ;}
    break;

  case 351:
#line 1606 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_divide );
                                         FUD(("PARSER: op->divide       pop,pop\n"));
                                    ;}
    break;

  case 352:
#line 1610 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_modulus );
                                         FUD(("PARSER: op->modulus      pop,pop\n"));
                                    ;}
    break;

  case 353:
#line 1614 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_equals );
                                         FUD(("PARSER: op->equals       pop,pop\n"));
                                    ;}
    break;

  case 354:
#line 1619 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_less_than_equals );
                                         FUD(("PARSER: op->less_than_eq pop,pop\n"));
                                    ;}
    break;

  case 355:
#line 1624 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_greater_than_equals );
                                         FUD(("PARSER: op->grtr_then_eq pop,pop\n"));
                                    ;}
    break;

  case 356:
#line 1628 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_less_than );
                                         FUD(("PARSER: op->less_than    pop,pop\n"));
                                    ;}
    break;

  case 357:
#line 1632 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_greater_than );
                                         FUD(("PARSER: op->grtr_then    pop,pop\n"));
                                    ;}
    break;

  case 358:
#line 1636 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_notequals );
                                         FUD(("PARSER: op->not_equal    pop,pop\n"));
                                    ;}
    break;

  case 359:
#line 1641 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_left_shift );
                                         FUD(("PARSER: op->left_shift    pop,pop\n"));
                                    ;}
    break;

  case 360:
#line 1646 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_right_shift );
                                         FUD(("PARSER: op->right_shift    pop,pop\n"));
                                    ;}
    break;

  case 361:
#line 1651 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_binary_or );
                                         FUD(("PARSER: op->binary_or    pop,pop\n"));
                                    ;}
    break;

  case 362:
#line 1656 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_binary_and );
                                         FUD(("PARSER: op->binary_and   pop,pop\n"));
                                    ;}
    break;

  case 363:
#line 1661 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_binary_xor );
                                         FUD(("PARSER: op->binary_xor    pop,pop\n"));
                                    ;}
    break;

  case 364:
#line 1665 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 365:
#line 1665 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_assign, FERITE_OPCODE_assign ); ;}
    break;

  case 366:
#line 1666 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 367:
#line 1666 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_add_assign, FERITE_OPCODE_add ); ;}
    break;

  case 368:
#line 1667 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 369:
#line 1667 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_minus_assign, FERITE_OPCODE_minus ); ;}
    break;

  case 370:
#line 1668 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 371:
#line 1668 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_mult_assign, FERITE_OPCODE_mult ); ;}
    break;

  case 372:
#line 1669 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 373:
#line 1669 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_divide_assign, FERITE_OPCODE_divide ); ;}
    break;

  case 374:
#line 1670 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 375:
#line 1670 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_left_shift_assign, FERITE_OPCODE_left_shift ); ;}
    break;

  case 376:
#line 1671 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 377:
#line 1671 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_right_shift_assign, FERITE_OPCODE_right_shift ); ;}
    break;

  case 378:
#line 1672 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 379:
#line 1672 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_binary_and_assign, FERITE_OPCODE_binary_and ); ;}
    break;

  case 380:
#line 1673 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 381:
#line 1673 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_binary_or_assign, FERITE_OPCODE_binary_or ); ;}
    break;

  case 382:
#line 1674 "ferite_parser.y"
    { ASSIGNMENT_SETUP() ;}
    break;

  case 383:
#line 1674 "ferite_parser.y"
    { ASSIGNMENT_TEARDOWN( FERITE_OPCODE_binary_xor_assign, FERITE_OPCODE_binary_xor ); ;}
    break;

  case 384:
#line 1676 "ferite_parser.y"
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

  case 385:
#line 1686 "ferite_parser.y"
    {
                                         FUD(("PARSER: unary plus\n"));
                                         ferite_do_unary_op( FERITE_OPCODE_positive_var );
                                    ;}
    break;

  case 386:
#line 1691 "ferite_parser.y"
    {
                                         FUD(("PARSER: unary minus\n"));
                                         ferite_do_unary_op( FERITE_OPCODE_negative_var );
                                    ;}
    break;

  case 387:
#line 1695 "ferite_parser.y"
    {
                                         FUD(("PARSER: right increment\n"));
                                         ferite_do_unary_op( FERITE_OPCODE_right_incr );
                                    ;}
    break;

  case 388:
#line 1699 "ferite_parser.y"
    {
                                         FUD(("PARSER: left increment\n"));
                                         ferite_do_unary_op( FERITE_OPCODE_left_incr );
                                    ;}
    break;

  case 389:
#line 1703 "ferite_parser.y"
    {
                                         FUD(("PARSER: right decrement\n"));
                                         ferite_do_unary_op( FERITE_OPCODE_right_decr );
                                    ;}
    break;

  case 390:
#line 1707 "ferite_parser.y"
    {
                                         FUD(("PARSER: left decrement\n"));
                                         ferite_do_unary_op( FERITE_OPCODE_left_decr );
                                    ;}
    break;

  case 391:
#line 1711 "ferite_parser.y"
    {
                                         ferite_do_unary_op( FERITE_OPCODE_not_op );
                                         FUD(("PARSER: op->not_op           pop\n" ));
                                    ;}
    break;

  case 392:
#line 1715 "ferite_parser.y"
    {
                                         ferite_do_unary_op( FERITE_OPCODE_eval );
                                         FUD(("PARSER: op->eval\n"));
                                    ;}
    break;

  case 393:
#line 1719 "ferite_parser.y"
    {
                                         ferite_do_unary_op( FERITE_OPCODE_include );
                                         FUD(("PARSER: op->include\n"));
                                    ;}
    break;

  case 394:
#line 1723 "ferite_parser.y"
    {
                                         ferite_temp_variable = ferite_create_string_variable_from_ptr( NULL, "strconst", ferite_variable_id_to_str( NULL,ferite_last_type), 0, FE_CHARSET_DEFAULT, FE_STATIC );
                                         MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                         ferite_do_push( ferite_temp_variable );
                                         ferite_do_binary_op( FERITE_OPCODE_isa );
                                         FUD(("PARSER: op->include\n"));
                                    ;}
    break;

  case 395:
#line 1730 "ferite_parser.y"
    {
                                         ferite_do_binary_op( FERITE_OPCODE_instanceof );
                                         FUD(("PARSER: op->include\n"));
                                    ;}
    break;

  case 396:
#line 1734 "ferite_parser.y"
    { ferite_do_get_args(); ;}
    break;

  case 397:
#line 1735 "ferite_parser.y"
    { ferite_do_get_yield_block(); ;}
    break;

  case 407:
#line 1748 "ferite_parser.y"
    { ferite_do_if_statement(); ;}
    break;

  case 408:
#line 1749 "ferite_parser.y"
    { ferite_do_after_then_before_else_statement(); ;}
    break;

  case 409:
#line 1750 "ferite_parser.y"
    { ferite_do_after_else_statement(); ;}
    break;

  case 411:
#line 1755 "ferite_parser.y"
    { ferite_do_yield_block( FE_FALSE ); ;}
    break;

  case 412:
#line 1756 "ferite_parser.y"
    { ferite_do_yield_block( FE_TRUE ); ;}
    break;

  case 413:
#line 1760 "ferite_parser.y"
    { ferite_do_push_main_namespace(); ;}
    break;

  case 414:
#line 1762 "ferite_parser.y"
    {
                                        int *value = ferite_stack_pop( FE_NoScript, ferite_argcount_stack );
                                        FUD(("PARSER: op->c_func       %s\n", (yyvsp[(1) - (6)].text) ));
                                        ferite_do_function_call( (yyvsp[(1) - (6)].text), ferite_current_arg_count );
                                        ffree_ngc( (yyvsp[(1) - (6)].text) ); FECT;
                                        if( value != NULL )
                                        {
                                            ferite_current_arg_count = *value;
                                            ffree_ngc( value );
                                        }
                                    ;}
    break;

  case 415:
#line 1773 "ferite_parser.y"
    { ferite_do_super_push(); ;}
    break;

  case 416:
#line 1775 "ferite_parser.y"
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

  case 417:
#line 1784 "ferite_parser.y"
    { ferite_do_self_push(); ;}
    break;

  case 418:
#line 1786 "ferite_parser.y"
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

  case 419:
#line 1796 "ferite_parser.y"
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

  case 420:
#line 1809 "ferite_parser.y"
    {
                                        int *value = fmalloc_ngc(sizeof(int));
                                        *value = ferite_current_arg_count;
                                        ferite_stack_push( FE_NoScript, ferite_argcount_stack, value );
                                        ferite_current_arg_count = 0;
                                    ;}
    break;

  case 421:
#line 1815 "ferite_parser.y"
    {
                                        int *value = fmalloc_ngc(sizeof(int));
                                        *value = ferite_current_arg_count;
                                        ferite_stack_push( FE_NoScript, ferite_argcount_stack, value );
                                        ferite_current_arg_count = 0;
                                    ;}
    break;

  case 425:
#line 1830 "ferite_parser.y"
    { ferite_current_arg_count++ ;}
    break;

  case 427:
#line 1834 "ferite_parser.y"
    {
                                        char *buf = fmalloc_ngc(256);
                                        ferite_var_array_count++;
                                        snprintf( buf, 256, "!__array%d__", ferite_var_array_count );
                                        ferite_do_add_variable( buf, F_VAR_UARRAY, ferite_subtype_link(CURRENT_SCRIPT, "A"), FE_FALSE, FE_FALSE, FE_FALSE, FE_FALSE, FE_ITEM_IS_PUBLIC, FE_FALSE );
                                        ferite_stack_push( FE_NoScript, ferite_compiled_arrays_stack, buf );
                                        FUD(("PARSER: push             array_start\n"));

                                        ferite_do_variable_push( buf );
                                        ferite_do_unary_op( FERITE_OPCODE_array_clear );
                                        ferite_do_pop();
                                    ;}
    break;

  case 428:
#line 1846 "ferite_parser.y"
    {
                                        char *buf = ferite_stack_pop( FE_NoScript, ferite_compiled_arrays_stack );
                                        ferite_do_variable_push( buf );
                                        ffree_ngc( buf );
                                    ;}
    break;

  case 433:
#line 1865 "ferite_parser.y"
    {
                                        ferite_do_variable_push( ferite_stack_top( FE_NoScript, ferite_compiled_arrays_stack ) );
                                        ferite_do_swap_top(0);
                                        ferite_do_swap_top(1);
                                        ferite_do_swap_top(0);
                                        ferite_do_many_op( FERITE_OPCODE_array_index_assign, 3 );
                                        ferite_do_pop();
                                    ;}
    break;

  case 434:
#line 1874 "ferite_parser.y"
    {
                                        ferite_temp_variable = ferite_create_void_variable( NULL, "void-placeholder", FE_STATIC );
                                        MARK_VARIABLE_AS_PLACEHOLDER( ferite_temp_variable );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                    ;}
    break;

  case 435:
#line 1880 "ferite_parser.y"
    {   ferite_do_swap_top(0); ;}
    break;

  case 436:
#line 1884 "ferite_parser.y"
    { ferite_do_if_statement(); ;}
    break;

  case 437:
#line 1885 "ferite_parser.y"
    {
                                        ferite_do_after_then_before_else_statement();
                                        /* we have to push a value onto the stack, otherwise we end up with a smashed stack */
                                        ferite_temp_variable = ferite_create_boolean_variable( NULL, "false", FE_FALSE, FE_STATIC );
                                        MARK_VARIABLE_AS_COMPILED( ferite_temp_variable );
                                        ferite_do_push( ferite_temp_variable );
                                        ferite_do_after_else_statement();
                                   ;}
    break;

  case 438:
#line 1893 "ferite_parser.y"
    { ferite_do_not_if_statement(); ;}
    break;

  case 439:
#line 1894 "ferite_parser.y"
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
#line 4890 "ferite_parser.tab.c"
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


#line 1904 "ferite_parser.y"


int ferite_parse()
{
     int parser_return = 0;

     FE_ENTER_FUNCTION;
     parser_return = fepparse();
     FE_LEAVE_FUNCTION( parser_return );
}


