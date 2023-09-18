//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Lock the current thread on one CPU core, whichever it is, but just one.
//----------------------------------------------------------------------------

#if !defined(CPU_AFFINITY_H)
#define CPU_AFFINITY_H 1

void lock_cpu_affinity();

#endif
