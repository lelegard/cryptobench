//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Some benchmarks on mathematical operations used in asymmetric crypto.
//----------------------------------------------------------------------------

#include "bench_math.h"

//----------------------------------------------------------------------------
// Build the name of the object instance.
//----------------------------------------------------------------------------

std::string bench_math::name_of(const std::string& name, const bignum& mod)
{
    return sys::format("math: mod-%d: %s", BN_num_bits(mod.ptr()), name.c_str());
}

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mod func, const bignum& op1, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _op1(op1),
    _mod(mod),
    _f_op1_mod(func),
    _one_iteration(&bench_math::one_iteration_op1_mod)
{
}

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mod_ret func, const bignum& op1, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _op1(op1),
    _mod(mod),
    _f_op1_mod_ret(func),
    _one_iteration(&bench_math::one_iteration_op1_mod_ret)
{
}

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op2_mod func, const bignum& op1, const bignum& op2, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _op1(op1),
    _op2(op2),
    _mod(mod),
    _f_op2_mod(func),
    _one_iteration(&bench_math::one_iteration_op2_mod)
{
}

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mont func, const bignum& op1, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _op1(op1),
    _mod(mod),
    _f_op1_mont(func),
    _one_iteration(&bench_math::one_iteration_op1_mont)
{
    init_montgomery();
}

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op2_mont func, const bignum& op1, const bignum& op2, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _op1(op1),
    _op2(op2),
    _mod(mod),
    _f_op2_mont(func),
    _one_iteration(&bench_math::one_iteration_op2_mont)
{
    init_montgomery();
}

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mod_mont func, const bignum& op1, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _op1(op1),
    _mod(mod),
    _f_op1_mod_mont(func),
    _one_iteration(&bench_math::one_iteration_op1_mod_mont)
{
    init_montgomery();
}

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op2_mod_mont func, const bignum& op1, const bignum& op2, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _op1(op1),
    _op2(op2),
    _mod(mod),
    _f_op2_mod_mont(func),
    _one_iteration(&bench_math::one_iteration_op2_mod_mont)
{
    init_montgomery();
}

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mod_mont_word func, BN_ULONG word, const bignum& op1, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _word(word),
    _op1(op1),
    _mod(mod),
    _f_op1_mod_mont_word(func),
    _one_iteration(&bench_math::one_iteration_op1_mod_mont_word)
{
    init_montgomery();
}

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op2_recp func, const bignum& op1, const bignum& op2, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _op1(op1),
    _op2(op2),
    _mod(mod),
    _f_op2_recp(func),
    _one_iteration(&bench_math::one_iteration_op2_recp)
{
    init_reciprocal();
}

bench_math::bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_rem_op1_recp func, const bignum& op1, const bignum& mod) :
    bench(name_of(name, mod), min_usec, min_iterations),
    _op1(op1),
    _mod(mod),
    _f_rem_op1_recp(func),
    _one_iteration(&bench_math::one_iteration_rem_op1_recp)
{
    init_reciprocal();
}

//----------------------------------------------------------------------------
// Montgomery and reciprocal functions set the modulus in the constructor.
//----------------------------------------------------------------------------

void bench_math::init_montgomery()
{
    if (BN_MONT_CTX_set(openssl::bn_mont_ctx, _mod.ptr(), openssl::bn_ctx) < 0) {
        openssl::fatal("BN_MONT_CTX_set");
    }
}

void bench_math::init_reciprocal()
{
    if (BN_RECP_CTX_set(openssl::bn_recp_ctx, _mod.ptr(), openssl::bn_ctx) < 0) {
        openssl::fatal("BN_RECP_CTX_set");
    }
}

//----------------------------------------------------------------------------
// Iteration methods.
//----------------------------------------------------------------------------

void bench_math::one_iteration()
{
    if (!(this->*_one_iteration)()) {
        openssl::fatal(name() + ": arithmetic error");
    }
}

bool bench_math::one_iteration_op1_mod()
{
    return _f_op1_mod(_res.ptr(), _op1.ptr(), _mod.ptr(), openssl::bn_ctx) > 0;
}

bool bench_math::one_iteration_op2_mod()
{
    return _f_op2_mod(_res.ptr(), _op1.ptr(), _op2.ptr(), _mod.ptr(), openssl::bn_ctx) > 0;
}

bool bench_math::one_iteration_op1_mod_ret()
{
    return _f_op1_mod_ret(_res.ptr(), _op1.ptr(), _mod.ptr(), openssl::bn_ctx) != nullptr;
}

bool bench_math::one_iteration_op1_mont()
{
    return _f_op1_mont(_res.ptr(), _op1.ptr(), openssl::bn_mont_ctx, openssl::bn_ctx) > 0;
}

bool bench_math::one_iteration_op2_mont()
{
    return _f_op2_mont(_res.ptr(), _op1.ptr(), _op2.ptr(), openssl::bn_mont_ctx, openssl::bn_ctx) > 0;
}

bool bench_math::one_iteration_op1_mod_mont()
{
    return _f_op1_mod_mont(_res.ptr(), _op1.ptr(), _mod.ptr(), openssl::bn_ctx, openssl::bn_mont_ctx) > 0;
}

bool bench_math::one_iteration_op2_mod_mont()
{
    return _f_op2_mod_mont(_res.ptr(), _op1.ptr(), _op2.ptr(), _mod.ptr(), openssl::bn_ctx, openssl::bn_mont_ctx) > 0;
}

bool bench_math::one_iteration_op1_mod_mont_word()
{
    return _f_op1_mod_mont_word(_res.ptr(), _word, _op1.ptr(), _mod.ptr(), openssl::bn_ctx, openssl::bn_mont_ctx) > 0;
}

bool bench_math::one_iteration_op2_recp()
{
    return _f_op2_recp(_res.ptr(), _op1.ptr(), _op2.ptr(), openssl::bn_recp_ctx, openssl::bn_ctx) > 0;
}

bool bench_math::one_iteration_rem_op1_recp()
{
    return _f_rem_op1_recp(_res.ptr(), _rem.ptr(), _op1.ptr(), openssl::bn_recp_ctx, openssl::bn_ctx) > 0;
}
