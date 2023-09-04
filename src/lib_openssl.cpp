//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// OpenSSL cryptographic library.
//----------------------------------------------------------------------------

#include "lib_openssl.h"
#include <openssl/opensslv.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#if !defined(OPENSSL_VERSION_MAJOR) // before v3
#define OPENSSL_VERSION_MAJOR (OPENSSL_VERSION_NUMBER >> 28)
#endif

#if !defined(OPENSSL_VERSION_MINOR) // before v3
#define OPENSSL_VERSION_MINOR ((OPENSSL_VERSION_NUMBER >> 20) & 0xFF)
#endif

//----------------------------------------------------------------------------
// Constructor/destructor.
//----------------------------------------------------------------------------

lib_openssl::lib_openssl() :
    lib("openssl")
{
}

lib_openssl::~lib_openssl()
{
    if (_rsa_public_key != nullptr) {
        EVP_PKEY_free(_rsa_public_key);
        _rsa_public_key = nullptr;
    }
    if (_rsa_private_key != nullptr) {
        EVP_PKEY_free(_rsa_private_key);
        _rsa_private_key = nullptr;
    }
    if (_rsa_encrypt_ctx != nullptr) {
        EVP_PKEY_CTX_free(_rsa_encrypt_ctx);
        _rsa_encrypt_ctx = nullptr;
    }
    if (_rsa_decrypt_ctx != nullptr) {
        EVP_PKEY_CTX_free(_rsa_decrypt_ctx);
        _rsa_decrypt_ctx = nullptr;
    }
    if (_rsa_sign_ctx != nullptr) {
        EVP_PKEY_CTX_free(_rsa_sign_ctx);
        _rsa_sign_ctx = nullptr;
    }
    if (_rsa_verify_ctx != nullptr) {
        EVP_PKEY_CTX_free(_rsa_verify_ctx);
        _rsa_verify_ctx = nullptr;
    }
}

//----------------------------------------------------------------------------
// Library initialization/cleanup.
//----------------------------------------------------------------------------

void lib_openssl::init()
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
}

void lib_openssl::cleanup()
{
    EVP_cleanup();
    ERR_free_strings();
}

//----------------------------------------------------------------------------
// Cryptographic library properties.
//----------------------------------------------------------------------------

std::string lib_openssl::version() const
{
#if defined(OPENSSL_FULL_VERSION_STRING) // v3
    return sys::format("%s (%s)", OpenSSL_version(OPENSSL_FULL_VERSION_STRING), OpenSSL_version(OPENSSL_CPU_INFO));
#else
    return OpenSSL_version(OPENSSL_VERSION);
#endif
}

bool lib_openssl::rsa_available() const
{
    return true;
}

bool lib_openssl::aes_available() const
{
    return true;
}

//----------------------------------------------------------------------------
// Print last OpenSSL error and exit.
//----------------------------------------------------------------------------

void lib_openssl::ossl_fatal(const std::string& message)
{
    if (!message.empty()) {
        std::cerr << "openssl: " << message << std::endl;
    }
    ERR_print_errors_fp(stderr);
    std::exit(EXIT_FAILURE);
}

//----------------------------------------------------------------------------
// Generate an RSA key pair and save them as PEM files.
//----------------------------------------------------------------------------

int64_t lib_openssl::generate_rsa_key(size_t bits, const std::string& filename_private, const std::string& filename_public)
{
    // Generate the key, evaluate the CPU time.
    const int64_t time1 = sys::cpu_time();
#if OPENSSL_VERSION_MAJOR >= 3
    EVP_PKEY* key = EVP_RSA_gen(bits);
#else
    // Legacy version
    EVP_PKEY_CTX* ctx = nullptr;
    if ((ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr)) == nullptr) {
        ossl_fatal("EVP_PKEY_CTX_new_id");
    }
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        ossl_fatal("EVP_PKEY_keygen_init");
    }
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
        ossl_fatal("EVP_PKEY_CTX_set_rsa_keygen_bits");
    }
    EVP_PKEY* key = nullptr;
    if (EVP_PKEY_keygen(ctx, &key) <= 0) {
        ossl_fatal("EVP_PKEY_keygen");
    }
    EVP_PKEY_CTX_free(ctx);
