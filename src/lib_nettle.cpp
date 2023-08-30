//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Nettle cryptographic library.
//----------------------------------------------------------------------------

#include "lib_nettle.h"

#define MAX_RSA_KEY_BITS 8192

//----------------------------------------------------------------------------
// Constructor/destructor.
//----------------------------------------------------------------------------

lib_nettle::lib_nettle() :
    lib("nettle")
{
}

lib_nettle::~lib_nettle()
{
    if (_rsa_public_key_valid) {
        nettle_rsa_public_key_clear(&_rsa_public_key);
        _rsa_public_key_valid = false;
    }
    if (_rsa_private_key_valid) {
        nettle_rsa_private_key_clear(&_rsa_private_key);
        _rsa_private_key_valid = false;
    }
}

//----------------------------------------------------------------------------
// Library initialization/cleanup.
//----------------------------------------------------------------------------

void lib_nettle::init()
{
}

void lib_nettle::cleanup()
{
}

//----------------------------------------------------------------------------
// Cryptographic library version.
//----------------------------------------------------------------------------

std::string lib_nettle::version() const
{
    return sys::format("%d.%d", nettle_version_major(), nettle_version_minor());
}

bool lib_nettle::rsa_available() const
{
    // WARNING: RSA currently disabled with nettle until we can fix the
    // loading of public and private key files.
    return false;
}

bool lib_nettle::aes_available() const
{
    return true;
}

//----------------------------------------------------------------------------
// Load private key from a PEM file.
//----------------------------------------------------------------------------

void lib_nettle::load_rsa_private_key(const std::string& filename)
{
    // Read the PEM file for the key.
    bytes_t der;
    sys::load_pem_file_as_der(der, filename);

    rsa_public_key pubkey;
    nettle_rsa_public_key_init(&pubkey);

    if (!_rsa_private_key_valid) {
        nettle_rsa_private_key_init(&_rsa_private_key);
        _rsa_private_key_valid = true;
    }

    // WARNING: something not working here with key files from OpenSSL.
    if (!nettle_rsa_keypair_from_der(&pubkey, &_rsa_private_key, MAX_RSA_KEY_BITS, der.size(), der.data())) {
        sys::fatal(name() + ": cannot decode DER as private key in " + filename);
    }

    nettle_rsa_public_key_clear(&pubkey);
}

//----------------------------------------------------------------------------
// Load public key from a PEM file.
//----------------------------------------------------------------------------

void lib_nettle::load_rsa_public_key(const std::string& filename)
{
    // Read the PEM file for the key.
    bytes_t der;
    sys::load_pem_file_as_der(der, filename);

    if (!_rsa_public_key_valid) {
        nettle_rsa_public_key_init(&_rsa_public_key);
        _rsa_public_key_valid = true;
    }

    // WARNING: something not working here with key files from OpenSSL.
    if (!nettle_rsa_keypair_from_der(&_rsa_public_key, nullptr, MAX_RSA_KEY_BITS, der.size(), der.data())) {
        sys::fatal(name() + ": cannot decode DER as public key in " + filename);
    }
}

//----------------------------------------------------------------------------
// Current RSA key sizes.
//----------------------------------------------------------------------------

size_t lib_nettle::rsa_private_key_bits() const
{
    return _rsa_private_key_valid ? _rsa_private_key.size * 8 : 0;
}

size_t lib_nettle::rsa_public_key_bits() const
{
    return _rsa_public_key_valid ? _rsa_public_key.size * 8 : 0;
}

//----------------------------------------------------------------------------
// RSA encrypt, according to current mode.
//----------------------------------------------------------------------------

size_t lib_nettle::rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; // to be implemented after fixing the key issue above
}

//----------------------------------------------------------------------------
// RSA decrypt, according to current mode.
//----------------------------------------------------------------------------

size_t lib_nettle::rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; // to be implemented after fixing the key issue above
}

//----------------------------------------------------------------------------
// AES-ECB encryption.
//----------------------------------------------------------------------------

