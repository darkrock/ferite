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
#include <math.h>

void ferite_uarray_copy_on_write( FeriteScript *script, FeriteUnifiedArray *out );

/**
 * @group Unified Array
 * @description This group is to ferite array's what the String group is to ferite strings.
 */

/**
 * @function ferite_uarray_create
 * @declaration FeriteUnifiedArray *ferite_uarray_create()
 * @brief Create a new empty array using the compiled in default size
 * @param The script to create the array in
 * @return A newly allocated FeriteUnifiedArray structure
 */
FeriteUnifiedArray *ferite_uarray_create( FeriteScript *script )
{
    FeriteUnifiedArray *out = NULL;

    FE_ENTER_FUNCTION;
	out = fmalloc(sizeof(FeriteUnifiedArray));
	out->size = 0;
	out->actual_size = FE_ARRAY_DEFAULT_SIZE;
	out->hash = ferite_create_hash( NULL, FE_ARRAY_DEFAULT_SIZE );
	out->array = fmalloc( sizeof(FeriteVariable*) * out->actual_size );
	out->iteration = -1;
	out->iterator = NULL;
	out->iterator_type = 0;
	out->copy_on_write = FE_FALSE;
    FE_LEAVE_FUNCTION( out );
}

/**
 * @function ferite_uarray_destroy
 * @declaration void ferite_uarray_destroy( FeriteScript *script, FeriteUnifiedArray *array )
 * @brief Destroy the array and free up any memory
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to delete
 * @description This function will iterate through the array and destroy each variable. It will then clear up any memory
                used by the array.
 */
void ferite_uarray_destroy( FeriteScript *script, FeriteUnifiedArray *array )
{
	int i;

	FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	if( ! array->copy_on_write ) {
		FUD(("ARRAY: %p: Deleting array, size %ld, actual size %ld----------------------------\n", array, array->size, array->actual_size));
		ferite_delete_hash( script, array->hash, NULL );
		for( i = 0; i < array->size; i++ ) {
			if( array->array[i] != NULL ) {
				FUD(("ARRYA: Deleting array item %d\n", i ));
				ferite_variable_destroy( script, array->array[i] );
			}
		}
		ffree( array->array );
	}
	if( array->iterator != NULL )
		ffree( array->iterator );
	ffree( array );
	FUD(("ARRAY: Done Deleteing array\n" ));
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_uarray_add
 * @declaration void ferite_uarray_add( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *var, char *id, int pos )
 * @brief Add a variable to the array
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to add to
 * @param FeriteVariable *var The variable to add
 * @param char *id The hash id
 * @param int pos The position in the array where the variable should be put
 * @description The hash id can be NULL - in which case the variable wont be hashed. If it isn't NULL
 *              the variable gets added to the array and also hashed in aswell. The variable can either
 *              be placed at the beginning or at the end of the array. Use either FE_ARRAY_ADD_AT_END
 *              or FE_ARRAY_ADD_AT_START to place the variable at the end or start respectively.
 */
void ferite_uarray_add( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *var, char *id, int pos )
{
    long i = 0;

    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );
	
    if( pos > 0 ) /* we either add from the beginning or the end */
      pos = FE_ARRAY_ADD_AT_END;

    if( id != NULL ) /* add it to the hash */
    {
        ferite_set_variable_name( script, var, id );
        /* We asume that the array is a hash, check against a magic number
         * to see if we need to grow the number of buckets */
        if( array->size > array->hash->size * 20 )
			array->hash = ferite_hash_grow( script, array->hash );
		if( ferite_hash_get( script, array->hash, var->vname ) ) {
			ferite_hash_update( script, array->hash, var->vname, var );
		} else {
	        ferite_hash_add( script, array->hash, var->vname, var );
		}
    }
    else
    {
        ferite_set_static_variable_name( script, var, "" );
    }

    if( FE_VAR_IS_DISPOSABLE( var ) )
    {
        UNMARK_VARIABLE_AS_DISPOSABLE( var );
    }

    if( pos == FE_ARRAY_ADD_AT_END )
    {
        if( array->size == array->actual_size ) /* we need to bump up the size of the array */
        {
            FUD(( "  resizing array from %ld to %ld\n", array->actual_size, array->actual_size * 2 ));
            array->actual_size *= 2;
            array->array = frealloc( array->array, sizeof(FeriteVariable*) * array->actual_size );
        }
        array->array[array->size] = var;
        FUD(( " to end slot %ld in array %p\n", array->size, array->array ));
        var->index = array->size;
        array->size++;

        FE_LEAVE_FUNCTION( NOWT );
    }
    if( pos == FE_ARRAY_ADD_AT_START )
    {
        if( array->size == array->actual_size ) /* we need to bump up the size of the array */
        {
            array->actual_size += FE_ARRAY_DEFAULT_SIZE;
            array->array = frealloc( array->array, sizeof(FeriteVariable*) * array->actual_size );
        }
        /* we need to do a memory move here */
        FUD(( " shifting array contents\n" ));
        memmove( array->array + 1, array->array, sizeof(FeriteVariable*) * array->size );
        /* and insert the stuff here */
        array->array[0] = var;
        FUD(( " to slot %d in array %p\n", 0, array->array ));
        array->size++;

        /* we re-index the variables here. this makes it very very expesive. fun. */
        for( i = 0; i < array->size; i++ )
          array->array[i]->index = i;

        FE_LEAVE_FUNCTION( NOWT );
    }
    ferite_error( script, 0, "Invalid add position %d\n", pos );
    FE_LEAVE_FUNCTION( NOWT );
}


