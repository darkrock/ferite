#ifndef __FERITE_PROFILE_H__
#define __FERITE_PROFILE_H__

#ifdef FERITE_PROFILE
#include <time.h>

extern int ferite_profile_enabled;

struct timespec ferite_profile_function_begin(FeriteScript *script, void *container, FeriteFunction *function, FeriteProfileFunction **fpf);
struct timespec ferite_profile_function_end(FeriteScript *script, FeriteProfileFunction *fpf, int end_line, struct timespec *begin_ts);
struct timespec ferite_profile_line_begin(FeriteScript *script, const char *filename, const size_t line);
void ferite_profile_line_end(FeriteScript *script, const char *filename, const size_t line, struct timespec *begin_ts);
FeriteProfileFunction *ferite_profile_get_function_profile(FeriteScript *script, void *container, FeriteFunction *function);
void ferite_profile_set_caller(FeriteScript *script, void *container, FeriteFunction *function);
void ferite_profile_add_caller(FeriteProfileFunction *callee, FeriteProfileFunction *caller, int caller_line, struct timespec duration);

#endif /* FERITE_PROFILE */
#endif /* __FERITE_PROFILE_H__ */
