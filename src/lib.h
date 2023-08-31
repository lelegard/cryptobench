//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Base class of all cryptographic libraries.
//----------------------------------------------------------------------------

#pragma once
#include "sys.h"

class lib
{
public:
    // Any error terminates the application.
    // Encrypt/decrypt methods return the output size in bytes.

    // Some common crypto constants.
    static constexpr size_t AES_BLOCK_SIZE = 16;

    // Check if RSA and AES are available.
    // False by default, subclasses must override them to support an algo.
    virtual bool rsa_available() const;
    virtual bool aes_available() const;

    // Cryptographic library names and version.
    virtual std::string version() const;
    std::string name() const { return _name; }
    std::string rsa_name() const;
    std::string aes_name() const;

    // Each instance shall be able to load one RSA key pair from DER data.
    virtual void load_rsa_private_key_der(const uint8_t* der, size_t der_size);
    virtual void load_rsa_public_key_der(const uint8_t* der, size_t der_size);
    virtual size_t rsa_private_key_bits() const;
    virtual size_t rsa_public_key_bits() const;

    // Convenience methods to load an RSA key pair from PEM files.
    void load_rsa_private_key_file(const std::string& filename);
    void load_rsa_public_key_file(const std::string& filename);

    // Initialize RSA context for AOEP encrypt/decrypt.
    virtual void rsa_init_encrypt_oaep();
    virtual void rsa_init_decrypt_oaep();

    // RSA encrypt/decrypt, according to current mode.
    virtual size_t rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);
    virtual size_t rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);

    // Initialize RSA context for PSS sign/verify.
    virtual void rsa_init_sign_pss();
    virtual void rsa_init_verify_pss();

    // RSA sign/verify. The verification status is returned.
    virtual size_t rsa_sign(const uint8_t* msg, size_t msg_size, uint8_t* sig, size_t sig_maxsize);
    virtual bool rsa_verify(const uint8_t* msg, size_t msg_size, const uint8_t* sig, size_t sig_size);

    // AES encryption or decryption of one block.
    virtual size_t aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);
    virtual size_t aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);

    // AES CBC encryption or decryption, without padding, must be a multiple of block size.
    virtual size_t aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);
    virtual size_t aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);

    // Last AES key size.
    size_t aes_key_bits() const { return _aes_key_bits; }

    // RSA auto-test. A valid key pair must have been loaded.
    void rsa_auto_test();

    // AES auto-test using some known test vectors.
    void aes_auto_test();

protected:
    // Constructor.
    lib() = delete;
    lib(const std::string& name);

    // Utility methods for subclasses.
    void check_aes_key_size(size_t key_size);
    void check_aes_size(size_t key_size, size_t input_size, size_t output_maxsize);
    void check_aes_cbc_size(size_t key_size, size_t iv_size, size_t input_size, size_t output_maxsize);

private:
    const std::string _name;
    size_t _aes_key_bits;
};