/**
 * @function ferite_uarray_set_size
 * @declaration void ferite_uarray_set_size( FeriteScript *script, FeriteUnifiedArray *array, int size )
 * @brief This will grow an array to a new size.
 * @param FeriteScript *script The script context
 * @param FeriteUnifiedArray *array The array to change
 * @param int size The new size of the array
 * @description This function is useful for pre-growing an array where rapid growth is expected - such as filling it
                with an enormous amount of data. This stops the array having to grow automatically when adding items
                and can provide a welcome perfomance gain in various scenarios.
 */
void ferite_uarray_set_size( FeriteScript *script, FeriteUnifiedArray *array, int size )
{
    int i = array->size;
    
    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );
    if( array->actual_size < size )
    {
        array->actual_size = size;
        array->array = frealloc( array->array, sizeof(FeriteVariable*) * array->actual_size );
    }
    for( i = array->size; i < array->actual_size; i++ )
        array->array[i] = NULL;
    array->size = size;
    FE_LEAVE_FUNCTION( NOWT );
}
/**
 * @function ferite_uarray_get_index
 * @declaration FeriteVariable *ferite_uarray_get_index( FeriteScript *script, FeriteUnifiedArray *array, int index )
 * @brief Get a variable from the array based upon an index
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to get the variable from
 * @param int index The index to obtained
 * @return The variable if it exists, or NULL otherwise
 */
FeriteVariable *ferite_uarray_get_index( FeriteScript *script, FeriteUnifiedArray *array, int index )
{    
    FE_ENTER_FUNCTION;
    FUD(( "trying to get index %d\n", index ));
    if( array->size == 0 )
    {
        ferite_error( script, 0,"Invalid index: array size is 0\n");
        FE_LEAVE_FUNCTION( NULL );
    }

    if(index < 0)
    {
        index = array->size + index;
    }

    if( index >= array->size )
    {
        ferite_error( script, 0,"Index %d is out of array bounds [indexes 0 to %d]\n", index, array->size - 1 );
        FE_LEAVE_FUNCTION( NULL );
    }
    
    if( array->array[index] == NULL )
        array->array[index] = ferite_create_void_variable( script, "uvar", FE_STATIC );
    
    FE_LEAVE_FUNCTION( array->array[index] );
}

/**
 * @function ferite_uarray_get_from_string
 * @declaration FeriteVariable *ferite_uarray_get_from_string( FeriteScript *script, FeriteUnifiedArray *array, char *id )
 * @brief Get the variable from the array based upon a string
 * @param FeriteScript *script The Script
 * @param FeriteUnifiedArray *array The array to extract the variable from
 * @param char *id The name of the variable to get out of the array
 * @return The variable if it exists or NULL otherwise
 */
FeriteVariable *ferite_uarray_get_from_string( FeriteScript *script, FeriteUnifiedArray *array, char *id )
{
    FeriteVariable *ptr = NULL;
    FE_ENTER_FUNCTION;
    ptr = ferite_hash_get( script, array->hash, id );
    FE_LEAVE_FUNCTION(ptr);
}

