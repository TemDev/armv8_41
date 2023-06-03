#include <stdio.h>
#include <stdint.h>
#include "test_functions.h"

void printBits( int64_t x, int nbits ) {
	uint64_t mask = 1 << (nbits-1);
	for( int i=0; i<nbits; i++ ) {
		int bit = (x & mask) != 0;
		putchar( '0' + bit );
		mask >>= 1;
	}
	putchar('\n');
}
