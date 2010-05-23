/*
 * Copyright (C) 2000-2005 Chris Ross and various contributors
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

/* WARNING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * This code is terible, I wouldn't show it to anyone, it needs to be
 * re-written actually in ferite or something. But, it does work! So
 * please forgive me. Infact, I would look anywhere but here for examples of
 * my coding skillz! =P
 */


#include <ferite.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "builder.h"
#include "aphex.h"

#if defined(HAVE_CONFIG_HEADER)
# include "config.h"
#endif

#ifdef HAVE_LIBGC
#define GC_THREADS
#define GC_REDIRECT_TO_LOCAL
#include "gc.h"
#endif

#ifndef WIN32
#  include <sys/time.h>
#else
# include <direct.h>

# define NATIVE_LIBRARY_DIR getLibraryPath(1)
# define XPLAT_LIBRARY_DIR  getLibraryPath(2)
# define LIBRARY_DIR        getLibraryPath(0)

# define PACKAGE   FERITE_NAME
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
		sprintf( p, "%s\\lib", b );
		break;
	case 1: /* Native */
		sprintf( p, "%s\\lib\\module-native\\win32", b );
		break;
	case 2: /* xplat */
		sprintf( p, "%s\\lib\\module-source", b );
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
}
opt;

FeriteStack   *class_stack = NULL;
FeriteStack   *file_stack = NULL;
BuilderModule *current_module = NULL;
char           module_init_name[8096];
char           file_list[8096];
char           internal_file_list[8096];
char           dist_file_list[8096];
FILE          *module_current_c_file = NULL;
int            in_class = 0;
int            want_deinit = 1;
int            want_makefile = 1;
int            want_configm4 = 1;
int            force_makefile = 0;
int            want_xml = 1;
int            is_open = FE_TRUE;
char           default_builder_path[] = "@BUILDER@";
char           default_module_path[] = "@MODULE_SRC_PREFIX@";
char          *builder_path = NULL, *module_path = NULL;
char           generated_file_prefix[8096];

void print_version()
{
    printf( "%s [%s] %s [built: %s %s] [%s]\n", "builder", PACKAGE, FERITE_VERSION, __DATE__, __TIME__, PLATFORM );
}

void print_copyright()
{
    printf( "%s (%s) - Copyright (c) 1999-2005, Chris Ross\n", PACKAGE, "builder" );
}

void print_usage()
{
    printf( "Usage: %s [OPTIONS] [FILE]\n", "builder" );
}

void show_help()
{
    print_version();
    print_copyright();
    printf( "\n" );
    print_usage();

    printf( "\n\tLong Version      Short      Description\n" );
    printf( "----------------------------------------------------------------\n" );
    printf( "\t--help              -h       Show this help screen.\n" );
    printf( "\t--verbose           -v       Be verbose when running.\n" );
    printf( "\t--showargs          -s       Show all the arguments passed to ferite.\n" );
    printf( "\t--execute           -e       Execute the script supplied on the command line\n" );
    printf( "\t--module-name       -m       Name of the module to be generated (default: modulename)\n" );
    printf( "\t--add-file          -a       Add an external file to the generated makefile\n" );
    printf( "\t--init-name         -i       Change the name of the default module init function (default: module_init)\n" );
    printf( "\t--no-deinit         -d       Stop builder generating a module_deinit function\n" );
    printf( "\t--no-makefile       -f       Stop builder generating a 'Makfile.am'\n" );
    printf( "\t--no-configm4       -nc      Stop builder generating a 'config.m4'\n" );
    printf( "\t--no-xml            -x       Stop builder generating a 'modulename.xml'\n" );
    printf( "\t--force-makefile    -k       Force builder to generate a 'Makefile.am'\n" );
    printf( "\t--add-dist-file     -A       Add a file to the distributed files.\n" );
    printf( "\t--builder-path      -b       The value to use for the builder location in the Makefile.am (default: @BUILDER@).\n" );
    printf( "\t--source-prefix     -p       The value to use for the src prefix in the Makefile.am (default: @MODULE_SRC_PREFIX@)\n" );
    printf( "\t--closed            -c       Generate source code to generate .fec file structure without needing the .fec\n" );
    printf( "\t--file-prefix       -fp      Place each file generated in the directory given\n" );
    printf( "\t--version                    Print out the version\n" );
    printf( "\t--copyright                  Print out the copyright\n" );
    printf( "\n" );

    ferite_show_help();
    exit(0);
}

