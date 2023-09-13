//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Various benchmarks on multiplication instructions.
//----------------------------------------------------------------------------

        // Declare a function.
        .macro  func symbol
        .text
        .align  2
        .globl	\symbol
\symbol:
        .endm

        // Prolog of all functions in this module.
        // Profile: int64_t func(int64_t iterations);
        // Return: total duration of all iteration bodies in microseconds.
        .macro  mul_begin symbol
        func    \symbol
        // Stack frame layout.
        .struct 0
mul.call_frame: .space  16      // call frame (x29, x30)
mul.iter:       .space  8       // iteration count
mul.time1:      .space  8       // start / duration of main instruction loop
mul.time2:      .space  8       // start of empty loop
                .align  4       // align stack frame on 16 bytes
mul.stack_size:
        .text
        // Function prolog.
        stp     x29, x30, [sp, -mul.stack_size]!
        mov     x29, sp
        // Start of function.
        str     x0, [sp, mul.iter]
        bl      cpu_time
        str     x0, [sp, mul.time1]
        ldr     x0, [sp, mul.iter]
        mov     x1, #0xCDEF             // x1 = 0x0123456789ABCDEF
        movk    x1, #0x89AB, lsl 16
        movk    x1, #0x4567, lsl 32
        movk    x1, #0x0123, lsl 48
        mov     x2, #0x3210             // x2 = 0FEDCBA9876543210
        movk    x2, #0x7654, lsl 16
        movk    x2, #0xBA98, lsl 32
        movk    x2, #0xFEDC, lsl 48
        // Start of main instruction loop.
        // Iteration count in x0, generic operands in x1 and x2.
1:      cbz     x0, 5f
        sub     x0, x0, #1
        .endm

        // End of all functions.
        .macro  mul_end
        b       1b                      // Loop back to instruction sequence.
5:      // End of main instruction loop.
        bl      cpu_time
        str     x0, [sp, mul.time2]     // time2 = start of second loop
        ldr     x1, [sp, mul.time1]
        sub     x2, x0, x1
        str     x2, [sp, mul.time1]     // time1 = duration of the instruction loop
        ldr     x0, [sp, mul.iter]
        // Start of empty instruction loop.
6:      cbz     x0, 7f
        sub     x0, x0, #1
        b       6b
7:      // End of empty instruction loop.
        bl      cpu_time
        ldr     x1, [sp, mul.time2]
        sub     x2, x0, x1              // x2 = duration of empty loop
        ldr     x1, [sp, mul.time1]
        sub     x0, x1, x2              // x0 (result) = duration of main loop - duration of empty loop
        // Function epilog
        ldp     x29, x30, [sp], mul.stack_size
        ret
        .endm

//----------------------------------------------------------------------------

        mul_begin mul_nop_8
        .rept   8
        nop
        .endr
        mul_end

        mul_begin mul_nop_16
        .rept   16
        nop
        .endr
        mul_end

        mul_begin mul_nop_128
        .rept   128
        nop
        .endr
        mul_end

        mul_begin mul_nop_512
        .rept   512
        nop
        .endr
        mul_end

        mul_begin mul_mul_8
        .irp    reg,9,10,11,12,13,14,15,9
        mul     x\reg, x1, x2
        .endr
        mul_end

        mul_begin mul_mul_umulh_4
        mul     x9, x1, x2
        umulh   x10, x1, x2
        mul     x11, x1, x2
        umulh   x12, x1, x2
        mul     x13, x1, x2
        umulh   x14, x1, x2
        mul     x15, x1, x2
        umulh   x9, x1, x2
        mul_end

        mul_begin mul_maua_2_adcs
        mul     x9, x1, x2
        adcs    x10, x1, x2
        umulh   x11, x1, x2
        adcs    x12, x1, x2
        mul     x13, x1, x2
        adcs    x14, x1, x2
        umulh   x15, x1, x2
        adcs    x9, x1, x2
        mul_end

        mul_begin mul_maua_2_add
        mul     x9, x1, x2
        add     x10, x1, x2
        umulh   x11, x1, x2
        add     x12, x1, x2
        mul     x13, x1, x2
        add     x14, x1, x2
        umulh   x15, x1, x2
        add     x9, x1, x2
        mul_end
