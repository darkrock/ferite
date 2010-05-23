#ifndef ARRAY_UTIL_H
#define ARRAY_UTIL_H

struct sort_variable {
    FeriteScript *script;  /* The script this variable belongs to. */
    FeriteVariable *var;
    FeriteObject *closure;
};

int fe_compare_vars(const void *a, const void *b);

#endif
