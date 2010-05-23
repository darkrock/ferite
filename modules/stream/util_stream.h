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

#ifndef __FERITE_UTIL_STREAM__
#define __FERITE_UTIL_STREAM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ferite.h"
#define STREAM_READ_BUFFER 32768
#define StreamObject ((struct Stream *)(self->odata))

#define STREAM_RESET( STREAM ) do { \
	STREAM->file_pointer = NULL; \
	STREAM->file_descriptor = -1; \
} while(0)

extern int errno;

struct Stream
{
	FeriteString input_buffer;
	FeriteBuffer *output_buffer;
	char *endofline;
	FeriteFunction *read;
	FeriteFunction *write;
	FeriteVariable **read_args;
	FeriteVariable **write_args;
	int aggressive;
	char *errmsg;
	char *filename;
	void *file_pointer;
	int   file_descriptor;
	int eos;
	int pos;
	void *lock;
};

FERITE_API int stream_flush( FeriteScript *script, FeriteObject *self );
FERITE_API FeriteVariable *system_create_pointer_var( FeriteScript *script, char *name, void *ptr );
FERITE_API FeriteVariable *system_create_stream_object( FeriteScript *script, char *stream_class, FILE *f );
FERITE_API void stream_clear_input( struct Stream *s );

#define lock_object aphex_mutex_lock( StreamObject->lock )
#define unlock_object aphex_mutex_unlock( StreamObject->lock )

#endif /* __FERITE_UTIL_STREAM__ */
