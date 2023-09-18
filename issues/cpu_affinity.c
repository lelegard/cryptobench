//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Lock the current thread on one CPU core.
//----------------------------------------------------------------------------

#include "cpu_affinity.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if defined(__linux__)
    #define _GNU_SOURCE
    #include <sched.h>
#endif

// Number of active CPU cores in the systems.
int cpu_count()
{
    const long nproc = sysconf(_SC_NPROCESSORS_ONLN);
    if (nproc < 0) {
        perror("sysconf(_SC_NPROCESSORS_ONLN)");
        exit(EXIT_FAILURE);
    }
    return (int)nproc;
}

// Lock the current thread on one CPU core, whichever it is, but just one.
void lock_cpu_affinity()
{
#if defined(__linux__)

    // Lock on CPU #1, if it exists. Still better than CPU #0 if this one is used by system tasks or interrupts.
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu_count() > 1 ? 1 : 0, &set);
    if (sched_setaffinity(0, sizeof(set), &set) < 0) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }

#endif
}
