//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// LibTomCrypt cryptographic library.
//----------------------------------------------------------------------------

#include "lib_tomcrypt.h"

// Static members:
int lib_tomcrypt::_aes_index = -1;
int lib_tomcrypt::_sha256_index = -1;
int lib_tomcrypt::_yarrow_prng_index = -1;
bool lib_tomcrypt::_yarrow_prng_valid = false;
prng_state lib_tomcrypt::_yarrow_prng;

const ltc_math_descriptor* const lib_tomcrypt::_ltm_desc =
#if defined(LTM_DESC)
    &ltm_desc;
#else
    nullptr;
#endif

const ltc_math_descriptor* const lib_tomcrypt::_gmp_desc =
#if defined(GMP_DESC)
    &gmp_desc;
#else
    nullptr;
#endif

//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

lib_tomcrypt::lib_tomcrypt(bool use_gmp) :
    lib(use_gmp ? "tomcrypt-gmp" : "tomcrypt"),
    _math_desc(use_gmp ? _gmp_desc : _ltm_desc)
{
}

//----------------------------------------------------------------------------
// Destructor.
//----------------------------------------------------------------------------

lib_tomcrypt::~lib_tomcrypt()
{
    rsa_cleanup();
}

//----------------------------------------------------------------------------
// Set the multi-precision calculus backend.
//----------------------------------------------------------------------------

void lib_tomcrypt::set_mp() const
{
    if (_math_desc == nullptr) {
        sys::fatal(name() + ": no math backend");
    }
    else if (ltc_mp.invmod != _math_desc->invmod) {
        // The descriptor is a complete structure, not a simple pointer.
        // Copy it only when necessary.
        ltc_mp = *_math_desc;
    }
}

//----------------------------------------------------------------------------
// Check if RSA and AES are available.
//----------------------------------------------------------------------------

bool lib_tomcrypt::rsa_available() const
{
    return _math_desc != nullptr;
}

bool lib_tomcrypt::aes_available() const
{
    // AES is the same, regardless of the math backend. No need to run the AES
    // tests on all variants. Run AES tests only with the default math backend.
    return _math_desc == _ltm_desc;
}

//----------------------------------------------------------------------------
// LibTomCrypt initialization.
//----------------------------------------------------------------------------

void lib_tomcrypt::init()
{
    register_all_ciphers();
    register_all_hashes();
    register_all_prngs();

    _aes_index = tom_find_cipher("aes");
    _sha256_index = tom_find_hash("sha256");
    _yarrow_prng_index = tom_find_prng("yarrow");

    // Initialize the PRNG.
    int err = yarrow_start(&_yarrow_prng);
    tom_fatal(err, "error in yarrow_start");
    _yarrow_prng_valid = true;

    // Preload the PRNG state until ready.
    if (prng_descriptor[_yarrow_prng_index].ready(&_yarrow_prng) != CRYPT_OK) {
        FILE* fp = ::fopen("/dev/random", "r");
        uint8_t data[16];
        if (fp == NULL) {
            sys::fatal("error opening /dev/random", errno);
        }
        while (prng_descriptor[_yarrow_prng_index].ready(&_yarrow_prng) != CRYPT_OK) {
            size_t size = ::fread(data, 1, sizeof(data), fp);
            int err = prng_descriptor[_yarrow_prng_index].add_entropy(data, size, &_yarrow_prng);
            tom_fatal(err, "error in prng_add_entropy");
        }
        ::fclose(fp);
    }
}

//----------------------------------------------------------------------------
// LibTomCrypt cleanup.
//----------------------------------------------------------------------------

void lib_tomcrypt::cleanup()
{
    if (_yarrow_prng_valid) {
        yarrow_done(&_yarrow_prng);
        _yarrow_prng_valid = false;
    }
}

//----------------------------------------------------------------------------
// Cryptographic library version.
//----------------------------------------------------------------------------

std::string lib_tomcrypt::version() const
{
    return SCRYPT;
}

//----------------------------------------------------------------------------
// Print LibTomCrypt errors if not success and exit.
//----------------------------------------------------------------------------

