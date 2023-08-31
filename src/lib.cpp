//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Base class of all cryptographic libraries.
//----------------------------------------------------------------------------

#include "lib.h"

//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

lib::lib(const std::string& name) :
    _name(name)
{
}

//----------------------------------------------------------------------------
// Cryptographic library names.
//----------------------------------------------------------------------------

std::string lib::rsa_name() const
{
    return sys::format("rsa-%zu", rsa_private_key_bits());
}

std::string lib::aes_name() const
{
    return sys::format("aes-%zu", aes_key_bits());
}

//----------------------------------------------------------------------------
// Default implementation: does nothing, to be overriden in subclasses.
//----------------------------------------------------------------------------

std::string lib::version() const
{
    return "";
}
bool lib::rsa_available() const
{
    return false;
}
bool lib::aes_available() const
{
    return false;
}
void lib::load_rsa_private_key_der(const uint8_t* der, size_t der_size)
{
}
void lib::load_rsa_public_key_der(const uint8_t* der, size_t der_size)
{
}
size_t lib::rsa_private_key_bits() const
{
    return 0; // no key loaded
}
size_t lib::rsa_public_key_bits() const
{
    return 0; // no key loaded
}
void lib::rsa_init_encrypt_oaep()
{
}
void lib::rsa_init_decrypt_oaep()
{
}
size_t lib::rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; // no output
}
size_t lib::rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; // no output
}
void lib::rsa_init_sign_pss()
{
}
void lib::rsa_init_verify_pss()
{
}
size_t lib::rsa_sign(const uint8_t* msg, size_t msg_size, uint8_t* sig, size_t sig_maxsize)
{
    return 0; // no output
}
bool lib::rsa_verify(const uint8_t* msg, size_t msg_size, const uint8_t* sig, size_t sig_size)
{
    return false; // verification failed
}
size_t lib::aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; // no output
}
size_t lib::aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; // no output
}
size_t lib::aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; // no output
}
size_t lib::aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; // no output
}

//----------------------------------------------------------------------------
// Convenience methods to load an RSA key pair from PEM files.
//----------------------------------------------------------------------------

void lib::load_rsa_private_key_file(const std::string& filename)
{
    bytes_t der;
    sys::load_pem_file_as_der(der, filename);
    load_rsa_private_key_der(der.data(), der.size());
}

void lib::load_rsa_public_key_file(const std::string& filename)
{
    bytes_t der;
    sys::load_pem_file_as_der(der, filename);
    load_rsa_public_key_der(der.data(), der.size());
}

//----------------------------------------------------------------------------
// Utility methods for subclasses.
//----------------------------------------------------------------------------

void lib::check_aes_key_size(size_t key_size)
{
    if (key_size != 128/8 && key_size != 192/8 && key_size != 256/8) {
        sys::fatal(sys::format("invalid AES key size: %d bytes", key_size));
    }
    _aes_key_bits = 8 * key_size;
}

void lib::check_aes_size(size_t key_size, size_t input_size, size_t output_maxsize)
{
    check_aes_key_size(key_size);
    if (input_size != AES_BLOCK_SIZE) {
        sys::fatal(sys::format("invalid AES input size: %d bytes", input_size));
    }
    if (output_maxsize < AES_BLOCK_SIZE) {
        sys::fatal(sys::format("invalid AES output buffer size: %d bytes", output_maxsize));
    }
}

void lib::check_aes_cbc_size(size_t key_size, size_t iv_size, size_t input_size, size_t output_maxsize)
{
    check_aes_key_size(key_size);
    if (iv_size != AES_BLOCK_SIZE) {
        sys::fatal(sys::format("invalid AES IV size: %d bytes", iv_size));
    }
    if (input_size % AES_BLOCK_SIZE != 0) {
        sys::fatal(sys::format("invalid AES input size: %d bytes", input_size));
    }
    if (output_maxsize < input_size) {
        sys::fatal(sys::format("invalid AES output buffer size: %d bytes", output_maxsize));
    }
}

//----------------------------------------------------------------------------
// RSA auto-test. A valid key pair must have been loaded.
//----------------------------------------------------------------------------

