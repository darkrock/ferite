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

/*
 * This is an implementation of AMT's as described in the paper 'Ideal Hash Tries' by Phil Bagwell
 * It follows the described version within the paper and extends it with some changes:
 *   - The AMT can be initialised to consider either the high or low order bits as significant
 *   - The AMT sub-trees can be initialised as having direct or compressed storage. The compressed
 *     is the same as the one in the paper, direct is the whole base storage allocated.
 *
 * This changes are designed to allow better use within ferite.
 */

#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif

#include "ferite.h"
#include <math.h>

typedef struct __ferite_amt_index_setters AMTIndexSetters;
struct __ferite_amt_index_setters {
	FeriteAMTNode *(*get)(FeriteScript*,FeriteAMTTree*,int);
	FeriteAMTTree *(*set)(FeriteScript*,FeriteAMTTree*,int,FeriteAMTNode*);
	FeriteAMTTree *(*dup)(FeriteScript*,FeriteAMTTree*,void*(*)(FeriteScript*,void*,void*), void *);
};

/* Borrowed from http://infolab.stanford.edu/~manku/bitcount/bitcount.html */
#define _TWO(c)     (0x1u << (c))
#define _MASK(c)    (((unsigned int)(-1)) / (_TWO(_TWO(c)) + 1u))
#define _COUNT(x,c) ((x) & _MASK(c)) + (((x) >> (_TWO(c))) & _MASK(c))
static inline int bitcount (unsigned int n)
{
	n = _COUNT(n, 0);
	n = _COUNT(n, 1);
	n = _COUNT(n, 2);
	n = _COUNT(n, 3);
	n = _COUNT(n, 4);
	/* n = COUNT(n, 5); for 64-bit integers */
	return n ;
}

#define GETBIT( MAP, BIT )        (((MAP) >> (BIT)) & 1)
#define _SETBIT1( MAP, BIT )      ((MAP) | (1 << (BIT)))
#define SETBIT( MAP, BIT, VALUE ) (VALUE ? _SETBIT1(MAP,BIT) : (MAP & (~_SETBIT1(0UL,BIT))))

#define AMT_SHIFT_AMOUNT               5
#define AMT_SHIFT_START                (32)
#define AMT_APPLY_SHIFT( FE_FE_TRUEE, INDEX ) (FE_FE_TRUEE->index_function)( INDEX, shiftAmount )
#define AMT_SHIFT_AND_CHECK( VALUE )   do { shiftAmount -= 5; if( shiftAmount <= 0 ) { return NULL; } } while(0)
#define AMT_UNSHIFT_AND_CHECK( VALUE ) do { shiftAmount += 5; if( shiftAmount > AMT_SHIFT_START ) { return NULL; } } while(0)
#define AMT_CompressedBaseSize         4

#define IS_NODE(NODE)    ((NODE)->type == FeriteAMTType_HNode || (NODE)->type == FeriteAMTType_ANode)
#define NODE_ID(NODE)    ((NODE)->u.value.id)
#define NODE_VALUE(NODE) ((NODE)->u.value.data)

char *__ferite_amt_printbits( unsigned int map ) {
	static char mapbuf[AMT_SHIFT_START + 1];
	int i = 0;
	
	memset( mapbuf, '\0', AMT_SHIFT_START + 1 );
	for( i = (AMT_SHIFT_START - 1); i >= 0; i-- ) {
		mapbuf[31 - i] = '0';
		if( GETBIT(map, i) )
			mapbuf[31 - i] = '1';
	}
	return mapbuf;
}

