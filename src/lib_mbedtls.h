//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// MbedTLS cryptographic library (formerly PolarSSL).
//----------------------------------------------------------------------------

#pragma once
#include "lib.h"
#include <mbedtls/platform.h>
#include <mbedtls/version.h>
#include <mbedtls/error.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/cipher.h>
#include <mbedtls/md.h>
#include <mbedtls/pk.h>
#include <mbedtls/rsa.h>

class lib_mbedtls: public lib
{
public:
    // Constructor / destructor.
    lib_mbedtls();
    ~lib_mbedtls();

    // Initializes and cleanup cryptographic library.
    static void init();
    static void cleanup();

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
    static bool                     _entropy_valid;
    static mbedtls_entropy_context  _entropy;
    static bool                     _ctr_drbg_valid;
    static mbedtls_ctr_drbg_context _ctr_drbg;

    bool               _rsa_public_key_valid = false;
    mbedtls_pk_context _rsa_public_key;
    bool               _rsa_private_key_valid = false;
    mbedtls_pk_context _rsa_private_key;

    // Set padding and hash on an RSA key context.
    static void set_rsa_padding(mbedtls_pk_context& key, int padding, mbedtls_md_type_t hash_id);

    // AES for encrypt or decrypt.
    size_t aes(mbedtls_operation_t op, bool cbc,
               const uint8_t* key, size_t key_size,
               const uint8_t* iv, size_t iv_size,
               const uint8_t* input, size_t input_size,
               uint8_t* output, size_t output_maxsize);

    // Print MbedTLS errors if not success and exit.
    static void mbed_fatal(int err, const std::string& message = std::string());
};
