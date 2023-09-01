//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// MbedTLS cryptographic library (formerly PolarSSL).
//----------------------------------------------------------------------------

#include "lib_mbedtls.h"

// Static members:
mbedtls_entropy_context lib_mbedtls::_entropy;
mbedtls_ctr_drbg_context lib_mbedtls::_ctr_drbg;
bool lib_mbedtls::_entropy_valid = false;
bool lib_mbedtls::_ctr_drbg_valid = false;

//----------------------------------------------------------------------------
// Constructor/destructor.
//----------------------------------------------------------------------------

lib_mbedtls::lib_mbedtls() :
    lib("mbedtls")
{
}

lib_mbedtls::~lib_mbedtls()
{
    if (_rsa_public_key_valid) {
        mbedtls_pk_free(&_rsa_public_key);
        _rsa_public_key_valid = false;
    }
    if (_rsa_private_key_valid) {
        mbedtls_pk_free(&_rsa_private_key);
        _rsa_private_key_valid = false;
    }
}

//----------------------------------------------------------------------------
// Library initialization/cleanup.
//----------------------------------------------------------------------------

void lib_mbedtls::init()
{
    mbedtls_entropy_init(&_entropy);
    _entropy_valid = true;
    mbedtls_ctr_drbg_init(&_ctr_drbg);
    _ctr_drbg_valid = true;

    static const uint8_t pers[] = {
        0x0B, 0x0F, 0xB1, 0x60, 0x98, 0xFD, 0x31, 0x55, 0x59, 0xE6, 0x7E, 0x2F, 0x51, 0xC5, 0x44, 0x99,
        0xAC, 0xC9, 0xEF, 0x66, 0xBE, 0xE9, 0x5B, 0x7B, 0x17, 0x48, 0xE8, 0x3B, 0x30, 0x26, 0x43, 0xE3,
    };

    int err = mbedtls_ctr_drbg_seed(&_ctr_drbg, mbedtls_entropy_func, &_entropy, pers, sizeof(pers));
    mbed_fatal(err, "error in mbedtls_ctr_drbg_seed\n");
}

void lib_mbedtls::cleanup()
{
    if (_ctr_drbg_valid) {
        mbedtls_ctr_drbg_free(&_ctr_drbg);
        _ctr_drbg_valid = false;
    }
    if (_entropy_valid) {
        mbedtls_entropy_free(&_entropy);
        _entropy_valid = false;
    }
}

//----------------------------------------------------------------------------
// Cryptographic library properties.
//----------------------------------------------------------------------------

std::string lib_mbedtls::version() const
{
    char mbed_version[256];
    mbedtls_version_get_string_full(mbed_version);
    return mbed_version;
}

bool lib_mbedtls::rsa_available() const
{
    return true;
}

bool lib_mbedtls::aes_available() const
{
    return true;
}

//----------------------------------------------------------------------------
// Print MbedTLS errors if not success and exit.
//----------------------------------------------------------------------------

