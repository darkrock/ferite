/*
 * Copyright (C) 2000-2007 Chris Ross
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
#include <math.h>

#define VAA( var ) (FeriteAMTArray*)(var->data.pval)

typedef struct _ferite_amt_array_entry FeriteAMTArrayEntry;
typedef struct _ferite_amt_array FeriteAMTArray;

struct _ferite_amt_array_entry {
	FeriteVariable *variable;
	char *key;
};
struct _ferite_amt_array {
	FeriteAMT *_array;
	FeriteAMT *_hash;
	
	int upperLimit;
	int lowerLimit;
};

FeriteAMTArray *ferite_amtarray_create( FeriteScript *script )
{
	FeriteAMTArray *array = fmalloc(sizeof(FeriteAMTArray));
	array->_array = ferite_amt_create( script, FeriteAMT_Low );
	array->_hash = ferite_amt_create( script, FeriteAMT_High );
	array->upperLimit = array->lowerLimit = (INT_MAX / 2);
	return array;
}

void ferite_amtarray_destroy( FeriteScript *script, FeriteAMTArray *array )
{

}

void ferite_amtarray_add( FeriteScript *script, FeriteAMTArray *array, FeriteVariable *var, char *id, int pos )
{
	FeriteAMTArrayEntry *entry = fmalloc(sizeof(FeriteAMTArrayEntry));
	unsigned long index = 0;
	
	if( pos >= 0 || pos == FE_ARRAY_ADD_AT_END ) {
		index = array->upperLimit++;
	} else {
		index = array->lowerLimit--;
	}
	
	entry->variable = var;
	entry->key = NULL;
	if( id ) {
		entry->key = fstrdup(id);
		ferite_hamt_set( script, array->_hash, entry->key, (void*)index );
	}
	
	ferite_amt_set( script, array->_array, index, entry );
}
#define REAL_INDEX( ARRAY, INDEX, REALINDEX ) do { \
		if( INDEX < 0 ) \
			REALINDEX = (ARRAY->upperLimit + INDEX); \
		else \
			REALINDEX = (ARRAY->lowerLimit + INDEX); \
		if( REALINDEX <= ARRAY->lowerLimit || REALINDEX >= ARRAY->upperLimit ) { \
			ferite_error( script, 0, "Index %d is out of array bounds 0 to %u!\n", INDEX, (ARRAY->upperLimit - ARRAY->lowerLimit) ); \
			return NULL; \
		} \
	} while(0)

FeriteVariable *ferite_amtarray_get_index( FeriteScript *script, FeriteAMTArray *array, int index )
{    
	unsigned int real_index = index;
	FeriteAMTArrayEntry *entry = NULL;
	
	REAL_INDEX( array, index, real_index );	
	entry = ferite_amt_get( script, array->_array, real_index );
	if( entry ) {
		return entry->variable;
	}
	return NULL;
}

FeriteVariable *ferite_amtarray_get_hash( FeriteScript *script, FeriteAMTArray *array, char *id )
{
	FeriteAMTArrayEntry *entry = ferite_amt_get( script, array->_array, (unsigned long)(ferite_hamt_get(script, array->_hash, id)) );
	if( entry ) {
		return entry->variable;
	}
	return NULL;
}

FeriteVariable *ferite_amtarray_get_exceptions( FeriteScript *script, FeriteAMTArray *array, FeriteVariable *index, int want_exceptions )
{
	FeriteVariable *rval = NULL;
	
	FE_ENTER_FUNCTION;
	switch(F_VAR_TYPE(index)) {
		case F_VAR_LONG:
			rval = ferite_amtarray_get_index( script, array, VAI(index) );
			if( want_exceptions && rval == NULL ) {
				ferite_error( script, 0, "Index does not exist '%d' in array.\n", VAI(index) );
				FE_LEAVE_FUNCTION( NULL );
			}
			break;
		case F_VAR_DOUBLE:
			rval = ferite_amtarray_get_index( script, array, (int)floor(VAF(index)) );
			if( want_exceptions && rval == NULL ) {
				ferite_error( script, 0, "Index does not exist '%d' in array.\n", (int)floor(VAF(index)) );
				FE_LEAVE_FUNCTION( NULL );
			}
			break;
		case F_VAR_STR:
			rval = ferite_amtarray_get_hash( script, array, FE_STR2PTR(index));
			if( want_exceptions && rval == NULL ) {
				ferite_error( script, 0, "Invalid index: no such key '%s' in array.\n", FE_STR2PTR(index) );
				FE_LEAVE_FUNCTION( NULL );
			}
			break;
		case F_VAR_OBJ: {
			FeriteVariable *hash_value = NULL, *return_value = NULL;
			FeriteFunction *hash = ferite_object_get_function_for_params( script, VAO(index), "hash", NULL );	    
			hash_value = ferite_call_function( script, VAO(index), NULL, hash, NULL );

			if( F_VAR_TYPE(hash_value) != F_VAR_STR ) {
				if( want_exceptions )
					ferite_error( script, 0, "Invalid index: unable to get string key from object to access array.\n" );
				FE_LEAVE_FUNCTION(NULL);
			}
			return_value = ferite_amtarray_get_hash( script, array, FE_STR2PTR(hash_value) );
			if( want_exceptions && return_value == NULL ) {
				ferite_error( script, 0, "Invalid index: no such key '%s' (otained from object hash) in array.\n", FE_STR2PTR(hash_value) );
			}
			ferite_variable_destroy( script, hash_value );
			FE_LEAVE_FUNCTION( return_value );	    
		}
	}
	FE_LEAVE_FUNCTION( rval );
}
FeriteVariable *ferite_amtarray_get( FeriteScript *script, FeriteAMTArray *array, FeriteVariable *index )
{
	return ferite_amtarray_get_exceptions( script, array, index, FE_TRUE );
}

FeriteVariable *ferite_amtarray_set(FeriteScript *script, FeriteAMTArray *array, FeriteVariable *index, FeriteVariable *value )
{
	FeriteVariable *ptr = NULL;
	FeriteVariable *value_copy = NULL;

	FE_ENTER_FUNCTION;
	if(F_VAR_TYPE(index) == F_VAR_VOID && FE_VAR_IS_PLACEHOLDER( index ) )  /* a[] = b */ {
		value_copy = ferite_duplicate_variable( script, value, NULL );
		ferite_amtarray_add( script, array, value_copy, NULL, FE_ARRAY_ADD_AT_END );
	} else {
		ptr = ferite_amtarray_get_exceptions( script, array, index, FE_FALSE );
		if( ptr == NULL && F_VAR_TYPE(index) == F_VAR_STR ) /* Set a variable that doesn't exist */ {
			value_copy = ferite_duplicate_variable( script, value, NULL );
			ferite_amtarray_add( script, array, value_copy, FE_STR2PTR(index), FE_ARRAY_ADD_AT_END );
		} else 	{
			if( ferite_types_are_equal( script, F_VAR_TYPE(ptr), F_VAR_TYPE(value) ) )
				ferite_variable_fast_assign( script, ptr, value );
			else {
				FeriteString *index_str = ferite_variable_to_str( script, index, FE_TRUE );
				ferite_error( script, 0, "Trying to set variable of type %s at index %s when a variable of type %s already exists\n", 
											ferite_variable_id_to_str(script, F_VAR_TYPE(value)), index_str->data, ferite_variable_id_to_str(script, F_VAR_TYPE(ptr)) );
				ferite_str_destroy( script, index_str );
				ptr = NULL;
			}
		}
	}
	FE_LEAVE_FUNCTION( value );
}

