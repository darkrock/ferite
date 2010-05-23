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

#ifndef __FERITE_OPS__
#define __FERITE_OPS__

#define FERITE_MANY_OP( opname ) FERITE_API FeriteVariable *ferite_op_##opname (FeriteScript *script, FeriteOp *current_op, FeriteVariable **vars, int count )
#define FERITE_BINARY_OP( opname ) FERITE_API FeriteVariable *ferite_op_##opname (FeriteScript *script, FeriteOp *current_op, FeriteVariable *a, FeriteVariable *b)
#define FERITE_UNARY_OP( opname ) FERITE_API FeriteVariable *ferite_op_##opname (FeriteScript *script, FeriteOp *current_op, FeriteVariable *a)
#define FERITE_OP_CALL( opname ) (void *)ferite_op_##opname

FERITE_UNARY_OP( not_op );
FERITE_UNARY_OP( left_incr );
FERITE_UNARY_OP( right_incr );
FERITE_UNARY_OP( left_decr );
FERITE_UNARY_OP( right_decr );
FERITE_UNARY_OP( positive_var );
FERITE_UNARY_OP( negative_var );
FERITE_UNARY_OP( eval );
FERITE_UNARY_OP( include );

FERITE_BINARY_OP( add );
FERITE_BINARY_OP( minus );
FERITE_BINARY_OP( mult );
FERITE_BINARY_OP( divide );
FERITE_BINARY_OP( modulus );
FERITE_BINARY_OP( assign );
FERITE_BINARY_OP( add_assign );
FERITE_BINARY_OP( minus_assign );
FERITE_BINARY_OP( mult_assign );
FERITE_BINARY_OP( divide_assign );
FERITE_BINARY_OP( binary_or );
FERITE_BINARY_OP( binary_and );
FERITE_BINARY_OP( binary_xor );
FERITE_BINARY_OP( logical_or );
FERITE_BINARY_OP( logical_and );

FERITE_BINARY_OP( equals );
FERITE_BINARY_OP( case );
FERITE_BINARY_OP( notequals );
FERITE_BINARY_OP( less_than );
FERITE_BINARY_OP( less_than_equals );
FERITE_BINARY_OP( greater_than );
FERITE_BINARY_OP( greater_than_equals );

FERITE_BINARY_OP( array_index );
FERITE_MANY_OP( array_slice );
FERITE_UNARY_OP( array_clear );
FERITE_MANY_OP( array_index_assign );

FERITE_BINARY_OP( left_shift );
FERITE_BINARY_OP( right_shift );
FERITE_BINARY_OP( left_shift_assign );
FERITE_BINARY_OP( right_shift_assign );
FERITE_BINARY_OP( binary_and_assign );
FERITE_BINARY_OP( binary_or_assign );
FERITE_BINARY_OP( binary_xor_assign );

FERITE_BINARY_OP( isa );
FERITE_BINARY_OP( instanceof );

#define FERITE_OPCODE_not_op                0
#define FERITE_OPCODE_left_incr             1
#define FERITE_OPCODE_right_incr            2
#define FERITE_OPCODE_left_decr             3
#define FERITE_OPCODE_right_decr            4
#define FERITE_OPCODE_positive_var          5
#define FERITE_OPCODE_negative_var          6
#define FERITE_OPCODE_eval                  7
#define FERITE_OPCODE_include               8
#define FERITE_OPCODE_add                   9
#define FERITE_OPCODE_minus                10
#define FERITE_OPCODE_mult                 11
#define FERITE_OPCODE_divide               12  
#define FERITE_OPCODE_modulus              13
#define FERITE_OPCODE_assign               14
#define FERITE_OPCODE_add_assign           15
#define FERITE_OPCODE_minus_assign         16
#define FERITE_OPCODE_mult_assign          17
#define FERITE_OPCODE_divide_assign        18
#define FERITE_OPCODE_binary_or            19
#define FERITE_OPCODE_binary_and           20
#define FERITE_OPCODE_binary_xor           21
#define FERITE_OPCODE_logical_or           22
#define FERITE_OPCODE_logical_and          23
#define FERITE_OPCODE_equals               24
#define FERITE_OPCODE_case                 25 
#define FERITE_OPCODE_notequals            26
#define FERITE_OPCODE_less_than            27
#define FERITE_OPCODE_less_than_equals     28
#define FERITE_OPCODE_greater_than         29
#define FERITE_OPCODE_greater_than_equals  30
#define FERITE_OPCODE_array_index          31
#define FERITE_OPCODE_array_slice          32
#define FERITE_OPCODE_left_shift           33
#define FERITE_OPCODE_right_shift          34
#define FERITE_OPCODE_left_shift_assign    35
#define FERITE_OPCODE_right_shift_assign   36
#define FERITE_OPCODE_binary_and_assign    37
#define FERITE_OPCODE_binary_or_assign     38
#define FERITE_OPCODE_binary_xor_assign    39
#define FERITE_OPCODE_isa                  40
#define FERITE_OPCODE_instanceof           41
#define FERITE_OPCODE_array_clear          42
#define FERITE_OPCODE_array_index_assign   43

#endif /* __FERITE_OPS__ */