FeriteAMTNode **ferite_amt_create_base( FeriteScript *script, int size ) {
	FeriteAMTNode **base = fmalloc(size * sizeof(FeriteAMTNode*));
	memset( base, '\0', (size * sizeof(FeriteAMTNode*)) );
	return base;
}
FeriteAMTNode *ferite_amt_direct_get( FeriteScript *script, FeriteAMTTree *tree, int index ) {
	return tree->base[index];
}
FeriteAMTTree *ferite_amt_direct_set( FeriteScript *script, FeriteAMTTree *tree, int index, FeriteAMTNode *node ) {
	tree->base[index] = node;
	tree->map = SETBIT( tree->map, index, (node ? 1 : 0) );
	return tree;
}
#define LOCAL_INDEX( TREE, INDEX ) (TREE->map & (INDEX ? (~0U >> (32 - INDEX)) : 0U))
	
FeriteAMTNode *ferite_amt_compressed_get( FeriteScript *script, FeriteAMTTree *tree, int index ) {
	int local_index = bitcount( LOCAL_INDEX(tree, index) );
	return tree->base[local_index];
}
FeriteAMTTree *ferite_amt_compressed_set( FeriteScript *script, FeriteAMTTree *tree, int index, FeriteAMTNode *node ) {
	int local_index = bitcount( LOCAL_INDEX(tree, index) );
	int index_size = bitcount( tree->map ) + 1;
	
	if( node ) {	
		if( !GETBIT( tree->map, index ) ) {
			if( tree->base_size == 0 ) { /* First creation */
				tree->base_size = AMT_CompressedBaseSize;
				tree->base = ferite_amt_create_base( script, tree->base_size );
			} else if( tree->base_size < index_size ) { /* Resize */
				int new_size = ( (tree->base_size * 2) < 32 ? (tree->base_size * 2) : 32 );
				FeriteAMTNode **new_base = ferite_amt_create_base( script, new_size );
				memcpy( new_base, tree->base, sizeof(FeriteAMTNode*) * (local_index) );
				memcpy( &new_base[local_index + 1], &tree->base[local_index], (tree->base_size - local_index) * sizeof(FeriteAMTNode*) );
				ffree( tree->base );
				tree->base = new_base;
				tree->base_size = new_size; 
			} else {
				memmove( &tree->base[local_index+1], &tree->base[local_index], (tree->base_size - local_index - 1) * sizeof(FeriteAMTNode*) );
			}
		}
		tree->base[local_index] = node;
	} else { /* delete an item */
		memmove( &tree->base[local_index], &tree->base[local_index+1], (tree->base_size - local_index) * sizeof(FeriteAMTNode*) );
	}
	
	tree->map = SETBIT( tree->map, index, (node ? 1 : 0) );
	return tree;
}

FeriteAMTTree *__ferite_amt_tree_dup( FeriteScript *script, FeriteAMTTree *tree, void*(*dup)(FeriteScript*,void*,void*), void *extra );

FeriteAMTTree *ferite_amt_compressed_dup( FeriteScript *script, FeriteAMTTree *tree, void*(*dup)(FeriteScript*,void*,void*), void *extra ) {
	FeriteAMTTree *newTree = fmalloc(sizeof(FeriteAMTTree));
	memset(newTree, 0, sizeof(FeriteAMTTree));
	newTree->map = tree->map;
	newTree->index_type = tree->index_type;
	newTree->base_size = tree->base_size;
	newTree->base = ferite_amt_create_base( script, newTree->base_size );
	if( newTree->map ) {
		int i = 0;
		FeriteAMTNode *node = NULL;
		for( i = 0; i < newTree->base_size; i++ ) {
			if( (node = tree->base[i]) != NULL ) {
				newTree->base[i] = fmalloc(sizeof(FeriteAMTNode));
				memset(newTree->base[i], 0, sizeof(FeriteAMTNode));
				newTree->base[i]->type = node->type;
				if( IS_NODE(node) ) {
					newTree->base[i]->u.value.id = node->u.value.id;
					newTree->base[i]->u.value.data = (dup ? (dup)( script, node->u.value.data, extra ) : node->u.value.data);
					newTree->base[i]->u.value.key = NULL;
					if( node->u.value.key ) {
						newTree->base[i]->u.value.key = fstrdup(node->u.value.key);
					}
				} else if( node->type == FeriteAMTType_Tree ) {
					newTree->base[i]->u.tree = __ferite_amt_tree_dup( script, node->u.tree, dup, extra );
				}
			}
		}
	}
	return newTree;
}

