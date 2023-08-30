//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// "Reference" benchmark to establish a base indicator for the system.
//----------------------------------------------------------------------------

#include "bench_reference.h"

//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

bench_reference::bench_reference(int64_t min_usec, int64_t min_iterations) :
    bench("reference", min_usec, min_iterations),
    _data(1024)
{
}

//----------------------------------------------------------------------------
// One iteration: lots of modular arithmetics and bitwise operations.
//----------------------------------------------------------------------------

void bench_reference::one_iteration()
{
    // Initialize the buffer with increasing values.
    for (size_t i = 0; i < _data.size(); i++) {
        _data[i] = 3 + 7 * i;
    }

    for (size_t loops = 0; loops < 2048; loops++) {
        // Additions.
        for (size_t i = 0; i < _data.size() - 1; i++) {
            _data[i] += _data[i+1];
        }
        // Multiplications.
        for (size_t i = 0; i < _data.size() - 1; i++) {
            _data[i] *= _data[i+1];
        }
        // Substractions.
        for (size_t i = 0; i < _data.size() - 1; i++) {
            _data[i] -= _data[i+1];
        }
        // Divisions.
        for (size_t i = 0; i < _data.size() - 1; i++) {
            if (_data[i+1] != 0) {
                _data[i] -= _data[i+1];
            }
        }
        // Shifts and masks.
        for (size_t i = 1; i < _data.size() - 1; i++) {
            _data[i] = (_data[i-1] << 3) ^ (_data[i+1] >> 2);
        }
    }
}
