//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Armv8 evaluation of multiplication sequences.
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#if defined(__linux__)
    #include <sys/auxv.h>
#endif

#define DECLARE(func) extern double func(int64_t count) asm(#func);

DECLARE(xxx_nop);
DECLARE(xxx_add);
DECLARE(xxx_adc);
DECLARE(xxx_adds);
DECLARE(xxx_adcs);
DECLARE(xxx_mul);
DECLARE(xxx_mul_umulh);
DECLARE(xxx_mul_add_umulh_add);
DECLARE(xxx_mul_adcs);
DECLARE(xxx_mul_adcs_umulh_adcs);
DECLARE(xxx_montgo_seq_add);
DECLARE(xxx_montgo_seq_adcs);
DECLARE(xxx_pacia);
DECLARE(xxx_autia);
DECLARE(xxx_pacia_autia);

#define REFCOUNT 200000000 // 200 million iterations (adapted to sequence)

// Check if PAC is supported on the current CPU.
int has_pac()
{
#if defined(__APPLE__)
    return 1; // supported on all Apple Silicon chips
#else
    return (getauxval(AT_HWCAP) & HWCAP_PACA) != 0;
#endif
}

// Application entry point.
int main(int argc, char* argv[])
{
    printf("nop:                 %.3f ns/inst\n", xxx_nop(REFCOUNT * 4));
    printf("add:                 %.3f ns/inst\n", xxx_add(REFCOUNT));
    printf("adc:                 %.3f ns/inst\n", xxx_adc(REFCOUNT));
    printf("adds:                %.3f ns/inst\n", xxx_adds(REFCOUNT));
    printf("adcs:                %.3f ns/inst\n", xxx_adcs(REFCOUNT));
    printf("mul:                 %.3f ns/inst\n", xxx_mul(REFCOUNT));
    printf("mul umulh:           %.3f ns/inst\n", xxx_mul_umulh(REFCOUNT));
    printf("mul adcs umulh adcs: %.3f ns/inst\n", xxx_mul_adcs_umulh_adcs(REFCOUNT));
    printf("mul adcs:            %.3f ns/inst\n", xxx_mul_adcs(REFCOUNT));
    printf("mul add umulh add:   %.3f ns/inst\n", xxx_mul_add_umulh_add(REFCOUNT));
    printf("ossl seq with adcs:  %.3f ns/inst\n", xxx_montgo_seq_adcs(REFCOUNT / 2));
    printf("ossl seq with add:   %.3f ns/inst\n", xxx_montgo_seq_add(REFCOUNT / 2));
    if (has_pac()) {
        printf("pacia:               %.3f ns/inst\n", xxx_pacia(REFCOUNT));
        printf("autia:               %.3f ns/inst\n", xxx_autia(REFCOUNT));
        printf("pacia autia:         %.3f ns/inst\n", xxx_pacia_autia(REFCOUNT));
    }
    return EXIT_SUCCESS;
}
