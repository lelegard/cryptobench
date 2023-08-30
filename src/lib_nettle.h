//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Nettle cryptographic library.
//----------------------------------------------------------------------------

#pragma once
#include "lib.h"
#include <nettle/rsa.h>
#include <nettle/aes.h>
#include <nettle/cbc.h>
#include <nettle/version.h>

// Cleanup poor defines in nettle header files.
// The actual
#if defined(rsa_public_key_init)
#undef rsa_public_key_init
#endif
#if defined(rsa_public_key_clear)
#undef rsa_public_key_clear
#endif
#if defined(rsa_public_key_prepare)
#undef rsa_public_key_prepare
#endif
#if defined(rsa_private_key_init)
#undef rsa_private_key_init
#endif
#if defined(rsa_private_key_clear)
#undef rsa_private_key_clear
#endif
#if defined(rsa_private_key_prepare)
#undef rsa_private_key_prepare
#endif
#if defined(rsa_pkcs1_verify)
#undef rsa_pkcs1_verify
#endif
#if defined(rsa_pkcs1_sign)
#undef rsa_pkcs1_sign
#endif
#if defined(rsa_pkcs1_sign_tr)
#undef rsa_pkcs1_sign_tr
#endif
#if defined(rsa_md5_sign)
#undef rsa_md5_sign
#endif
#if defined(rsa_md5_sign_tr)
#undef rsa_md5_sign_tr
#endif
#if defined(rsa_md5_verify)
#undef rsa_md5_verify
#endif
#if defined(rsa_sha1_sign)
#undef rsa_sha1_sign
#endif
#if defined(rsa_sha1_sign_tr)
#undef rsa_sha1_sign_tr
#endif
#if defined(rsa_sha1_verify)
#undef rsa_sha1_verify
#endif
#if defined(rsa_sha256_sign)
#undef rsa_sha256_sign
#endif
#if defined(rsa_sha256_sign_tr)
#undef rsa_sha256_sign_tr
#endif
#if defined(rsa_sha256_verify)
#undef rsa_sha256_verify
#endif
#if defined(rsa_sha512_sign)
#undef rsa_sha512_sign
#endif
#if defined(rsa_sha512_sign_tr)
#undef rsa_sha512_sign_tr
#endif
#if defined(rsa_sha512_verify)
#undef rsa_sha512_verify
#endif
#if defined(rsa_md5_sign_digest)
#undef rsa_md5_sign_digest
#endif
#if defined(rsa_md5_sign_digest_tr)
#undef rsa_md5_sign_digest_tr
#endif
#if defined(rsa_md5_verify_digest)
#undef rsa_md5_verify_digest
#endif
#if defined(rsa_sha1_sign_digest)
#undef rsa_sha1_sign_digest
#endif
#if defined(rsa_sha1_sign_digest_tr)
#undef rsa_sha1_sign_digest_tr
#endif
#if defined(rsa_sha1_verify_digest)
#undef rsa_sha1_verify_digest
#endif
#if defined(rsa_sha256_sign_digest)
#undef rsa_sha256_sign_digest
#endif
#if defined(rsa_sha256_sign_digest_tr)
#undef rsa_sha256_sign_digest_tr
#endif
#if defined(rsa_sha256_verify_digest)
#undef rsa_sha256_verify_digest
#endif
#if defined(rsa_sha512_sign_digest)
#undef rsa_sha512_sign_digest
#endif
#if defined(rsa_sha512_sign_digest_tr)
#undef rsa_sha512_sign_digest_tr
#endif
#if defined(rsa_sha512_verify_digest)
#undef rsa_sha512_verify_digest
#endif
#if defined(rsa_pss_sha256_sign_digest_tr)
#undef rsa_pss_sha256_sign_digest_tr
#endif
#if defined(rsa_pss_sha256_verify_digest)
#undef rsa_pss_sha256_verify_digest
#endif
#if defined(rsa_pss_sha384_sign_digest_tr)
#undef rsa_pss_sha384_sign_digest_tr
#endif
#if defined(rsa_pss_sha384_verify_digest)
#undef rsa_pss_sha384_verify_digest
#endif
#if defined(rsa_pss_sha512_sign_digest_tr)
#undef rsa_pss_sha512_sign_digest_tr
#endif
#if defined(rsa_pss_sha512_verify_digest)
#undef rsa_pss_sha512_verify_digest
#endif
#if defined(rsa_encrypt)
#undef rsa_encrypt
#endif
#if defined(rsa_decrypt)
#undef rsa_decrypt
#endif
#if defined(rsa_decrypt_tr)
#undef rsa_decrypt_tr
#endif
#if defined(rsa_sec_decrypt)
#undef rsa_sec_decrypt
#endif
#if defined(rsa_compute_root)
#undef rsa_compute_root
#endif
#if defined(rsa_compute_root_tr)
#undef rsa_compute_root_tr
#endif
#if defined(rsa_generate_keypair)
#undef rsa_generate_keypair
#endif
#if defined(rsa_keypair_to_sexp)
#undef rsa_keypair_to_sexp
#endif
#if defined(rsa_keypair_from_sexp_alist)
#undef rsa_keypair_from_sexp_alist
#endif
#if defined(rsa_keypair_from_sexp)
#undef rsa_keypair_from_sexp
#endif
#if defined(rsa_public_key_from_der_iterator)
#undef rsa_public_key_from_der_iterator
#endif
#if defined(rsa_private_key_from_der_iterator)
#undef rsa_private_key_from_der_iterator
#endif
#if defined(rsa_keypair_from_der)
#undef rsa_keypair_from_der
#endif
#if defined(rsa_keypair_to_openpgp)
#undef rsa_keypair_to_openpgp
#endif
#if defined(aes_set_encrypt_key)
#undef aes_set_encrypt_key
#endif
#if defined(aes_set_decrypt_key)
#undef aes_set_decrypt_key
#endif
#if defined(aes_invert_key)
#undef aes_invert_key
#endif
#if defined(aes_encrypt)
#undef aes_encrypt
#endif
#if defined(aes_decrypt)
#undef aes_decrypt
#endif
#if defined(aes128_set_encrypt_key)
#undef aes128_set_encrypt_key
#endif
#if defined(aes128_set_decrypt_key)
#undef aes128_set_decrypt_key
#endif
#if defined(aes128_invert_key)
#undef aes128_invert_key
#endif
#if defined(aes128_encrypt)
#undef aes128_encrypt
#endif
#if defined(aes128_decrypt)
#undef aes128_decrypt
#endif
#if defined(aes192_set_encrypt_key)
#undef aes192_set_encrypt_key
#endif
#if defined(aes192_set_decrypt_key)
#undef aes192_set_decrypt_key
#endif
#if defined(aes192_invert_key)
#undef aes192_invert_key
#endif
#if defined(aes192_encrypt)
#undef aes192_encrypt
#endif
#if defined(aes192_decrypt)
#undef aes192_decrypt
#endif
#if defined(aes256_set_encrypt_key)
#undef aes256_set_encrypt_key
#endif
#if defined(aes256_set_decrypt_key)
#undef aes256_set_decrypt_key
#endif
#if defined(aes256_invert_key)
#undef aes256_invert_key
#endif
#if defined(aes256_encrypt)
#undef aes256_encrypt
#endif
#if defined(aes256_decrypt)
#undef aes256_decrypt
#endif
#if defined(md5_init)
#undef md5_init
#endif
#if defined(md5_update)
#undef md5_update
#endif
#if defined(md5_digest)
#undef md5_digest
#endif
#if defined(md5_compress)
#undef md5_compress
#endif
#if defined(sha1_init)
#undef sha1_init
#endif
#if defined(sha1_update)
#undef sha1_update
#endif
#if defined(sha1_digest)
#undef sha1_digest
#endif
#if defined(sha1_compress)
#undef sha1_compress
#endif
#if defined(sha224_init)
#undef sha224_init
#endif
#if defined(sha224_digest)
#undef sha224_digest
#endif
#if defined(sha256_init)
#undef sha256_init
#endif
#if defined(sha256_update)
#undef sha256_update
#endif
#if defined(sha256_digest)
#undef sha256_digest
#endif
#if defined(sha256_compress)
#undef sha256_compress
#endif
#if defined(sha384_init)
#undef sha384_init
#endif
#if defined(sha384_digest)
#undef sha384_digest
#endif
#if defined(sha512_init)
#undef sha512_init
#endif
#if defined(sha512_update)
#undef sha512_update
#endif
#if defined(sha512_digest)
#undef sha512_digest
#endif
#if defined(sha512_compress)
#undef sha512_compress
#endif
#if defined(sha512_224_init  )
#undef sha512_224_init
#endif
#if defined(sha512_224_digest)
#undef sha512_224_digest
#endif
#if defined(sha512_256_init  )
#undef sha512_256_init
#endif
#if defined(sha512_256_digest)
#undef sha512_256_digest
#endif
#if defined(sha3_permute)
#undef sha3_permute
#endif
#if defined(sha3_224_init)
#undef sha3_224_init
#endif
#if defined(sha3_224_update)
#undef sha3_224_update
#endif
#if defined(sha3_224_digest)
#undef sha3_224_digest
#endif
#if defined(sha3_256_init)
#undef sha3_256_init
#endif
#if defined(sha3_256_update)
#undef sha3_256_update
#endif
#if defined(sha3_256_digest)
#undef sha3_256_digest
#endif
#if defined(sha3_256_shake)
#undef sha3_256_shake
#endif
#if defined(sha3_384_init)
#undef sha3_384_init
#endif
#if defined(sha3_384_update)
#undef sha3_384_update
#endif
#if defined(sha3_384_digest)
#undef sha3_384_digest
#endif
#if defined(sha3_512_init)
#undef sha3_512_init
#endif
#if defined(sha3_512_update)
#undef sha3_512_update
#endif
#if defined(sha3_512_digest)
#undef sha3_512_digest
#endif
#if defined(AES_BLOCK_SIZE)
#undef AES_BLOCK_SIZE
#endif
#if defined(AES128_KEY_SIZE)
#undef AES128_KEY_SIZE
#endif
#if defined(AES192_KEY_SIZE)
#undef AES192_KEY_SIZE
#endif
#if defined(AES256_KEY_SIZE)
#undef AES256_KEY_SIZE
#endif
#if defined(cbc_encrypt)
#undef cbc_encrypt
#endif
#if defined(cbc_decrypt)
#undef cbc_decrypt
#endif
#if defined(cbc_aes128_encrypt)
#undef cbc_aes128_encrypt
#endif
#if defined(cbc_aes192_encrypt)
#undef cbc_aes192_encrypt
#endif
#if defined(cbc_aes256_encrypt)
#undef cbc_aes256_encrypt
#endif

class lib_nettle: public lib
{
public:
    // Constructor / destructor.
    lib_nettle();
    ~lib_nettle();

    // Initializes and cleanup cryptographic library.
    static void init();
    static void cleanup();

    // Important: Nettle cannot do OAEP on RSA encryption, only PKCS#1 v1.5 padding.
    // rsa_encrypt() and rsa_decrypt() use PKCS#1 v1.5 padding.

    // Implementation of "lib" interface.
    virtual std::string version() const override;
    virtual bool rsa_available() const override;
    virtual bool aes_available() const override;
    virtual void load_rsa_private_key(const std::string& filename) override;
    virtual void load_rsa_public_key(const std::string& filename) override;
    virtual size_t rsa_private_key_bits() const override;
    virtual size_t rsa_public_key_bits() const override;
    virtual size_t rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;
    virtual size_t aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize) override;

private:
    bool            _rsa_public_key_valid = false;
    rsa_public_key  _rsa_public_key;
    bool            _rsa_private_key_valid = false;
    rsa_private_key _rsa_private_key;
};
