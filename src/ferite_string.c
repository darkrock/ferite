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
 * @group Strings
 * @description Ferite strings provide a nice clean method for encoding and storing strings, and
 *              managing binary data. The functions below provide several means of string mangement
 *              which makes twiddling 'string' variables easier.
 */

/**
 * @function ferite_str_new
 * @declaration FeriteString *ferite_str_new( char *str, int length, int encoding )
 * @brief Create a new string with exisitng data and encoding
 * @param FeriteScript *script The script context
 * @param char *str The data to use
 * @param int length The length of the data, if it is 0, strlen() is called on the data.
 * @param int encoding The encoding to use, if you are not sure use FE_CHARSET_DEFAULT
 * @return A FeriteString containing the data
 */
FeriteString *ferite_str_new( FeriteScript *script, char *str, size_t length, int encoding )
{
    FeriteString *ptr;

    FE_ENTER_FUNCTION;
    ptr = fmalloc( sizeof( FeriteString ) );
    ptr->pos = -1;
    ptr->encoding = encoding;

    if( str == NULL || *str == '\0' )
    {
        ptr->data = fcalloc( length + 1, sizeof(char) );
    }
    else
    {
        if(length == 0)
          length = strlen( str );
        ptr->data = fmalloc( length + 1 );
        memcpy( ptr->data, str, length );
        ptr->data[length] = '\0';
    }
    ptr->length = length;
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_str_dup
 * @declaration FeriteString *ferite_str_dup( FeriteString *str )
 * @brief Duplicate a ferite string
 * @param FeriteScript *script The script context
 * @param FeriteString *str The string to duplicate
 * @return A new string 
 */
FeriteString *ferite_str_dup( FeriteScript *script, FeriteString *str )
{
    FeriteString *ptr;
    FE_ENTER_FUNCTION;
    if( str == NULL )
    {
        ptr = ferite_str_new( script, NULL, 0, FE_CHARSET_DEFAULT );
    }
    else
    {
        ptr = fmalloc( sizeof( FeriteString ) );
        ptr->pos = -1;
        ptr->data = fmalloc( str->length + 1 );
        memcpy( ptr->data, str->data, str->length );
        ptr->data[str->length] = '\0';
        ptr->length = str->length;
        ptr->encoding = str->encoding;
    }
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_str_set
 * @declaration void ferite_str_set( FeriteString *str, char *data, int length, int encoding )
 * @brief Set the data contained within an existing string
 * @param FeriteScript *script The script context
 * @param FeriteString *str The string to set
 * @param char *data The data to use
 * @param int length The length of the data
 * @param int encoding The encoding of the data, for a default use FE_CHARSET_DEFAULT
 */
void ferite_str_set( FeriteScript *script, FeriteString *str, char *data, size_t length, int encoding )
{
    FE_ENTER_FUNCTION;
    if( str->data )
      ffree( str->data );
    if( data == NULL )
      data = "";
    if( length == 0 )
      length = strlen( data );
    str->data = fmalloc( length + 1 );
    memcpy( str->data, data, length );
    str->data[length] = '\0';
    str->length = length;
    str->encoding = encoding;
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_str_destroy
 * @declaration void ferite_str_destroy( FeriteString *str )
 * @brief Destroy the string and free all memory
 * @param FeriteScript *script The script context
 * @param FeriteString *str The string to destroy
 */
void ferite_str_destroy( FeriteScript *script, FeriteString *str )
{
    FE_ENTER_FUNCTION;
    if( str )
    {
        if( str->data )
        {
            ffree( str->data );
        }
        ffree( str );
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_str_cmp
 * @declaration int ferite_str_cmp( FeriteString *str1, FeriteString *str2 )
 * @brief Compare two strings
 * @param FeriteScript *script The script context
 * @param FeriteString *str1 The first string
 * @param FeriteString *str2 The second string
 * @return 1 if they match, 0 if they dont
 */
int ferite_str_cmp( FeriteScript *script, FeriteString *str1, FeriteString *str2 )
{
    size_t i = 0;

    FE_ENTER_FUNCTION;
    if( str1->length != str2->length )
    {
        FE_LEAVE_FUNCTION( 0 );
    }
    for( i = 0; i < str1->length; i++ )
    {
        if( str1->data[i] != str2->data[i] )
        {
            FE_LEAVE_FUNCTION( 0 );
        }
    }
    FE_LEAVE_FUNCTION( 1 );
}

/**
 * @function ferite_str_case_cmp
 * @declaration int ferite_str_case_cmp( FeriteString *str1, FeriteString *str2 )
 * @brief Compare two strings but dont take into consideration what case the strings are
 * @param FeriteScript *script The script context
 * @param FeriteString *str1 The first string
 * @param FeriteString *str2 The second string
 * @return 1 if they match, 0 if they dont
 */
int ferite_str_case_cmp( FeriteScript *script, FeriteString *str1, FeriteString *str2 )
{
    size_t i;

    FE_ENTER_FUNCTION;
    
	if( str1->length != str2->length )
    {
        FE_LEAVE_FUNCTION( 0 );
    }
    for( i = 0; i < str1->length; i++ )
    {
        if( toupper(str1->data[i]) != toupper(str2->data[i]) )
        {
            FE_LEAVE_FUNCTION( 0 );
        }
    }
    FE_LEAVE_FUNCTION( 1 );
}

/**
 * @function ferite_str_ncmp
 * @declaration int ferite_str_ncmp( FeriteString *str1, FeriteString *str2, int size )
 * @brief Compare two strings upto a certain size
 * @param FeriteScript *script The script context
 * @param FeriteString *str1 The first string
 * @param FeriteString *str2 The second string
 * @param int size The size upto which the strings should be compared
 * @return 1 if they match, 0 if they dont
 */
int ferite_str_ncmp( FeriteScript *script, FeriteString *str1, FeriteString *str2, size_t size )
{
    size_t i,len;

    FE_ENTER_FUNCTION;
    len = (size < str1->length) ? size : str1->length;
    for( i = 0; i < len; i++ )
    {
        if( str1->data[i] != str2->data[i] )
        {
            FE_LEAVE_FUNCTION( 0 );
        }
    }
    FE_LEAVE_FUNCTION( 1 );
}

/**
 * @function ferite_str_case_ncmp
 * @declaration int ferite_str_case_ncmp( FeriteString *str1, FeriteString *str2, int size )
 * @brief Compare two strings upto a certain size without considering the case of the strings
 * @param FeriteScript *script The script context
 * @param FeriteString *str1 The first string
 * @param FeriteString *str2 The second string
 * @param int size The size upto which the strings should be compared
 * @return 1 if they match, 0 if they dont
 */
int ferite_str_case_ncmp( FeriteScript *script, FeriteString *str1, FeriteString *str2, size_t size )
{
    size_t i,len;

    FE_ENTER_FUNCTION;
    len = (size < str1->length) ? size : str1->length;
    for( i = 0; i < len; i++ )
    {
        if( toupper(str1->data[i]) != toupper(str2->data[i]) )
        {
            FE_LEAVE_FUNCTION( 0 );
        }
    }
    FE_LEAVE_FUNCTION( 1 );
}

/**
 * @function ferite_str_cpy
 * @declaration int ferite_str_cpy( FeriteString *str1, FeriteString *str2 )
 * @brief Copy the contents of the second string to the first string
 * @param FeriteScript *script The script context
 * @param FeriteString *str1 The first string
 * @param FeriteString *str2 The second string
 * @return The amount of data copied
 */
int ferite_str_cpy( FeriteScript *script, FeriteString *str1, FeriteString *str2 )
{
    FE_ENTER_FUNCTION;
    ffree( str1->data);
    str1->data = fmalloc( str2->length + 1 );
    memcpy( str1->data, str2->data, str2->length );
    str1->data[str2->length] = '\0';
    str1->length = str2->length;
    FE_LEAVE_FUNCTION((int)str1->length);
}

/**
 * @function ferite_str_ncpy
 * @declaration int ferite_str_ncpy( FeriteString *str1, FeriteString *str2, int size )
 * @brief Copy the contents of the second string to the first string upto size character
 * @param FeriteScript *script The script context
 * @param FeriteString *str1 The first string
 * @param FeriteString *str2 The second string
 * @param int size The amount of data to copy
 * @return The amount of data copied
 */
int ferite_str_ncpy( FeriteScript *script, FeriteString *str1, FeriteString *str2, size_t size )
{
    size_t len;

    FE_ENTER_FUNCTION;
    ffree( str1->data);
    len = (size >= str2->length) ? str2->length : size;
    str1->data = fmalloc( len + 1 );
    str1->length = len;
    memcpy( str1->data, str2->data, len );
    str1->data[len] = '\0';
    FE_LEAVE_FUNCTION((int)len);
}

/**
 * @function ferite_str_cat
 * @declaration int ferite_str_cat( FeriteString *str1, FeriteString *str2 )
 * @brief Concatenate the second string onto the first
 * @param FeriteScript *script The script context
 * @param FeriteString *str1 The first string
 * @param FeriteString *str2 The second string
 */
int ferite_str_cat( FeriteScript *script, FeriteString *str1, FeriteString *str2 )
{
    char *newbuf;

    FE_ENTER_FUNCTION;
    newbuf = fmalloc( str1->length + str2->length + 1);
    memcpy( newbuf, str1->data, str1->length );
    memcpy( newbuf + str1->length, str2->data, str2->length );
    newbuf[ str1->length + str2->length ] = '\0';
    ffree( str1->data );
    str1->data = newbuf;
    str1->length = str1->length + str2->length;
    FE_LEAVE_FUNCTION(0);
}

/**
 * @function ferite_str_data_cat
 * @declaration int ferite_str_data_cat( FeriteString *str1, void *data, int size )
 * @brief Concatenate the data onto the first string
 * @param FeriteScript *script The script context
 * @param FeriteString *str1 The first string
 * @param void *data The data to concatenate
 * @param int size The length of the data to concatenate
 */
int ferite_str_data_cat( FeriteScript *script, FeriteString *str, void *data, size_t size )
{
    char *newbuf;
    FE_ENTER_FUNCTION;
    newbuf = fmalloc( str->length + size + 1);
    memcpy( newbuf, str->data, str->length );
    memcpy( newbuf + str->length, data, size );
    newbuf[ str->length + size ] = '\0';
    ffree( str->data );
    str->data = newbuf;
    str->length = str->length + size;
    FE_LEAVE_FUNCTION(1);
}

/**
 * @function ferite_str_replace
 * @declaration FeriteString *ferite_str_replace( FeriteString *str, FeriteString *what, FeriteString *with )
 * @brief Replace all occurances of a string with another
 * @param FeriteScript *script The script context
 * @param FeriteString *str The string to scane
 * @param FeriteString *what The string to scan for
 * @param FeriteString *with The string to replace 'what' with
 * @return A new string with the replacements
 */
FeriteString *ferite_str_replace( FeriteScript *script, FeriteString *str, FeriteString *what, FeriteString *with )
{
    FeriteString *ptr = NULL;
    char *data = NULL;

    FE_ENTER_FUNCTION;
    if( str != NULL && what != NULL && with != NULL )
    {
        data = ferite_replace_string( str->data, what->data, with->data );
        ptr = ferite_str_new( script, data, strlen(data), FE_CHARSET_DEFAULT );
        ffree( data );
        FE_LEAVE_FUNCTION( ptr );
    }
    ptr = ferite_str_new( script, "", 0, FE_CHARSET_DEFAULT );
    FE_LEAVE_FUNCTION( ptr );
}

FeriteString *ferite_str_escape( FeriteScript *script, FeriteString *str )
{
	FeriteString *ptr = NULL;
	char *data = NULL, *data2 = NULL, *data3 = NULL;;
	
	FE_ENTER_FUNCTION;
    if( str != NULL )
    {
        data = ferite_replace_string( str->data, "\\", "\\\\" );
        data2 = ferite_replace_string( data, "\"", "\\\"" );
		data3 = ferite_replace_string( data2, "$", "\\$" );
        ptr = ferite_str_new( script, data3, strlen(data3), FE_CHARSET_DEFAULT );
        ffree( data );
		ffree( data2 );
		ffree( data3 );
        FE_LEAVE_FUNCTION( ptr );
    }
    ptr = ferite_str_new( script, "", 0, FE_CHARSET_DEFAULT );
	FE_LEAVE_FUNCTION( ptr );
}

/* FIXME
FeriteString *ferite_str_copy( FeriteString *str, size_t from, size_t to )
{
    FeriteString *ptr = NULL;

    FE_ENTER_FUNCTION;
    FE_LEAVE_FUNCTION( ptr );
}

FeriteUnifiedArray *ferite_str_split( FeriteScript *script, FeriteString *str, FeriteString *spliter )
{
    FeriteUnifiedArray *ptr = NULL;
    FE_ENTER_FUNCTION;
    FE_LEAVE_FUNCTION( ptr );
}
*/
/*
 * int ferite_str_ncat( FeriteString *str1, FeriteString *str2, int size );
 * int ferite_str_find( FeriteString *str, char *token );
 * int ferite_str_delete( FeriteString *str, int from, int to );
 * int ferite_str_insert( FeriteString *str, FeriteString *data, int pos );
 */

/**
 * @end
 */
