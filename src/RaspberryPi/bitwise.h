// Header file for bitwise instructions. All the binary shifts are declared here

#include <stdint.h>

// Function performs logical shift left with 64 bit mode
extern int64_t lsl_64(int64_t operand, int shift_amount);

// Function performs logical shift right with 64 bit mode
extern int64_t lsr_64(int64_t operand, int shift_amount);

// Function performs arithmetic shift right with 64 bit mode
extern int64_t asr_64(int64_t operand, int shift_amount);

// Function rotate right shift with 64 bit mode
extern int64_t ror_64(int64_t operand, int shift_amount);

// Function performs logical shift left with 32 bit mode
extern int32_t lsl_32(int64_t operand, int shift_amount);

// Function performs logical shift right with 32 bit mode
extern int32_t lsr_32(int64_t operand, int shift_amount);

// Function performs arithmetic shift right with 32 bit mode
extern int32_t asr_32(int64_t operand, int shift_amount);

// Function rotate right shift with 32 bit mode
extern int32_t ror_32(int64_t operand, int shift_amount);
