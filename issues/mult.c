//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Armv8 evaluation of multiplication sequences.
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "cpu_time.h"
#include "mult_arm.h"

// Test program.
int main(int argc, char* argv[])
{
    int64_t time1, time2, time3;

#define TEST(text,func,nbinst,count)                 \
    time1 = cpu_time();                              \
    mul_empty(count);                                \
    time2 = cpu_time();                              \
    func(count, 0x123456789ABCDEF0, 123456789);      \
    time3 = cpu_time();                              \
    printf("%s: %.3f ns/inst\n", (text), (1000.0 * ((time3 - time2) - (time2 - time1))) / ((double)count * nbinst))

    TEST("nop (x8)", mul_nop_8, 8, 1000000000ULL);
    TEST("nop (x16)", mul_nop_16, 16, 500000000ULL);
    TEST("mul (x8)", mul_mul_8, 8, 50000000);
    TEST("mul umulh (x4)", mul_mul_umulh_4, 8, 50000000);
    TEST("mul adcs umulh adcs (x2)", mul_maua_2, 8, 50000000);

    return EXIT_SUCCESS;
}
