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
#include <ferite/fmem_jedi.h>
#include <ferite/fmem_libgc.h>


int ferite_is_initialised = 0;
int ferite_pow_lookup[32];
/**
 * @group General
 * @description These are general functions that are critical to the operation of ferite within 
 *			  another application. This includes the initialisation and deinitialisation of the
 *			  ferite engine aswell as tools to extract the data of parameter lists.
 */

/**
 * @function ferite_init
 * @brief First call to the ferite engine to initialise it
 * @declaration int ferite_init( int argc, char **argv )
 * @param int argc Number of arguments
 * @param char **argc Standard array of pointers
 * @description This must be called to allow the engine to work. Without it, things go
 *			  really rather wrong. You can pass arguments into the scripts and control
 *			  the engine.<nl/>
 *			  <nl/>
 *			  --fe-use-classic - this will tell ferite to use malloc/free rather than the jedi memory manager<nl/>
 *			  --fe-debug - tell ferite to dump debug out to stdout, warning: this will produce a lot of output, ferite also has to be compiled with debugging support.<nl/>
 *			  --fe-show-mem-use - tell ferite to dump to stdout a set of memory statistics, this is useful for detecting leaks<nl/>
 *			  <nl/>
 *			  This function can be called multiple times without fear - it will only set things up
 *			  if they are needed.
 * @return	  1 if ferite engine is already initialised.
 */

int ferite_init( int argc, char **argv )
{
	int i = 0;
	int wantDebugBanner = FE_TRUE;
	
	FE_ENTER_FUNCTION;

	if( !ferite_is_initialised )
	{
		int memory_setup = FE_FALSE;
		int gc_setup = FE_FALSE;

		for( i = 0; i < 32; i++ )
			ferite_pow_lookup[i] = (int)pow( (double)2, (double)i );

		if( argv != NULL )
		{
			for( i = 0; i < argc; i++ )
			{
				if( strcmp( argv[i], "--fe-use-classic" ) == 0 )
				{
					memory_setup = FE_TRUE;
					aphex_init( FE_FALSE );
					ferite_classic_memory_init();
#ifdef DEBUG
					fprintf( stderr, "Using classic memory (system malloc/free)\n");
#endif
				}
				if( strcmp( argv[i], "--fe-use-jedi" ) == 0 )
				{
					memory_setup = FE_TRUE;
					aphex_init( FE_FALSE );
					ferite_jedi_memory_init();
#ifdef DEBUG
					fprintf( stderr, "Using jedi memory\n");
#endif
				}
#ifdef HAVE_LIBGC
				if( strcmp( argv[i], "--fe-use-libgc" ) == 0 )
				{
					memory_setup = FE_TRUE;
					gc_setup = FE_TRUE;
					aphex_init( FE_TRUE );
					ferite_libgc_memory_init();
					ferite_init_gc = ferite_init_libgc_gc;
					ferite_deinit_gc = ferite_deinit_libgc_gc;
					ferite_add_to_gc = ferite_add_to_libgc_gc;
					ferite_check_gc = ferite_check_libgc_gc;
					ferite_merge_gc = ferite_merge_libgc_gc;
#ifdef DEBUG
					fprintf( stderr, "Using libgc memory\n");
#endif
				}
#endif
				if( strcmp( argv[i], "--fe-use-std-gc" ) == 0 )
				{
					gc_setup = FE_TRUE;
					ferite_init_gc = ferite_init_std_gc;
					ferite_deinit_gc = ferite_deinit_std_gc;
					ferite_add_to_gc = ferite_add_to_std_gc;
					ferite_check_gc = ferite_check_std_gc;
					ferite_merge_gc = ferite_merge_std_gc;
				}
				if( strcmp( argv[i], "--fe-use-generation-gc" ) == 0 )
				{
					gc_setup = FE_TRUE;
					ferite_init_gc = ferite_init_generation_gc;
					ferite_deinit_gc = ferite_deinit_generation_gc;
					ferite_add_to_gc = ferite_add_to_generation_gc;
					ferite_check_gc = ferite_check_generation_gc;
					ferite_merge_gc = ferite_merge_generation_gc;
				}
				if( strcmp( argv[i], "--fe-debug" ) == 0 )
				  ferite_show_debug = 1;
				if( strcmp( argv[i], "--fe-show-mem-use" ) == 0 )
				  ferite_hide_mem_use = 0;
				if( strcmp( argv[i], "--fe-use-mm-with-pcre" ) == 0 )
				  ferite_use_mm_with_pcre = 1;
				if( strcmp( argv[i], "--fe-hide-debug" ) == 0 )
					wantDebugBanner = FE_FALSE;
				if( strcmp( argv[i], "--fe-show-partial-implementation") == 0 )
					ferite_show_partial_implementation = FE_TRUE;
			}
		}

		if( !memory_setup ) {
			memory_setup = FE_TRUE;
#ifdef HAVE_LIBGC
			gc_setup = FE_TRUE;
			aphex_init( FE_TRUE );
			ferite_libgc_memory_init();
			ferite_init_gc = ferite_init_libgc_gc;
			ferite_deinit_gc = ferite_deinit_libgc_gc;
			ferite_add_to_gc = ferite_add_to_libgc_gc;
			ferite_check_gc = ferite_check_libgc_gc;
			ferite_merge_gc = ferite_merge_libgc_gc;
#ifdef DEBUG
			fprintf( stderr, "No memory/GC option specified - defaulting to libgc\n");
#endif
#else
			aphex_init( FE_FALSE );
			ferite_jedi_memory_init();
#ifdef DEBUG
			fprintf( stderr, "No memory/GC option specified - defaulting to jedi\n");
#endif
#endif
		}
		if( !gc_setup ) {
			gc_setup = FE_TRUE;
			ferite_init_gc = ferite_init_generation_gc;
			ferite_deinit_gc = ferite_deinit_generation_gc;
			ferite_add_to_gc = ferite_add_to_generation_gc;
			ferite_check_gc = ferite_check_generation_gc;
			ferite_merge_gc = ferite_merge_generation_gc;
		}

#ifdef DEBUG
		if( wantDebugBanner )
			fprintf( stderr, "This a Debug Build.\n" );
#endif

		if( !ferite_init_module_list() )
		{
			fprintf( stderr, "Unable to initialise the module subsystem\n" ); /* FIXME: replace with something nicer */
			ferite_memory_deinit();
			FE_LEAVE_FUNCTION( ferite_is_initialised );
		}

		ferite_init_compiler();
		ferite_init_regex();
		ferite_set_script_argv( 0, NULL );

		ferite_is_initialised = 1;
	}
	FE_LEAVE_FUNCTION( ferite_is_initialised );
}

