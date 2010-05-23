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
  The Ferite library loader, ltdl edition. Uses ltdl as the lib loader
*/

#include "../../../config.h"


#ifdef USE_LTDL

#define HAVE_CONFIG_H
#include "./libltdl/config.h"
#undef HAVE_CONFIG_H
#ifndef LTDL_SHLIB_EXT
#define LTDL_SHLIB_EXT ".so"
#endif

LIB_INIT {
   return lt_dlinit();
}

LIB_DEINIT { 
	lt_dlexit();
}

LIB_EXT {
   return LTDL_SHLIB_EXT;
}

LIB_OPEN {
   lt_dlhandle *h = malloc( sizeof( lt_dlhandle ) );
   *h = lt_dlopen(path);
   return h;
}

LIB_SYM {
   void *h;
   h = lt_dlsym(*handle, symbol);
   if(h == NULL) {
      printf("LOADER: '%s' not found!\n",symbol);
   }
   return h;
}

LIB_CLOSE {
   int r = lt_dlclose(*h);
   free(h);
   return r;
}

LIB_ERROR {
   return lt_dlerror();
}

#endif