void lib_mbedtls::mbed_fatal(int err, const std::string& message)
{
    if (err < 0) {
        if (!message.empty()) {
            std::cerr << "mbedtls: " << message << std::endl;
        }
        char msg[1024];
        mbedtls_strerror(err, msg, sizeof(msg));
        std::cerr << "mbedtls: error " << err << ": " << msg << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

//----------------------------------------------------------------------------
// Load RSA private key from DER data.
//----------------------------------------------------------------------------

void lib_mbedtls::load_rsa_private_key_der(const uint8_t* der, size_t der_size)
{
    if (_rsa_private_key_valid) {
        mbedtls_pk_free(&_rsa_private_key);
    }
    mbedtls_pk_init(&_rsa_private_key);
    _rsa_private_key_valid = true;

    // Starting with mbedtls v3, the function to parse private keys needs a PRNG source.
    int err = mbedtls_pk_parse_key(&_rsa_private_key, der, der_size, nullptr, 0
#if MBEDTLS_VERSION_MAJOR >= 3
                                   , mbedtls_ctr_drbg_random, &_ctr_drbg
#endif
                                   );
    mbed_fatal(err, "error loading RSA private key DER data");

    if (mbedtls_pk_get_type(&_rsa_private_key) != MBEDTLS_PK_RSA || mbedtls_rsa_check_privkey(mbedtls_pk_rsa(_rsa_private_key)) != 0) {
        sys::fatal(name() + ": not a private RSA key");
    }
}

//----------------------------------------------------------------------------
// Load RSA public key from DER data.
//----------------------------------------------------------------------------

void lib_mbedtls::load_rsa_public_key_der(const uint8_t* der, size_t der_size)
{
    if (_rsa_public_key_valid) {
        mbedtls_pk_free(&_rsa_public_key);
    }
    mbedtls_pk_init(&_rsa_public_key);
    _rsa_public_key_valid = true;

    int err = mbedtls_pk_parse_public_key(&_rsa_public_key, der, der_size);
    mbed_fatal(err, "error parsing RSA public key DER data");

    if (mbedtls_pk_get_type(&_rsa_public_key) != MBEDTLS_PK_RSA || mbedtls_rsa_check_pubkey(mbedtls_pk_rsa(_rsa_public_key)) != 0) {
        sys::fatal(name() + ": not a public RSA key");
    }
}

//----------------------------------------------------------------------------
// Current RSA key sizes.
//----------------------------------------------------------------------------

size_t lib_mbedtls::rsa_private_key_bits() const
{
    return _rsa_private_key_valid ? mbedtls_pk_get_bitlen(&_rsa_private_key) : 0;
}

size_t lib_mbedtls::rsa_public_key_bits() const
{
    return _rsa_public_key_valid ? mbedtls_pk_get_bitlen(&_rsa_public_key) : 0;
}


//----------------------------------------------------------------------------
// Set padding and hash on an RSA key context.
//----------------------------------------------------------------------------

void lib_mbedtls::set_rsa_padding(mbedtls_pk_context& key, int padding, mbedtls_md_type_t hash_id)
{
#if MBEDTLS_VERSION_MAJOR >= 3
    int err =
#endif
    mbedtls_rsa_set_padding(mbedtls_pk_rsa(key), padding, hash_id);
#if MBEDTLS_VERSION_MAJOR >= 3
    mbed_fatal(err, "error setting padding on RSA key");
#endif
}

//----------------------------------------------------------------------------
// RSA encrypt/decrypt.
//----------------------------------------------------------------------------

size_t lib_mbedtls::rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    set_rsa_padding(_rsa_public_key, MBEDTLS_RSA_PKCS_V21, MBEDTLS_MD_SHA256);
    size_t output_len = 0;
    int err = mbedtls_pk_encrypt(&_rsa_public_key, input, input_size, output, &output_len, output_maxsize, mbedtls_ctr_drbg_random, &_ctr_drbg);
    mbed_fatal(err, "error in mbedtls_pk_encrypt");
    return output_len;
}

size_t lib_mbedtls::rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    set_rsa_padding(_rsa_private_key, MBEDTLS_RSA_PKCS_V21, MBEDTLS_MD_SHA256);
    size_t output_len = 0;
    int err = mbedtls_pk_decrypt(&_rsa_private_key, input, input_size, output, &output_len, output_maxsize, mbedtls_ctr_drbg_random, &_ctr_drbg);
    mbed_fatal(err, "error in mbedtls_pk_decrypt");
    return output_len;
}

//----------------------------------------------------------------------------
// RSA sign/verify.
//----------------------------------------------------------------------------

size_t lib_mbedtls::rsa_sign(const uint8_t* msg, size_t msg_size, uint8_t* sig, size_t sig_maxsize)
{
    // Use PKCS#1 v1.5 padding.
    // With MbedTLS v2, mbedtls_pk_sign_ext() does not exist (but mbedtls_pk_verify_ext() exists!)
    // With MbedTLS v3, trying to use PSS returns MBEDTLS_ERR_PK_BAD_INPUT_DATA:
    // int err = mbedtls_pk_sign_ext(MBEDTLS_PK_RSASSA_PSS, &_rsa_private_key, MBEDTLS_MD_SHA256, msg, msg_size, sig, sig_maxsize, &sig_len, mbedtls_ctr_drbg_random, &_ctr_drbg);

#if MBEDTLS_VERSION_MAJOR >= 3
    size_t sig_len = 0;
    int err = mbedtls_pk_sign(&_rsa_private_key, MBEDTLS_MD_SHA256, msg, msg_size, sig, sig_maxsize, &sig_len, mbedtls_ctr_drbg_random, &_ctr_drbg);
#else
    size_t sig_len = sig_maxsize;
    int err = mbedtls_pk_sign(&_rsa_private_key, MBEDTLS_MD_SHA256, msg, msg_size, sig, &sig_len, mbedtls_ctr_drbg_random, &_ctr_drbg);
#endif
    mbed_fatal(err, "error in mbedtls_pk_sign");
    return sig_len;
}

