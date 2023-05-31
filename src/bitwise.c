#include <stdio.h>
#define BIT_MASK_32 ((1UL << 32) - 1)

int lsl_64(int operand, int shift_amount) 
{ 
    return operand << shift_amount;
}

int lsr_64(int operand, int shift_amount) 
{ 
    return (signed) (((unsigned) operand) >> shift_amount);  // only if unsigned
}

int asr_64(int operand, int shift_amount) 
{ 
    return operand >> shift_amount;  // only if signed
}

int ror_64(int operand, int shift_amount) 
{ 
    int mask = (1 << shift_amount) - 1;
    int rotated_bits = operand & mask;
    int new_bits = rotated_bits << (64 - shift_amount);
    return (signed) (((unsigned) lsl_64(operand, shift_amount)) + new_bits);
}

int lsl_32(int operand, int shift_amount)
{
    return lsl_64(operand & BIT_MASK_32, shift_amount);
}

int lsr_32(int operand, int shift_amount)
{
    return lsr_64(operand & BIT_MASK_32, shift_amount);
}

int asr_32(int operand, int shift_amount)
{
    return asr_64(operand & BIT_MASK_32, shift_amount);
}

int ror_32(int operand, int shift_amount)
{
    return ror_64(operand & BIT_MASK_32, shift_amount);
}

int main(void) {
	long testvals[] = {21, -21, 51294967296, -5194967296};
	for(int i = 0; i < 4; i++) {
		int x = testvals[i];
		printf("logical LS of %d is %d\n", x, lsl_64(x, 1));
		printf("using 32 bit mode, it is %d\n\n", lsl_32(x, 1));
		printf("arithmetic RS of %d is %d\n", x, asr_64(x, 1));
		printf("using 32 bit mode, it is  %d\n\n", asr_32(x, 1));
		printf("logical RS of %d is %d\n", x, lsl_64(x, 1));
		printf("using 32 bit mode, it is  %d\n\n", lsl_32(x, 1));
		printf("rotation R of %d is %d\n", x, ror_64(x, 1));
		printf("using 32 bit mode, it is  %d\n\n-----------------------------------\n\n", ror_32(x, 1));
	}
}
