#include <ferite.h>
#include "utility.h"

SerializeContex *Serialize_walk_init( FeriteScript *script )
{
    SerializeContex *ctx = fmalloc( sizeof( SerializeContex ) );
    ctx->buf = ferite_buffer_new( script, 0 );
    ctx->objects = ferite_create_stack( script, 100 );
    return ctx;
}
void Serialize_walk_deinit( FeriteScript *script, SerializeContex *ctx )
{
    if( ctx )
    {
        if( ctx->buf )
            ferite_buffer_delete( script, ctx->buf );
        if( ctx->objects )
            ferite_delete_stack( script, ctx->objects );
        ffree( ctx );
    }
}

int Serialize_walk_native( FeriteScript *script, SerializeContex *ctx, FeriteVariable *v, int level )
{
    FeriteIterator iter;
    FeriteHashBucket *buk;    
    int i;
    
    if( level >= 99 )
    {
        ferite_error( script, 0, "Serializing too deeply nested\n" );
        return 0;
    }
    
    switch( F_VAR_TYPE(v) )
    {
        case F_VAR_BOOL:
            ferite_buffer_printf( script, ctx->buf, "%d:%d:%s:%d\n", F_VAR_BOOL, strlen(v->vname), v->vname, VAB(v) );
            break;	
        case F_VAR_LONG:
            ferite_buffer_printf( script, ctx->buf, "%d:%d:%s:%d\n", F_VAR_LONG, strlen(v->vname), v->vname, VAI(v) );
            break;
        case F_VAR_DOUBLE:
            ferite_buffer_printf( script, ctx->buf, "%d:%d:%s:%f\n", F_VAR_DOUBLE, strlen(v->vname), v->vname, VAF(v) );
            break;
        case F_VAR_STR:
            ferite_buffer_printf(script, ctx->buf, "%d:%d:%s:%d:", F_VAR_STR, strlen(v->vname), v->vname, FE_STRLEN(v) );
            ferite_buffer_add( script, ctx->buf, FE_STR2PTR(v), FE_STRLEN(v) );
            ferite_buffer_add_char( script, ctx->buf, '\n' );
            break;
        case F_VAR_OBJ:
            if( VAO( v ) == NULL )
            {
                /* This is an empty object */
                ferite_buffer_printf( script, ctx->buf, "%d:%d:%s:0:\n", F_VAR_OBJ, strlen(v->vname),v->vname );
            }
            else
            {
                FeriteFunction *func = NULL;
                func = ferite_object_get_function_for_params( script, VAO( v ), "serializeSleep", NULL );
                if( func != NULL ) 
                {
                    ferite_variable_destroy( script, ferite_call_function( script, v, NULL, func, NULL) );
                }
		
                if( (i = Serialize_walk_objects( script, ctx, v )) == -1 )
                {
					char *klassName = ferite_generate_class_fqn( script, VAO(v)->klass );
					FeriteObjectVariable *obv = NULL;
					
					ferite_stack_push( script, ctx->objects, VAO( v ) );
					ferite_buffer_printf( script, ctx->buf, "%d:%d:%s:%d:%s\n", F_VAR_OBJ, strlen(v->vname), v->vname, strlen(klassName), klassName );
					ffree(klassName);
					for( obv = VAO(v)->variables; obv != NULL; obv = obv->parent )
					{
						memset( &iter, 0, sizeof( FeriteIterator ) );
						while( (buk = ferite_hash_walk( script, obv->variables, &iter )) != NULL )
						{
							/* Recursive walk on non-empty objects */
							Serialize_walk_native( script, ctx, (FeriteVariable *)buk->data, level+1 );
						}
					}
					ferite_buffer_add( script, ctx->buf, "0:0::\n", 6 );
                }
                else
                {
                    /* This is a reference to an object */
                    ferite_buffer_printf( script, ctx->buf ,"-1:%d:%s:%d\n", strlen(v->vname),v->vname, i );
                }
            }
            break;
        case F_VAR_UARRAY:
            ferite_buffer_printf( script, ctx->buf, "%d:%d:%s\n", F_VAR_UARRAY, strlen(v->vname), v->vname );
            for( i = 0; i < VAUA(v)->size; i++ )
            {
                /* Recursive walk on arrays */
                Serialize_walk_native(script, ctx, VAUA(v)->array[i],level+1);
            }
            ferite_buffer_add(script, ctx->buf, "0:0::\n", 6 );
            break;
		case F_VAR_VOID:
            ferite_buffer_printf( script, ctx->buf, "%d:%d:%s\n", F_VAR_VOID, strlen(v->vname), v->vname );
			break;
		case F_VAR_NS:
		{
			char *name = ferite_generate_namespace_fqn( script, VAN(v) );
            ferite_buffer_printf( script, ctx->buf, "%d:%d:%s:%d:%s\n", F_VAR_NS, strlen(v->vname), v->vname, strlen(name), name );
			ffree( name );
			break;
		}
		case F_VAR_CLASS:
		{
			char *name = ferite_generate_class_fqn( script, VAC(v) );
            ferite_buffer_printf( script, ctx->buf, "%d:%d:%s:%d:%s\n", F_VAR_CLASS, strlen(v->vname), v->vname, strlen(name), name );
			ffree( name );
			break;
		}
            
    }
    if( level == 0 )
    {
        /* FIXME: add crc or token that the buffer has ended */
        return 0;
    }
    return 1;
}
int Serialize_walk_XML( FeriteScript *script, SerializeContex *ctx, char *name, FeriteVariable *v, int level )
{
    FeriteIterator iter;
    FeriteHashBucket *buk;
    int i;
    char tab[101];
    char namebuf[1024];


    if( level >= 99 )
    {
        ferite_error( script, 0, "Serializing too deeply\n" );
        return 0;
    }
	memset( namebuf, 0, 1024 );
	if( name ) {
		sprintf( namebuf, " name=\"%s\"", name );
	}

    memset( tab, '\t', level + 1 );
    switch( F_VAR_TYPE(v) )
    {
        case F_VAR_BOOL:
            ferite_buffer_printf( script, ctx->buf, "%.*s<boolean%s>%s</boolean>\n",level,tab, namebuf, (VAB(v) ? "true" : "false") );
            break;
        case F_VAR_LONG:
            ferite_buffer_printf( script, ctx->buf, "%.*s<number%s type=\"long\">%d</number>\n",level,tab, namebuf, VAI(v) );
            break;
        case F_VAR_DOUBLE:
            ferite_buffer_printf( script, ctx->buf, "%.*s<number%s type=\"long\">%f</number>\n",level,tab, namebuf, VAF(v) );
            break;
        case F_VAR_STR:
            ferite_buffer_printf( script, ctx->buf, "%.*s<string%s><![CDATA[", level, tab, namebuf );
			if( FE_STRLEN(v) > 0 )
				ferite_buffer_add( script, ctx->buf, FE_STR2PTR(v), FE_STRLEN(v) );
            ferite_buffer_printf( script, ctx->buf, "]]></string>\n" );
            break;
        case F_VAR_OBJ:
            if( VAO( v ) == NULL )
            {
                /* This is an empty object */
                ferite_buffer_printf( script, ctx->buf, "%.*s<object%s type=\"object\" value=\"null\" />\n", level, tab, namebuf );
            }
            else
            {
                FeriteFunction *func = NULL;
                func = ferite_object_get_function_for_params( script, VAO( v ), "serializeSleep", NULL );
                if( func != NULL ) 
                {
                    ferite_variable_destroy( script, ferite_call_function( script, v, NULL, func, NULL) );
                }
		
                if( (i = Serialize_walk_objects( script, ctx, v )) == -1 )
                {
                    FeriteObjectVariable *obv = NULL;
                    char *klassName = ferite_generate_class_fqn( script, VAO(v)->klass );
                    
                    ferite_stack_push( script, ctx->objects, VAO( v ) );
                    ferite_buffer_printf( script, ctx->buf, "%.*s<object%s type=\"object\" class=\"%s\" referenceid=\"%d\">\n", level, tab, namebuf, klassName, ctx->objects->stack_ptr );
                    ffree( klassName );
                    for( obv = VAO(v)->variables; obv != NULL; obv = obv->parent )
                    {
                        memset( &iter, 0, sizeof( FeriteIterator ) );
                        while( VAO( v ) && (buk = ferite_hash_walk( script, obv->variables, &iter )) != NULL )
                        {
                            /* Recursive walk on non-empty objects */
                            Serialize_walk_XML( script, ctx, buk->id, (FeriteVariable *)buk->data, level+1 );
                        }
                    }
                    ferite_buffer_printf( script, ctx->buf, "%.*s</object>\n", level, tab );
                }
                else
                {
                    ferite_buffer_printf( script, ctx->buf, "%.*s<object%s type=\"reference\" id=\"%d\" />\n", level, tab, namebuf, i );
                }
            }
            break;
        case F_VAR_UARRAY:
            ferite_buffer_printf( script, ctx->buf, "%.*s<array%s type=\"array\">\n",level,tab, namebuf );
            for( i = 0; i < VAUA(v)->size; i++ )
            {
                /* Recursive walk on arrays */
                Serialize_walk_XML(script, ctx, NULL, VAUA(v)->array[i],level+1 );
            }
                ferite_buffer_printf( script, ctx->buf, "%.*s</array>\n", level, tab, namebuf );
            break;
		case F_VAR_VOID:
			ferite_buffer_printf( script, ctx->buf, "%.*s<void%s />\n", level, tab, namebuf );
			break;
		case F_VAR_NS:
		{
			char *name = ferite_generate_namespace_fqn( script, VAN(v) );
			ferite_buffer_printf( script, ctx->buf, "%.*s<namespace%s location=\"%s\" />\n", level, tab, namebuf, name );
			ffree( name );
			break;
		}
		case F_VAR_CLASS:
		{
			char *name = ferite_generate_class_fqn( script, VAC(v) );
			ferite_buffer_printf( script, ctx->buf, "%.*s<class%s location=\"%s\" />\n", level, tab, namebuf, name );
			ffree( name );
			break;
		}
    }
    if( level == 0 )
        return 0;
	return 1;
}
int Serialize_walk_objects( FeriteScript *script, SerializeContex *ctx, FeriteVariable *v )
{
    int i;
    for( i = 0; i <= ctx->objects->stack_ptr ; i++ )
    {
        
        if( (FeriteObject *)ctx->objects->stack[i] == VAO( v ) )
        {
            return i;
        }
    }
    return -1;
}
int Serialize_native_decode_first( FeriteScript *script, char **str, int *type, int *len, int level )
{
    char *ptr;
    
    if( level >= 99 )
    {
        ferite_error( script, 0, "Structure is too deeply nested" );
        return 1;
    }
    ptr = Serialize_strsep( str, ":" );
    if( ptr == NULL )
    {
        ferite_error( script, 0, "Corrupted serialized data:no type field" );
        return 2;
    }
    *type = atoi( ptr );
    ptr = Serialize_strsep( str, ":" );
    if( ptr == NULL )
    {
        ferite_error( script, 0, "Corrupted serialized data:no length field" );
        return 3;
    }
    *len = atoi( ptr );
    return 0;
}
char *Serialize_strsep( register char **stringp , register const char *delim )
{
    register char *s;
    register const char *spanp;
    register int c, sc;
    char *tok;
    
    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;)
    {
        c = *s++;
        spanp = delim;
        do
        {
            if ((sc = *spanp++) == c)
            {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        }
        while (sc != 0);
    }
}

