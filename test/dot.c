
#include <ferite.h>

void dot_write_function( FeriteScript *script, FeriteFunction *function, FILE *output ) 
{
	
}

void dot_write_class( FeriteScript *script, FeriteClass *klass, FILE *output )
{
	FeriteHashBucket *buk = NULL;
	FeriteIterator *iter = NULL;
	FeriteFunction *function = NULL;
	
	fprintf( output, "\"c:%s [%p]\" -- \"static [%p]\"\n", klass->name, klass, klass );
	fprintf( output, "\"c:%s [%p]\" -- \"instance [%p]\"\n", klass->name, klass, klass );
	
	iter = ferite_create_iterator(script);
	while((buk = (FeriteHashBucket*)ferite_hash_walk(script,klass->class_methods,iter)) != NULL) {
		function = buk->data;
		fprintf( output, "\"static [%p]\" -- \"f:%s [%p]\"\n", klass, function->name, function );
		dot_write_function( script, function, output );
	}
	ffree(iter);	

	iter = ferite_create_iterator(script);
	while((buk = (FeriteHashBucket*)ferite_hash_walk(script,klass->object_methods,iter)) != NULL) {
		function = buk->data;
		fprintf( output, "\"instance [%p]\" -- \"f:%s [%p]\"\n", klass, function->name, function );
		dot_write_function( script, function, output );
	}
	ffree(iter);		
}

void dot_write_namespace( FeriteScript *script, FeriteNamespace *ns, FILE *output )
{
	FeriteHashBucket *buk = NULL;
    FeriteIterator *iter = NULL;
    
    FE_ENTER_FUNCTION;
    iter = ferite_create_iterator(script);
    while((buk = (FeriteHashBucket*)ferite_hash_walk(script,ns->code_fork,iter)) != NULL)
    {
        FeriteNamespaceBucket *b = buk->data;
		switch( b->type ) {
			case FENS_FNC: {
				FeriteFunction *function = b->data;
				fprintf( output, "\"n:%s [%p]\" -- \"f:%s [%p]\"\n", ns->name, ns, function->name, function );
				dot_write_function( script, function, output );
				break;
			}				
			case FENS_NS: {
				FeriteNamespace *namespace = b->data;
				fprintf( output, "\"n:%s [%p]\" -- \"n:%s [%p]\"\n", ns->name, ns, namespace->name, namespace );
				dot_write_namespace( script, namespace, output );
				break;
			}				
			case FENS_CLS: {
				FeriteClass *klass = b->data;
				fprintf( output, "\"n:%s [%p]\" -- \"c:%s [%p]\"\n", ns->name, ns, klass->name, klass );
				dot_write_class( script, klass, output );
				break;
			}				
		}
    }
    ffree(iter);
}

void dot_write_script( FeriteScript *script, char *path ) 
{
	FILE *output = fopen( path, "w+" );
	fprintf( output, "graph G {\n" );
	dot_write_namespace( script, script->mainns, output );
	fprintf( output, "}\n" );
	fclose( output );
}