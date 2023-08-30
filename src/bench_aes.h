//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// AES benchmarks.
//----------------------------------------------------------------------------

#pragma once
#include "bench.h"
#include "lib.h"

//----------------------------------------------------------------------------
// AES encrypt with key scheduling benchmark.
// Key scheduling is performed with each iteration (one CBC encrypt).
//----------------------------------------------------------------------------

class bench_aes_encrypt: public bench
{
public:
    // Constructor.
    bench_aes_encrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, size_t key_bits, size_t data_size);

protected:
    // One iteration.
    virtual void one_iteration() override;

private:
    lib&    _crypto;
    bytes_t _key;
    bytes_t _iv;
    bytes_t _input;
    bytes_t _output;
};

//----------------------------------------------------------------------------
// AES decrypt with key scheduling benchmark.
// Key scheduling is performed with each iteration (one CBC decrypt).
//----------------------------------------------------------------------------

class bench_aes_decrypt: public bench
{
public:
    // Constructor.
    bench_aes_decrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, size_t key_bits, size_t data_size);

protected:
    // One iteration.
    virtual void one_iteration() override;

private:
    lib&    _crypto;
    bytes_t _key;
    bytes_t _iv;
    bytes_t _input;
    bytes_t _output;
};