size_t lib_nettle::aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    switch (key_size) {
        case 128/8: {
            aes128_ctx ctx;
            nettle_aes128_set_encrypt_key(&ctx, key);
            nettle_aes128_encrypt(&ctx, input_size, output, input);
            return input_size;
        }
        case 192/8: {
            aes192_ctx ctx;
            nettle_aes192_set_encrypt_key(&ctx, key);
            nettle_aes192_encrypt(&ctx, input_size, output, input);
            return input_size;
        }
        case 256/8: {
            aes256_ctx ctx;
            nettle_aes256_set_encrypt_key(&ctx, key);
            nettle_aes256_encrypt(&ctx, input_size, output, input);
            return input_size;
        }
        default:
            sys::fatal(sys::format("%s: invalid key size %zu bits", name().c_str(), key_size * 8));
            return 0; // unreachable
    }
}

//----------------------------------------------------------------------------
// AES-ECB decryption.
//----------------------------------------------------------------------------

size_t lib_nettle::aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    switch (key_size) {
        case 128/8: {
            aes128_ctx ctx;
            nettle_aes128_set_decrypt_key(&ctx, key);
            nettle_aes128_decrypt(&ctx, input_size, output, input);
            return input_size;
        }
        case 192/8: {
            aes192_ctx ctx;
            nettle_aes192_set_decrypt_key(&ctx, key);
            nettle_aes192_decrypt(&ctx, input_size, output, input);
            return input_size;
        }
        case 256/8: {
            aes256_ctx ctx;
            nettle_aes256_set_decrypt_key(&ctx, key);
            nettle_aes256_decrypt(&ctx, input_size, output, input);
            return input_size;
        }
        default:
            sys::fatal(sys::format("%s: invalid key size %zu bits", name().c_str(), key_size * 8));
            return 0; // unreachable
    }
}

//----------------------------------------------------------------------------
// AES-CBC encryption.
//----------------------------------------------------------------------------

size_t lib_nettle::aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);

    uint8_t ivbuf[AES_BLOCK_SIZE];
    ::memcpy(ivbuf, iv, sizeof(ivbuf));

    switch (key_size) {
        case 128/8: {
            aes128_ctx ctx;
            nettle_aes128_set_encrypt_key(&ctx, key);
            nettle_cbc_aes128_encrypt(&ctx, ivbuf, input_size, output, input);
            return input_size;
        }
        case 192/8: {
            aes192_ctx ctx;
            nettle_aes192_set_encrypt_key(&ctx, key);
            nettle_cbc_aes192_encrypt(&ctx, ivbuf, input_size, output, input);
            return input_size;
        }
        case 256/8: {
            aes256_ctx ctx;
            nettle_aes256_set_encrypt_key(&ctx, key);
            nettle_cbc_aes256_encrypt(&ctx, ivbuf, input_size, output, input);
            return input_size;
        }
        default:
            sys::fatal(sys::format("%s: invalid key size %zu bits", name().c_str(), key_size * 8));
            return 0; // unreachable
    }
}

//----------------------------------------------------------------------------
// AES-CBC decryption.
//----------------------------------------------------------------------------

size_t lib_nettle::aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);

    uint8_t ivbuf[AES_BLOCK_SIZE];
    ::memcpy(ivbuf, iv, sizeof(ivbuf));

    switch (key_size) {
        case 128/8: {
            aes128_ctx ctx;
            nettle_aes128_set_decrypt_key(&ctx, key);
            nettle_cbc_decrypt(&ctx, (nettle_cipher_func*)(nettle_aes128_decrypt), AES_BLOCK_SIZE, ivbuf, input_size, output, input);
            return input_size;
        }
        case 192/8: {
            aes192_ctx ctx;
            nettle_aes192_set_encrypt_key(&ctx, key);
            nettle_cbc_decrypt(&ctx, (nettle_cipher_func*)(nettle_aes192_decrypt), AES_BLOCK_SIZE, ivbuf, input_size, output, input);
            return input_size;
        }
        case 256/8: {
            aes256_ctx ctx;
            nettle_aes256_set_encrypt_key(&ctx, key);
            nettle_cbc_decrypt(&ctx, (nettle_cipher_func*)(nettle_aes256_decrypt), AES_BLOCK_SIZE, ivbuf, input_size, output, input);
            return input_size;
        }
        default:
            sys::fatal(sys::format("%s: invalid key size %zu bits", name().c_str(), key_size * 8));
            return 0; // unreachable
    }
}