void parse_args( int argc, char **argv )
{
    int i = 0;
    char *buf = NULL;

    opt.verbose = 0;
    opt.showargs = 0;
    opt.estring = 0;
	opt.scriptname = NULL;
    is_open = FE_TRUE;
    strcpy( current_module->name, "modulename" );
    strcpy( module_init_name, "modulename_register" );
	
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if((!strcmp(argv[i], "--verbose")) || (!strcmp(argv[i], "-v")))
              opt.verbose = 1;
            if((!strcmp(argv[i], "--showargs")) || (!strcmp(argv[i], "-s")))
              opt.showargs = 1;
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
            if((!strcmp(argv[i], "--execute")) || (!strcmp(argv[i], "-e")))
            {
                opt.scriptname = strdup( argv[++i] );
                opt.estring = 1;
            }
            if((!strcmp(argv[i], "--module-name")) || (!strcmp(argv[i], "-m")))
            {
                strcpy( current_module->name, argv[++i] );
                if( strcmp( module_init_name, "modulename_register" ) == 0 )
                {
                    sprintf( module_init_name, "ferite_%s_register", current_module->name );
                }
            }
            if((!strcmp(argv[i], "--add-file")) || (!strcmp(argv[i], "-a")))
            {
                strcat( file_list, argv[++i] ); strcat( file_list, " " );
            }
            if((!strcmp(argv[i], "--add-dist-file")) || (!strcmp(argv[i], "-A")))
            {
                strcat( dist_file_list, argv[++i] ); strcat( dist_file_list, " " );
            }
            if((!strcmp(argv[i], "--init-name")) || (!strcmp(argv[i], "-i")))
            {
                strcpy( module_init_name, argv[++i] );
            }
            if((!strcmp(argv[i], "--no-deinit")) || (!strcmp(argv[i], "-d")))
            {
                want_deinit = 0;
            }
            if((!strcmp(argv[i], "--no-makefile")) || (!strcmp(argv[i], "-f")))
            {
                want_makefile = 0;
            }
            if((!strcmp(argv[i], "--no-configm4")) || (!strcmp(argv[i], "-nc")))
            {
                want_configm4 = 0;
            }
            if((!strcmp(argv[i], "--no-xml")) || (!strcmp(argv[i], "-x")))
            {
                want_xml = 0;
            }
            if((!strcmp(argv[i], "--closed")) || (!strcmp(argv[i], "-cc")))
            {
                is_open = FE_FALSE;
            }
            if((!strcmp(argv[i], "--force-makefile")) || (!strcmp(argv[i], "-k")))
            {
                force_makefile = 1;
            }
            if((!strcmp(argv[i], "--builder-path")) || (!strcmp(argv[i], "-b")))
            {
                if( builder_path != NULL )
                  free( builder_path );
                builder_path = strdup( argv[++i] );
            }
            if((!strcmp(argv[i], "--source-prefix")) || (!strcmp(argv[i], "-p")))
            {
                if( module_path != NULL )
                  free( module_path );
                module_path = strdup( argv[++i] );
            }
            if((!strcmp(argv[i], "--file-prefix")) || (!strcmp(argv[i], "-fp")))
            {
                sprintf( generated_file_prefix, "%s", argv[++i] );
            }
        }
        else
        {
            buf = aphex_relative_to_absolute( argv[i] );
            printf( "builder: processing filename %s\n", buf );
            opt.scriptname = buf;
        }
    }

    if(!opt.scriptname)
    {
        print_version();
        print_copyright();
        printf( "\n" );
        print_usage();
        printf( "\n\tYou need to supply a file name. For help try builder --help\n\n" );
        exit( -1 );
    }

    if( builder_path == NULL )
      builder_path = strdup( default_builder_path );
    if( module_path == NULL )
      module_path = strdup( default_module_path );
}

FILE *builder_fopen( char *filename, char *modes )
{
    FILE *ptr = NULL;
    char path[PATH_MAX];
    
	FE_ENTER_FUNCTION;
    sprintf( path, "%s%s", generated_file_prefix, filename );
	printf("builder: trying to open %s\n", path);
    ptr = fopen( path, modes );
	if( ptr == NULL ) {
		printf("builder: unable to open %s - exiting\n", path);
	}
    FE_LEAVE_FUNCTION(ptr);
}
int builder_stat( char *filename )
{
    int r = 0;
    char path[PATH_MAX];
    struct stat sbuf;
    
    FE_ENTER_FUNCTION;
    sprintf( path, "%s%s", generated_file_prefix, filename );
    r = stat( path, &sbuf );
    FE_LEAVE_FUNCTION(r);
}

char *builder_generate_current_name( int to_end, int want_fe_prefix )
{
    int i, end = current_module->name_stack->stack_ptr;
    static char nameBuffer[1024];

    FE_ENTER_FUNCTION;
    nameBuffer[0] = '\0';
    if( to_end == FE_FALSE )
      end--;

    if( want_fe_prefix )
        strcat( nameBuffer, "ferite_" );
    for( i = 1; i <= end; i++ )
    {
        strcat( nameBuffer, current_module->name_stack->stack[i] );
        strcat( nameBuffer, "_" );
    }
    nameBuffer[strlen(nameBuffer)-1] = '\0';
    FE_LEAVE_FUNCTION( nameBuffer );
}

char *builder_generate_current_function_name()
{
    int i;
    static char nameBuffer[1024];

    FE_ENTER_FUNCTION;
    nameBuffer[0] = '\0';
    for( i = 2; i <= current_module->name_stack->stack_ptr; i++ )
    {
        strcat( nameBuffer, current_module->name_stack->stack[i] );
        strcat( nameBuffer, "." );
    }
    nameBuffer[strlen(nameBuffer)-1] = '\0';
    FE_LEAVE_FUNCTION( nameBuffer );
}

void builder_process_variable( FeriteScript *script, FeriteVariable *var, char *parent )
{
    FE_ENTER_FUNCTION;
    if( strcmp( parent, "'module-init" ) == 0 ||
        strcmp( parent, "'module-deinit" ) == 0 ||
        strcmp( parent, "'module-header" ) == 0 ||
        strcmp( parent, "'module-register" ) == 0 ||
        strcmp( parent, "'module-unregister" ) == 0 ||
        strcmp( parent, "'module-flags" ) == 0 )
    {
        FE_LEAVE_FUNCTION(NOWT);
    }

    ferite_stack_push( FE_NoScript, current_module->name_stack, parent );
    if( opt.verbose )
      printf( "%s - processing variable\n", builder_generate_current_name(FE_TRUE,FE_TRUE) );
    fprintf( current_module->core, "      %s( script, %s%s, ",
             (in_class == 1 ? "ferite_register_class_variable" : "ferite_register_ns_variable" ),
             builder_generate_current_name(FE_FALSE,FE_TRUE),
             (in_class == 1 ? "_class" : "_namespace") );

#define DO_STATIC  ( in_class == 1 ? ( FE_VAR_IS_STATIC( var ) ? ", 1" : ", 0" ) : "" )

	fprintf( current_module->core, "\"%s\", ", parent );

    switch( F_VAR_TYPE(var) )
    {
      case F_VAR_LONG:
        fprintf( current_module->core, "fe_new_lng( \"%s\", %ld )%s );", parent, VAI(var), DO_STATIC );
        break;
      case F_VAR_DOUBLE:
        fprintf( current_module->core, "fe_new_lng( \"%s\", %f )%s );", parent, VAF(var), DO_STATIC );
        break;
      case F_VAR_STR:
        fprintf( current_module->core, "fe_new_str( \"%s\", \"%s\" )%s );", parent, VAS(var)->data, DO_STATIC );
        break;
      case F_VAR_OBJ:
        fprintf( current_module->core, "fe_new_obj( \"%s\" )%s );", parent, DO_STATIC );
        break;
      case F_VAR_UARRAY:
        fprintf( current_module->core, "fe_new_array( \"%s\", 0 )%s );", parent, DO_STATIC );
        break;
      case F_VAR_VOID:
        fprintf( current_module->core, "fe_new_void( \"%s\" )%s );", parent, DO_STATIC );
        break;
    }
    fprintf( current_module->core, "\n" );
    ferite_stack_pop( FE_NoScript, current_module->name_stack );
    FE_LEAVE_FUNCTION( NOWT );
}

