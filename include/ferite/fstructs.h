/*
 * Copyright (C) 2000-2007 Chris Ross and various contributors
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

#ifndef __FERITE_STRUCTS_H__
# define __FERITE_STRUCTS_H__

/*
 * The 'typedefs' for the structures. If you are going to play with ferite
 * do it using the typedef's _not_ the raw structures.
 */
typedef struct _ferite_hash_bucket                 FeriteHashBucket;
typedef struct _ferite_hash                        FeriteHash;
typedef struct _ferite_stack                       FeriteStack;
typedef struct _ferite_string                      FeriteString;
typedef struct _ferite_unified_array               FeriteUnifiedArray;
typedef struct _ferite_function                    FeriteFunction;
typedef struct _ferite_class                       FeriteClass;
typedef struct _ferite_script                      FeriteScript;
typedef struct _ferite_script_attached_data        FeriteScriptAttachedData;
typedef struct _ferite_object                      FeriteObject;
typedef struct _ferite_object_variable             FeriteObjectVariable;
typedef struct _ferite_function_native_information FeriteFunctionNative;
typedef struct _ferite_parameter_record            FeriteParameterRecord;
typedef struct _ferite_variable                    FeriteVariable;
typedef struct _ferite_op                          FeriteOp;
typedef struct _ferite_op_function_data            FeriteOpFncData;
typedef struct _ferite_opcode_list                 FeriteOpcodeList;
typedef struct _ferite_iterator                    FeriteIterator;
typedef struct _ferite_buffer                      FeriteBuffer;
typedef struct _ferite_namespace                   FeriteNamespace;
typedef struct _ferite_execute_rec                 FeriteExecuteRec;
typedef struct _ferite_gc_generation               FeriteGCGeneration;
typedef struct _ferite_gen_gc                      FeriteGenGC;
typedef struct _ferite_std_gc                      FeriteStdGC;
typedef struct _ferite_thread                      FeriteThread;
typedef struct _ferite_thread_group                FeriteThreadGroup;
typedef struct _ferite_namespace_bucket            FeriteNamespaceBucket;
typedef struct _ferite_module                      FeriteModule;
typedef struct _ferite_regex                       FeriteRegex;
typedef struct _ferite_native_function_record      FeriteNativeFunctionRecord;
typedef struct _ferite_op_table                    FeriteOpTable;
typedef struct _ferite_compile_record              FeriteCompileRecord;
typedef struct _ferite_bk_req                      FeriteBkRequest;
typedef struct _ferite_variable_accessors          FeriteVariableAccessors;
typedef struct _ferite_variable_subtype            FeriteVariableSubType;

typedef void (*FeriteVariableGetAccessor)(FeriteScript*,FeriteVariable*);
typedef void (*FeriteVariableSetAccessor)(FeriteScript*,FeriteVariable*,FeriteVariable*);
typedef void (*FeriteVariableCleanupAccessor)(FeriteScript*,void*);
typedef void (*FeriteAttachedDataCleanup)(FeriteScript*,int,char*,void*);

typedef struct _ferite_amt_node                    FeriteAMTNode;
typedef struct _ferite_amt_tree                    FeriteAMTTree;
typedef struct _ferite_amt                         FeriteAMT;

/*
 * The actual structures
 */

struct _ferite_amt_node {
	char type;
	union {
		FeriteAMTTree *tree;
		struct {
			unsigned long id;
			void *data;
			char *key;
		} value;
	} u;
};
struct _ferite_amt_tree {
	unsigned int    map;
	FeriteAMTNode **base;
	FeriteAMTTree  *parent;
	char            index_type;
	char            base_size;
};
struct _ferite_amt {
	FeriteAMTTree *root;
	unsigned int (*index_function)(unsigned int, unsigned int);
	unsigned int lower_bound;
	unsigned int upper_bound;
	unsigned int zero_bound;
	unsigned int last_index;
	unsigned int total;
};

struct _ferite_stack
{
    int    stack_ptr; /* The point where the top of the stack points too */
    int    size;      /* The allocated size of the stack */
    void **stack;     /* The stack itself */
};

struct _ferite_string
{
    size_t  length;     /* How long the string is */
    int     encoding;   /* What encoding the string has, eg. Latin-1 or UTF-8 etc */
    size_t  pos;        /* Current position within the string */
	char   *data;       /* The strings actual data */
};

struct _ferite_variable_accessors
{
    FeriteVariableGetAccessor      get;
    FeriteVariableSetAccessor      set;
    FeriteVariableCleanupAccessor  cleanup;
    void                          *odata;
    int                            owner;
};

struct _ferite_variable_subtype
{
	FeriteString *name;
	short         type;
	union {
		FeriteVariableSubType *_subtype;
		FeriteClass           *_class;
		FeriteNamespace       *_namespace;
	}
	data;
};

