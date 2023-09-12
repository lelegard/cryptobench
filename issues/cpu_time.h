//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Get current CPU time resource usage in microseconds.
//----------------------------------------------------------------------------

#if !defined(CPU_TIME_H)
#define CPU_TIME_H 1

#include <inttypes.h>

#define USECPERSEC 1000000 // microseconds per second

int64_t cpu_time();

#endif
