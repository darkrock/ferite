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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
# include <windows.h>
# include <direct.h>
#else
# include <unistd.h>
# include <dirent.h>
#endif

#include "aphex.h"

int __aphex_wild_match( unsigned char *m, unsigned char *n );

char            *aphex_get_home_directory()
{
    char *str = NULL;

    if( (str = getenv("HOME")) != NULL )
      return strdup( str );
    return NULL;
}

char            *aphex_get_temp_directory()
{
    char *str = NULL;

    if( (str = getenv("TMP")) != NULL )
      return strdup( str );
#ifndef WIN32
    return strdup( "/tmp" );
#else
	return NULL;
#endif
}

AphexDirectory  *aphex_directory_read( char *path )
{
    AphexDirectory *retval = NULL;
#ifdef WIN32
    WIN32_FIND_DATA FindFileData;
    HANDLE          dir;
    char real_path[PATH_MAX];
#else
    DIR *dir;
    struct dirent *ent;
#endif

#ifndef WIN32
    dir = opendir( path );
    if( dir != NULL )
#else
      sprintf( real_path, "%s\\*.*", path );
    dir = FindFirstFile( real_path, &FindFileData );
    if( dir != INVALID_HANDLE_VALUE )
#endif
    {
        retval = aphex_malloc( sizeof( AphexDirectory ) );
        retval->contents = NULL;
        retval->size = 0;
        retval->pos = 0;
#ifndef WIN32
        while( (ent = readdir( dir )) )
#else
          while( dir != INVALID_HANDLE_VALUE && GetLastError() != ERROR_NO_MORE_FILES )
#endif
          {
#ifndef WIN32
              if( strcmp( ent->d_name, "." ) != 0 && strcmp( ent->d_name, ".." ) != 0 )
#else
                if( strcmp( FindFileData.cFileName, "." ) != 0 && strcmp( FindFileData.cFileName, ".." ) != 0 )
#endif
                {
                    retval->size++;
                    retval->contents = (char **)aphex_realloc( retval->contents, sizeof(char *) * retval->size );
#ifndef WIN32
                    retval->contents[retval->size-1] = strdup( ent->d_name );
#else
                    retval->contents[retval->size-1] = strdup( FindFileData.cFileName );
#endif
                }
#ifdef WIN32
              FindNextFile( dir, &FindFileData );
#endif
          }
#ifndef WIN32
        closedir( dir );
#else
        FindClose( dir );
#endif
    }
    return retval;
}

AphexDirectory  *aphex_directory_read_with_filter( char *path, char *filter )
{
    AphexDirectory *retval = NULL;
#ifdef WIN32
    WIN32_FIND_DATA FindFileData;
    HANDLE          dir;
    char real_path[PATH_MAX];
#else
    DIR *dir;
    struct dirent *ent;
#endif

#ifndef WIN32
    dir = opendir( path );
    if( dir != NULL )
#else
      sprintf( real_path, "%s\\*.*", path );
    dir = FindFirstFile( real_path, &FindFileData );
    if( dir != INVALID_HANDLE_VALUE )
#endif
    {
        retval = aphex_malloc( sizeof( AphexDirectory ) );
        retval->contents = NULL;
        retval->size = 0;
        retval->pos = 0;
#ifndef WIN32
        while( (ent = readdir( dir )) )
#else
          while( dir != INVALID_HANDLE_VALUE && GetLastError() != ERROR_NO_MORE_FILES )
#endif
          {
#ifndef WIN32
              if( strcmp( ent->d_name, "." ) != 0 && strcmp( ent->d_name, ".." ) != 0 )

#else
                if( strcmp( FindFileData.cFileName, "." ) != 0 && strcmp( FindFileData.cFileName, ".." ) != 0 )
#endif
                {
                    if( __aphex_wild_match(
#ifdef WIN32
                                           FindFileData.cFileName
#else
                                           ent->d_name
#endif
                                           , filter ) )
                    {
                        retval->size++;
                        retval->contents = (char **)aphex_realloc( retval->contents, sizeof(char *) * retval->size );
#ifndef WIN32
                        retval->contents[retval->size-1] = strdup( ent->d_name );
#else
                        retval->contents[retval->size-1] = strdup( FindFileData.cFileName );
#endif
                    }
                }
#ifdef WIN32
              FindNextFile( dir, &FindFileData );
#endif
          }
#ifndef WIN32
        closedir( dir );
#else
        FindClose( dir );
#endif
    }
    return retval;
}

AphexDirectory  *aphex_directory_read_unique_prefix( char *path )
{
    AphexDirectory *retval = NULL;
#ifdef WIN32
    WIN32_FIND_DATA FindFileData;
    HANDLE          dir;
    char real_path[PATH_MAX];
#else
    DIR *dir;
    struct dirent *ent;
#endif
    char filename[256], *ext;
    int i = 0, got_name = 0;

#ifndef WIN32
    dir = opendir( path );
    if( dir != NULL )
#else
      sprintf( real_path, "%s\\*.*", path );
    dir = FindFirstFile( real_path, &FindFileData );
    if( dir != INVALID_HANDLE_VALUE )
#endif
    {
        retval = aphex_malloc( sizeof( AphexDirectory ) );
        retval->contents = NULL;
        retval->size = 0;
        retval->pos = 0;

#ifndef WIN32
        while( (ent = readdir( dir )) )
#else
          while( dir != INVALID_HANDLE_VALUE && GetLastError() != ERROR_NO_MORE_FILES )
#endif
          {
#ifndef WIN32
              if( strcmp( ent->d_name, "." ) != 0 && strcmp( ent->d_name, ".." ) != 0 )

#else
                if( strcmp( FindFileData.cFileName, "." ) != 0 && strcmp( FindFileData.cFileName, ".." ) != 0 )
#endif
                {
#ifndef WIN32
                    strcpy( filename, ent->d_name );
#else
                    strcpy( filename, FindFileData.cFileName );
#endif
                    ext = strrchr( filename, '.' );
                    if( ext != NULL )
                    {
                        *ext = '\0';
                        got_name = 0;
                        for( i = 0; i < retval->size; i++ )
                        {
                            if( strcmp( retval->contents[i], filename ) == 0 )
                            {
                                got_name = 1;
                                break;
                            }
                        }
                        if( !got_name )
                        {
                            retval->size++;
                            retval->contents = (char **)aphex_realloc( retval->contents, sizeof(char *) * retval->size );
                            retval->contents[retval->size-1] = strdup( filename );
                        }
                    }
                }
#ifdef WIN32
              FindNextFile( dir, &FindFileData );
#endif
          }
#ifndef WIN32
        closedir( dir );
#else
        FindClose( dir );
#endif
    }
    return retval;
}

