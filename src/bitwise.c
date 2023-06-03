#include <stdio.h>
#include "bitwise.h"
// #include "test_functions.h"
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
    return (int64_t) (((uint64_t) lsr_64(operand, shift_amount)) + new_bits);
}

int32_t lsl_32(int64_t operand, int shift_amount)
{
    return lsl_64(operand, shift_amount) & BIT_MASK_32;
}

int32_t lsr_32(int64_t operand, int shift_amount)
{
    return ((uint32_t) operand) >> shift_amount;
}

int32_t asr_32(int64_t operand, int shift_amount)
{
    return ((int32_t) operand) >> shift_amount;
}

int32_t ror_32(int64_t operand, int shift_amount)
{
    int mask = (1 << shift_amount) - 1;
    int32_t rotated_bits = operand & mask;
    int32_t new_bits = rotated_bits << (32 - shift_amount);
    return (int32_t) (((uint32_t) lsr_32(operand, shift_amount)) + new_bits);
}

// int main(void) {
//    int64_t testvals[] = {21, -21, 51294967296, -5194967296};
//    char *function_names[] = {"logical LS", "arithmetic RS", "logical RS", "rotation"};
//    typedef int64_t (*func_pointer_64)(int64_t, int);
//    func_pointer_64 shift_functions_64[] = {&lsl_64, &asr_64, &lsr_64, &ror_64};
//    typedef int32_t (*func_pointer_32)(int64_t, int);
//    func_pointer_32 shift_functions_32[] = {&lsl_32, &asr_32, &lsr_32, &ror_32};
//	for(int i = 0; i < 4; i++) {
//        	int64_t x = testvals[i];
//        	for(int j = 0; j < 4; j++) {
//            		printf("%s of %ld is %ld\n", function_names[j], x, shift_functions_64[j](x, 1));
//            		printBits(x, 64);
//            		printBits(shift_functions_64[j](x, 1), 64);
//            		printf("\nusing 32 bit mode, it is %d\n", shift_functions_32[j](x, 1));
//            		printBits(x, 32);
//           		printBits(shift_functions_32[j](x, 1), 32);
//            		printf("\n");
//        	}
//        	printf("-----------------------------------------------\n");
//	}
	return 0;
}