static AMTIndexSetters index_functions[2] = {
	{ ferite_amt_direct_get,     ferite_amt_direct_set,     ferite_amt_compressed_dup },          /* direct, base_size == number of elements within base */
	{ ferite_amt_compressed_get, ferite_amt_compressed_set, ferite_amt_compressed_dup }   /* compressed, bit counting is used to keep the base size small */
};

#define AMT_SET (index_functions[root->index_type].set)
#define AMT_GET (index_functions[root->index_type].get)

FeriteAMTTree *__ferite_amt_tree_dup( FeriteScript *script, FeriteAMTTree *tree, void*(*dup)(FeriteScript*,void*,void*), void *extra ) {
	return index_functions[tree->index_type].dup( script, tree, dup, extra );
}

unsigned int ferite_highorderindex( unsigned int index, unsigned int shiftAmount ) {
	return (((index << (AMT_SHIFT_START - shiftAmount)) >> 27) & 0x1F);
}
unsigned int ferite_loworderindex( unsigned int index, unsigned int shiftAmount ) {
	return (((index << (shiftAmount - AMT_SHIFT_AMOUNT)) >> 27) & 0x1F);
}
FeriteAMTTree *ferite_amt_tree_create( FeriteScript *script, FeriteAMTTree *parent, int index_type ) {
	FeriteAMTTree *tree = fmalloc(sizeof(FeriteAMTTree));
	memset(tree, 0, sizeof(FeriteAMTTree));
	tree->map = 0;
	tree->parent = parent;
	tree->index_type = index_type;
	tree->base = NULL;
	tree->base_size = 0;
	if( index_type == FeriteAMTIndex_Direct ) { /* pre-create the base memory jobby */
		tree->base = ferite_amt_create_base(script,(int)pow(2,AMT_SHIFT_AMOUNT));
		tree->base_size = (int)pow(2,AMT_SHIFT_AMOUNT);
	}
	return tree;
}
FeriteAMTNode *ferite_amt_create_value_node( FeriteScript *script, unsigned long id, char *optional_key, void *value ) {
	FeriteAMTNode *node = fmalloc(sizeof(FeriteAMTNode));
	memset(node, 0, sizeof(FeriteAMTNode));
	node->type = FeriteAMTType_ANode;
	node->u.value.id = id;
	if( optional_key ) {
		node->type = FeriteAMTType_HNode;
		node->u.value.key = fstrdup(optional_key);
	}
	node->u.value.data = value;
	return node;
}
FeriteAMTNode *ferite_amt_create_tree_node( FeriteScript *script, FeriteAMTTree *parent ) {
	FeriteAMTNode *node = fmalloc(sizeof(FeriteAMTNode));
	memset(node, 0, sizeof(FeriteAMTNode));
	node->type = FeriteAMTType_Tree;
	node->u.tree = ferite_amt_tree_create( script, parent, FeriteAMTIndex_Compressed );
	return node;
}
FeriteAMT *ferite_amt_create( FeriteScript *script, int higherorder ) {
	FeriteAMT *amt = fmalloc(sizeof(FeriteAMT));
	memset(amt, 0, sizeof(FeriteAMT));
	amt->root = ferite_amt_tree_create( script, NULL, FeriteAMTIndex_Direct );
	amt->index_function = NULL;
	if( higherorder ) {
		amt->index_function = &ferite_highorderindex;
	} else {
		amt->index_function = &ferite_loworderindex;
	}
	amt->lower_bound = 0;
	amt->upper_bound = 0;
	amt->zero_bound = 0;
	amt->total = 0;
	return amt;
}
void __ferite_amt_tree_destroy( FeriteScript *script, FeriteAMTTree *tree, void(*delete)(FeriteScript*,void*) ) {
	int i = 0;
	if( tree ) {
		if( tree->base ) {
			for( i = 0; i < AMT_SHIFT_START; i++ ) {
				if( GETBIT(tree->map, i) )
				{
					FeriteAMTNode *baseItem = (index_functions[tree->index_type].get)( script, tree, i );
					if( IS_NODE(baseItem) ) {
						if( baseItem->type == FeriteAMTType_ANode ) {
							if( delete )
								(delete)(script,baseItem->u.value.data);
						} else {
							if( delete )
								(delete)(script,baseItem->u.value.data);
							ffree( baseItem->u.value.key );
						}
					} else if( baseItem->type == FeriteAMTType_Tree ) {
						__ferite_amt_tree_destroy( script,baseItem->u.tree, delete );
					}
					ffree(baseItem);
				}
			}
			ffree( tree->base );
		}
		ffree( tree );
	}
}
void ferite_amt_destroy( FeriteScript *script, FeriteAMT *tree, void(*delete)(FeriteScript*,void*) ) {
	__ferite_amt_tree_destroy( script, tree->root, delete );
	ffree( tree );
}