AphexDirectory  *aphex_directory_read_subdirs( char *path )
{
    return NULL;
}

void             aphex_directory_delete( AphexDirectory *dir )
{
    int i = 0;

    if( dir != NULL )
    {
        for( i = 0; i < dir->size; i++ )
          aphex_free( dir->contents[i] );
        aphex_free( dir->contents );
        aphex_free( dir );
    }
}

char *aphex_remove_symlinks( char *filename )
{
#ifndef WIN32
    char *buffer = aphex_calloc( PATH_MAX, sizeof(char) );
    char *tmp = realpath( filename, buffer );

    if( tmp == NULL )
    {
        aphex_free( buffer );
        return strdup( filename );
    }
#else
    char *buffer = strdup( filename );
#endif
    return buffer;
}

char *aphex_relative_to_absolute( char *path )
{
 /* ok this is what we do:
  * if the directory starts with the dir delim, return a copy of it
  * else
  *   get current working directory
  *   cat the two together
  *   flatten all ./'s to nothing
  *   flatten all ../'s
  *     this is done like so:
  *       if( ../ )
  *         backup
  *
  *   eg.
  *
  *   /ferite/ferite/modules/file/../../builder/builder
  *   /ferite/ferite/modules/../builder/builder
  *   /ferite/ferite/builder/builder
  */

    char *cwd = aphex_calloc( PATH_MAX, sizeof(char) );
    char *buf = aphex_calloc( strlen( path ) + PATH_MAX + 1, sizeof(char) );
    size_t i = 0, j = 0;

#ifndef WIN32
    if( path[0] == DIR_DELIM )
#else
	if( isalpha(path[0]) && path[1] == ':' && path[2] == DIR_DELIM )
#endif
    {
        aphex_free( buf );
        buf = strdup( path );
    }
    else
    {
#ifdef WIN32
#define getcwd _getcwd
#endif
        getcwd( cwd, PATH_MAX );
        sprintf( buf, "%s%c%s", cwd, DIR_DELIM, path );
    }

    for( i = 0; i < strlen( buf ) - 1; i++ )
    {
        if( buf[i] == '.' && buf[i+1] == '.' ) /* we have double dots */
        {
            j = i - 2; /* before the dir delimiter */
            for( ; j > 0; j-- )
              if( buf[j] == DIR_DELIM )
                break;
            /* now j points to the start block we have to delete */
            memmove( buf+j, buf+i+2, strlen(buf+i+2)+1 );
            i = j;
        }
    }
    for( i = 0; i < strlen( buf ) - 1; i++ )
    {
        if( buf[i] == '.' && buf[i+1] == DIR_DELIM )        /* remove './'s */
          memmove( buf+i-1, buf+i+1, strlen(buf+i+1)+1 );
        if( buf[i] == DIR_DELIM && buf[i+1] == DIR_DELIM )  /* remove '//'s */
          memmove( buf+i, buf+i+1, strlen(buf+i+1)+1 );
    }
    aphex_free( cwd );
    return buf;
}

char            *aphex_absolute_to_relative( char *path )
{
    char *cwd = NULL;
    char *p = path;
    size_t len = strlen(path);
    size_t i = 0;

    if( path[0] != DIR_DELIM )
    {
      return path;
    }
    
    cwd = aphex_calloc( PATH_MAX, sizeof(char) );
    getcwd( cwd, PATH_MAX );
    if( cwd[strlen(cwd)-1] != DIR_DELIM )
        cwd[strlen(cwd)] = DIR_DELIM;
    
    for( i = 0; i < len; i++ )
    {
	if( path[i] != cwd[i] )
	{
	    aphex_free( cwd );
	    return p;
	}
	else
	  p++;
    }
    aphex_free( cwd );
    return path;
}

char *aphex_directory_name( char *path ) 
{
	int i, len = strlen( path );
	if( path[len - 1] == DIR_DELIM )
		return path;
	
	for( i = len - 1; i >= 0; i-- ) 
    {
		if( path[i] == DIR_DELIM )
		{
			char *ret = aphex_calloc( i + 2, sizeof(char) );
			strncpy( ret, path, i + 1 );
			return ret;
		}
    }
	return NULL;
}
char *aphex_file_name( char *path ) 
{
	int i, len = strlen( path );
	if( path[len - 1] == DIR_DELIM )
		return path;
	
	for( i = len - 1; i >= 0; i-- ) 
    {
		if( path[i] == DIR_DELIM )
		{
			char *ret = aphex_calloc( len, sizeof(char) );
			strcpy( ret, path + i + 1 );
			return ret;
		}
    }
	return NULL;
}