bool lib_mbedtls::rsa_verify(const uint8_t* msg, size_t msg_size, const uint8_t* sig, size_t sig_size)
{
    // Equivalent with PSS, if we can make it work:
    // int err = mbedtls_pk_verify_ext(MBEDTLS_PK_RSASSA_PSS, nullptr, &_rsa_public_key, MBEDTLS_MD_SHA256, msg, msg_size, sig, sig_size);

    int err = mbedtls_pk_verify(&_rsa_public_key, MBEDTLS_MD_SHA256, msg, msg_size, sig, sig_size);
    if (err == MBEDTLS_ERR_RSA_VERIFY_FAILED) {
        return false;
    }
    else {
        mbed_fatal(err, "error in mbedtls_pk_verify");
        return true;
    }
}

//----------------------------------------------------------------------------
// AES for encrypt or decrypt.
//----------------------------------------------------------------------------

size_t lib_mbedtls::aes(mbedtls_operation_t op, bool cbc,
                        const uint8_t* key, size_t key_size,
                        const uint8_t* iv, size_t iv_size,
                        const uint8_t* input, size_t input_size,
                        uint8_t* output, size_t output_maxsize)
{
    mbedtls_cipher_type_t type = MBEDTLS_CIPHER_NONE;
    if (cbc) {
        switch (key_size) {
            case 128/8: type = MBEDTLS_CIPHER_AES_128_CBC; break;
            case 192/8: type = MBEDTLS_CIPHER_AES_192_CBC; break;
            case 256/8: type = MBEDTLS_CIPHER_AES_256_CBC; break;
            default: sys::fatal(sys::format("%s: invalid key size %zu bits", name().c_str(), key_size * 8));
        }
    }
    else {
        switch (key_size) {
            case 128/8: type = MBEDTLS_CIPHER_AES_128_ECB; break;
            case 192/8: type = MBEDTLS_CIPHER_AES_192_ECB; break;
            case 256/8: type = MBEDTLS_CIPHER_AES_256_ECB; break;
            default: sys::fatal(sys::format("%s: invalid key size %zu bits", name().c_str(), key_size * 8));
        }
    }

    const mbedtls_cipher_info_t* cipher_info = mbedtls_cipher_info_from_type(type);
    if (cipher_info == nullptr) {
        sys::fatal(sys::format("error in mbedtls_cipher_info_from_type, type: %d", int(type)));
    }

    mbedtls_cipher_context_t cipher;
    mbedtls_cipher_init(&cipher);

    int err = mbedtls_cipher_setup(&cipher, cipher_info);
    mbed_fatal(err, sys::format("error in mbedtls_cipher_setup, type: %d", int(type)));

    err = mbedtls_cipher_setkey(&cipher, key, key_size * 8, op);
    mbed_fatal(err, sys::format("error in mbedtls_cipher_setkey, type: %d, key_size: %zu bits", int(type), key_size * 8));

    if (cbc) {
        err =  mbedtls_cipher_set_padding_mode(&cipher, MBEDTLS_PADDING_NONE);
        mbed_fatal(err, "error in mbedtls_cipher_set_padding_mode");
    }

    size_t output_len = output_maxsize;
    err = mbedtls_cipher_crypt(&cipher, iv, iv_size, input, input_size, output, &output_len);
    mbed_fatal(err, "error in mbedtls_cipher_crypt");

    mbedtls_cipher_free(&cipher);
    return output_len;
}

//----------------------------------------------------------------------------
// AES-ECB encryption.
//----------------------------------------------------------------------------

size_t lib_mbedtls::aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    return aes(MBEDTLS_ENCRYPT, false, key, key_size, nullptr, 0, input, input_size, output, output_maxsize);
}

//----------------------------------------------------------------------------
// AES-ECB decryption.
//----------------------------------------------------------------------------

size_t lib_mbedtls::aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    return aes(MBEDTLS_DECRYPT, false, key, key_size, nullptr, 0, input, input_size, output, output_maxsize);
}

//----------------------------------------------------------------------------
// AES-CBC encryption.
//----------------------------------------------------------------------------

size_t lib_mbedtls::aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);
    return aes(MBEDTLS_ENCRYPT, true, key, key_size, iv, iv_size, input, input_size, output, output_maxsize);
}

//----------------------------------------------------------------------------
// AES-CBC decryption.
//----------------------------------------------------------------------------

size_t lib_mbedtls::aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);
    return aes(MBEDTLS_DECRYPT, true, key, key_size, iv, iv_size, input, input_size, output, output_maxsize);
}