void *ferite_amt_DefaultDuplicate( FeriteScript *script, void *value, void *extra ) {
	return value;
}
FeriteAMT *ferite_amt_dup( FeriteScript *script, FeriteAMT *tree, void*(*dup)(FeriteScript*,void*,void*), void *extra ) {
	FeriteAMT *newTree = fmalloc(sizeof(FeriteAMT));
	memset(newTree, 0, sizeof(FeriteAMT));
	newTree->root = __ferite_amt_tree_dup( script, tree->root, dup, extra );
	newTree->index_function = tree->index_function;
	return newTree;
}


void __ferite_amt_ws( int depth ) {
	while( depth ) {
		printf("  ");
		depth--;
	}
}
void __ferite_amt_print( FeriteScript *script, FeriteAMTTree *tree, int depth ) {
	int i = 0;
	int count = bitcount( tree->map );
	
	__ferite_amt_ws(depth);
	printf("Bit Map: '%s' [%s: %d]\n", __ferite_amt_printbits(tree->map), 
										(tree->index_type == FeriteAMTIndex_Direct ? "direct" : "compressed"),
										(tree->base_size - count) );

	for( i = 0; i < ((int)pow(2,AMT_SHIFT_AMOUNT)); i++ ) {
		if( GETBIT(tree->map, i) ) {
			FeriteAMTNode *baseItem = (index_functions[tree->index_type].get)( script, tree, i );
			__ferite_amt_ws(depth);
			if( baseItem->type == FeriteAMTType_ANode ) {
				printf("- %d: Node: %lu: %s\n", i, baseItem->u.value.id, (char*)baseItem->u.value.data);
			} else if( baseItem->type == FeriteAMTType_HNode ) {
				printf("- %d: Hash Node: %s [%lu]: %s\n", i, baseItem->u.value.key, baseItem->u.value.id, (char*)baseItem->u.value.data);
			} else if( baseItem->type == FeriteAMTType_Tree ) {
				printf("- %d: Tree:\n", i );
				__ferite_amt_print( script,baseItem->u.tree, depth + 1 );
			}
		}
	}
}
void ferite_amt_print( FeriteScript *script, FeriteAMT *tree ) {
	printf("------------------------------------------------------------------\n");
	__ferite_amt_print( script, tree->root, 0 );
	printf("------------------------------------------------------------------\n");
}