void builder_process_closed_function( FeriteScript *script, FeriteFunction *fnc, char *parent )
{
    int i, is_ptr = 0;
    int decrementArgCount = 0;
    char c_variable_type[64], *buf, c_variable_init[512];
    FeriteStack *stk;
    FeriteFunctionNative *native_info;
    FILE *target;

    FE_ENTER_FUNCTION;
    
    if( fnc->next != NULL )
      builder_process_closed_function( script, fnc->next, parent );
    
    
    if( fnc->type == FNC_IS_INTRL ) /* we dont touch internal functions */
    {
        if( opt.verbose )
          printf( "  Function '%s' is not native code.\n", fnc->name );
        FE_LEAVE_FUNCTION(NOWT);
    }

    target = module_current_c_file;
    if( current_module->name_stack->stack_ptr == 1 )
      target = current_module->misc;

    ferite_stack_push( FE_NoScript, current_module->name_stack, fnc->name );
    if( opt.verbose )
      printf( "  Writing function %s\n", parent );
    stk = ferite_create_stack( NULL, 30 );
    fprintf( current_module->header, "FE_NATIVE_FUNCTION( %s );\n", builder_generate_current_name(FE_TRUE,FE_TRUE) );
    fprintf( current_module->core, "      %s( script, %s%s, \"%s\", %s, \"",
             (in_class == 1 ? "fe_create_cls_fnc" : "fe_create_ns_fnc"),
             builder_generate_current_name(FE_FALSE,FE_TRUE),
             (in_class == 1 ? "_class" : "_namespace"),
             fnc->name,
             builder_generate_current_name(FE_TRUE,FE_TRUE) );

    fprintf( target, "FE_NATIVE_FUNCTION( %s )\n{\n", builder_generate_current_name(FE_TRUE,FE_TRUE) );
    for( i = 0; i < fnc->arg_count; i++ )
    {
		if( !(fnc->signature[i]->is_dots) )
		{ 
	        switch( F_VAR_TYPE(fnc->signature[i]->variable) )
	        {
	          case F_VAR_LONG:
	          case F_VAR_DOUBLE:
	            strcpy( c_variable_type, "double" );
				c_variable_init[0] = '\0';
	            fprintf( current_module->core, "n" );
	            is_ptr = 0;
	            break;
	          case F_VAR_STR:
	            strcpy( c_variable_type, "FeriteString" );
	            is_ptr = 1;
	            break;
	          case F_VAR_OBJ:
	            strcpy( c_variable_type, "FeriteObject" );
				c_variable_init[0] = '\0';
	            if( i < (in_class == 0 ? fnc->arg_count : fnc->arg_count - 2) )
	              fprintf( current_module->core, "o" );
	            is_ptr = 1;
	            break;
	          case F_VAR_UARRAY:
	            strcpy( c_variable_type, "FeriteUnifiedArray" );
				c_variable_init[0] = '\0';
	            fprintf( current_module->core, "a" );
	            is_ptr = 1;
	            break;
	          case F_VAR_VOID:
	            strcpy( c_variable_type, "FeriteVariable" );
				sprintf( c_variable_init, " = params[%d]", i );
				fprintf( current_module->core, "v" );
				is_ptr = 1;
				break;
	        }
	        if( i < (in_class == 0 ? fnc->arg_count -1 : fnc->arg_count - 3) )
	          fprintf( current_module->core, "," );
	        fprintf( target, "   %s %s%s%s;\n", c_variable_type, (is_ptr ? "*" : "" ), fnc->signature[i]->name, c_variable_init );
		}
        if( !fnc->signature[i]->is_dots && F_VAR_TYPE(fnc->signature[i]->variable) != F_VAR_VOID )
          sprintf( c_variable_type, ", %s%s", (F_VAR_TYPE(fnc->signature[i]->variable) == F_VAR_STR ? "" : "&"), fnc->signature[i]->name );
        else
          sprintf( c_variable_type, ", NULL" );
        ferite_stack_push( FE_NoScript, stk, fstrdup( c_variable_type ) );
    }
    if( !in_class )
    {
        fprintf( current_module->core, "\" );\n" );
    }
    else
    {
        fprintf( current_module->core, "\", %s );\n", (fnc->is_static ? "1" : "0") );
    }
    if( fnc->arg_count > 0 )
    {
        fprintf( target, "\n   ferite_get_parameters( params, %d", 
                 (decrementArgCount ? fnc->arg_count - (in_class ? 3 : 1) : fnc->arg_count) );
        for( i = 1; i <= stk->stack_ptr; i++ )
        {
            buf = stk->stack[i];
            fprintf( target, "%s", buf );
            ffree( buf );
        }
        fprintf( target, " );\n" );
    }
    ferite_delete_stack( NULL, stk );
    native_info = fnc->native_information;
    fprintf( target, "\n   { /* Main function body. */\n" );

    if( (buf = strrchr( native_info->file, '/' )) != NULL )
      buf++;
    fprintf( target, "#line %d \"%s\"\n", native_info->line, 
		ferite_replace_string( (buf != NULL ? buf : native_info->file), "\\", "\\\\" ) );
    fprintf( target, "%s\n   }\n   FE_RETURN_VOID;\n   self = NULL;\n}\n\n", native_info->code );
    ferite_stack_pop( FE_NoScript, current_module->name_stack );
    FE_LEAVE_FUNCTION( NOWT );
}

