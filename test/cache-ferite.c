/*
 * Copyright (C) 2000-2004 Chris Ross and various contributors
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

#include <ferite.h>
#include "aphex.h"

#ifndef WIN32
#ifdef HAVE_CONFIG_HEADER
# include "config.h"
#endif
# include <sys/time.h>
# include <signal.h>
#else
# include <windows.h>
# include <direct.h>

# ifdef NATIVE_LIBRARY_PATH
#  undef NATIVE_LIBRARY_PATH
# endif
# define NATIVE_LIBRARY_DIR getLibraryPath(1)
# ifdef XPLAT_LIBRARY_DIR
#  undef XPLAT_LIBRARY_DIR
# endif
# define XPLAT_LIBRARY_DIR  getLibraryPath(2)
# ifdef LIBRARY_DIR
#  undef LIBRARY_DIR
# endif
# define LIBRARY_DIR		getLibraryPath(0)

# define PACKAGE   "cache-" FERITE_NAME
# define VERSION   FERITE_VERSION
# define PLATFORM  getWindowsPlatform()

char *getWindowsPlatform()
{
	static char buf[1024];
	char *b = buf;
	sprintf( b, "Microsoft Windows %d.%d (%d)", _winmajor, _winminor, _osver );
	return b;
}

char *getLibraryPath( int native )
{
	static char path[1024];
	char buf[1024], *b = buf;
	char *p = path;

	_getcwd( b, 1024 );
	switch( native ) {
	case 0: /* Library dir */
		sprintf( p, "%s", getenv("FERITE_LIBRARY_PATH") );
		break;
	case 1: /* Native */
		sprintf( p, "%s", getenv("FERITE_MODULE_NATIVE_PATH") );
		break;
	case 2: /* xplat */
		sprintf( p, "%s", getenv("FERITE_MODULE_XPLAT_PATH") );
		break;
	}
	return p;
}

#endif

struct
{
	char *scriptname;
	int verbose;
	int showargs;
	int estring;
	int compile_only;
	int show_warnings;
	int sleep;
	int iterations;
}
opt;

FeriteScript *script = NULL;


void print_version()
{
	printf( "%s %s [built: %s %s] [%s]\n", PACKAGE, VERSION, __DATE__, __TIME__, PLATFORM );
	printf( "  Base Cross Platform Library Path: %s\n", XPLAT_LIBRARY_DIR );
	printf( "  Base Native Library Path: %s\n", NATIVE_LIBRARY_DIR ); 
}

void print_copyright()
{
	printf( "%s - Copyright (c) 1999-2007, Chris Ross\n", PACKAGE );
}

void print_usage()
{
	printf( "Usage: %s [ferite options] [script] [-- script arguments]\n", PACKAGE );
}

void show_help()
{
	print_version();
	printf( "\n" );
	print_usage();
	printf( "\n" );
	printf( " Long Version       Short	 Description\n" );
	printf( "----------------------------------------------------------------\n" );
	printf( " --help             -h        Show this help screen.\n" );
	printf( " --verbose          -v        Be verbose when running.\n" );
	printf( " --showargs         -s        Show all the arguments passed to ferite.\n" );
	printf( " --execute          -e        Execute the script supplied on the command line.\n" );
	printf( " --include path     -Ipath    Add 'path' to ferite's search path for scripts. [effects uses's usage]\n" );
	printf( " --native-inc path  -Npath    Add 'path' to ferite's search path for native modules. [effects uses's usage]\n" );
	printf( " --preload module   -Pmodule  Get ferite to preload 'module' at compile time. [same as a 'uses' statement]\n" );
	printf( " --compile-only     -c        Only compile script - check for syntax.\n" );
	printf( " --hide-warnings    -w        Hide warnings.\n" );
	printf( " --iterations N     -i N      Number of times to re-run the targeted script; default: 10.\n" );
	printf( " --sleep                      Sleep for 10 seconds once completed.\n" );
	printf( " --version                    Print the version.\n" );
	printf( " --copyright                  Print out the copyright.\n" );
	printf( "\nTo pass options to the script, add them to the end of the command line after a '--', otherwise no\n" );
	printf( "will be in the 'argv' array in the script.\n" );
	ferite_show_help();
	printf( "\n" );
}

