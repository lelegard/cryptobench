//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Some benchmarks on mathematical operations used in asymmetric crypto.
// This is an experimental module, subject to change according to
// obsevations and test results.
//----------------------------------------------------------------------------

#pragma once
#include "bench.h"
#include "lib_openssl.h"


//----------------------------------------------------------------------------
// Encapsulation of an OpenSSL BIGNUM with automation creation/deletion.
//----------------------------------------------------------------------------

class bignum
{
public:
    BIGNUM* ptr = nullptr;
    bignum();
    bignum(BIGNUM* value);
    bignum(int bits, bool must_be_odd); // random value
    ~bignum();
};

//----------------------------------------------------------------------------
// Base class for math benchmarks
//----------------------------------------------------------------------------

class bench_math: public bench
{
protected:
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations);
    ~bench_math();

    BN_CTX* _ctx = nullptr;
};

//----------------------------------------------------------------------------
// Modular exponentation benchmarks: a ^ p (mod m)
//----------------------------------------------------------------------------

class bench_math_mod_exp: public bench_math
{
public:
    bench_math_mod_exp(int64_t min_usec, int64_t min_iterations, BIGNUM* p, BIGNUM* m);
protected:
    virtual void one_iteration() override;
private:
    bignum _r;  // result variable
    bignum _a;  // arbitrary value, large size
    bignum _p;
    bignum _m;
};

//----------------------------------------------------------------------------
// Modular exponentation benchmarks using BN_mod_exp_simple.
//----------------------------------------------------------------------------

class bench_math_mod_exp_simple: public bench_math
{
public:
    bench_math_mod_exp_simple(int64_t min_usec, int64_t min_iterations, BIGNUM* p, BIGNUM* m);
protected:
    virtual void one_iteration() override;
private:
    bignum _r;  // result variable
    bignum _a;  // arbitrary value, large size
    bignum _p;
    bignum _m;
};

//----------------------------------------------------------------------------
// Modular exponentation benchmarks using BN_mod_exp_mont.
//----------------------------------------------------------------------------

class bench_math_mod_exp_mont: public bench_math
{
public:
    bench_math_mod_exp_mont(int64_t min_usec, int64_t min_iterations, BIGNUM* p, BIGNUM* m);
    ~bench_math_mod_exp_mont();
protected:
    virtual void one_iteration() override;
private:
    BN_MONT_CTX* _mont = nullptr;
    bignum _r;  // result variable
    bignum _a;  // arbitrary value, large size
    bignum _p;
    bignum _m;
};