void builder_process_open_function( FeriteScript *script, FeriteFunction *fnc, char *parent )
{
    int i, is_ptr = 0;
    int decrementArgCount = 0;
    char c_variable_type[64], *buf = NULL, c_variable_init[512], *sig = NULL;
    FeriteStack *stk = NULL;
    FeriteFunctionNative *native_info = NULL;
    FILE *target = NULL;

    FE_ENTER_FUNCTION;
    
    if( fnc->next != NULL )
        builder_process_open_function( script, fnc->next, parent );
    
    if( fnc->type == FNC_IS_INTRL ) /* we dont touch internal functions */
    {
        if( opt.verbose )
          printf( "  Function '%s' is not native code.\n", fnc->name );
        FE_LEAVE_FUNCTION(NOWT);
    }

    target = module_current_c_file;
    if( current_module->name_stack->stack_ptr == 1 )
      target = current_module->misc;

    ferite_stack_push( FE_NoScript, current_module->name_stack, fnc->name );
    if( opt.verbose )
      printf( "  Writing function %s\n", parent );
    stk = ferite_create_stack( NULL, 30 );
    
    sig = ferite_function_generate_sig_string( script, fnc );
    fprintf( current_module->header, "FE_NATIVE_FUNCTION( %s_%s );\n", builder_generate_current_name(FE_TRUE,FE_TRUE), sig );
    fprintf( current_module->core, "      ferite_module_register_native_function( \"%s_%s\", %s_%s );\n", 
             builder_generate_current_function_name(), sig, 
             builder_generate_current_name(FE_TRUE,FE_TRUE), sig );
    fprintf( target, "FE_NATIVE_FUNCTION( %s_%s )\n{\n", builder_generate_current_name(FE_TRUE,FE_TRUE), sig );
    ffree( sig );
    
    for( i = 0; i < fnc->arg_count; i++ )
    {
        memset( &c_variable_init, '\0', 512 );
		if( fnc->signature[i]->variable ) {
	        switch( F_VAR_TYPE(fnc->signature[i]->variable) )
	        {
			  case F_VAR_BOOL:
				strcpy( c_variable_type, "char" );
				strcpy( c_variable_init, " = FE_FALSE" );
				is_ptr = 0;
				break;
	          case F_VAR_LONG:
	          case F_VAR_DOUBLE:
	            strcpy( c_variable_type, "double" );
				strcpy( c_variable_init, " = 0.0" );
	            is_ptr = 0;
	            break;
	          case F_VAR_STR:
	            strcpy( c_variable_type, "FeriteString" );
				strcpy( c_variable_init, " = NULL" );
	            is_ptr = 1;
	            break;
	          case F_VAR_OBJ:
	            strcpy( c_variable_type, "FeriteObject" );
				strcpy( c_variable_init, " = NULL" );
	            is_ptr = 1;
	            break;
	          case F_VAR_UARRAY:
	            strcpy( c_variable_type, "FeriteUnifiedArray" );
				strcpy( c_variable_init, " = NULL" );
	            is_ptr = 1;
	            break;
	          case F_VAR_VOID:
	            strcpy( c_variable_type, "FeriteVariable" );
	            sprintf( c_variable_init, " = params[%d]", i );
	            is_ptr = 1;
	            break;
	        }
            fprintf( target, "   %s %s%s%s;\n", c_variable_type, (is_ptr ? "*" : "" ), fnc->signature[i]->name, c_variable_init );
		}
        if( !(fnc->signature[i]->is_dots) && F_VAR_TYPE(fnc->signature[i]->variable) != F_VAR_VOID )
          sprintf( c_variable_type, ", &%s", fnc->signature[i]->name );
        else
          sprintf( c_variable_type, ", NULL" );
        ferite_stack_push( FE_NoScript, stk, fstrdup( c_variable_type ) );
    }
    
    if( fnc->klass != NULL && !fnc->is_static )
    {
        fprintf( target, "   FeriteObject *self = FE_CONTAINER_TO_OBJECT;\n" );
        fprintf( target, "   FeriteObject *super = FE_CONTAINER_TO_OBJECT;\n" );
    }
    else if( fnc->klass != NULL && fnc->is_static )
      fprintf( target, "   FeriteClass *self = FE_CONTAINER_TO_CLASS;\n" );
    else
      fprintf( target, "   FeriteNamespace *self = FE_CONTAINER_TO_NS;\n" );
    
    if( fnc->arg_count > 0 )
    {
        fprintf( target, "\n   ferite_get_parameters( params, %d", (decrementArgCount
                                                                    ? fnc->arg_count - (in_class ? 3 : 1)
                                                                    : fnc->arg_count) );
        for( i = 1; i <= stk->stack_ptr; i++ )
        {
            buf = stk->stack[i];
            fprintf( target, "%s", buf );
            ffree( buf );
        }
        fprintf( target, " );\n" );
    }
    ferite_delete_stack( NULL, stk );
    native_info = fnc->native_information;
    fprintf( target, "\n   { /* Main function body. */\n" );

    if( (buf = strrchr( native_info->file, '/' )) != NULL )
      buf++;
    fprintf( target, "#line %d \"%s\"\n", native_info->line,  
		ferite_replace_string( (buf != NULL ? buf : native_info->file), "\\", "\\\\" ) );
    fprintf( target, "%s\n   }\n   FE_RETURN_VOID;\n   self = NULL;\n", native_info->code );
    if( fnc->klass != NULL && !fnc->is_static )
      fprintf( target, "   super = NULL;\n" );
    fprintf( target, "}\n\n" );
    ferite_stack_pop( FE_NoScript, current_module->name_stack );
    FE_LEAVE_FUNCTION( NOWT );
}

void builder_process_function( FeriteScript *script, FeriteFunction *fnc, char *parent )
{
    FE_ENTER_FUNCTION;
    if( fnc->is_alias == FE_FALSE )
    {
        if( is_open == FE_TRUE )
            builder_process_open_function( script, fnc, parent );
        else
            builder_process_closed_function( script, fnc, parent );
    }
    FE_LEAVE_FUNCTION( NOWT );
}

