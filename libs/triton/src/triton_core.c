/*
 * Copyright (C) 2001-2007 Evan Webb, Chris Ross
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

#include "triton.h"

int triton_init(void)
{
   /* printf("Welcome to Triton!\n"); */
   /* printf("TRITON: ::init...\n"); */
    return __triton_low_init();
}

void triton_deinit(void)
{
   /* printf("TRITON: ::deinit...\n"); */
    __triton_low_deinit();
}

void *triton_open(char *file)
{
   /* printf("TRITON ::open...\n"); */
    return __triton_low_open(file);
}

void *triton_openext(char *file)
{
    char *tmp = NULL;
    char *ext = 0;
    void *handle;
    const char *mext = __triton_low_module_ext();

    if(!file)
    {
        return triton_open(file);
    }

    ext = strrchr(file, '.');
    if(ext && ((strcmp (ext, mext) == 0)))
    {
        return triton_open(file);
    }

    tmp = malloc(strlen(file) + strlen(mext) +1);
    strcpy(tmp, file);
    strcat(tmp, mext);

   /* so now go ahead and try to open it, then check for errors */
    handle = triton_open(tmp);
    if(handle)
    {
        free(tmp);
        return handle;
    }

   /* giving up, couldnt find it */
   /*  ferite_error(NULL, "File not found: '%s' (lib error: '%s')\n",file, ferite_lib_error()); */
    return NULL;
}

void *triton_getsym(void *h, char *f)
{
   /* printf("::getsym ('%s')\n",f); */
    return __triton_low_getsym(h,f);
}

int triton_close(void *h)
{
    return __triton_low_close(h);
}

const char *triton_error(void)
{
    return __triton_low_error();
}

const char *triton_library_ext(void)
{
    return __triton_low_module_ext();
}
