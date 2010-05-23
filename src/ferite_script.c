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
#include "aphex.h"
#include "fcache.h"

/**
 * @group Script Helpers
 * @description Various functions for creating, loading and deleting script objects
 */

/**
 * @function ferite_new_script
 * @declaration FeriteScript *ferite_new_script()
 * @brief This will allocate a new script and set it up ready for use.
 * @return A pointer to the script
 */
FeriteScript *ferite_new_script()
{
    FeriteScript *ptr = NULL;

    FE_ENTER_FUNCTION;
    ptr = fmalloc_ngc( sizeof( FeriteScript ) );
    ptr->filename = NULL;
    ptr->scripttext = NULL;
 
    ptr->mainns = ferite_register_namespace( ptr, NULL, NULL );

    ptr->include_list = ferite_create_stack( NULL, FE_COMPILER_INTERNAL_STACK_SIZE );

    ptr->current_op_file = NULL;
    ptr->current_op_line = 0;

    ptr->error_state = 0;
    ptr->keep_execution = 0;
    ptr->is_executing = 0;
    ptr->is_being_deleted = FE_FALSE;
    ptr->return_value = 0;
    ptr->error = NULL;
    ptr->warning = NULL;
    ptr->last_regex_count = 0;
    ptr->stack_level = 0;

    ptr->gc = NULL;
    ptr->gc_running = FE_FALSE;
    ptr->gc_count = 0;
    ptr->gc_lock = NULL;
	ptr->gc_stack = NULL;
	ptr->lock = NULL;
    ptr->thread_group = NULL;
    ptr->parent = NULL;
    ptr->is_multi_thread = FE_FALSE;
	
#ifdef THREAD_SAFE
    ptr->thread_group = ferite_create_thread_group( ptr );
    ptr->lock = aphex_mutex_create();
    ptr->gc_lock = aphex_mutex_recursive_create();
#endif
    
	ptr->vars = NULL;
	ptr->objects = NULL;
	ptr->stacks = NULL;

    ferite_init_cache( ptr );
    ptr->_odata = NULL;
    ptr->odata_id = 1;

	ptr->globals = ferite_AMTHash_Create(ptr);
	ptr->types = ferite_AMTHash_Create(ptr);
	
    FE_LEAVE_FUNCTION( ptr );
}

/**
 * @function ferite_script_load
 * @declaration int ferite_script_load( FeriteScript *script, char *filename )
 * @brief Load the text of a script so that it may be compiled.
 * @param FeriteScript *script The script to load the text into
 * @param char *filename The name of the script to load.
 * @return Returns 0 on fail and 1 on success
 */
int ferite_script_load( FeriteScript *script, char *filename )
{
	char *scripttext = NULL;
	int scripttext_destroy = FE_FALSE;

    FE_ENTER_FUNCTION;
    script->filename = NULL;
    script->scripttext = NULL;

	scripttext = ferite_cache_has_code( script, filename );
	if( !scripttext ) {
		scripttext = aphex_file_to_string( filename );
		if( scripttext == NULL ) {
			FE_LEAVE_FUNCTION(FE_FALSE);
		} else {
			scripttext_destroy = FE_TRUE;
			if( scripttext[0] == '#' ) {
				int i = 0;
				for( i = 0; scripttext[i] != '\n'; i++ )
					scripttext[i] = ' ';
			}
			ferite_parser_script_normalise( scripttext );
			ferite_cache_register_code( script, filename, scripttext );
		}
	} else {
		scripttext_destroy = FE_FALSE;
	}

	if( scripttext != NULL ) {
		script->filename = fstrdup( filename );
		script->scripttext = fstrdup( scripttext );
		if( scripttext_destroy ) {
			aphex_free(scripttext);
		}
		FE_LEAVE_FUNCTION(FE_TRUE);
	}
	FE_LEAVE_FUNCTION(FE_FALSE);
}

/**
 * @function ferite_script_being_deleted
 * @declaration int ferite_script_being_deleted( FeriteScript *script )
 * @brief Find out if the script in question is being deleted.
 * @description This function is often useful for making decisions within a native destructor.
 * @param FeriteScript *script The script to check
 * @return FE_TRUE if the script is being deleted, FE_FALSE otherwise
 */
int ferite_script_being_deleted( FeriteScript *script )
{
    FE_ENTER_FUNCTION;
    FE_LEAVE_FUNCTION( script && script->is_being_deleted );
}

void __ferite_script_clean_odata( FeriteScript *script, void *data, char *key )
{
   FeriteScriptAttachedData *adata = data;
   FeriteAttachedDataCleanup cb = adata->cleanup;
   FE_ENTER_FUNCTION;
   if( cb )
     (cb)( script, adata->id, key, adata->data );
   ffree( adata );
   FE_LEAVE_FUNCTION(NOWT);
}

void ferite_script_global_variable_destroy( FeriteScript *script, FeriteVariable *variable ) {
	if( FE_VAR_IS_COMPILED(variable) ) {
		ferite_variable_destroy( script, variable );
	}
}