void builder_process_open_class( FeriteScript *script, FeriteClass *cls, char *parent )
{
    char buf[1024];

    FE_ENTER_FUNCTION;
    ferite_stack_push( FE_NoScript, current_module->name_stack, cls->name );
    sprintf( buf, "%s.c", builder_generate_current_name(FE_TRUE,FE_FALSE) );
    strcat( internal_file_list, buf ); strcat( internal_file_list, " " );
    if( opt.verbose )
      printf( "Generating file %s for class %s\n", buf, parent );
    ferite_stack_push( FE_NoScript, file_stack, module_current_c_file );
    module_current_c_file = builder_fopen( buf, "w" );

    fprintf( module_current_c_file,
             "/* This file has been automatically generated by builder part of the ferite distribution */\n" \
                         "/* file:  %s */\n" \
                         "/* class: %s */\n\n" \
                         "#include <ferite.h>       /* we need this without a doubt */\n" \
                         "#include \"%s_header.h\"  /* this is the module header */\n\n", buf, cls->name, current_module->name );
    in_class = 1;
    ferite_process_hash( script, cls->object_methods, (void (*)(FeriteScript*,void *,char*))builder_process_function );
    ferite_process_hash( script, cls->class_methods, (void (*)(FeriteScript*,void *,char*))builder_process_function );
    in_class = 0;
    fclose( module_current_c_file );
    module_current_c_file = ferite_stack_pop( FE_NoScript, file_stack );
    ferite_stack_pop( FE_NoScript, current_module->name_stack );
    FE_LEAVE_FUNCTION( NOWT );
}

int builder_class_in_stack( char *name )
{
    int i;

    FE_ENTER_FUNCTION;
    for( i = 1; i <= class_stack->stack_ptr; i++ )
    {
        if( strcmp( (char*)class_stack->stack[i], name ) == 0 )
        {
            FE_LEAVE_FUNCTION(1);
        }
    }
    FE_LEAVE_FUNCTION(0);
}

void builder_process_closed_class( FeriteScript *script, FeriteClass *cls, char *parent )
{
    char buf[1024];

    FE_ENTER_FUNCTION;

    if( (char *)cls->parent != NULL ) /* we have a inheirted class */
    {
        if( !builder_class_in_stack( (char *)cls->parent ) ) /* it's not in the class stack */
        {
            FeriteNamespaceBucket *nsb = ferite_find_namespace( script, script->mainns, (char *)cls->parent, FENS_CLS );
            if( nsb != NULL )
            {
                builder_process_class( script, nsb->data, parent );
                ferite_stack_push( FE_NoScript, class_stack, fstrdup((char *)cls->parent) );
            }
            else
            {
                ferite_warning( script, "Class '%s' extends '%s' which does not exist in this module - assuming it is external\n", cls->name, (char*)cls->parent );
            }
        }
    }
    if( builder_class_in_stack( cls->name ) )
    {
        /* we return if we are already in the list */
        FE_LEAVE_FUNCTION(NOWT);
    }

    ferite_stack_push( FE_NoScript, current_module->name_stack, cls->name );
    sprintf( buf, "%s.c", builder_generate_current_name(FE_TRUE,FE_FALSE) );
    strcat( internal_file_list, buf ); strcat( internal_file_list, " " );
    if( opt.verbose )
      printf( "Generating file %s for class %s\n", buf, parent );
    ferite_stack_push( FE_NoScript, file_stack, module_current_c_file );
    module_current_c_file = builder_fopen( buf, "w" );

    fprintf( module_current_c_file,
             "/* This file has been automatically generated by builder part of the ferite distribution */\n" \
                         "/* file:  %s */\n" \
                         "/* class: %s */\n\n" \
                         "#include <ferite.h>       /* we need this without a doubt */\n" \
                         "#include \"%s_header.h\"  /* this is the module header */\n\n", buf, cls->name, current_module->name );

    fprintf( current_module->core, "   if( ferite_namespace_element_exists( script, %s_namespace, \"%s\" ) == NULL )\n" \
                                  "   {\n" \
                            "      FeriteClass *%s_%s_class = ferite_register_inherited_class( script, %s_namespace, \"%s\", %s%s%s );\n",
             builder_generate_current_name( FE_FALSE,FE_TRUE ),
             cls->name,
             builder_generate_current_name( FE_TRUE,FE_TRUE ),
             cls->name,
             builder_generate_current_name( FE_FALSE,FE_TRUE ),
             cls->name,
             ( cls->parent == NULL ? "" : "\"" ),
             ( cls->parent == NULL ? "NULL" : (char *)cls->parent ),
             ( cls->parent == NULL ? "" : "\"" )
             );
    if( cls->parent != NULL )
    {
        ffree( cls->parent );
    }
    in_class = 1;
	ferite_process_hash( script, cls->object_vars, (void (*)(FeriteScript*,void *,char*))builder_process_variable );
    ferite_process_hash( script, cls->class_vars, (void (*)(FeriteScript*,void *,char*))builder_process_variable );
    ferite_process_hash( script, cls->object_methods, (void (*)(FeriteScript*,void *,char*))builder_process_function );
    ferite_process_hash( script, cls->class_methods, (void (*)(FeriteScript*,void *,char*))builder_process_function );
    in_class = 0;
    fprintf( current_module->core, "   }\n\n" );

    fclose( module_current_c_file );
    module_current_c_file = ferite_stack_pop( FE_NoScript, file_stack );
    ferite_stack_pop( FE_NoScript, current_module->name_stack );
    FE_LEAVE_FUNCTION( NOWT );
}

void builder_process_class( FeriteScript *script, FeriteClass *cls, char *parent )
{
    FE_ENTER_FUNCTION;
    if( strcmp(cls->name,"Obj") && cls->name[0] != '!' )
    {
        if( is_open == FE_TRUE )
        {
            builder_process_open_class( script, cls, parent );
        }
        else
        {
            builder_process_closed_class( script, cls, parent );
        }
    }
    FE_LEAVE_FUNCTION( NOWT );
}

