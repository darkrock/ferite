#include "array_header.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static int compare_longs(long a, long b)
{
    if(a < b) return -1;
    else if(a == b) return 0;
    else return 1;
}

static int compare_doubles(double a, double b)
{
    if(a < b) return -1;
    else if(a == b) return 0;
    else return 1;
}

static int compare_strings(FeriteString *a, FeriteString *b)
{
    return memcmp(a->data, b->data, MIN(a->length, b->length));
}

static int compare_uarrays(FeriteUnifiedArray *a, FeriteUnifiedArray *b)
{
/* When we compare two arrays, we actually just compare the number of elements
 * in each array. */
    if(a->size < b->size) return -1;
    else if(a->size == b->size) return 0;
    else return 1;
}

static int compare_objs(FeriteScript *script, FeriteObject *a, FeriteObject *b)
{
    int ret = 0;
    FeriteFunction *ff;
    FeriteVariable *fret, **params;

    if(!(params = ferite_create_parameter_list_from_data(script, "oo", a, b)))
        return 0;
    
    if(!(ff = ferite_class_get_function_for_params(script, a->klass, "compare", params)))
      return 0;

    fret = ferite_call_function(script, a->klass, NULL, ff, params);
    ferite_delete_parameter_list(script, params);
    if(!fret) return 0;

    if(F_VAR_TYPE(fret) == F_VAR_LONG) ret = (int)VAI(fret);
    else if(F_VAR_TYPE(fret) == F_VAR_DOUBLE) ret = (int)VAF(fret);

    ferite_variable_destroy(script, fret);

    return ret;
}

/* This variable comparison function is only intended to be called by qsort. */
int fe_compare_vars(const void *a, const void *b)
{
    struct sort_variable *sv1 = (struct sort_variable *)a;
    struct sort_variable *sv2 = (struct sort_variable *)b;
	FeriteScript *script = sv1->script;
	
    if( sv1->closure != NULL )
    {
        int ret = 0;
        FeriteFunction *ff = NULL;
        FeriteVariable *fret, **params = NULL;
        
        params = fmalloc( sizeof(FeriteVariable*) * 3 );
        params[0] = sv1->var;
        params[1] = sv2->var;
        params[2] = NULL;
        
        if(!(ff = ferite_object_get_function(sv1->script, sv1->closure, "invoke")))
            return 0;
        
        fret = ferite_call_function(sv1->script, sv1->closure, NULL, ff, params);
        ffree(params);
        if(!fret) return 0;
        
        if(F_VAR_TYPE(fret) == F_VAR_LONG) ret = (int)VAI(fret);
        else if(F_VAR_TYPE(fret) == F_VAR_DOUBLE) ret = (int)VAF(fret);
        
        ferite_variable_destroy(sv1->script, fret);
        
        return ret;
    }
    else
    {
        /* We need to cope with the case of one variable being a long and the other
        * being a double, because as far as the user is concerned they are both
        * the same type (ie. numbers): */
        if(F_VAR_TYPE(sv1->var) == F_VAR_LONG && F_VAR_TYPE(sv2->var) == F_VAR_DOUBLE)
            return compare_doubles((double)VAI(sv1->var), VAF(sv2->var));
        else if(F_VAR_TYPE(sv1->var) == F_VAR_DOUBLE && F_VAR_TYPE(sv2->var) == F_VAR_LONG)
            return compare_doubles(VAF(sv1->var), (double)VAI(sv2->var));
        else if(F_VAR_TYPE(sv1->var) != F_VAR_TYPE(sv2->var)) return 0;
        
        switch(F_VAR_TYPE(sv1->var))
        {
            case F_VAR_LONG:
                return compare_longs(VAI(sv1->var), VAI(sv2->var));
            case F_VAR_DOUBLE:
                return compare_doubles(VAF(sv1->var), VAF(sv2->var));
            case F_VAR_STR:
                return compare_strings(VAS(sv1->var), VAS(sv2->var));
            case F_VAR_UARRAY:
                return compare_uarrays(VAUA(sv1->var), VAUA(sv2->var));
            case F_VAR_OBJ:
                return compare_objs(sv1->script, VAO(sv1->var), VAO(sv2->var));
        }
    }

    return 0;
}