/**
 * @function ferite_uarray_delete_from_string
 * @declaration FeriteVariable *ferite_uarray_delete_from_string( FeriteScript *script, FeriteUnifiedArray *array, char *id )
 * @brief Delete a value from the array based upon a string
 * @param FeriteScript *script The Script
 * @param FeriteUnifiedArray *array The array to delete the variable from
 * @param char *id The name of the variable to delete out of the array
 * @return The variable that has been deleted
 */
FeriteVariable *ferite_uarray_delete_from_string( FeriteScript *script, FeriteUnifiedArray *array, char *id )
{
    int real_index = 0;

    FeriteVariable *ptr = NULL;
    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );
    ptr = ferite_hash_get( script, array->hash, id );
    if( ptr == NULL )
    {
        ferite_error( script, 0, "Unknown index '%s'\n", id );
        FE_LEAVE_FUNCTION(NULL);
    }
    real_index = ptr->index;
    ferite_hash_delete( script, array->hash, id );
    ferite_uarray_del_index( script, array, real_index );
    FE_LEAVE_FUNCTION(ptr);
}

/**
 * @function ferite_uarray_get
 * @declaration FeriteVariable *ferite_uarray_get( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *var )
 * @brief Get a variable from an array based upon the value of a FeriteVariable
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to get the value from
 * @param FeriteVariable *var The variable to take the value from
 * @return The variable if it exists or NULL otherwise
 */
FeriteVariable *ferite_uarray_get_exceptions( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *var, int want_exceptions )
{
    FE_ENTER_FUNCTION;
    switch(F_VAR_TYPE(var))
    {
      case F_VAR_LONG:
        FE_LEAVE_FUNCTION( ferite_uarray_get_index( script, array, VAI(var)) );
        break;
      case F_VAR_DOUBLE:
        FE_LEAVE_FUNCTION( ferite_uarray_get_index( script, array, (int)floor(VAF(var)) ) );
        break;
      case F_VAR_STR: {
        FeriteVariable *rvar = ferite_uarray_get_from_string( script, array, FE_STR2PTR(var));
		if( want_exceptions && rvar == NULL ) {
			ferite_error( script, 0, "Invalid index: no such key '%s' in array.\n", FE_STR2PTR(var) );
	        FE_LEAVE_FUNCTION( NULL );
		}
		FE_LEAVE_FUNCTION(rvar);
        break;
	  }
      case F_VAR_OBJ: {
		    FeriteVariable *hash_value = NULL, *return_value = NULL;
		    FeriteFunction *hash = ferite_object_get_function_for_params( script, VAO(var), "hash", NULL );	    
		    hash_value = ferite_call_function( script, VAO(var), NULL, hash, NULL );
	    
		    if( F_VAR_TYPE(hash_value) != F_VAR_STR ) {
				if( want_exceptions )
					ferite_error( script, 0, "Invalid index: unable to get string key from object to access array.\n" );
				FE_LEAVE_FUNCTION(NULL);
		    }
		    return_value = ferite_uarray_get_from_string( script, array, FE_STR2PTR(hash_value) );
			if( want_exceptions && return_value == NULL ) {
				ferite_error( script, 0, "Invalid index: no such key '%s' (otained from object hash) in array.\n", FE_STR2PTR(hash_value) );
			}
		    ferite_variable_destroy( script, hash_value );
		    FE_LEAVE_FUNCTION( return_value );	    
		}
    }
    FE_LEAVE_FUNCTION( NULL );
}
FeriteVariable *ferite_uarray_get( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *var )
{
	return ferite_uarray_get_exceptions( script, array, var, FE_FALSE /* FE_TRUE */ );
}