void builder_process_open_namespace_element( FeriteScript *script, FeriteNamespaceBucket *nsb, char *parent )
{
    char buf[1024];

    FE_ENTER_FUNCTION;
    if( nsb != NULL )
    {
        switch( nsb->type )
        {
          case FENS_NS:
            ferite_stack_push( FE_NoScript, current_module->name_stack, parent );
            sprintf( buf, "%s.c", builder_generate_current_name(FE_TRUE,FE_FALSE) );
            strcat( internal_file_list, buf ); strcat( internal_file_list, " " );
            if( opt.verbose )
              printf( "Generating file %s for namespace %s\n", buf, parent );
            ferite_stack_push( FE_NoScript, file_stack, module_current_c_file );
            module_current_c_file = builder_fopen( buf, "w" );
            fprintf( module_current_c_file, "/* This file has been automatically generated by builder part of the ferite distribution */\n" \
"/* file: %s */\n" \
"/* namespace: %s */\n\n" \
"#include <ferite.h>       /* we need this without a doubt */\n" \
"#include \"%s_header.h\"  /* this is the module header */\n\n", buf, parent, current_module->name );
            in_class = 0;
			ferite_process_hash( script, ((FeriteNamespace*)nsb->data)->data_fork, (void (*)(FeriteScript *,void *,char*))builder_process_namespace_element );
            ferite_process_hash( script, ((FeriteNamespace*)nsb->data)->code_fork, (void (*)(FeriteScript *,void *,char*))builder_process_namespace_element );
            in_class = 0;
            fclose( module_current_c_file );
            module_current_c_file = ferite_stack_pop( FE_NoScript, file_stack );
            ferite_stack_pop( FE_NoScript, current_module->name_stack );
            break;
          case FENS_FNC:
            builder_process_function( script, nsb->data, parent );
            break;
          case FENS_CLS:
            builder_process_class( script, nsb->data, parent );
            break;
        }
    }
    FE_LEAVE_FUNCTION( NOWT );
}

void builder_process_closed_namespace_element( FeriteScript *script, FeriteNamespaceBucket *nsb, char *parent )
{
    char buf[1024];

    FE_ENTER_FUNCTION;
    if( nsb != NULL )
    {
        switch( nsb->type )
        {
          case FENS_NS:
            ferite_stack_push( FE_NoScript, current_module->name_stack, parent );
            sprintf( buf, "%s.c", builder_generate_current_name(FE_TRUE,FE_FALSE) );
            strcat( internal_file_list, buf ); strcat( internal_file_list, " " );
            if( opt.verbose )
              printf( "Generating file %s for namespace %s\n", buf, parent );
            ferite_stack_push( FE_NoScript, file_stack, module_current_c_file );
            module_current_c_file = builder_fopen( buf, "w" );
            fprintf( module_current_c_file, "/* This file has been automatically generated by builder part of the ferite distribution */\n" \
"/* file: %s */\n" \
"/* namespace: %s */\n\n" \
"#include <ferite.h>       /* we need this without a doubt */\n" \
"#include \"%s_header.h\"  /* this is the module header */\n\n", buf, parent, current_module->name );
            fprintf( current_module->core, "   if( ferite_namespace_element_exists( script, %s_namespace, \"%s\" ) == NULL )\n", builder_generate_current_name(FE_FALSE,FE_TRUE), parent );
            fprintf( current_module->core, "   {\n" );
            fprintf( current_module->core, "      FeriteNamespace *%s_namespace = ferite_register_namespace( script, \"%s\", %s_namespace );\n", builder_generate_current_name(FE_TRUE,FE_TRUE), parent, builder_generate_current_name(FE_FALSE,FE_TRUE) );
            in_class = 0;
			ferite_process_hash( script, ((FeriteNamespace*)nsb->data)->data_fork, (void (*)(FeriteScript *,void *,char*))builder_process_namespace_element );
            ferite_process_hash( script, ((FeriteNamespace*)nsb->data)->code_fork, (void (*)(FeriteScript *,void *,char*))builder_process_namespace_element );
            in_class = 0;
            fprintf( current_module->core, "   }\n" );
            fclose( module_current_c_file );
            module_current_c_file = ferite_stack_pop( FE_NoScript, file_stack );
            ferite_stack_pop( FE_NoScript, current_module->name_stack );
            break;
          case FENS_VAR:
            builder_process_variable( script, nsb->data, parent );
            break;
          case FENS_FNC:
            builder_process_function( script, nsb->data, parent );
            break;
          case FENS_CLS:
            builder_process_class( script, nsb->data, parent );
            break;
          default:
            break;
        }
    }
    FE_LEAVE_FUNCTION( NOWT );
}

void builder_process_namespace_element( FeriteScript *script, FeriteNamespaceBucket *nsb, char *parent )
{
    FE_ENTER_FUNCTION;
    if( is_open == FE_TRUE )
    {
        builder_process_open_namespace_element( script, nsb, parent );
    }
    else
    {
        builder_process_closed_namespace_element( script, nsb, parent );
    }
    FE_LEAVE_FUNCTION( NOWT );
}

char *builder_get_dependancy_list()
{
    FeriteBuffer *buf = ferite_buffer_new(FE_NoScript, 0);
    FeriteString *str = NULL;
    char *return_value = NULL;
    char current_file[1024];
    unsigned int start = 0, i = 0;

    FE_ENTER_FUNCTION;

    for( i = 0; i < strlen( dist_file_list ); i++ )
    {
        if( dist_file_list[i] == ' ' )
        {
            sprintf( current_file, "%s", dist_file_list + start );
            ferite_buffer_printf( FE_NoScript, buf, "%s/%s/%s ", module_path, current_module->name, current_file );
            start = i + 1;
        }
    }

   /* convert it to a form we like */
    str = ferite_buffer_to_str( FE_NoScript, buf );
    return_value = fstrdup( str->data );
    ferite_str_destroy( FE_NoScript, str );
    ferite_buffer_delete( FE_NoScript, buf );
    FE_LEAVE_FUNCTION( return_value );
}

char *builder_get_local_dependancy_list()
{
    FeriteBuffer *buf = ferite_buffer_new(FE_NoScript, 0);
    FeriteString *str = NULL;
    char *return_value = NULL;
    char current_file[1024];
    unsigned int start = 0, i = 0;

    FE_ENTER_FUNCTION;

    for( i = 0; i < strlen( internal_file_list ); i++ )
    {
        if( internal_file_list[i] == ' ' )
        {
            sprintf( current_file, "%s", internal_file_list + start );
            if( start > 0 )
              ferite_buffer_printf( FE_NoScript, buf, "%s: %s_core.c\n", current_file, current_module->name );
            start = i + 1;
        }
    }

   /* convert it to a form we like */
    str = ferite_buffer_to_str( FE_NoScript, buf );
    return_value = fstrdup( str->data );
    ferite_str_destroy( FE_NoScript, str );
    ferite_buffer_delete( FE_NoScript, buf );
    FE_LEAVE_FUNCTION( return_value );
}