#endif
    const int64_t time2 = sys::cpu_time();

    if (key == nullptr) {
        ossl_fatal("EVP_RSA_gen error");
    }

    FILE* fp = nullptr;
    if ((fp = ::fopen(filename_private.c_str(), "w")) == nullptr) {
        sys::fatal("error creating " + filename_private, errno);
    }
    if (PEM_write_PrivateKey(fp, key, nullptr, nullptr, 0, nullptr, nullptr) <= 0) {
        ossl_fatal("error writing private key to " + filename_private);
    }
    fclose(fp);

    if ((fp = ::fopen(filename_public.c_str(), "w")) == nullptr) {
        sys::fatal("error creating " + filename_public, errno);
    }
    if (PEM_write_PUBKEY(fp, key) <= 0) {
        ossl_fatal("error writing public key to " + filename_public);
    }
    fclose(fp);

    EVP_PKEY_free(key);

    return time2 - time1;
}

//----------------------------------------------------------------------------
// Load RSA private key from DER data.
//----------------------------------------------------------------------------

void lib_openssl::load_rsa_private_key_der(const uint8_t* der, size_t der_size)
{
    if (_rsa_private_key != nullptr) {
        EVP_PKEY_free(_rsa_private_key);
    }
    const uint8_t* der_parse = der;
    if ((_rsa_private_key = d2i_PrivateKey(EVP_PKEY_RSA, nullptr, &der_parse, der_size)) == nullptr) {
        ossl_fatal("error loading private key from DER data");
    }
}

//----------------------------------------------------------------------------
// Load RSA public key from DER data.
//----------------------------------------------------------------------------

void lib_openssl::load_rsa_public_key_der(const uint8_t* der, size_t der_size)
{
    if (_rsa_public_key != nullptr) {
        EVP_PKEY_free(_rsa_public_key);
    }
    const uint8_t* der_parse = der;
    if ((_rsa_public_key = d2i_PUBKEY(nullptr, &der_parse, der_size)) == nullptr) {
        ossl_fatal("error loading public key from DER data");
    }
}

//----------------------------------------------------------------------------
// Current RSA key sizes.
//----------------------------------------------------------------------------

size_t lib_openssl::rsa_private_key_bits() const
{
    return _rsa_private_key == nullptr ? 0 : EVP_PKEY_bits(_rsa_private_key);
}

size_t lib_openssl::rsa_public_key_bits() const
{
    return _rsa_public_key == nullptr ? 0 : EVP_PKEY_bits(_rsa_public_key);
}


//----------------------------------------------------------------------------
// Initialize RSA context for AOEP encrypt.
//----------------------------------------------------------------------------