void lib_tomcrypt::tom_fatal(int err, const std::string& message)
{
    if (err != CRYPT_OK) {
        if (!message.empty()) {
            std::cerr << "tomcrypt: " << message << std::endl;
        }
        std::cerr << "tomcrypt: error " << err << ": " << error_to_string(err) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

//----------------------------------------------------------------------------
// Find a cipher, hash or prng. Fail on error.
//----------------------------------------------------------------------------

int lib_tomcrypt::tom_find_cipher(const std::string& str)
{
    const int index = find_cipher(str.c_str());
    if (index < 0) {
        sys::fatal("tomcrypt: cipher \"" + str + "\" not found");
    }
    return index;
}

int lib_tomcrypt::tom_find_hash(const std::string& str)
{
    const int index = find_hash(str.c_str());
    if (index < 0) {
        sys::fatal("tomcrypt: hash \"" + str + "\" not found");
    }
    return index;
}

int lib_tomcrypt::tom_find_prng(const std::string& str)
{
    const int index = find_prng(str.c_str());
    if (index < 0) {
        sys::fatal("tomcrypt: prng \"" + str + "\" not found");
    }
    return index;
}

//----------------------------------------------------------------------------
// Cleanup RSA data in the object.
//----------------------------------------------------------------------------

void lib_tomcrypt::rsa_cleanup()
{
    set_mp();
    if (_rsa_public_key_valid) {
        rsa_free(&_rsa_public_key);
        _rsa_public_key_valid = false;
    }
    if (_rsa_private_key_valid) {
        rsa_free(&_rsa_private_key);
        _rsa_private_key_valid = false;
    }
}

//----------------------------------------------------------------------------
// Load private key from a PEM file.
//----------------------------------------------------------------------------

void lib_tomcrypt::load_rsa_private_key(const std::string& filename)
{
    set_mp();

    // Read the PEM file for the key.
    bytes_t der;
    sys::load_pem_file_as_der(der, filename);

    if (_rsa_private_key_valid) {
        rsa_free(&_rsa_private_key);
    }

    // Inport the RSA key from DER.
    int err = rsa_import_pkcs8(der.data(), der.size(), nullptr, 0, &_rsa_private_key);
    tom_fatal(err, "cannot import RSA private key from " + filename);
    _rsa_private_key_valid = true;
}

//----------------------------------------------------------------------------
// Load public key from a PEM file.
//----------------------------------------------------------------------------

void lib_tomcrypt::load_rsa_public_key(const std::string& filename)
{
    set_mp();

    // Read the PEM file for the key.
    bytes_t der;
    sys::load_pem_file_as_der(der, filename);

    if (_rsa_public_key_valid) {
        rsa_free(&_rsa_public_key);
    }

    // Inport the RSA key from DER.
    int err = rsa_import(der.data(), der.size(), &_rsa_public_key);
    tom_fatal(err, "cannot import RSA public key from " + filename);
    _rsa_public_key_valid = true;
}

//----------------------------------------------------------------------------
// Current RSA key sizes.
//----------------------------------------------------------------------------

size_t lib_tomcrypt::rsa_private_key_bits() const
{
    set_mp();
    return 8 * rsa_get_size(const_cast<rsa_key*>(&_rsa_private_key));
}

size_t lib_tomcrypt::rsa_public_key_bits() const
{
    set_mp();
    return 8 * rsa_get_size(const_cast<rsa_key*>(&_rsa_public_key));
}


//----------------------------------------------------------------------------
// Initialize RSA context for AOEP encrypt.
//----------------------------------------------------------------------------

void lib_tomcrypt::rsa_init_encrypt_oaep()
{
    // Nothing to do.
}

//----------------------------------------------------------------------------
// Initialize RSA context for AOEP decrypt.
//----------------------------------------------------------------------------

void lib_tomcrypt::rsa_init_decrypt_oaep()
{
    // Nothing to do.
}

//----------------------------------------------------------------------------
// RSA encrypt, according to current mode.
//----------------------------------------------------------------------------

size_t lib_tomcrypt::rsa_encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    set_mp();
    unsigned long output_len = output_maxsize;
    int err = rsa_encrypt_key(input, input_size, output, &output_len, nullptr, 0, &_yarrow_prng, _yarrow_prng_index, _sha256_index, &_rsa_public_key);
    tom_fatal(err, "error in rsa_encrypt_key");
    return output_len;
}

//----------------------------------------------------------------------------
// RSA decrypt, according to current mode.
//----------------------------------------------------------------------------

size_t lib_tomcrypt::rsa_decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    set_mp();
    int stat = 0;
    unsigned long output_len = output_maxsize;
    int err = rsa_decrypt_key(input, input_size, output, &output_len, nullptr, 0, _sha256_index, &stat, &_rsa_private_key);
    tom_fatal(err, "error in rsa_decrypt_key");
    return output_len;
}

//----------------------------------------------------------------------------
// AES-ECB encryption.
//----------------------------------------------------------------------------

size_t lib_tomcrypt::aes_encrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    symmetric_key skey;
    int err = aes_setup(key, key_size, 0, &skey);
    tom_fatal(err, "error in aes_setup");
    err = aes_ecb_encrypt(input, output, &skey);
    tom_fatal(err, "error in aes_ecb_encrypt");
    aes_done(&skey);
    return AES_BLOCK_SIZE;
}

//----------------------------------------------------------------------------
// AES-ECB decryption.
//----------------------------------------------------------------------------

size_t lib_tomcrypt::aes_decrypt(const uint8_t* key, size_t key_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_size(key_size, input_size, output_maxsize);
    symmetric_key skey;
    int err = aes_setup(key, key_size, 0, &skey);
    tom_fatal(err, "error in aes_setup");
    err = aes_ecb_decrypt(input, output, &skey);
    tom_fatal(err, "error in aes_ecb_encrypt");
    aes_done(&skey);
    return AES_BLOCK_SIZE;
}

//----------------------------------------------------------------------------
// AES-CBC encryption.
//----------------------------------------------------------------------------

size_t lib_tomcrypt::aes_encrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);
    symmetric_CBC cbc;
    int err = cbc_start(_aes_index, iv, key, key_size, 0, &cbc);
    tom_fatal(err, "error in cbc_start");
    err = cbc_encrypt(input, output, input_size, &cbc);
    tom_fatal(err, "error in cbc_encrypt");
    err = cbc_done(&cbc);
    tom_fatal(err, "error in cbc_done");
    return input_size;
}

//----------------------------------------------------------------------------
// AES-CBC decryption.
//----------------------------------------------------------------------------

size_t lib_tomcrypt::aes_decrypt_cbc(const uint8_t* key, size_t key_size, const uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    check_aes_cbc_size(key_size, iv_size, input_size, output_maxsize);
    symmetric_CBC cbc;
    int err = cbc_start(_aes_index, iv, key, key_size, 0, &cbc);
    tom_fatal(err, "error in cbc_start");
    err = cbc_decrypt(input, output, input_size, &cbc);
    tom_fatal(err, "error in cbc_encrypt");
    err = cbc_done(&cbc);
    tom_fatal(err, "error in cbc_done");
    return input_size;
}
