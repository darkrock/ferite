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

#ifndef __FERITE_THREAD_H__
#define __FERITE_THREAD_H__

#ifdef THREAD_SAFE
# define FE_LOCK_SCRIPT( scr ) aphex_mutex_lock( scr->lock );
# define FE_UNLOCK_SCRIPT( scr ) aphex_mutex_unlock( scr->lock );
# define LOCK_VARIABLE( var ) if( var->lock != NULL ) aphex_mutex_lock( var->lock )
# define UNLOCK_VARIABLE( var ) if( var->lock != NULL ) aphex_mutex_unlock( var->lock )
#else
# define FE_LOCK_SCRIPT( scr ) 
# define FE_UNLOCK_SCRIPT( scr ) 
# define LOCK_VARIABLE( x )
# define UNLOCK_VARIABLE( x )
#endif

FERITE_API FeriteThreadGroup *ferite_create_thread_group( FeriteScript *script );
FERITE_API void ferite_thread_group_destroy( FeriteScript *script, FeriteThreadGroup *group );
FERITE_API void ferite_thread_group_attach( FeriteScript *script, FeriteThreadGroup *group, FeriteThread *data );
FERITE_API void ferite_thread_group_dettach( FeriteScript *script, FeriteThreadGroup *group, FeriteThread *data );
FERITE_API void ferite_thread_group_wait( FeriteScript *script, FeriteThreadGroup *group );

#endif
