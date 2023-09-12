//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Various benchmarks on multiplication instructions.
//----------------------------------------------------------------------------

#if !defined(MULT_ARM_H)
#define MULT_ARM_H 1

#include <inttypes.h>

void mul_empty(uint64_t count);
void mul_nop_8(uint64_t count, uint64_t op1, uint64_t op2);
void mul_nop_16(uint64_t count, uint64_t op1, uint64_t op2);
void mul_mul_8(uint64_t count, uint64_t op1, uint64_t op2);
void mul_mul_umulh_4(uint64_t count, uint64_t op1, uint64_t op2);
void mul_maua_2(uint64_t count, uint64_t op1, uint64_t op2);

#endif
