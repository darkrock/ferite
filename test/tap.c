#include <stdio.h>
#include "tap.h"

int ntest = 0;
int nfailed = 0;

char *OK = "ok";
char *NOT_OK = "not ok";

char *update_counters_and_get_status_str(int status) {
	ntest++;
	if (!status) {
		nfailed++;
		return NOT_OK;
	}
	return OK;
}

int vok(int status, char *msg, va_list ap) {
	char *out = update_counters_and_get_status_str(status);
	printf("%s %d - ", out, ntest);
	vprintf(msg, ap);
	printf("\n");
	return status;
}

int ok(int status, char *msg, ...) {
	va_list ap;
	char *out = update_counters_and_get_status_str(status);
	printf("%s %d - ", out, ntest);
	va_start(ap, msg);
	vprintf(msg, ap);
	va_end(ap);
	printf("\n");
	return status;
}

int is(int got, int want, char *msg, ...) {
	int success = 0;
	va_list ap;
	if (got == want) {
		success = 1;
	}
	va_start(ap, msg);
	vok (success, msg, ap);
	va_end(ap);
	if (! success) {
		printf("want: %d\n", want);
		printf(" got: %d\n", got);
	}
	return success;
}

int is_str(char *got, char *want, char *msg, ...) {
	int success = 0;
	va_list ap;
	if (got == NULL && want == NULL) {
		success = 1;
	} else if (got == NULL || want == NULL) {
		success = 0;
	} else {
		success = strcmp(got, want) == 0;
	}
	va_start(ap, msg);
	vok (success, msg, ap);
	va_end(ap);
	if (! success) {
		printf("want: \"%s\"\n", want);
		printf(" got: \"%s\"\n", got);
	}
	return success;
}

void diag(char *msg, ...) {
	va_list ap;
	printf("# ");
	va_start(ap, msg);
	vprintf(msg, ap);
	va_end(ap);
	printf("\n");
}

int done_testing() {
	printf("1..%d\n", ntest);
	return nfailed != 0;
}
