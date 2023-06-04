// Header file for bitwise instructions.

#include <stdint.h>

extern int64_t lsl_64(int64_t operand, int shift_amount);
extern int64_t lsr_64(int64_t operand, int shift_amount);
extern int64_t asr_64(int64_t operand, int shift_amount);
extern int64_t ror_64(int64_t operand, int shift_amount);
extern int32_t lsl_32(int64_t operand, int shift_amount);
extern int32_t lsr_32(int64_t operand, int shift_amount);
extern int32_t asr_32(int64_t operand, int shift_amount);
extern int32_t ror_32(int64_t operand, int shift_amount);