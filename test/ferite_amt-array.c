#include "tap.h"
#include "ferite.h"

void testArrayCreate() {
	FeriteAMT *amt = ferite_AMTArray_Create(NULL);
	is (amt->lower_bound, 0, "Create: lower bound must be 0");
	is (amt->upper_bound, 0, "Create: upper bound must be 0");
}

void testArraySetAndGet() {
	char *got;
	char *want = "Item 10";

	FeriteAMT *amt = ferite_AMTArray_Create(NULL);
	ferite_amt_set(NULL, amt, 10, want);
	is (amt->upper_bound, 10, "After add at 10 upper bound must be 10");
	got = ferite_amt_get(NULL, amt, 10);
	is_str(got, want, "Get array[10] must return '%s'", want);
}

int main(int argc, char *argv[]) {
	ferite_init(argc, argv);
	testArrayCreate();
	testArraySetAndGet();
	return done_testing();
}
