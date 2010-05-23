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

#include "../../config.h"

#ifdef WIN32
#include <windows.h>
#include <sys/time.h>
#include <time.h>
#include <utime.h>
#endif 

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

#include "ferite.h"

extern int errno;

struct FeTm
{
    FeriteVariable *tm_sec;
    FeriteVariable *tm_min;
    FeriteVariable *tm_hour;
    FeriteVariable *tm_mday;
    FeriteVariable *tm_mon;
    FeriteVariable *tm_year;
    FeriteVariable *tm_wday;
    FeriteVariable *tm_yday;
    FeriteVariable *tm_isdst;
    FeriteVariable *tm_zone;
    FeriteVariable *tm_gmtoff;
};

struct alarm_data {
    FeriteFunction       *function;
    FeriteScript         *script;
    int                  interval;
    int                  recurring;
    int                  timeleft;
};
typedef struct alarm_data AlarmData;
void timer_sig_alarm( int );
#ifndef FE_SYS_ALARM_OWNER
extern AlarmData *currentAlarm;
#endif

FeriteVariable *system_call_tm( FeriteScript *script, struct tm *tm );
int system_sync_to_tm( struct FeTm *Tm, struct tm *tm );
int system_sync_to_FeTm( struct FeTm *Tm, struct tm *tm );

#ifndef PF_LOCAL
#define PF_LOCAL PF_UNIX
#endif    
