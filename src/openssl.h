//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// OpenSSL utilities.
//----------------------------------------------------------------------------

#pragma once
#include "sys.h"
#include <openssl/opensslv.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#if !defined(OPENSSL_VERSION_MAJOR) // before v3
#define OPENSSL_VERSION_MAJOR (OPENSSL_VERSION_NUMBER >> 28)
#endif

#if !defined(OPENSSL_VERSION_MINOR) // before v3
#define OPENSSL_VERSION_MINOR ((OPENSSL_VERSION_NUMBER >> 20) & 0xFF)
#endif

#if OPENSSL_VERSION_MAJOR >= 3
#include <openssl/core_names.h>
#endif

class openssl
{
public:
    // Initializes and cleanup cryptographic library.
    static void init();
    static void cleanup();

    // Print last OpenSSL error and exit.
    static void fatal(const std::string& message = std::string());

    // Common contexts for all explicit BIGNUM computations in benchmarks.
    static BN_CTX* bn_ctx;
    static BN_MONT_CTX* bn_mont_ctx;
    static BN_RECP_CTX* bn_recp_ctx;
};
