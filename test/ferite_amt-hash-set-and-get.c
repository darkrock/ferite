#include "tap.h"
#include "test_amt.h"

#include "ferite.h"

void test2HashCollisions() {
	// The int hashes for the keys in the following key-value pair collides
	struct HashTest collides[] = {
		/*
		 * Note: ferite_AMTHash_create() delegates to
		 * ferite_amt_create(script, FeriteAMT_High) which sets the FeriteAMT to use the
		 * ferite_highorderindex for shifting the hash, and
		 * ferite_highorderindex() has this signature:
		 *
		 * unsigned int ferite_highorderindex( unsigned int index, unsigned int shiftAmount );
		 *
		 * So only the lower 32 bits (the int equivalent) of the long hash are
		 * used for getting the index.
		 *
		 * This should be fixed so that the index function uses all the bits in
		 * the long hash instead of casting it to int.
		 */

		/* Different long hash but same int hash */
		// long 16871661377530893130 int 174592842
		{ "object:Workflow.Tag/view:1/44" , "foo", "foo" },
		// long 12494831704221422410 int 174592842
		{ "object:Workflow.Action/view:1/231293" , "bar", "bar" },

		/* Same long hash and same int hash */
		// long 99461941 int 99461941
		{ "hones", "value1", "value1" },
		//	long 99461941 int 99461941
		{ "hop's", "value2", "value2" },
	};
	int ntest = sizeof(collides) / sizeof(collides[0]);

	testHashPutAndGet("Hash Collision", collides, ntest);
}

void testNoCollision() {
	struct HashTest tests[] = {
		{ "one", "two", "two" },
		{ "three", "four", "four" },
		{ "five", "six", "six" },
		{ "seven", "8", "8" },
		{ "nine", "10", "10" },
	};
	int ntest = sizeof(tests) / sizeof(tests[0]);

	testHashPutAndGet("No Collision", tests, ntest);

}

void testPutWithZeroLengthKey() {
	struct CustomHashTest tests[] = {
		{ 1, "", "value for empty key", "value for empty key" },
	};
	int ntest = sizeof(tests) / sizeof(tests[0]);

	testCustomHashPutAndGet("One Zero Length", tests, ntest);
}

void testPutWithZeroLengthKeyWithNonZeroLengthKey() {
	struct CustomHashTest tests[] = {
		{ 1, "", "value for empty key", "value for empty key" },
		{ 1, "foo", "value for foo", "value for foo" },
	};
	int ntest = sizeof(tests) / sizeof(tests[0]);

	testCustomHashPutAndGet("Zero Length With Nonzero Length", tests, ntest);
}

void testCustomSameHash() {
	struct CustomHashTest tests[] = {
		{ 1, "key1", "value1", "value1" },
		{ 1, "key2", "value2", "value2" },
		{ 1, "k", "value3", "value3" },
		{ 1, "key2a", "value4", "value4" },
	};
	int ntest = sizeof(tests) / sizeof(tests[0]);

	testCustomHashPutAndGet("CustomSameHash", tests, ntest);
}

void testCollisionWithMissingOptionalKeyMustReturnNull() {
	char *desc = "Missing Optional Key";
	FeriteScript *script = NULL;
	FeriteAMT *amt = ferite_AMTHash_Create( script );
	char *key = "foo";
	char *value = "a";
	char *got = (char *) _ferite_amt_set(NULL, amt, 1, key, value);

	is_str(got, value, "%s: Put '%s' must return '%s'", desc, key, value);

	key = NULL;
	value ="b";
	got = (char *) _ferite_amt_set(NULL, amt, 1, key, value);
	is_str(got, NULL, "%s: NULL key must return NULL", desc);
}

void testCollisionWithMatchingKeyMustReplaceExistingOne() {
	char *desc = "Collision With Matching Key";
	FeriteScript *script = NULL;
	FeriteAMT *amt = ferite_AMTHash_Create( script );
	char *key = "foo";
	char *value = "a";
	char *got = (char *) _ferite_amt_set(NULL, amt, 1, key, value);

	is_str(got, value, "%s: Put '%s' must return '%s'", desc, key, value);

	key = "foo";
	value ="b";
	got = (char *) _ferite_amt_set(NULL, amt, 1, key, value);
	is_str(got, "a", "%s: Put '%s' must return '%s'", desc, key, "a");

	got = (char *)ferite_hamt_get(NULL, amt, key);
	ok (got != NULL, "%s: ferite_hamt_get must not return NULL", desc);
	if (got) {
		is_str(got, "b", "%s: Get '%s' must return '%s'", desc, key, value);
	}
}

void testReplaceExistingMustReturnOldValue() {
	struct HashTest tests[] = {
		{ "foo", "elephant", "elephant" },
		{ "foo", "hippo", "elephant" },
	};
	int ntest = sizeof(tests) / sizeof(tests[0]);

	testHashPut("Replace must return old", tests, ntest);

}

void test3HashCollisions() {
	struct CustomHashTest tests[] = {
		{ 1, "f", "mammal", "mammal" },
		{ 1, "fo", "bird", "bird" },
		{ 1, "foo", "fish", "fish" },
	};
	int ntest = sizeof(tests) / sizeof(tests[0]);

	testCustomHashPutAndGet("3 Hash Collisions", tests, ntest);
}

int main(int argc, char *argv[]) {
	ferite_init(argc, argv);
	testNoCollision();
	test2HashCollisions();
	testCustomSameHash();
	testCollisionWithMissingOptionalKeyMustReturnNull();
	testCollisionWithMatchingKeyMustReplaceExistingOne();
	testReplaceExistingMustReturnOldValue();
	testPutWithZeroLengthKey();
	testPutWithZeroLengthKeyWithNonZeroLengthKey();
	test3HashCollisions();
	return done_testing();
}
