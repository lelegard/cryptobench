//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// GnuTLS cryptographic library.
//----------------------------------------------------------------------------

#pragma once
#include "lib.h"
#include <gnutls/gnutls.h>
#include <gnutls/crypto.h>

class lib_gnutls: public lib
{
public:
    // Constructor / destructor.
    lib_gnutls();
    ~lib_gnutls();

    // Initializes and cleanup cryptographic library.
    static void init();
    static void cleanup();

    // Implementation of "lib" interface.
    virtual std::string version() const override;
    virtual void load_rsa_private_key(const std::string& filename) override;
    virtual void load_rsa_public_key(const std::string& filename) override;
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
    // Get AES CBC cipher algo from key size.
    gnutls_cipher_algorithm_t aes_cbc_algo(size_t key_size);

    // Print GnuTLS errors if not success and exit.
    static void gtls_fatal(int err, const std::string& message = std::string());
};