void *_ferite_amt_set( FeriteScript *script, FeriteAMT *tree, unsigned long index, char *optional_key, void *value ) {
	FeriteAMTTree *root = tree->root;
	unsigned long shiftAmount = AMT_SHIFT_START;
	unsigned long baseIndex = AMT_APPLY_SHIFT(tree, index);
	FeriteAMTNode *baseItem = NULL;
	
	if( index > tree->upper_bound )
		tree->upper_bound = index;
	if( index < tree->lower_bound )
		tree->lower_bound;
	tree->last_index = index;
	
	while( FE_TRUE ) {
		if( !(GETBIT(root->map, baseIndex)) ) {
			tree->total++;
			AMT_SET( script, root, AMT_APPLY_SHIFT(tree, index), ferite_amt_create_value_node(script, index, optional_key, value) );
			return value;
		}
		else {
			baseItem = AMT_GET( script, root, baseIndex );
			if( IS_NODE(baseItem) ) {
				if( (baseItem->type == FeriteAMTType_ANode && baseItem->u.value.id == index) ||
				    (baseItem->type == FeriteAMTType_HNode && baseItem->u.value.id == index) ) {
					void *old_value = NODE_VALUE(baseItem);
					if( baseItem->type == FeriteAMTType_HNode )
						ffree(baseItem->u.value.key);
					baseItem->type = FeriteAMTType_ANode;
					baseItem->u.value.id = index;
					baseItem->u.value.data = value;
					baseItem->u.value.key = NULL;
					if( optional_key ) {
						baseItem->type = FeriteAMTType_HNode;
						baseItem->u.value.key = fstrdup(optional_key);
					}
					return old_value;
				} else {
					/* We have to split this node */
					FeriteAMTNode *old_node = baseItem;
					int old_id = 0, new_id = 0;
					while( AMT_APPLY_SHIFT(tree, old_node->u.value.id) == AMT_APPLY_SHIFT(tree, index) ) {
						FeriteAMTNode *new_node = ferite_amt_create_tree_node( script,root );
						AMT_SET( script, root, baseIndex, new_node );
						baseItem = AMT_GET( script, root, baseIndex );
						root = baseItem->u.tree;
						AMT_SHIFT_AND_CHECK( NULL );
						baseIndex = AMT_APPLY_SHIFT(tree, index);
					}
					// Old Node
					old_id = AMT_APPLY_SHIFT(tree, old_node->u.value.id);
					new_id = AMT_APPLY_SHIFT(tree, index);
					AMT_SET( script, root, old_id, old_node );
					AMT_SET( script, root, new_id, ferite_amt_create_value_node(script, index, optional_key, value) );
					tree->total++;
					return value;
				}
				break;
			} else if( baseItem->type == FeriteAMTType_Tree ) {
				AMT_SHIFT_AND_CHECK( NULL );
				baseIndex = AMT_APPLY_SHIFT(tree, index);
				root = baseItem->u.tree;
			} else {
				printf("base item: %p -> %d\n", baseItem, baseItem->type);
			}
		}
	}
	return NULL;
}
void *ferite_amt_set( FeriteScript *script, FeriteAMT *tree, unsigned long index, void *value ) {
	return _ferite_amt_set( script, tree, index, NULL, value );
}
void *ferite_amt_add( FeriteScript *script, FeriteAMT *tree, void *value ) {
	return _ferite_amt_set( script, tree, tree->upper_bound++, NULL, value );
}
FeriteAMTNode *_ferite_amt_get( FeriteScript *script, FeriteAMT *tree, unsigned long _index ) {
	FeriteAMTTree *root = tree->root;
	unsigned int index = _index;
	unsigned int shiftAmount = AMT_SHIFT_START;
	unsigned int baseIndex = AMT_APPLY_SHIFT(tree, index);
	FeriteAMTNode *baseItem = NULL;

	tree->last_index = index;
	while( FE_TRUE ) {
		if( !(GETBIT(root->map, baseIndex)) )
			return NULL;
		else {
			baseItem = AMT_GET( script, root, baseIndex );
			if( IS_NODE(baseItem) ) {
				return baseItem;
			} else if( baseItem->type == FeriteAMTType_Tree ) {
				AMT_SHIFT_AND_CHECK( NULL );
				baseIndex = AMT_APPLY_SHIFT(tree, index);
				root = baseItem->u.tree;
			}
		}
	}
}

