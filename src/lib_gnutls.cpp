//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// GnuTLS cryptographic library.
//----------------------------------------------------------------------------

#include "lib_gnutls.h"

//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

lib_gnutls::lib_gnutls() :
    lib("gnutls")
{
    //@@@
}

//----------------------------------------------------------------------------
// Destructor.
//----------------------------------------------------------------------------

lib_gnutls::~lib_gnutls()
{
    //@@@
}

//----------------------------------------------------------------------------
// GnuTLS initialization.
//----------------------------------------------------------------------------

void lib_gnutls::init()
{
    //@@@
}

//----------------------------------------------------------------------------
// GnuTLS cleanup.
//----------------------------------------------------------------------------

void lib_gnutls::cleanup()
{
    //@@@
}

//----------------------------------------------------------------------------
// Cryptographic library version.
//----------------------------------------------------------------------------

std::string lib_gnutls::version() const
{
    return GNUTLS_VERSION;
}

//----------------------------------------------------------------------------
// Print GnuTLS errors if not success and exit.
//----------------------------------------------------------------------------

void lib_gnutls::gtls_fatal(int err, const std::string& message)
{
    if (err < 0) {
        if (!message.empty()) {
            std::cerr << "gnutls: " << message << std::endl;
        }
        std::cerr << "gnutls: error " << err << ": " << gnutls_strerror(err) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

//----------------------------------------------------------------------------
// Load private key from a PEM file.
//----------------------------------------------------------------------------

void lib_gnutls::load_rsa_private_key(const std::string& filename)
{
    //@@@
}

//----------------------------------------------------------------------------
// Load public key from a PEM file.
//----------------------------------------------------------------------------

void lib_gnutls::load_rsa_public_key(const std::string& filename)
{
    //@@@
}

//----------------------------------------------------------------------------
// Current RSA key sizes.
//----------------------------------------------------------------------------

size_t lib_gnutls::rsa_private_key_bits() const
{
    return 0; //@@@
}

size_t lib_gnutls::rsa_public_key_bits() const
{
    return 0; //@@@
}


//----------------------------------------------------------------------------
// Initialize RSA context for AOEP encrypt.
//----------------------------------------------------------------------------

void lib_gnutls::rsa_init_encrypt_oaep()
{
    //@@@
}

//----------------------------------------------------------------------------
// Initialize RSA context for AOEP decrypt.
//----------------------------------------------------------------------------

void lib_gnutls::rsa_init_decrypt_oaep()
{
    //@@@
}

//----------------------------------------------------------------------------
// RSA encrypt, according to current mode.
//----------------------------------------------------------------------------

size_t lib_gnutls::rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; //@@@
}

//----------------------------------------------------------------------------
// RSA decrypt, according to current mode.
//----------------------------------------------------------------------------

size_t lib_gnutls::rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    return 0; //@@@
}

//----------------------------------------------------------------------------
// AES-ECB encryption / decryption.
// GnuTLS does not support ECB mode.
// On one block, ECB is quivalent to CBC with a zero IV.
//----------------------------------------------------------------------------

namespace {
    const uint8_t zero_iv[lib::AES_BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}

size_t lib_gnutls::aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    return aes_encrypt_cbc(key, key_size, zero_iv, sizeof(zero_iv), input, input_size, output, output_maxsize);
}

size_t lib_gnutls::aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    return aes_decrypt_cbc(key, key_size, zero_iv, sizeof(zero_iv), input, input_size, output, output_maxsize);
}

//----------------------------------------------------------------------------
// Get AES CBC cipher algo from key size.
//----------------------------------------------------------------------------

gnutls_cipher_algorithm_t lib_gnutls::aes_cbc_algo(size_t key_size)
{
    switch (key_size) {
        case 128/8: return GNUTLS_CIPHER_AES_128_CBC;
        case 192/8: return GNUTLS_CIPHER_AES_192_CBC;
        case 256/8: return GNUTLS_CIPHER_AES_256_CBC;
        default: sys::fatal(sys::format("%s: invalid key size %zu bits", name().c_str(), key_size * 8));
    }
    return GNUTLS_CIPHER_UNKNOWN; // never reached
}

//----------------------------------------------------------------------------
// AES-CBC encryption.
//----------------------------------------------------------------------------

size_t lib_gnutls::aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);

    gnutls_cipher_hd_t hd;
    gnutls_datum_t key_datum = {const_cast<uint8_t*>(key), (unsigned int)(key_size)};
    gnutls_datum_t iv_datum = {const_cast<uint8_t*>(iv), (unsigned int)(iv_size)};

    int err = gnutls_cipher_init(&hd, aes_cbc_algo(key_size), &key_datum, &iv_datum);
    gtls_fatal(err, "error in gnutls_cipher_init");
    err = gnutls_cipher_encrypt2(hd, input, input_size, output, output_maxsize);
    gtls_fatal(err, "error in gnutls_cipher_encrypt2");
    gnutls_cipher_deinit(hd);

    return input_size;
}

//----------------------------------------------------------------------------
// AES-CBC decryption.
//----------------------------------------------------------------------------

size_t lib_gnutls::aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);

    gnutls_cipher_hd_t hd;
    gnutls_datum_t key_datum = {const_cast<uint8_t*>(key), (unsigned int)(key_size)};
    gnutls_datum_t iv_datum = {const_cast<uint8_t*>(iv), (unsigned int)(iv_size)};

    int err = gnutls_cipher_init(&hd, aes_cbc_algo(key_size), &key_datum, &iv_datum);
    gtls_fatal(err, "error in gnutls_cipher_init");
    err = gnutls_cipher_decrypt2(hd, input, input_size, output, output_maxsize);
    gtls_fatal(err, "error in gnutls_cipher_decrypt2");
    gnutls_cipher_deinit(hd);

    return input_size;
}