/**
 * @function ferite_deinit
 * @declaration int ferite_deinit()
 * @brief Shuts the ferite engine down, clears the garbage collector and frees up unused memory
 * @description Use this when the engine is no longer needed. Once this is called you must not call
				any other ferite engine functions. If you wish to re-use ferite after calling this 
				function it is necessary to call ferite_init() again.
 */
int ferite_deinit()
{
	FE_ENTER_FUNCTION;
	if( ferite_is_initialised )
	{
		ferite_variable_destroy( NULL, ferite_ARGV );
		ferite_deinit_module_list();
		ferite_memory_deinit();
		ferite_deinit_regex();
		ferite_deinit_compiler();
		ferite_is_initialised = 0;
	}
	FE_LEAVE_FUNCTION( !ferite_is_initialised );
}

/**
 * @function ferite_get_parameters
 * @declaration int ferite_get_parameters( FeriteVariable **list, int num_args, ... )
 * @brief Get the ferite variables from a list and put them into pointers to real C variables
 * @param FeriteVariable **list The list of variables passed to the function calling this method
 * @param int num_args The number of arguments you wish to extract from the list
 * @param ... Pointers to the real C variables
 * @description This method is very useful for translation from the internal variable format to a standard C variable.
 * @warning Numbers can be either double or long so these need to be handled carefully
 * @example <nl/><code>
	char *str;<nl/>
	FeriteClass *klass;<nl/>
	ferite_get_parameters( params, 2, &str, &klass );<nl/>
   </code>
 */
int ferite_get_parameters( FeriteVariable **list, int num_args, ... )
{
	va_list  ap;
	void	*vptr;
	int	  current_arg = 0;

	FE_ENTER_FUNCTION;

	if( num_args < 1 )
	{
		FE_LEAVE_FUNCTION(1);
	}
	if( list != NULL )
	{
		va_start( ap, num_args );
		while( current_arg < num_args && list[current_arg] != NULL )
		{
			if( list[current_arg] != NULL )
			{
				switch( F_VAR_TYPE(list[current_arg]) )
				{
				  case F_VAR_LONG:
					vptr = va_arg( ap, double * );
					if( vptr == NULL ) break;
					*((double *)vptr) = (long)(list[current_arg]->data.lval);
					break;
				  case F_VAR_DOUBLE:
					vptr = va_arg( ap, double * );
					if( vptr == NULL ) break;
					*((double *)vptr) = list[current_arg]->data.dval;
					break;
				  case F_VAR_STR:
					vptr = va_arg( ap, void ** );
					if( vptr == NULL ) break;
					*((void **)vptr) = list[current_arg]->data.sval;
					break;
				  case F_VAR_OBJ:
				  case F_VAR_UARRAY:
				  case F_VAR_CLASS:
				  case F_VAR_NS:
					vptr = va_arg(ap, void ** );
					if( vptr == NULL ) break;
					*((void **)vptr) = list[current_arg]->data.pval;
					break;
				  case F_VAR_VOID:
					vptr = va_arg(ap, void ** );
					if( vptr == NULL ) break;
					break;
				  case F_VAR_BOOL:
					vptr = va_arg( ap, char * );
					if( vptr == NULL ) break;
					*((char *)vptr) = (char)(list[current_arg]->data.lval);
					break;					
				}
			}
			else
			  break;
			current_arg++;
		}
		va_end( ap );
		FE_LEAVE_FUNCTION( current_arg );
	}
	else
	{
		FE_LEAVE_FUNCTION( -1 );
	}
}