void lib_openssl::rsa_init_encrypt_oaep()
{
    if (_rsa_encrypt_ctx != nullptr) {
        EVP_PKEY_CTX_free(_rsa_encrypt_ctx);
    }
    if ((_rsa_encrypt_ctx = EVP_PKEY_CTX_new(_rsa_public_key, nullptr)) == nullptr) {
        ossl_fatal("error creating EVP_PKEY_CTX for public key");
    }
    if (EVP_PKEY_encrypt_init(_rsa_encrypt_ctx) <= 0) {
        ossl_fatal("encrypt init error on public key");
    }
    if (EVP_PKEY_CTX_set_rsa_padding(_rsa_encrypt_ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        ossl_fatal("error setting padding on public key");
    }
}

//----------------------------------------------------------------------------
// Initialize RSA context for AOEP decrypt.
//----------------------------------------------------------------------------

void lib_openssl::rsa_init_decrypt_oaep()
{
    if (_rsa_decrypt_ctx != nullptr) {
        EVP_PKEY_CTX_free(_rsa_decrypt_ctx);
    }
    if ((_rsa_decrypt_ctx = EVP_PKEY_CTX_new(_rsa_private_key, nullptr)) == nullptr) {
        ossl_fatal("error creating EVP_PKEY_CTX for private key");
    }
    if (EVP_PKEY_decrypt_init(_rsa_decrypt_ctx) <= 0) {
        ossl_fatal("decrypt init error on private key");
    }
    if (EVP_PKEY_CTX_set_rsa_padding(_rsa_decrypt_ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        ossl_fatal("error setting padding on private key");
    }
}

//----------------------------------------------------------------------------
// RSA encrypt.
//----------------------------------------------------------------------------

size_t lib_openssl::rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    size_t output_len = output_maxsize;
    if (EVP_PKEY_encrypt(_rsa_encrypt_ctx, output, &output_len, input, input_size) <= 0) {
        ossl_fatal("RSA encrypt error");
    }
    return output_len;
}

//----------------------------------------------------------------------------
// RSA decrypt.
//----------------------------------------------------------------------------

size_t lib_openssl::rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    size_t output_len = output_maxsize;
    if (EVP_PKEY_decrypt(_rsa_decrypt_ctx, output, &output_len, input, input_size) <= 0) {
        ossl_fatal("RSA decrypt error");
    }
    return output_len;
}

//----------------------------------------------------------------------------
// Initialize RSA context for PSS sign.
//----------------------------------------------------------------------------

void lib_openssl::rsa_init_sign_pss()
{
    if (_rsa_sign_ctx != nullptr) {
        EVP_PKEY_CTX_free(_rsa_sign_ctx);
    }
    if ((_rsa_sign_ctx = EVP_PKEY_CTX_new(_rsa_private_key, nullptr)) == nullptr) {
        ossl_fatal("error creating EVP_PKEY_CTX for private key signature");
    }
    if (EVP_PKEY_sign_init(_rsa_sign_ctx) <= 0) {
        ossl_fatal("sign init error on private key");
    }
    if (EVP_PKEY_CTX_set_rsa_padding(_rsa_sign_ctx, RSA_PKCS1_PSS_PADDING) <= 0) {
        ossl_fatal("error setting padding on private key");
    }
    if (EVP_PKEY_CTX_set_signature_md(_rsa_sign_ctx, EVP_sha256()) <= 0) {
        ossl_fatal("error setting hash on private key signature");
    }
}

//----------------------------------------------------------------------------
// Initialize RSA context for PSS verify.
//----------------------------------------------------------------------------

void lib_openssl::rsa_init_verify_pss()
{
    if (_rsa_verify_ctx != nullptr) {
        EVP_PKEY_CTX_free(_rsa_verify_ctx);
    }
    if ((_rsa_verify_ctx = EVP_PKEY_CTX_new(_rsa_public_key, nullptr)) == nullptr) {
        ossl_fatal("error creating EVP_PKEY_CTX for public key verification");
    }
    if (EVP_PKEY_verify_init(_rsa_verify_ctx) <= 0) {
        ossl_fatal("sign init error on public key");
    }
    if (EVP_PKEY_CTX_set_rsa_padding(_rsa_verify_ctx, RSA_PKCS1_PSS_PADDING) <= 0) {
        ossl_fatal("error setting padding on public key verification");
    }
    if (EVP_PKEY_CTX_set_signature_md(_rsa_verify_ctx, EVP_sha256()) <= 0) {
        ossl_fatal("error setting hash on public key verification");
    }
}

//----------------------------------------------------------------------------
// RSA sign/verify.
//----------------------------------------------------------------------------

size_t lib_openssl::rsa_sign(const uint8_t* msg, size_t msg_size, uint8_t* sig, size_t sig_maxsize)
{
    size_t sig_len = sig_maxsize;
    if (EVP_PKEY_sign(_rsa_sign_ctx, sig, &sig_len, msg, msg_size) <= 0) {
        ossl_fatal("RSA sign error");
    }
    return sig_len;
}

