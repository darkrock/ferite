/*
 * Copyright (C) 1999-2007 Chris Ross
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

#ifndef __FERITE_CONTAINER__
#define __FERITE_CONTAINER__

enum 
{
	FE_CONTAINER_SOURCE_EXTERNAL = 1,
	FE_CONTAINER_SOURCE_SELF = 2,
	FE_CONTAINER_SOURCE_RELATIVE = 4
}; /* Up to 64 */

#define MARK_SOURCE_EXTERNAL( item )             item->flags |= FE_CONTAINER_SOURCE_EXTERNAL
#define UNMARK_SOURCE_EXTERNAL( item )           item->flags &= ~FE_CONTAINER_SOURCE_EXTERNAL
#define MARK_SOURCE_SELF( item )                 item->flags |= FE_CONTAINER_SOURCE_SELF
#define UNMARK_SOURCE_SELF( item )               item->flags &= ~FE_CONTAINER_SOURCE_SELF
#define MARK_SOURCE_RELATIVE( item )             item->flags |= FE_CONTAINER_SOURCE_RELATIVE
#define UNMARK_SOURCE_RELATIVE( item )           item->flags &= ~FE_CONTAINER_SOURCE_RELATIVE

#define C_SOURCE_EXTERNAL( item )                (item->flags & FE_CONTAINER_SOURCE_EXTERNAL)
#define C_SOURCE_SELF( item )                    (item->flags & FE_CONTAINER_SOURCE_SELF)
#define C_SOURCE_RELATIVE( item )                (item->flags & FE_CONTAINER_SOURCE_RELATIVE)

#endif /* __FERITE_CONTAINER__ */
