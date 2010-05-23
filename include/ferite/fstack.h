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

#ifndef __FERITE_STACK_H__
#define __FERITE_STACK_H__

FERITE_API FeriteStack *ferite_create_stack( FeriteScript *script, int size );
FERITE_API void         ferite_stack_push( FeriteScript *script, FeriteStack *stck, void *ptr );
FERITE_API void        *ferite_stack_pop( FeriteScript *script, FeriteStack *stck );
FERITE_API void         ferite_delete_stack(  FeriteScript *script, FeriteStack *stack );
FERITE_API void        *ferite_stack_top( FeriteScript *script, FeriteStack *stck );
FERITE_API void **ferite_duplicate_stack_contents( FeriteScript *script, FeriteStack *stack, void *(*ddup)( FeriteScript*,void *data,void *data2 ), void *data2 );
FERITE_API FeriteStack *ferite_duplicate_stack( FeriteScript *script, FeriteStack *stack, void *(*ddup)( FeriteScript*,void *data,void *data2 ), void *data2 );
  
#endif /* __FERITE_STACK_H__ */
