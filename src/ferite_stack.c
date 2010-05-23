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

/**
 * @group Stacks
 * @description Stacks are another useful data structure. Here is a stright forward implementation.
 * @warning It must be noted that if you wish to tuck into the structure of the stack, elements are placed
            within a 'C' array of pointers and the first element is at [1] *not* [0] as you would expect.
            The stack_ptr variable points to the current head of the stack, for instance if stack_ptr is
            equal to 1, then [1] has a value in it.
 */

/**
 * @function ferite_create_stack
 * @declaration FeriteStack *ferite_create_stack( FeriteScript *script, int size )
 * @brief Create a stack
 * @param FeriteScript *script The script
 * @param int size The initial size of the stack
 * @return A new stack structure
 * @description It is important to note that the stacks contents go from 1 to stack->stack_ptr
 */
FeriteStack *ferite_create_stack( FeriteScript *script, int size )
{
    FeriteStack *ptr = NULL;
    int i = 0;

    FE_ENTER_FUNCTION;
    if( script && script->stacks->stack_ptr )
		ptr = ferite_stack_pop( script, script->stacks );
    else
		ptr = fmalloc( sizeof( FeriteStack ) );
		
    ptr->size = size;
    ptr->stack_ptr = 0;
    ptr->stack = fmalloc( sizeof( void * ) * size );
    for( i = 0; i < size; i++ ) /* clear the stack. make life easy */
		ptr->stack[i] = NULL;
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_stack_push 
 * @declaration void ferite_stack_push( FeriteStack *stck, void *ptr )
 * @brief Push a value onto the stack
 * @param FeriteStack *stck The stack to push the value onto
 * @param void *ptr The pointer to push onto the stack
 */
void ferite_stack_push( FeriteScript *script, FeriteStack *stck, void *ptr )
{
    int i;

    FE_ENTER_FUNCTION;
    stck->stack_ptr++;
    if( stck->stack_ptr < stck->size )
        stck->stack[stck->stack_ptr] = ptr;
    else
    {
        FUD(("Resizing STACK\n"));
        /* stack aint big enough. make it bigger. */
        stck->size *= 2;
        stck->stack = frealloc( stck->stack, sizeof( void * ) * stck->size );
        for( i = stck->stack_ptr; i < stck->size; i++ )
          stck->stack[i] = NULL;
        stck->stack[stck->stack_ptr] = ptr;
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_stack_pop
 * @declaration void *ferite_stack_pop( FeriteStack *stck )
 * @brief Pop a value off the top of the stack and return it
 * @param FeriteStack *stck The stack to pop the value off
 * @return The popped off value
 */
void *ferite_stack_pop( FeriteScript *script, FeriteStack *stck )
{
    void *ptr = NULL;

    FE_ENTER_FUNCTION;
    if( stck->stack_ptr >= 1 )
    {
        ptr = stck->stack[stck->stack_ptr];
        stck->stack[stck->stack_ptr] = NULL;
        stck->stack_ptr--;
        if( stck->stack_ptr < 0 )
            stck->stack_ptr = 0;
    }
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_delete_stack
 * @declaration void ferite_delete_stack( FeriteScript *script, FeriteStack *stack )
 * @brief Delete a stack and free up the memory it uses
 * @param FeriteScript *script The script
 * @param FeriteStack *stack The stack to delete
 */
void ferite_delete_stack( FeriteScript *script, FeriteStack *stack )
{
    FE_ENTER_FUNCTION;
    if( stack )
    {
        ffree( stack->stack ); /* we dont need to free the stack items, it's the job of the push
                                * pullers to do that :) */
        if( script && script->stacks->stack_ptr < script->stacks->size - 1 )
            ferite_stack_push( script, script->stacks, stack );
        else
            ffree( stack );
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_stack_top
 * @declaration void *ferite_stack_top( FeriteStack *stck )
 * @brief Return the top item off the stack without changing the stack's state
 * @param FeriteStack *stck The stack to get the item off
 * @return The value
 */
void *ferite_stack_top( FeriteScript *script, FeriteStack *stck )
{
    void *ptr = NULL;

    FE_ENTER_FUNCTION;
    ptr = stck->stack[stck->stack_ptr];
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_duplicate_stack_contents
 * @declaration void **ferite_duplicate_stack_contents( FeriteScript *script, FeriteStack *stack, void *(*ddup)( FeriteScript*,void *data,void *data2 ), void *data2 )
 * @brief Duplicate the contents of a stack
 * @param FeriteScript *script The script
 * @param FeriteStack *stack The stack whose contents should be duplicated
 * @param void *ddup The function that gets called to copy the data
 * @return An exact copy of the stacks' contents
 */
void **ferite_duplicate_stack_contents( FeriteScript *script, FeriteStack *stack, void *(*ddup)( FeriteScript*,void *data,void *data2 ), void *data2 )
{
    void **arr = fmalloc( sizeof( void * ) * (stack->size + 1) );
    int i = 0;
    
    for( i = 0; i <= stack->stack_ptr; i++ )
    {
        arr[i] = NULL;
        if( stack->stack[i] != NULL )
          arr[i] = (ddup)( script, stack->stack[i], data2 );
    }
    arr[stack->stack_ptr+1] = NULL;
    return arr;
}

/**
* @function ferite_duplicate_stack
 * @declaration FeriteStack *ferite_duplicate_stack( FeriteScript *script, FeriteStack *stack, void *(*ddup)( FeriteScript*,void *data,void *data2 ), void *data2 )
 * @brief Duplicate a stack
 * @param FeriteScript *script The script
 * @param FeriteStack *stack The stack that should be duplicated
 * @param void *ddup The function that gets called to copy the data within the stack
 * @return An exact copy of the stacks and it's contents
 */
FeriteStack *ferite_duplicate_stack( FeriteScript *script, FeriteStack *stack, void *(*ddup)( FeriteScript*,void *data,void *data2 ), void *data2 )
{
    FeriteStack *value = fmalloc( sizeof(FeriteStack) );
    
    value->size = stack->size;
    value->stack_ptr = stack->stack_ptr;
    value->stack = ferite_duplicate_stack_contents( script, stack, ddup, data2 );
    return value;
}

/**
 * @end
 */
