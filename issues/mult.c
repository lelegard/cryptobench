//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Armv8 evaluation of multiplication sequences.
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "mult_arm.h"

#define TEST(text,func,nbinst,count) \
    printf("%s: %.3f ns/inst\n", (text), (1000.0 * (double)func(count)) / ((double)count * (double)nbinst))

// Test program.
int main(int argc, char* argv[])
{
    TEST("nop (x8)",                 mul_nop_8,        8, 1000000000);
    TEST("nop (x16)",                mul_nop_16,      16,  500000000);
    TEST("nop (x128)",               mul_nop_128,    128,   40000000);
    TEST("nop (x512)",               mul_nop_512,    512,   40000000);
    TEST("mul (x8)",                 mul_mul_8,        8,  100000000);
    TEST("mul umulh (x4)",           mul_mul_umulh_4,  8,  100000000);
    TEST("mul adcs umulh adcs (x2)", mul_maua_2_adcs,  8,  100000000);
    TEST("mul add umulh add (x2)",   mul_maua_2_add,   8,  100000000);

    return EXIT_SUCCESS;
}
