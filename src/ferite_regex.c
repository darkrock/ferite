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
#include <pcre.h> /* perl compatible regualr expressions */

/**
 * @group Regular Expressions
 * @description Regular expressions play an important role within the ferite language. They can
 *              be used from native code using a very simple interface.
 */

/***************************************************************
 * The way some of this code operates (eg within the matches)  *
 * was semi borrowed from php, this is because the pcre module *
 * that is shiped with php is rather good.                     *
 *                                                             *
 * Those parts of the code borrowed were written by            *
 *   Andrei Zmievski <andrei@ispi.net> and are subject to the  *
 *   PHP 2.02 lisence - http://www.php.net/license/2_02.txt    *
 ***************************************************************/

/* memory functions so pcre uses ferite's memory management */
void *(*old_pcre_malloc)(size_t);
void  (*old_pcre_free)(void *);

void *ferite_regex_malloc( size_t size )
{
    void *ptr;
    FE_ENTER_FUNCTION;
    ptr = fmalloc_ngc( size );
    FE_LEAVE_FUNCTION( ptr );
}

void ferite_regex_free( void *ptr )
{
    FE_ENTER_FUNCTION;
    ffree_ngc( ptr );
    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_init_regex()
{
    FE_ENTER_FUNCTION;
    if( ferite_use_mm_with_pcre == 1 )
    {
        old_pcre_malloc = pcre_malloc;
        old_pcre_free = pcre_free;
        pcre_malloc = ferite_regex_malloc;
        pcre_free = ferite_regex_free;
    }
    FUD(( "REGEX, Using: PCRE %s\n", pcre_version() ));
    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_deinit_regex()
{
    FE_ENTER_FUNCTION;
    if( ferite_use_mm_with_pcre == 1 )
    {
        pcre_malloc = old_pcre_malloc;
        pcre_free = old_pcre_free;
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_create_regex
 * @declaration FeriteRegex *ferite_create_regex()
 * @brief Create a regular expression structure used to store various information for them
 * @return A new FeriteRegex structure
 */
FeriteRegex *ferite_create_regex()
{
    FeriteRegex *ptr = NULL;

    FE_ENTER_FUNCTION;

    ptr = fmalloc_ngc( sizeof( FeriteRegex ) );
    ptr->pattern = NULL;
 //   ptr->type = F_RGX_MATCH;
    ptr->pcre_options = 0;
    ptr->fergx_options = 0;
    ptr->compiled_re = NULL;
    ptr->compile_buf = NULL;
    ptr->swap_buf = NULL;
    ptr->extra_info = NULL;

    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_delete_regex
 * @declaration void ferite_delete_regex( FeriteRegex *rgx )
 * @brief Delete a strucute created using ferite_create_regex()
 * @param FeriteRegex *rgx The regex to delete
 */
void ferite_delete_regex( FeriteRegex *rgx )
{
    FE_ENTER_FUNCTION;

    if( rgx != NULL )
    {
        if( rgx->pattern != NULL )
          ffree_ngc( rgx->pattern );
        if( rgx->compiled_re != NULL )
        {
            if( !ferite_use_mm_with_pcre )
              pcre_free( rgx->compiled_re );
            else
              ffree_ngc( rgx->compiled_re );
        }
        if( rgx->compile_buf != NULL )
          ffree_ngc( rgx->compile_buf );
        if( rgx->swap_buf != NULL )
          ffree_ngc( rgx->swap_buf );
        ffree_ngc( rgx );
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_regex_dup
 * @declaration FeriteRegex *ferite_regex_dup( FeriteRegex *rgx )
 * @brief Duplicate a regular expression structure
 * @param FeriteRegex *rgx The regex to duplicate
 * @return An exact copy of the regexular expression
 */
FeriteRegex *ferite_regex_dup( FeriteRegex *rgx )
{
    FeriteRegex *ptr = NULL;

    FE_ENTER_FUNCTION;
    if( rgx != NULL )
    {
        ptr = fmalloc_ngc(sizeof(FeriteRegex));
        ptr->pattern = fstrdup( rgx->pattern );
        ptr->pcre_options = rgx->pcre_options;
        ptr->fergx_options = rgx->fergx_options;
        ptr->compile_buf = fstrdup( rgx->compile_buf );
        ptr->swap_buf = fstrdup( rgx->swap_buf );
        ptr->extra_info = NULL;
        ptr->compiled_re = NULL;
    }
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_generate_regex
 * @declaration FeriteRegex *ferite_generate_regex( FeriteScript *script, char *pattern, char *swapbuffer, char type, char *flags )
 * @brief A convinience function to create and popular a regex structure
 * @param FeriteScript *script The script
 * @param char *pattern The regular expression
 * @param char *swapbuffer The buffer that is evaluated on a match
 * @param char type The type of the regular expression. s for a swap, m for a match, c for an assignment.
 * @param char *flags The flags the effect the behavior of the regular expression.
 * @return A regular expression structure setup and populated correctly
 */
FeriteRegex *ferite_generate_regex( FeriteScript *script, char *pattern, char *flags )
{
    FeriteRegex *ptr = NULL;
    size_t i = 0;

    FE_ENTER_FUNCTION;

    ptr = ferite_create_regex();
    ptr->pattern = fstrdup( pattern );

   /* go from end to beginning to get the options */
    FUD(( "REGEX postfix's\n" ));
    for( i = 0; i < strlen( flags ); i++ )
    {
        switch( flags[i] )
        {
            /* pcre supported perl'isms */
          case 'x': FUD(( "REGEX: `-> Extended Line\n" ));    ptr->pcre_options |= PCRE_EXTENDED; break;
          case 's': FUD(( "REGEX: `-> Single Line Mode\n" )); ptr->pcre_options |= PCRE_DOTALL; break;
          case 'm': FUD(( "REGEX: `-> Multi Line Mode\n" ));  ptr->pcre_options |= PCRE_MULTILINE; break;
          case 'i': FUD(( "REGEX: `-> Case Insensitive\n" )); ptr->pcre_options |= PCRE_CASELESS; break;
            /* pcre + ferite internal'isms */
          case 'g': FUD(( "REGEX: `-> Global Matching\n" ));  ptr->fergx_options |= F_RGX_GLOBAL; break;
          case 'o': FUD(( "REGEX: `-> Cache Compile\n" ));    ptr->fergx_options |= F_RGX_COMPILE; break;
            /* pcre's specific stuff */
          case 'A': FUD(( "REGEX: `-> Anchored\n" ));         ptr->pcre_options |= PCRE_ANCHORED; break;
          case 'D': FUD(( "REGEX: `-> Dollar @ ABS End\n" )); ptr->pcre_options |= PCRE_DOLLAR_ENDONLY; break;
          default:
            ferite_warning( script, "Regex Option %c - UNKNOWN, ignoring\n", flags[i] );
        }
    }

   /* setup compile buffers and pre-compile the regex */
    ptr->compile_buf = fstrdup( ptr->pattern );
    if( ptr->fergx_options & F_RGX_COMPILE )
    {
        FUD(( "REGEX: Compiling RGX: \"%s\"\n", ptr->compile_buf ));
        ptr->compiled_re = ferite_compile_regex( script, ptr->compile_buf, ptr->pcre_options );
    }
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_compile_regex
 * @declaration void *ferite_compile_regex( FeriteScript *script, char *pattern, int options )
 * @brief Compile a regular expression to make things faster
 * @param FeriteScript *script The script
 * @param char *pattern The pattern to compile
 * @param int options The options that effect how it is compiled
 * @return A pointer to the in memory compiled regular expression
 */
void *ferite_compile_regex( FeriteScript *script, char *pattern, int options )
{
    void       *ptr;
    const char *error;
    int         erroroffset;

    FE_ENTER_FUNCTION;
    ptr = pcre_compile( pattern, options, &error, &erroroffset, NULL );

    if( ptr == NULL )
    {
        ferite_warning( script, "Regex Compilation failed: %s at offset %d\n", error, erroroffset );
        return NULL;
    }

    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @end
 */