enum 
{
	FE_FLAG_DISPOSABLE = 1,   /* if we can nuke it */
	FE_FLAG_FINAL = 2,        /* if it's a final variable */
	FE_FLAG_FINALSET = 4,     /* if it's a final variable and has a value */
	FE_FLAG_COMPILED = 8,     /* if it's a compiled in constant */
	FE_FLAG_IS_STATIC = 16,   /* if the varaible is a static variable */
	FE_FLAG_PLACEHOLDER = 32, /* if this var is just a placeholder in the stack */
	FE_FLAG_STATIC_NAME = 64  /* if the variable name is allocated or not */
};

struct _ferite_variable
{
	short                    type;       /* The variables type */
	short                    flags;      /* Enum flags as above */
	short                    state;      /* Wether or not the variable is public, protected, private */
	char                    *vname;      /* It's name - used for hashing the variable and debug */
	union
	{
		long                 lval; /* The value of the variable if it is a number [long] */
		double               dval; /* The value of the variable if it is a number [double] */
		FeriteString        *sval; /* Pointer to the value if it is a string */
		FeriteObject        *oval; /* If it is an object, == NULL if it's not instantiated */
		FeriteUnifiedArray  *aval; /* The array */
		void                *pval; /* Used to sneak non-variable around the engine */
		FeriteClass         *cval; /* A class */
		FeriteNamespace     *nval; /* A namespace */
	}
	data;
	long                     index;     /* It's index if it is in an array */
	void                    *lock;      /* If the variable is 'atomic' this will be an AphexMutex */
	short                    refcount;  /* The number of references to this variable */
	FeriteVariableAccessors *accessors; /* Native variable accessors */
	FeriteVariableSubType   *subtype;
};

struct _ferite_op_function_data
{
    char            argument_count;  /* Number of arguements within the call */
};

struct _ferite_op
{
    int   OP_TYPE;              /* The op's type */
    void *opdata;               /* Usually a pointer to a char* which holds a name */
    FeriteOpFncData *opdataf;   /* the function data we require */
    long  addr;                 /* The address to jump to [depending on instruction] */
    int   line;                 /* The line the op was generated from in the source - for error reporting */
    int   block_depth;          /* block depth */
	short           flags;
};

struct _ferite_opcode_list
{
    long       size;            /* Size of the opcodelist */
    long       current_op_loc;  /* The current opcode we are pointing to, used when populating the list */
    char      *filename;        /* The file in which the opcode list was generated */
    FeriteOp **list;            /* The list */
};

struct ferite_memory_block     /* This is used by the classic memory manager for debugging */
{
    void   *data;               /* The allocated pointer */
    int     size;               /* The size of the data allocated */
    char   *file;               /* The .c file it was allocated in */
    int     line;               /* The line it was allocated on */
    struct ferite_memory_block *next;
};

struct _ferite_bk_req  /* Used within the compiler for internal address resolving in loops and jumps */
{
    FeriteOp *reqop;    /* The op that needs the address */
    int       addr;     /* .. or an address to give to an op */
    int       type;     /* The type of op/block this is for - sanity check */
};

struct _ferite_hash_bucket
{
    char *id;                /* The hash key of the bucket */
    unsigned int hashval;    /* It's hashed value */
    void *data;              /* The data it holds */
    FeriteHashBucket *next;  /* The next bucket in the chain */
};

struct _ferite_hash
{
    int size;                /* Size of the hash */
	int count;
    FeriteHashBucket **hash; /* The top level hash list */
};

struct _ferite_iterator /* Used to iterate through a hash */
{
    int curindex;                 /* Current index within the hash */
    FeriteHashBucket *curbucket;  /* The Current Bucket */
};

struct _ferite_parameter_record /* Used in function signatures */
{
    FeriteVariable *variable;          /* A variable do describe it */
	char           *name;
    char            has_default_value; /* If it has a default value - NB: not currently used */
    char            pass_type;         /* either FE_BY_VALUE or FE_BY_REFERENCE */
    char            is_dots;
};

struct _ferite_function_native_information /* Stores the native code info for builder and ferite */
{
    char *code;   /* The code in the block */
    char *file;   /* The file it was declared in */
    int   line;   /* The line it was declared on */
};