void *ferite_amt_get( FeriteScript *script, FeriteAMT *tree, unsigned long index ) {
	FeriteAMTNode *baseItem = _ferite_amt_get( script, tree, index );
	if( baseItem && NODE_ID(baseItem) == index ) {
		return NODE_VALUE(baseItem);
	}
	return NULL;
}


void *_ferite_amt_delete( FeriteScript *script, FeriteAMT *tree, unsigned long index, char *optional_key ) {
	FeriteAMTTree *root = tree->root;
	unsigned long shiftAmount = AMT_SHIFT_START;
	unsigned long baseIndex = AMT_APPLY_SHIFT(tree, index);
	FeriteAMTNode *baseItem = NULL;
	
	while( FE_TRUE ) {
		if( !(GETBIT(root->map, baseIndex)) )
			return NULL;
		else {
			baseItem = AMT_GET( script, root, baseIndex );
			if( IS_NODE(baseItem) ) {
				void *data = baseItem->u.value.data;
				
				int delete = FE_FALSE;
				if( baseItem->type == FeriteAMTType_ANode  &&  baseItem->u.value.id == index ) {
					delete = FE_TRUE;
				} else if( baseItem->type == FeriteAMTType_HNode && 
							baseItem->u.value.id == index &&
							strcmp(baseItem->u.value.key,optional_key) == 0 ) {
					ffree(baseItem->u.value.key);
					delete = FE_TRUE;
				}
				if( delete ) {
					tree->total--;
					AMT_SET( script, root, baseIndex, NULL );
					ffree( baseItem );
					/*	
						This code should ideally collapse the tree
						if( !bitcount(root->map) == 1 && root->parent ) {
							FeriteAMTNode *root_item = NULL;
							FeriteAMTNode *new_item = AMT_GET(tree, )
							AMT_UNSHIFT_AND_CHECK( data );
						}
					 */
				}
				return data;
			} else if( baseItem->type == FeriteAMTType_Tree ) {
				AMT_SHIFT_AND_CHECK( NULL );
				baseIndex = AMT_APPLY_SHIFT(tree, index);
				root = baseItem->u.tree;
			}
		}
	}
}
void *ferite_amt_delete( FeriteScript *script, FeriteAMT *tree, unsigned long index ) {
	return _ferite_amt_delete( script, tree, index, NULL );
}

