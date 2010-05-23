
#define CHECK_AND_INIT( CACHE, RETURN ) if( !CACHE ) { if( ferite_cache_enabled ) { CACHE = ferite_AMTHash_Create(script); } else { return (RETURN); } } 

#define CACHE_OUT(VALUE) FUD(VALUE)

#define IMPLEMENT_CACHE_LINE( TYPE, DEFAULT, NAME, COPY )                                    \
	FeriteAMT *ferite_cache_##NAME = NULL;                                                   \
	TYPE ferite_cache_has_##NAME( FeriteScript *script, char *line ) {                       \
		TYPE existing = DEFAULT;                                                             \
		CHECK_AND_INIT(ferite_cache_##NAME,DEFAULT);                                         \
		existing = ferite_hamt_get( script, ferite_cache_##NAME, line );                     \
		CACHE_OUT(("Looking for "  #NAME " '%s' [%p]\n", line, existing));                   \
		if( !existing ) {                                                                    \
			CACHE_OUT((#NAME " cache miss: %s\n", line));                                    \
		}                                                                                    \
		return existing;                                                                     \
	}                                                                                        \
	int ferite_cache_register_##NAME( FeriteScript *script, char *line, TYPE line_value ) {  \
		TYPE existing = NULL;                                                                \
		CHECK_AND_INIT(ferite_cache_##NAME,FE_FALSE);                                        \
		existing = ferite_cache_has_##NAME( script, line );                                  \
		if( !existing ) {                                                                    \
			CACHE_OUT(("Registering " #NAME " '%s' [%p]\n", line, line_value ));             \
			existing = COPY(line_value);                                                     \
			ferite_hamt_set( script, ferite_cache_##NAME, line, existing );                  \
			return FE_TRUE;                                                                  \
		}                                                                                    \
		return FE_FALSE;                                                                     \
	}
#define DECLARE_CACHE_LINE( TYPE, DEFAULT, NAME )                                            \
	TYPE ferite_cache_has_##NAME( FeriteScript *script, char *line );                        \
	int ferite_cache_register_##NAME( FeriteScript *script, char *line, TYPE line_value );
	
DECLARE_CACHE_LINE( char*, NULL, code );
DECLARE_CACHE_LINE( FeriteClass*, NULL, closure );
DECLARE_CACHE_LINE( FeriteFunction*, NULL, function );

FeriteFunction *ferite_cache_reference_function( FeriteScript *script, char *fqfunction );