bool lib_openssl::rsa_verify(const uint8_t* msg, size_t msg_size, const uint8_t* sig, size_t sig_size)
{
    // Status: 1=verified, 0=not verified, <0 = error
    const int res = EVP_PKEY_verify(_rsa_verify_ctx, sig, sig_size, msg, msg_size);
    if (res < 0) {
        ossl_fatal("RSA verify error");
    }
    return res > 0;
}

//----------------------------------------------------------------------------
// AES for encrypt or decrypt.
//----------------------------------------------------------------------------

size_t lib_openssl::aes(bool encrypt, bool cbc,
                        const uint8_t* key, size_t key_size,
                        const uint8_t* iv, size_t iv_size,
                        const uint8_t* input, size_t input_size,
                        uint8_t* output, size_t output_maxsize)
{
    const EVP_CIPHER* evp = nullptr;
    if (cbc) {
        switch (key_size) {
            case 128/8: evp = EVP_aes_128_cbc(); break;
            case 192/8: evp = EVP_aes_192_cbc(); break;
            case 256/8: evp = EVP_aes_256_cbc(); break;
            default: sys::fatal(sys::format("%s: invalid key size %zu bits", name().c_str(), key_size * 8));
        }
    }
    else {
        switch (key_size) {
            case 128/8: evp = EVP_aes_128_ecb(); break;
            case 192/8: evp = EVP_aes_192_ecb(); break;
            case 256/8: evp = EVP_aes_256_ecb(); break;
            default: sys::fatal(sys::format("%s: invalid key size %zu bits", name().c_str(), key_size * 8));
        }
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (ctx == nullptr) {
        ossl_fatal("error creating cipher context");
    }

    if (encrypt) {
        if (EVP_EncryptInit(ctx, evp, key, iv) <= 0) {
            ossl_fatal("encrypt init error");
        }
    }
    else {
        if (EVP_DecryptInit(ctx, evp, key, iv) <= 0) {
            ossl_fatal("decrypt init error");
        }
    }

    if (EVP_CIPHER_CTX_set_padding(ctx, 0) <= 0) {
        ossl_fatal("set no padding error");
    }

    int output_len = 0;
    int final_len = 0;
    if (encrypt) {
        if (EVP_EncryptUpdate(ctx, output, &output_len, input, input_size) <= 0) {
            ossl_fatal("encrypt update error");
        }
        if (EVP_EncryptFinal(ctx, output + output_len, &final_len) <= 0) {
            ossl_fatal("encrypt final error");
        }
    }
    else {
        if (EVP_DecryptUpdate(ctx, output, &output_len, input, input_size) <= 0) {
            ossl_fatal("decrypt update error");
        }
        if (EVP_DecryptFinal(ctx, output + output_len, &final_len) <= 0) {
            ossl_fatal("decrypt final error");
        }
    }

    EVP_CIPHER_CTX_free(ctx);

    const size_t total_out = output_len + final_len;
    if (total_out > output_maxsize) {
        sys::fatal(sys::format("buffer overflow in AES encrypt, buffer size: %zu, output size: %zu", output_maxsize, total_out));
    }
    return total_out;
}

//----------------------------------------------------------------------------
// AES-ECB encryption.
//----------------------------------------------------------------------------

size_t lib_openssl::aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    return aes(true, false, key, key_size, nullptr, 0, input, input_size, output, output_maxsize);
}

//----------------------------------------------------------------------------
// AES-ECB decryption.
//----------------------------------------------------------------------------

size_t lib_openssl::aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    return aes(false, false, key, key_size, nullptr, 0, input, input_size, output, output_maxsize);
}

//----------------------------------------------------------------------------
// AES-CBC encryption.
//----------------------------------------------------------------------------

size_t lib_openssl::aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);
    return aes(true, true, key, key_size, iv, iv_size, input, input_size, output, output_maxsize);
}

//----------------------------------------------------------------------------
// AES-CBC decryption.
//----------------------------------------------------------------------------

size_t lib_openssl::aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);
    return aes(false, true, key, key_size, iv, iv_size, input, input_size, output, output_maxsize);
}
