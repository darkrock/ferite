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
#include "triton.h"
#include "aphex.h"

#ifdef VCWIN32
# include "snprintf.h" /* This is so that we have somethings */
#endif

/**
 * @group Modules
 * @description This set of functions are useful for module writers. The module system provides
                a uniform mechanism for loading native libraries and registering the functions
                within the system.
 */

FeriteModule *ferite_root_module = NULL;
FeriteModule *ferite_current_module = NULL;
FeriteAMT    *ferite_native_function_hash = NULL;
FeriteStack  *ferite_module_search_list = NULL;
FeriteStack  *ferite_module_preload_list = NULL;
FeriteStack  *ferite_module_native_search_list = NULL;
char         *ferite_script_extensions[] = { ".fec", ".feh", ".fe", NULL };
int         (*ferite_module_can_be_loaded)( char *module );

extern int    ferite_compile_error;

int __ferite_module_can_be_loaded( char *module )
{
    return FE_TRUE;
}

FeriteModule *ferite_create_module( char *name, char *filename )
{
    FeriteModule *ptr = NULL;
    
    FE_ENTER_FUNCTION;
    ptr = fmalloc_ngc( sizeof(FeriteModule) );
    ptr->name = fstrdup( name );
    ptr->filename = fstrdup( filename );
    ptr->handle = NULL;
    ptr->module_init = NULL;
    ptr->module_deinit = NULL;
    ptr->module_unregister = NULL;
    ptr->module_register = NULL;
    ptr->next = NULL;
    FE_LEAVE_FUNCTION( ptr );
}

int ferite_init_module_list()
{
    FE_ENTER_FUNCTION;
    /* initialise */
    FUD(( "MODULE LOADER: initialising module loader... " ));
    if( triton_init() != 0 )
    {
        FE_LEAVE_FUNCTION( 0 );
    }
    else
    {
        ferite_module_search_list = ferite_create_stack( NULL, 5 );
        ferite_module_native_search_list = ferite_create_stack( NULL, 5 );
        ferite_module_preload_list = ferite_create_stack( NULL, 5 );
        ferite_native_function_hash = ferite_AMTHash_Create( NULL );
        ferite_root_module = ferite_create_module( "ferite_root_module", "" );
        ferite_current_module = ferite_root_module;
        ferite_module_can_be_loaded = __ferite_module_can_be_loaded;
    }
    FE_LEAVE_FUNCTION( 1 );
}