int parse_args( int argc, char **argv )
{
	int i, gotScript = 0;
	char *buf = NULL;

	opt.verbose = FE_FALSE;
	opt.showargs = FE_FALSE;
	opt.estring = FE_FALSE;
	opt.compile_only = FE_FALSE;
	opt.show_warnings = FE_TRUE;
	opt.sleep = FE_FALSE;
	opt.scriptname = NULL;
	opt.iterations = 10;
	
	for(i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			if( (!strcmp( argv[i], "--")) )
			{
				argc = i+1;
				break;
			}
			if((!strcmp(argv[i], "--verbose")) || (!strcmp(argv[i], "-v")))
				opt.verbose = FE_TRUE;
			if((!strcmp(argv[i], "--showargs")) || (!strcmp(argv[i], "-s")))
				opt.showargs = FE_TRUE;
			if((!strcmp(argv[i], "--help")) || (!strcmp(argv[i], "-h")))
			{
				show_help();
				exit(0);
			}
			if( !strcmp(argv[i], "--version") )
			{
				print_version();
				exit(0);
			}
			if( !strcmp(argv[i], "--copyright") )
			{
				print_copyright();
				exit(0);
			}
			if((!strcmp(argv[i], "--execute")) || (!strcmp(argv[i], "-e")) &&
			   argv[i + 1])
			{
				opt.scriptname = aphex_strdup( argv[++i] );
				opt.estring = FE_TRUE;
				gotScript = FE_TRUE;
			}
			if( !strcmp(argv[i], "--include" ) )
			{
				ferite_add_library_search_path( argv[++i] );
				if( opt.verbose )
				  printf( "--> adding '%s' as a library search path\n", argv[i] );
			}
			if( !strncmp( argv[i], "-I", 2 ) )
			{
				ferite_add_library_search_path( argv[i]+2 );
				if( opt.verbose )
				  printf( "--> adding '%s' as a library search path\n", argv[i]+2 );
			}
			if( !strcmp(argv[i], "--native-inc" ) )
			{
				ferite_set_library_native_path( argv[++i] );
				if( opt.verbose )
					printf( "--> adding '%s' as a native library search path\n", argv[i] );
			}
			if( !strncmp( argv[i], "-N", 2 ) )
			{
				ferite_set_library_native_path( argv[i]+2 );
				if( opt.verbose )
					printf( "--> adding '%s' as a native library search path\n", argv[i]+2 );
			}
			if((!strcmp(argv[i], "--compile-only")) || (!strcmp(argv[i], "-c")))
			{
				opt.compile_only = FE_TRUE;
			}
			if((!strcmp(argv[i], "--iterations")) || (!strcmp(argv[i], "-i")))
			{
				opt.iterations = atol(argv[++i]);
			}
			if((!strcmp(argv[i], "--hide-warnings")) || (!strcmp(argv[i], "-w")))
			{
				opt.show_warnings = FE_FALSE;
			}
			if((!strcmp(argv[i], "--sleep")))
			{
				opt.sleep = FE_TRUE;
			}
			if( !strcmp(argv[i], "--preload" ) )
			{
				ferite_module_add_preload( argv[++i] );
				if( opt.verbose )
				  printf( "--> adding '%s' as a pre-loaded module\n", argv[i] );
			}
			if( !strncmp( argv[i], "-P", 2 ) )
			{
				ferite_module_add_preload( argv[i]+2 );
				if( opt.verbose )
				  printf( "--> adding '%s' as a pre-loaded module\n", argv[i]+2 );
			}
		}
		else
		{
			if( !gotScript )
			{
				gotScript = 1;
				opt.scriptname = aphex_relative_to_absolute( argv[i] );
			}
		}
	}

	if(!opt.scriptname)
	{
		print_version();
		printf( "\n" );
		print_usage();
		printf( "\n\tYou need to supply a file name. For help try ferite --help\n\n" );
		exit( -1 );
	}
	return argc;
}

void sig_ctrl( int sig )
{
	ferite_error( script, 0, "Ctrl+C Pressed\n" );
}

inline static double get_time(void)
{
    struct timeval timev;
    gettimeofday(&timev, NULL);
    return (double) timev.tv_sec +
      (((double) timev.tv_usec) / 1000000);
}

