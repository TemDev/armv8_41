// All the binary shifts are defined here

#include <stdio.h>
#include "bitwise.h"
#define BIT_MASK_32 ((1UL << 32) - 1)

// Function performs logical shift left with 64 bit mode
int64_t lsl_64(int64_t operand, int shift_amount)
{
    return operand << shift_amount;
}

// Function performs logical shift right with 64 bit mode
int64_t lsr_64(int64_t operand, int shift_amount)
{
    return ((uint64_t) operand) >> shift_amount;
}

// Function performs arithmetic shift right with 64 bit mode
int64_t asr_64(int64_t operand, int shift_amount)
{
    return operand >> shift_amount;
}

// Function rotate right shift with 64 bit mode
int64_t ror_64(int64_t operand, int shift_amount)
{
    long mask = (((long) 1) << shift_amount) - (long) 1;
    int64_t rotated_bits = operand & mask;
    int64_t new_bits = rotated_bits << (64 - shift_amount);
    return (int64_t) (((uint64_t) lsr_64(operand, shift_amount)) + new_bits);
}

// Function performs logical shift left with 32 bit mode
int32_t lsl_32(int64_t operand, int shift_amount)
{
    return lsl_64(operand, shift_amount) & BIT_MASK_32;
}

// Function performs logical shift right with 32 bit mode
int32_t lsr_32(int64_t operand, int shift_amount)
{
    return ((uint32_t) operand) >> shift_amount;
}

// Function performs arithmetic shift right with 32 bit mode
int32_t asr_32(int64_t operand, int shift_amount)
{
    return ((int32_t) operand) >> shift_amount;
}

// Function rotate right shift with 32 bit mode
int32_t ror_32(int64_t operand, int shift_amount)
{
  int mask = (((int) 1) << shift_amount) - (int) 1;
    int32_t rotated_bits = operand & mask;
    int32_t new_bits = rotated_bits << (32 - shift_amount);
    return (int32_t) (((uint32_t) lsr_32(operand, shift_amount)) + new_bits);
}
