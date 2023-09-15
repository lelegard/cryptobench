//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Various benchmarks on multiplication instructions.
//----------------------------------------------------------------------------

        // Declare a function.
        .macro  func symbol
        .text
        .align  2
        .globl  \symbol
\symbol:
        .endm

        // Prolog of all functions in this module.
        // Profile: double func(int64_t iterations);
        // Return: average time of one instruction in iteration body in nanoseconds.
        .macro  xxx_begin symbol
        func    \symbol
        // Stack frame layout.
        .struct 0
xxx.call_frame: .space  16      // call frame (x29, x30)
xxx.iter:       .space  8       // iteration count
xxx.time1:      .space  8       // start / duration of main instruction loop
xxx.time2:      .space  8       // start of empty loop
xxx.reg1920:    .space  16      // saved registers x19-x20
xxx.reg2122:    .space  16      // saved registers x21-x22
                .align  4       // align stack frame on 16 bytes
xxx.stack_size:
        .text
        // Function prolog.
        stp     x29, x30, [sp, -xxx.stack_size]!
        stp     x19, x20, [sp, xxx.reg1920]
        stp     x21, x22, [sp, xxx.reg2122]
        mov     x29, sp
        // Start of function.
        str     x0, [sp, xxx.iter]
        bl      cpu_time
        str     x0, [sp, xxx.time1]
        ldr     x0, [sp, xxx.iter]
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
        // Register usage in main instruction loop:
        // - x3-x15: free to use without breaking the ABI.
        // - x16-x18: temporary registers.
        // - x19-x22: saved registers.
1:      cbz     x0, 5f
        sub     x0, x0, #1
3:
        .endm

        // End of all functions.
        .macro  xxx_end
4:      b       1b                      // Loop back to instruction sequence.
5:      // End of main instruction loop.
        bl      cpu_time
        str     x0, [sp, xxx.time2]     // time2 = start of second loop
        ldr     x1, [sp, xxx.time1]
        sub     x2, x0, x1
        str     x2, [sp, xxx.time1]     // time1 = duration of the instruction loop
        ldr     x0, [sp, xxx.iter]
        // Start of empty instruction loop.
6:      cbz     x0, 7f
        sub     x0, x0, #1
        b       6b
7:      // End of empty instruction loop.
        bl      cpu_time
        ldr     x1, [sp, xxx.time2]
        sub     x2, x0, x1              // x2 = duration of empty loop
        ldr     x1, [sp, xxx.time1]
        sub     x0, x1, x2              // x0 = instructions duration (main loop - empty loop)
        mov     x1, #1000
        mul     x2, x0, x1              // x2 = instructions duration in nanoseconds
        mov     x1, #(4b - 3b) / 4      // x1 = number in instructions in main loop
        ldr     x0, [sp, xxx.iter]
        mul     x3, x1, x0              // x3 = total number of executed instructions
        ucvtf   d2, x2
        ucvtf   d1, x3
        fdiv    d0, d2, d1              // d0 (result) = average duration of one instruction in nanosecond
        // Function epilog
        ldp     x19, x20, [sp, xxx.reg1920]
        ldp     x21, x22, [sp, xxx.reg2122]
        ldp     x29, x30, [sp], xxx.stack_size
        ret
        .endm

//----------------------------------------------------------------------------

        // NOP, use 512 to avoid looping in a cache line
        xxx_begin xxx_nop
        .rept   512
        nop
        .endr
        xxx_end

        // MUL, 8 instructions, independent output registers.
        xxx_begin xxx_mul
        .irp    reg,3,4,5,6,7,8,9,10
        mul     x\reg, x1, x2
        .endr
        xxx_end

        // MUL UMULH, 8 instructions, independent output registers.
        xxx_begin xxx_mul_umulh
        mul     x3, x1, x2
        umulh   x4, x1, x2
        mul     x5, x1, x2
        umulh   x6, x1, x2
        mul     x7, x1, x2
        umulh   x8, x1, x2
        mul     x9, x1, x2
        umulh   x10, x1, x2
        xxx_end

        // MUL ADD UMULH ADD, 8 instructions, independent output registers.
        xxx_begin xxx_mul_add_umulh_add
        mul     x3, x1, x2
        add     x4, x1, x2
        umulh   x5, x1, x2
        add     x6, x1, x2
        mul     x7, x1, x2
        add     x8, x1, x2
        umulh   x9, x1, x2
        add     x10, x1, x2
        xxx_end

        // MUL ADCS UMULH ADCS, 8 instructions, independent output registers.
        xxx_begin xxx_mul_adcs_umulh_adcs
        mul     x3, x1, x2
        adcs    x4, x1, x2
        umulh   x5, x1, x2
        adcs    x6, x1, x2
        mul     x7, x1, x2
        adcs    x8, x1, x2
        umulh   x9, x1, x2
        adcs    x10, x1, x2
        xxx_end

        // OpenSSL sequence for Montgomery multiplication.
        xxx_begin xxx_montgo_seq_adcs
        adds    x3,x3,x11               
        mul     x11,x12,x13
        adcs    x4,x4,x14
        mul     x14,x15,x13
        adcs    x5,x5,x20
        mul     x20,x19,x13
        adcs    x8,x8,x21
        umulh   x21,x16,x13             
        adcs    x9,x9,x11
        umulh   x11,x17,x13
        adcs    x6,x6,x14
        umulh   x14,x18,x13
        adcs    x7,x7,x20
        umulh   x20,x22,x13
        adc     x10,xzr,xzr             
        adds    x4,x4,x21               
        umulh   x21,x12,x13
        adcs    x5,x5,x11
        umulh   x11,x15,x13
        adcs    x8,x8,x14
        umulh   x14,x19,x13
        adcs    x9,x9,x20
        mul     x20,x17,x16
        adcs    x6,x6,x21
        mul     x21,x18,x16
        adcs    x7,x7,x11
        mul     x11,x22,x16
        adc     x10,x10,x14
        xxx_end

        // OpenSSL sequence for Montgomery multiplication with ADD instead of AD[C][S].
        xxx_begin xxx_montgo_seq_add
        add     x3,x3,x11               
        mul     x11,x12,x13
        add     x4,x4,x14
        mul     x14,x15,x13
        add     x5,x5,x20
        mul     x20,x19,x13
        add     x8,x8,x21
        umulh   x21,x16,x13             
        add     x9,x9,x11
        umulh   x11,x17,x13
        add     x6,x6,x14
        umulh   x14,x18,x13
        add     x7,x7,x20
        umulh   x20,x22,x13
        add     x10,xzr,xzr             
        add     x4,x4,x21               
        umulh   x21,x12,x13
        add     x5,x5,x11
        umulh   x11,x15,x13
        add     x8,x8,x14
        umulh   x14,x19,x13
        add     x9,x9,x20
        mul     x20,x17,x16
        add     x6,x6,x21
        mul     x21,x18,x16
        add     x7,x7,x11
        mul     x11,x22,x16
        add     x10,x10,x14
        xxx_end
