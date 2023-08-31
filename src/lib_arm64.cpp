//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Arm64 accelerated cryptography library.
//----------------------------------------------------------------------------

#include "lib_arm64.h"

//----------------------------------------------------------------------------
// Constructor / destructor.
//----------------------------------------------------------------------------

lib_arm64::lib_arm64() :
    lib("arm64")
{
}

lib_arm64::~lib_arm64()
{
}

//----------------------------------------------------------------------------
// Cryptographic library properties.
//----------------------------------------------------------------------------

std::string lib_arm64::version() const
{
    return sys::format("aes-inst: %s", arm64::has_aes_instructions() ? "yes" : "no");
}

bool lib_arm64::aes_available() const
{
    // Available only if on Arm64 with AES instructions.
    return arm64::has_aes_instructions();
}

//----------------------------------------------------------------------------
// Provide AES only.
//----------------------------------------------------------------------------

size_t lib_arm64::aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    if (!_aes.set_key(key, key_size)) {
        return 0;
    }
    return _aes.encrypt(input, input_size, output, output_maxsize);
}

size_t lib_arm64::aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    if (!_aes.set_key(key, key_size)) {
        return 0;
    }
    return _aes.decrypt(input, input_size, output, output_maxsize);
}

size_t lib_arm64::aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    if (iv_size != AES_BLOCK_SIZE || !_aes.set_key(key, key_size)) {
        return 0;
    }
    uint8_t iv2[AES_BLOCK_SIZE];
    ::memcpy(iv2, iv, AES_BLOCK_SIZE);
    return _aes.encrypt_cbc(iv2, iv_size, input, input_size, output, output_maxsize);
}

size_t lib_arm64::aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    if (iv_size != AES_BLOCK_SIZE || !_aes.set_key(key, key_size)) {
        return 0;
    }
    uint8_t iv2[AES_BLOCK_SIZE];
    ::memcpy(iv2, iv, AES_BLOCK_SIZE);
    return _aes.decrypt_cbc(iv2, iv_size, input, input_size, output, output_maxsize);
}
