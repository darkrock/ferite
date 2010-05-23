/*
 * Copyright (C) 2001-2007 Chris Ross, Stephan Engstrom, Alex Holden et al
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

#include "ferite.h"

typedef struct __ferite_sys_signal 
{
   FeriteScript *script;
   FeriteObject *object;
   FeriteFunction *function;
   int sig;
} FeriteSysSignal;

FeriteSysSignal *ferite_signal_create( FeriteScript *script, FeriteObject *object, FeriteFunction *function, int sig );
void ferite_signal_init();
void ferite_signal_deinit();
void ferite_signal_remove_script( FeriteScript *script );
int ferite_signal_register_handler( FeriteScript *script, FeriteObject *object, FeriteFunction *function, int s );
int ferite_signal_register_object_handler( FeriteScript *script, FeriteObject *object, char *name, int s );
int ferite_signal_register_function_handler( FeriteScript *script, char *name, int s );
int ferite_signal_unregister_handler( FeriteScript *script, int sig );
void ferite_signal_handler( int s );
