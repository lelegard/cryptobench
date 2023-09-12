//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Get current CPU time resource usage in microseconds.
//----------------------------------------------------------------------------

#include "cpu_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

int64_t cpu_time()
{
    struct rusage ru;
    if (getrusage(RUSAGE_SELF, &ru) < 0) {
        perror("getrusage");
        exit(EXIT_FAILURE);
    }
    return ((int64_t)(ru.ru_utime.tv_sec) * USECPERSEC) + ru.ru_utime.tv_usec +
           ((int64_t)(ru.ru_stime.tv_sec) * USECPERSEC) + ru.ru_stime.tv_usec;
}
