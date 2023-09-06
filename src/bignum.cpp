//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Some benchmarks on mathematical operations used in asymmetric crypto.
// Encapsulation of an OpenSSL BIGNUM with automation creation/deletion.
//----------------------------------------------------------------------------

#include "bignum.h"

//----------------------------------------------------------------------------
// Constructor / destructor.
//----------------------------------------------------------------------------

bignum::bignum(BIGNUM* bn) :
    _ptr(bn == nullptr ? new_or_die() : bn)
{
}

bignum::~bignum()
{
    if (_ptr != nullptr) {
        BN_free(_ptr);
        _ptr = nullptr;
    }
}

bignum::bignum(const bignum& other) :
    _ptr(dup_or_die(other._ptr))
{
}

bignum::bignum(bignum&& other) :
    _ptr(other._ptr)
{
    // After move semantics, other is dead, cannot be used anymore, except in destructor.
    other._ptr = nullptr;
}

bignum& bignum::operator=(const bignum& other)
{
    if (&other != this) {
        copy(other._ptr);
    }
    return *this;
}

bignum& bignum::operator=(bignum&& other)
{
    if (&other != this) {
        BN_free(_ptr);
        _ptr = other._ptr;
        // After move semantics, other is dead, cannot be used anymore, except in destructor.
        other._ptr = nullptr;
    }
    return *this;
}

void bignum::copy(const BIGNUM* other)
{
    if (other == nullptr) {
        BN_free(_ptr);
        _ptr = new_or_die();
    }
    else if (BN_copy(_ptr, other) == nullptr) {
        openssl::fatal("BN_copy");
    }
}

void bignum::capture(BIGNUM* other)
{
    BN_free(_ptr);
    _ptr = other == nullptr ? new_or_die() : other;
}

//----------------------------------------------------------------------------
// Equivalent to BN_new() and BN_dup() but fail on error.
//----------------------------------------------------------------------------

BIGNUM* bignum::new_or_die()
{
    BIGNUM* bn = BN_new();
    if (bn == nullptr) {
        openssl::fatal("BN_new");
    }
    return bn;
}

BIGNUM* bignum::dup_or_die(const BIGNUM* other)
{
    BIGNUM* bn = BN_dup(other);
    if (bn == nullptr) {
        openssl::fatal("BN_dup");
    }
    return bn;
}

//----------------------------------------------------------------------------
// Return a random number of the given bit-size.
//----------------------------------------------------------------------------

bignum bignum::random(int bits, bool must_be_odd)
{
    bignum res;
    if (BN_rand(res._ptr, bits, 0, must_be_odd) < 0) {
        openssl::fatal("BN_rand");
    }
    return res;
}

//----------------------------------------------------------------------------
// Return a random prime number of the given bit-size.
//----------------------------------------------------------------------------

bignum bignum::random_prime(int bits)
{
    bignum res;
#if OPENSSL_VERSION_MAJOR >= 3
    if (BN_generate_prime_ex(res._ptr, bits, 0, nullptr, nullptr, nullptr) < 0) {
        openssl::fatal("BN_generate_prime_ex");
    }
#else
    // Legacy version.
    if (BN_generate_prime(res._ptr, bits, 0, nullptr, nullptr, nullptr, nullptr) == nullptr) {
        openssl::fatal("BN_generate_prime");
    }
#endif
    return res;
}

//----------------------------------------------------------------------------
// Basic modular operations.
//----------------------------------------------------------------------------

bignum bignum::square_mod(const bignum& mod)
{
    bignum res;
    if (BN_mod_sqr(res._ptr, _ptr, mod._ptr, openssl::bn_ctx) <= 0) {
        openssl::fatal("BN_mod_sqr");
    }
    return res;
}

bignum bignum::square_root_mod(const bignum& mod)
{
    bignum res;
    if (BN_mod_sqrt(res._ptr, _ptr, mod._ptr, openssl::bn_ctx) == nullptr) {
        openssl::fatal("BN_mod_sqrt");
    }
    return res;
}

bignum bignum::inverse_mod(const bignum& mod)
{
    bignum res;
    if (BN_mod_inverse(res._ptr, _ptr, mod._ptr, openssl::bn_ctx) == nullptr) {
        openssl::fatal("BN_mod_inverse");
    }
    return res;
}
