#include <stdio.h>
#include "bitwise.h"
#define BIT_MASK_32 ((1UL << 32) - 1)


// logical left shift 64 bit using built in C shift
int64_t lsl_64(int64_t operand, int shift_amount)
{
    return operand << shift_amount;
}

// logical right shift 64 bit - cast as unsigned to prevent sign extention
int64_t lsr_64(int64_t operand, int shift_amount)
{
    return ((uint64_t) operand) >> shift_amount;
}

// arithmetic right shift 64 bit using built in C shift
int64_t asr_64(int64_t operand, int shift_amount)
{
    return operand >> shift_amount;
}

/* 
right rotation 64 bit: first the rotated bits are masked, shifted to the other end
of the bit string, and the remaining bits are logically shifted right so the
rotated bits can be added
*/ 
int64_t ror_64(int64_t operand, int shift_amount)
{
    int mask = (1 << shift_amount) - 1;
    int64_t rotated_bits = operand & mask;
    int64_t new_bits = rotated_bits << (64 - shift_amount);
    return (int64_t) (((uint64_t) lsr_64(operand, shift_amount)) + new_bits);
}

// logical left shift 32 bit
int32_t lsl_32(int64_t operand, int shift_amount)
{
    return lsl_64(operand, shift_amount) & BIT_MASK_32;
}

// logical right shift 32 bit
int32_t lsr_32(int64_t operand, int shift_amount)
{
    return ((uint32_t) operand) >> shift_amount;
}

// arithmetic right shift 32 bit
int32_t asr_32(int64_t operand, int shift_amount)
{
    return ((int32_t) operand) >> shift_amount;
}

// right rotation 32 bit
int32_t ror_32(int64_t operand, int shift_amount)
{
    int mask = (1 << shift_amount) - 1;
    int32_t rotated_bits = operand & mask;
    int32_t new_bits = rotated_bits << (32 - shift_amount);
    return (int32_t) (((uint32_t) lsr_32(operand, shift_amount)) + new_bits);
}

