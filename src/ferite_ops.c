/* -*- mode: c; mode: fold; -*- */
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
#include "aphex.h"
#include <math.h>

#define GET_VAR( var ) if( var && var->accessors && var->accessors->get ) (var->accessors->get)( script, var )
#define GET_A_VAR GET_VAR( a )
#define GET_B_VAR GET_VAR( b )
#define GET_INPUT_VARS GET_A_VAR; GET_B_VAR

extern FeriteOpTable ferite_op_table[];

/*{{{ FERITE_UNARY_OP( left_incrc ) */
FERITE_UNARY_OP( left_incr )
{
    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    GET_A_VAR;

    if( !FE_VAR_IS_FINALSET( a ) )
    {
        switch( F_VAR_TYPE(a) )
        {
          case F_VAR_LONG:
            FUD(("OPS: Incrementing %s from %ld to ", a->vname, VAI( a ) ));
            VAI( a ) += 1;
            FUD(("%ld\n", VAI( a ) ));
/*            ptr = ferite_duplicate_variable( script, a, NULL );
            MARK_VARIABLE_AS_DISPOSABLE( ptr ); */
            break;
          default:
            ferite_error( script, 0, "Can't increment variables of type %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)) );
        }
        if( FE_VAR_IS_FINAL( a ) )
          MARK_VARIABLE_AS_FINALSET( a );
    }
    UNLOCK_VARIABLE(a);
    FE_LEAVE_FUNCTION( a/*ptr*/ );
}
/*}}}*/

/*{{{ FERITE_UNARY_OP( right_incr ) */
FERITE_UNARY_OP( right_incr )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    GET_A_VAR;

    if( !FE_VAR_IS_FINALSET( a ) )
    {
        switch( F_VAR_TYPE(a) )
        {
          case F_VAR_LONG:
            ptr = ferite_duplicate_variable( script, a, NULL );
            FUD(("OPS: Incrementing %s from %ld to ", a->vname, VAI( a ) ));
            VAI( a ) += 1;
            FUD(("%ld\n", VAI( a ) ));
            MARK_VARIABLE_AS_DISPOSABLE( ptr );
            break;
          default:
            ferite_error( script, 0,"Can't increment variables of type %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)) );
        }
        if( FE_VAR_IS_FINAL( a ) )
          MARK_VARIABLE_AS_FINALSET( a );
    }
    UNLOCK_VARIABLE(a);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_UNARY_OP( left_decr ) */
FERITE_UNARY_OP( left_decr )
{
    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    GET_A_VAR;

    if( !FE_VAR_IS_FINALSET( a ) )
    {
        switch( F_VAR_TYPE(a) )
        {
          case F_VAR_LONG:
            FUD(("OPS: Decrementing %s from %ld to ", a->vname, VAI( a ) ));
            VAI( a ) -= 1;
            FUD(("%ld\n", VAI( a ) ));
/*            ptr = ferite_duplicate_variable( script, a, NULL );
            MARK_VARIABLE_AS_DISPOSABLE( ptr );*/
            break;
          default:
            ferite_error( script, 0,"Can't decrement variables of type %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)) );
        }
        if( FE_VAR_IS_FINAL( a ) )
          MARK_VARIABLE_AS_FINALSET( a );
    }
    UNLOCK_VARIABLE(a);
    FE_LEAVE_FUNCTION( a/*ptr*/ );
}
/*}}}*/

/*{{{ FERITE_UNARY_OP( right_decr ) */
FERITE_UNARY_OP( right_decr )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    GET_A_VAR;

    if( !FE_VAR_IS_FINALSET( a ) )
    {
        switch( F_VAR_TYPE(a) )
        {
          case F_VAR_LONG:
            ptr = ferite_duplicate_variable( script, a, NULL );
            MARK_VARIABLE_AS_DISPOSABLE( ptr );
            FUD(("OPS: Decrementing %s from %ld to ", a->vname, VAI( a ) ));
            VAI( a ) -= 1;
            FUD(("%ld\n", VAI( a ) ));
            break;
          default:
            ferite_error( script, 0, "Can't decrement variables of type %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)) );
        }
        if( FE_VAR_IS_FINAL( a ) )
          MARK_VARIABLE_AS_FINALSET( a );
    }
    UNLOCK_VARIABLE(a);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_UNARY_OP( positive_var ) */
FERITE_UNARY_OP( positive_var )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    GET_A_VAR;

    switch( F_VAR_TYPE(a) )
    {
      case F_VAR_LONG:
        ptr = ferite_duplicate_variable( script, a, NULL );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        if( VAI(ptr) < 0 )
        {
            VAI(ptr) = 0 - VAI(ptr);
        }
        break;
      case F_VAR_DOUBLE:
        ptr = ferite_duplicate_variable( script, a, NULL );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        if( VAF(ptr) < 0 )
        {
            VAF(ptr) = 0 - VAF(ptr);
        }
        break;
      default:
        ferite_error( script, 0, "Can't positise variables of type %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)) );
    }
    UNLOCK_VARIABLE(a);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_UNARY_OP( negative_var ) */
FERITE_UNARY_OP( negative_var )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    GET_A_VAR;

    switch( F_VAR_TYPE(a) )
    {
      case F_VAR_LONG:
        ptr = ferite_duplicate_variable( script, a, NULL );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        VAI(ptr) = 0 - VAI(ptr);
        break;
      case F_VAR_DOUBLE:
        ptr = ferite_duplicate_variable( script, a, NULL );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        VAF(ptr) = 0 - VAF(ptr);
        break;
      default:
        ferite_error( script, 0, "Can't negatise variables of type %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)) );
    }
    UNLOCK_VARIABLE(a);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ MACROS FOR WRITING CLEAN OPERATORS */

#define RETVNAME( n ) "op-" n "-return-value"

#define BEGIN_BLOCK( t, var ) \
  case t: \
    switch( F_VAR_TYPE(var) ) \
      {

#define END_BLOCK \
    } \
    break;

#define BEGIN_OP( var ) \
    FeriteVariable *retv = NULL; \
    FE_ENTER_FUNCTION; \
    LOCK_VARIABLE(a); \
    LOCK_VARIABLE(b); \
    GET_INPUT_VARS; \
    switch( F_VAR_TYPE(var) ){

#define BLOCK_DIE( t ) \
  default: \
    ferite_error( script, 1, "Can't %s variables of type %s and %s\n", t,  \
                              ferite_variable_id_to_str( script,F_VAR_TYPE(a)),  ferite_variable_id_to_str( script,F_VAR_TYPE(b)) );

#define END_OP( d ) \
    BLOCK_DIE( d )\
        } \
    if( retv != NULL ) { MARK_VARIABLE_AS_DISPOSABLE( retv ); } \
    UNLOCK_VARIABLE(a); \
    UNLOCK_VARIABLE(b); \
    FE_LEAVE_FUNCTION( retv );

#define DO_OP( t, cv, c ) \
  case t: \
    cv; \
    c; \
    break;

#define FE_VAR_TRUE( var, op ) var = ferite_create_boolean_variable( script, RETVNAME( op ), FE_TRUE, FE_STATIC )
#define FE_VAR_FALSE( var, op ) var = ferite_create_boolean_variable( script, RETVNAME( op ), FE_FALSE, FE_STATIC )

#define FE_VAR_TEST( test, op ) \
	if( test ) \
		FE_VAR_TRUE( ptr, op ); \
	else \
		FE_VAR_FALSE( ptr, op );

#define FE_OP_RETURN_VOID( var, op ) \
	var = ferite_create_void_variable( script, RETVNAME( op ), FE_STATIC ); \
	MARK_VARIABLE_AS_DISPOSABLE(var); \
	FE_LEAVE_FUNCTION( var );

#define TEST_ZERO( test ) \
    if( test == 0 ){ \
        ferite_error( script, 0, "Divide By Zero Error\n" ); \
        FE_LEAVE_FUNCTION( NULL ); \
	}

/*}}}*/

/*{{{ FERITE_UNARY_OP( not_op ) */
FERITE_UNARY_OP( not_op )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    GET_A_VAR;

    if( !ferite_variable_is_false( script,a ) )
      FE_VAR_FALSE( ptr, "not-op" );
    else
      FE_VAR_TRUE( ptr, "not-op" );
    MARK_VARIABLE_AS_DISPOSABLE( ptr );
    UNLOCK_VARIABLE(a);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_UNARY_OP( eval ) */
FERITE_UNARY_OP( eval )
{
    FeriteVariable *new_script_return = NULL;

    FE_ENTER_FUNCTION;
    GET_A_VAR;

    if( F_VAR_TYPE(a) != F_VAR_STR )
    {
        ferite_error( script, 0, "Can not eval variables of type %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)) );
        new_script_return = ferite_create_number_long_variable( script, "eval-return", FE_FALSE, FE_STATIC );
    }
    else
      new_script_return = ferite_script_eval( script, FE_STR2PTR(a) );

    if( new_script_return == NULL )
      new_script_return = ferite_create_void_variable( script, "eval-gone-wrong", FE_STATIC );

    MARK_VARIABLE_AS_DISPOSABLE( new_script_return );
    FE_LEAVE_FUNCTION( new_script_return );
}
/*}}}*/

/*{{{ FERITE_UNARY_OP( include ) */
FERITE_UNARY_OP( include )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    GET_A_VAR;

    if( F_VAR_TYPE(a) != F_VAR_STR )
      ferite_error( script, 0, "You must pass include a string\n" );

    ptr = ferite_script_include( script, FE_STR2PTR(a) );

    if( ptr == NULL )
    {
        ferite_error( script, 0, "Unable to include file '%s'\n", FE_STR2PTR(a) );
        ptr = ferite_create_void_variable( script, "include-gone-wrong", FE_STATIC );
    }

    MARK_VARIABLE_AS_DISPOSABLE( ptr );
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( add ) */
FERITE_BINARY_OP( add )
{
    double dval = 0;
    FeriteString *str = NULL;

    BEGIN_OP( a )
      BEGIN_BLOCK( F_VAR_DOUBLE, b )
        DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_double_variable( script, RETVNAME( "add" ), VAF(a) + VAI(b), FE_STATIC ) )
        DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_double_variable( script, RETVNAME( "add" ), VAF(a) + VAF(b), FE_STATIC ) )
        BLOCK_DIE( "add" )
      END_BLOCK
      BEGIN_BLOCK( F_VAR_STR, b )
  		default:
    		retv = ferite_create_string_variable( script, "add", VAS(a), FE_STATIC );
    		str = ferite_variable_to_str( script, b,0 );
    		ferite_str_cat( script, VAS(retv), str );
    		ferite_str_destroy( script, str );
    		FUD(( "returning (str)\"%s\"\n", FE_STR2PTR(retv)));
      END_BLOCK
      BEGIN_BLOCK( F_VAR_LONG, b )
        DO_OP( F_VAR_LONG,
               dval = (double)VAI(a) + (double)VAI(b);
               if( dval > (double)LONG_MAX )
               retv = ferite_create_number_double_variable( script, RETVNAME( "add" ), dval, FE_STATIC );
               else
               retv = ferite_create_number_long_variable( script, RETVNAME( "add" ), VAI(a) + VAI(b), FE_STATIC ), NOWT )
        DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_double_variable( script, RETVNAME( "add" ), VAI(a)+VAF(b), FE_STATIC ); )
        BLOCK_DIE( "add" );
      END_BLOCK
    END_OP( "add" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( minus ) */
FERITE_BINARY_OP( minus )
{
    double dval = 0;
    FeriteString *var = NULL, *tmp = NULL;

    BEGIN_OP( a )
      BEGIN_BLOCK( F_VAR_DOUBLE, b )
        DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_double_variable( script, RETVNAME( "minus" ), VAF(a) - VAI(b), FE_STATIC ) )
        DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_double_variable( script, RETVNAME( "minus" ), VAF(a) - VAF(b), FE_STATIC ) )
        BLOCK_DIE( "minus" )
      END_BLOCK
      BEGIN_BLOCK( F_VAR_LONG, b )
        DO_OP( F_VAR_LONG,
               dval = (double)VAI(a) - (double)VAI(b);
               if( dval < (double)LONG_MIN )
               retv = ferite_create_number_double_variable( script, RETVNAME( "minus" ), (double)VAI(a) - (double)VAI(b), FE_STATIC );
               else
               retv = ferite_create_number_long_variable( script, RETVNAME( "minus" ), VAI(a)-VAI(b), FE_STATIC ) , NOWT )
        DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_double_variable( script, RETVNAME( "minus" ), VAI(a)-VAF(b), FE_STATIC ); )
        BLOCK_DIE( "minus" );
      END_BLOCK
      BEGIN_BLOCK( F_VAR_STR, b )
        DO_OP( F_VAR_STR,
               NOWT,
               tmp = ferite_str_new( script, "", 0, FE_CHARSET_DEFAULT );
               var = ferite_str_replace( script, VAS(a), VAS(b), tmp );
               retv = ferite_create_string_variable( script, RETVNAME( "minus" ), var, FE_STATIC );
               ferite_str_destroy( script, tmp );
               ferite_str_destroy( script, var ); )
        BLOCK_DIE( "minus" )
      END_BLOCK
    END_OP( "minus" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( mult ) */
FERITE_BINARY_OP( mult )
{
    double dval = 0;

    BEGIN_OP( a )
      BEGIN_BLOCK( F_VAR_DOUBLE, b )
        DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_double_variable( script, RETVNAME( "mult" ), VAF(a) * VAI(b), FE_STATIC ) )
        DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_double_variable( script, RETVNAME( "mult" ), VAF(a) * VAF(b), FE_STATIC ) )
		BLOCK_DIE( "mult" )
      END_BLOCK
      BEGIN_BLOCK( F_VAR_LONG, b )
        DO_OP( F_VAR_LONG,
               dval = (double)VAI(a) * (double)VAI(b);
               if( dval > (double)LONG_MAX )
               retv = ferite_create_number_double_variable( script, RETVNAME( "mult" ), dval, FE_STATIC );
               else
               retv = ferite_create_number_long_variable( script, RETVNAME( "mult" ), VAI(a) * VAI(b), FE_STATIC ); , NOWT)
        DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_double_variable( script, RETVNAME( "mult" ), VAI(a)*VAF(b), FE_STATIC ); )
        BLOCK_DIE( "mult" );
      END_BLOCK
    END_OP( "mult" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( divide ) */
FERITE_BINARY_OP( divide )
{
    BEGIN_OP( a )
      BEGIN_BLOCK( F_VAR_DOUBLE, b )
        DO_OP( F_VAR_LONG, TEST_ZERO( VAI(b) ), retv = ferite_create_number_double_variable( script, RETVNAME( "divide" ), VAF(a) / VAI(b), FE_STATIC ) )
          DO_OP( F_VAR_DOUBLE, TEST_ZERO( VAF(b) ), retv = ferite_create_number_double_variable( script, RETVNAME( "divide" ), VAF(a) / VAF(b), FE_STATIC ) )
            BLOCK_DIE( "divide" )
              END_BLOCK
      BEGIN_BLOCK( F_VAR_LONG, b )
        DO_OP( F_VAR_LONG, TEST_ZERO( VAI(b) ), retv = ferite_create_number_long_variable( script, RETVNAME( "divide" ), VAI(a)/VAI(b), FE_STATIC ); )
          DO_OP( F_VAR_DOUBLE, TEST_ZERO( VAF(b) ), retv = ferite_create_number_double_variable( script, RETVNAME( "divide" ), VAI(a)/VAF(b), FE_STATIC ); )
            BLOCK_DIE( "divide" );
    END_BLOCK
      END_OP( "divide" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( modulus ) */
FERITE_BINARY_OP( modulus )
{
    BEGIN_OP( a )
      BEGIN_BLOCK( F_VAR_DOUBLE, b )
        DO_OP( F_VAR_LONG, if( VAI(b) == 0 ) ferite_error( script, 0, "modulus By Zero Error\n" ); , retv = ferite_create_number_double_variable( script, RETVNAME( "modulus" ), (long)VAF(a) % VAI(b), FE_STATIC ) )
          DO_OP( F_VAR_DOUBLE, if( VAF(b) == 0 ) ferite_error( script, 0, "modulus By Zero Error\n" ); , retv = ferite_create_number_double_variable( script, RETVNAME( "modulus" ), (long)VAF(a) % (long)VAF(b), FE_STATIC ) )
            BLOCK_DIE( "modulus" )
              END_BLOCK
      BEGIN_BLOCK( F_VAR_LONG, b )
        DO_OP( F_VAR_LONG, if( VAI(b) == 0 ) ferite_error( script, 0, "modulus By Zero Error\n" );, retv = ferite_create_number_long_variable( script, RETVNAME( "modulus" ), VAI(a)%VAI(b), FE_STATIC ); )
          DO_OP( F_VAR_DOUBLE, if( VAF(b) == 0 ) ferite_error( script, 0, "modulus By Zero Error\n" );, retv = ferite_create_number_double_variable( script, RETVNAME( "modulus" ), VAI(a)%(long)VAF(b), FE_STATIC ); )
            BLOCK_DIE( "modulus" );
    END_BLOCK
      END_OP( "modulus" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( assign ) */
FERITE_BINARY_OP( assign )
{
    FeriteVariable *retv = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_B_VAR;
	
    if( !FE_VAR_IS_FINALSET( a ) )
    {
        switch( F_VAR_TYPE(a) )
        {
            BEGIN_BLOCK( F_VAR_LONG, b )
              DO_OP( F_VAR_LONG, VAI(a) = VAI(b);, retv = ferite_create_number_long_variable( script, RETVNAME( "assign" ), VAI(a), FE_STATIC ); )
              DO_OP( F_VAR_DOUBLE, F_VAR_TYPE(a) = F_VAR_DOUBLE; VAF(a) = VAF(b);, retv = ferite_create_number_double_variable( script, RETVNAME( "assign" ), VAF(a), FE_STATIC ); )
              DO_OP( F_VAR_BOOL, VAI(a) = VAB(b);, retv = ferite_create_number_long_variable( script, RETVNAME( "assign" ), VAI(a), FE_STATIC ); )
              BLOCK_DIE( "assign" )
            END_BLOCK

            BEGIN_BLOCK( F_VAR_BOOL, b )
              DO_OP( F_VAR_BOOL, VAB(a) = VAB(b);, retv = ferite_create_boolean_variable( script, RETVNAME( "assign" ), VAB(a), FE_STATIC ); )
              DO_OP( F_VAR_LONG, 
					if( VAI(b) == 1 || VAI(b) == 0 ) {
						VAB(a) == VAB(b);
						retv = ferite_create_number_long_variable( script, RETVNAME( "assign" ), VAB(a), FE_STATIC );
					}
					else {
						ferite_error( script, 0, "Can not assign any number other than 0 or 1 to a boolean type.\n" );
				        retv = ferite_create_void_variable( script, RETVNAME( "assign" ), FE_STATIC );
					}
					, )
              BLOCK_DIE( "assign" )
            END_BLOCK

            BEGIN_BLOCK( F_VAR_STR, b )
              DO_OP( F_VAR_STR, ferite_str_cpy( script, VAS(a), VAS(b));, retv = ferite_create_string_variable( script, RETVNAME( "assign" ), VAS(a), FE_STATIC ); )
              BLOCK_DIE( "assign" )
            END_BLOCK

            BEGIN_BLOCK( F_VAR_DOUBLE, b )
              DO_OP( F_VAR_DOUBLE, VAF(a) = VAF(b);, retv = ferite_create_number_double_variable( script, RETVNAME( "assign" ), VAF(a), FE_STATIC ); )
              DO_OP( F_VAR_LONG, F_VAR_TYPE(a) = F_VAR_LONG; VAI(a) = VAI(b);, retv = ferite_create_number_long_variable( script, RETVNAME( "assign" ), VAI(a), FE_STATIC ); )
              BLOCK_DIE( "assign" )
            END_BLOCK

            BEGIN_BLOCK( F_VAR_OBJ, b )
              DO_OP( F_VAR_OBJ,
                     if( VAO(a) != NULL )
                     {
						 FDECREFI(VAO(a), a->refcount);
#ifdef DEBUG
                         if( VAO(a)->refcount <= 0 )
							 FUD(("OPS: GC: Object \"%s\" Flagged for DELETION\n", a->vname));
#endif
                     }
                     if( VAO(b) != NULL ) {
						 FINCREFI(VAO(b), (a->refcount + 1));
					 }
                     VAO(a) = VAO(b);,
                     retv = ferite_create_object_variable( script, RETVNAME( "assign" ), FE_STATIC );
                     VAO(retv) = VAO(b);
                   )
              BLOCK_DIE( "assign" );
            END_BLOCK

            BEGIN_BLOCK( F_VAR_UARRAY, b )
              DO_OP( F_VAR_UARRAY,
                     ferite_uarray_destroy( script, VAUA(a));,
                     VAUA(a) = ferite_uarray_dup( script, VAUA(b) );
                     retv = ferite_duplicate_variable( script, a, NULL );
                   )
              BLOCK_DIE( "assign" );
            END_BLOCK

            BEGIN_BLOCK( F_VAR_VOID, b )
	            DO_OP( F_VAR_BOOL,
	                   F_VAR_TYPE(a) = F_VAR_BOOL;
	                   VAI(a) = VAI(b);,
						 retv = ferite_create_boolean_variable( script, RETVNAME( "assign" ), VAB(a), FE_STATIC );
	            )
              DO_OP( F_VAR_LONG,
                     F_VAR_TYPE(a) = F_VAR_LONG;
                     VAI(a) = VAI(b);,
					 retv = ferite_create_number_long_variable( script, RETVNAME( "assign" ), VAI(a), FE_STATIC );
              )
              DO_OP( F_VAR_DOUBLE,
                     F_VAR_TYPE(a) = F_VAR_DOUBLE;
                     VAF(a) = VAF(b);,
                     retv = ferite_create_number_double_variable( script, RETVNAME( "assign" ), VAF(a), FE_STATIC );
			  )
              DO_OP( F_VAR_STR,
                     F_VAR_TYPE(a) = F_VAR_STR;
                     VAS(a) = ferite_str_dup(script, VAS(b));,
                     retv = ferite_create_string_variable( script, RETVNAME( "assign" ), VAS(a), FE_STATIC );
              ) 
			  DO_OP( F_VAR_OBJ,
				 F_VAR_TYPE(a) = F_VAR_OBJ;
				 if( VAO(b) != NULL ) {
			 		 FINCREF(VAO(b));
				 }
				 VAO(a) = VAO(b);,
				 retv = ferite_create_number_long_variable( script, RETVNAME( "assign" ), 1, FE_STATIC );
			  )
              DO_OP( F_VAR_UARRAY,
				 F_VAR_TYPE(a) = F_VAR_UARRAY;
				 VAUA(a) = ferite_uarray_dup( script, VAUA(b) );,
				 retv = ferite_duplicate_variable( script, a, NULL );
              )
              DO_OP( F_VAR_VOID, NOWT,
				 retv = ferite_create_void_variable( script, RETVNAME( "assign" ), FE_STATIC );
              )
			  DO_OP( F_VAR_NS, NOWT,
				 F_VAR_TYPE(a) = F_VAR_NS;
				 VAP(a) = VAN(b); 
				 retv = ferite_create_void_variable( script, RETVNAME( "assign" ), FE_STATIC );
				 F_VAR_TYPE(retv) = F_VAR_NS;
				 VAP(retv) = VAN(b);
			  )
			  DO_OP( F_VAR_CLASS, NOWT,
				 F_VAR_TYPE(a) = F_VAR_CLASS;
				 VAP(a) = VAC(b); 
				 retv = ferite_create_void_variable( script, RETVNAME( "assign" ), FE_STATIC );
				 F_VAR_TYPE(retv) = F_VAR_CLASS;
				 VAP(retv) = VAC(b);
			  )
		      BLOCK_DIE( "assign" );
			END_BLOCK
				
			BEGIN_BLOCK( F_VAR_CLASS, b )
				DO_OP( F_VAR_CLASS, NOWT, 
					VAC(a) = VAC(b);
					retv = ferite_create_class_variable( script, RETVNAME( "assign" ), VAC(b), FE_STATIC );
				)
				BLOCK_DIE( "assign" );
			END_BLOCK
			
			BEGIN_BLOCK( F_VAR_NS, b )
				DO_OP( F_VAR_NS, NOWT, 
					VAN(a) = VAN(b);
					retv = ferite_create_namespace_variable( script, RETVNAME( "assign" ), VAN(b), FE_STATIC );
				)
				BLOCK_DIE( "assign" );
			END_BLOCK

		    BLOCK_DIE( "assign" );
        }
        if( FE_VAR_IS_FINAL( a ) )
          MARK_VARIABLE_AS_FINALSET( a ); /* set it so that the variable is constant for forever and a day */

        /* call the set method */
        if( a->accessors != NULL && a->accessors->set != NULL )
        {
            FUD(( "calling var->set()\n" ));
            (a->accessors->set)( script, a, b );
        }
    }
    else
    {
        ferite_error( script, 0, "Can not assign to a constant variable.\n" );
        retv = ferite_create_void_variable( script, RETVNAME( "assign" ), FE_STATIC );
    }
    if( retv != NULL )
      MARK_VARIABLE_AS_DISPOSABLE( retv );
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( retv );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( add_assign ) */
FERITE_BINARY_OP( add_assign )
{
    FeriteVariable *ptr = NULL, *tmp_ptr = NULL;
    
    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    
    tmp_ptr = ferite_op_add( script, current_op, a, b );
    if( tmp_ptr != NULL )
    {	
        ptr = ferite_op_assign( script, current_op, a, tmp_ptr );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        ferite_variable_destroy( script, tmp_ptr );
    }
    
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( minus_assign ) */
FERITE_BINARY_OP( minus_assign )
{
    FeriteVariable *ptr = NULL, *tmp_ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);

    tmp_ptr = ferite_op_minus( script, current_op, a, b );
    if( tmp_ptr != NULL )
    {
        ptr = ferite_op_assign( script, current_op, a, tmp_ptr );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        ferite_variable_destroy( script, tmp_ptr );
    }
    
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( mult_assign ) */
FERITE_BINARY_OP( mult_assign )
{
    FeriteVariable *ptr = NULL, *tmp_ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);

    tmp_ptr = ferite_op_mult( script, current_op, a, b );
    if( tmp_ptr !=  NULL )
    {
        ptr = ferite_op_assign( script, current_op, a, tmp_ptr );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        ferite_variable_destroy( script, tmp_ptr );
    }
    
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( divide_assign ) */
FERITE_BINARY_OP( divide_assign )
{
    FeriteVariable *ptr = NULL, *tmp_ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);

    tmp_ptr = ferite_op_divide( script, current_op, a, b );
    if( tmp_ptr != NULL )
    {
        ptr = ferite_op_assign( script, current_op, a, tmp_ptr );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        ferite_variable_destroy( script, tmp_ptr );
    }
    
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( logical_or ) */
FERITE_BINARY_OP( logical_or )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;

    FE_VAR_TEST( (!ferite_variable_is_false( script,a) || !ferite_variable_is_false( script,b )), "logical_or" );
    MARK_VARIABLE_AS_DISPOSABLE( ptr );

    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( logical_and ) */
FERITE_BINARY_OP( logical_and )
{
    FeriteVariable *ptr;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;

    FE_VAR_TEST( (!ferite_variable_is_false( script,a) && !ferite_variable_is_false( script,b )), "logical_and" );
    MARK_VARIABLE_AS_DISPOSABLE( ptr );

    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( equals ) */
/* binary operators that push a true or falsae value onto the stack */
FERITE_BINARY_OP( equals )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;
    if( F_VAR_TYPE(a) != F_VAR_TYPE(b) && 
        !(F_VAR_TYPE(a) == F_VAR_LONG && F_VAR_TYPE(b) == F_VAR_DOUBLE) && 
        !(F_VAR_TYPE(a) == F_VAR_DOUBLE && F_VAR_TYPE(b) == F_VAR_LONG) )
    {
        FUD(("OPS: Variable types do not match in equals( %s, %s )\nReturning false\n", a->vname, b->vname ));
        ptr = ferite_create_number_long_variable( script, "equals", 0, FE_STATIC );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        UNLOCK_VARIABLE(a);
        UNLOCK_VARIABLE(b);
        FE_LEAVE_FUNCTION( ptr );
    }
    /* we know that they are the same */
    switch( F_VAR_TYPE(a) )
    {
      case F_VAR_LONG:
      case F_VAR_DOUBLE:
      {
          double left = (F_VAR_TYPE(a) == F_VAR_LONG ? (double)VAI(a) : VAF(a));
          double right = (F_VAR_TYPE(b) == F_VAR_LONG ? (double)VAI(b) : VAF(b));
          double delta = left - right;
          if( F_VAR_TYPE(a) == F_VAR_DOUBLE || F_VAR_TYPE(b) == F_VAR_DOUBLE )
          {
              FE_VAR_TEST( (delta < 0.000001 && delta > -0.000001), "equals" );
          }
          else
          {
              FE_VAR_TEST( (VAI(a) == VAI(b)), "equals" );
          }
          break;
      }
      case F_VAR_BOOL:
	  FE_VAR_TEST( (VAB(a) == VAB(b)), "equals");
	  break;
      case F_VAR_STR:
          FE_VAR_TEST( (ferite_str_cmp( script, VAS(a), VAS(b) ) == 1), "equals" );
        break;
      case F_VAR_OBJ:
          FE_VAR_TEST( (VAO(a) == VAO(b)), "equals" );
          break;
      case F_VAR_UARRAY:
          FE_VAR_TEST( (ferite_uarray_cmp( script, VAUA(a), VAUA(b) ) == 1), "equals" );
          break;
      case F_VAR_VOID:
          FE_VAR_TEST( FE_TRUE, "equals" );
          break;
      case F_VAR_CLASS:
          FE_VAR_TEST( (VAC(a) == VAC(b)), "equals" );
          break;
      case F_VAR_NS:
          FE_VAR_TEST( (VAN(a) == VAN(b)), "equals" );
          break;          
      default:
        ferite_error( script, 0, "EEEK: unknown type %s in equals()\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a) ) );
    }
    if( ptr )
      MARK_VARIABLE_AS_DISPOSABLE( ptr );
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( case ) */
/* binary operators that push a true or falsae value onto the stack */
FERITE_BINARY_OP( case )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;

    if( F_VAR_TYPE(a) != F_VAR_TYPE(b) && 
        !(F_VAR_TYPE(a) == F_VAR_LONG && F_VAR_TYPE(b) == F_VAR_DOUBLE) && 
        !(F_VAR_TYPE(a) == F_VAR_DOUBLE && F_VAR_TYPE(b) == F_VAR_LONG) )
    {
        FUD(("OPS: Variable types do not match in equals( %s, %s )\nReturning false\n", a->vname, b->vname ));
        ptr = ferite_create_number_long_variable( script, "equals", 0, FE_STATIC );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        UNLOCK_VARIABLE(a);
        UNLOCK_VARIABLE(b);
        FE_LEAVE_FUNCTION( ptr );
    }
    /* we know that they are the same */
    switch( F_VAR_TYPE(a) )
    {
        case F_VAR_LONG:
        case F_VAR_DOUBLE:
        {
            double left = (F_VAR_TYPE(a) == F_VAR_LONG ? (double)VAI(a) : VAF(a));
            double right = (F_VAR_TYPE(b) == F_VAR_LONG ? (double)VAI(b) : VAF(b));
            double delta = left - right;
            if( F_VAR_TYPE(a) == F_VAR_DOUBLE || F_VAR_TYPE(b) == F_VAR_DOUBLE )
            {
                FE_VAR_TEST( (delta < 0.000001 && delta > -0.000001), "equals" );
            }
            else
            {
                FE_VAR_TEST( (VAI(a) == VAI(b)), "equals" );
            }
            break;
        }
        case F_VAR_BOOL:
	    FE_VAR_TEST( (VAB(a) == VAB(b)), "equals");
	    break;
        case F_VAR_STR:
            FE_VAR_TEST( (ferite_str_cmp( script, VAS(a), VAS(b) ) == 1), "equals" );
            break;
        case F_VAR_OBJ:
            FE_VAR_TEST( (VAO(a) == VAO(b)), "equals" );
            break;
        default:
            ferite_error( script, 0, "EEEK: unknown type %s in equals()\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a) ) );
    }
    if( ptr )
      MARK_VARIABLE_AS_DISPOSABLE( ptr );
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( notequals ) */
FERITE_BINARY_OP( notequals )
{
    FeriteVariable *ptr, *tptr;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;
    tptr = ferite_op_equals( script, current_op, a, b );
    FE_VAR_TEST( (ferite_variable_is_false( script,tptr )), "not_equals" );
    MARK_VARIABLE_AS_DISPOSABLE( ptr );
    ferite_variable_destroy( script, tptr );
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( less_than ) */
FERITE_BINARY_OP( less_than )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;

    switch( F_VAR_TYPE(a) )
    {
      case F_VAR_LONG:
      case F_VAR_DOUBLE:
      {
          switch( F_VAR_TYPE(b) )
          {
              case F_VAR_LONG:
              case F_VAR_DOUBLE:
              {
                  double left = (F_VAR_TYPE(a) == F_VAR_LONG ? (double)VAI(a) : VAF(a));
                  double right = (F_VAR_TYPE(b) == F_VAR_LONG ? (double)VAI(b) : VAF(b));
                  double delta = left - right;
                  if( F_VAR_TYPE(a) == F_VAR_DOUBLE || F_VAR_TYPE(b) == F_VAR_DOUBLE )
                  {
                      FE_VAR_TEST( (delta < 0.0), "less-than" );
                  }
                  else
                  {
                      FE_VAR_TEST( (VAI(a) < VAI(b)), "less-than" );
                  }
                  break;
              }
              default:
                  ferite_error( script, 0, "ERROR: can't compare values of type %s with integers\n", ferite_variable_id_to_str( script,F_VAR_TYPE(b)) );
          }
          break;
      }
      default:
          ferite_error( script, 0, "ERK, can't compare items of type %s and %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)), ferite_variable_id_to_str( script, F_VAR_TYPE(b) ));
    }
    if( ptr )
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( less_than_equals ) */
FERITE_BINARY_OP( less_than_equals )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;
    
    switch( F_VAR_TYPE(a) )
    {
        case F_VAR_LONG:
        case F_VAR_DOUBLE:
        {
            switch( F_VAR_TYPE(b) )
            {
                case F_VAR_LONG:
                case F_VAR_DOUBLE:
                {
                    double left = (F_VAR_TYPE(a) == F_VAR_LONG ? (double)VAI(a) : VAF(a));
                    double right = (F_VAR_TYPE(b) == F_VAR_LONG ? (double)VAI(b) : VAF(b));
                    double delta = left - right;
                    if( F_VAR_TYPE(a) == F_VAR_DOUBLE || F_VAR_TYPE(b) == F_VAR_DOUBLE )
                    {
                        FE_VAR_TEST( (delta <= 0.0), "less-than-equals" );
                    }
                    else
                    {
                        FE_VAR_TEST( (VAI(a) <= VAI(b)), "less-than-equals" );
                    }
                    break;
                }
                default:
                    ferite_error( script, 0, "ERROR: can't compare values of type %s with integers\n", ferite_variable_id_to_str( script,F_VAR_TYPE(b)) );
            }
            break;
        }
        default:
            ferite_error( script, 0, "ERK, can't compare items of type %s and %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)), ferite_variable_id_to_str( script, F_VAR_TYPE(b) ));
    }
    if( ptr )
      MARK_VARIABLE_AS_DISPOSABLE( ptr );
            
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( greater_than ) */
FERITE_BINARY_OP( greater_than )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;
    
    switch( F_VAR_TYPE(a) )
    {
        case F_VAR_LONG:
        case F_VAR_DOUBLE:
        {
            switch( F_VAR_TYPE(b) )
            {
                case F_VAR_LONG:
                case F_VAR_DOUBLE:
                {
                    double left = (F_VAR_TYPE(a) == F_VAR_LONG ? (double)VAI(a) : VAF(a));
                    double right = (F_VAR_TYPE(b) == F_VAR_LONG ? (double)VAI(b) : VAF(b));
                    double delta = left - right;
                    if( F_VAR_TYPE(a) == F_VAR_DOUBLE || F_VAR_TYPE(b) == F_VAR_DOUBLE )
                    {
                        FE_VAR_TEST( (delta > 0.0), "greater-than" );
                    }
                    else
                    {
                        FE_VAR_TEST( (VAI(a) > VAI(b)), "greater-than" );
                    }
                    break;
                }
                default:
                    ferite_error( script, 0, "ERROR: can't compare values of type %s with integers\n", ferite_variable_id_to_str( script,F_VAR_TYPE(b)) );
            }
            break;
        }
        default:
            ferite_error( script, 0, "ERK, can't compare items of type %s and %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)), ferite_variable_id_to_str( script, F_VAR_TYPE(b) ));
    }
    if( ptr )
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
    
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( greater_than_equals ) */
FERITE_BINARY_OP( greater_than_equals )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;
    
    switch( F_VAR_TYPE(a) )
    {
        case F_VAR_LONG:
        case F_VAR_DOUBLE:
        {
            switch( F_VAR_TYPE(b) )
            {
                case F_VAR_LONG:
                case F_VAR_DOUBLE:
                {
                    double left = (F_VAR_TYPE(a) == F_VAR_LONG ? (double)VAI(a) : VAF(a));
                    double right = (F_VAR_TYPE(b) == F_VAR_LONG ? (double)VAI(b) : VAF(b));
                    double delta = left - right;
                    if( F_VAR_TYPE(a) == F_VAR_DOUBLE || F_VAR_TYPE(b) == F_VAR_DOUBLE )
                    {
                        FE_VAR_TEST( (delta >= 0.0), "greater-than-equals" );
                    }
                    else
                    {
                        FE_VAR_TEST( (VAI(a) >= VAI(b)), "greater-than-equals" );
                    }
                    break;
                }
                default:
                    ferite_error( script, 0, "ERROR: can't compare values of type %s with integers\n", ferite_variable_id_to_str( script,F_VAR_TYPE(b)) );
            }
            break;
        }
        default:
            ferite_error( script, 0, "ERK, can't compare items of type %s and %s\n", ferite_variable_id_to_str( script,F_VAR_TYPE(a)), ferite_variable_id_to_str( script, F_VAR_TYPE(b) ));
    }
    if( ptr )
      MARK_VARIABLE_AS_DISPOSABLE( ptr );
    
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_MANY_OP( array_slice ) */
FERITE_MANY_OP(array_slice)
{
    FeriteVariable *result = NULL;
    FeriteVariable *array = NULL, *a = NULL, *b = NULL, *tmp = NULL;
    long size,lower,upper,ii,cal_lo,cal_up;
    char *buf = NULL;
    FeriteString *str = NULL;

    FE_ENTER_FUNCTION;

    /* FIXME: thread locking! */

    array = vars[2];
    a = vars[1]; /* the lower bound */
    b = vars[0]; /* the upper bound */

    GET_VAR( array );
    GET_INPUT_VARS;

    if(F_VAR_TYPE(array) != F_VAR_STR && F_VAR_TYPE(array) != F_VAR_UARRAY)
    {
        ferite_error(script, 0, "Slices only work on Arrays and Strings\n");
        FE_OP_RETURN_VOID( result, "array_slice" );
    }

    if(F_VAR_TYPE(array) == F_VAR_STR)
      size = FE_STRLEN(array);
    else
      size = VAUA(array)->size;
    
    if(F_VAR_TYPE(a) == F_VAR_LONG)
      lower = VAI(a);
    else if(F_VAR_TYPE(a) == F_VAR_DOUBLE)
      lower = (int)floor(VAF(a));
    else
    {
        ferite_error(script, 0, "Invalid lower slice bound type: %s\n", ferite_variable_id_to_str(script, F_VAR_TYPE(a)));
        FE_OP_RETURN_VOID( result, "array_slice" );
    }

    if(F_VAR_TYPE(b) == F_VAR_VOID && FE_VAR_IS_PLACEHOLDER( b ) )
      upper = size - 1;
    else if(F_VAR_TYPE(b) == F_VAR_LONG)
      upper = VAI(b);
    else if(F_VAR_TYPE(b) == F_VAR_DOUBLE)
      upper = (int)floor(VAF(b));
    else
    {
        ferite_error(script, 0, "Invalid upper slice bound type: %s\n", ferite_variable_id_to_str(script, F_VAR_TYPE(b)));
        FE_OP_RETURN_VOID( result, "array_slice" );
    }

    cal_lo = lower;
    if(lower < 0)
      cal_lo = size + lower;
    
    cal_up = upper;
    if( upper < 0 )
      cal_up = size + upper;

    if(cal_lo < 0 || cal_up < 0 || cal_lo > size - 1 || cal_up > size - 1)
    {
        ferite_error(script, 0, "Invalid slice bounds: %d (%d) to %d (%d)\n",lower,cal_lo,upper,cal_up);
        FE_OP_RETURN_VOID( result, "array_slice" );
    }
    if(F_VAR_TYPE(array) == F_VAR_STR)
    {
        str = ferite_str_new( script, NULL, FE_STRLEN(array), FE_CHARSET_DEFAULT );
        buf = str->data;
        ii = 0;
        if(cal_lo > cal_up)
        {
            for(size=cal_lo;size >= cal_up; size--)
            {
                buf[ii] = FE_STR2PTR(array)[size];
                ii++;
            }
        }
        else
        {
            for(size=cal_lo;size <= cal_up; size++)
            {
                buf[ii] = FE_STR2PTR(array)[size];
                ii++;
            }
        }

        buf[ii] = '\0';
        str->length = ii;

        result = ferite_create_string_variable( script, "spliced_content", str, FE_STATIC );
        ferite_str_destroy( script, str );
    }
    else
    {
        result = ferite_create_uarray_variable( script, "spliced_content", VAUA(array)->size, FE_STATIC );
        if(cal_lo > cal_up)
        {
            for(size=cal_lo;size >= cal_up; size--)
            {
                tmp = ferite_uarray_get_index(script, VAUA(array),(int)size);
                tmp = ferite_duplicate_variable(script, tmp, NULL);
                ferite_uarray_add(script, VAUA(result), tmp, NULL, FE_ARRAY_ADD_AT_END);
            }
        }
        else
        {
            for(size=cal_lo;size <= cal_up; size++)
            {
                tmp = ferite_uarray_get_index(script, VAUA(array),(int)size);
                tmp = ferite_duplicate_variable(script, tmp, NULL);
                ferite_uarray_add(script, VAUA(result), tmp, NULL, FE_ARRAY_ADD_AT_END);
            }
        }
    }
    MARK_VARIABLE_AS_DISPOSABLE( result );
    FE_LEAVE_FUNCTION( result );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( array_index ) */
FERITE_BINARY_OP( array_index )
{
   /* a = array variable */
   /* b = index */
    FeriteVariable *ptr = NULL;
    char *s;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;

    if( F_VAR_TYPE(a) != F_VAR_STR && F_VAR_TYPE(a) != F_VAR_UARRAY)
    {
        ferite_error( script, 0, "OPS: array_index: First Variable is not an Array or String\n" );
        UNLOCK_VARIABLE(a);
        UNLOCK_VARIABLE(b);
        FE_LEAVE_FUNCTION(ptr);
    }

    switch( F_VAR_TYPE(a) )
    {
      case F_VAR_STR:
        {
            switch( F_VAR_TYPE(b) )
            {
              case F_VAR_LONG:
                {
                    size_t offset;
                    if( VAI(b) > (signed)FE_STRLEN( a ) )
                    {
                        ferite_error( script, 0, "String index out of range [%d]\n", VAI(b) );
                        break;
                    }
                    s = fmalloc(sizeof(char) * 2);
                    if(VAI(b) < 0)
                    {
                        offset = FE_STRLEN( a ) + VAI( b );
                        // printf("grabbing index %d (%d)\n",offset,strlen(VAS(a)) -1 );
                    }
                    else
                    {
                        offset = VAI(b);
                    }
                    s[0] = FE_STR2PTR(a)[offset];
                    s[1] = '\0';
                    ptr = ferite_create_string_variable_from_ptr( script, "array_String_return", s, 1, FE_CHARSET_DEFAULT, FE_STATIC );
                    MARK_VARIABLE_AS_DISPOSABLE( ptr );
                    ffree(s);
                }

                break;
              case F_VAR_DOUBLE:
                {
                    size_t index = (size_t)VAF(b);
                    if( VAI(b) > (signed)FE_STRLEN( a ) )
                    {
                        ferite_error( script, 0, "String index out of range [%d]\n", index );
                        break;
                    }
                    s = fmalloc(sizeof(char) * 2);
                    if(index < 0)
                    {
                        index = FE_STRLEN( a ) + index;
                    }
                    s[0] = FE_STR2PTR( a )[index];
                    s[1] = '\0';
                    ptr = ferite_create_string_variable_from_ptr( script, "array_String_return", s, 1, FE_CHARSET_DEFAULT, FE_STATIC );
                    MARK_VARIABLE_AS_DISPOSABLE( ptr );
                    ffree(s);
                }
                break;
              default:
                ferite_error( script, 0, "Other index methods not implemented in strings (%s)\n", ferite_variable_id_to_str( script, F_VAR_TYPE(b) ) );
            }
            break;
        }
      case F_VAR_UARRAY:
        {
			
            if( !ferite_variable_is_false(script, b) || (F_VAR_TYPE(b) == F_VAR_VOID && FE_VAR_IS_PLACEHOLDER(b)) || F_VAR_TYPE(b) == F_VAR_LONG ) {
                ptr = ferite_uarray_get( script, VAUA(a), b );
				if( ptr == NULL ) {
					ptr = ferite_create_void_variable( script, "no-entry", FE_STATIC );
					MARK_VARIABLE_AS_DISPOSABLE( ptr );
				}
			}
			else 
	            ferite_error( script, 0, "You have provided an bad index method for the array (%s) - it could be an empty string, null object etc.\n", ferite_variable_id_to_str( script, F_VAR_TYPE(b) ) );
            break;
        }
    }
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

int ferite_can_assign( FeriteScript *script, FeriteVariable *left, FeriteVariable *right, FeriteVariableSubType *left_subtype, FeriteVariableSubType *right_subtype ) {
	if( left_subtype == NULL ) {
//		printf("no subtype - can assign any type\n");
		return FE_TRUE;
	} else {
		if( ferite_types_are_equal( script, left_subtype->type, right_subtype->type ) ) {
//			printf("types equal\n");
			switch( left_subtype->type ) {
				case F_VAR_OBJ:
				case F_VAR_CLASS:
					if( left_subtype->data._class ) {
						FeriteClass *source = (left_subtype->type == F_VAR_OBJ ? VAO(right)->klass : VAC(right));
//						printf("  we have a specification:\n");
						if( source == left_subtype->data._class ) {
//							printf("    same class\n");
							return FE_TRUE;
						} else if( ferite_class_is_subclass( left_subtype->data._class, source ) ) {
//							printf("    sub class\n");
							return FE_TRUE;
						}
					} else {
						return FE_TRUE;
					}
					break;
				default:
					return FE_TRUE;
			}
		}
	}

//	printf("!!!!not a valid assignment\n");
	return FE_FALSE;
}

FERITE_MANY_OP( array_index_assign )
{
    FeriteVariable *result = NULL;
    FeriteVariable *array = NULL, *a = NULL, *b = NULL;
	int sub_op = 0;
	
    FE_ENTER_FUNCTION;

    array = vars[2];
    a = vars[1]; /* the index bound */
    b = vars[0]; /* the value */

    GET_VAR( array );
    GET_INPUT_VARS;

	sub_op = (int)((long)current_op->opdataf);

    if(F_VAR_TYPE(array) != F_VAR_UARRAY)
    {
        ferite_error(script, 0, "You can only index assign to arrays\n");
        FE_OP_RETURN_VOID( result, "array_assign" );
    }
    if( !ferite_variable_is_false(script, a) || (F_VAR_TYPE(a) == F_VAR_VOID && FE_VAR_IS_PLACEHOLDER(a)) || F_VAR_TYPE(a) == F_VAR_LONG ) {
		if( sub_op != FERITE_OPCODE_assign ) {
			FeriteVariable *tmp = ferite_uarray_get( script, VAUA(array), a );
			FeriteVariable *(*binaryop)( FeriteScript *s, FeriteOp*, FeriteVariable *a, FeriteVariable *b );

			if( !tmp ) {
				FE_LEAVE_FUNCTION(NULL);
			}
		    binaryop = (FeriteVariable *(*)( FeriteScript *, FeriteOp*, FeriteVariable *, FeriteVariable * ))ferite_op_table[sub_op].ptr;
			b = (binaryop)( script, current_op, tmp, b );
		}
		if( NO_ERROR(script ) ) {
			int can_assign = FE_FALSE;

//			can_assign = ferite_can_assign( script, a, b, array->subtype->data._subtype, b->subtype );

//			if( can_assign ) {
//				printf("allowed to perform array set\n");
//			}

			ferite_uarray_set( script, VAUA(array), a, b );
		}
		
	} else 
        ferite_error( script, 0, "You have provided an bad index method for the array (%s) - it could be an empty string, null object etc.\n", ferite_variable_id_to_str( script, F_VAR_TYPE(a) ) );

	if( NO_ERROR(script) ) {
		result = ferite_duplicate_variable( script, b, NULL );
		MARK_VARIABLE_AS_DISPOSABLE( result );
    }
	FE_LEAVE_FUNCTION( result );
}

/*{{{ FERITE_UNARY_OP( array_clear ) */
FERITE_UNARY_OP( array_clear )
{
    FeriteVariable *ptr = NULL;
    long iteration = 0, iteration_type = 0;
    
    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    GET_A_VAR;

    if( F_VAR_TYPE(a) == F_VAR_UARRAY && VAUA(a)->size > 0 )
    {
		iteration = VAUA(a)->iteration;
		iteration_type = VAUA(a)->iterator_type;
	
        ferite_uarray_destroy( script, VAUA(a) );
        VAUA(a) = ferite_uarray_create( script );
	
		VAUA(a)->iteration = iteration;
		VAUA(a)->iterator_type = iteration_type;
    }
    UNLOCK_VARIABLE(a);
    FE_OP_RETURN_VOID( ptr, "array_clear" );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( left_shift ) */
FERITE_BINARY_OP( left_shift )
{
	BEGIN_OP( a )
		BEGIN_BLOCK( F_VAR_LONG, b )
			DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "left_shift" ), VAI(a) << VAI(b), FE_STATIC ); )
			DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "left_shift" ), VAI(a) << (long)VAF(b), FE_STATIC ); )
			BLOCK_DIE( "left_shift" );
		END_BLOCK
		BEGIN_BLOCK( F_VAR_DOUBLE, b )
			DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "left_shift" ), (long)VAF(a) << VAI(b), FE_STATIC ); )
			DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "left_shift" ), (long)VAF(a) << (long)VAF(b), FE_STATIC ); )
			BLOCK_DIE( "left_shift" );
		END_BLOCK
	END_OP( "left_shift" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( right_shift ) */
FERITE_BINARY_OP( right_shift )
{
	BEGIN_OP( a )
		BEGIN_BLOCK( F_VAR_LONG, b )
			DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "right_shift" ), VAI(a) >> VAI(b), FE_STATIC ); )
			DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "right_shift" ), VAI(a) >> (long)VAF(b), FE_STATIC ); )
			BLOCK_DIE( "right_shift" );
		END_BLOCK
		BEGIN_BLOCK( F_VAR_DOUBLE, b )
			DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "right_shift" ), (long)VAF(a) >> VAI(b), FE_STATIC ); )
			DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "right_shift" ), (long)VAF(a) >> (long)VAF(b), FE_STATIC ); )
			BLOCK_DIE( "right_shift" );
		END_BLOCK
	END_OP( "right_shift" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( binary_or ) */
FERITE_BINARY_OP( binary_or )
{
    BEGIN_OP( a )
      BEGIN_BLOCK( F_VAR_LONG, b )
        DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_or" ), VAI(a) | VAI(b), FE_STATIC ); )
          DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_or" ), VAI(a) | (long)VAF(b), FE_STATIC ); )
            BLOCK_DIE( "binary_or" );
    END_BLOCK
      BEGIN_BLOCK( F_VAR_DOUBLE, b )
        DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_or" ), (long)VAF(a) | VAI(b), FE_STATIC ); )
          DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_or" ), (long)VAF(a) | (long)VAF(b), FE_STATIC ); )
            BLOCK_DIE( "binary_or" );
    END_BLOCK
      END_OP( "binary_or" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( binary_and ) */
FERITE_BINARY_OP( binary_and )
{
    BEGIN_OP( a )
      BEGIN_BLOCK( F_VAR_LONG, b )
        DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_and" ), VAI(a) & VAI(b), FE_STATIC ); )
          DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_and" ), VAI(a) & (long)VAF(b), FE_STATIC ); )
            BLOCK_DIE( "binary_and" );
    END_BLOCK
      BEGIN_BLOCK( F_VAR_DOUBLE, b )
        DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_and" ), (long)VAF(a) & VAI(b), FE_STATIC ); )
          DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_and" ), (long)VAF(a) & (long)VAF(b), FE_STATIC ); )
            BLOCK_DIE( "binary_and" );
    END_BLOCK
      END_OP( "binary_and" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( binary_xor ) */
FERITE_BINARY_OP( binary_xor )
{
    BEGIN_OP( a )
      BEGIN_BLOCK( F_VAR_LONG, b )
        DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_xor" ), VAI(a) ^ VAI(b), FE_STATIC ); )
          DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_xor" ), VAI(a) ^ (long)VAF(b), FE_STATIC ); )
            BLOCK_DIE( "binary_xor" );
    END_BLOCK
      BEGIN_BLOCK( F_VAR_DOUBLE, b )
        DO_OP( F_VAR_LONG, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_xor" ), (long)VAF(a) ^ VAI(b), FE_STATIC ); )
          DO_OP( F_VAR_DOUBLE, NOWT, retv = ferite_create_number_long_variable( script, RETVNAME( "binary_xor" ), (long)VAF(a) ^ (long)VAF(b), FE_STATIC ); )
            BLOCK_DIE( "binary_xor" );
    END_BLOCK
      END_OP( "binary_xor" )
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( left_shift_assign ) */
FERITE_BINARY_OP( left_shift_assign )
{
    FeriteVariable *ptr = NULL, *tmp_ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    tmp_ptr = ferite_op_left_shift( script, current_op, a, b );
    if( tmp_ptr != NULL )
    {
        ptr = ferite_op_assign( script, current_op, a, tmp_ptr );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        ferite_variable_destroy( script, tmp_ptr );
    }
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( right_shift_assign ) */
FERITE_BINARY_OP( right_shift_assign )
{
    FeriteVariable *ptr = NULL, *tmp_ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    tmp_ptr = ferite_op_right_shift( script, current_op, a, b );
    if( tmp_ptr != NULL )
    {
        ptr = ferite_op_assign( script, current_op, a, tmp_ptr );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        ferite_variable_destroy( script, tmp_ptr );
    }
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( binary_and_assign ) */
FERITE_BINARY_OP( binary_and_assign )
{
    FeriteVariable *ptr = NULL, *tmp_ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    tmp_ptr = ferite_op_binary_and( script, current_op, a, b );
    if( tmp_ptr != NULL )
    {
        ptr = ferite_op_assign( script, current_op, a, tmp_ptr );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        ferite_variable_destroy( script, tmp_ptr );
    }
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( binary_or_assign ) */
FERITE_BINARY_OP( binary_or_assign )
{
    FeriteVariable *ptr = NULL, *tmp_ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    tmp_ptr = ferite_op_binary_or( script, current_op, a, b );
    if( tmp_ptr != NULL )
    {
        ptr = ferite_op_assign( script, current_op, a, tmp_ptr );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        ferite_variable_destroy( script, tmp_ptr );
    }
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( binary_xor_assign ) */
FERITE_BINARY_OP( binary_xor_assign )
{
    FeriteVariable *ptr = NULL, *tmp_ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    tmp_ptr = ferite_op_binary_xor( script, current_op, a, b );
    if( tmp_ptr != NULL )
    {
        ptr = ferite_op_assign( script, current_op, a, tmp_ptr );
        MARK_VARIABLE_AS_DISPOSABLE( ptr );
        ferite_variable_destroy( script, tmp_ptr );
    }
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( isa ) */
FERITE_BINARY_OP( isa )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;
    FE_VAR_TEST( (strcmp( VAS(b)->data, ferite_variable_id_to_str(script,F_VAR_TYPE(a)) ) == 0), "isa" );
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    if( ptr != NULL )
      MARK_VARIABLE_AS_DISPOSABLE( ptr );
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

/*{{{ FERITE_BINARY_OP( instanceof ) */
FERITE_BINARY_OP( instanceof )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
    LOCK_VARIABLE(a);
    LOCK_VARIABLE(b);
    GET_INPUT_VARS;
    if( F_VAR_TYPE(a) == F_VAR_OBJ && F_VAR_TYPE(b) == F_VAR_CLASS && VAO(a) != NULL && VAO(a)->klass == b->data.pval )
      FE_VAR_TRUE( ptr, "instanceof" );
    else
      FE_VAR_FALSE( ptr, "instanceof" );
    UNLOCK_VARIABLE(a);
    UNLOCK_VARIABLE(b);
    if( ptr != NULL )
      MARK_VARIABLE_AS_DISPOSABLE( ptr );
    FE_LEAVE_FUNCTION( ptr );
}
/*}}}*/

FeriteOpTable ferite_op_table[] =
{
    { FERITE_OPCODE_not_op,              "not_op",              FERITE_OP_CALL( not_op ) },
    { FERITE_OPCODE_left_incr,           "left_incr",           FERITE_OP_CALL( left_incr ) },
    { FERITE_OPCODE_right_incr,          "right_incr",          FERITE_OP_CALL( right_incr ) },
    { FERITE_OPCODE_left_decr,           "left_decr",           FERITE_OP_CALL( left_decr ) },
    { FERITE_OPCODE_right_decr,          "right_decr",          FERITE_OP_CALL( right_decr ) },
    { FERITE_OPCODE_positive_var,        "positive_var",        FERITE_OP_CALL( positive_var ) },
    { FERITE_OPCODE_negative_var,        "negative_var",        FERITE_OP_CALL( negative_var ) },
    { FERITE_OPCODE_eval,                "eval",                FERITE_OP_CALL( eval ) },
    { FERITE_OPCODE_include,             "include",             FERITE_OP_CALL( include ) },
    { FERITE_OPCODE_add,                 "add",                 FERITE_OP_CALL( add ) },
    { FERITE_OPCODE_minus,               "minus",               FERITE_OP_CALL( minus ) },
    { FERITE_OPCODE_mult,                "mult",                FERITE_OP_CALL( mult ) },
    { FERITE_OPCODE_divide,              "divide",              FERITE_OP_CALL( divide ) },
    { FERITE_OPCODE_modulus,             "modulus",             FERITE_OP_CALL( modulus ) },
    { FERITE_OPCODE_assign,              "assign",              FERITE_OP_CALL( assign ) },
    { FERITE_OPCODE_add_assign,          "add_assign",          FERITE_OP_CALL( add_assign ) },
    { FERITE_OPCODE_minus_assign,        "minus_assign",        FERITE_OP_CALL( minus_assign ) },
    { FERITE_OPCODE_mult_assign,         "mult_assign",         FERITE_OP_CALL( mult_assign ) },
    { FERITE_OPCODE_divide_assign,       "divide_assign",       FERITE_OP_CALL( divide_assign ) },
    { FERITE_OPCODE_binary_or,           "binary_or",           FERITE_OP_CALL( binary_or ) },
    { FERITE_OPCODE_binary_and,          "binary_and",          FERITE_OP_CALL( binary_and ) },
    { FERITE_OPCODE_binary_xor,          "binary_xor",          FERITE_OP_CALL( binary_xor ) },
    { FERITE_OPCODE_logical_or,          "logical_or",          FERITE_OP_CALL( logical_or ) },
    { FERITE_OPCODE_logical_and,         "logical_and",         FERITE_OP_CALL( logical_and ) },
    { FERITE_OPCODE_equals,              "equals",              FERITE_OP_CALL( equals ) },
    { FERITE_OPCODE_case,                "case",                FERITE_OP_CALL( case ) },
    { FERITE_OPCODE_notequals,           "notequals",           FERITE_OP_CALL( notequals ) },
    { FERITE_OPCODE_less_than,           "less_than",           FERITE_OP_CALL( less_than ) },
    { FERITE_OPCODE_less_than_equals,    "less_than_equals",    FERITE_OP_CALL( less_than_equals ) },
    { FERITE_OPCODE_greater_than,        "greater_than",        FERITE_OP_CALL( greater_than ) },
    { FERITE_OPCODE_greater_than_equals, "greater_than_equals", FERITE_OP_CALL( greater_than_equals ) },
    { FERITE_OPCODE_array_index,         "array_index",         FERITE_OP_CALL( array_index ) },
    { FERITE_OPCODE_array_slice,         "array_slice",         FERITE_OP_CALL( array_slice ) },
    { FERITE_OPCODE_left_shift,          "left_shift",          FERITE_OP_CALL( left_shift ) },
    { FERITE_OPCODE_right_shift,         "right_shift",         FERITE_OP_CALL( right_shift ) },
    { FERITE_OPCODE_left_shift_assign,   "left_shift_assign",   FERITE_OP_CALL( left_shift_assign ) },
    { FERITE_OPCODE_right_shift_assign,  "right_shift_assign",  FERITE_OP_CALL( right_shift_assign ) },
    { FERITE_OPCODE_binary_and_assign,   "binary_and_assign",   FERITE_OP_CALL( binary_and_assign ) },
    { FERITE_OPCODE_binary_or_assign,    "binary_or_assign",    FERITE_OP_CALL( binary_or_assign ) },
    { FERITE_OPCODE_binary_xor_assign,   "binary_xor_assign",   FERITE_OP_CALL( binary_xor_assign ) },
    { FERITE_OPCODE_isa,                 "isa",                 FERITE_OP_CALL( isa ) },
    { FERITE_OPCODE_instanceof,          "instanceof",          FERITE_OP_CALL( instanceof ) },
    { FERITE_OPCODE_array_clear,         "array_clear",         FERITE_OP_CALL( array_clear ) },
    { FERITE_OPCODE_array_index_assign,  "array_index_assign",  FERITE_OP_CALL( array_index_assign ) }
};
