//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// OpenSSL cryptographic library.
//----------------------------------------------------------------------------

#pragma once
#include "lib.h"
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

class lib_openssl: public lib
{
public:
    // Constructor / destructor.
    lib_openssl();
    ~lib_openssl();

    // Initializes and cleanup cryptographic library.
    static void init();
    static void cleanup();

    // Print last OpenSSL error and exit.
    static void ossl_fatal(const std::string& message = std::string());

    // Generate an RSA key pair and save them as PEM files.
    // Return the number of microseconds for the key generation.
    static int64_t generate_rsa_key(size_t bits, const std::string& filename_private, const std::string& filename_public);

    // Get the parameters of an RSA private key in a PEM file.
    static void load_rsa_private_key_values(const std::string& filename, BIGNUM*& n, BIGNUM*& e, BIGNUM*& d);

    // Implementation of "lib" interface.
    virtual std::string version() const override;
    virtual bool rsa_available() const override;
    virtual bool aes_available() const override;
    virtual void load_rsa_private_key_der(const uint8_t* der, size_t der_size) override;
    virtual void load_rsa_public_key_der(const uint8_t* der, size_t der_size) override;
    virtual size_t rsa_private_key_bits() const override;
    virtual size_t rsa_public_key_bits() const override;
    virtual void rsa_init_encrypt_oaep() override;
    virtual void rsa_init_decrypt_oaep() override;
    virtual size_t rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual void rsa_init_sign_pss() override;
    virtual void rsa_init_verify_pss() override;
    virtual size_t rsa_sign(const uint8_t* msg, size_t msg_size, uint8_t* sig, size_t sig_maxsize) override;
    virtual bool rsa_verify(const uint8_t* msg, size_t msg_size, const uint8_t* sig, size_t sig_size) override;
    virtual size_t aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;

private:
    EVP_PKEY* _rsa_public_key = nullptr;
    EVP_PKEY* _rsa_private_key = nullptr;
    EVP_PKEY_CTX* _rsa_encrypt_ctx = nullptr;
    EVP_PKEY_CTX* _rsa_decrypt_ctx = nullptr;
    EVP_PKEY_CTX* _rsa_sign_ctx = nullptr;
    EVP_PKEY_CTX* _rsa_verify_ctx = nullptr;

    // AES for encrypt or decrypt.
    size_t aes(bool encrypt, bool cbc,
               const uint8_t* key, size_t key_size,
               const uint8_t* iv, size_t iv_size,
               const uint8_t* input, size_t input_size,
               uint8_t* output, size_t output_maxsize);
};
