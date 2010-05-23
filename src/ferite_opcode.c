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

FeriteOp *ferite_create_op()
{
    FeriteOp *ptr;

    FE_ENTER_FUNCTION;
    ptr = fmalloc_ngc( sizeof( FeriteOp ) );
    ptr->OP_TYPE = F_OP_NOP;
    ptr->opdata = NULL;
    ptr->opdataf = NULL;
    ptr->line = 0;
    ptr->addr = 0;
	ptr->flags = 0;
    FE_LEAVE_FUNCTION( ptr );
}

FeriteOpcodeList *ferite_create_opcode_list( int size )
{
    FeriteOpcodeList *ptr;

    FE_ENTER_FUNCTION;
    ptr = fmalloc_ngc( sizeof( FeriteOpcodeList ) );
    ptr->size = size;
    ptr->filename = NULL;
    ptr->current_op_loc = -1;
    ptr->list = fmalloc_ngc( sizeof( FeriteOp * ) * size );
	memset( ptr->list, 0, sizeof(FeriteOp*) * size );
    ptr->list[0] = NULL;
    FE_LEAVE_FUNCTION( ptr );
}

/*
 * This function will grow the oplist if it needs to be grown.
 */
void ferite_oplist_grow( FeriteOpcodeList *oplist )
{
    FE_ENTER_FUNCTION;
    if( !((oplist->current_op_loc+2) < oplist->size) )
    {
	/* we have run out of space
	 * - double oplist->size
	 * - realloc */
		size_t new_list_size = sizeof(FeriteOp *) * oplist->size * 2;
		FeriteOp **new_list = fmalloc_ngc( new_list_size );
		
		memset( new_list, 0, new_list_size );
		memcpy( new_list, oplist->list, oplist->size * sizeof(FeriteOp*) );
		oplist->size *= 2;
		ffree_ngc( oplist->list );
		oplist->list = new_list;
    }
    FE_LEAVE_FUNCTION( NOWT );
}

FeriteOp *ferite_get_next_op( FeriteOpcodeList *oplist )
{
    FE_ENTER_FUNCTION;
    /* We need room for the current op and the next op to be NULL */
    ferite_oplist_grow( oplist );
    
    oplist->current_op_loc++;
    if( oplist->list[oplist->current_op_loc] == NULL )
      oplist->list[oplist->current_op_loc] = ferite_create_op();
/*    oplist->current_op = oplist->list[oplist->current_op_loc];*/
    if(oplist->current_op_loc < oplist->size - 1)
        oplist->list[oplist->current_op_loc + 1] = NULL;
    FE_LEAVE_FUNCTION( oplist->list[oplist->current_op_loc] );
}

FeriteOp *ferite_current_op( FeriteOpcodeList *oplist )
{
    FE_ENTER_FUNCTION;
    FE_LEAVE_FUNCTION( oplist->list[oplist->current_op_loc] );
}

/* returns the address of the next operator with out incrmenting the internal program counter */
FeriteOp *ferite_get_next_op_address( FeriteOpcodeList *oplist )
{
    FE_ENTER_FUNCTION;
    /* We need room for the current op and the next op to be NULL */
    ferite_oplist_grow( oplist );

    if( oplist->list[oplist->current_op_loc+1] == NULL )
      oplist->list[oplist->current_op_loc+1] = ferite_create_op();
    oplist->list[oplist->current_op_loc + 2] = NULL;
    FE_LEAVE_FUNCTION( oplist->list[oplist->current_op_loc+1] );
}

/* returns the index of the next operator with out incrmenting the internal program counter */
int ferite_get_next_op_loc( FeriteOpcodeList *oplist )
{
    FE_ENTER_FUNCTION;
    /* We need room for the current op and the next op to be NULL */
    ferite_oplist_grow( oplist );

    if( oplist->list[oplist->current_op_loc+1] == NULL )
      oplist->list[oplist->current_op_loc+1] = ferite_create_op();
    oplist->list[oplist->current_op_loc + 2] = NULL;
    FE_LEAVE_FUNCTION( oplist->current_op_loc+1 );
}

