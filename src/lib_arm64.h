//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Arm64 accelerated cryptography library.
//----------------------------------------------------------------------------

#pragma once
#include "lib.h"

class lib_arm64: public lib
{
public:
    // Constructor / destructor.
    lib_arm64();
    ~lib_arm64();

    // Implementation of "lib" interface.
    virtual std::string version() const override;
    virtual bool aes_available() const override;
    virtual size_t aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;

private:
};
