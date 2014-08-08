#include "test_amt.h"
#include "tap.h"

FeriteAMT *_testHashPut(FeriteAMT *amt, char *desc, struct HashTest *tests,  size_t ntests) {
	struct HashTest *p;
	size_t i;

	if (amt == NULL) {
		amt = ferite_AMTHash_Create( NULL );
	}

	for (i = 0; i < ntests; i++) {
		p = &tests[i];
		char *key = p->key;
		char *value = p->value;
		char *want = p->returnedValue;
		char *ret = ferite_hamt_set( NULL, amt, key, value );
		is_str (ret, want, "%s: Put '%s' with value '%s' must return '%s'", desc, key, value, want);
	}
	return amt;
}

void _testHashGet(FeriteAMT *amt, char *desc, struct HashTest *tests,  size_t ntests) {
	struct HashTest *p;
	char *got;
	size_t i;

	if (amt == NULL) {
		amt = ferite_AMTHash_Create( NULL );
	}
	for (i = 0; i < ntests; i++) {
		p = &tests[i];
		char *key = p->key;
		char *want = p->value;
		got = (char *)ferite_hamt_get(NULL, amt, key);
		is_str(got, want, "%s: Get '%s' must return '%s'", desc, key, want);
	}
}

FeriteAMT *testHashPut(char *desc, struct HashTest *tests,  size_t ntests) {
	return _testHashPut(NULL, desc, tests, ntests);
}

FeriteAMT *testHashPutAndGet(char *desc, struct HashTest *tests,  size_t ntests) {
	FeriteScript *script = NULL;
	FeriteAMT *amt = ferite_AMTHash_Create( script );

	_testHashPut(amt, desc, tests, ntests);
	_testHashGet(amt, desc, tests, ntests);
	return amt;
}

FeriteAMT *testCustomHashPut(FeriteAMT *amt, char *desc, struct CustomHashTest *tests,  size_t ntests) {
	size_t i;
	FeriteScript *script = NULL;

	if (amt == NULL) {
		amt = ferite_AMTHash_Create( script );
	}

	for (i = 0; i < ntests; i++) {
		struct CustomHashTest *t = &tests[i];
		unsigned long hash = t->hash;
		char *key =t->key;
		char *value = t->value;
		char *want = t->returnedValue;
		char *got = (char *) _ferite_amt_set( NULL, amt, hash, key, value );
		is_str (got, want, "%s: hash %lu Put '%s' with value '%s' must return '%s'",
				desc, hash, key, value, want);
	}
	return amt;
}

void testCustomHashGet(FeriteAMT *amt, char *desc, struct CustomHashTest *tests,  size_t ntests) {
	size_t i;

	for (i = 0; i < ntests; i++) {
		struct CustomHashTest *t = &tests[i];
		unsigned long hash = t->hash;
		char *key =t->key;
		char *want = t->value;
		FeriteAMTNode *baseItem = _ferite_hamt_get_with_key(NULL, amt, hash, key);

		ok (baseItem != NULL, "Must get baseItem for key '%s'", key);
		if (baseItem) {
			char *got = baseItem->u.value.data;
			is_str(got, want, "%s: hash %lu Get '%s' must return '%s'",
					desc, hash, key, want);
		}
	}
}

FeriteAMT *testCustomHashPutAndGet(char *desc, struct CustomHashTest *tests,  size_t ntests) {
	FeriteAMT *amt = testCustomHashPut(NULL, desc, tests, ntests);

	testCustomHashGet(amt, desc, tests, ntests);
	return amt;
}