void __ferite_amt_tree_walk( FeriteScript *script, FeriteAMT *top, FeriteAMTTree *tree, AMTWalkCallback walk, void *extra, void *extraextra ) {
	int i = 0;
	for( i = 0; i < AMT_SHIFT_START; i++ ) {
		if( GETBIT(tree->map, i) )
		{
			FeriteAMTNode *baseItem = (index_functions[tree->index_type].get)( script, tree, i );
			if( baseItem->type == FeriteAMTType_ANode || baseItem->type == FeriteAMTType_HNode ) {
				(walk)(script, top, baseItem->u.value.id, baseItem->u.value.key, baseItem->u.value.data, extra, extraextra);
			} else if( baseItem->type == FeriteAMTType_Tree ) {
				__ferite_amt_tree_walk( script, top, baseItem->u.tree, walk, extra, extraextra );
			}
		}
	}
}
void ferite_amt_walk( FeriteScript *script, FeriteAMT *tree, AMTWalkCallback walk, void *extra, void *extraextra ) {
	__ferite_amt_tree_walk( script, tree, tree->root, walk, extra, extraextra );
}
int __ferite_amt_cmp_tree( FeriteScript *script, FeriteAMTTree *left, FeriteAMTTree *right, AMTCompareCallback cmp, void *extra ) {
	int i = 0;
	if( left->map != right->map )
		return FE_FALSE;
		
	for( i = 0; i < AMT_SHIFT_START; i++ ) {
		if( GETBIT(left->map, i) )
		{
			FeriteAMTNode *leftItem = (index_functions[left->index_type].get)( script, left, i );
			FeriteAMTNode *rightItem = (index_functions[right->index_type].get)( script, right, i );
			if( leftItem->type != rightItem->type )
				return FE_FALSE;
			if( leftItem->type == FeriteAMTType_ANode && !(cmp)(script, leftItem->u.value.data, rightItem->u.value.data, extra) ) {
				return FE_FALSE;
			} else if( leftItem->type == FeriteAMTType_HNode && (strcmp(leftItem->u.value.key,rightItem->u.value.key) != 0 || !(cmp)(script, leftItem->u.value.data, rightItem->u.value.data, extra)) ) {
				return FE_FALSE;
			} else if( leftItem->type == FeriteAMTType_Tree && !__ferite_amt_cmp_tree( script, leftItem->u.tree, rightItem->u.tree, cmp, extra ) ) {
				return FE_FALSE;
			}
		}
	}
	return FE_TRUE;
}
int ferite_amt_cmp( FeriteScript *script, FeriteAMT *left, FeriteAMT *right, AMTCompareCallback cmp, void *extra ) {
	return __ferite_amt_cmp_tree( script, left->root, right->root, cmp, extra );
}

unsigned long ferite_hamt_hash_gen( char *key ) {
    size_t i, keylen = strlen(key);
    unsigned long hashval = 0;
	
    for( i = 0; i < keylen; i++ )
      hashval = *key++ + 31 * hashval;
	return hashval;
}

void *ferite_hamt_set( FeriteScript *script, FeriteAMT *tree, char *key, void *value ) {
	return _ferite_amt_set( script, tree, ferite_hamt_hash_gen(key), key, value );
}
void *ferite_hamt_get( FeriteScript *script, FeriteAMT *tree, char *key ) {
	FeriteAMTNode *baseItem = _ferite_amt_get( script, tree, ferite_hamt_hash_gen(key) );
	if( baseItem && strcmp(baseItem->u.value.key,key) == 0 )
		return NODE_VALUE(baseItem);
	return NULL;
}
void *ferite_hamt_delete( FeriteScript *script, FeriteAMT *tree, char *key ) {
	return _ferite_amt_delete( script, tree, ferite_hamt_hash_gen(key), key );
}