int main( int argc, char **argv )
{
	int i = 0, retval = 0;
	char *errmsg = NULL, *tmp = NULL;
    double start = 0, end = 0;
    double compile_diff, dup_diff;

#ifndef WIN32
	signal( SIGTERM, sig_ctrl );
	signal( SIGINT, sig_ctrl );
#endif

	if ( argc < 2 )
	{
		print_version();
		printf( "\n" );
		print_usage();
		printf( "\n\tYou need to supply a file name. For help try ferite --help\n\n" );
		exit( 3 );
	}
	else
	{
		if( opt.verbose )
			printf( "--> initialising ferite\n");

			/* initialise the ferite engine */
		if( ferite_init( argc, argv ) )
		{

			if( opt.verbose )
				printf( "---> turning on compile cache\n" );
			ferite_cache_toggle(FE_TRUE);

#ifdef AUTOLOAD_CORE
			if( opt.verbose )
				printf( "---> preloading core modules:\n" );

			if( opt.verbose ) printf( "	  :array\n" );
			ferite_module_add_preload( "array" );
			if( opt.verbose ) printf( "	  :console\n" );
			ferite_module_add_preload( "console" );
			if( opt.verbose ) printf( "	  :filesystem\n" );
			ferite_module_add_preload( "filesystem" );
			if( opt.verbose ) printf( "	  :math\n" );
			ferite_module_add_preload( "math" );
			if( opt.verbose ) printf( "	  :regexp\n" );
			ferite_module_add_preload( "regexp" );
			if( opt.verbose ) printf( "	  :string\n" );
			ferite_module_add_preload( "string" );
			if( opt.verbose ) printf( "	  :sys\n" );
			ferite_module_add_preload( "sys" );
			if( opt.verbose ) printf( "	  :xml\n" );
			ferite_module_add_preload( "xml" );	
#endif
			/* parse the arguments */
			i = parse_args( argc, argv );			
			
			/*
			 * add the system wide directory for scripts and modules eg, console.fec and sys.fec
			 * this is where the modules are looked for.
			 */
			ferite_add_library_search_path( XPLAT_LIBRARY_DIR );
			if( opt.verbose )
				printf( "--> Adding '%s' as a library search path\n", LIBRARY_DIR );
			
			/*
			 * add the system wide directory for native modules eg. console.so and sys.so
			 * this is where foo.lib is looked for.
			 */
			ferite_set_library_native_path( NATIVE_LIBRARY_DIR );
			if( opt.verbose )
				printf( "--> Setting '%s' as the native library search path\n", NATIVE_LIBRARY_DIR );
			

			/* setup the arguments to be passed to the scripts */
			ferite_set_script_argv( argc - i, argv+i );

			/* check to see if we should show our arguments */
			if( opt.showargs )
			{
				printf( "Argument Count: %d %d\n", argc, i );
				for( ; i < argc; i++ )
					printf( "Argument #%d: %s\n", i+1, argv[i] );
			}

			if( opt.verbose )
				printf( "--> Compiling script\n" );

			if( !opt.estring ) {
				int   attempt_cache = FE_TRUE;
				char *buf = strdup( opt.scriptname );
				char *ext = strrchr( buf, DIR_DELIM );
				if( ext != NULL )
					*ext = '\0';

				if( opt.verbose )
					printf( "  --> File '%s'\n", opt.scriptname );

				/* Add a library path for the directory the script lives in */
				ferite_add_library_search_path( buf );
				/* compile the script file */
				
				start = get_time();
			    script = ferite_script_compile( opt.scriptname );
				/* check to see if there is a compile error */
				if( !ferite_has_compile_error( script ) )
				{
					/* If we want warnings to be shown */
					if( ferite_has_warnings( script ) && opt.show_warnings )
					{
						errmsg = ferite_get_warning_string( script );
						fprintf( stderr, "[ferite: compilation]\n%s", errmsg );
						ffree( errmsg );
					}
					ferite_reset_warnings( script );

					if( !opt.compile_only )
					{
						if( opt.verbose )
							printf( "--> Executing script\n" );

						/* execute script */
						ferite_script_execute( script );

						/* check to see if there is a runtime error */
						if( ferite_has_runtime_error( script ) )
						{
							/* output all errors and warnings */
							attempt_cache = FE_FALSE;
							errmsg = ferite_get_error_log( script );
							fprintf( stderr, "[ferite: execution]\n%s", errmsg );
							ffree( errmsg );
							retval = 2;
						}
						else
						{
							retval = script->return_value;

							/* If we want warnings to be shown */
							if( ferite_has_warnings( script ) && opt.show_warnings )
							{
								errmsg = ferite_get_warning_string( script );
								fprintf( stderr, "[ferite: execution]\n%s", errmsg );
								ffree( errmsg );
							}
							ferite_reset_warnings( script );
						}
					}
					else
					{
						fprintf( stderr, "SUCCESS: Script compilation successfull.\n" );
					}
				}
				else
				{
					/* output the compile errors */
					errmsg = ferite_get_error_log( script );
					fprintf( stderr, "[ferite: compile]\n%s", errmsg );
					ffree( errmsg );
					if( opt.compile_only )
						fprintf( stderr, "FAIL: Script compilation failed.\n" );
					retval = 1;
					attempt_cache = FE_FALSE;
				}
				if( opt.verbose )
					printf( "--> Destroying script object\n" );

				/* delete the script */
				ferite_script_delete( script );

			    end = get_time();
			    compile_diff = end - start;
			    printf( "initial compile time: start %f, end %f, difference %f\n", start, end, compile_diff );

				if( attempt_cache ) {
					for( i = 0; i < opt.iterations; i++ ) {
					    start = get_time();
					    script = ferite_script_compile( opt.scriptname );
						if( !ferite_has_compile_error( script ) )
						{
							/* If we want warnings to be shown */
							if( ferite_has_warnings( script ) && opt.show_warnings )
							{
								errmsg = ferite_get_warning_string( script );
								fprintf( stderr, "[ferite: compilation]\n%s", errmsg );
								ffree( errmsg );
							}
							ferite_reset_warnings( script );

							if( !opt.compile_only )
							{
								if( opt.verbose )
									printf( "--> Executing script\n" );

								/* execute script */
								ferite_script_execute( script );

								/* check to see if there is a runtime error */
								if( ferite_has_runtime_error( script ) )
								{
									/* output all errors and warnings */
									attempt_cache = FE_FALSE;
									errmsg = ferite_get_error_log( script );
									fprintf( stderr, "[ferite: execution]\n%s", errmsg );
									ffree( errmsg );
									retval = 2;
								}
								else
								{
									retval = script->return_value;

									/* If we want warnings to be shown */
									if( ferite_has_warnings( script ) && opt.show_warnings )
									{
										errmsg = ferite_get_warning_string( script );
										fprintf( stderr, "[ferite: execution]\n%s", errmsg );
										ffree( errmsg );
									}
									ferite_reset_warnings( script );
								}
							}
							else
							{
								fprintf( stderr, "SUCCESS: Script compilation successfull.\n" );
							}
						}
						else
						{
							/* output the compile errors */
							errmsg = ferite_get_error_log( script );
							fprintf( stderr, "[ferite: compile]\n%s", errmsg );
							ffree( errmsg );
							if( opt.compile_only )
								fprintf( stderr, "FAIL: Script compilation failed.\n" );
							retval = 1;
							attempt_cache = FE_FALSE;
						}
						if( opt.verbose )
							printf( "--> Destroying script object\n" );

						/* delete the script */
						ferite_script_delete( script );
						
					    end = get_time();
					    dup_diff += end - start;
					}
					dup_diff = dup_diff / opt.iterations;
					printf("average cache compile time: %f\n", dup_diff);
				    printf( "cache compile is %f%% faster than compilation\n", (compile_diff/dup_diff) * 100 );
				}
				
				free( buf );
			}

			/* deinitialise the engien */
			ferite_deinit();
			aphex_free( opt.scriptname );
		}
		else
		{
			fprintf( stderr, "Unable to initialise the ferite engine!\n" );
			retval = 4;
		}

#ifndef WIN32
		signal( SIGTERM, SIG_DFL );
		signal( SIGINT, SIG_DFL );
#endif
		if( opt.sleep ) {
			fprintf( stderr, "Sleeping...\n" );
			sleep(2048);
			fprintf( stderr, "Waking up...\n" );
		}
		
		if( opt.verbose )
			printf( "--> Quiting Application\n" );

	}
	return retval;
}