/**
 * @function ferite_get_parameter_count
 * @declaration int ferite_get_parameter_count( FeriteVariable **list )
 * @brief Returns the number of paramters within a list
 * @param FeriteVariable **list The list of variables passed to the function calling this method
 * @description This method is useful for obtaining the number of variables passed to the function. 
				It is suggested that this method is used rather than
 *			  any other as it will allow for underneath changes without breaking existing code.
 */
int ferite_get_parameter_count( FeriteVariable **list )
{
	int argcount = 0;

	FE_ENTER_FUNCTION;
	if( list != NULL )
	{
		while( list[argcount++] != NULL );
		FE_LEAVE_FUNCTION( --argcount );
	}
	else
	{
		FE_LEAVE_FUNCTION( -1 );
	}
}

/**
 * @function ferite_get_required_parameter_list_size
 * @declaration int ferite_get_required_parameter_list_size( FeriteScript *script, FeriteFunction *function )
 * @brief Given a pointer to a function, this will return the expected number of parameters.
 * @param FeriteScript *script The current script
 * @param FeriteFunction *function The function to check
 * @return int The expected count.
 */
int ferite_get_required_parameter_list_size( FeriteScript *script, FeriteFunction *function )
{
   FE_ENTER_FUNCTION;
   FE_LEAVE_FUNCTION( function->arg_count );
}


/**
 * @function ferite_get_parameter
 * @declaration void *ferite_get_parameter( FeriteVariable **list, int num )
 * @brief Get the nth parameter from the parameter list
 * @param FeriteVariable **list The list
 * @param int num The variable to get
 * @description This function will make sure that there isn't an 'out-of-bounds' fault on the parameter array. If you
				are confident of the parameter size it is suggested that you just access the parameter directly.
 */
void *ferite_get_parameter( FeriteVariable **list, int num )
{
	FE_ENTER_FUNCTION;
	if( list != NULL )
	{
		if( num < ferite_get_parameter_count(list) )
		{
			FE_LEAVE_FUNCTION( list[num] );
		}
	}
	FE_LEAVE_FUNCTION( NULL );
}

void ferite_show_help()
{
	FE_ENTER_FUNCTION;
#ifdef DEBUG
	printf( "\nPlease Note: This a Debug Build of ferite\n" );
#endif
	printf( "\nferite specific features:\n\n" );
#ifdef DEBUG
	printf( " --fe-debug	   \t	 Run in debugging-mode.\n" );
#endif
	printf( " --fe-use-classic \t	 Run w/ alternate MM mode. (enables allocation tracking; will cause slow downs)\n" );
	printf( " --fe-use-std-gc  \t	 Run w/ simple GC mode. (will cause slow downs)\n" );
	printf( " --fe-show-mem-use\t	 Report memory use at script end.\n" );
	printf( " --fe-use-mm-with-pcre\t Use PCRE [Regular Expression Engine] with ferite's MM\n" );
	printf( "\n MM = Memory Manager\n" );
	FE_LEAVE_FUNCTION( NOWT );
}

/**
 * @function ferite_set_script_argc
 * @declaration void ferite_set_script_argv( int argc, char **argv )
 * @brief Set the argv array that scripts can access
 * @param int argc The number of arguments
 * @param char **argv The arguments
 * @description The 'argv' array within scripts is used to pass values from outside thee
 *			  script to the inside - much like the way arguments are passed to a C program.
 *			  This function will iterate through the arguments and add them to the globally
				scoped argv variable. When a script is compiled, the array will be copied for
				that script. This means that it is possible to change the arguments to new
				scripts but not existingly compiled scripts.
 */
void ferite_set_script_argv( int argc, char **argv )
{
	int i = 0;
	FeriteScript *script = NULL;
	FE_ENTER_FUNCTION;
	if( ferite_ARGV != NULL )
		ferite_variable_destroy( NULL, ferite_ARGV );

	ferite_ARGV = ferite_create_uarray_variable( NULL, "argv", argc, FE_STATIC );
	if( argv != NULL )
	{
		for( i = 0; i < argc; i++ )
		{
			ferite_uarray_add( NULL, VAUA(ferite_ARGV), fe_new_str(argv[i], argv[i], 0, FE_CHARSET_DEFAULT), NULL, FE_ARRAY_ADD_AT_END );
		}
	}
	FE_LEAVE_FUNCTION(NOWT);
}

/**
 * @end
 */