FeriteVariable *ferite_uarray_set(FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *index, FeriteVariable *rhs )
{
    FeriteVariable *ptr = NULL;
	FeriteVariable *rhs_copy = NULL;

    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );
    if(F_VAR_TYPE(index) == F_VAR_VOID && FE_VAR_IS_PLACEHOLDER( index ) )  /* a[] = b */ {
		rhs_copy = ferite_duplicate_variable( script, rhs, NULL );
        ferite_uarray_add( script, array, rhs_copy, NULL, FE_ARRAY_ADD_AT_END );
	} else {
        ptr = ferite_uarray_get_exceptions( script, array, index, FE_FALSE );
        if( ptr == NULL ){
			if( NO_ERROR(script) ) /* Set a variable that doesn't exist */ {
				rhs_copy = ferite_duplicate_variable( script, rhs, NULL );
				ferite_uarray_add( script, array, rhs_copy, (F_VAR_TYPE(index) == F_VAR_STR ? FE_STR2PTR(index) : NULL ), FE_ARRAY_ADD_AT_END );
			}
		} else 	{
			if( ferite_types_are_equal( script, F_VAR_TYPE(ptr), F_VAR_TYPE(rhs) ) )
				ferite_variable_fast_assign( script, ptr, rhs );
			else {
				FeriteString *index_str = ferite_variable_to_str( script, index, FE_TRUE );
				ferite_error( script, 0, "Trying to set variable of type %s at index %s when a variable of type %s already exists\n", 
											ferite_variable_id_to_str(script, F_VAR_TYPE(rhs)), index_str->data, ferite_variable_id_to_str(script, F_VAR_TYPE(ptr)) );
				ferite_str_destroy( script, index_str );
				ptr = NULL;
			}
		}
    }
    FE_LEAVE_FUNCTION( rhs );
}

/**
 * @function ferite_uarray_del_var
 * @declaration void ferite_uarray_del_var( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *index )
 * @brief Delete a value from an array based upon the value of a FeriteVariable
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to delete the value from
 * @param FeriteVariable *var The variable to take the value from
 */
void ferite_uarray_del_var( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *index )
{
    int  real_index = 0;
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );
    if( F_VAR_TYPE(index) == F_VAR_STR )
    {
        ptr = ferite_hash_get( script, array->hash, FE_STR2PTR(index) );
        if( ptr == NULL )
        {
            ferite_error( script, 0, "Unknown index '%s'\n", FE_STR2PTR(index) );
            FE_LEAVE_FUNCTION(NOWT);
        }
        real_index = ptr->index;
    }
    else if( F_VAR_TYPE(index) == F_VAR_LONG )
    {
        real_index = VAI(index);
    }
    else if( F_VAR_TYPE(index) == F_VAR_DOUBLE )
    {
        real_index = (int)floor( VAF(index) );
    }
    else
    {
        ferite_error( script, 0, "Invalid index type '%s' on array\n", ferite_variable_id_to_str( script, F_VAR_TYPE(index) ) );
        FE_LEAVE_FUNCTION( NOWT );
    }
    ferite_uarray_del_index( script, array, real_index );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_uarray_del_index
 * @declaration void ferite_uarray_del_index( FeriteScript *script, FeriteUnifiedArray *array, int index )
 * @brief Delete a value from an array based upon an index
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to delete from
 * @param int index The index to delete
 */
