//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Accelerated cryptographic algorithms on Arm64.
//----------------------------------------------------------------------------

#pragma once
#include <cstddef>
#include <cinttypes>

namespace arm64 {

    // Check if the CPU supports AES acccelerated instructions.
    // If it returns false, calling AES functions in this module will fail.
    bool has_aes_instructions();

    // AES implementation using Arm64 accelerated instructions.
    class aes
    {
    public:
        // Constructor/destructor.
        aes();
        ~aes();

        // AES characteristics.
        static constexpr size_t BLOCK_SIZE = 16;
        static constexpr size_t MAX_KEY_SIZE = 32;

        // Set a new key. Size must be 16, 24 or 32 bytes.
        // Return true on success, false on error.
        bool set_key(const uint8_t* key, size_t key_size);

        // Wipe the key content, erase all secret stuff from memory.
        // Also automatically called in the destructor.
        void wipe_key();

        // Encrypt/decrypt one block. Input size must be 16 bytes.
        // Return the output size, always 16.
        size_t encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);
        size_t decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);

        // Encrypt/decrypt data in CBC mode, without padding. Input size must be a multiple of 16 bytes.
        // The IV size must be 16 bytes. The IV is updated with the last cipher block to allow chaining.
        // Return the output size, always the same as input size.
        size_t encrypt_cbc(uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);
        size_t decrypt_cbc(uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize);

    private:
        // This substructure contains the secret stuff. Its actual content depends
        // on Arm64 header files that we do not want to include in the applications.
        struct key_data;
        uint8_t _secret[496];
    };
}
