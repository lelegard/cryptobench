//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Encapsulation of an OpenSSL BIGNUM with automation creation/deletion.
//----------------------------------------------------------------------------

#pragma once
#include "openssl.h"

class bignum
{
public:
    // Direct access to big number for BN_ functions. Never null.
    BIGNUM* ptr() { return _ptr; }
    const BIGNUM* ptr() const { return _ptr; }

    // Constructor. If bn is not null, capture the pointer, free the BIGNUM no later than the destructor.
    bignum(BIGNUM* = nullptr);

    // Move/copy.
    bignum(const bignum&);
    bignum(bignum&&);
    bignum& operator=(const bignum&);
    bignum& operator=(bignum&&);

    // Copy a BIGNUM into the object.
    void copy(const BIGNUM*);

    // Capture a BIGNUM pointer in this object. The BIGNUM will be freed no later than the destructor.
    void capture(BIGNUM*);

    // Big number size.
    int bits() const { return BN_num_bits(_ptr); }

    // Return a random number of the given bit-size.
    static bignum random(int bits, bool must_be_odd = false);

    // Return a random prime number of the given bit-size.
    static bignum random_prime(int bits);

    // Basic modular operations.
    bignum square_mod(const bignum& mod);
    bignum square_root_mod(const bignum& mod);
    bignum inverse_mod(const bignum& mod);

    // Destructor.
    ~bignum();

private:
    // Managed object. Never null.
    BIGNUM* _ptr = nullptr;

    // Equivalent to BN_new() and BN_dup() but fail on error.
    static BIGNUM* new_or_die();
    static BIGNUM* dup_or_die(const BIGNUM*);
};