FeriteVariable *ferite_amtarray_delete_index( FeriteScript *script, FeriteAMTArray *array, int index )
{
	return NULL;
}
FeriteVariable *ferite_amtarray_delete_hash( FeriteScript *script, FeriteAMTArray *array, char *id )
{
	return NULL;
}

void ferite_amtarray_push( FeriteScript *script, FeriteAMTArray *array, FeriteVariable *var )
{
	ferite_amtarray_add( script, array, var, NULL, FE_ARRAY_ADD_AT_END );
}

void ferite_amtarray_unshift( FeriteScript *script, FeriteAMTArray *array, FeriteVariable *var )
{
	ferite_amtarray_add( script, array, var, NULL, FE_ARRAY_ADD_AT_START );
}

FeriteVariable *ferite_amtarray_pop( FeriteScript *script, FeriteAMTArray *array )
{
	FeriteAMTArrayEntry *entry = ferite_amt_get( script, array->_array, array->upperLimit - 1 );
	FeriteVariable *result = entry->variable;
	
	ferite_amt_delete( script, array->_array, --array->upperLimit );
	if( entry->key ) {
		ferite_hamt_delete( script, array->_hash, entry->key );
		ffree( entry->key );
	}
	ffree( entry );
	
	return result;
}

FeriteVariable *ferite_amtarray_shift( FeriteScript *script, FeriteAMTArray *array )
{
	FeriteAMTArrayEntry *entry = ferite_amt_get( script, array->_array, array->lowerLimit + 1 );
	FeriteVariable *result = entry->variable;
	
	ferite_amt_delete( script, array->_array, ++array->lowerLimit );
	if( entry->key ) {
		ferite_hamt_delete( script, array->_hash, entry->key );
		ffree( entry->key );
	}
	ffree( entry );
	
	return result;
}

