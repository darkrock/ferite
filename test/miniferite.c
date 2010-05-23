#include <stdio.h>
#include <stdlib.h>
#include "ferite.h"
/*
 * This is a shell script to build miniferite
LIBDIR=`ferite-config --module-library-path`
NATIVELIBDIR=`ferite-config --native-library-path`
LIBS=`ferite-config --libs`
CFLAGS=`ferite-config --cflags`

gcc miniferite.c $CFLAGS $LIBS \
    -DLIBRARY_DIR=\"$LIBDIR\" -DNATIVE_LIBRARY_DIR=\"$NATIVELIBDIR\" \
    -o miniferite
*/

int run_script( int argc, char **argv )
{
    FeriteScript *script;
    char *errmsg = NULL, *scriptfile = "test.fe";
	if( argc > 1 ) {
		scriptfile = argv[1];
	}
    script = ferite_script_compile( scriptfile );
    if( ferite_has_compile_error( script ) )
    {
        errmsg = ferite_get_error_log( script );
        fprintf( stderr, "[ferite: compile]\n%s", errmsg );
    }
    else
    {
        ferite_script_execute( script );
        if( ferite_has_runtime_error( script ) )
        {
            errmsg = ferite_get_error_log( script );
            fprintf( stderr, "[ferite: execution]\n%s", errmsg );
        }
    }
    if( errmsg )
      ffree( errmsg );
    ferite_script_delete( script );
	return FE_TRUE;
}

int main( int argc, char **argv )
{
	int i = 0;
    if( ferite_init( argc, argv ) )
    {
        ferite_add_library_search_path( LIBRARY_DIR );
        ferite_set_library_native_path( NATIVE_LIBRARY_DIR );
		for( i = 0; i < 1; i++ ) {
			if( (i % 1000) == 0 ) {
				printf("DING! GRATZ! I R LVL %d\n", i);
			}
			run_script( argc, argv );
		}
		printf("Sleeping\n");
		sleep(10000000);
        ferite_deinit( );
    }
    exit( 0 );
}
