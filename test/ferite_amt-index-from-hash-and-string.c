#include "tap.h"
#include "ferite.h"

struct HashAndKeyAndIndeces {
	char *desc;
	unsigned int maxDepth;
	unsigned int hash;
	char *key;
	int expectedIndeces[20];
};

#define BYTES_IN_HASH sizeof(unsigned int)
#define NBITS(nbytes) (8 * (nbytes))
#define NBITS2DEPTH(nbits) ((nbits/5) + (nbits%5?1:0))
#define maxDepthForHashWithKey(str) NBITS2DEPTH(NBITS(BYTES_IN_HASH + sizeof(str) - 1))
#define TEST(str, hash, key) str " with key = \"" key "\"", \
	maxDepthForHashWithKey(key), \
  hash, \
  key
struct HashAndKeyAndIndeces testVectors[] = {
	{
		TEST("Sequential", 0x08864299, ""),
	//      MSB...LSB MSB...LSB
	//  ab: 0110 0001 0110 0010
	//          Hex:    0    8    8    6    4   2     9    9|
	//       Binary: 0000 1000 1000 0110 0100 0010 1001 1001|...
	/*5-bit Decimal:*/{   1,    2,    3,    4,     5,    6,    8 }
	//        depth:      0     1     2     3      4     5     6
	},
	{
		TEST("Sequential", 0x08864298, ""),
	//      MSB...LSB MSB...LSB
	//  ab: 0110 0001 0110 0010
	//          Hex:    0    8    8    6    4   2     9    8|
	//       Binary: 0000 1000 1000 0110 0100 0010 1001 1000|...
	/*5-bit Decimal:*/{   1,    2,    3,    4,     5,    6,    0 }
	//        depth:      0     1     2     3      4     5     6
	},
	{
		TEST("Sequential", 0x08864299, "a"),
	//      MSB   LSB
	//   a: 0110 0001                                        a
	//          Hex:    0    8    8    6    4   2     9    9|MSB   LSB
	//       Binary: 0000 1000 1000 0110 0100 0010 1001 1001|0110 0001
	/*5-bit Decimal:*/{   1,    2,    3,    4,     5,    6,   11,    1 }
	//        depth:      0     1     2     3      4     5     6     7
	},
	{
		TEST("Sequential", 0x08864299, "ab"),
	//      MSB...LSB MSB...LSB
	//  ab: 0110 0001 0110 0010                              b         a
	//          Hex:    0    8    8    6    4   2     9    9|
	//       Binary: 0000 1000 1000 0110 0100 0010 1001 1001|0110 0010 0110 0001..
	/*5-bit Decimal:*/{   1,    2,    3,    4,     5,    6,   11,    2,    12,   4 }
	//        depth:      0     1     2     3      4     5     6     7      8    9
	},
	{
		TEST("Sequential", 0x08864290, "ab"),
	//      MSB...LSB MSB...LSB
	//  ab: 0110 0001 0110 0010                              b         a
	//          Hex:    0    8    8    6    4   2     9    0|
	//       Binary: 0000 1000 1000 0110 0100 0010 1001 0000|0110 0010 0110 0001..
	/*5-bit Decimal:*/{   1,    2,    3,    4,     5,    4,    3,    2,    12,   4 }
	//        depth:      0     1     2     3      4     5     6     7      8    9
	},
	{
		TEST("Sequential", 0x08864299, "AIW"),
	//                                                       W         I         A
	//          Hex:    0    8    8    6    4   2     9    9|   5    7    4    9    4    1
	//       Binary: 0000 1000 1000 0110 0100 0010 1001 1001|0101 0111 0100 1001 0100 0001 ....
	/*5-bit Decimal:*/{   1,    2,    3,    4,     5,    6,   10,   23,     9,    5,    0,   16 }
	//        depth:      0     1     2     3      4     5     6     7      8     9,   10,   11
	},
	{
		TEST("Sequential", 0x08864299, "abcdef"),
	//      MSB...LSB MSB...LSB
	//  ab: 0110 0001 0110 0010                              f         e         d         c         b         a
	//          Hex:    0    8    8    6    4   2     9    9|
	//       Binary: 0000 1000 1000 0110 0100 0010 1001 1001|0110 0110 0110 0101 0110 0100 0110 0011 0110 0010 0110 0001
	/*5-bit Decimal:*/{   1,    2,    3,    4,     5,    6,   11,    6,    12,   21,   18,    6,     6,   24,   19,    1 }
	//        depth:      0     1     2     3      4     5     6     7      8     9    10    11     12    13    14    15
	},
	{
		TEST("Sequential", 0x08864299, "abcdefg"),
	//      MSB...LSB MSB...LSB
	//  ab: 0110 0001 0110 0010                              g         f         e         d         c         b         a
	//          Hex:    0    8    8    6    4   2     9    9|
	//       Binary: 0000 1000 1000 0110 0100 0010 1001 1001|0110 0111 0110 0110 0110 0101 0110 0100 0110 0011 0110 0010 0110 0001 ..
	/*5-bit Decimal:*/{   1,    2,    3,    4,     5,    6,   11,    7,    12,   25,   18,   22,     8,   24,   27,    2,    12,    4  }
	//        depth:      0     1     2     3      4     5     6     7      8     9    10    11     12    13    14    15     16    17
	},
};

void testIndexFromHashAndString() {
	size_t ntest = sizeof(testVectors) / sizeof(testVectors[0]);
	size_t i;
	for (i = 0; i < ntest; i++) {
		struct HashAndKeyAndIndeces *t = &testVectors[i];
		size_t d = 0;
		size_t keyLen = strlen(t->key);
		for (d = 0; d < t->maxDepth; d++) {
			size_t got = ferite_hamt_index(t->hash, t->key, keyLen, d);
			is (got, t->expectedIndeces[d],
					"Index From Hash And String: %s depth = %d", t->desc, d);
		}
	}
}

int main()
{
	is (maxDepthForHashWithKey("a"), 8,
			"hash with key \"a\" must have depth %d",
			maxDepthForHashWithKey("a"));
	is (maxDepthForHashWithKey("ab"), 10,
			"hash with key \"ab\" must have depth %d",
			maxDepthForHashWithKey("ab"));

	testIndexFromHashAndString();
	return done_testing();
}
