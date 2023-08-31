//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// RSA benchmarks. A valid RSA key pair must be loaded first.
//----------------------------------------------------------------------------

#pragma once
#include "bench.h"
#include "lib.h"

//----------------------------------------------------------------------------
// RSA encrypt benchmark.
// Optionally rekey before each operation.
//----------------------------------------------------------------------------

class bench_rsa_encrypt: public bench
{
public:
    // Constructor.
    bench_rsa_encrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* input, size_t input_size, bool rekey);

protected:
    // One iteration.
    virtual void one_iteration() override;

private:
    lib&           _crypto;
    bool           _rekey = false;
    const uint8_t* _input = nullptr;
    size_t         _input_size = 0;
    uint8_t        _encrypted[1024];
};

//----------------------------------------------------------------------------
// RSA decrypt benchmark.
// Optionally rekey before each operation.
//----------------------------------------------------------------------------

class bench_rsa_decrypt: public bench
{
public:
    // Constructor.
    bench_rsa_decrypt(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* input, size_t input_size, bool rekey);

protected:
    // One iteration.
    virtual void one_iteration() override;

private:
    lib&           _crypto;
    bool           _rekey = false;
    const uint8_t* _input = nullptr;
    size_t         _input_size = 0;
    uint8_t        _decrypted[1024];
};


//----------------------------------------------------------------------------
// RSA signature benchmark.
// Optionally rekey before each operation.
//----------------------------------------------------------------------------

class bench_rsa_sign: public bench
{
public:
    // Constructor.
    bench_rsa_sign(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* msg, size_t msg_size, bool rekey);

protected:
    // One iteration.
    virtual void one_iteration() override;

private:
    lib&           _crypto;
    bool           _rekey = false;
    const uint8_t* _msg = nullptr;
    size_t         _msg_size = 0;
    uint8_t        _sig[1024];
};

//----------------------------------------------------------------------------
// RSA signature verification benchmark.
// Optionally rekey before each operation.
//----------------------------------------------------------------------------

class bench_rsa_verify: public bench
{
public:
    // Constructor.
    bench_rsa_verify(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* msg, size_t msg_size, const uint8_t* sig, size_t sig_size, bool rekey);

protected:
    // One iteration.
    virtual void one_iteration() override;

private:
    lib&           _crypto;
    bool           _rekey = false;
    const uint8_t* _msg = nullptr;
    size_t         _msg_size = 0;
    const uint8_t* _sig = nullptr;
    size_t         _sig_size = 0;
};

//----------------------------------------------------------------------------
// RSA public key load benchmark.
//----------------------------------------------------------------------------

class bench_rsa_load_public: public bench
{
public:
    // Constructor.
    bench_rsa_load_public(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* der, size_t der_size);

protected:
    // One iteration.
    virtual void one_iteration() override;

private:
    lib&           _crypto;
    const uint8_t* _der = nullptr;
    size_t         _der_size = 0;
};

//----------------------------------------------------------------------------
// RSA private key load benchmark.
//----------------------------------------------------------------------------

class bench_rsa_load_private: public bench
{
public:
    // Constructor.
    bench_rsa_load_private(lib& crypto, int64_t min_usec, int64_t min_iterations, const uint8_t* der, size_t der_size);

protected:
    // One iteration.
    virtual void one_iteration() override;

private:
    lib&           _crypto;
    const uint8_t* _der = nullptr;
    size_t         _der_size = 0;
};
