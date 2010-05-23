
/*
 * Copyright (C) 2000-2007 Chris Ross 
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

#ifndef __FERITE_AMT_H__
#define __FERITE_AMT_H__

#define FeriteAMT_Low             0
#define FeriteAMT_High            1

#define FeriteAMTType_ANode       1
#define FeriteAMTType_HNode       2
#define FeriteAMTType_Tree        3

#define FeriteAMTIndex_Direct     0
#define FeriteAMTIndex_Compressed 1

typedef void (*AMTWalkCallback)(FeriteScript*,FeriteAMT*,int,char*,void*,void*,void*);
typedef int (*AMTCompareCallback)(FeriteScript*,void*,void*,void*);
typedef FeriteString *(*AMTSerializeCallback)(FeriteScript*,void*);

FeriteAMT  *ferite_amt_create( FeriteScript *script, int higherorder );
void  ferite_amt_destroy( FeriteScript *script, FeriteAMT *tree, void(*delete)(FeriteScript*,void*) );
void *ferite_amt_DefaultDuplicate( FeriteScript *script, void *value, void *extra );
FeriteAMT  *ferite_amt_dup( FeriteScript *script, FeriteAMT *tree, void*(*dup)(FeriteScript*,void*,void*), void *extra );
void  ferite_amt_print( FeriteScript *script, FeriteAMT *tree );
void *ferite_amt_add( FeriteScript *script, FeriteAMT *tree, void *value );
void *ferite_amt_set( FeriteScript *script, FeriteAMT *tree, unsigned long index, void *value );
void *ferite_amt_get( FeriteScript *script, FeriteAMT *tree, unsigned long index );
void *ferite_amt_delete( FeriteScript *script, FeriteAMT *tree, unsigned long index );

void  ferite_amt_walk( FeriteScript *script, FeriteAMT *tree, AMTWalkCallback walk, void *extra, void *extraextra );
int   ferite_amt_cmp( FeriteScript *script, FeriteAMT *left, FeriteAMT *right, AMTCompareCallback cmp, void *extra );

unsigned long ferite_hamt_hash_gen( char *key );
void *ferite_hamt_set( FeriteScript *script, FeriteAMT *tree, char *key, void *value );
void *ferite_hamt_get( FeriteScript *script, FeriteAMT *tree, char *key );
void *ferite_hamt_delete( FeriteScript *script, FeriteAMT *tree, char *key );

FeriteString *ferite_hamt_serialize( FeriteScript *script, FeriteAMT *tree, char *header, AMTSerializeCallback serialize );
FeriteString *ferite_amt_serialize( FeriteScript *script, FeriteAMT *tree, AMTSerializeCallback serialize );
FeriteAMT  *ferite_amt_deserialize( FeriteScript *script, FeriteString *serial, void *(*deserialize)(FeriteScript*,FeriteString*) );

#define AMTFOREACH( TYPE, AMT ) \
do { \
	int i = 0; \
	TYPE value = (TYPE)NULL; \
	for( value = ferite_amt_get(script, (AMT), i++); value != (TYPE)NULL; value = (TYPE)ferite_amt_get(script, (AMT), i++) ) { 

#define AMTARRAYFOREACH( TYPE, AMT ) \
do { \
	int i = 0; \
	for( i = (AMT)->lower_bound; i <= (AMT)->upper_bound; i++ ) { \
		TYPE value = (TYPE)ferite_amt_get(script, (AMT), i);

#define AMTEND \
    } \
} while(0);

#define HAMTFOREACH( TYPE, HAMT ) \
do { \
	int i = 0; \
	FeriteAMT *keys = ferite_AMTHash_Keys( script, HAMT ); \
	FeriteString *key = ferite_amt_get( script, keys, i++ ); \
	TYPE value = (key ? (TYPE)ferite_hamt_get(script, HAMT, key->data) : NULL); \
	for( ; key != NULL && value != (TYPE)NULL; key = ferite_amt_get(script, keys, i++), value = (key ? (TYPE)ferite_hamt_get(script, HAMT, key->data) : NULL) ) {

#define HAMTEND \
    } \
	ferite_amt_destroy( script, keys, ferite_str_destroy ); \
} while(0);


FeriteAMT   *ferite_AMTArray_Create( FeriteScript *script );
FeriteAMT   *ferite_AMTArray_FromItems( FeriteScript *script, ... );
FeriteAMT   *ferite_AMTHash_Create( FeriteScript *script );
FeriteAMT   *ferite_AMTHash_FromItems( FeriteScript *script, ... );
FeriteAMT   *ferite_AMTHash_Keys( FeriteScript *script, FeriteAMT *self );

#endif /* __FERITE_AMT_H__ */
