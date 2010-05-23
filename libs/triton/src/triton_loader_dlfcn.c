/*
 * Copyright (C) 2001-2002 Evan Webb, Chris Ross
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

/*
   The Ferite library loader, dlfnc edition. Uses the system dl
   as the lib loader
*/

#include "../../../config.h"

#ifdef USE_DLFCN

# include "triton.h"

LIB_INIT
{
    return 0;
}

LIB_DEINIT
{
    return;
}

LIB_EXT
{
# ifdef USING_CYGWIN
    return ".dll";
# else
    return ".so";
# endif
}

LIB_OPEN
{
# ifdef USING_OPENBSD
    return dlopen(path, DL_LAZY );
# else
    return dlopen(path, RTLD_NOW | RTLD_GLOBAL);
# endif
}

LIB_SYM
{
# ifdef USING_OPENBSD /* this has to be done because OpenBSD is broken */
    char *real_symbol = NULL;
    void *ptr = NULL;

    ptr = dlsym( handle, symbol );
    if( ptr ==  NULL )
    {
        real_symbol = calloc( strlen(symbol) + 2, sizeof(char) );
        sprintf( real_sybmol, "_%s", symbol );
        ptr = dlsym(handle, real_symbol);
        free( real_symbol );
    }
    return ptr;
# else
    return dlsym(handle, symbol);
# endif
}

LIB_CLOSE
{
    return dlclose(handle);
}

LIB_ERROR
{
    return dlerror();
}

#endif
