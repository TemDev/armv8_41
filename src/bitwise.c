#include <stdio.h>
#include <stdint.h>
#define BIT_MASK_32 ((1UL << 32) - 1)

int64_t lsl_64(int64_t operand, int shift_amount)
{ 
    return operand << shift_amount;
}

int64_t lsr_64(int64_t operand, int shift_amount)
{
    return ((uint64_t) operand) >> shift_amount;
}

int64_t asr_64(int64_t operand, int shift_amount)
{ 
    return operand >> shift_amount;
}

int64_t ror_64(int64_t operand, int shift_amount)
{ 
    int mask = (1 << shift_amount) - 1;
    int64_t rotated_bits = operand & mask;
    int64_t new_bits = rotated_bits << (64 - shift_amount);
    return (int64_t) (((uint64_t) lsl_64(operand, shift_amount)) + new_bits);
}

int32_t lsl_32(int64_t operand, int shift_amount)
{
    return lsl_64(operand, shift_amount) & BIT_MASK_32;
}

int32_t lsr_32(int64_t operand, int shift_amount)
{
//    return lsr_64(operand & BIT_MASK_32, shift_amount);
    return ((uint32_t) operand) >> shift_amount;
}

int32_t asr_32(int64_t operand, int shift_amount)
{
//    return asr_64(operand, shift_amount) & BIT_MASK_32;
    return ((int32_t) operand) >> shift_amount;
}

int32_t ror_32(int64_t operand, int shift_amount)
{
    int mask = (1 << shift_amount) - 1;
    int32_t rotated_bits = operand & mask;
    int32_t new_bits = rotated_bits << (32 - shift_amount);
    return (int32_t) (((uint32_t) lsl_32(operand, shift_amount)) + new_bits);
}

int main(void) {
    int64_t testvals[] = {21, -21, 51294967296, -5194967296};
	for(int i = 0; i < 4; i++) {
        int64_t x = testvals[i];
		printf("logical LS of %ld is %ld\n", x, lsl_64(x, 1));
		printf("using 32 bit mode, it is %d\n\n", lsl_32(x, 1));
		printf("arithmetic RS of %ld is %ld\n", x, asr_64(x, 1));
		printf("using 32 bit mode, it is %d\n\n", asr_32(x, 1));
		printf("logical RS of %ld is %ld\n", x, lsr_64(x, 1));
		printf("using 32 bit mode, it is %d\n\n", lsr_32(x, 1));
		printf("rotation R of %ld is %ld\n", x, ror_64(x, 1));
		printf("using 32 bit mode, it is %d\n\n-----------------------------------\n\n", ror_32(x, 1));
	}
}
