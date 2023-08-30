//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// "Reference" benchmark to establish a base indicator for the system.
// Mostly integer operations since crypto does not use floating points.
//----------------------------------------------------------------------------

#pragma once
#include "bench.h"

class bench_reference: public bench
{
public:
    // Constructor.
    bench_reference(int64_t min_usec, int64_t min_iterations);

protected:
    // One iteration.
    virtual void one_iteration() override;

private:
    std::vector<uint64_t> _data;
};
