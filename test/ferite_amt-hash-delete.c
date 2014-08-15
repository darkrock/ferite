#include "tap.h"
#include "test_amt.h"
#include "ferite.h"

void testDeleteOneItem() {
	struct HashTest tests[] = {
		{ "foo", "bar", "bar" },
	};
	int ntests = ARRAY_SIZE(tests);
	FeriteAMT *amt = testHashPut("One Item", tests, ntests);
	char *value = ferite_hamt_delete(NULL, amt, "foo");
	is_str(value, "bar", "One Item: Delete '%s' must return '%s'", "foo", "bar");
	value = ferite_hamt_get(NULL, amt, "foo");
	ok(value == NULL, "One Item: Get deleted item must return NULL");
}

void testDeleteTwoItems() {
	struct HashTest tests[] = {
		{ "foo", "bar", "bar" },
		{ "baz", "quux", "quux" },
	};
	int ntests = ARRAY_SIZE(tests);
	FeriteAMT *amt = testHashPut("Two items:", tests, ntests);
	char *value = ferite_hamt_delete(NULL, amt, "foo");
	is_str(value, "bar", "Two Items: Delete '%s' must return '%s'", "foo", "bar");
	value = ferite_hamt_get(NULL, amt, "foo");
	ok(value == NULL, "Two Items: Get deleted item must return NULL");

	value = ferite_hamt_delete(NULL, amt, "baz");
	is_str(value, "quux", "Two Items: Delete '%s' must return '%s'", "baz", "quux");
	value = ferite_hamt_get(NULL, amt, "baz");
	ok(value == NULL, "Two Items: Get deleted 2nd item must return NULL");
}

void testDeleteWithCollision() {
	char *desc = "Delete With Collison";
	struct CustomHashTest tests[] = {
		{ 1, "f", "january", "january" },
		{ 1, "fo", "february", "february" },
		{ 1, "fooo", "march", "march" },
	};
	int ntests = ARRAY_SIZE(tests);
	FeriteAMT *amt = testCustomHashPut(NULL, desc, tests, ntests);
	char *value = _ferite_amt_delete(NULL, amt, 1, tests[0].key);
	is_str(value, tests[0].value, "%s: Delete '%s' must return '%s'", desc, tests[0].key, tests[0].value);
}

int main(int argc, char *argv[]) {
	ferite_init(argc, argv);
	testDeleteOneItem();
	testDeleteTwoItems();
	testDeleteWithCollision();
	return done_testing();
}