void builder_dump_plan( FeriteScript *script )
{
    char buf[1024];
    struct stat sbuf;
    size_t i = 0;
    FeriteNamespaceBucket *nsb;
    FeriteVariable *var;
    char *dep_list = NULL, *dep_list2 = NULL;

    FE_ENTER_FUNCTION;
    current_module->name_stack = ferite_create_stack( NULL, 30 );
    ferite_stack_push( FE_NoScript, current_module->name_stack, current_module->name );

    sprintf( buf, "%s_core.c", current_module->name );
    strcat( internal_file_list, buf ); strcat( internal_file_list, " " );
    if( opt.verbose )
      printf( "Generating file %s for the module core\n", buf );
    current_module->core = builder_fopen( buf, "w" );
    fprintf( current_module->core, "/* This file has been automatically generated by builder part of the ferite distribution */\n" \
                 "#include <ferite.h>       /* we need this without a doubt */\n" \
                 "#include \"%s_header.h\"  /* this is the module header */\n\n", current_module->name );
    fprintf( current_module->core, "void %s()\n{\n", module_init_name );

    sprintf( buf, "%s_misc.c", current_module->name );
    strcat( internal_file_list, buf ); strcat( internal_file_list, " " );
    current_module->misc = builder_fopen( buf, "w" );
    fprintf( current_module->misc, "/* This file has been automatically generated by builder part of the ferite distribution */\n" \
                 "#include <ferite.h>       /* we need this without a doubt */\n" \
                 "#include \"%s_header.h\"  /* this is the module header */\n\n", current_module->name );

   /******************************************************************
    * MODULE FLAGS
    ******************************************************************/
    nsb = ferite_namespace_element_exists(script, script->mainns, "'module-flags");
    if(nsb != NULL && nsb->data != NULL)
    {
        sprintf(buf, "%s_flags.sh", current_module->name );
        if( opt.verbose )
          printf("Generating file %s for the module flags\n", buf);
        current_module->flags = builder_fopen(buf,"w");
        fprintf(current_module->flags, "# This file has been automatically generated by builder part of the ferite distribution\n\n");
        var = nsb->data;
        fprintf( current_module->flags,"%s\n", FE_STR2PTR(var));
        fclose(current_module->flags);
    }

   /******************************************************************
    * MODULE HEADER
    ******************************************************************/
    sprintf( buf, "%s_header.h", current_module->name );
    if( opt.verbose )
      printf( "Generating file %s for the module header\n", buf );
    current_module->header = builder_fopen( buf, "w" );
    fprintf( current_module->header, "/* This file has been automatically generated by builder part of the ferite distribution */\n" \
                 "#ifndef __%s_HEADER__\n" \
                 "#define __%s_HEADER__\n\n", current_module->name, current_module->name );
    fprintf( current_module->header, "#include <ferite.h>\n\n" );
    fprintf( current_module->header, "FERITE_API void ferite_%s_register();\n", current_module->name );
    fprintf( current_module->header, "FERITE_API void ferite_%s_unregister();\n", current_module->name );
    fprintf( current_module->header, "FERITE_API void ferite_%s_init( FeriteScript *script );\n", current_module->name );
    fprintf( current_module->header, "FERITE_API void ferite_%s_deinit( FeriteScript *script );\n\n", current_module->name );
    
    nsb = ferite_namespace_element_exists( script, script->mainns, "'module-header" );
    if( nsb != NULL && nsb->data != NULL )
    {
        var = nsb->data;
        fprintf( current_module->header, "    /* Start user defined header code */\n" );
        fprintf( current_module->header, "%s\n", FE_STR2PTR(var) );
        fprintf( current_module->header, "    /* End user defined header code */\n\n" );
    }

    if( is_open == FE_TRUE )
    {
        ferite_process_hash( script, script->mainns->data_fork, (void (*)(FeriteScript*,void *,char*))builder_process_namespace_element );
        ferite_process_hash( script, script->mainns->code_fork, (void (*)(FeriteScript*,void *,char*))builder_process_namespace_element );
    }
   /******************************************************************
    * MODULE REGISTER
    ******************************************************************/
    nsb = ferite_namespace_element_exists( script, script->mainns, "'module-register" );
    if( nsb != NULL && nsb->data != NULL )
    {
        var = nsb->data;
        fprintf( current_module->core, "    /* Start user defined register code */\n" );
        fprintf( current_module->core, "%s\n", FE_STR2PTR(var) );
        fprintf( current_module->core, "    /* End user defined register code */\n" );
    }
    fprintf( current_module->core, "}\n\n" ); /* end of the register function */

   /******************************************************************
    * MODULE INITIALISER
    ******************************************************************/
    fprintf( current_module->core, "void ferite_%s_init( FeriteScript *script )\n{\n", current_module->name );
    if( is_open == FE_FALSE )
    {
        fprintf( current_module->core, "FeriteNamespace *%s_namespace = script->mainns;\n", current_module->name );
		ferite_process_hash( script, script->mainns->data_fork, (void (*)(FeriteScript*,void *,char*))builder_process_namespace_element );
        ferite_process_hash( script, script->mainns->code_fork, (void (*)(FeriteScript*,void *,char*))builder_process_namespace_element );
    }
    nsb = ferite_namespace_element_exists( script, script->mainns, "'module-init" );
    if( nsb != NULL && nsb->data != NULL )
    {
        var = nsb->data;
        fprintf( current_module->core, "%s", FE_STR2PTR(var) );
    }
    fprintf( current_module->core, "}\n\n" );

   /******************************************************************
  * MODULE DEINITIALISER
  ******************************************************************/
    if( want_deinit )
    {
        fprintf( current_module->core, "void ferite_%s_deinit( FeriteScript *script )\n{\n", current_module->name );
        nsb = ferite_namespace_element_exists( script, script->mainns, "'module-deinit" );
        if( nsb != NULL && nsb->data != NULL )
        {
            var = nsb->data;
            fprintf( current_module->core, "    /* Start user defined deinitialisation code */\n" );
            fprintf( current_module->core, "%s\n", FE_STR2PTR(var) );
            fprintf( current_module->core, "    /* End user defined deinitialisation code */\n" );
        }
        fprintf( current_module->core, "}\n\n" );
    }

   /******************************************************************
  * MODULE UNREGISTER
  ******************************************************************/
    fprintf( current_module->core, "void ferite_%s_unregister()\n{\n", current_module->name );
    nsb = ferite_namespace_element_exists( script, script->mainns, "'module-unregister" );
    if( nsb != NULL && nsb->data != NULL )
    {
        var = nsb->data;
        fprintf( current_module->core, "    /* Start user defined unregister code */\n" );
        fprintf( current_module->core, "%s\n", FE_STR2PTR(var) );
        fprintf( current_module->core, "    /* End user defined unregister code */\n" );
    }
    fprintf( current_module->core, "}\n\n" );

    fprintf( current_module->header, "\n#endif /* __%s_HEADER__ */\n", current_module->name );
    if( current_module->core )
      fclose( current_module->core );
    if( current_module->header )
      fclose( current_module->header );
    ferite_delete_stack( NULL, current_module->name_stack );

   /******************************************************************
  * MODULE Makefile.am
  ******************************************************************/
    if( (builder_stat( "Makefile.am" ) != 0 && want_makefile) || force_makefile == 1 )
    {
        if( opt.verbose )
          printf( "Generating Makefile.am\n" );
        current_module->Makefile = builder_fopen( "Makefile.am", "w" );
        i = strlen( opt.scriptname ) - 1;
        strcat( internal_file_list, current_module->name ); strcat( internal_file_list, "_header.h " );
        while( opt.scriptname[i] != '/' ) i--;
        i++;
        memmove( opt.scriptname, opt.scriptname + i, strlen( opt.scriptname + i ) + 1 );
        dep_list = builder_get_dependancy_list();
        dep_list2 = builder_get_local_dependancy_list();
        fprintf( current_module->Makefile, MakefileTemplate,
                 current_module->name, current_module->name, current_module->name,
                 current_module->name,
                 current_module->name,
                 dist_file_list,
                 current_module->name,
                 current_module->name,
                 current_module->name, internal_file_list, file_list,
                 current_module->name,
                 current_module->name,
                 current_module->name, dep_list,
                 builder_path, current_module->name, module_path, current_module->name, opt.scriptname,
                 dep_list2 );
        ffree( dep_list );
        ffree( dep_list2 );
        fclose( current_module->Makefile );
    }

   /******************************************************************
  * MODULE Config.m4
  ******************************************************************/
    if( builder_stat( "config.m4" ) != 0 && want_configm4)
    {
        if( opt.verbose )
          printf( "Generating config.m4\n" );
        current_module->Makefile = builder_fopen( "config.m4", "w" );
        fprintf( current_module->Makefile, ConfigM4Template,
                 current_module->name,
                 current_module->name,
                 current_module->name,
                 current_module->name,
                 current_module->name );
        fclose( current_module->Makefile );
    }
    /******************************************************************
     * MODULE module.xml
     ******************************************************************/
    sprintf( buf, "%s.xml", current_module->name );
    if( builder_stat( buf ) != 0 && want_xml)
    {
        FILE *module_xml = NULL;
        int i = 0;
        
        if( opt.verbose )
            printf( "Generating %s\n", buf );
        module_xml = builder_fopen( buf, "w" );
        fprintf( module_xml, ModuleXMLTemplateStart, current_module->name );
        for( i = 1; script->include_list->stack[i] != NULL; i++ )
            fprintf( module_xml, ModuleXMLIncludeTemplate, aphex_absolute_to_relative(script->include_list->stack[i]) );
        fprintf( module_xml, ModuleXMLTemplateEnd );
        fclose( module_xml );
    }
    FE_LEAVE_FUNCTION( NOWT );
}

