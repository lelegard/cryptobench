//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Some benchmarks on mathematical operations used in asymmetric crypto.
//----------------------------------------------------------------------------

#include "bench_math.h"

//----------------------------------------------------------------------------
// Encapsulation of an OpenSSL BIGNUM with automation creation/deletion.
//----------------------------------------------------------------------------

bignum::bignum() : ptr(BN_new())
{
    if (ptr == nullptr) {
        lib_openssl::ossl_fatal("BN_new");
    }
}

bignum::bignum(BIGNUM* value) : ptr(BN_dup(value))
{
    if (ptr == nullptr) {
        lib_openssl::ossl_fatal("BN_dup");
    }
}

bignum::bignum(int bits, bool must_be_odd) : bignum()
{
    if (BN_rand(ptr, bits, 0, must_be_odd) < 0) {
        lib_openssl::ossl_fatal("BN_rand");
    }
}

bignum::~bignum()
{
    if (ptr != nullptr) {
        BN_free(ptr);
        ptr = nullptr;
    }
}

//----------------------------------------------------------------------------
// Base class for math benchmarks
//----------------------------------------------------------------------------

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations) :
    bench(name, min_usec, min_iterations)
{
    if ((_ctx = BN_CTX_new()) == nullptr) {
        lib_openssl::ossl_fatal("BN_CTX_new");
    }
}

bench_math::~bench_math()
{
    if (_ctx != nullptr) {
        BN_CTX_free(_ctx);
        _ctx = nullptr;
    }
}

//----------------------------------------------------------------------------
// Modular exponentation benchmarks: a ^ p (mod m)
//----------------------------------------------------------------------------

bench_math_mod_exp::bench_math_mod_exp(int64_t min_usec, int64_t min_iterations, BIGNUM* p, BIGNUM* m) :
    bench_math(sys::format("exp-%d-mod-%d", BN_num_bits(p), BN_num_bits(m)), min_usec, min_iterations),
    _a(BN_num_bits(m) - 1, false),
    _p(p),
    _m(m)
{
}

void bench_math_mod_exp::one_iteration()
{
    if (BN_mod_exp(_r.ptr, _a.ptr, _p.ptr, _m.ptr, _ctx) <= 0) {
        lib_openssl::ossl_fatal("BN_mod_exp");
    }
}

//----------------------------------------------------------------------------
// Modular exponentation benchmarks using BN_mod_exp_simple.
//----------------------------------------------------------------------------

bench_math_mod_exp_simple::bench_math_mod_exp_simple(int64_t min_usec, int64_t min_iterations, BIGNUM* p, BIGNUM* m) :
    bench_math(sys::format("exp-%d-mod-%d-simple", BN_num_bits(p), BN_num_bits(m)), min_usec, min_iterations),
    _a(BN_num_bits(m) - 1, false),
    _p(p),
    _m(m)
{
    // Check that BN_mod_exp_simple and BN_mod_exp are identical.
    bignum r1, r2;
    if (BN_mod_exp(r1.ptr, _a.ptr, _p.ptr, _m.ptr, _ctx) <= 0) {
        lib_openssl::ossl_fatal("BN_mod_exp");
    }
    if (BN_mod_exp_simple(r2.ptr, _a.ptr, _p.ptr, _m.ptr, _ctx) <= 0) {
        lib_openssl::ossl_fatal("BN_mod_exp_simple");
    }
    if (BN_cmp(r1.ptr, r2.ptr)) {
        sys::fatal("BN_mod_exp and BN_mod_exp_simple are not identical");
    }
}

void bench_math_mod_exp_simple::one_iteration()
{
    if (BN_mod_exp_simple(_r.ptr, _a.ptr, _p.ptr, _m.ptr, _ctx) <= 0) {
        lib_openssl::ossl_fatal("BN_mod_exp_simple");
    }
}

//----------------------------------------------------------------------------
// Modular exponentation benchmarks using BN_mod_exp_mont.
//----------------------------------------------------------------------------

bench_math_mod_exp_mont::bench_math_mod_exp_mont(int64_t min_usec, int64_t min_iterations, BIGNUM* p, BIGNUM* m) :
    bench_math(sys::format("exp-%d-mod-%d-mont", BN_num_bits(p), BN_num_bits(m)), min_usec, min_iterations),
    _a(BN_num_bits(m) - 1, false),
    _p(p),
    _m(m)
{
    if ((_mont = BN_MONT_CTX_new()) == nullptr) {
        lib_openssl::ossl_fatal("BN_MONT_CTX_new");
    }
    if (BN_MONT_CTX_set(_mont, _m.ptr, _ctx) < 0) {
        lib_openssl::ossl_fatal("BN_MONT_CTX_set");
    }
    // Check that BN_mod_exp_mont and BN_mod_exp are identical.
    bignum r1, r2;
    if (BN_mod_exp(r1.ptr, _a.ptr, _p.ptr, _m.ptr, _ctx) <= 0) {
        lib_openssl::ossl_fatal("BN_mod_exp");
    }
    if (BN_mod_exp_mont(r2.ptr, _a.ptr, _p.ptr, _m.ptr, _ctx, _mont) <= 0) {
        lib_openssl::ossl_fatal("BN_mod_exp_mont");
    }
    if (BN_cmp(r1.ptr, r2.ptr)) {
        sys::fatal("BN_mod_exp and BN_mod_exp_mont are not identical");
    }
}

bench_math_mod_exp_mont::~bench_math_mod_exp_mont()
{
    if (_mont != nullptr) {
        BN_MONT_CTX_free(_mont);
        _mont = nullptr;
    }
}

void bench_math_mod_exp_mont::one_iteration()
{
    if (BN_mod_exp_mont(_r.ptr, _a.ptr, _p.ptr, _m.ptr, _ctx, _mont) <= 0) {
        lib_openssl::ossl_fatal("BN_mod_exp_mont");
    }
}
