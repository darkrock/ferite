#include "tap.h"
#include "ferite.h"

#define FeriteAMT_SHIFT_AMOUNT (5) // This must equal to the one defined in ObjAMT.c
#define FeriteAMT_SHIFT_START                (32)

struct TestVector {
	char *desc;
	unsigned int  hash;
	int values[7];
};

struct TestVector highOrderIndexTests[] = {
	{
		"High Order All Ones",
		0xffffffff,
	//          Hex:    f    f    f    f    f   f     f    f
	//       Binary: 1111 1111 1111 1111 1111 1111 1111 1111 ...
	//5-bit Decimal:     31    31    31    31     31    31    24
		{				 31,   31,   31,   31,    31,   31,   24 }
	},
	{
		"High Order All Zeroes",
		0x00000000,
	//          Hex:    0    0    0    0    0   0     0    0
	//       Binary: 0000 0000 0000 0000 0000 0000 0000 0000 ...
	//5-bit Decimal:      0     0     0     0      0     0     0
		{				  0,    0,    0,    0,     0,    0,    0 }
	},
	{
		"High Order Sequence",
		0x08864299,
	//          Hex:    0    8    8    6    4   2     9    9
	//       Binary: 0000 1000 1000 0110 0100 0010 1001 1001 ...
	//5-bit Decimal:      1     2     3     4      5     6     8
		{				  1,    2,    3,    4,     5,    6,    8 }
	},
	{
		"High Order Sequence Desc",
		0xffbbcde9,
	//          Hex:    f    f    b    b    c   d     e    9
	//       Binary: 1111 1111 1011 1011 1100 1101 1110 1001 ...
	//5-bit Decimal:     31    30    29    28     27    26     8
		{				 31,   30,   29,   28,    27,   26,    8 }
	}

};

struct TestVector lowOrderIndexTests[] = {
	{
		"Low Order All Ones",
		0xffffffff,
	//           Hex:        f    f    f    f    f    f    f    f
	//        Binary: ... 1111 1111 1111 1111 1111 1111 1111 1111
	//x5-bit Decimal:      3    31    31     31    31    31    31
		{				   3,   31,   31,    31,   31,   31,   31 }
	},
	{
		"Low Order All Zeroes",
		0x00000000,
	//           Hex:        0    0    0    0    0    0    0    0
	//        Binary: ... 0000 0000 0000 0000 0000 0000 0000 0000
	//x5-bit Decimal:      0     0     0      0     0     0     0
		{				   0,    0,    0,     0,    0,    0,    0 }
	},
	{
		"Low Order Sequence",
		0xcc520c41,
	//           Hex:        c    c    5    2    0    c    4    1
	//        Binary: ... 1100 1100 0101 0010 0000 1100 0100 0001
	// 5-bit Decimal:      3     6     5      4     3     2     1
		{				   3,    6,    5,     4,    3,    2,    1 }
	},
	{
		"Low Order Sequence Desc",
		0xfdde6f59,
	//           Hex:        f    d    d    e    6    f    5    9
	//        Binary: ... 1111 1101 1101 1110 0110 1111 0101 1001
	// 5-bit Decimal:      3    30    29     28    27    26    25
		{				   3,   30,   29,    28,   27,   26,   25 }
	}
};


void runHighOrderIndexTests(char *desc, unsigned int hash, int values[], size_t sz) {
	int shiftAmount = 32;
	size_t i = 0;
	while (i < sz) {
		int want = values[i];
		int got = ferite_highorderindex(hash, shiftAmount);
		is(got, want, "%s: ferite_highorderindex(0x%x,  %d) must return %d",
				desc, hash, shiftAmount, want);
		shiftAmount -= FeriteAMT_SHIFT_AMOUNT;
		i++;
	}
}

void runLowOrderIndexTests(char *desc, unsigned int hash, int values[], size_t sz) {
	int shiftAmount = 32;
	size_t i = 0;

	while (i < sz) {
		int want = values[sz - 1 - i];
		int got = ferite_loworderindex(hash, shiftAmount);
		is (got, want, "%s: ferite_loworderindex(0x%x,  %d) must return %d",
				desc, hash, shiftAmount, want);
		shiftAmount -= FeriteAMT_SHIFT_AMOUNT;
		i++;
	}
}

void testHighOrderIndex() {
	size_t i;
	for (i = 0; i < sizeof(highOrderIndexTests)/sizeof(highOrderIndexTests[0]); i++) {
		struct TestVector *t = &highOrderIndexTests[i];
		runHighOrderIndexTests(t->desc, t->hash, t->values,
				sizeof(t->values)/sizeof(t->values[0])) ;
	}
}

void testLowOrderIndex() {
	size_t i;
	for (i = 0; i < sizeof(lowOrderIndexTests)/sizeof(lowOrderIndexTests[0]); i++) {
		struct TestVector *t = &lowOrderIndexTests[i];
		runLowOrderIndexTests(t->desc, t->hash, t->values,
				sizeof(t->values)/sizeof(t->values[0])) ;
	}
}

int main() {
	testHighOrderIndex();
	testLowOrderIndex();
	return done_testing();
}