struct _ferite_function  /* Encapsulate a native and script function */
{
    char                    *name;      /* Name of the function */
    char                    type;      /* It's type - see include/ferite/ffunction.h */
    FeriteVariable         *(*fncPtr)( FeriteScript*,void*,FeriteObject*,FeriteFunction*,FeriteVariable**);
                                       /* If it is native, it's function pointer */
    FeriteFunctionNative   *native_information; /* The native infomation about the function */
    void                   *odata;     /* If we happen to have any native data */
    int                     arg_count; /* The number of arguments in the signature */
    char                    is_static; /* If the function is a static class method */
    FeriteParameterRecord **signature; /* Description of the functions' arguments */
    FeriteStack            *localvars; /* Local variables in the function - script only */
    FeriteOpcodeList       *bytecode;     /* The compiled function body */
    void                   *lock;      /* If the function is atomic this will be a AphexMutex */
    FeriteClass            *klass;     /* The class that owns this function, NULL= a namespace function */
    char                    state;      /* The state of the function - public, protected, private */
    char                    is_alias;   /* The structure is an alias and therefore can't have it's members freed */
    FeriteFunction         *next;
	int                     length; /* The length of the function in the code */
	int                     cached; /* True if the function is cached */
	int                     return_type; 
	FeriteVariableSubType  *return_subtype;
};

struct _ferite_class
{
    char               *name;           /* The name of the class */
    long                id;             /* It's unique id in the engine - runtime generated */
    short               state;          /* Abstract or final or protocol */
    void               *odata;          /* If we happen to have any native data */
    FeriteClass        *parent;         /* The class it inherits from */
    FeriteHash         *object_vars;    /* The object's variables */
    FeriteHash         *class_vars;     /* The class's variables */
    FeriteHash         *object_methods; /* The class's functions */
    FeriteHash         *class_methods;  /* The class's functions */
    FeriteClass        *next;           /* The next class in the list - NB: not used at the moment */
    FeriteNamespace    *container;      /* The namespace that the class resides in */
    FeriteStack        *impl_list;      /* List that carries the protocol list */
	int                 cached; /* True if the function is cached */
};

struct _ferite_object_variable
{
    FeriteClass          *klass;
    FeriteHash           *variables;
    FeriteObjectVariable *parent;
};

struct _ferite_object /* an actual instance of a FeriteClass */
{
    char                 *name;       /* The objects name -> same as the class's name */
    int                   oid;        /* It's id -> same as the class's id */
    void                 *odata;      /* A pointer in each object that can be used by a programmer writing
                                       * native code, this is _not_ touched by ferite. It is up to the
                                       * programmer using it to clear things up
                                       */
    int                   refcount;   /* How many times the object is referenced */
    FeriteClass          *klass;      /* the class template used */
    FeriteObjectVariable *variables;  /* Instance variables */
    FeriteHash           *functions;  /* A pointer to the class's function hash */
};

struct _ferite_buffer /* Used to quickly build up data without reallocing data */
{
   /* A buffer is a block of memory where this struct is placed first, and data are
    * appended after the struct, so the 'void *ptr' must not be free'd */
    size_t size;                /* The size of the this buffer page */
    size_t count;               /* Number of bytes allocated from this page */
    void *ptr;                  /* Pointer to where data begins */
    FeriteBuffer *next;         /* The next buffer in the list */
    FeriteBuffer *current;      /* The current buffer, is only used in the first page */
};

struct _ferite_execute_rec  /* Used in the executor to bundle information together */
{
	FeriteExecuteRec   *parent;
	FeriteFunction     *function;       /* The function being executed */
	FeriteVariable    **variable_list;  /* A duplicate of the function's local variable hash */
	FeriteStack        *stack;          /* The function's local execution stack */
	int                 block_depth;
	int                 line;
};

struct _ferite_gc_generation /* Used in the generational GC */
{
	int size;                     /* Size of the generation */
	int next_free;                /* The next free slot in the generation */
	FeriteObject **contents;      /* The contents holds pointers to the objects */
	FeriteGCGeneration *younger;  /* The generation younger than this one */
	FeriteGCGeneration *older;    /* The generation older than this one */
};

struct _ferite_std_gc /* Standard 'old skool' GC */
{
	FeriteObject **contents; /* The contents holds pointers to the objects */
	int            size;     /* Size of the GC */
};

struct _ferite_thread
{
	void         *ctxt;    /* actually an AphexThread* */
	FeriteScript *script;  /* The script the thread is running in */
	FeriteObject *obj;     /* The thread object that owns the thread */
	int           running; /* Is it running ? */
	int           referenced; /* true until a thread destructor is called */
	int           pass_exceptions; /* Should we pass exceptions in the thread onto the parent script? */
};

struct _ferite_thread_group /* Used to group running threads so a script wont exit until they
                             * have finished */
{
    void         *lock;         /* The lock */
    FeriteStack  *thread_list;  /* The list of currently running threads. A thread gets added when the
                                 * .start() method gets called */
    FeriteScript *owner; /* The script that owns this thread_group */
};

struct _ferite_script
{
    /* once compiled these variables never change */
    char               *filename;           /* The file the script was generated from */
    char               *scripttext;         /* The contents of the script file - only used during compilation */
    FeriteNamespace    *mainns;             /* The main namespace of the script */
	FeriteClass        *closureClassPtr;
    
