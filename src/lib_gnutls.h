//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// GnuTLS cryptographic library.
//----------------------------------------------------------------------------

#pragma once
#include "lib.h"
#include <gnutls/gnutls.h>

class lib_gnutls: public lib
{
public:
    // Constructor / destructor.
    lib_gnutls();
    ~lib_gnutls();

    // Initializes and cleanup cryptographic library.
    static void init();
    static void cleanup();

    // To be completed: How can we do OAEP with GnuTLS?
    // What kind of padding does gnutls_pubkey_encrypt_data() use?

    // Implementation of "lib" interface.
    virtual std::string version() const override;
    virtual bool rsa_available() const override;
    virtual bool aes_available() const override;
    virtual void load_rsa_private_key_der(const uint8_t* der, size_t der_size) override;
    virtual void load_rsa_public_key_der(const uint8_t* der, size_t der_size) override;
    virtual size_t rsa_private_key_bits() const override;
    virtual size_t rsa_public_key_bits() const override;
    virtual size_t rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t rsa_sign(const uint8_t* msg, size_t msg_size, uint8_t* sig, size_t sig_maxsize) override;
    virtual bool rsa_verify(const uint8_t* msg, size_t msg_size, const uint8_t* sig, size_t sig_size) override;
    virtual size_t aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;

private:
    gnutls_pubkey_t _rsa_public_key = nullptr;
    gnutls_privkey_t _rsa_private_key = nullptr;

    // Get AES CBC cipher algo from key size.
    gnutls_cipher_algorithm_t aes_cbc_algo(size_t key_size);

    // Print GnuTLS errors if not success and exit.
    static void gtls_fatal(int err, const std::string& message = std::string());
};
