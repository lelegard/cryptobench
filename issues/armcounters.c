//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Armv8 counters demo.
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <locale.h>
#include "cpu_time.h"
#include "cpu_affinity.h"

int main(int argc, char* argv[])
{
    lock_cpu_affinity();
    
    // Make sure printf knows how to format integers.
    setlocale(LC_ALL, "en_US.UTF-8");

    // Get counter frequency (not the CPU frequency).
    uint64_t cntfrq = 0;
    asm("mrs %0, cntfrq_el0" : "=r" (cntfrq));
    printf("CNTFRQ_EL0: %'" PRId64 "\n", cntfrq);

    int64_t time1 = cpu_time();
    int64_t counter1 = 0;
    asm("mrs %0, cntpct_el0" : "=r" (counter1));
    printf("CNTPCT_EL0: %'" PRId64 "\n", counter1);

    uint64_t dummy = 0xDEADBEEF;
    for (int64_t i = 2000000000; i > 0; i--) {
        dummy = (3 * dummy) ^ dummy;
    }
    printf("dummy: 0x%016" PRIu64 "\n", dummy);

    int64_t time2 = cpu_time();
    int64_t counter2 = 0;
    asm("mrs %0, cntpct_el0" : "=r" (counter2));

    printf("cpu_time: %'" PRId64 "\n", time2 - time1);
    printf("counters: %'" PRId64 "\n", counter2 - counter1);
    printf("CNTPCT_EL0: %'" PRId64 "\n", counter2);

    return EXIT_SUCCESS;
}