void ferite_uarray_del_index( FeriteScript *script, FeriteUnifiedArray *array, int index )
{
    FeriteVariable *var = NULL;
    long i = 0;

    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );

    if( index >= array->size || index < 0 )
    {
        ferite_error( script, 0, "Index out of bounds %d, can't delete item\n", index );
        FE_LEAVE_FUNCTION( NOWT );
    }

   /* delete the entry in the array */
    var = array->array[index];
    if( ferite_hash_get( script, array->hash, var->vname ) != NULL )
      ferite_hash_delete( script, array->hash, var->vname );

    ferite_variable_destroy( script, var );

   /* we shift the items left one */
    memmove( array->array + index,
             array->array + index + 1,
             (array->size - index) * sizeof(FeriteVariable*) );

    array->size--;

   /* we re-index the variables here. this makes it very very expesive. fun. */
    for( i = index; i < array->size; i++ )
      array->array[i]->index = i;

    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_uarray_copy_on_write( FeriteScript *script, FeriteUnifiedArray *out ) 
{
	FE_ENTER_FUNCTION;
	if( out->copy_on_write ) {
		FeriteUnifiedArray _array;
		FeriteUnifiedArray *array = &_array;
		int i = 0;
		
		memcpy(array, out, sizeof(FeriteUnifiedArray));
		
		out->hash = ferite_create_hash( script, array->hash->size );
		out->array = fmalloc( sizeof(FeriteVariable*) * array->actual_size );

		/* this will go through and copy the variables, and where needs be add them to the hash */
	    for( i = 0; i < array->size; i++ ) {
			FeriteVariable *ptr = ferite_duplicate_variable( script, array->array[i], NULL );
			out->array[i] = ptr;
			if( ptr->index > -1 && ptr->vname[0] != '\0' )
				ferite_hash_add( script, out->hash, ptr->vname, ptr );
	    }
	    out->copy_on_write = FE_FALSE;
	}
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_uarray_dup
 * @declaration FeriteUnifiedArray *ferite_uarray_dup( FeriteScript *script, FeriteUnifiedArray *array )
 * @brief Duplicate an array
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to duplicate
 * @return A copy of the array and it's contents.
 */
FeriteUnifiedArray *ferite_uarray_dup( FeriteScript *script, FeriteUnifiedArray *array )
{
	FeriteUnifiedArray *out = NULL;
	FeriteVariable *ptr = NULL;
	int i;

	FE_ENTER_FUNCTION;
	out = fmalloc(sizeof(FeriteUnifiedArray));
	out->hash = array->hash;
	out->size = array->size;
	out->actual_size = array->actual_size;
	out->array = array->array;
	out->iteration = -1;
	out->iterator = NULL;
	out->copy_on_write = FE_TRUE;
	ferite_uarray_copy_on_write( script, out );
    FE_LEAVE_FUNCTION( out );
}

/**
 * @function ferite_uarray_to_str
 * @declaration FeriteString *ferite_uarray_to_str( FeriteScript *script, FeriteUnifiedArray *array)
 * @brief Create a FeriteString based upon the contents of an array
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to covert
 * @return The string version of the array
 * @description This is useful for converting an array to a string to write to disk, or print out.
 *              The function will recusre and handle inbuild arrays correctly. The output format is
                the same as the format used when building arrays within a ferite script.
 */
FeriteString *ferite_uarray_to_str( FeriteScript *script, FeriteUnifiedArray *array )
{
	FeriteVariable *var;	 
	     int i;	 
	     FeriteBuffer *buf;	 
	     FeriteString *str,*s;	 

	     FE_ENTER_FUNCTION;	 

	     buf = ferite_buffer_new(script, FE_DEFAULT_BUFFER_SIZE);	 

	     ferite_buffer_add_char(script, buf, '[');	 

	     for(i = 0; i < array->size; i++)	 
	     {	 
	         var = array->array[i];	 
	         s = ferite_variable_to_str( script, var, 1);	 
	         if(strcmp("",var->vname))	 
	         {	 
	             ferite_buffer_printf( script, buf," '%s' => %.*s",var->vname,s->length,s->data);	 
	         }	 
	         else	 
	         {	 
	             ferite_buffer_add_char( script, buf, ' ');	 
	             ferite_buffer_add( script, buf, s->data, s->length);	 
	         }	 
	         ferite_str_destroy( script, s );	 
	         if(i < array->size - 1)	 
	         {	 
	             ferite_buffer_add_char( script, buf, ',');	 
	         }	 
	     }	 
	     ferite_buffer_add_char( script, buf, ' ');	 
	     ferite_buffer_add_char( script, buf, ']');	 
	     str = ferite_buffer_to_str( script, buf );	 
	     ferite_buffer_delete( script, buf);	 
	     FE_LEAVE_FUNCTION( str );
}

/**
 * @function ferite_uarray_push
 * @declaration void ferite_uarray_push( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *var )
 * @brief Push a value onto the end array like a stack
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to push the value onto
 * @param FeriteVariable *var The variable to push onto the array
 */
void ferite_uarray_push( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *var )
{
    FeriteVariable *ptr = NULL;

    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );
    ptr = ferite_duplicate_variable( script, var, NULL );
    ferite_uarray_add( script, array, ptr, NULL, FE_ARRAY_ADD_AT_END );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_uarray_unshift
 * @declaration void ferite_uarray_unshift( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *var )
 * @brief Shift a value onto the front of the array
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to shift the value onto
 * @param FeriteVariable *var The variable to shift onto the array
 */
void ferite_uarray_unshift( FeriteScript *script, FeriteUnifiedArray *array, FeriteVariable *var )
{
    FeriteVariable *v;

    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );
    v = ferite_duplicate_variable( script, var, NULL );
    ferite_uarray_add( script, array, v, NULL, FE_ARRAY_ADD_AT_START );
    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_uarray_pop
 * @declaration FeriteVariable *ferite_uarray_pop( FeriteScript *script, FeriteUnifiedArray *array )
 * @brief Pop a value off the end of the array and return it
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to pop a value off the end
 */
FeriteVariable *ferite_uarray_pop( FeriteScript *script, FeriteUnifiedArray *array )
{
    FeriteVariable *out = NULL;

    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );
    if( array->size > 0 )
    {
        out = ferite_duplicate_variable( script, ferite_uarray_get_index( script, array, (array->size) - 1 ), NULL );
        ferite_uarray_del_index( script, array, (array->size)-1);
    }
    else
    {
        ferite_warning( script, "Trying to pop element off an empty array!\n" );
        out = ferite_create_void_variable( script, "no_value", FE_STATIC );
    }
    MARK_VARIABLE_AS_DISPOSABLE( out );
    FE_LEAVE_FUNCTION( out );
}

