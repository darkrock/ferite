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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#ifndef WIN32
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dirent.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/un.h>
#include <pwd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <syslog.h>
#include <utime.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#endif

#include "../../config.h"
#include "ferite.h"

#define SelfStat ((struct stat *)(self->odata))
#define ENDOFLINE "\n"

extern int errno;

FeriteVariable *system_call_stat( FeriteScript *script, struct stat *in );
int make_fd_set(FeriteScript *script, FeriteUnifiedArray *array, fd_set *f);
int update_SelectResult(FeriteScript *script, FeriteObject *obj, FeriteUnifiedArray *inarray, char *vname, fd_set *f);
int set_signal_action(FeriteScript *script, int sig, void *action);
void ferite_signal_handler(int signal);
int add_signal_handler(FeriteScript *script, int signal, FeriteString *func, FeriteObject *obj);
void remove_signal_handler(FeriteScript *script, int signal);

#ifndef PF_LOCAL
#define PF_LOCAL PF_UNIX
#endif    