int __ferite_amtarray_cmp_hash_data( void *left, void *right ) 
{
	if( left == right )
		return FE_TRUE;
	return FE_FALSE;
}
int __ferite_amtarray_cmp_array_data( FeriteScript *script, FeriteAMTArrayEntry *left, FeriteAMTArrayEntry *right, FeriteScript *_script )
{
	if( (left->key && !right->key) || (!left->key && right->key) )
		return FE_FALSE;
	if( strcmp( left->key, right->key ) != 0 )
		return FE_FALSE;
	return ferite_fast_variable_cmp( script, left->variable, right->variable );
}
int ferite_amtarray_cmp( FeriteScript *script, FeriteAMTArray *left, FeriteAMTArray *right )
{
	if( left->upperLimit != right->upperLimit )
		return FE_FALSE;
	if( left->lowerLimit != right->lowerLimit )
		return FE_FALSE;
	if( !ferite_amt_cmp( script, left->_array, right->_array, (int (*)(FeriteScript*,void*,void*,void*))__ferite_amtarray_cmp_array_data, script ) )
		return FE_FALSE;
	if( !ferite_amt_cmp( script, left->_hash, right->_hash, (int (*)(FeriteScript*,void*,void*,void*))__ferite_amtarray_cmp_hash_data, script ) )
		return FE_FALSE;
	return FE_TRUE;
}
FeriteAMTArrayEntry *__ferite_amtarray_dup_array_data( FeriteScript *script, FeriteAMTArrayEntry *entry, FeriteScript *_script )
{
	FeriteAMTArrayEntry *newEntry = fmalloc(sizeof(FeriteAMTArrayEntry));
	if( entry->key )
		newEntry->key = fstrdup(entry->key);
	newEntry->variable = ferite_duplicate_variable( script, entry->variable, NULL );
	return newEntry;
}
void *__ferite_amtarray_dup_hash_data( FeriteScript *script, void *value, FeriteScript *_script )
{
	return value;
}
FeriteAMTArray *ferite_amtarray_dup( FeriteScript *script, FeriteAMTArray *array )
{
	FeriteAMTArray *newArray = fmalloc(sizeof(FeriteAMTArray));
	newArray->_array = ferite_amt_dup( script, array->_array, (void *(*)(FeriteScript*,void*,void*))__ferite_amtarray_dup_array_data, script );
	newArray->_hash = ferite_amt_dup( script, array->_hash, (void *(*)(FeriteScript*,void*,void*))__ferite_amtarray_dup_hash_data, script );
	newArray->upperLimit = array->upperLimit;
	newArray->lowerLimit = array->lowerLimit;
	return newArray;
}

void _ferite_amtarray_item_to_str( FeriteScript *script, FeriteAMT *top, int id, char *key, FeriteAMTArrayEntry *entry, FeriteBuffer *buf, void *_extra ) {
	FeriteVariable *var = entry->variable;
	FeriteString *s = ferite_variable_to_str( script, var, 1 );
	
	if( entry->key )
		ferite_buffer_printf( script, buf, " '%s' => %.*s", entry->key, s->length, s->data );	 
	else {
		ferite_buffer_add_char( script, buf, ' ');
		ferite_buffer_add( script, buf, s->data, s->length);
	}
	ferite_str_destroy( script, s );
	ferite_buffer_add_char( script, buf, ',');
}
FeriteString *ferite_amtarray_to_str( FeriteScript *script, FeriteAMTArray *array )
{
	FeriteBuffer *buf = NULL;
	FeriteString *str = NULL;
	
	FE_ENTER_FUNCTION;
	buf = ferite_buffer_new(script, FE_DEFAULT_BUFFER_SIZE);
	ferite_buffer_add_char(script, buf, '[');
	ferite_amt_walk( script, array->_array, (AMTWalkCallback)_ferite_amtarray_item_to_str, buf, NULL );
	ferite_buffer_del_char( script, buf );
	ferite_buffer_add_char( script, buf, ' ');
	ferite_buffer_add_char( script, buf, ']');
	str = ferite_buffer_to_str( script, buf );
	ferite_buffer_delete( script, buf );
	FE_LEAVE_FUNCTION( str );
}

/**
 * @end
 */
