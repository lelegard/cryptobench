//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// OpenSSL utilities.
//----------------------------------------------------------------------------

#include "openssl.h"

BN_CTX* openssl::bn_ctx = nullptr;
BN_MONT_CTX* openssl::bn_mont_ctx = nullptr;
BN_RECP_CTX* openssl::bn_recp_ctx = nullptr;

void openssl::init()
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    if (bn_ctx == nullptr && (bn_ctx = BN_CTX_new()) == nullptr) {
        fatal("BN_CTX_new");
    }
    if (bn_mont_ctx == nullptr && (bn_mont_ctx = BN_MONT_CTX_new()) == nullptr) {
        fatal("BN_MONT_CTX_new");
    }
    if (bn_recp_ctx == nullptr && (bn_recp_ctx = BN_RECP_CTX_new()) == nullptr) {
        fatal("BN_RECP_CTX_new");
    }
}

void openssl::cleanup()
{
    EVP_cleanup();
    ERR_free_strings();

    if (bn_ctx != nullptr) {
        BN_CTX_free(bn_ctx);
        bn_ctx = nullptr;
    }
    if (bn_mont_ctx != nullptr) {
        BN_MONT_CTX_free(bn_mont_ctx);
        bn_mont_ctx = nullptr;
    }
    if (bn_recp_ctx != nullptr) {
        BN_RECP_CTX_free(bn_recp_ctx);
        bn_recp_ctx = nullptr;
    }
}

void openssl::fatal(const std::string& message)
{
    if (!message.empty()) {
        std::cerr << "openssl: " << message << std::endl;
    }
    ERR_print_errors_fp(stderr);
    std::exit(EXIT_FAILURE);
}
