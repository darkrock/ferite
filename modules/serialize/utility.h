

typedef struct
{
    FeriteBuffer *buf;
    FeriteStack *objects;
} SerializeContex;

SerializeContex *Serialize_walk_init( FeriteScript *script );
void Serialize_walk_deinit( FeriteScript *script, SerializeContex *ctx );
int Serialize_walk_native(FeriteScript *script, SerializeContex *ctx, FeriteVariable *v, int level );
int Serialize_walk_XML( FeriteScript *script, SerializeContex *ctx, char *name, FeriteVariable *v, int level );
int Serialize_walk_objects( FeriteScript *script, SerializeContex *ctx, FeriteVariable *v );
int Serialize_native_decode_first( FeriteScript *script, char **str, int *type, int *len, int level );
char *Serialize_strsep( register char **stringp , register const char *delim );