/**
 * @function ferite_uarray_shift
 * @declaration FeriteVariable *ferite_uarray_shift( FeriteScript *script, FeriteUnifiedArray *array )
 * @brief Shift a value off the front of the array
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *array The array to shift a value off the front
 */
FeriteVariable *ferite_uarray_shift( FeriteScript *script, FeriteUnifiedArray *array )
{
    FeriteVariable *out;

    FE_ENTER_FUNCTION;
	FE_ASSERT( array != NULL );
	ferite_uarray_copy_on_write( script, array );
    if( array->size > 0 )
    {
        out = ferite_duplicate_variable( script, ferite_uarray_get_index( script, array, 0 ), NULL );
        ferite_uarray_del_index( script, array, 0 );
    }
    else
    {
        ferite_warning( script, "Trying to shift element off an empty array!\n" );
        out = ferite_create_void_variable( script, "no_value", FE_STATIC );
    }
    MARK_VARIABLE_AS_DISPOSABLE( out );
    FE_LEAVE_FUNCTION( out );
}

/**
 * @function ferite_uarray_cmp
 * @declaration int ferite_uarray_cmp( FeriteScript *script, FeriteUnifiedArray *left, FeriteUnifiedArray *right )
 * @brief Compare two arrays
 * @param FeriteScript *script The script
 * @param FeriteUnifiedArray *left The first array
 * @param FeriteUnifiedArray *right The second array
 * @return FE_FALSE if the arrays differ, FE_TRUE otherwise
 * @description This function checks not only the array elements but also the hash elements of the array.
 */
int ferite_uarray_cmp( FeriteScript *script, FeriteUnifiedArray *left, FeriteUnifiedArray *right )
{
    int i = 0;

    FE_ENTER_FUNCTION;
    if( left->size != right->size )
    {
        FE_LEAVE_FUNCTION(FE_FALSE);
    }

   /* go through each element in the array */
    for( i = 0; i < left->size; i++ )
    {
        /* check the type of the variables */
        if( F_VAR_TYPE(left->array[i]) != F_VAR_TYPE(right->array[i]) )
        {
            FE_LEAVE_FUNCTION(FE_FALSE);
        }
        /* check names match, if they don't ignore it */
        if( strcmp( left->array[i]->vname, right->array[i]->vname ) != 0 ) 
        {
            FE_LEAVE_FUNCTION(FE_FALSE);
        }
        if( strcmp( left->array[i]->vname, "" ) != 0 )
        {
            /* they are hashed */
            if( ferite_hash_get(script,left->hash,left->array[i]->vname) == NULL || ferite_hash_get(script,right->hash,right->array[i]->vname) == NULL )
            {
                FE_LEAVE_FUNCTION(FE_FALSE);
            }
        }
        
        /* check their values */
		if( !ferite_fast_variable_cmp( script, left->array[i], right->array[i] ) ) {
			FE_LEAVE_FUNCTION(FE_FALSE);
		}
    }
   /* if we have got here they are the same :) */
    FE_LEAVE_FUNCTION(FE_TRUE);
}

/**
 * @end
 */
