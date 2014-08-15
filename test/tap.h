#ifndef TAP_H
#define TAP_H
#include <string.h>
#include <stdarg.h>
int vok(int status, char *msg, va_list ap);
int ok(int status, char *msg, ...);
int is_str(char *a, char *b, char *msg, ...);
int is(int a, int b, char *msg, ...);
void diag(char *msg, ...);
int done_testing();
#endif
