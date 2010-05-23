/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 133 "ferite_parser.y"
{
	char   *text;
	int	    inumber;
	double  fnumber;
}
/* Line 1529 of yacc.c.  */
#line 285 "ferite_parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



