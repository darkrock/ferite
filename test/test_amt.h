#ifndef TEST_AMT_H
#define TEST_AMT_H
#include "ferite.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

struct HashTest {
	char *key;
	char *value;
	char *returnedValue; /* Expected returned value when storing */
};

struct CustomHashTest {
	unsigned long hash; /* Custom hash for the key, skips
						   ferite_hamt_hash_gen() */
	char *key;
	char *value;
	char *returnedValue;
};

FeriteAMT *_testHashPut(FeriteAMT *amt, char *desc, struct HashTest *tests,  size_t ntests);
void _testHashGet(FeriteAMT *amt, char *desc, struct HashTest *tests,  size_t ntests);
FeriteAMT * testHashPut(char *desc, struct HashTest *tests,  size_t ntests);
FeriteAMT *testHashPutAndGet(char *desc, struct HashTest *tests,  size_t ntests);
FeriteAMT *testCustomHashPutAndGet(char *desc, struct CustomHashTest *tests,  size_t ntests);
FeriteAMT *testCustomHashPut(FeriteAMT *amt, char *desc, struct CustomHashTest *tests,  size_t ntests);
void testCustomHashGet(FeriteAMT *amt, char *desc, struct CustomHashTest *tests,  size_t ntests);


#endif