void __ferite_amt_tree_serialize( FeriteScript *script, FeriteAMTTree *tree, FeriteBuffer *progress, AMTSerializeCallback serialize, int hamt ) {
	int i = 0;
	for( i = 0; i < AMT_SHIFT_START; i++ ) {
		if( GETBIT(tree->map, i) )
		{
			FeriteAMTNode *baseItem = (index_functions[tree->index_type].get)( script, tree, i );
			if( IS_NODE(baseItem) ) {
				FeriteString *data = (serialize)( script, baseItem->u.value.data );
				if( baseItem->type == FeriteAMTType_HNode && hamt ){
					ferite_buffer_add_str( script, progress, "\"" );
					ferite_buffer_add_str( script, progress, baseItem->u.value.key );
					ferite_buffer_add_str( script, progress, "\"" );
					ferite_buffer_add_str( script, progress, " : " );
				}
				ferite_buffer_add_fstr( script, progress, data );
				ferite_buffer_add_str( script, progress, ", " );
			} else if( baseItem->type == FeriteAMTType_Tree ) {
				__ferite_amt_tree_serialize( script, baseItem->u.tree, progress, serialize, hamt );
			}
		}
	}
}
FeriteString * ferite_hamt_serialize( FeriteScript *script, FeriteAMT *tree, char *header, AMTSerializeCallback serialize ) {
	FeriteBuffer *progress = ferite_buffer_new( script, 0 );
	FeriteString *retval = NULL;
	
	ferite_buffer_add_str( script, progress, "{");
	if( header ) {
		ferite_buffer_add_str( script, progress, header );
	}
	ferite_buffer_add_str( script, progress, " ");
	if( tree->root->map ) {
		__ferite_amt_tree_serialize( script, tree->root, progress, serialize, FE_TRUE );
	}
	ferite_buffer_add_str( script, progress, " }");
	
	retval = ferite_buffer_to_str(script, progress);
	ferite_buffer_delete(script, progress);
	return retval;
}
FeriteString * ferite_amt_serialize( FeriteScript *script, FeriteAMT *tree, AMTSerializeCallback serialize ) {
	FeriteBuffer *progress = ferite_buffer_new( script, 0 );
	FeriteString *retval = NULL;
	int first_done = FE_FALSE;
	
	ferite_buffer_add_str( script, progress, "[ ");
	if( tree->root->map ) {
		int i = 0;
		for( i = tree->lower_bound; i <= tree->upper_bound; i++ ) {
			FeriteString *data = ferite_amt_get( script, tree, i );
			if( data ) {
				if( !first_done ) {
					first_done = FE_TRUE;
				} else {
					ferite_buffer_add_str( script, progress, ", " );
				}
				ferite_buffer_add_fstr( script, progress, data );
			}
		}
	}
	ferite_buffer_add_str( script, progress, " ]");

	retval = ferite_buffer_to_str(script, progress);
	ferite_buffer_delete(script, progress);
	return retval;
}
FeriteAMT  *ferite_amt_deserialize( FeriteScript *script, FeriteString *serial, void *(*deserialize)(FeriteScript*,FeriteString*) ) {
	return NULL;
}
FeriteAMT  *ferite_AMTArray_Create( FeriteScript *script ) {
	FeriteAMT *array = ferite_amt_create( script, FeriteAMT_Low );
	return array;
}
FeriteAMT  *ferite_AMTArray_FromItems( FeriteScript *script, ... ){
	FeriteAMT *array = ferite_AMTArray_Create(script);
	va_list  argument_list;
    void    *current_item = NULL;

	va_start( argument_list, script );
	do {
		current_item = va_arg( argument_list, void* );
		if( current_item ) {
			ferite_amt_add( script, array, current_item );
		}
	} while( current_item != NULL );
	va_end( argument_list );
	return array;
}
FeriteAMT  *ferite_AMTHash_Create( FeriteScript *script ) {
	return ferite_amt_create( script, FeriteAMT_High );
}
FeriteAMT  *ferite_AMTHash_FromItems( FeriteScript *script, ... ){
	FeriteAMT *hash = ferite_AMTArray_Create(script);
	va_list  argument_list;
	char    *current_key = "";
    void    *current_item = NULL;

	va_start( argument_list, script );
	do {
		current_key = va_arg( argument_list, char* );
		current_item = va_arg( argument_list, void* );
		if( current_key ) {
			ferite_hamt_set( script, hash, current_key, current_item );
		}
	} while( current_key != NULL );
	va_end( argument_list );
	return hash;
}
void _ferite_AMTHash_Keys_Walk( FeriteScript *script, FeriteAMT *top, int id, char *key, void *_index, FeriteAMT *list, void *_extra ) {
	if( key != NULL ) {
		ferite_amt_add( script, list, ferite_str_new(script, key, 0, FE_CHARSET_DEFAULT) );
	}
}
FeriteAMT *ferite_AMTHash_Keys( FeriteScript *script, FeriteAMT *self ) {
	FeriteAMT *list = ferite_AMTArray_Create(script);
	ferite_amt_walk( script, 
					 self, 
					 (AMTWalkCallback)_ferite_AMTHash_Keys_Walk, 
					 list, 
					 NULL );

	return list;
}
