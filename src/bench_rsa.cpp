//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// RSA benchmarks.
//----------------------------------------------------------------------------

#include "bench_rsa.h"

//----------------------------------------------------------------------------
// RSA encrypt benchmark.
//----------------------------------------------------------------------------

bench_rsa_encrypt::bench_rsa_encrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* input, size_t input_size) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": encrypt", min_usec, min_iterations),
    _crypto(crypto),
    _input(input),
    _input_size(input_size)
{
}

void bench_rsa_encrypt::one_iteration()
{
    _crypto.rsa_encrypt(_input, _input_size, _encrypted, sizeof(_encrypted));
}

//----------------------------------------------------------------------------
// RSA decrypt benchmark.
//----------------------------------------------------------------------------

bench_rsa_decrypt::bench_rsa_decrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* input, size_t input_size) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": decrypt", min_usec, min_iterations),
    _crypto(crypto),
    _input(input),
    _input_size(input_size)
{
}

void bench_rsa_decrypt::one_iteration()
{
    _crypto.rsa_decrypt(_input, _input_size, _decrypted, sizeof(_decrypted));
}

//----------------------------------------------------------------------------
// RSA public key rekeying benchmark.
//----------------------------------------------------------------------------

bench_rsa_rekey_public::bench_rsa_rekey_public(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* der, size_t der_size) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": rekey-public", min_usec, min_iterations),
    _crypto(crypto),
    _der(der),
    _der_size(der_size)
{
}

void bench_rsa_rekey_public::one_iteration()
{
    _crypto.load_rsa_public_key_der(_der, _der_size);
}

//----------------------------------------------------------------------------
// RSA private key rekeying benchmark.
//----------------------------------------------------------------------------

bench_rsa_rekey_private::bench_rsa_rekey_private(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* der, size_t der_size) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": rekey-private", min_usec, min_iterations),
    _crypto(crypto),
    _der(der),
    _der_size(der_size)
{
}

void bench_rsa_rekey_private::one_iteration()
{
    _crypto.load_rsa_private_key_der(_der, _der_size);
}
