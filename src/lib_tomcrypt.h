//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// LibTomCrypt cryptographic library.
//----------------------------------------------------------------------------

#pragma once
#include "lib.h"
#include <tomcrypt.h>

class lib_tomcrypt: public lib
{
public:
    // The asymmetric crypto is based on a multi-precision calculus backend.
    // By default, use libtommath. GMP can be selected in constructor.
    // Warning: The MP backend is set globally in LibTomCrypt.
    // The MP backend is set at each operation on a lib_tomcrypt instance.
    // This means that class is not thread-safe. CryptoBench is single-threaded.

    // Constructor / destructor.
    lib_tomcrypt(bool use_gmp = false);
    ~lib_tomcrypt();

    // Initializes and cleanup cryptographic library.
    static void init();
    static void cleanup();

    // Implementation of "lib" interface.
    virtual bool rsa_available() const override;
    virtual bool aes_available() const override;
    virtual std::string version() const override;
    virtual void load_rsa_private_key(const std::string& filename) override;
    virtual void load_rsa_public_key(const std::string& filename) override;
    virtual size_t rsa_private_key_bits() const override;
    virtual size_t rsa_public_key_bits() const override;
    virtual size_t rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;

private:
    static int        _aes_index;
    static int        _sha256_index;
    static int        _yarrow_prng_index;
    static bool       _yarrow_prng_valid;
    static prng_state _yarrow_prng;

    static const ltc_math_descriptor* const _ltm_desc;
    static const ltc_math_descriptor* const _gmp_desc;

    const ltc_math_descriptor* const _math_desc = nullptr;
    bool    _rsa_public_key_valid = false;
    rsa_key _rsa_public_key;
    bool    _rsa_private_key_valid = false;
    rsa_key _rsa_private_key;

    // Set the multi-precision calculus backend.
    void set_mp() const;

    // Find a cipher, hash or prng. Fail on error.
    static int tom_find_cipher(const std::string& name);
    static int tom_find_hash(const std::string& name);
    static int tom_find_prng(const std::string& name);

    // Cleanup RSA data in the object.
    void rsa_cleanup();

    // Print LibTomCrypt errors if not success and exit.
    static void tom_fatal(int err, const std::string& message = std::string());
};