void ferite_delete_opcode_list( FeriteScript *script, FeriteOpcodeList *oplist )
{
    int i = 0, currentVar = 0, canFree = 0;
    FeriteVariable *ptr;
    void **freed_variables = fcalloc( sizeof(void *) * oplist->size, sizeof(char) );

    /* we only free instructions and compiled in constants - eg strings and stuff */
    FE_ENTER_FUNCTION;
    if( oplist->filename != NULL )
      ffree( oplist->filename );

    for( i = 0; i <= oplist->current_op_loc; i++ )
    {
        FUD(("Freeing instruction: %d\n", i ));
		if( oplist->list[i] ) {
	        switch( oplist->list[i]->OP_TYPE )
	        {
	          case F_OP_PUSH:
	            ptr = (FeriteVariable *)(oplist->list[i]->opdata);
	            if( ptr != NULL )
	            {
	                currentVar = 0;
	                canFree = 1;
	                while( freed_variables[currentVar] != NULL )
	                {
	                    if( ptr == freed_variables[currentVar] )
	                    {
	                        canFree = 0;
	                        break;
	                    }
	                    currentVar++;
	                }
	                if( FE_VAR_IS_COMPILED( ptr ) && canFree )
	                {
	                    ferite_variable_destroy( script, ptr );
	                    freed_variables[currentVar] = ptr;
	                }
	            }
	            ffree( oplist->list[i] );
	            break;
	          case F_OP_PUSHINDEX:
	          case F_OP_JMP:
	          case F_OP_BNE:
	          case F_OP_BIE:
	          case F_OP_ERR:
	          case F_OP_POP:
	          case F_OP_NOP:
	          case F_OP_EXIT:
	          case F_OP_UNARY:
	          case F_OP_BINARY:
	          case F_OP_NEWOBJ:
	            if( oplist->list[i]->opdataf != NULL )
	              ffree( oplist->list[i]->opdataf );
	            ffree( oplist->list[i] );
	            break;
	          case F_OP_FUNCTION:
	          case F_OP_METHOD:
	          case F_OP_PUSHVAR:
	          case F_OP_PUSHATTR:
			  case F_OP_DELIVER:
			  case F_OP_CLSRE_ASSGN:
	            if( oplist->list[i]->opdataf != NULL )
	              ffree( oplist->list[i]->opdataf );
				if( oplist->list[i]->opdata != NULL )
		            ffree( oplist->list[i]->opdata );
	            ffree( oplist->list[i] );
	            break;
			  case F_OP_PUSHGLOBAL:
	          case F_OP_MANY:
				if( oplist->list[i]->opdata != NULL )
		            ffree( oplist->list[i]->opdata );
	            ffree( oplist->list[i] );
	            break;
	          default:
	            ffree( oplist->list[i] );
	        }
		}
    }
    ffree( oplist->list );
    ffree( oplist );
    ffree( freed_variables );
    FE_LEAVE_FUNCTION( NOWT );
}

extern FeriteOpTable ferite_op_table[];

void ferite_opcode_dump( FeriteOpcodeList *oplist )
{
    int i = 0;

    FE_ENTER_FUNCTION;
    printf( "Current Op Location: %ld\n", oplist->current_op_loc );
    printf( "Offset\t Address\n" );
    for( i = 0; i <= oplist->current_op_loc; i++ )
    {
        switch( oplist->list[i]->OP_TYPE )
        {
		  case -1: {
			
		  }
          case F_OP_PUSH: {
				FeriteString *str = ferite_variable_to_str( NULL, oplist->list[i]->opdata, FE_TRUE );
	            printf( "[%d]\t [%p] PUSH         %s\n", i, (void *)(oplist->list[i]), str->data );
				ferite_str_destroy( NULL, str );
			}
            break;
          case F_OP_POP:
            printf( "[%d]\t [%p] POP\n", i, (void *)(oplist->list[i]) );
            break;
          case F_OP_NOP:
            printf( "[%d]\t [%p] NOP\n", i, (void *)(oplist->list[i]) );
            break;
          case F_OP_UNARY:
            printf( "[%d]\t [%p] UNARYOP      %s\n", i, (void *)(oplist->list[i]), ferite_op_table[oplist->list[i]->addr].name );
            break;
          case F_OP_BINARY:
            printf( "[%d]\t [%p] BINARYOP     %s\n", i, (void *)(oplist->list[i]), ferite_op_table[oplist->list[i]->addr].name );
            break;
          case F_OP_MANY:
            printf( "[%d]\t [%p] MANYOP       %s\n", i, (void *)(oplist->list[i]), ferite_op_table[oplist->list[i]->addr].name );
            break;
          case F_OP_FUNCTION:
            printf( "[%d]\t [%p] FUNCTION     %s\n", i, (void *)(oplist->list[i]), (char *)oplist->list[i]->opdata );
            break;
          case F_OP_METHOD:
            printf( "[%d]\t [%p] METHOD       %s\n", i, (void *)(oplist->list[i]), (char *)oplist->list[i]->opdata );
            break;
          case F_OP_JMP:
            printf( "[%d]\t [%p] JMP          %ld\n", i, (void *)(oplist->list[i]), oplist->list[i]->addr );
            break;
          case F_OP_BNE:
            printf( "[%d]\t [%p] BNE          %ld\n", i, (void *)(oplist->list[i]), oplist->list[i]->addr );
            break;
          case F_OP_BIE:
            printf( "[%d]\t [%p] BIE          %ld\n", i, (void *)(oplist->list[i]), oplist->list[i]->addr );
            break;
          case F_OP_EXIT:
            printf( "[%d]\t [%p] EXIT\n", i, (void *)(oplist->list[i]) );
            break;
          case F_OP_NEWOBJ:
            printf( "[%d]\t [%p] NEWOBJ\n", i, (void *)(oplist->list[i]) );
            break;
          case F_OP_PUSHATTR:
            printf( "[%d]\t [%p] PUSHATTR     %s(%p)\n", i, (void *)(oplist->list[i]), (char *)oplist->list[i]->opdata, oplist->list[i]->opdata );
            break;
          case F_OP_PUSHVAR:
            printf( "[%d]\t [%p] PUSHVAR      %s(%p)\n", i, (void *)(oplist->list[i]), (char *)oplist->list[i]->opdata, oplist->list[i]->opdata );
            break;
          case F_OP_PUSHGLOBAL:
            printf( "[%d]\t [%p] PUSHGLOBAL   %s:%ld(%p)\n", i, (void *)(oplist->list[i]), (char *)oplist->list[i]->opdata, oplist->list[i]->addr, oplist->list[i]->opdata );
            break;
          case F_OP_PUSHINDEX:
            printf( "[%d]\t [%p] PUSHINDEX    %ld\n", i, (void *)(oplist->list[i]), oplist->list[i]->addr );
            break;
          case F_OP_ERR:
            printf( "[%d]\t [%p] ERR          %ld (%s)\n", i, (void *)(oplist->list[i]), oplist->list[i]->addr, (oplist->list[i]->addr == -1 ? "reset" : "set") );
            break;
          case F_OP_CASE:
            printf( "[%d]\t [%p] CASE\n", i, (void *)(oplist->list[i]) );
            break;
          case F_OP_ARGS:
            printf( "[%d]\t [%p] ARGS\n", i, (void *)(oplist->list[i]) );
            break;
          case F_OP_DELIVER:
            printf( "[%d]\t [%p] DELIVER\n", i, (void *)(oplist->list[i]) );
            break;
          case F_OP_SET_DELIVER:
            printf( "[%d]\t [%p] SET DELIVER (using) : %s\n", i, (void *)(oplist->list[i]), (oplist->list[i]->addr ? "Current Yield Block" : "From Object"));
            break;
          case F_OP_GET_DELIVER:
            printf( "[%d]\t [%p] GET DELIVER (recipient)\n", i, (void *)(oplist->list[i]) );
            break;
          case F_OP_SWAP_TOP:
            printf( "[%d]\t [%p] SWAPTOP      %ld <-> %ld\n", i, (void *)(oplist->list[i]), (oplist->list[i]->addr), (oplist->list[i]->addr) + 1 );
            break;
          case F_OP_VRST:
            printf( "[%d]\t [%p] VRST\n", i, (void *)(oplist->list[i]) );
            break;
		case F_OP_RAISE:
			printf( "[%d]\t [%p] RAISE\n", i, (void *)(oplist->list[i]) );
			break;
          case F_OP_NOPTOP:
            printf( "[%d]\t [%p] NOPTOP\n", i, (void *)(oplist->list[i]) );
            break;
		  default:
            printf( "[%d]\t [%p] UKNOWNOP(%d)\n", i, (void *)(oplist->list[i]), oplist->list[i]->OP_TYPE );
        }
    }
    FE_LEAVE_FUNCTION( NOWT );
}

