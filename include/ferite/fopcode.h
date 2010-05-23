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

#ifndef __FERITE_OPCODE_H__
# define __FERITE_OPCODE_H__

/* the instructions we have */
 
# define F_OP_NOP             0     /* no op */
# define F_OP_BINARY          1     /* call binary opcode->op with opcode->opdata_one & two as data items */
# define F_OP_UNARY           2     /* call unary operator opcode->op with op->opdata_one */
# define F_OP_FUNCTION        3     /* call a function (char *)opcode->opdata_one as the
                                   * object, (char *)opcode->opdata_two as the function
                                   * job of called function to pull items off stack
                                   * and then push its result onto the stack */
# define F_OP_METHOD          4
# define F_OP_NEWOBJ          5
# define F_OP_JMP             6     /* just jump */
# define F_OP_EXIT            7     /* used as a return type thing =P ie to signal end of execution */
# define F_OP_PUSH            8
# define F_OP_PUSHVAR         9     /* push variable onto stack */
# define F_OP_PUSHINDEX       10    /* push a function variable onto the stack */
# define F_OP_PUSHATTR        11    /* push an object attribute onto the stack */
# define F_OP_POP             12    /* pop variable from stack -> ie to get rid of shit pushed onto the stack */
# define F_OP_BIE             13
# define F_OP_BNE             14    /* jump if the staus flag marks the old false, else ignore and continue */
# define F_OP_CLSRE_ASSGN     15
# define F_OP_ERR             16
# define F_OP_MANY            17
# define F_OP_CASE            18
# define F_OP_ARGS            19
# define F_OP_DELIVER         20
# define F_OP_SET_DELIVER     21
# define F_OP_GET_DELIVER     22
# define F_OP_SWAP_TOP        23
# define F_OP_VRST            24
# define F_OP_RAISE           25
# define F_OP_PUSHGLOBAL      26
# define F_OP_NOPTOP          27

# define OP_MANY( p, o, d) \
   p->OP_TYPE = F_OP_MANY; \
   p->addr = o; \
   p->opdata = d;

# define OP_BINARY( ptr, opf, data1 ) \
   ptr->OP_TYPE = F_OP_BINARY;       \
   ptr->addr = opf;                  \
   ptr->opdata = data1;

# define OP_UNARY( ptr, opf, data1 )  \
   ptr->OP_TYPE = F_OP_UNARY;        \
   ptr->addr = opf;                  \
   ptr->opdata = data1;

# define OP_PUSH( ptr, data1 )        \
   ptr->OP_TYPE = F_OP_PUSH;         \
   ptr->opdata = data1;

# define OP_INDEX( ptr, data1 )   \
   ptr->OP_TYPE = F_OP_PUSHINDEX;    \
   ptr->addr = data1;

void ferite_delete_opcode_list( FeriteScript *script, FeriteOpcodeList *oplist );
FeriteOpcodeList *ferite_create_opcode_list( int size );
void ferite_oplist_grow( FeriteOpcodeList *oplist );
FeriteOp *ferite_get_next_op( FeriteOpcodeList *list );
FeriteOp *ferite_create_op(void);
FeriteOp *ferite_current_op( FeriteOpcodeList *oplist );
FeriteOp *ferite_get_next_op_address( FeriteOpcodeList *oplist );
int       ferite_get_next_op_loc( FeriteOpcodeList *oplist ); /* same as ferite_get_next_op_address
                                                               * but will return an index rather than
                                                               * address :)
                                                               */
void ferite_opcode_dump( FeriteOpcodeList *oplist );
FeriteOpcodeList *ferite_opcode_dup( FeriteScript *script, FeriteOpcodeList *oplist );

#endif /* __FERITE_OPCODE_H__ */
