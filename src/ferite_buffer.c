/*
 * Copyright (C) 2001-2007 Stephan Engström
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

#include <stdio.h>
#include <sys/types.h>
#ifndef WIN32
# include <sys/uio.h>
#else
# ifdef VCWIN32
#   include "snprintf.h" /* This is so that we have somethings */
# endif
#endif
#include <stdarg.h>
#include "ferite.h"

/**
 * @group Buffers
 * @description Data management is tricky at the best of times, but keeping it together can be
 *              a nightmare in memory management. Ferite buffers provide a clean mechanism of
 *              quickly storing various different data types together and outputting them in various
 *              different forms.
 */

/**
 * @function ferite_buffer_new
 * @declaration FeriteBuffer *ferite_buffer_new( size_t size )
 * @brief Create a new FeriteBuffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param size_t size The initial size of the buffer
 */
FeriteBuffer *ferite_buffer_new( FeriteScript *script, size_t size )
{
    FeriteBuffer *buf;

    FE_ENTER_FUNCTION;

    if(size < FE_DEFAULT_BUFFER_SIZE)
      size = FE_DEFAULT_BUFFER_SIZE;
    buf = fmalloc(size + sizeof(FeriteBuffer));
    buf->ptr = (char *)buf + sizeof(FeriteBuffer);
	memset(buf->ptr, 0, size);
    buf->size = size;
    buf->count = 0;
    buf->next = NULL;
    buf->current = buf;
    FE_LEAVE_FUNCTION( buf );
}

/** 
 * @function ferite_buffer_add
 * @declaration void ferite_buffer_add( FeriteBuffer *buf, void *ptr, size_t size )
 * @brief Add data to a buffer
* @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to use
 * @param void *ptr A pointer to the data
 * @param size_t size The amount of data to add - this is used so buffers can handle binary data
 */
