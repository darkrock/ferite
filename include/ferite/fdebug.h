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

#ifndef __FERITE_DEBUG_H__
# define __FERITE_DEBUG_H__

# ifdef DEBUG
#  ifndef _DEBUG
#    define _DEBUG
#  endif
/* provides slower but more secure memory tracking */
#  define FERITE_MEM_DEBUG
#  define FUD( var ) if( ferite_show_debug == 1 ) printf var ;
#  define FE_ENTER_FUNCTION \
    int ferite_depth = ferite_call_level; \
    FUD(("|%s--> %s [line %d] Entering %s()\n", ferite_stroflen(' ', (ferite_call_level*2)), __FILE__, __LINE__, __FUNCTION__)); \
    ferite_call_level++;

#  define FE_ENTER_NAMED_FUNCTION( NAME ) \
    int ferite_depth = ferite_call_level; \
    FUD(("|%s--> %s [line %d] Entering %s()\n", ferite_stroflen(' ', (ferite_call_level*2)), __FILE__, __LINE__, NAME)); \
    ferite_call_level++;

#  define FE_LEAVE_FUNCTION( blim ) { ferite_call_level--; \
    FUD(("|%s<-- %s [line %d] Leaving  %s()\n", ferite_stroflen(' ', (ferite_call_level*2)), __FILE__, __LINE__, __FUNCTION__)); \
    if( ferite_call_level != ferite_depth ) { \
        FUD(( "Depth Error: (file %s, line %d, function %s, depth=%d, expected %d)\n", __FILE__, __LINE__, __FUNCTION__, ferite_call_level, ferite_depth ));  } \
    return blim; }

#  define FE_LEAVE_NAMED_FUNCTION( NAME ) { ferite_call_level--; \
    FUD(("|%s<-- %s [line %d] Leaving  %s()\n", ferite_stroflen(' ', (ferite_call_level*2)), __FILE__, __LINE__, NAME)); \
    if( ferite_call_level != ferite_depth ) { \
        FUD(( "Depth Error: (file %s, line %d, function %s, depth=%d, expected %d)\n", __FILE__, __LINE__, NAME, ferite_call_level, ferite_depth )); } }

# else
#  define FUD( var )
#  define FE_ENTER_NAMED_FUNCTION( NAME )
#  define FE_LEAVE_NAMED_FUNCTION( NAME )
#  define FE_ENTER_FUNCTION
#  define FE_LEAVE_FUNCTION( blim ) return blim;
# endif

# define NOWT

# define FE_ASSERT( test ) \
      if( !(test) ) { \
          ferite_assert( "Assertion failed on line %d, %s: %s\n", __LINE__, __FILE__, #test ); \
      }

extern int call_level;
void ferite_debug_catch( void *p, int count );

#endif /* __FERITE_DEBUG_H__ */
