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

#include "util_date.h"
#include "../stream/util_stream.h"

FeriteVariable *system_call_tm( FeriteScript *script, struct tm *tm )
{
    FeriteVariable *object = NULL, *pointer = NULL, **args = NULL;
    FeriteClass *cls = NULL;
    FeriteFunction *function = NULL;

    if((cls = ferite_find_class( script, script->mainns, "Date" )) != NULL)
    {
        pointer = system_create_pointer_var( script, "struct::tm", tm );
        object = ferite_new_object( script, cls, NULL );
        function = ferite_object_get_function( script, VAO(object), "__RegisterFromPointer__" );
        args = ferite_create_parameter_list_from_data( script, "o", VAO(pointer) );
        ferite_variable_destroy( script, ferite_call_function( script, VAO(object), NULL, function, args ) );
        ferite_variable_destroy( script, pointer );
        ferite_delete_parameter_list( script, args );
        FE_RETURN_VAR( object );
    }
    FE_RETURN_NULL_OBJECT;
}
int system_sync_to_tm( struct FeTm *Tm, struct tm *tm )
{
    memset( tm, '\0', sizeof(struct tm) );
    tm->tm_sec   = VAI(Tm->tm_sec);
    tm->tm_min   = VAI(Tm->tm_min);
    tm->tm_hour  = VAI(Tm->tm_hour);
    tm->tm_mday  = VAI(Tm->tm_mday);
    tm->tm_mon   = VAI(Tm->tm_mon) - 1;
    tm->tm_year  = VAI(Tm->tm_year) - 1900;
    tm->tm_wday  = VAI(Tm->tm_wday);
    tm->tm_yday  = VAI(Tm->tm_yday);
    tm->tm_isdst = VAI(Tm->tm_isdst);
#if !defined(USING_SOLARIS) && !defined(USING_CYGWIN) && !defined(USING_MINGW)
    tm->tm_zone  = VAS(Tm->tm_zone)->data;
    tm->tm_gmtoff = VAI(Tm->tm_gmtoff);
#endif
    return 0;
}

int system_sync_to_FeTm( struct FeTm *Tm, struct tm *tm )
{
#if !defined(USING_SOLARIS) && !defined(USING_CYGWIN) && !defined(USING_MINGW)
	char *zone = (char *)tm->tm_zone;
#endif

    VAI(Tm->tm_sec)   = tm->tm_sec;
    VAI(Tm->tm_min)   = tm->tm_min;
    VAI(Tm->tm_hour)  = tm->tm_hour;
    VAI(Tm->tm_mday)  = tm->tm_mday;
    VAI(Tm->tm_mon)   = tm->tm_mon + 1;
    VAI(Tm->tm_year)  = tm->tm_year + 1900;
    VAI(Tm->tm_wday)  = tm->tm_wday;
    VAI(Tm->tm_yday)  = tm->tm_yday;
    VAI(Tm->tm_isdst) = tm->tm_isdst;
#if !defined(USING_SOLARIS) && !defined(USING_CYGWIN) && !defined(USING_MINGW)
    VAI(Tm->tm_gmtoff) = tm->tm_gmtoff;
    ferite_str_destroy( FE_NoScript, VAS(Tm->tm_zone) );
    VAS(Tm->tm_zone) = ferite_str_new( FE_NoScript, zone, 0, FE_CHARSET_DEFAULT );
#endif
    return 0;
}

#define FE_SYS_ALARM_OWNER 1
AlarmData *currentAlarm;
void timer_sig_alarm( int signum )
{
#ifndef WIN32
    if( currentAlarm != NULL )
    {
        ferite_script_function_execute( currentAlarm->script, currentAlarm->script->mainns, NULL, currentAlarm->function, NULL );
        if( currentAlarm->recurring )
          alarm( currentAlarm->interval );
    }
#endif
}
