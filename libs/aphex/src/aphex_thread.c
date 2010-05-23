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

#ifndef APHEX_AS_COMPONENT
#include "../../../config.h"
#endif

#ifdef HAVE_LIBGC
# define GC_THREADS
# define GC_REDIRECT_TO_LOCAL
# include "gc.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#ifndef WIN32
# include <unistd.h>
#endif
#include <string.h>

#include "aphex.h"

void *aphex_gc_malloc( size_t size ) {
#ifdef HAVE_LIBGC
	return GC_MALLOC(size);
#else
	return malloc(size);
#endif
}
void *aphex_gc_realloc( void *old, size_t new ) {
#ifdef HAVE_LIBGC
	return GC_REALLOC(old, new);
#else
	return realloc(old, new);
#endif
}
void *aphex_gc_calloc( size_t size, size_t block ) {
#ifdef HAVE_LIBGC
	void *ptr = GC_MALLOC(size * block);
	memset(ptr, 0, (size * block));
	return ptr;
#else
	return calloc(size, block);
#endif
}
void  aphex_gc_free( void * ptr ) { 
#ifdef HAVE_LIBGC
#else
	free(ptr);
#endif
}
char *aphex_gc_strdup( char *original ) {
#ifdef HAVE_LIBGC
	return GC_STRDUP(original);
#else
	return strdup(original);
#endif
}

void *aphex_system_malloc( size_t size ) {
	return malloc(size);
}
void *aphex_system_realloc( void *old, size_t new ) {
	return realloc(old, new);
}
void *aphex_system_calloc( size_t size, size_t block ) {
	return calloc(size, block);
}
void  aphex_system_free( void * ptr ) { 
	free(ptr);
}
char *aphex_system_strdup( char *original ) {
	return strdup(original);
}


void *(*_aphex_malloc)( size_t size );
void *(*_aphex_calloc)( size_t size, size_t blk_size );
void *(*_aphex_realloc)( void *ptr, size_t size );
void  (*_aphex_free)( void *ptr );
char *(*_aphex_strdup)( char * original );

void *aphex_malloc( size_t size ) {
	return _aphex_malloc(size);
}
void *aphex_realloc( void *old, size_t new ) {
	return _aphex_realloc(old, new);
}
void *aphex_calloc( size_t size, size_t block ) {
	return _aphex_calloc(size, block);
}
void  aphex_free( void * ptr ) { 
	_aphex_free(ptr);
}
char *aphex_strdup( char *original ) {
	return _aphex_strdup(original);
}

int aphex_init( int allowed_to_use_gc ) {
	_aphex_malloc  = aphex_system_malloc;
	_aphex_calloc  = aphex_system_calloc;
	_aphex_realloc = aphex_system_realloc;
	_aphex_free    = aphex_system_free;
	_aphex_strdup  = aphex_system_strdup;
	
	if( allowed_to_use_gc ) {
		_aphex_malloc  = aphex_gc_malloc;
		_aphex_calloc  = aphex_gc_calloc;
		_aphex_realloc = aphex_gc_realloc;
		_aphex_free    = aphex_gc_free;
		_aphex_strdup  = aphex_gc_strdup;
	}
}

/***************************************************
 * THREAD
 ***************************************************/
AphexThread *aphex_thread_create()
{
    AphexThread *thread = aphex_malloc(sizeof(AphexThread));
    return thread;
}

void aphex_thread_destroy( AphexThread *thread )
{
    if( thread != NULL )
    {
        if( thread->running )
          aphex_thread_stop( thread );
        aphex_free( thread );
    }
}

int aphex_thread_start( AphexThread *thread, void *(*start_routine)(void *), void *arg, int detach )
{
    int retval = 0;

    if( thread != NULL && start_routine != NULL )
    {
#ifdef USE_PTHREAD
		thread->running = 1;
        retval = pthread_create( &(thread->thread), NULL, start_routine, arg );
        if( detach )
          pthread_detach( thread->thread );
#endif
        if( retval != 0 )
        {
            printf( "aphex: ERROR: Not enough system resources to create thread\n" );
            return -1;
        }
    }
    return 0;
}

void aphex_thread_stop( AphexThread *thread )
{
    if( thread != NULL )
    {
#ifdef USE_PTHREAD
		if( thread->running )
			pthread_cancel( thread->thread );
#endif
        thread->running = 0;
    }
}

#ifdef WIN32
void usleep( int useconds )
{
    if (useconds > 500)
      sleep ((useconds+500)/1000);
    else if (useconds > 0)
      sleep (1);
    else
      sleep (0);
}
#endif

void aphex_thread_sleep( AphexThread *thread, int msecs )
{
    usleep( msecs * 1000 );
}

void aphex_thread_join( AphexThread *thread )
{
    if( thread != NULL )
    {
#ifdef USE_PTHREAD
		pthread_join( thread->thread, NULL );
#endif
        thread->running = 0;
    }
}

void aphex_thread_exit( AphexThread *thread, void *rval )
{
    if( thread != NULL )
    {
#ifdef USE_PTHREAD
		pthread_exit( rval );
#endif
        thread->running = 0;
    }
}

/***************************************************
 * MUTEX
 ***************************************************/
AphexMutex *aphex_mutex_create()
{
    AphexMutex *mutex = aphex_malloc(sizeof(AphexMutex));
#ifdef USE_PTHREAD
    pthread_mutex_init( &(mutex->mutex), NULL );
# if defined(USING_FAKE_RECURSIVE_MUTEX)
    mutex->count = -1;
    memset( &(mutex->owner), '\0', sizeof(pthread_t) );
# endif
#endif
    mutex->recursive = 0;
    return mutex;
}