void ferite_buffer_add( FeriteScript *script, FeriteBuffer *buf, void *ptr, size_t size )
{
	char *target = NULL;
    FE_ENTER_FUNCTION;
    target = ferite_buffer_alloc( script, buf, size );
    memcpy( target, (char*)ptr, size );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_buffer_alloc
 * @declaration void *ferite_buffer_alloc( FeriteBuffer *buf, size_t size )
 * @brief Alloc a slab of memory and add it to the buffer if needed
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to add the memory to
 * @param size_t size The size of the slab to add
 */
void *ferite_buffer_alloc( FeriteScript *script, FeriteBuffer *buf, size_t size )
{
    FeriteBuffer *current = buf->current;
    char *ptr;
    FE_ENTER_FUNCTION;
    if( size > ( current->size - current->count ))
    {
        buf->current = current->next = ferite_buffer_new( script, size );
        current = buf->current;
    }
    ptr = (char *)current->ptr + current->count;
    current->count += size;
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_buffer_add_char
 * @declaration void ferite_buffer_add_char( FeriteBuffer *buf, int character )
 * @brief Add a character onto the end of a buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to add onto
 * @param int character The character to add
 */
void ferite_buffer_add_char( FeriteScript *script, FeriteBuffer *buf, int character )
{
    FeriteBuffer *current = buf->current;
    char *ptr;
    FE_ENTER_FUNCTION;
    if( (current->size - current->count) == 0 )
    {
        buf->current = current->next = ferite_buffer_new( script, 0 );
        current = buf->current;
    }
    ptr = current->ptr;
    ptr[current->count] = character;
    current->count++;
    FE_LEAVE_FUNCTION( NOWT );
}
void ferite_buffer_del_char( FeriteScript *script, FeriteBuffer *buf )
{
    FeriteBuffer *current = buf->current;
    char *ptr;
    FE_ENTER_FUNCTION;
    ptr[current->count] = '\0';
    current->count--;
    FE_LEAVE_FUNCTION( NOWT );
}
/**
 * @function ferite_buffer_add_fstr
 * @declaration void ferite_buffer_add_fstr( FeriteBuffer *buf, FeriteString *str )
 * @brief Add a FeriteString onto the end of a buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to add onto
 * @param FeriteString *str The String to add
 */
void ferite_buffer_add_fstr( FeriteScript *script, FeriteBuffer *buf, FeriteString *str )
{
    FE_ENTER_FUNCTION;
    ferite_buffer_add( script, buf, str->data, str->length );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_buffer_add_str
 * @declaration void ferite_buffer_add_str( FeriteBuffer *buf, char *str )
 * @brief Add a null terminated C string onto the buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to add onto
 * @param char *str null terminated string to use
 */
void ferite_buffer_add_str( FeriteScript *script, FeriteBuffer *buf, char *str )
{
    FE_ENTER_FUNCTION;
    ferite_buffer_add( script, buf, str, strlen(str) );
    FE_LEAVE_FUNCTION( NOWT );
}
/**
 * @function ferite_buffer_add_long
 * @declaration void ferite_buffer_add_long( FeriteBuffer *buf, long data )
 * @brief Add a long onto the end of a buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The Buffer to add onto
 * @param long data The long value to use
 */
void ferite_buffer_add_long( FeriteScript *script, FeriteBuffer *buf, long data )
{
    char str[100];
    int i;
    FE_ENTER_FUNCTION;
    i = sprintf(str,"%ld",data);
    ferite_buffer_add( script, buf, str, i );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_buffer_add_double
 * @declaration void ferite_buffer_add_double( FeriteBuffer *buf, double data )
 * @brief Add a double onto the end of a buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The Buffer to add onto
 * @param double data The double value to use
 */
void ferite_buffer_add_double( FeriteScript *script, FeriteBuffer *buf, double data )
{
    char str[100];
    int i;
    FE_ENTER_FUNCTION;
    i = sprintf(str,"%f",data);
    ferite_buffer_add( script, buf, str, i );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_buffer_add_variable
 * @declaration void ferite_buffer_add_variable( FeriteScript *script, FeriteBuffer *buf, FeriteVariable *v )
 * @brief Add a ferite variable's value onto the end of a buffer
 * @param FeriteScript *script The script the variable belongs to
 * @param FeriteBuffer *buf The Buffer to add onto
 * @param FeriteVariable *v The variable to use
 */
void ferite_buffer_add_variable( FeriteScript *script, FeriteBuffer *buf, FeriteVariable *v )
{
    FeriteString *s = NULL;

    FE_ENTER_FUNCTION;
    s = ferite_variable_to_str(script,v,0);
    ferite_buffer_add(script, buf,s->data,s->length);
    ferite_str_destroy( script, s );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_buffer_vprintf
 * @declaration int ferite_buffer_vprintf( FeriteBuffer *buf, char *fmt, va_list *args )
 * @brief Same as a standard C vprintf except onto a ferite buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to use
 * @param char *fmt The format to use - please see the vprintf man pages for more information
 * @param va_list *args The arguments to use with the format
 * @return The length of the entire format
 */
int ferite_buffer_vprintf( FeriteScript *script, FeriteBuffer *buf, char *fmt, va_list *args )
{
    int len = 0;
#ifdef WIN32
	int real_len = 0;
#endif
    char str[2], *ptr = NULL;
    va_list ap;

    FE_ENTER_FUNCTION;
    memcpy((char*) &ap, (char*) args, sizeof(va_list));
#ifndef WIN32
    len = vsnprintf( str, 1, fmt, ap );
    ptr = ferite_buffer_alloc( script, buf, len + 1);
    memcpy((char*) &ap, (char*) args, sizeof(va_list));
    vsnprintf( ptr, len + 1, fmt, ap );
    buf->current->count--;
#else
    len = 1024;
    ptr = fmalloc( len );
    memset( ptr, '\0', len );
    real_len = vsnprintf( ptr, len, fmt, ap );
    ferite_buffer_add_str( script, buf, ptr );	
    ffree( ptr );
#endif
    FE_LEAVE_FUNCTION( len );
}

/**
 * @function ferite_buffer_printf
 * @declaration int ferite_buffer_printf( FeriteBuffer *buf, char *fmt, ... )
 * @brief Same as a standard C printf except onto a ferite buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to use
 * @param char *fmt The format to use - please see the printf man pages for more information
 * @param . .. The arguments to use with the format
 * @return The length of the entire format
 */
int ferite_buffer_printf( FeriteScript *script, FeriteBuffer *buf, char *fmt, ... )
{
    int len = 0;
    va_list ap;

    FE_ENTER_FUNCTION;

    va_start(ap, fmt);
    len = ferite_buffer_vprintf( script, buf, fmt, &ap );
    va_end(ap);
    FE_LEAVE_FUNCTION( len );
}

/**
 * @function ferite_buffer_merge
 * @declaration void ferite_buffer_merge( FeriteBuffer *dest, FeriteBuffer *source )
 * @brief Take two buffers and combine them into one
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *dest The buffer to merge into
 * @param FeriteBuffer *source The buffer to merge
 * @description Please remember, after merging buffers, you must forget about
 * the source buffer. You do not need to delete the buffer.
 */
void ferite_buffer_merge( FeriteScript *script, FeriteBuffer *dest, FeriteBuffer *source )
{
    FeriteBuffer *buf = NULL;
    buf = dest;
    while( buf->next )
      buf = buf->next;
    buf->next = dest;

    source->current = dest->current;
    source->count = dest->count;
    source->size = dest->size;
}

/**
 * @function ferite_buffer_delete
 * @declaration void ferite_buffer_delete( FeriteBuffer *buf )
 * @brief Clean up a buffer and free all memory it uses
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to delete
 */
void ferite_buffer_delete( FeriteScript *script, FeriteBuffer *buf )
{
    FeriteBuffer *tmp;
    FE_ENTER_FUNCTION;
    while( buf )
    {
        tmp = buf;
        buf = buf->next;
        ffree(tmp);
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/* How to extract a buffer */

/**
 * @function ferite_buffer_get_size
 * @declaration int ferite_buffer_get_size( FeriteBuffer *buf )
 * @brief Get the total size of the data contained within a buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to use
 * @return The length
 */
size_t ferite_buffer_get_size( FeriteScript *script, FeriteBuffer *buf )
{
    size_t count = 0;
    FE_ENTER_FUNCTION;
    while( buf )
    {
        count += buf->count;
        buf = buf->next;
    }
    FE_LEAVE_FUNCTION( count );
}

/**
 * @function ferite_buffer_get
 * @declaration void *ferite_buffer_get( FeriteBuffer *buf, int *len )
 * @brief Get the entire contents of the bugffer in a contiguous memory block
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer whose contents is wanted
 * @param int *len A pointer to an int where the length of the data can be put
 * @description You must call ffree() on the memory returned otherwise leaks will occur
 * @return A pointer to a memory block containing len number of bytes of data
 */
void *ferite_buffer_get( FeriteScript *script, FeriteBuffer *buf, int *len )
{
    size_t count;
    char *p, *result;

    FE_ENTER_FUNCTION;

    count = ferite_buffer_get_size( script, buf );
    result = p = fmalloc( count + 2 );

    while( buf )
    {
        memcpy(result,buf->ptr,buf->count);
        result += buf->count;
        buf = buf->next;
    }
    *result = '\0';
    if( len != NULL )
      *len = count;
    FE_LEAVE_FUNCTION( p );
}

/**
 * @function ferite_buffer_to_fd
 * @declaration int ferite_buffer_to_fd( FeriteBuffer *buf, int fd )
 * @brief Write a buffer out to a system file descriptor
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to write
 * @param int fd The file descriptor to write to
 * @return The number of bytes written to the descriptor
 */
int ferite_buffer_to_fd( FeriteScript *script, FeriteBuffer *buf, int fd )
{
#ifndef WIN32
    struct iovec *vec;
#endif
    int i = 0, count = 0;

    FE_ENTER_FUNCTION;
#ifndef WIN32
    count = ferite_buffer_get_size( script, buf );
    vec = fmalloc( sizeof(struct iovec) * count );
    while( buf )
    {
        vec[i].iov_base = buf->ptr;
        vec[i++].iov_len = buf->count;
        buf = buf->next;
    }
    writev( fd, vec, i );
#endif
    FE_LEAVE_FUNCTION( count );
}

/**
 * @function ferite_buffer_to_file
 * @declaration int ferite_buffer_to_file( FeriteBuffer *buf, FILE *f )
 * @brief Write a buffer out to a file pointed to by a C FILE stream
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to write
 * @param FILE *f The file to write to
 * @return The number of bytes written to the stream
 */
int ferite_buffer_to_file( FeriteScript *script, FeriteBuffer *buf, FILE *f )
{
    size_t count=0;

    FE_ENTER_FUNCTION;

    while( buf )
    {
        count += fwrite(buf->ptr, 1, buf->count, f);
        buf = buf->next;
    }
    FE_LEAVE_FUNCTION( (int)count );
}

/**
 * @function ferite_buffer_to_var
 * @declaration FeriteVariable *ferite_buffer_to_var( FeriteScript *script, FeriteBuffer *buf )
 * @brief Create a ferite string variable based upon the data within the buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteScript *script The script in which to create the variable
 * @param FeriteBuffer *buf The buffer to use
 * @return A FeriteVariable with the contents of the buffer
 */
FeriteVariable *ferite_buffer_to_var( FeriteScript *script, FeriteBuffer *buf )
{
    FeriteVariable *v = NULL;
    FeriteString *str = NULL;

    FE_ENTER_FUNCTION;
    str = ferite_buffer_to_str( script, buf );
    v = ferite_create_string_variable( script, "buffer", str, FE_STATIC );
    ferite_str_destroy( script, str );
    FE_LEAVE_FUNCTION( v );
}

/**
 * @function ferite_buffer_to_str
 * @declaration FeriteString *ferite_buffer_to_str( FeriteBuffer *buf )
 * @brief Create a FeriteString containing the contents of a buffer
 * @param FeriteScript *script The script context, NULL otherwise
 * @param FeriteBuffer *buf The buffer to use
 * @return A FeriteString with the contents of the buffer
 */
FeriteString *ferite_buffer_to_str( FeriteScript *script, FeriteBuffer *buf )
{
    int len;
    char *ptr;
    FeriteString *str;
    FE_ENTER_FUNCTION;
    ptr = ferite_buffer_get( script, buf , &len );
    str = fmalloc( sizeof(FeriteString) );
    str->data = ptr;
    str->length = len;
    str->encoding = FE_CHARSET_DEFAULT;
    FE_LEAVE_FUNCTION( str );
}

/* These are not used below, so, we won't document them until stephan fixes them ;) */

FeriteVariable *ferite_sprintf(FeriteScript *script, char *format, FeriteVariable **params )
{
    FeriteBuffer *buf;
    FeriteVariable *v;

    FE_ENTER_FUNCTION;
    buf = ferite_buffer_new(script, 0);
    ferite_format( script, buf, format, params );

    v = ferite_variable_alloc( NULL, NULL, 0 );
    F_VAR_TYPE(v) = F_VAR_STR;
	MARK_VARIABLENAME_AS_STATIC( v );
    VAS(v) = ferite_buffer_get( script, buf , NULL);
    FE_LEAVE_FUNCTION( v );
}

int ferite_format( FeriteScript *script, FeriteBuffer *buf, char *format, FeriteVariable **params )
{
    int current = 0;
	size_t len = strlen(format), i = 0;
    FeriteVariable *arg;

    FE_ENTER_FUNCTION;
    for(i=0 ; i < len ; i++)
    {
        arg = params[current];
        if( format[i] == '%' )
        {
            i++;
            if( arg == NULL )
            {
                ferite_buffer_add(script, buf,"(null)",6);
                continue;
            }
            switch( format[i] )
            {
              case '%':
                ferite_buffer_add_char(script, buf,'%');
                break;
              case 'n':
                switch( F_VAR_TYPE(arg) )
                {
                  case F_VAR_LONG:
                    ferite_buffer_add_long(script, buf, VAI(arg));
                    break;
                  case F_VAR_DOUBLE:
                    ferite_buffer_add_double(script, buf, VAF(arg));
                    break;
                  default:
               /* Error or not, that is the question */
                    break;
                }
                break;
              case 'd':
                ferite_buffer_add_long(script, buf, ferite_number_as_long(NULL, arg));
                break;
              case 'f':
                ferite_buffer_add_double(script, buf, ferite_number_as_double(NULL, arg));
                break;
              case 's':
                ferite_buffer_add(script, buf, FE_STR2PTR(arg), FE_STRLEN(arg));
                break;
              case 'v':
              default:
            /* FIXME: ferite_buffer_add_variable(script, buf, arg);*/
                break;
            }
            current++;
        }
    }
    return 0;
}

/**
 * @end
 */
