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
# include "../../../config.h"
#else
# ifdef VCWIN32
#  include "snprintf.h" /* This is so that we have somethings */
# endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
# include <unistd.h>
#else
# include <io.h>
#endif
#include <string.h>

#include "aphex.h"
#include <fcntl.h>

/*#define APHEX_FILE_DEBUG*/
#ifdef APHEX_FILE_DEBUG
# define D(foo) printf foo
#else
# define D(foo)
#endif

char *aphex_file_exists_path( char *pf, ... )
{
    va_list ap;
    char buf[PATH_MAX], *file = NULL;
    struct stat filestat;

    va_start( ap, pf );
    vsnprintf( buf, PATH_MAX, pf, ap );
    file = aphex_relative_to_absolute( buf );
    if( stat( file, &filestat ) != -1 )
    {
        return file;
    }
    aphex_free( file );
    va_end( ap );
    return NULL;
}

int aphex_file_exists( char *pf, ... )
{
    va_list ap;
    char buf[PATH_MAX], *file = NULL;
    struct stat filestat;
    int retval = 0;

    va_start( ap, pf );
    vsnprintf( buf, PATH_MAX, pf, ap );
    file = aphex_relative_to_absolute( buf );
    if( stat( file, &filestat ) != -1 )
    {
        retval = 1;
    }
    aphex_free( file );
    va_end( ap );
    return retval;
}

char *aphex_file_to_string( char *file )
{
    struct stat in;
    char *data = NULL;

#ifdef VCWIN32
#define open  _open
#define read  _read
#define close _close
#define fileno _fileno
#endif
 
	FILE *f = fopen( file, "r" );

	if( f != NULL )
    {
        fstat( fileno(f), &in );

        data = aphex_malloc( in.st_size + 2 );
		memset( data, '\0', in.st_size + 2 );
        
		if( fread( data, sizeof(char), in.st_size, f ) != -1 )
        {
            fclose( f );
            return data;
        }
        fclose( f );
		aphex_free( data );
    }
    return NULL;
}

AphexFile *aphex_open_file( char *filename, char *mode, AphexSearchList *paths )
{
    AphexFile *file = NULL;
    FILE *handle = NULL;
    char actual_filename[PATH_MAX];
    struct stat filestat;
    int i = 0;

    sprintf( actual_filename, "%s", filename );
    D(( "aphex deubg: testing orignal filename %s\n", actual_filename ));
    if( stat( actual_filename, &filestat ) == -1 ) /* tried the actual file name */
    {
        if( filename[0] != DIR_DELIM && paths != NULL)
        {
            for( i = 0; i < paths->next_free; i++ )
            {
                sprintf( actual_filename, "%s%c%s", paths->path[i], DIR_DELIM, filename );
                D(( "aphex deubg: testing for %s\n", actual_filename ));
                if( stat( actual_filename, &filestat ) != -1 ) /* same sirectory as main script */
                {
                    break;
                }
            }
        }
    }
    handle = fopen( actual_filename, mode );
    if( handle != NULL )
    {
        file = aphex_malloc( sizeof( AphexFile ) );
        file->size = filestat.st_size;
        file->handle = handle;
        file->filename = strdup( actual_filename );
        return file;
    }
    return NULL;
}

void             aphex_close_file( AphexFile *file )
{
    if( file != NULL )
    {
        fclose( file->handle );
        aphex_free( file->filename );
        aphex_free( file );
    }
}

size_t           aphex_read_file( AphexFile *file, char *buffer, size_t length )
{
    if( file != NULL && buffer != NULL )
    {
        return fread( buffer, sizeof(char), length, file->handle );
    }
    return 0;
}

AphexSearchList *aphex_create_search_list()
{
    AphexSearchList *list = aphex_malloc( sizeof( AphexSearchList ) );
    list->path = aphex_calloc( sizeof( char * ) * 10, sizeof(char*) );
    list->size = 10;
    list->next_free = 0;
    return list;
}

void             aphex_add_to_list( AphexSearchList *list, char *path )
{
    if( list != NULL )
    {
        if( list->next_free >= list->size )
        {
            list->size += 5;
            list->path = aphex_realloc( list->path, sizeof( char * ) * list->size );
        }
        D(( "aphex debug: adding '%s' to slot %d\n", path, list->next_free ));
        list->path[list->next_free++] = strdup( path );
    }
}

void             aphex_delete_search_list( AphexSearchList *list )
{
    int i = 0;
    if( list != NULL )
    {
        for( i = 0; i < list->next_free; i++ )
          aphex_free( list->path[i] );
        aphex_free( list->path );
        aphex_free( list );
    }
}

