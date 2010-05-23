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

#ifndef __APHEX_H__
# define __APHEX_H__

# include <stdio.h>
# include <stdarg.h>
# include <limits.h>

#define APHEX_API
#if defined(WIN32) && !defined(USING_MINGW)
# include <windows.h>
# undef  APHEX_API
# ifdef FERITELIB_EXPORTS
#  define APHEX_API __declspec(dllexport)
# else
#  define APHEX_API __declspec(dllimport)
# endif
#endif

# ifndef WIN32
#  define DIR_DELIM '/'
# else
#  define DIR_DELIM '\\'
# endif

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# ifdef USE_PTHREAD
#  include <pthread.h>
#  if (!defined(PTHREAD_MUTEX_RECURSIVE) && defined(PTHREAD_MUTEX_RECURSIVE_NP)) || defined(USING_LINUX)
#     define PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
#   endif
# endif

# if defined(USING_DARWIN) || defined(USING_SNOWLEOPARD) || defined(USING_CYGWIN) || defined(USING_SOLARIS)
#  define USING_FAKE_RECURSIVE_MUTEX
# endif

typedef struct __aphex_file
{
    FILE *handle;
    char *filename;
    long  size;
} AphexFile;

typedef struct __aphex_directoy
{
    char **contents;
    long   size;
	long   pos;
} AphexDirectory;

typedef struct __aphex_search_list
{
    char **path;
    long   size;
    long   next_free;
} AphexSearchList;

typedef struct __aphex_thread
{
# ifdef USE_PTHREAD
    pthread_t       thread;
    pthread_attr_t  attrs;
    int             running;
# else
#  ifdef WIN32
    HANDLE          thread;
    int             running;
#  endif
# endif
} AphexThread;

typedef struct __aphex_mutex
{
# ifdef USE_PTHREAD
    pthread_mutex_t     mutex;
    pthread_mutexattr_t attr;
#  if defined(USING_FAKE_RECURSIVE_MUTEX)
    int             count;
	int             is_owned;
    pthread_t       owner;
    pthread_cond_t  cond;
#  endif
# else
#  ifdef WIN32

#  endif
# endif
    int recursive;
} AphexMutex;

typedef struct __aphex_event
{
# ifdef USE_PTHREAD
    pthread_mutex_t     mutex;
    pthread_mutexattr_t attr;
    pthread_cond_t      cond;
# else
#  ifdef WIN32
    int                 condition;
#  endif /* WIN32 */    
// XXX: darwin/cygwin/solaris/win32
# endif
} AphexEvent;

APHEX_API int aphex_init( int allowed_to_use_gc );

APHEX_API void *aphex_malloc( size_t size );
APHEX_API void *aphex_realloc( void *old, size_t new );
APHEX_API void *aphex_calloc( size_t size, size_t block );
APHEX_API void  aphex_free( void * ptr );
APHEX_API char *aphex_strdup( char *original );

/* Disk Stuff */

APHEX_API char            *aphex_file_exists_path( char *pf, ... );
APHEX_API int              aphex_file_exists( char *pf, ... );
APHEX_API char            *aphex_file_to_string( char *file );

APHEX_API AphexFile       *aphex_open_file( char *filename, char *mode, AphexSearchList *paths );
APHEX_API void             aphex_close_file( AphexFile *file );
APHEX_API size_t           aphex_read_file( AphexFile *file, char *buffer, size_t length );

APHEX_API AphexSearchList *aphex_create_search_list();
APHEX_API void             aphex_add_to_list( AphexSearchList *list, char *path );
APHEX_API void             aphex_delete_search_list( AphexSearchList *list );

APHEX_API char            *aphex_get_home_directory();
APHEX_API char            *aphex_get_temp_directory();

APHEX_API AphexDirectory  *aphex_directory_read( char *path );
APHEX_API AphexDirectory  *aphex_directory_read_with_filter( char *path, char *filter );
APHEX_API AphexDirectory  *aphex_directory_read_unique_prefix( char *path );
APHEX_API AphexDirectory  *aphex_directory_read_subdirs( char *path );
APHEX_API void             aphex_directory_delete( AphexDirectory *dir );
APHEX_API char            *aphex_relative_to_absolute( char *path );
APHEX_API char            *aphex_absolute_to_relative( char *path );
APHEX_API char            *aphex_directory_name( char *path );
APHEX_API char            *aphex_file_name( char *path );

/* Configuration Reading Stuff */

APHEX_API void             aphex_configuration_load();
APHEX_API void             aphex_configuration_store();
APHEX_API char            *aphex_get_string( char *path );
APHEX_API void             aphex_set_string( char *path, char *value );
APHEX_API long             aphex_get_long( char *path );
APHEX_API void             aphex_set_long( char *path, long value );

/* thread stuff */

APHEX_API AphexThread     *aphex_thread_create();
APHEX_API void             aphex_thread_destroy( AphexThread *thread );
APHEX_API int              aphex_thread_start( AphexThread *thread, void *(*start_routine)(void *), void *arg, int detach );
APHEX_API void             aphex_thread_stop( AphexThread *thread );
APHEX_API void             aphex_thread_sleep( AphexThread *thread, int msecs );
APHEX_API void             aphex_thread_join( AphexThread *thread );
APHEX_API void             aphex_thread_exit( AphexThread *thread, void *rval );

APHEX_API AphexMutex      *aphex_mutex_create();
APHEX_API AphexMutex      *aphex_mutex_recursive_create();
APHEX_API void             aphex_mutex_destroy( AphexMutex *mutex );
APHEX_API int              aphex_mutex_lock( AphexMutex *mutex );
APHEX_API int              aphex_mutex_unlock( AphexMutex *mutex );

APHEX_API AphexEvent      *aphex_event_create();
APHEX_API void             aphex_event_destroy( AphexEvent *event );
APHEX_API int              aphex_event_signal( AphexEvent *event );
APHEX_API int              aphex_event_wait( AphexEvent *event );
APHEX_API int              aphex_event_timedwait( AphexEvent *event, int seconds);

/* Atomic counter */
typedef struct __aphex_atomic_type 
{
	int counter;
} AphexAtomicType;
/*
APHEX_API AphexAtomicType *aphex_atomic_create( int value );
APHEX_API void             aphex_atomic_destroy( AphexAtomicType *target );
APHEX_API static __inline__ int aphex_atomic_increment( AphexAtomicType *target );
APHEX_API static __inline__ int aphex_atomic_decrement( AphexAtomicType *target );
APHEX_API static __inline__ int aphex_atomic_increment_v( AphexAtomicType *target, int value );
APHEX_API static __inline__ int aphex_atomic_decrement_v( AphexAtomicType *target, int value );
 */
# define DEFAULT_TIMEOUT 100

APHEX_API int aphex_notify_can_read_with_timeout(int fd, int wait, int ce);
APHEX_API int aphex_notify_can_read(int fd, int ce);

#endif /* __APHEX_H__ */
