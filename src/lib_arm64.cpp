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
    return "0.0";
}

bool lib_arm64::aes_available() const
{
    // to be completed: true if on Arm64 and AES instructions are available.
    return false;
}

//----------------------------------------------------------------------------
// Provide AES only.
//----------------------------------------------------------------------------

size_t lib_arm64::aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0;  //@@@ to be completed
}

size_t lib_arm64::aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0;  //@@@ to be completed
}

size_t lib_arm64::aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0;  //@@@ to be completed
}

size_t lib_arm64::aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0;  //@@@ to be completed
}