void ferite_destroy_module_list( FeriteModule *mod )
{
    FE_ENTER_FUNCTION;
    if( mod != NULL )
    {
        if( mod->next != NULL )
          ferite_destroy_module_list( mod->next );

        FUD(( "MODULE LOADER: deleting module \"%s\"\n", mod->name ));

        mod->module_init = NULL;
        mod->module_deinit = NULL;

        if( mod->module_unregister != NULL )
        {
            (mod->module_unregister)();
        }

        mod->module_unregister = NULL;

        ffree_ngc( mod->filename );
        ffree_ngc( mod->name );
        ffree_ngc( mod );
        mod = NULL;
    }
    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_deinit_module_list()
{
    int i = 0;

    FE_ENTER_FUNCTION;

#define DeleteStringStack( STACK ) \
   /* tidy up the search paths */ \
    for( i = 0; i <= STACK->stack_ptr; i++ ) \
    { \
        if( STACK->stack[i] != NULL ) \
            ffree_ngc( STACK->stack[i] ); \
    } \
    ferite_delete_stack( NULL, STACK ); \
    STACK = NULL;
	
	DeleteStringStack( ferite_module_search_list );
	DeleteStringStack( ferite_module_native_search_list );

   /* tidy up the preload lists */
    for( i = 0; i <= ferite_module_preload_list->stack_ptr; i++ )
    {
        if( ferite_module_preload_list->stack[i] != NULL )
        {
            ffree_ngc( ferite_module_preload_list->stack[i] );
        }
    }
    ferite_delete_stack( NULL, ferite_module_preload_list );
    ferite_module_preload_list = NULL;

    FUD(( "MODULE LOADER: closing all opened modules\n" ));
    ferite_module_delete_native_function_list();

    ferite_destroy_module_list( ferite_root_module );
    ferite_root_module = NULL;

    FUD(( "MODULE LOADER: closing down module loader\n" ));

    FE_LEAVE_FUNCTION( NOWT );
}

void ferite_clear_module_search_list()
{
    int i = 0;
	FE_ENTER_FUNCTION;
	DeleteStringStack( ferite_module_search_list );
	ferite_module_search_list = ferite_create_stack( NULL, 5 );
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_load_module
 * @declaration int ferite_load_module( FeriteScript *script, FeriteNamespace *ns, char *name )
 * @brief Load a module (script or native)
 * @param FeriteScript *script The script to load the module into
 * @param FeriteNamespace *ns The namespace in which the module will be loaded. If you are not sure
                              which namespace to use, just pass in script->mainns
 * @param char *name The name of the module
 * @return 0 on fail, otherwise greater than 0 for sucess
 */
int ferite_load_module( FeriteScript *script, FeriteNamespace *ns, char *name )
{
    char *extension = NULL;
    char *filename = NULL;
    int result = 0;

    FE_ENTER_FUNCTION;

    extension = strrchr( name, '.' );         /* get the start of the extension */
    filename = strrchr( name, DIR_DELIM );    /* get the start of the filename */

    FUD(( "-------------------->" ));
    FUD(( "we have been asked to include %s\n", name ));

    if( extension == NULL || extension < filename ) 
                         /* no extension (or extension is before filename eg ../fnar, we 
                            first try to load a native module
                          * then we try and load a script with that name */
    {
        result = ferite_load_script_module( script, name, 1 );
        if( result == 0 )
          ferite_error( script, 0, "Unable to find module '%s'\n", name );
        FE_LEAVE_FUNCTION(result);
    }
    else
    {
        if( strcmp( extension, ".lib" ) == 0 ) /* if it's a library */
        {
            FE_LEAVE_FUNCTION( ferite_load_native_module( name, script ) );
        }
        FE_LEAVE_FUNCTION( ferite_load_script_module( script, name, 0 ) );
    }
    FUD(( "<-------------------" ));

    FE_LEAVE_FUNCTION(0);
}

int ferite_load_script_module( FeriteScript *script, char *name, int do_extension )
{
    char file[PATH_MAX];
    char *filename = file;
    int i = 0, j = 0, result = 0;

    FE_ENTER_FUNCTION;
    /* so now we go though file extensions that it could be - in all all the search paths .fec .feh  .fe*/
    for( i = 0; i <= ferite_module_search_list->stack_ptr; i++ )
    {
        if( ferite_module_search_list->stack[i] != NULL )
        {
            /* we check this path */
            if( do_extension == 1 )
            {
                for( j = 0; ferite_script_extensions[j] != NULL; j++ )
                {
                    memset( filename, '\0', PATH_MAX );
                    FUD(( "module_path: %s, %s, '%c', %s\n", (char *)ferite_module_search_list->stack[i], name, DIR_DELIM, ferite_script_extensions[j] ));
                    snprintf( filename, PATH_MAX, "%s%c%s%s", (char *)ferite_module_search_list->stack[i], DIR_DELIM, name, ferite_script_extensions[j] );
                    FUD(( "module_filename: %s\n", filename ));
                    result = ferite_do_load_script( filename );
                    if( result > -1 ){
                        FE_LEAVE_FUNCTION( result );
                    }
                }
            }
            else
            {
                snprintf( filename, PATH_MAX, "%s%c%s", (char *)ferite_module_search_list->stack[i], DIR_DELIM, name );
                result = ferite_do_load_script(filename);
                if( result > -1 ){
                    FE_LEAVE_FUNCTION( result );
                }
            }
        }
    }

    if( do_extension == 1 )
    {
        for( j = 0; ferite_script_extensions[j] != NULL; j++ )          
        {
            snprintf( filename, PATH_MAX, "%s%s", name, ferite_script_extensions[j] );
            result = ferite_do_load_script(filename);
            if( result > -1 ){
                FE_LEAVE_FUNCTION( result );
            }
        }
    }
    else
    {
        result = ferite_do_load_script(name);
        if( result > -1 ){
            FE_LEAVE_FUNCTION( result );
        }
    }
    ferite_error( script, 0, "Unable to find script module '%s'\n", name );

    FE_LEAVE_FUNCTION(0);
}

/**
 * @function ferite_module_find
 * @declaration FeriteModule *ferite_module_find( char *name )
 * @brief Locate a module within the ferite runtime
 * @param char *name The name of the module
 * @description This function is useful to find out if a module has been loaded within the ferite
                system.
 * @return A FeriteModule pointer on success, NULL otherwise
 */
FeriteModule *ferite_module_find( char *name )
{
    FeriteModule *ptr = NULL;

    FE_ENTER_FUNCTION;
    FUD(( "MODULE LOADER: Trying to locate module \"%s\"\n", name ));
   /* check it'#s not already loaded */
    for( ptr = ferite_root_module; ptr != NULL; ptr = ptr->next )
    {
        FUD(("MODULE LOADER: Checking \"%s\" and \"%s\"\n", name, ptr->name ));
        if( strcmp( name, ptr->name ) == 0 )
        {
            FUD(( "MODULE LOADER: Module \"%s\" is allready loaded\n", name ));
            FE_LEAVE_FUNCTION(ptr); /* we return 1 because we have it :) */
        }
    }
    FE_LEAVE_FUNCTION(NULL);
}

int ferite_unload_native_module( char *name, FeriteScript *script )
{
    FeriteModule *ptr = NULL;
    
    FE_ENTER_FUNCTION;
    ptr = ferite_module_find( name );
    if( ptr != NULL && ptr->module_deinit != NULL )
    {
        (ptr->module_deinit)( script );
        FE_LEAVE_FUNCTION(1);
    }
    FE_LEAVE_FUNCTION(0);
}

int ferite_load_native_module( char *module_name, FeriteScript *script )
{
    char buf[PATH_MAX], real_buf[PATH_MAX], *extension = NULL, *name = NULL;
    void *handle = NULL;
    FeriteModule *ptr = NULL;
	int i = 0;
    
    FE_ENTER_FUNCTION;
    
    memset( buf, '\0', PATH_MAX );
    memset( real_buf, '\0', PATH_MAX );
    
    name = fstrdup( module_name );
    extension = strrchr( name, '.' );
    if( extension != NULL )
    {
        /* get rid of the extension */
        *extension = '\0';
    }
    if( (*ferite_module_can_be_loaded)( name ) )
    {
        if( ferite_compiler_include_in_list( script, name ) == 0 )
        {
            ptr = ferite_module_find( name );
            if( ptr != NULL )
            {
                (ptr->module_init)( script );
                ferite_stack_push( FE_NoScript, script->include_list, fstrdup(name) );
                ffree_ngc( name );
                /* we return 1 because we have it :) */
                FE_LEAVE_FUNCTION(1); 
			}
            
            /* we dont already have it, lets try to open it */
			for( i = 1; i <= ferite_module_native_search_list->stack_ptr; i++ ) 
            {
                sprintf( buf, "%s%c%s", (char *)(ferite_module_native_search_list->stack[i]), DIR_DELIM, (char*)name );
                
                /* now we try to load this module */
                FUD(( "MODULE LOADER: trying to load native module %s\n", buf ));
                handle = (void *)triton_openext( buf );
                if( handle == NULL )
                {
                    FUD(( "MODULE LOADER: checking for file's existance: %s\n", real_buf ));
                    if( aphex_file_exists( "%s%s", buf, triton_library_ext() ) == 1 )
                    {
                        ferite_error( script, 0, "Library Loader: %s\n", triton_error() );
                        ffree_ngc( name );
                        FE_LEAVE_FUNCTION(0);
                    }
                }
				else
				{
					/* We have loaded the module */
					break;
				}
            }
            
            if( handle == NULL ) 
            {
                /* we failed to open, lets just try opening here and now */
                sprintf( buf, "%s", name );
                
                FUD(( "MODULE LOADER: trying to load native module %s\n", buf ));
                handle = (void *)triton_openext( buf );
                if( handle == NULL )
                {
                    sprintf( real_buf, "%s%s", buf, triton_library_ext() );                
                    FUD(( "MODULE LOADER: checking for file's existance: %s\n", real_buf ));
                    if( aphex_file_exists( "%s%s", buf, triton_library_ext() ) == 1 )
                    {
                        ferite_error( script, 0, "Library Loader: %s\n", triton_error() );
                        ffree_ngc( name );
                        FE_LEAVE_FUNCTION(0);
                    }
                    else
                    {
                        ferite_error( script, 0, "Library Loader: Can't find module '%s'\n", module_name );
                        ffree_ngc( name );
                        FE_LEAVE_FUNCTION(0);
                    }
                }
            }
            
            /* if we have a handle go for it! */
            if( handle != NULL )
            {
                FUD(( "MODULE LOADER: loaded!\n" ));
                
                ferite_current_module->next = ferite_create_module( name, buf );
                FUD(( "MODULE LOADER: Loading module \"%s\" from file \"%s\"\n", name, buf ));
                ferite_current_module->next->handle = ptr;
                
                /* register function */
                sprintf( buf, "ferite_%s_register", name );
                ferite_current_module->next->module_register = (void (*)())triton_getsym( handle, buf );
                if( ferite_current_module->next->module_register == NULL )
                { 
                    /* we dont have a register function. we are in the sh1t */
                    triton_close( handle );  /* close the lib */
                    ferite_destroy_module_list( ferite_current_module->next );
                    ferite_current_module->next = NULL;
                    ferite_error( script, 0, "Library Loader: can't find '%s' in module '%s', ferite needs this to function correctly.\n", buf, name );
                    ffree_ngc( name );
                    FE_LEAVE_FUNCTION(0);
                }
                ferite_current_module = ferite_current_module->next;
                
                /* init function */
                sprintf( buf, "ferite_%s_init", name );
                ferite_current_module->module_init = (void (*)(FeriteScript*))triton_getsym( handle, buf );
                
                /* deinit function -> we dont use this just yet */
                sprintf( buf, "ferite_%s_deinit", name );
                ferite_current_module->module_deinit = (void (*)(FeriteScript*))triton_getsym( handle, buf );
                
                /* module_unregister */
                sprintf( buf, "ferite_%s_unregister", name );
                ferite_current_module->module_unregister = (void (*)())triton_getsym( handle, buf );
                
                (ferite_current_module->module_register)();
                FUD(( "MODULE LOADER: Calling %s::module_init()\n", name ));
                (ferite_current_module->module_init)( script );
                
                ferite_stack_push( FE_NoScript, script->include_list, fstrdup(name) );
            }
            else
            {
                ferite_error( script, 0, "Library Loader: can't load module '%s', %s.", name, triton_error() );
                ffree_ngc( name );
                FE_LEAVE_FUNCTION(0);
            }
        }
    }
    ffree_ngc( name );
    FE_LEAVE_FUNCTION(1);
}

/**
 * @function ferite_add_library_search_path
 * @declaration void ferite_add_library_search_path( char *path )
 * @brief Add a path to the list of paths that ferite searches for scripts
 * @param char *path The path to add
 * @description Everytime a module is asked to be loaded, ferite will go through each of
                the search paths within the system. By default there are no paths used
                apart from the current directory.
 */
void ferite_add_library_search_path( char *path )
{
	char *p = fstrdup(path);
	int  last = strlen(path) - 1;
    FE_ENTER_FUNCTION;
	if( p[last] == DIR_DELIM ) {
		p[last] = '\0';
	}
    ferite_stack_push( FE_NoScript, ferite_module_search_list, p );
    FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_pop_library_search_path
 * @declaration void ferite_pop_library_search_path()
 * @brief Remove the last added search path from the list of paths
 */
void ferite_pop_library_search_path()
{
    char *path = NULL;

    FE_ENTER_FUNCTION;
    path = ferite_stack_pop( FE_NoScript, ferite_module_search_list );
    if( path != NULL )
      ffree_ngc( path );
    FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_set_library_native_path
 * @declaration void ferite_set_library_native_path( char *path )
 * @brief Set the path where ferite looks for the native libraries to load
 * @param char *path The path where they libraries are located
 * @description The system uses multiple native search paths, this will add the path onto 
                stack of paths used.
 */
void ferite_set_library_native_path( char *path )
{
    FE_ENTER_FUNCTION;
    ferite_stack_push( FE_NoScript, ferite_module_native_search_list, fstrdup(path) );
    FE_LEAVE_FUNCTION(NOWT);
}
/**
 * @function ferite_pop_library_native_search_path
 * @declaration void ferite_pop_library_native_search_path()
 * @brief Remove the last added native search path from the list of paths
 */
void ferite_pop_library_native_search_path()
{
    char *path = NULL;
	
    FE_ENTER_FUNCTION;
    path = ferite_stack_pop( FE_NoScript, ferite_module_native_search_list );
    if( path != NULL )
		ffree_ngc( path );
    FE_LEAVE_FUNCTION(NOWT);
}

void ferite_module_register_native_function( char *lookupname, 
                                             FeriteVariable *(*ptr)(FeriteScript*,void*,FeriteObject*,FeriteFunction*,FeriteVariable** ) )
{
    FeriteNativeFunctionRecord *record = NULL;

    FE_ENTER_FUNCTION;
    if( ferite_native_function_hash != NULL )
    {
        record = ferite_hamt_get( NULL, ferite_native_function_hash, lookupname );
        if( record == NULL )
        {
            FUD(( "MODULE: Registering native function '%s'\n", lookupname ));
            record = fmalloc_ngc( sizeof( FeriteNativeFunctionRecord ) );
            record->function = ptr;
            ferite_hamt_set( NULL, ferite_native_function_hash, lookupname, record );
        }
        else
          fprintf( stderr, "The native function '%s' has already exists, will not re-register.\n", lookupname );
    }
    FE_LEAVE_FUNCTION(NOWT);
}

/** 
 * @function ferite_module_register_fake_module
 * @declaration void ferite_module_register_fake_module( char *name, void *_register, void *_unregister, void *_init, void *_deinit )
 * @brief Register a fake native module.
 * @param char *name The name of the module. Even though this is a fake module it must be post fixed with the ".lib" extension.
 * @param void *_register A pointer to the register function - can be NULL
 * @param void *_unregister A pointer to the unregister function - can be NULL
 * @param void *_init A pointer to the init function - must not be NULL
 * @param void *_deinit A pointer to the deinit function - must not be NULL
 * @description This function is used to register fake modules from an application. This allows you to
 *              to setup a module to be loaded from an application. Combined with ferite_module_do_preload
 *              this will provide a method of exporting api to scripts using standard module methods.
 */
void ferite_module_register_fake_module( char *name, void *_register, void *_unregister, void *_init, void *_deinit )
{
    FE_ENTER_FUNCTION;
    ferite_current_module->next = ferite_create_module( name, "-fake-module-" );    
    ferite_current_module->next->handle = NULL;
    
    ferite_current_module->next->module_register = _register;
    ferite_current_module->next->module_unregister = _unregister;
    ferite_current_module->next->module_init = _init;
    ferite_current_module->next->module_deinit = _deinit;
    (ferite_current_module->next->module_register)();
    
    ferite_current_module = ferite_current_module->next;
    FE_LEAVE_FUNCTION(NOWT);    
}

void ferite_delete_native_function_record( FeriteScript *script, void *data )
{
    FE_ENTER_FUNCTION;
    if( data != NULL )
      ffree_ngc( data );
    FE_LEAVE_FUNCTION(NOWT);
}

void ferite_module_delete_native_function_list()
{
    FE_ENTER_FUNCTION;
    if( ferite_native_function_hash != NULL )
    {
        ferite_amt_destroy( NULL, ferite_native_function_hash, ferite_delete_native_function_record );
        ferite_native_function_hash = NULL;
    }
    FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_module_find_function
 * @declaration void *ferite_module_find_function( char *name 
 * @brief Locate a function pointer to the specified native function
 * @param char *name The name of the function to find
 * @description The name of the function should be a dot separated path to the function, followed by 
                an underscore and then a list of the types the function accepts. For instance a function
                'f' within the class 'F' that takes a string, a void and a number would be specified by:
                <code>F.f_svn</code>.
 * @return A non-NULL pointer on success, NULL otherwise
 */
void *ferite_module_find_function( char *name )
{
    void *ptr = NULL;
    FeriteNativeFunctionRecord *record = NULL;

    FE_ENTER_FUNCTION;
    record = ferite_hamt_get( NULL, ferite_native_function_hash, name );
    if( record != NULL )
      ptr = record->function;
    FE_LEAVE_FUNCTION(ptr);
}

/**
 * @function ferite_module_add_preload
 * @declaration void ferite_module_add_preload( char *name )
 * @brief Add a module to the list of libraries that are loaded when a script is compiled
 * @param char *name The name of the module
 * @description When a script is compiled, the compiler will, before any script is loaded, 
                go through the pre-load list and load each noted module. This is equivalent
                to pre-pending the script to be loaded with a uses statement for each module.
                <nl/><nl/>
                This function is often used to pre-load modules that you know a script needs
                that the script may not load - such as a script running within an application.
 */
void ferite_module_add_preload( char *name )
{
    FE_ENTER_FUNCTION;
    ferite_stack_push( FE_NoScript, ferite_module_preload_list, fstrdup(name) );
    FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @function ferite_module_do_preload
 * @declaration int ferite_module_do_preload( FeriteScript *script )
 * @brief Preload the modules into the script
 * @param FeriteScript *script The script to load into
 * @description This function, usually called from the compiler, will go through the pre-load
                list created by calling ferite_module_add_preload, and compile them into the
                script.
 */
int ferite_module_do_preload( FeriteScript *script )
{
    int i = 0;

    FE_ENTER_FUNCTION;
    for( i = 0; i <= ferite_module_preload_list->stack_ptr; i++ )
    {
        if( ferite_module_preload_list->stack[i] != NULL )
        {
            if( ferite_load_module( script, script->mainns, ferite_module_preload_list->stack[i] ) <= 0 )
            {
                ferite_error( script, 0, "Attempt to pre-load module '%s' failed.\n", (char *)ferite_module_preload_list->stack[i] );
                FE_LEAVE_FUNCTION(0);
            }
        }
    }
    FE_LEAVE_FUNCTION(1);
}

/**
 * @end
 */
