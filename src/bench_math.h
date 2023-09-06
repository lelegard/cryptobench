//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Some benchmarks on mathematical operations used in asymmetric crypto.
// This is an experimental module, subject to change according to
// observations and test results.
//----------------------------------------------------------------------------

#pragma once
#include "bench.h"
#include "bignum.h"

class bench_math: public bench
{
public:
    // BN_ modular arithmetic functions with 1 or 2 operands and a modulus.
    typedef int (*function_op1_mod)(BIGNUM* res, const BIGNUM* op1, const BIGNUM* mod, BN_CTX*);
    typedef int (*function_op2_mod)(BIGNUM* res, const BIGNUM* op1, const BIGNUM* op2, const BIGNUM* mod, BN_CTX*);

    // Variant which also return the result, possibly allocated if res is null.
    typedef BIGNUM* (*function_op1_mod_ret)(BIGNUM* res, const BIGNUM* op1, const BIGNUM* mod, BN_CTX*);

    // BN_ montgomery modular arithmetic functions.
    // In some cases, the modulus is implicit from the BN_MONT_CTX, sometimes it is explicit (and the ctx order is reversed).
    typedef int (*function_op1_mont)(BIGNUM* res, const BIGNUM* op1, BN_MONT_CTX*, BN_CTX*);
    typedef int (*function_op2_mont)(BIGNUM* res, const BIGNUM* op1, const BIGNUM* op2, BN_MONT_CTX*, BN_CTX*);
    typedef int (*function_op1_mod_mont)(BIGNUM* res, const BIGNUM* op1, const BIGNUM* mod, BN_CTX*, BN_MONT_CTX*);
    typedef int (*function_op2_mod_mont)(BIGNUM* res, const BIGNUM* op1, const BIGNUM* op2, const BIGNUM* mod, BN_CTX*, BN_MONT_CTX*);
    typedef int (*function_op1_mod_mont_word)(BIGNUM* res, BN_ULONG word, const BIGNUM* op1, const BIGNUM* mod, BN_CTX*, BN_MONT_CTX*);

    // BN_ reciprocal modular arithmetic functions.
    typedef int (*function_op2_recp)(BIGNUM* res, const BIGNUM* op1, const BIGNUM* op2, BN_RECP_CTX*, BN_CTX*);
    typedef int (*function_rem_op1_recp)(BIGNUM* res, BIGNUM* rem, const BIGNUM* op1, BN_RECP_CTX*, BN_CTX*);

    // Constructors with a function and operands.
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mod func, const bignum& op1, const bignum& mod);
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mod_ret func, const bignum& op1, const bignum& mod);
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op2_mod func, const bignum& op1, const bignum& op2, const bignum& mod);
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mont func, const bignum& op1, const bignum& mod);
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op2_mont func, const bignum& op1, const bignum& op2, const bignum& mod);
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mod_mont func, const bignum& op1, const bignum& mod);
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op2_mod_mont func, const bignum& op1, const bignum& op2, const bignum& mod);
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op1_mod_mont_word func, BN_ULONG word, const bignum& op1, const bignum& mod);
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_op2_recp func, const bignum& op1, const bignum& op2, const bignum& mod);
    bench_math(const std::string& name, int64_t min_usec, int64_t min_iterations, function_rem_op1_recp func, const bignum& op1, const bignum& mod);

protected:
    virtual void one_iteration() override;

private:
    bignum _res;  // operation result, unused
    bignum _rem;  // additional operation result (remainder), unused
    BN_ULONG _word;
    bignum _op1;
    bignum _op2;
    bignum _mod;
    function_op1_mod _f_op1_mod = nullptr;
    function_op2_mod _f_op2_mod = nullptr;
    function_op1_mod_ret _f_op1_mod_ret = nullptr;
    function_op1_mont _f_op1_mont = nullptr;
    function_op2_mont _f_op2_mont = nullptr;
    function_op1_mod_mont _f_op1_mod_mont = nullptr;
    function_op2_mod_mont _f_op2_mod_mont = nullptr;
    function_op1_mod_mont_word _f_op1_mod_mont_word = nullptr;
    function_op2_recp _f_op2_recp = nullptr;
    function_rem_op1_recp _f_rem_op1_recp = nullptr;

    // Actual iteration method, depending on function type.
    // Return true on success, false on error
    bool (bench_math::*_one_iteration)() = nullptr;

    // Each type of function has its dedicated iteration method.
    bool one_iteration_op1_mod();
    bool one_iteration_op2_mod();
    bool one_iteration_op1_mod_ret();
    bool one_iteration_op1_mont();
    bool one_iteration_op2_mont();
    bool one_iteration_op1_mod_mont();
    bool one_iteration_op2_mod_mont();
    bool one_iteration_op1_mod_mont_word();
    bool one_iteration_op2_recp();
    bool one_iteration_rem_op1_recp();

    // Montgomery and reciprocal functions need to set the modulus in the constructor.
    void init_montgomery();
    void init_reciprocal();

    // Build the name of the object instance.
    static std::string name_of(const std::string& name, const bignum& mod);
};