void lib::rsa_auto_test()
{
    if (rsa_private_key_bits() == 0 || rsa_public_key_bits() == 0 || rsa_private_key_bits() != rsa_public_key_bits()) {
        sys::fatal("RSA auto test impossible without a valid key pair");
    }

    static const uint8_t plain[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    };
    uint8_t output[4096]; // large enough for signature
    size_t output_len = 0;

    memset(output, 0, sizeof(output));
    rsa_init_encrypt_oaep();
    output_len = rsa_encrypt(plain, sizeof(plain), output, sizeof(output));
    if (output_len != rsa_public_key_bits() / 8) {
        sys::fatal(sys::format("RSA encrypt auto-test failed, key size: %zu bits, output size: %zu bytes (%zu bits)", rsa_public_key_bits(), output_len, output_len * 8));
    }

    rsa_init_decrypt_oaep();
    output_len = rsa_decrypt(output, output_len, output, sizeof(output));
    if (output_len != sizeof(plain) || ::memcmp(plain, output, output_len) != 0) {
        sys::fatal("RSA encrypt auto-test failed, decrypted data do not match");
    }

    uint8_t message[32];
    ::memset(message, 0xA5, sizeof(message));

    rsa_init_sign_pss();
    output_len = rsa_sign(message, sizeof(message), output, sizeof(output));
    if (output_len != rsa_public_key_bits() / 8) {
        sys::fatal(sys::format("RSA sign auto-test failed, key size: %zu bits, sig size: %zu bytes (%zu bits)", rsa_public_key_bits(), output_len, output_len * 8));
    }

    rsa_init_verify_pss();
    if (!rsa_verify(message, sizeof(message), output, output_len)) {
        sys::fatal("RSA verify auto-test failed, signature not verified");
    }

    message[7] ^= 0xFF;
    if (rsa_verify(message, sizeof(message), output, output_len)) {
        sys::fatal("RSA verify auto-test failed, signature verified for corrupeted message");
    }
}

//----------------------------------------------------------------------------
// AES auto-test using some known test vectors.
//----------------------------------------------------------------------------

