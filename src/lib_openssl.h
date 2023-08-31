//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// OpenSSL cryptographic library.
//----------------------------------------------------------------------------

#pragma once
#include "lib.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

class lib_openssl: public lib
{
public:
    // Constructor / destructor.
    lib_openssl();
    ~lib_openssl();

    // Initializes and cleanup cryptographic library.
    static void init();
    static void cleanup();

    // Generate an RSA key pair and save them as PEM files.
    // Return the number of microseconds for the key generation.
    static int64_t generate_rsa_key(size_t bits, const std::string& filename_private, const std::string& filename_public);

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
    virtual size_t aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;

private:
    EVP_PKEY* _rsa_public_key = nullptr;
    EVP_PKEY* _rsa_private_key = nullptr;
    EVP_PKEY_CTX* _rsa_encrypt_ctx = nullptr;
    EVP_PKEY_CTX* _rsa_decrypt_ctx = nullptr;

    // AES for encrypt or decrypt.
    size_t aes(bool encrypt, bool cbc,
               const uint8_t* key, size_t key_size,
               const uint8_t* iv, size_t iv_size,
               const uint8_t* input, size_t input_size,
               uint8_t* output, size_t output_maxsize);

    // Print last OpenSSL error and exit.
    static void ossl_fatal(const std::string& message = std::string());
};