    /* only these variables are altered */
    FeriteStack        *include_list;       /* Absolute path list of all included modules */

    /* Threading stuff */
    void               *lock;               /* Script based lock */
    FeriteThreadGroup  *thread_group;       /* ... and it's thread group */
    FeriteScript       *parent;             /* The parent script where the thread was executed from */
	char                is_multi_thread;    /* optimisations */
    
    /* Caching stuff */
    FeriteStack        *vars;               /* Variable cache */
    FeriteStack        *objects;            /* Object cache */
    FeriteStack        *stacks;             /* Stack cache */
    
    /* error stuff */
    char               *current_op_file;    /* File being executed in */
    unsigned int        current_op_line;    /* Current line */
    unsigned int        error_state;        /* The error state of the script */
    unsigned int        keep_execution;     /* If FE_FALSE the script will stop running */
    unsigned int        is_executing;       /* If the script is running */
    unsigned int        is_being_deleted;   /* If the script is being deleted */
    unsigned int        return_value;       /* The scripts return value */
    unsigned int        stack_level;        /* How deep are we in execution depth? */
    /* regex counting */
    unsigned int        last_regex_count;   /* The number of captured strings last time round */

    FeriteBuffer        *error;             /* The error messages */
    FeriteBuffer        *warning;           /* The warning messages */

    /* ferite gc */
    void               *gc;                 /* The scripts GC */
    int                 gc_running;         /* Is the GC running ? */
    int                 gc_count;           /* Count ops since last GC invocation */
    void               *gc_lock;            /* GC lock */
	FeriteExecuteRec   *gc_stack;           /* We need this to do a GC run */
    
    /* user information */
    FeriteHash         *_odata;             /* A programmer can attach data to a script if they so wish */  
    int                 odata_id;           /* Uid for the data you get */  
                        /* Renamed from odata to _odata to deliberately break code */

	FeriteAMT          *globals;
	FeriteAMT          *types;
};

struct _ferite_script_attached_data 
{
   int                        id;
   void                      *data;
   FeriteAttachedDataCleanup  cleanup;
};

struct _ferite_namespace_bucket
{
    int   type;  /* Type of data - see include/ferite/fns.h */
    void *data;  /* The data */
};

struct _ferite_namespace
{
    char            *name;       /* The name of the namespace */
    int              num;        /* Size of the namespace */
    int             *code_fork_ref;
    FeriteHash      *data_fork;
    FeriteHash      *code_fork; /* The code hash! */
    FeriteNamespace *container;  /* The namespace that holds this namespace */
};

struct _ferite_module /* A native module that is loaded by ferite */
{
    char *name;      /* Name of the module */
    char *filename;  /* It's filename */
    void *handle;    /* Pointer to the dl'd library */
   /* These are the functions that a module must export to be 'ferite' friendly */
    void (*module_register)();
    void (*module_unregister)();
    void (*module_init)( FeriteScript *script );
    void (*module_deinit)( FeriteScript *script );
    FeriteModule *next; /* Next module */
};

struct _ferite_regex
{
    char       *pattern;         /* original string pattern */
    int         pcre_options;    /* pcre options */
    int         fergx_options;   /* regex options we have to implement */
    void       *compiled_re;     /* compiled regex */
    char       *compile_buf;     /* the actual buffer to compile */
    char       *swap_buf;        /* string to swap with */
    void       *extra_info;      /* extra information */
};

struct _ferite_unified_array
{
    FeriteHash      *hash;        /* The hash keys are stored in this, strange huh? ;) */
    FeriteVariable **array;       /* our array */
    long             size;        /* where we are in the array */
    long             actual_size; /* size of allocated memory */
    long             iteration;   /* where we are currently sitting in the array */
    void            *iterator;    /* the iterator on the array */
    int              iterator_type; /* So we can maintain void on the iteration */
	int              copy_on_write;
};

struct _ferite_native_function_record /* This is used to register a native function when a module
                                       * is loaded */
{
    FeriteVariable *(*function)( FeriteScript *, void *, FeriteObject*, FeriteFunction*, FeriteVariable ** );
};

struct _ferite_op_table /* Table of op-codes */
{
    int    id;    /* Op ID - how it is refered to in the compiler */
    char  *name;  /* Name of the op */
    void  *ptr;   /* Pointer to it's function */
};

struct _ferite_compile_record /* Used in the compiler */
{
    FeriteFunction     *function;
    FeriteStack        *variables;
    FeriteClass        *cclass;
    FeriteScript       *script;
    FeriteNamespace    *ns;
	FeriteStack        *shadowed_local_variables;
	FeriteStack        *local_scope_frame;
	FeriteHash         *local_variable_hash;  
	FeriteVariable     *last_script_return;
	int in_closure;
	int want_container_finish;
};

#endif /* __FERITE_STRUCTS_H__ */