void lib::aes_auto_test()
{
    struct TV_AES {
        size_t  key_size;
        uint8_t key[32];
        uint8_t plain[16];
        uint8_t cipher[16];
    };

    static const TV_AES tv_aes[] = {
        {
            16,
            {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
            {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
            {0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A},
        },
        {
            16,
            {0xA0, 0x3B, 0x8E, 0xC0, 0x7B, 0x60, 0x9C, 0x66, 0x09, 0xC7, 0x9C, 0x09, 0x49, 0x41, 0xCE, 0x28},
            {0x15, 0xB2, 0xAB, 0x88, 0xC9, 0x28, 0x7D, 0xAF, 0xA8, 0xC0, 0x74, 0x36, 0x8D, 0x84, 0x52, 0xE7},
            {0xE5, 0x3E, 0x04, 0x7F, 0x8B, 0xF9, 0xF2, 0x21, 0xF3, 0xD5, 0x2E, 0x2E, 0x2B, 0x1D, 0xC9, 0x25},
        },
        {
            24,
            {0x8E, 0x10, 0x74, 0xEA, 0x14, 0x12, 0x9C, 0xA5, 0x39, 0x08, 0x05, 0xDB, 0xF7, 0x08, 0xAA, 0x0B,
             0xAC, 0xE9, 0xA4, 0xB4, 0x2A, 0x76, 0xD7, 0xFB},
            {0x92, 0x55, 0xAF, 0x1E, 0x31, 0x86, 0x54, 0x0C, 0x49, 0x05, 0x6E, 0x56, 0x81, 0xBC, 0x1D, 0xE2},
            {0x8A, 0x55, 0x2A, 0x90, 0x78, 0xB3, 0x1C, 0xD6, 0x21, 0x1D, 0x39, 0xD6, 0x8E, 0x5A, 0xE4, 0xCF},
        },
        {
            24,
            {0xBD, 0x45, 0x44, 0x9C, 0x8E, 0x94, 0xF2, 0x6E, 0xEB, 0x3F, 0xC7, 0x4D, 0x7A, 0x5F, 0x41, 0xF1,
             0x89, 0xBE, 0x85, 0x99, 0x10, 0xC9, 0x1B, 0xB7},
            {0x12, 0x5F, 0xE8, 0x31, 0xA1, 0xE8, 0x43, 0xAA, 0xCF, 0x8A, 0x56, 0xAB, 0x94, 0x9E, 0xBF, 0x4E},
            {0x7B, 0x5A, 0x4D, 0xFC, 0x8D, 0xD4, 0xB5, 0x9B, 0x01, 0xE5, 0xB3, 0xFC, 0x59, 0x17, 0xEA, 0x20},
        },
        {
            32,
            {0xCF, 0xB2, 0xEB, 0xF6, 0x8A, 0x0B, 0x4E, 0xC7, 0xCE, 0xA3, 0x2F, 0xAE, 0x3C, 0xB0, 0xD4, 0x22,
             0xA9, 0x2B, 0x87, 0x7A, 0x73, 0x18, 0x98, 0x89, 0x9F, 0xAB, 0xE5, 0xDF, 0xA0, 0x5A, 0x70, 0x7E},
            {0x71, 0x87, 0x8C, 0x5C, 0xE2, 0xA4, 0x09, 0x55, 0xA2, 0xBA, 0x9E, 0x9E, 0x61, 0x32, 0x70, 0xCF},
            {0xA8, 0xA6, 0x85, 0x8D, 0x3C, 0xDF, 0x77, 0x0B, 0xC4, 0x84, 0x5F, 0xE5, 0x67, 0x05, 0x3E, 0x37},
        },
        {
            32,
            {0x34, 0x7E, 0x7F, 0x80, 0x57, 0xC2, 0x80, 0x2C, 0x25, 0xF6, 0xDA, 0xA4, 0x5B, 0x98, 0xD6, 0x1E,
             0xCA, 0xBB, 0xA5, 0x4E, 0xBE, 0x47, 0x3F, 0x93, 0x98, 0x01, 0x65, 0x43, 0xC0, 0x26, 0x23, 0xD2},
            {0xC8, 0x02, 0xA4, 0xB9, 0xC7, 0xBC, 0xC5, 0xDC, 0xF3, 0xE1, 0x6F, 0x13, 0x3C, 0x12, 0x7C, 0x8E},
            {0xEF, 0xB1, 0x39, 0xB4, 0x83, 0x2A, 0x7D, 0x6E, 0x61, 0x0E, 0xB1, 0x02, 0xA9, 0x93, 0x30, 0x5E},
        },
        {0, {}, {}, {}} // end of list
    };

    for (const TV_AES* tv = tv_aes; tv->key_size != 0; tv++) {
        uint8_t output[AES_BLOCK_SIZE];
        size_t output_len = 0;

        output_len = aes_encrypt(tv->key, tv->key_size, tv->plain, sizeof(tv->plain), output, sizeof(output));
        if (output_len != sizeof(tv->cipher) || ::memcmp(output, tv->cipher, output_len) != 0) {
            sys::fatal(_name + ": " + aes_name() + ": ECB encrypt test vector failed");
        }

        output_len = aes_decrypt(tv->key, tv->key_size, tv->cipher, sizeof(tv->cipher), output, sizeof(output));
        if (output_len != sizeof(tv->plain) || ::memcmp(output, tv->plain, output_len) != 0) {
            sys::fatal(_name + ": " + aes_name() + ": ECB decrypt test vector failed");
        }
    }

    struct TV_AES_CHAIN {
        size_t  key_size;
        uint8_t key[32];
        size_t  iv_size;
        uint8_t iv[16];
        size_t  plain_size;
        uint8_t plain[256];
        size_t  cipher_size;
        uint8_t cipher[256];
    };

    static const TV_AES_CHAIN tv_cbc_aes[] = {
        {
            16,
            {0x59, 0xD0, 0x24, 0xE3, 0x2A, 0x1A, 0x5F, 0xBA, 0xF1, 0xED, 0x6A, 0x94, 0xB1, 0x14, 0xE0, 0x73},
            16,
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            16,
            {0x79, 0x97, 0x4F, 0x54, 0xF3, 0x3F, 0x46, 0x39, 0x3D, 0x30, 0x64, 0x62, 0x8F, 0xD5, 0x67, 0x0F},
            16,
            {0xA5, 0x12, 0x8E, 0xE5, 0x7F, 0x88, 0xF4, 0xA6, 0x00, 0x6B, 0xE8, 0x6E, 0xFC, 0x57, 0x03, 0x00},
        },
        {
            16,
            {0xA8, 0xB4, 0x78, 0x37, 0xD4, 0xF6, 0x16, 0x10, 0x7D, 0xC0, 0xB8, 0x41, 0x42, 0x83, 0xB0, 0xDC},
            16,
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            32,
            {0x50, 0x19, 0xE0, 0x69, 0xBA, 0x92, 0x63, 0xAB, 0x02, 0x8E, 0x51, 0xFC, 0x1E, 0x60, 0x40, 0x02,
             0xA2, 0x41, 0x66, 0x60, 0x84, 0x5D, 0x9E, 0x29, 0x6E, 0x53, 0xEF, 0x9B, 0x4C, 0x61, 0x1B, 0x0C},
            32,
            {0x76, 0x3F, 0x97, 0x8C, 0x9D, 0x0C, 0x81, 0xA4, 0x00, 0xFC, 0x25, 0x00, 0xA1, 0xED, 0x40, 0x3C,
             0x65, 0x02, 0xB2, 0x92, 0x9E, 0x59, 0xC0, 0xCE, 0x6A, 0xCB, 0x7C, 0x49, 0x28, 0x54, 0x26, 0xB6},
        },
        {0, {}, 0, {}, 0, {}, 0, {}} // end of list
    };

    for (const TV_AES_CHAIN* tv = tv_cbc_aes; tv->key_size != 0; tv++) {
        uint8_t output[sizeof(tv->cipher)];
        size_t output_len = 0;

        output_len = aes_encrypt_cbc(tv->key, tv->key_size, tv->iv, tv->iv_size, tv->plain, tv->plain_size, output, sizeof(output));
        if (output_len != tv->cipher_size || ::memcmp(output, tv->cipher, output_len) != 0) {
            sys::fatal(_name + ": " + aes_name() + ": CBC encrypt test vector failed");
        }

        output_len = aes_decrypt_cbc(tv->key, tv->key_size, tv->iv, tv->iv_size, tv->cipher, tv->cipher_size, output, sizeof(output));
        if (output_len != tv->plain_size || ::memcmp(output, tv->plain, output_len) != 0) {
            sys::fatal(_name + ": " + aes_name() + ": CBC decrypt test vector failed");
        }
    }
}