FeriteOpcodeList *ferite_opcode_dup( FeriteScript *script, FeriteOpcodeList *oplist )
{
    int i = 0, *value = NULL, v;
    FeriteVariable *var = NULL;
    FeriteOpcodeList *ptr = NULL;

    FE_ENTER_FUNCTION;

    if( oplist != NULL )
    {
        ptr = fmalloc( sizeof( FeriteOpcodeList ) );
        ptr->size = oplist->size;
        if( oplist->filename != NULL )
          ptr->filename = fstrdup( oplist->filename );
        ptr->current_op_loc = oplist->current_op_loc;
        ptr->list = fcalloc( sizeof( FeriteOp * ) * ptr->size, sizeof(FeriteOp *) );

        for( i = 0; i <= oplist->current_op_loc; i++ )
        {
            ptr->list[i] = fmalloc( sizeof( FeriteOp ) );
            ptr->list[i]->OP_TYPE = oplist->list[i]->OP_TYPE;
            ptr->list[i]->line = oplist->list[i]->line;
            ptr->list[i]->addr = oplist->list[i]->addr;
            if( oplist->list[i]->opdataf != NULL )
            {
                ptr->list[i]->opdataf = fmalloc( sizeof( FeriteOpFncData ) );
                ptr->list[i]->opdataf->argument_count = oplist->list[i]->opdataf->argument_count;
            }
	   else
	     ptr->list[i]->opdataf = NULL;

            switch( oplist->list[i]->OP_TYPE )
            {
              case F_OP_PUSH:
                var = (FeriteVariable *)(oplist->list[i]->opdata);
                if( var != NULL )
                {
                    ptr->list[i]->opdata = ferite_duplicate_variable( script, var, NULL );
                    if( FE_VAR_IS_COMPILED( var ) )
                      MARK_VARIABLE_AS_COMPILED( ((FeriteVariable*)ptr->list[i]->opdata) );
                }
                break;
              case F_OP_FUNCTION:
              case F_OP_METHOD:
              case F_OP_PUSHVAR:
                ptr->list[i]->opdata = fstrdup( oplist->list[i]->opdata );
                break;
              case F_OP_MANY:
                value = (int *)(oplist->list[i]->opdata);
                v = *value;
                value = fmalloc( sizeof(int) );
                *value = v;
                ptr->list[i]->opdata = value;
                break;
            }
        }
    }
    FE_LEAVE_FUNCTION(ptr);
}
