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
}

//----------------------------------------------------------------------------
// Destructor.
//----------------------------------------------------------------------------

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
// GnuTLS initialization.
//----------------------------------------------------------------------------

void lib_gnutls::init()
{
    // Nothing to do.
}

//----------------------------------------------------------------------------
// GnuTLS cleanup.
//----------------------------------------------------------------------------

void lib_gnutls::cleanup()
{
    // Nothing to do.
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
    int err = 0;
    if (_rsa_private_key == nullptr) {
        err = gnutls_privkey_init(&_rsa_private_key);
        gtls_fatal(err, "error in gnutls_privkey_init");
    }
    bytes_t pem;
    sys::load_file(pem, filename);
    gnutls_datum_t pem_datum = {pem.data(), (unsigned int)(pem.size())};
    err = gnutls_privkey_import_x509_raw(_rsa_private_key, &pem_datum, GNUTLS_X509_FMT_PEM, nullptr, 0);
    gtls_fatal(err, "error loading RSA private key from " + filename);
}

//----------------------------------------------------------------------------
// Load public key from a PEM file.
//----------------------------------------------------------------------------

void lib_gnutls::load_rsa_public_key(const std::string& filename)
{
    int err = 0;
    if (_rsa_public_key == nullptr) {
        err = gnutls_pubkey_init(&_rsa_public_key);
        gtls_fatal(err, "error in gnutls_pubkey_init");
    }
    bytes_t pem;
    sys::load_file(pem, filename);
    gnutls_datum_t pem_datum = {pem.data(), (unsigned int)(pem.size())};
    err = gnutls_pubkey_import(_rsa_public_key, &pem_datum, GNUTLS_X509_FMT_PEM);
    gtls_fatal(err, "error loading RSA public key from " + filename);
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
// Initialize RSA context for AOEP encrypt.
//----------------------------------------------------------------------------

void lib_gnutls::rsa_init_encrypt_oaep()
{
    // How can we do this with GnuTLS?
    // What kind of padding use gnutls_pubkey_encrypt_data()?
}

//----------------------------------------------------------------------------
// Initialize RSA context for AOEP decrypt.
//----------------------------------------------------------------------------

void lib_gnutls::rsa_init_decrypt_oaep()
{
    // How can we do this with GnuTLS?
    // What kind of padding use gnutls_privkey_decrypt_data()?
}

//----------------------------------------------------------------------------
// RSA encrypt, according to current mode.
//----------------------------------------------------------------------------

size_t lib_gnutls::rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    gnutls_datum_t in = {const_cast<uint8_t*>(input), (unsigned int)(input_size)};
    gnutls_datum_t out = {nullptr, 0};
    int err = gnutls_pubkey_encrypt_data(_rsa_public_key, 0, &in, &out);
    gtls_fatal(err, "error in gnutls_pubkey_encrypt_data");
    ::memcpy(output, out.data, std::min<size_t>(output_maxsize, out.size));
    gnutls_free(out.data);
    return out.size;
}

//----------------------------------------------------------------------------
// RSA decrypt, according to current mode.
//----------------------------------------------------------------------------

size_t lib_gnutls::rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    gnutls_datum_t in = {const_cast<uint8_t*>(input), (unsigned int)(input_size)};
    gnutls_datum_t out = {nullptr, 0};
    int err = gnutls_privkey_decrypt_data(_rsa_private_key, 0, &in, &out);
    gtls_fatal(err, "error in gnutls_privkey_decrypt_data");
    ::memcpy(output, out.data, std::min<size_t>(output_maxsize, out.size));
    gnutls_free(out.data);
    return out.size;
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