/**
 * @function ferite_script_clean
 * @declaration int ferite_script_clean( FeriteScript *script )
 * @brief Clean a script structure of all non-error/warning reporting structures
 * @param FeriteScript *script A pointer to the script to be cleaned
 * @return Returns 0 on fail and 1 on success
 */
int ferite_script_clean( FeriteScript *script )
{
    int i;

    FE_ENTER_FUNCTION;

    if( script != NULL )
    {
        /*
         * THREAD: Basically here we will go through the list of threads and loop until
         * they have finished, this means that threads can't be deleted until they are
         * actually dead.
         */

#ifdef THREAD_SAFE
        if( script->lock != NULL )
        {
            ferite_thread_group_destroy( script, script->thread_group );
            aphex_mutex_destroy( script->lock );
            script->lock = NULL;
            script->thread_group = NULL;
        }        
#endif

        script->is_being_deleted = FE_TRUE;

       if( script->_odata )
	 {	    
	    ferite_process_hash( script, script->_odata, __ferite_script_clean_odata );
	    ferite_delete_hash( script, script->_odata, NULL );
		script->_odata = NULL;
	 }
       
        /* delete the garbage collector */
        if( script->gc != NULL )
        {
            ferite_deinit_gc( script );
#ifdef THREAD_SAFE
            if( script->gc_lock != NULL )
            {
                aphex_mutex_destroy( script->gc_lock );
                script->gc_lock = NULL;
            }        
#endif
        }
		/* delete the include list */
        if( script->include_list != NULL )
        {
            for( i = 0; i <= script->include_list->stack_ptr; i++ )
            {
                if( script->include_list->stack[i] != NULL )
                {
                    ferite_unload_native_module( script->include_list->stack[i], script );
                    ffree( script->include_list->stack[i] );
                }
            }
            ferite_delete_stack( NULL, script->include_list );
            script->include_list = NULL;
        }

        /* delete the body of the script */
        if( script->mainns != NULL )
        {
            ferite_delete_namespace( script, script->mainns );
            script->mainns = NULL;
        }
  
		if( script->globals ) {
			ferite_amt_destroy( script, script->globals, (void(*)(FeriteScript*,void*))ferite_script_global_variable_destroy );
			script->globals = NULL;
		}
		if( script->types ) {
			ferite_amt_destroy( script, script->types, (void(*)(FeriteScript*,void*))ferite_subtype_destroy );
			script->types = NULL;
		}

        /* ... and finally... */
        if( script->filename != NULL )
          ffree( script->filename );
        if( script->scripttext != NULL )
          ffree( script->scripttext );

        script->is_being_deleted = FE_FALSE;
		script->is_multi_thread = FE_FALSE;

        FE_LEAVE_FUNCTION( 1 );
    }
    FE_LEAVE_FUNCTION(0);
}

/**
 * @function ferite_script_delete
 * @declaration int ferite_script_delete( FeriteScript *script )
 * @brief Delete a script object - it calls ferite_script_clean() first so you dont need to
 * @param FeriteScript *script The script to delete
 * @return 1 on success, 0 on fail
 */
int ferite_script_delete( FeriteScript *script )
{
    FE_ENTER_FUNCTION;
    if( script != NULL )
    {
        ferite_script_clean( script );
        ferite_free_cache( script );
        if( script->error != NULL )
          ferite_buffer_delete( script, script->error );
        if( script->warning != NULL )
          ferite_buffer_delete( script, script->warning );
        /* aww crap. my wrong. this is the final act! */
        ffree( script );
        FE_LEAVE_FUNCTION( 1 );
    }
    FE_LEAVE_FUNCTION(0);
}

/**
 * @function ferite_duplicate_script
 * @declaration FeriteScript *ferite_duplicate_script( FeriteScript *script )
 * @brief Duplicate a script object
 * @param FeriteScript *script The script to make a copy of
 * @return A brand new script object that can be executed and used as if it had been compiled
 */
FeriteScript *ferite_duplicate_script( FeriteScript *script )
{
    FeriteScript *ptr = NULL;
    FeriteClass *klass = NULL;
    FeriteStack *klass_list = NULL;
    FeriteNamespaceBucket *nsb = NULL;
    char *name = NULL;
    int i = 0;

    FE_ENTER_FUNCTION;
    if( script != NULL )
    {
        ptr = ferite_new_script();

        if( script->mainns != NULL )
        {
            /* we duplicate the include list and also the main namespace */
            ferite_delete_namespace( ptr, ptr->mainns );
/*REMOVE            ptr->odata = ferite_create_stack( script, 30 ); */
            ptr->mainns = ferite_namespace_dup( ptr, script->mainns, NULL );

            /* link up the klass's parents */
/*REMOVE          klass_list = ptr->odata;
            for( i = 0; i <= klass_list->stack_ptr; i++ )
            {
                klass = klass_list->stack[i];
                if( klass != NULL && klass->parent != NULL )
                {
                    name = (char *)klass->parent;
                    FUD(( "searching for parent '%s' of '%s' ... ", name, klass->name ));
                    nsb = ferite_find_namespace( ptr, ptr->mainns, name, FENS_CLS );
                    if( nsb != NULL )
                    {
                        klass->parent = nsb->data;
                        FUD(( "found '%s'", klass->parent->name ));
                    }
                    else
                      FUD(( "erk, can't find it" ));
                    FUD(( "\n" ));
                    ffree( name );
                }
            }
            ferite_delete_stack( script, ptr->odata ); */
        }

        /* copy the include list for the script */
        for( i = 0; i <= script->include_list->stack_ptr; i++ )
        {
            if( script->include_list->stack[i] != NULL )
              ferite_stack_push( ptr, ptr->include_list, fstrdup(script->include_list->stack[i]) );
        }

        if( script->gc )
        {
            /* setup the gc */
            ferite_init_gc( ptr );
        }
    }
    FE_LEAVE_FUNCTION( ptr );
}

