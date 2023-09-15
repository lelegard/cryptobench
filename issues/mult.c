//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Armv8 evaluation of multiplication sequences.
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#define DECLARE(func) extern double func(int64_t count) asm(#func);

DECLARE(xxx_nop);
DECLARE(xxx_mul);
DECLARE(xxx_mul_umulh);
DECLARE(xxx_mul_add_umulh_add);
DECLARE(xxx_mul_adcs_umulh_adcs);
DECLARE(xxx_montgo_seq_add);
DECLARE(xxx_montgo_seq_adcs);

int main(int argc, char* argv[])
{
    printf("nop:                 %.3f ns/inst\n", xxx_nop(40000000));
    printf("mul:                 %.3f ns/inst\n", xxx_mul(100000000));
    printf("mul umulh:           %.3f ns/inst\n", xxx_mul_umulh(100000000));
    printf("mul add umulh add:   %.3f ns/inst\n", xxx_mul_add_umulh_add(100000000));
    printf("mul adcs umulh adcs: %.3f ns/inst\n", xxx_mul_adcs_umulh_adcs(100000000));
    printf("ossl seq with add:   %.3f ns/inst\n", xxx_montgo_seq_add(100000000));
    printf("ossl seq with adcs:  %.3f ns/inst\n", xxx_montgo_seq_adcs(100000000));

    return EXIT_SUCCESS;
}
