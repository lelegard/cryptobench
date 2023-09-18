//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Armv8 counters demo.
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <locale.h>
#include "cpu_affinity.h"

int main(int argc, char* argv[])
{
    lock_cpu_affinity();
    
    // Make sure printf knows how to format integers.
    setlocale(LC_ALL, "en_US.UTF-8");

    uint64_t cntfrq = 0;
    asm("mrs %0, cntfrq_el0" : "=r" (cntfrq));
    printf("CNTFRQ_EL0: %'" PRId64 "\n", cntfrq);

    return EXIT_SUCCESS;
}