AphexMutex *aphex_mutex_recursive_create()
{
    AphexMutex *mutex = aphex_malloc(sizeof(AphexMutex));

#ifdef USE_PTHREAD
#if defined(USING_FAKE_RECURSIVE_MUTEX)
    mutex->count = 0;
    mutex->owner = NULL;
	mutex->is_owned = 0;
    pthread_cond_init( &mutex->cond, NULL );
    pthread_mutex_init( &mutex->mutex, NULL );
#else
    pthread_mutexattr_init( &mutex->attr );
    pthread_mutexattr_settype( &mutex->attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init( &mutex->mutex, &mutex->attr );
    pthread_mutexattr_destroy( &mutex->attr );
#endif
    mutex->recursive = 1;
#endif

    return mutex;
}

void aphex_mutex_destroy( AphexMutex *mutex )
{
    if( mutex != NULL )
    {
#ifdef USE_PTHREAD
#if defined(USING_FAKE_RECURSIVE_MUTEX)
        pthread_cond_destroy( &mutex->cond );
#else
#endif
        pthread_mutex_destroy( &mutex->mutex );
        aphex_free( mutex );
#endif
    }
}

int aphex_mutex_lock( AphexMutex *mutex )
{
#ifdef USE_PTHREAD
    pthread_t self = pthread_self();

    if( mutex != NULL )
    {
        if( pthread_mutex_lock( &mutex->mutex ) == -1 )
          return -1;

#if defined(USING_FAKE_RECURSIVE_MUTEX)
        if( mutex->recursive == 1 )
        {
            while (1)
            {
                if( pthread_equal(mutex->owner, self) )
                {
                    mutex->count++;
                    break;
                }
                else if( mutex->is_owned == 0 )
                {
                    mutex->owner = self;
                    mutex->count = 1;
					mutex->is_owned = 1;
                    break;
                }
                else
                {
                    if( pthread_cond_wait( &mutex->cond, &mutex->mutex ) == -1 )
                      return -1;
                }
            }
            pthread_mutex_unlock( &mutex->mutex );
        }
#endif
    }
#endif
    return 0;
}

int aphex_mutex_unlock( AphexMutex *mutex )
{
    if( mutex != NULL )
    {
#ifdef USE_PTHREAD
#if defined(USING_FAKE_RECURSIVE_MUTEX)
        if( mutex->recursive == 1 )
        {
            if( pthread_mutex_lock (&mutex->mutex) == -1 )
              return -1;
            
            mutex->count--;
            if( mutex->count == 0 )
            {
                mutex->is_owned = 0;
                pthread_cond_signal( &mutex->cond );
            }
        }
#endif
        pthread_mutex_unlock( &mutex->mutex );
#endif
    }
    return 0;
}

/***************************************************
 * EVENT
 ***************************************************/
AphexEvent *aphex_event_create()
{
    AphexEvent *event = aphex_malloc(sizeof(AphexEvent));
#ifdef USE_PTHREAD
    if(pthread_cond_init( &(event->cond), NULL) != 0){
        aphex_free(event);
	return NULL;
    }
    if(pthread_mutex_init( &(event->mutex), NULL ) != 0){
        aphex_free(event);
	return NULL;
    }    
#endif
    return event;
}

void aphex_event_destroy( AphexEvent *event )
{
    if( event != NULL )
    {
#ifdef USE_PTHREAD
         pthread_cond_destroy( &event->cond );
         pthread_mutex_destroy( &event->mutex );
#endif
         aphex_free(event);
    }
}

int aphex_event_signal( AphexEvent *event )
{
#ifdef USE_PTHREAD
    pthread_cond_signal(&(event->cond));
#endif
    return 0;
}

int aphex_event_wait( AphexEvent *event )
{
#ifdef USE_PTHREAD
    pthread_cond_wait(&(event->cond), &(event->mutex));
#endif
    return 0;
}

#if defined(WIN32)

#include <windows.h>
#include <time.h>

#ifndef __GNUC__
#define EPOCHFILETIME (116444736000000000i64)
#else
#define EPOCHFILETIME (116444736000000000LL)
#endif

struct timezone {
	int tz_minuteswest; /* minutes W of Greenwich */
	int tz_dsttime;     /* type of dst correction */
};

__inline int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	FILETIME        ft;
	LARGE_INTEGER   li;
	__int64         t;
	static int      tzflag;
	
	if (tv)
	{
		GetSystemTimeAsFileTime(&ft);
		li.LowPart  = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		t  = li.QuadPart;       /* In 100-nanosecond intervals */
		t -= EPOCHFILETIME;     /* Offset to the Epoch time */
		t /= 10;                /* In microseconds */
		tv->tv_sec  = (long)(t / 1000000);
		tv->tv_usec = (long)(t % 1000000);
	}
	
	if (tz)
	{
		if (!tzflag)
		{
			_tzset();
			tzflag++;
		}
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime = _daylight;
	}
	
	return 0;
}

#endif

int aphex_event_timedwait( AphexEvent *event , int seconds)
{
    int t_ret = 0;

#ifdef USE_PTHREAD
    struct timespec   ts;
    struct timeval    tp;

    gettimeofday(&tp, NULL);
    ts.tv_sec  = tp.tv_sec;
    ts.tv_nsec = tp.tv_usec * 1000;
    ts.tv_sec += seconds;

    t_ret = pthread_cond_timedwait(&(event->cond), &(event->mutex), &ts);
#endif
    
    if (t_ret != 0)
        return 1;
    else
        return 0;
}
