/*
 * Copyright (C) 2000-2007 Chris Ross and various contributors
 * Copyright (C) 1999-2000 Chris Ross
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

#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

#include "ferite.h"

/**
 * @group General Utils
 * @description Every now and then we cant have nice data structures, so here are some handy
 *              functions to help with dealing with data with existing apis. All the functions
 *              require that they return a string that it is free'd using ffree().
 */

/**
 * @function ferite_lowercase
 * @declaration char *ferite_lowercase( char *str )
 * @brief Convert a string to lowercase
 * @param char *str The string to convert
 * @return A string that is a lowercase version of the passed string
 * @warning This function modifies the calling string.
 */
char *ferite_lowercase( char *str )
{
    unsigned int i, strt = 0, in_quote = 0;
    char *tmpstr = NULL;

    FE_ENTER_FUNCTION;

    if( str )
    {
        tmpstr = fmalloc_ngc( strlen(str)+1 );
        memset( tmpstr, '\0', strlen(str)+1 );
        for( i = 0; i < strlen(str); i++ )
        {
            if( str[i] == '\"' )
              in_quote = (in_quote == 0 ? 1 : 0);
            if( !in_quote )
            {
                if( isupper(*(str+i)) )
                  tmpstr[strt++] = str[i]+32;
                else
                  tmpstr[strt++] = str[i];
            }
            else
              tmpstr[strt++] = str[i];
        }
        strcpy( str, tmpstr );
        ffree_ngc(tmpstr);
        FE_LEAVE_FUNCTION( str );
    }
    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_strip_whitespace
 * @declaration char *ferite_strip_whitespace( char *str )
 * @brief Create a new string based upon the data with the whitespace removed
 * @param char *str The string to remove the whitespace from
 * @return A new string without whitespace
 * @description This function will remove whitespace but not from within double quotes.
 * @warning This function modifies the calling string.
 */
char *ferite_strip_whitespace( char *str )
{
    unsigned int i, strt = 0, in_quote = 0;
    char *tmpstr = NULL;

    FE_ENTER_FUNCTION;

    if( str )
    {
        tmpstr = fmalloc_ngc( strlen(str)+1 );
        memset( tmpstr, '\0', strlen(str)+1 );
        for( i = 0; i < strlen(str); i++ )
        {
            if( str[i] == '\"' )
              in_quote = (in_quote == 0 ? 1 : 0);
            if( in_quote || ! isspace(*(str+i)) )
              tmpstr[strt++] = str[i];
        }
        strcpy( str, tmpstr );
        ffree_ngc(tmpstr);
        FE_LEAVE_FUNCTION( str );
    }
    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_find_string
 * @declaration int ferite_find_string( char *haystack, char *needle )
 * @brief Find a substring within a string and return the first occurance
 * @param char *haystack The string to scan
 * @param char *needle The string to look for
 * @return The index of the start of the substring or -1 if no string is found
 */
int ferite_find_string( char *haystack, char *needle )
{
    char *p;

    FE_ENTER_FUNCTION;
    if( ( p = strstr( haystack, needle ) ) == NULL )
    {
        FE_LEAVE_FUNCTION( -1 );
    }
    FE_LEAVE_FUNCTION( (int)(p - haystack) );
}

/**
 * @function ferite_copy_string
 * @declaration char *ferite_copy_string( char *str, int start, int end )
 * @brief Copy a subsection of a string
 * @param char *str The string to copy from
 * @param int start The start of the subsection
 * @param int end The end of the subsection
 * @return A string containing the data between the two bounds
 * @warning It is the responsibility of the calling function to free the returned string.
 */
char *ferite_copy_string( char *str, int start, int end )
{
    int i = 0;
    char *rstr = NULL;

    FE_ENTER_FUNCTION;
    if( str && (start <= end) && (end < (signed)strlen( str )) )
    {
        rstr = fmalloc_ngc( sizeof(char) * (end - start + 2) );
        for( i = start; i <= end && str[i] != '\0'; i++ )
          rstr[i-start] = str[i];
        rstr[end - start + 1] = '\0';
        FE_LEAVE_FUNCTION( rstr );
    }
    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_delete_string
 * @declaration char *ferite_delete_string( char *str, int start, int end )
 * @brief Cuts out a substring starting at "start" up to but not including "end"
 * @param int start The start of the substring
 * @param int end The end of the substring 
 * @return The string without the substring
 * @warning It is the responsibility of the calling function to free the returned string.
 */
char *ferite_delete_string( char *str, int start, int end )
{
    char *rstr = NULL;

    FE_ENTER_FUNCTION;
    if( str && (start <= end) && (end < (signed)strlen( str )) )
    {
        rstr = fcalloc_ngc( strlen(str) - (end - start) + 1, sizeof( char ) );
        strncpy( rstr, str, start );
        strcat( rstr, str+end );
        FE_LEAVE_FUNCTION( rstr );
    }
    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_insert_string
 * @declaration char *ferite_insert_string( char *str, char *istr, int pos )
 * @brief Insert a string within another string at a specified position
 * @param char *str The string to insert into
 * @param char *istr The string to insert
 * @param int pos The position to insert at
 * @return A new string
 * @warning It is the responsibility of the calling function to free the returned string.
 */
char *ferite_insert_string( char *str, char *istr, int pos )
{
    char *rstr = NULL;

    FE_ENTER_FUNCTION;
    if( str && istr && pos >= 0 && pos <= (signed)strlen( str ))
    {
        rstr = fcalloc_ngc( strlen(str) + strlen(istr) + 1, sizeof( char ) );
        strncpy( rstr, str, pos );
        strcat( rstr, istr );
        strcat( rstr, str+pos );
        FE_LEAVE_FUNCTION( rstr );
    }
    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_replace_string
 * @declaration char *ferite_replace_string( char *str, char *pattern, char *data )
 * @brief Replace all occurances of a pattern in a string with another
 * @param char *str The string to scan
 * @param char *pattern The pattern to look for
 * @param char *datat The data to replace with
 * @return A string that has had the replacements
 * @warning It is the responsibility of the calling function to free the returned string.
 */
char *ferite_replace_string( char *str, char *pattern, char *data )
{
    size_t i = 0, start = 0;
    char *rstr = NULL, *tmpbuf = NULL;

    FE_ENTER_FUNCTION;
    if( str && pattern && data )
    {
		/* empty string -- nothing to replace */
        if( !str[0] )
        {
            FE_LEAVE_FUNCTION(  fstrdup(str) );
        }
		/* empty pattern -- nothing to replace */
        if( !pattern[0] )
        {
            FE_LEAVE_FUNCTION(  fstrdup(str) );
        }

        if( !data[0] ) /* empty replacement -- string won't grow */
			rstr = fcalloc_ngc( strlen( str ) + 1, sizeof(char) );
        else /* none of the strings can have length zero now */
			rstr = fcalloc_ngc( strlen( str ) * strlen( pattern ) * strlen( data ) + 1, sizeof(char) );
		
        FUD(("replace_str: replace \"%s\" with \"%s\"\n", pattern, data ));
        while( ((i=ferite_find_string( str+start, pattern ))+1) )
        {
            strncat( rstr, str+start, i );
            strcat( rstr, data );
            start = i + start + strlen(pattern);
        }
        strcat( rstr, str + start );
        tmpbuf = fstrdup( rstr );
        ffree_ngc( rstr );
        FE_LEAVE_FUNCTION( tmpbuf );
    }
    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function fstrdup
 * @declaration char *fstrdup( char *str )
 * @brief Allocate the memory and copy the string
 * @param char *str The string to copy
 * @return The copied string
 * @warning It is the responsibility of the calling function to free the returned string.
 */
char *ferite_strdup( char *str, char *filename, int line )
{
    char *tmpbuf = NULL;
#ifdef DEBUG
    char buf[1024];
#endif

    FE_ENTER_FUNCTION;
    if( str != NULL )
    {
#ifdef DEBUG
        sprintf( buf, "strdup: %s,%d", filename, line );
        tmpbuf = (char *)ferite_malloc( strlen( str )+1, buf, 198, NULL );
#else
        tmpbuf = (char *)fmalloc_ngc( strlen( str )+1 );
#endif
        strcpy( tmpbuf, str );
        FUD(("fstrdup: Copying %s to %p\n", str, tmpbuf ));
        FE_LEAVE_FUNCTION( tmpbuf );
    }
    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_int_to_ptr
 * @declaration int *ferite_int_to_ptr( int i )
 * @brief Allocate an int and set the value to that of the given parameter
 * @param int i The value
 * @return A pointer to an int
 * @warning It is the responsibility of the calling function to free the returned string.
 */
int *ferite_int_to_ptr( int i )
{
    int *value = fmalloc_ngc( sizeof(int) );
    *value = i;
    return value;
}

/**
 * @function ferite_stroflen
 * @declaration char *ferite_stroflen(char c, int l)
 * @brief Provide a string of a given length l filled with the character c
 * @param char c The character to use
 * @param int l The length of the string required.
 * @description The length of the string must not exceed 1024. If l is greater than or equal to 1024, a zero length
                string will be returned.
 * @warning You must not try to free the returned pointer.
 */
char *ferite_stroflen(char c, int l)
{
    static char buf[1024];
    int i = 0;

    buf[0] = '\0';
    if( l > 0 && l < 1024 )
    {
        while (l-- && l >= 0)
          buf[i++] = c;
        buf[i] = '\0';
    }
    return buf;
}

/**
 * @function ferite_strcasecmp
 * @declaration int ferite_strcasecmp( char *left, char *right )
 * @brief Do a case insensitive strcmp on the two strings.
 * @param char *left
 * @param char *right
 */
int ferite_strcasecmp( char *left, char *right ) 
{
    int   retv = 0;
    char *left_copy, *right_copy;
        
    FE_ENTER_FUNCTION;
    left_copy = fstrdup(left);
    right_copy = fstrdup(right);
    ferite_lowercase( left_copy );
    ferite_lowercase( right_copy );
    retv = strcmp( left_copy, right_copy );
    ffree_ngc( left_copy );
    ffree_ngc( right_copy );
    FE_LEAVE_FUNCTION( retv );
}

/**
 * @end
 */
