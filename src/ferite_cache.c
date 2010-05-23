
#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

#include "ferite.h"
#include <setjmp.h>
#include "aphex.h"
#include "fcache.h"

int        ferite_cache_enabled = FE_FALSE;

void ferite_parser_script_agressive_normalise( char *str );

int ferite_cache_toggle( int state ) {
	if( state ) {
		ferite_cache_enabled = FE_TRUE;
	}
	return ferite_cache_enabled;
}

#define TAG_CACHED( ITEM ) ITEM; ITEM->cached = FE_TRUE; 

IMPLEMENT_CACHE_LINE( char*, NULL, code, fstrdup )
IMPLEMENT_CACHE_LINE( FeriteClass*, NULL, closure, TAG_CACHED )
IMPLEMENT_CACHE_LINE( FeriteFunction*, NULL, function, TAG_CACHED )

FeriteFunction *ferite_cache_reference_function( FeriteScript *script, char *fqfunction ) {
	FeriteFunction *existing = ferite_cache_has_function( script, fqfunction );
	if( existing ) {
		existing = ferite_create_alias_function( script, existing );
	}
	return existing;
}

