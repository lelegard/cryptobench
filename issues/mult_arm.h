//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Various benchmarks on multiplication instructions.
//----------------------------------------------------------------------------

#if !defined(MULT_ARM_H)
#define MULT_ARM_H 1

#include <inttypes.h>

#define DECLARE(func) extern int64_t func(int64_t count) asm(#func);
DECLARE(mul_nop_8);
DECLARE(mul_nop_16);
DECLARE(mul_nop_128);
DECLARE(mul_nop_512);
DECLARE(mul_mul_8);
DECLARE(mul_mul_umulh_4);
DECLARE(mul_maua_2_adcs);
DECLARE(mul_maua_2_add);
#undef DECLARE

#endif
