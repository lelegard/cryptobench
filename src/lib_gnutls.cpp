//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// GnuTLS cryptographic library.
//----------------------------------------------------------------------------

#include "lib_gnutls.h"
#include <gnutls/crypto.h>
#include <gnutls/abstract.h>

//----------------------------------------------------------------------------
// Constructor/destructor.
//----------------------------------------------------------------------------

lib_gnutls::lib_gnutls() :
    lib("gnutls")
{
}

lib_gnutls::~lib_gnutls()
{
    if (_rsa_public_key != nullptr) {
        gnutls_pubkey_deinit(_rsa_public_key);
        _rsa_public_key = nullptr;
    }
    if (_rsa_private_key != nullptr) {
        gnutls_privkey_deinit(_rsa_private_key);
        _rsa_private_key = nullptr;
    }
}

//----------------------------------------------------------------------------
// GnuTLS initialization/cleanup.
//----------------------------------------------------------------------------

void lib_gnutls::init()
{
    // Nothing to do.
}

void lib_gnutls::cleanup()
{
    // Nothing to do.
}

//----------------------------------------------------------------------------
// Cryptographic library properties.
//----------------------------------------------------------------------------

std::string lib_gnutls::version() const
{
    return GNUTLS_VERSION;
}

bool lib_gnutls::rsa_available() const
{
    return true;
}

bool lib_gnutls::aes_available() const
{
    return true;
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
// Load RSA private key from DER data.
//----------------------------------------------------------------------------

void lib_gnutls::load_rsa_private_key_der(const uint8_t* der, size_t der_size)
{
    int err = 0;
    if (_rsa_private_key == nullptr) {
        err = gnutls_privkey_init(&_rsa_private_key);
        gtls_fatal(err, "error in gnutls_privkey_init");
    }
    gnutls_datum_t der_datum = {(unsigned char*)(der), (unsigned int)(der_size)};
    err = gnutls_privkey_import_x509_raw(_rsa_private_key, &der_datum, GNUTLS_X509_FMT_DER, nullptr, 0);
    gtls_fatal(err, "error loading RSA private key");
}

//----------------------------------------------------------------------------
// Load RSA public key from DER data.
//----------------------------------------------------------------------------

void lib_gnutls::load_rsa_public_key_der(const uint8_t* der, size_t der_size)
{
    int err = 0;
    if (_rsa_public_key == nullptr) {
        err = gnutls_pubkey_init(&_rsa_public_key);
        gtls_fatal(err, "error in gnutls_pubkey_init");
    }
    gnutls_datum_t der_datum = {(unsigned char*)(der), (unsigned int)(der_size)};
    err = gnutls_pubkey_import(_rsa_public_key, &der_datum, GNUTLS_X509_FMT_DER);
    gtls_fatal(err, "error loading RSA public key");
}

//----------------------------------------------------------------------------
// Current RSA key sizes.
// GnuTLS has no public API to get the size of a public or private key.
// For these tests, we export the modulus from the key to get the key size.
// This is extremely unefficient.
//----------------------------------------------------------------------------

size_t lib_gnutls::rsa_private_key_bits() const
{
    gnutls_datum_t m = {nullptr, 0};
    int err = gnutls_privkey_export_rsa_raw(_rsa_private_key, &m, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    gtls_fatal(err, "error in gnutls_privkey_export_rsa_raw");
    size_t bits = sys::large_number_bits(m.data, m.size);
    gnutls_free(m.data);
    return bits;
}

size_t lib_gnutls::rsa_public_key_bits() const
{
    gnutls_datum_t m = {nullptr, 0};
    int err = gnutls_pubkey_export_rsa_raw(_rsa_public_key, &m, nullptr);
    gtls_fatal(err, "error in gnutls_pubkey_export_rsa_raw");
    size_t bits = sys::large_number_bits(m.data, m.size);
    gnutls_free(m.data);
    return bits;
}


//----------------------------------------------------------------------------
// RSA encrypt/decrypt.
//----------------------------------------------------------------------------

size_t lib_gnutls::rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    gnutls_datum_t in = {const_cast<uint8_t*>(input), (unsigned int)(input_size)};
    gnutls_datum_t out = {nullptr, 0};
    int err = gnutls_pubkey_encrypt_data(_rsa_public_key, 0, &in, &out);
    gtls_fatal(err, "error in gnutls_pubkey_encrypt_data");
    const size_t output_length = std::min<size_t>(output_maxsize, out.size);
    ::memcpy(output, out.data, output_length);
    gnutls_free(out.data);
    return output_length;
}

size_t lib_gnutls::rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    gnutls_datum_t in = {const_cast<uint8_t*>(input), (unsigned int)(input_size)};
    gnutls_datum_t out = {nullptr, 0};
    int err = gnutls_privkey_decrypt_data(_rsa_private_key, 0, &in, &out);
    gtls_fatal(err, "error in gnutls_privkey_decrypt_data");
    const size_t output_length = std::min<size_t>(output_maxsize, out.size);
    ::memcpy(output, out.data, output_length);
    gnutls_free(out.data);
    return output_length;
}

//----------------------------------------------------------------------------
// RSA sign/verify.
//----------------------------------------------------------------------------

size_t lib_gnutls::rsa_sign(const uint8_t* msg, size_t msg_size, uint8_t* sig, size_t sig_maxsize)
{
    gnutls_datum_t in = {const_cast<uint8_t*>(msg), (unsigned int)(msg_size)};
    gnutls_datum_t out = {nullptr, 0};
    int err = gnutls_privkey_sign_hash(_rsa_private_key, GNUTLS_DIG_SHA256, GNUTLS_PRIVKEY_SIGN_FLAG_RSA_PSS, &in, &out);
    gtls_fatal(err, "error in gnutls_privkey_sign_hash");
    const size_t sig_length = std::min<size_t>(sig_maxsize, out.size);
    ::memcpy(sig, out.data, sig_length);
    gnutls_free(out.data);
    return sig_length;
}

bool lib_gnutls::rsa_verify(const uint8_t* msg, size_t msg_size, const uint8_t* sig, size_t sig_size)
{
    gnutls_datum_t dmsg = {const_cast<uint8_t*>(msg), (unsigned int)(msg_size)};
    gnutls_datum_t dsig = {const_cast<uint8_t*>(sig), (unsigned int)(sig_size)};
    int err = gnutls_pubkey_verify_hash2(_rsa_public_key, GNUTLS_SIGN_RSA_PSS_SHA256, 0, &dmsg, &dsig);
    if (err == GNUTLS_E_PK_SIG_VERIFY_FAILED) {
        return false;
    }
    else {
        gtls_fatal(err, "error in gnutls_pubkey_verify_hash2");
        return true;
    }
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
