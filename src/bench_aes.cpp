//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// AES benchmarks (variants with and without key scheduling).
//----------------------------------------------------------------------------

#include "bench_aes.h"

//----------------------------------------------------------------------------
// AES encrypt with key scheduling constructor.
//----------------------------------------------------------------------------

bench_aes_encrypt::bench_aes_encrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, size_t key_bits, size_t data_size) :
    bench(sys::format("%s: aes-%d: encrypt %'zu bytes", crypto.name().c_str(), key_bits, data_size), min_usec, min_iterations),
    _crypto(crypto),
    _key(key_bits / 8, 0xA5),
    _iv(lib::AES_BLOCK_SIZE, 0x35),
    _input(((data_size + lib::AES_BLOCK_SIZE - 1) / lib::AES_BLOCK_SIZE) * lib::AES_BLOCK_SIZE, 0x78),
    _output(_input.size())
{
}

//----------------------------------------------------------------------------
// One AES encrypt with key scheduling iteration.
//----------------------------------------------------------------------------

void bench_aes_encrypt::one_iteration()
{
    _crypto.aes_encrypt_cbc(_key.data(), _key.size(), _iv.data(), _iv.size(), _input.data(), _input.size(), &_output[0], _output.size());
}

//----------------------------------------------------------------------------
// AES decrypt with key scheduling constructor.
//----------------------------------------------------------------------------

bench_aes_decrypt::bench_aes_decrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, size_t key_bits, size_t data_size) :
    bench(sys::format("%s: aes-%d: decrypt-sched %'zu bytes", crypto.name().c_str(), key_bits, data_size), min_usec, min_iterations),
    _crypto(crypto),
    _key(key_bits / 8, 0xA5),
    _iv(lib::AES_BLOCK_SIZE, 0x35),
    _input(data_size, 0x78),
    _output(data_size + lib::AES_BLOCK_SIZE)
{
}

//----------------------------------------------------------------------------
// One AES decrypt with key scheduling iteration.
//----------------------------------------------------------------------------

void bench_aes_decrypt::one_iteration()
{
    _crypto.aes_decrypt_cbc(_key.data(), _key.size(), _iv.data(), _iv.size(), _input.data(), _input.size(), &_output[0], _output.size());
}
