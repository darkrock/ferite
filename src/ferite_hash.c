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
 * @group Hashes
 * @description How could any program survive without hashing? The hash functions
 *              below provide a good hashing system implementation which is used
 *              throughout the ferite engine. They can be used to store anything
 *              and form the basis for namespaces.
 */

extern int ferite_pow_lookup[];

/**
 * @function ferite_hash_gen
 * @declaration unsigned int ferite_hash_gen( char *key, size_t keylen )
 * @brief Generate a hash value given a key and a hash length
 * @param char *key The value to hash
 * @param size_t keylen The size of the hash
 * @return An unsigned integer value representing the hash value
 */
unsigned int ferite_hash_gen( char *key, size_t keylen )
{
    size_t i;
    unsigned int hashval = 0;

    FE_ENTER_FUNCTION;
    for( i = 0; i < keylen; i++ )
      hashval = *key++ + 31 * hashval;
    FE_LEAVE_FUNCTION( hashval );
}

FeriteHashBucket *ferite_create_hash_bucket( FeriteScript *script, char *key, void *value )
{
    FeriteHashBucket *ptr;
    size_t len;

    FE_ENTER_FUNCTION;
    len = strlen( key );
    ptr = fmalloc( sizeof( FeriteHashBucket ) );
	ptr->id = fstrdup(key);
    ptr->hashval = ferite_hash_gen( key, len );
    ptr->data = value;
    ptr->next = NULL;
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_create_hash
 * @declaration FeriteHash *ferite_create_hash( FeriteScript *script, int size )
 * @brief Create a hash table
 * @param FeriteScript *script The script
 * @param int size The initial size of the hash
 * @return A new hash table
 */
FeriteHash *ferite_create_hash( FeriteScript *script, int size )
{
    FeriteHash *ptr;
    int i = 4; /* We don't want to small bucketsizes */

    FE_ENTER_FUNCTION;
    while( size > ferite_pow_lookup[i] )
      i++;
    size = ferite_pow_lookup[i];
    ptr = fcalloc( 1, sizeof( FeriteHash ) );
    ptr->size = size;
    ptr->hash = fcalloc( 1, (size * sizeof(FeriteHashBucket*)) );
	ptr->count = 0;
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_delete_hash
 * @declaration void ferite_delete_hash( FeriteScript *script, FeriteHash *hash, void (*cb)(FeriteScript*,void *data) )
 * @brief Delete a hash table
 * @param FeriteScript *script The script
 * @param FeriteHash *hash The hash table to delete
 * @param void cb The function to call to delete the hash's contents
 */
void ferite_delete_hash( FeriteScript *script, FeriteHash *hash, void (*cb)(FeriteScript*,void *data) )
{
    int i = 0;
    FeriteHashBucket *bucket = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( hash != NULL );
    if( hash->hash != NULL )
    {
        for( i = 0; i < hash->size; i++ ) {
			int chain_count = 0;
			
			bucket = hash->hash[i];
			while( bucket ) {
                FeriteHashBucket *current = bucket;

				bucket = current->next;
				chain_count++;
				FUD(("[%p] Index: %d, Chain: %d, Data: %p\n", hash, i, chain_count, current));

				if( cb ) {
					(cb)( script, current->data );
				}
				
				current->next = NULL;
				ffree( current->id );
                ffree( current );
			}
			hash->hash[i] = NULL;
        }
    }
	ffree( hash->hash );
    ffree( hash );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_process_hash
 * @declaration void ferite_process_hash( FeriteScript *script, FeriteHash *hash, void (*cb)(FeriteScript*,void *data) )
 * @brief Process a hash table by calling a function on each element
 * @param FeriteScript *script The script
 * @param FeriteHash *hash The hash table to process
 * @param void cb The function to call to process the hash's contents
 */
void ferite_process_hash( FeriteScript *script, FeriteHash *hash, void (*cb)(FeriteScript*,void *data,char *key) )
{
    int i;
    FeriteHashBucket *bucket, *next;

    FE_ENTER_FUNCTION;
    FE_ASSERT( hash != NULL );
    for( i = 0; i < hash->size; i++ )
    {
        for( bucket = hash->hash[i]; bucket; bucket = next )
        {
            next = bucket->next;
            if( cb )
              (cb)( script, bucket->data, bucket->id );
        }
    }
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_hash_add
 * @declaration void ferite_hash_add( FeriteScript *script, FeriteHash *hash, char *key, void *data )
 * @brief Add data to the hash
 * @param FeriteScript *script The script
 * @param FeriteHash *hash The hash to insert the data into
 * @param char *key The key on which to hash the data
 * @param void *data A pointer to some data to keep
 */
void ferite_hash_add( FeriteScript *script, FeriteHash *hash, char *key, void *data )
{
    int loc = 0;
    unsigned int hashval = 0;
    FeriteHashBucket *ptr = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( hash != NULL  && key != NULL );

    hashval = ferite_hash_gen( key, strlen( key ) );
    loc = hashval & hash->size - 1;
    FUD(( "HASH: Adding %s to key %d\n", key, loc ));

    ptr = ferite_create_hash_bucket( script, key, data );
    ptr->next = hash->hash[loc];
    hash->hash[loc] = ptr;
	hash->count++;
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_hash_get
 * @declaration void *ferite_hash_get( FeriteScript *script, FeriteHash *hash, char *key )
 * @brief Get the data at a specified key
 * @param FeriteScript *script The script to pass around
 * @param FeriteHash *hash The hash to get the data from
 * @param char *key The key to obtain the data for
 * @return The pointer to the data or NULL otherwise
 */
void *ferite_hash_get( FeriteScript *script, FeriteHash *hash, char *key )
{
    int loc = 0;
    unsigned int hashval = 0;
    FeriteHashBucket *ptr = NULL;

    FE_ENTER_FUNCTION;

    FE_ASSERT( hash != NULL  && key != NULL );

    hashval = ferite_hash_gen( key, strlen( key ) );
    loc = hashval & hash->size - 1;
	if( hash->hash ) {
	    for( ptr = hash->hash[loc]; ptr != NULL; ptr = ptr->next )
	    {
	        FUD(("HASH: Testing \"%s\" for a match on \"%s\"\n", ptr->id, key ));
	        if( ptr->hashval == hashval && strcmp( key, ptr->id ) == 0 )
	        {
	            FUD(("HASH: located data\n"));
	            FE_LEAVE_FUNCTION( ptr->data );
	        }
	    }
	}
    FUD(("HASH: Could not find data, returning NULL\n"));
    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_create_iterator
 * @declaration FeriteIterator *ferite_create_iterator( FeriteScript *script )
 * @brief Create an iterator to iterate over a hash
 * @param FeriteScript *script The script
 * @return A pointer to an iterator
 */
FeriteIterator *ferite_create_iterator( FeriteScript *script )
{
    FeriteIterator *iter = NULL;

    FE_ENTER_FUNCTION;
    iter = fmalloc(sizeof(FeriteIterator));
    iter->curbucket = NULL;
    iter->curindex = 0;
    FE_LEAVE_FUNCTION( iter );
}

/**
 * @function ferite_hash_walk
 * @declaration FeriteHashBucket *ferite_hash_walk(FeriteScript *script, FeriteHash *hash, FeriteIterator *iter)
 * @brief Walk the hash
 * @param FeriteScript *script The script
 * @param FeriteHash *hash The hash to walk over
 * @param FeriteIterator *iter The iterator to use
 * @description Each time this function is called it will return the next data value
 * @return The next hash bucket, NULL if there are no more
 */
FeriteHashBucket *ferite_hash_walk(FeriteScript *script, FeriteHash *hash, FeriteIterator *iter)
{
    int i;

    FE_ENTER_FUNCTION;

    FE_ASSERT( hash != NULL  && iter != NULL );

    if(iter->curbucket == NULL)
    {
        for(i = 0; i < hash->size; i++)
        {
            if(hash->hash[i] != NULL)
            {
                iter->curbucket = hash->hash[i];
                iter->curindex = i;
                FE_LEAVE_FUNCTION( iter->curbucket );
            }
        }
        FE_LEAVE_FUNCTION( iter->curbucket );
    }

    if(iter->curbucket->next != NULL)
    {
        iter->curbucket = iter->curbucket->next;
        FE_LEAVE_FUNCTION( iter->curbucket );
    }
    else
    {
        iter->curindex++;
    }

    for(i = iter->curindex; i < hash->size; i++)
    {
        if(hash->hash[i] != NULL)
        {
            iter->curbucket = hash->hash[i];
            iter->curindex = i;
            FE_LEAVE_FUNCTION( iter->curbucket );
        }
    }

    FE_LEAVE_FUNCTION( NULL );
}

/**
 * @function ferite_hash_update
 * @declaration void ferite_hash_update( FeriteScript *script, FeriteHash *hash, char *key, char *data )
 * @brief Update a value at the specified hash key
 * @param FeriteScript *script The script
 * @param FeriteHash *hash The has to update
 * @param char *key The key to the bucket to update
 * @param char *data Pointer to the data
 */
void ferite_hash_update( FeriteScript *script, FeriteHash *hash, char *key, void *data )
{
    int loc = 0;
    unsigned int hashval = 0;
    FeriteHashBucket *ptr = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( hash != NULL  && key != NULL );

    hashval = ferite_hash_gen( key, strlen( key ) );
    loc = hashval & hash->size - 1;
    for( ptr = hash->hash[loc]; ptr != NULL; ptr = ptr->next )
    {
        FUD(("HASH: Testing \"%s\" for a match on \"%s\"\n", ptr->id, key ));
        if( ptr->hashval == hashval && strcmp( key, ptr->id ) == 0 )
        {
            FUD(("HASH: located data\n"));
            ptr->data = data;
            FE_LEAVE_FUNCTION(NOWT);
        }
    }
    FUD(("HASH: Could not find data, returning NULL\n"));
    FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_hash_delete
 * @declaration void ferite_hash_delete( FeriteScript *script, FeriteHash *hash, char *key )
 * @brief Delete a key from the hash
 * @param FeriteScript *script The script to delete
 * @param FeriteHash *hash The hash to delete from 
 * @param char *key The key to delete
 */
void ferite_hash_delete( FeriteScript *script, FeriteHash *hash, char *key )
{
    int loc = 0;
    unsigned int hashval = 0;
    FeriteHashBucket *ptr = NULL;
    FeriteHashBucket *optr = NULL;

    FE_ENTER_FUNCTION;
    FE_ASSERT( hash != NULL && key != NULL );

    hashval = ferite_hash_gen( key, strlen( key ) );
    loc = hashval & hash->size - 1;
    for( ptr = hash->hash[loc]; ptr != NULL; optr = ptr, ptr = ptr->next )
    {
        FUD(("HASH: Testing \"%s\" for a match on \"%s\"\n", ptr->id, key ));
        if( ptr->hashval == hashval && strcmp( key, ptr->id ) == 0 )
        {
            if( hash->hash[loc] == ptr )
                hash->hash[loc] = ptr->next; /* remove head from array */
            else
                optr->next = ptr->next; /* remove buxket from list */
            FUD(("HASH: located data\n"));
			ffree( ptr->id );
            ffree( ptr );
			hash->count--;
            FE_LEAVE_FUNCTION( NOWT );
        }
    }
    FUD(("HASH: Could not find data, returning NULL\n"));
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_hash_dup
 * @declaration FeriteHash *ferite_hash_dup( FeriteScript *script, FeriteHash *hash, void *(*ddup)( FeriteScript*,void *data,void *data2 ), void *data2 )
 * @brief Duplicate a hash
 * @param FeriteScript *script The script
 * @param FeriteHash *hash The hash table to duplicate
 * @param void *ddup The function that gets called with to duplicate a buckets data
 * @return A new hash that is an exact copy of the original
 */
FeriteHash *ferite_hash_dup( FeriteScript *script, FeriteHash *hash, void *(*ddup)( FeriteScript*,void *data,void *data2 ), void *data2 )
{
    FeriteHash *ptr;
    FeriteHashBucket *bptr, *nbptr;
    int i;

    FE_ENTER_FUNCTION;
    FE_ASSERT( hash != NULL );
    ptr = ferite_create_hash( script, hash->size );
    for( i = 0; i < hash->size; i++ )
    {
        bptr = hash->hash[i];
        while( bptr )
        {
            nbptr = ferite_create_hash_bucket( script, bptr->id, (ddup)(script,bptr->data,data2) );
            nbptr->next = ptr->hash[i];
            ptr->hash[i] = nbptr;
            bptr = bptr->next;
        }
    }
	ptr->count = hash->count;
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_hash_grow
 * @declaration FeriteHash *ferite_hash_grow( FeriteScript *script, FeriteHash *hash )
 * @brief Grow the hash table to be faster
 * @param FeriteScript *script The script
 * @param FeriteHash *hash The hash to modify
 * @return The pointer to the new hash (this may be the same as the passed in hash)
 */
FeriteHash *ferite_hash_grow( FeriteScript *script, FeriteHash *hash )
{
    FeriteHash *newhash;
    FeriteHashBucket *ptr, *next;
    int size, i, loc;

    FE_ENTER_FUNCTION;
    if( (size = hash->size * 4) > 8192 )
      size = 8192;
    if( hash->size >= 8192 )
      FE_LEAVE_FUNCTION( hash );
    newhash = fcalloc( 1, sizeof( FeriteHash ) );
    newhash->hash = fcalloc( 1, (size * sizeof( FeriteHashBucket )) );
    newhash->size = size;

    for( i = 0; i < hash->size; i++ )
    {
        for( ptr = hash->hash[i]; ptr != NULL; ptr = next )
        {
            next = ptr->next;
            loc = ptr->hashval & newhash->size - 1;
            ptr->next = newhash->hash[loc];
            newhash->hash[loc] = ptr;
        }
    }
	ffree( hash->hash );
    ffree( hash );
    FE_LEAVE_FUNCTION( newhash );
}

/**
 * @function ferite_hash_print
 * @declaration void ferite_hash_print( FeriteScript *script, FeriteHash *hash )
 * @brief Print out the hash table if debug mode is turned on
 * @param FeriteScript *script The script to delete
 * @param FeriteHash *hash The has to print out
 * @description This simply prints out what keys are in the hash for debug purposes
 */
void ferite_hash_print( FeriteScript *script, FeriteHash *hash )
{
    FeriteHashBucket *bptr = NULL;
    int i = 0;

    FE_ENTER_FUNCTION;
    FE_ASSERT( hash != NULL );
	FUD(("hash table-------------------------------\nsize: %d; count: %d\n", hash->size, hash->count));
    for( i = 0; i < hash->size; i++ )
    {
        FUD(("hash->hash[%d]:", i));
        if( hash->hash[i] != NULL )
        {
            for( bptr = hash->hash[i]; bptr != NULL; bptr = bptr->next )
            {
                FUD(("-->'%s'", bptr->id ));
            }
            FUD(("-->NULL\n"));
        }
        else
        {
            FUD(("-->NULL\n"));
        }
    }
    FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @end
 */