void ferite_init_cache( FeriteScript *script )
{
    FE_ENTER_FUNCTION;
    if( script )
    {
		script->vars = ferite_create_stack( NULL, FE_CACHE_SIZE );
        script->objects = ferite_create_stack( NULL, FE_CACHE_SIZE );
        script->stacks = ferite_create_stack( NULL, FE_CACHE_SIZE );
    }
    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_free_cache( FeriteScript *script )
{
    int i = 0;

    FE_ENTER_FUNCTION;
    FE_ASSERT( script != NULL );

    if( script->vars != NULL )
    {
        /* clean up the variables */
        for( i = 1; i <= script->vars->stack_ptr; i++ ) {
            ffree( script->vars->stack[i] );
        }
        ferite_delete_stack( NULL, script->vars );
        script->vars = NULL;
    }

    if( script->objects != NULL )
    {
        /* clean up the objects */
        for( i = 1; i <= script->objects->stack_ptr; i++ ) {
            ffree( script->objects->stack[i] );
        }
        ferite_delete_stack( NULL, script->objects );
        script->objects = NULL;
    }

    if( script->stacks != NULL )
    {
        /* clean up the stacks */
        for( i = 1; i <= script->stacks->stack_ptr; i++ ) {
            ffree( script->stacks->stack[i] );
        }
        ferite_delete_stack( NULL, script->stacks );
        script->stacks = NULL;
    }

    FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_script_attach_data
 * @declaration int ferite_script_attach_data( FeriteScript *script, char *key, void *data, FeriteAttachedDataCleanup cleanup )
 * @brief Attach data to the script object
 * @param FeriteScript *script The script to attach data to
 * @param char *key The name of the data
 * @param void *data The data to attach
 * @param FeriteAttachedDataCleanup cleanup A pointer to the function to be run when the data is removed. If NULL, no function will be called.
 * @return A integer id that will represent the data
 */
int ferite_script_attach_data( FeriteScript *script, char *key, void *data, FeriteAttachedDataCleanup cleanup )
{
   FeriteScriptAttachedData *adata = NULL, *edata = NULL;
   FE_ENTER_FUNCTION;
   adata = fmalloc( sizeof(FeriteScriptAttachedData) );
   adata->id = script->odata_id++;
   adata->data = data;
   adata->cleanup = cleanup;

   if( script->_odata == NULL )
     script->_odata = ferite_create_hash( FE_NoScript, 4 );
   
   edata = ferite_hash_get( script, script->_odata, key );
   if( edata )
     ferite_script_remove_data( script, key );
   ferite_hash_add( script, script->_odata, key, adata );
   FE_LEAVE_FUNCTION( adata->id );
}
/**
 * @function ferite_script_remove_data
 * @declaration int ferite_script_remove_data( FeriteScript *script, char *key )
 * @brief Remove data from the script object.
 * @description This will remove the attached data and, if present, call the cleanup function.
 * @param FeriteScript *script The script to attach data to
 * @param char *key The name of the data
 * @return True if the data was deleted, false otherwise.
 */
int ferite_script_remove_data( FeriteScript *script, char *key )
{
   FeriteScriptAttachedData *edata = NULL;

   FE_ENTER_FUNCTION;
   edata = ferite_hash_get( script, script->_odata, key );
   if( edata != NULL ) 
     {
	__ferite_script_clean_odata( script, edata, key );
	ferite_hash_delete( script, script->_odata, key );	
	FE_LEAVE_FUNCTION(FE_TRUE);
     }
   FE_LEAVE_FUNCTION(FE_FALSE);
}
/**
 * @function ferite_script_fetch_data
 * @declaration int ferite_script_fetch_data( FeriteScript *script, char *key )
 * @brief Fetch data from the script object.
 * @param FeriteScript *script The script to attach data to
 * @param char *key The name of the data
 * @return A pointer to the data if it exists, NULL otherwise
 */
void *ferite_script_fetch_data( FeriteScript *script, char *key )
{
   FeriteScriptAttachedData *edata = NULL;

   FE_ENTER_FUNCTION;
   edata = ferite_hash_get( script, script->_odata, key );
   if( edata != NULL ) 
     {
	FE_LEAVE_FUNCTION(edata->data);
     }
   FE_LEAVE_FUNCTION(NULL);
}


/**
 * @end
 */
