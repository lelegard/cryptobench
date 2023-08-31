//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// RSA benchmarks.
//----------------------------------------------------------------------------

#include "bench_rsa.h"

//----------------------------------------------------------------------------
// RSA encrypt benchmark.
//----------------------------------------------------------------------------

bench_rsa_encrypt::bench_rsa_encrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* input, size_t input_size, bool rekey) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": encrypt" + std::string(rekey ? "-rekey" : ""), min_usec, min_iterations),
    _crypto(crypto),
    _rekey(rekey),
    _input(input),
    _input_size(input_size)
{
}

void bench_rsa_encrypt::one_iteration()
{
    if (_rekey) {
        _crypto.rsa_init_encrypt_oaep();
    }
    _crypto.rsa_encrypt(_input, _input_size, _encrypted, sizeof(_encrypted));
}

//----------------------------------------------------------------------------
// RSA decrypt benchmark.
//----------------------------------------------------------------------------

bench_rsa_decrypt::bench_rsa_decrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* input, size_t input_size, bool rekey) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": decrypt" + std::string(rekey ? "-rekey" : ""), min_usec, min_iterations),
    _crypto(crypto),
    _rekey(rekey),
    _input(input),
    _input_size(input_size)
{
}

void bench_rsa_decrypt::one_iteration()
{
    if (_rekey) {
        _crypto.rsa_init_decrypt_oaep();
    }
    _crypto.rsa_decrypt(_input, _input_size, _decrypted, sizeof(_decrypted));
}

//----------------------------------------------------------------------------
// RSA signature benchmark.
//----------------------------------------------------------------------------

bench_rsa_sign::bench_rsa_sign(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* msg, size_t msg_size, bool rekey) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": sign" + std::string(rekey ? "-rekey" : ""), min_usec, min_iterations),
    _crypto(crypto),
    _rekey(rekey),
    _msg(msg),
    _msg_size(msg_size)
{
}

void bench_rsa_sign::one_iteration()
{
    if (_rekey) {
        _crypto.rsa_init_sign_pss();
    }
    _crypto.rsa_sign(_msg, _msg_size, _sig, sizeof(_sig));
}

//----------------------------------------------------------------------------
// RSA signature verification benchmark.
//----------------------------------------------------------------------------

bench_rsa_verify::bench_rsa_verify(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* msg, size_t msg_size, const uint8_t* sig, size_t sig_size, bool rekey) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": verify" + std::string(rekey ? "-rekey" : ""), min_usec, min_iterations),
    _crypto(crypto),
    _rekey(rekey),
    _msg(msg),
    _msg_size(msg_size),
    _sig(sig),
    _sig_size(sig_size)
{
}

void bench_rsa_verify::one_iteration()
{
    if (_rekey) {
        _crypto.rsa_init_verify_pss();
    }
    if (!_crypto.rsa_verify(_msg, _msg_size, _sig, _sig_size)) {
        sys::fatal(name() + ": signature verification failed");
    }
}

//----------------------------------------------------------------------------
// RSA public key load benchmark.
//----------------------------------------------------------------------------

bench_rsa_load_public::bench_rsa_load_public(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* der, size_t der_size) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": load-public", min_usec, min_iterations),
    _crypto(crypto),
    _der(der),
    _der_size(der_size)
{
}

void bench_rsa_load_public::one_iteration()
{
    _crypto.load_rsa_public_key_der(_der, _der_size);
}

//----------------------------------------------------------------------------
// RSA private key load benchmark.
//----------------------------------------------------------------------------

bench_rsa_load_private::bench_rsa_load_private(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* der, size_t der_size) :
    bench(crypto.name() + ": " + crypto.rsa_name() + ": load-private", min_usec, min_iterations),
    _crypto(crypto),
    _der(der),
    _der_size(der_size)
{
}

void bench_rsa_load_private::one_iteration()
{
    _crypto.load_rsa_private_key_der(_der, _der_size);
}