int main( int argc, char **argv )
{
    FeriteScript *script = NULL;
    char *errmsg = NULL;
    int i = 0;

#ifdef HAVE_LIBGC
	GC_INIT();
#endif

    if ( argc < 2 )
    {
        print_version();
        print_copyright();
        printf( "\n" );
        print_usage();
        printf( "\n\tYou need to supply a file name. For help try builder --help\n\n" );
        exit( -1 );
    }
    else
    {

        if( opt.showargs )
        {
            printf( "Argument Count: %d\n", argc );
            for( i = 0; i < argc; i++ )
              printf( "Argument #%d: %s\n", i+1, argv[i] );
        }

        if( opt.verbose )
          printf( "--> initialising builder\n");

        memset( file_list, '\0', 8096 );
        memset( internal_file_list, '\0', 8096 );

        if( ferite_init( argc, argv ) )
        {
            current_module = fmalloc( sizeof(BuilderModule) );
            memset( current_module, '\0', sizeof(BuilderModule) );
            parse_args( argc, argv );

            if( opt.verbose )
              printf( "--> creating script object\n" );

            if( opt.verbose )
            {
                printf( "--> loading script from file\n" );
                printf( "-->   `-> script file: %s\n", opt.scriptname );
            }

            if( opt.verbose )
              printf( "--> compiling script\n" );

            ferite_compiler_keep_native_code(); /* we call this to make sure ferite wont delete our code we need */
            if( !opt.estring )
            {
                script = ferite_script_compile( opt.scriptname );
            }
            else
            {
                script = ferite_compile_string( opt.scriptname );
            }

            if( script->mainns != NULL )
            {
                if( is_open == FE_FALSE )
                {
                    class_stack = ferite_create_stack( NULL, 30 );
                }
                file_stack = ferite_create_stack( NULL, 30 );

                if( opt.verbose )
					printf( "--> dumping script\n" );
                builder_dump_plan( script );

                if( opt.verbose )
                  printf( "--> destroying script object\n" );
                ferite_delete_stack( NULL, file_stack );

                if( is_open == FE_FALSE )
                {
                    for( i = 1; i <= class_stack->stack_ptr; i++ )
                    {
                        if( class_stack->stack[i] != NULL )
                        {
                            ffree( class_stack->stack[i] );
                        }
                    }
                    ferite_delete_stack( NULL, class_stack );
                }
            }
            else
            {
                errmsg = ferite_get_error_log( script );
                fprintf( stderr, "%s", errmsg );
                ffree( errmsg );
            }
            ferite_script_delete( script );
            ffree( current_module );

            ferite_deinit();
        }
        else
        {
            fprintf( stderr, "Unable to initialise the ferite engine!\n" );
        }
        if( opt.verbose )
			printf( "--> Quiting Application\n" );
		
        exit(0);
    }
    return 0;
}
