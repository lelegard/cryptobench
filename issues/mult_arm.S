//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Various benchmarks on multiplication instructions.
// Functions profile: void f(uint64_t count, uint64_t op1, uint64_t op2);
//----------------------------------------------------------------------------

        .text

        .align  2
        .globl	mul_empty
mul_empty:
        cbz     x0, L_end_empty
        sub     x0, x0, #1
        b       mul_empty
L_end_empty:
        ret

        .align  2
        .globl	mul_nop_8
mul_nop_8:
        cbz     x0, L_end_nop_8
        sub     x0, x0, #1
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        b       mul_nop_8
L_end_nop_8:
        ret

        .align  2
        .globl	mul_nop_16
mul_nop_16:
        cbz     x0, L_end_nop_16
        sub     x0, x0, #1
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        b       mul_nop_16
L_end_nop_16:
        ret

        .align  2
        .globl	mul_mul_8
mul_mul_8:
        cbz     x0, L_end_mul_8
        sub     x0, x0, #1
        mul     x9, x1, x2
        mul     x10, x1, x2
        mul     x11, x1, x2
        mul     x12, x1, x2
        mul     x13, x1, x2
        mul     x14, x1, x2
        mul     x15, x1, x2
        mul     x9, x1, x2
        b       mul_mul_8
L_end_mul_8:
        ret

// Four mul/umulh instruction pairs.
        .align  2
        .globl	mul_mul_umulh_4
mul_mul_umulh_4:
        cbz     x0, L_end_mul_umulh_4
        sub     x0, x0, #1
        mul     x9, x1, x2
        umulh   x10, x1, x2
        mul     x11, x1, x2
        umulh   x12, x1, x2
        mul     x13, x1, x2
        umulh   x14, x1, x2
        mul     x15, x1, x2
        umulh   x9, x1, x2
        b       mul_mul_umulh_4
L_end_mul_umulh_4:
        ret

        .align  2
        .globl	mul_maua_2
mul_maua_2:
        cbz     x0, L_end_maua_2
        sub     x0, x0, #1
        mul     x9, x1, x2
        adcs    x10, x1, x2
        umulh   x11, x1, x2
        adcs    x12, x1, x2
        mul     x13, x1, x2
        adcs    x14, x1, x2
        umulh   x15, x1, x2
        adcs    x9, x1, x2
        b       mul_maua_2
L_end_maua_2:
        ret
