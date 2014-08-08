#include "tap.h"
#include "ferite.h"

struct TestVector {
	char *key;
	unsigned long hash;
} tests[] = {
	{
		"elephant",
		2877622596369UL,
	},
	{
		"giraffe",
		94527212404UL,
	}
};

void testHashGen() {
	size_t i;
	size_t ntests = sizeof(tests) / sizeof(tests[0]);

	for (i = 0; i < ntests; i++) {
		unsigned long got = ferite_hamt_hash_gen(tests[i].key);
		is (got, tests[i].hash,
				"key %s must hash to %lu",
				tests[i].key, tests[i].hash);
	}
}

int main() {
	testHashGen();
	done_testing();
	return 0;
}
