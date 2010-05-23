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

#include "util_posix.h"
#include "../stream/util_stream.h"

FeriteVariable *system_call_stat( FeriteScript *script, struct stat *in )
{
    FeriteVariable *object, *pointer, **args;
    FeriteClass *cls;

    pointer = system_create_pointer_var( script, "struct::stat", in );
    if((cls = ferite_find_class( script, script->mainns, "Posix.Stat" )) != NULL)
    {
        args = ferite_create_parameter_list_from_data( script, "o", VAO(pointer) );
        object = ferite_new_object( script, cls, args );
        ferite_delete_parameter_list( script, args );
        FE_RETURN_VAR( object );
    }
    FE_RETURN_NULL_OBJECT;
}

int make_fd_set(FeriteScript *script, FeriteUnifiedArray *array, fd_set *f)
{
    int i;
    int cnt = 0;
    FeriteVariable *fv = NULL;
    FeriteFunction *func = NULL;

    FD_ZERO(f);

    for(i = 0; i < array->size; i++)
    {
        fv = ferite_uarray_get_index(script, array, i);
        if(F_VAR_TYPE(fv) != F_VAR_OBJ || VAO(fv) == NULL)
        {
            ferite_set_error(script, 0, "Wrong variable type in array");
            return -1;
        }
        func = ferite_object_get_function_for_params(script, VAO(fv), "getDescriptor",NULL);
        if(!func)
        {
            ferite_set_error(script, 0, "Object in array doesn't have getDescriptor()");
            return -1;
        }
        fv = ferite_call_function(script, VAO(fv), NULL, func, NULL);
        if(!fv)
        {
            ferite_set_error(script, 0, "Failed to get file descriptor from object");
            return -1;
        }
        if(F_VAR_TYPE(fv) != F_VAR_LONG)
        {
            ferite_set_error(script, 0, "Object returned wrong type for getDescriptor()");
            return -1;
        }
        FD_SET(VAI(fv), f);
		if( FE_VAR_IS_DISPOSABLE(fv) )
            ferite_variable_destroy( script, fv );
        cnt++;
    }

    return cnt;
}

int update_SelectResult(FeriteScript *script, FeriteObject *obj,
                        FeriteUnifiedArray *inarray, char *vname, fd_set *f)
{
    int i;
    FeriteFunction *func = NULL;
    FeriteVariable *dup, *fv, *fo, *outarray;

    if(!(outarray = ferite_create_uarray_variable(script, vname, 0, FE_STATIC)))
    {
        return -1;
    }

    for(i = 0; i < inarray->size; i++)
    {
        fo = ferite_uarray_get_index(script, inarray, i);
        func = ferite_object_get_function_for_params(script, VAO(fo), "getDescriptor", NULL);
        fv = ferite_call_function(script, VAO(fo), NULL, func, NULL);
        if(FD_ISSET(VAI(fv), f))
        {
            dup = ferite_duplicate_variable(script, fo, NULL);
            ferite_uarray_add(script, VAUA(outarray), dup, NULL, FE_ARRAY_ADD_AT_END);
        }
        if( FE_VAR_IS_DISPOSABLE(fv) )
            ferite_variable_destroy( script, fv );
    }

    ferite_object_set_var(script, obj, vname, outarray);

    return 0;
}

int set_signal_action(FeriteScript *script, int sig, void *action)
{
    int ret;
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = action;

    while((ret = sigaction(sig, &act, NULL)) == -1 && errno == EINTR);

    if(ret == -1)
    {
        ferite_set_error(script, errno, "%s", strerror(errno));
        return -1;
    }

    return 0;
}
