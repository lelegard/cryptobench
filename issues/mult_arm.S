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

        // Declare the start of a structure (typically a stack frame).
        .macro  struct_begin
        .struct_offset = 0
        .endm

        // Declare a field in the current structure. Size in bytes.
        .macro  struct_field name, size
        \name = .struct_offset
        .struct_offset = .struct_offset + \size
        .endm

        // Realign inside current structure. Alignment in bytes.
        .macro  struct_align align
        .struct_offset = .struct_offset + (\align - (.struct_offset % \align)) % \align
        .endm

        // End of current structure. Define alignment in bytes and name for structure size.
        .macro  struct_end size_name, align=16
        struct_align \align
        \size_name = .struct_offset
        .endm

        // Prolog of all functions in this module.
        // Profile: double func(int64_t iterations);
        // Return: average time of one instruction in iteration body in nanoseconds.
        .macro  xxx_begin symbol
        func    \symbol
        // Stack frame layout.
        struct_begin
        struct_field xxx.call_frame, 16 // call frame (x29, x30)
        struct_field xxx.reg1920, 16    // saved registers x19-x20
        struct_field xxx.reg2122, 16    // saved registers x21-x22
        struct_field xxx.iter, 8        // iteration count
        struct_field xxx.time1, 8       // start / duration of main instruction loop
        struct_field xxx.time2, 8       // start of empty loop
        struct_end   xxx.stack_size
        // Function prolog.
        stp     x29, x30, [sp, -xxx.stack_size]!
        stp     x19, x20, [sp, xxx.reg1920]
        stp     x21, x22, [sp, xxx.reg2122]
        mov     x29, sp
        // Start of function.
        str     x0, [sp, xxx.iter]
        mov     x1, #0xCDEF             // x1 = 0x0123456789ABCDEF
        movk    x1, #0x89AB, lsl 16
        movk    x1, #0x4567, lsl 32
        movk    x1, #0x0123, lsl 48
        mov     x2, #0x3210             // x2 = 0FEDCBA9876543210
        movk    x2, #0x7654, lsl 16
        movk    x2, #0xBA98, lsl 32
        movk    x2, #0xFEDC, lsl 48
        isb                             // instruction barrier before getting timer counter
        mrs     x1, cntvct_el0          // virtual timer counter
        str     x1, [sp, xxx.time1]
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
        isb
        mrs     x0, cntvct_el0
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
        isb
        mrs     x0, cntvct_el0
        ldr     x1, [sp, xxx.time2]
        sub     x2, x0, x1              // x2 = duration of empty loop
        ldr     x1, [sp, xxx.time1]     // x1 = duration of the instruction loop
        sub     x0, x1, x2              // x0 = instructions duration, in virtual timer units
        ucvtf   d0, x0
        mov     x1, #0xCA00             // x1 = 0x3B9ACA00 = 1 billion = nano-sec / sec
        movk    x1, #0x3B9A, lsl 16
        ucvtf   d1, x1
        fmul    d2, d0, d1              // d2 = instructions duration * 1 billion
        mrs     x1, cntfrq_el0          // x1 = frequency in Hz of virtual timer
        ucvtf   d1, x1
        fdiv    d3, d2, d1              // d3 = instructions duration in nanoseconds
        mov     x1, #(4b - 3b) / 4      // x1 = number in instructions in main loop
        ldr     x0, [sp, xxx.iter]
        mul     x2, x1, x0              // x2 = total number of executed instructions
        ucvtf   d2, x2
        fdiv    d0, d3, d2              // d0 (result) = average duration of one instruction in nanoseconds
        // Function epilog
        ldp     x19, x20, [sp, xxx.reg1920]
        ldp     x21, x22, [sp, xxx.reg2122]
        ldp     x29, x30, [sp], xxx.stack_size
        ret
        .endm

//----------------------------------------------------------------------------

        // To keep a fair comparison between all sequences, try to keep all
        // loops to the same number of instructions. Otherwise, distinct usages
        // of cache may introduce some bias.

        // NOP
        xxx_begin xxx_nop
        .rept   8
        nop
        .endr
        xxx_end

        // MUL, independent output registers.
        xxx_begin xxx_mul
        .irp    reg,3,4,5,6,7,8,9,10
        mul     x\reg, x1, x2
        .endr
        xxx_end

        // ADD, independent output registers.
        xxx_begin xxx_add
        .irp    reg,3,4,5,6,7,8,9,10
        add     x\reg, x1, x2
        .endr
        xxx_end

        // ADCS, independent output registers.
        xxx_begin xxx_adcs
        .irp    reg,3,4,5,6,7,8,9,10
        adcs    x\reg, x1, x2
        .endr
        xxx_end

        // ADDS, independent output registers.
        xxx_begin xxx_adds
        .irp    reg,3,4,5,6,7,8,9,10
        adds    x\reg, x1, x2
        .endr
        xxx_end

        // ADC, independent output registers.
        xxx_begin xxx_adc
        .irp    reg,3,4,5,6,7,8,9,10
        adc     x\reg, x1, x2
        .endr
        xxx_end

        // MUL UMULH, independent output registers.
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

        // MUL ADD UMULH ADD, independent output registers.
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

        // MUL ADCS MUL ADCS, independent output registers.
        xxx_begin xxx_mul_adcs
        mul     x3, x1, x2
        adcs    x4, x1, x2
        mul     x5, x1, x2
        adcs    x6, x1, x2
        mul     x7, x1, x2
        adcs    x8, x1, x2
        mul     x9, x1, x2
        adcs    x10, x1, x2
        xxx_end

        // MUL ADCS UMULH ADCS, independent output registers.
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
        // This sequence has more than 8 instructions but we will
        // keep the same number of instructions in the next one.
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

        // PAC instructions.
        xxx_begin xxx_pacia
        pacia   x3, sp
        pacia   x4, sp
        pacia   x5, sp
        pacia   x6, sp
        pacia   x7, sp
        pacia   x8, sp
        pacia   x9, sp
        pacia   x10, sp
        xxx_end

        // AUT instructions.
        xxx_begin xxx_autia
        autia   x3, sp
        autia   x4, sp
        autia   x5, sp
        autia   x6, sp
        autia   x7, sp
        autia   x8, sp
        autia   x9, sp
        autia   x10, sp
        xxx_end

        // PAC/AUT instructions.
        xxx_begin xxx_pacia_autia
        pacia   x3, sp
        autia   x3, sp
        pacia   x5, sp
        autia   x5, sp
        pacia   x7, sp
        autia   x7, sp
        pacia   x9, sp
        autia   x9, sp
        xxx_end
