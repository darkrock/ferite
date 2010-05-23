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

#include "util_stream.h"

int stream_flush( FeriteScript *script, FeriteObject *self )
{
    FeriteVariable *str, *retval, **args;
    int written;

   /* Syscalls are expensive, CPU cycles are not ( atleast compared to Syscalls )
    * so we extract and build up one singel page of the buffer and then call __write__() */

    str = ferite_buffer_to_var( script, StreamObject->output_buffer );

    if( FE_STRLEN(str) == 0 )
    {
        ferite_variable_destroy( script, str );
        str = NULL;
    }

    if( str == NULL )
      return 0;

    args = ferite_create_parameter_list( script,4 );
    args[0] = str;
    MARK_VARIABLE_AS_DISPOSABLE(args[0]);

    args[1] = NULL;

    retval = ferite_call_function( script, self, NULL, StreamObject->write, args );
    written = VAI(retval);
    ferite_delete_parameter_list( script, args );

   /* No, we don't handle errors just yet, or partial writes for that matter */
    ferite_variable_destroy( script, retval );
    ferite_buffer_delete( script, StreamObject->output_buffer );
    StreamObject->output_buffer = ferite_buffer_new( script, 0 );
    return written;
}
FeriteVariable *system_create_pointer_var( FeriteScript *script, char *name, void *ptr )
{
    FeriteVariable *pointer;
    FeriteClass *cls;
    if((cls = ferite_find_class( script, script->mainns, "Stream.PointerObject" )) != NULL)
    {
        pointer = ferite_build_object( script, cls );
        VAO(pointer)->odata = ptr;
        ffree( VAO(pointer)->name );
        VAO(pointer)->name = fstrdup( name );
        FE_RETURN_VAR( pointer );
    }
    FE_RETURN_NULL_OBJECT;
}
FeriteVariable *system_create_stream_object( FeriteScript *script, char *stream_class, FILE *f )
{
    FeriteClass *cls;
    FeriteVariable *object, **args;
    if((cls = ferite_find_class( script, script->mainns, stream_class )) != NULL)
    {
        args = ferite_create_parameter_list( script,4 );
        args = ferite_add_to_parameter_list( args, system_create_pointer_var( script, "stdios", f ) );
        object = ferite_new_object( script, cls, args );
        ferite_delete_parameter_list( script, args );
        FE_RETURN_VAR( object );
    }
    FE_RETURN_NULL_OBJECT;
}
void stream_clear_input( struct Stream *s )
{
    s->input_buffer.data[0] = '\0';
    s->input_buffer.length = 0;
}
