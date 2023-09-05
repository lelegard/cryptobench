//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Main application for cryptographic benchmark.
//----------------------------------------------------------------------------

#include "options.h"
#include "bench_reference.h"
#include "bench_rsa.h"
#include "bench_aes.h"
#include "bench_math.h"
#include "lib_openssl.h"
#include "lib_mbedtls.h"
#include "lib_gnutls.h"
#include "lib_nettle.h"
#include "lib_tomcrypt.h"
#include "lib_arm64.h"

// Score adjustment factors, to give readable results.
#define RSA_SCORE_FACTOR 1000.0
#define AES_SCORE_FACTOR 1000.0
#define MATH_SCORE_FACTOR 1000.0

//----------------------------------------------------------------------------
// Run one RSA benchmark.
//----------------------------------------------------------------------------

void run_rsa(std::ostream& out, const options& opt, const bench& reference, lib& crypto, const std::string& privkey, const std::string& pubkey)
{
    crypto.load_rsa_private_key_file(privkey);
    crypto.load_rsa_public_key_file(pubkey);

    crypto.rsa_auto_test();
    out << crypto.rsa_name() << ": auto-test-passed" << std::endl;

    // Encryption benchmarks.
    crypto.rsa_init_encrypt_oaep();
    crypto.rsa_init_decrypt_oaep();

    // Fake plain text for RSA encryption (typically an AES-256 key).
    static const uint8_t plain[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    };

    // Perform one encryption to get some valid data to decrypt.
    uint8_t encrypted[1024];
    size_t encrypted_len = crypto.rsa_encrypt(plain, sizeof(plain), encrypted, sizeof(encrypted));

    // Encryption and decryption performance. No rekeying.
    bench_rsa_encrypt brsa_encrypt(crypto, opt.min_usec, opt.min_iterations, plain, sizeof(plain), false);
    brsa_encrypt.run();
    brsa_encrypt.display(out, &reference, RSA_SCORE_FACTOR);

    bench_rsa_decrypt brsa_decrypt(crypto, opt.min_usec, opt.min_iterations, encrypted, encrypted_len, false);
    brsa_decrypt.run();
    brsa_decrypt.display(out, &reference, RSA_SCORE_FACTOR);

    out << crypto.rsa_name() << ": decrypt/encrypt-ratio=" << brsa_decrypt.score_string(brsa_encrypt) << std::endl;

    // Encryption and decryption performance. With rekeying.
    bench_rsa_encrypt brsa_encrypt_rekey(crypto, opt.min_usec, opt.min_iterations, plain, sizeof(plain), true);
    brsa_encrypt_rekey.run();
    brsa_encrypt_rekey.display(out, &reference, RSA_SCORE_FACTOR);

    bench_rsa_decrypt brsa_decrypt_rekey(crypto, opt.min_usec, opt.min_iterations, encrypted, encrypted_len, true);
    brsa_decrypt_rekey.run();
    brsa_decrypt_rekey.display(out, &reference, RSA_SCORE_FACTOR);

    out << crypto.rsa_name() << ": decrypt-rekey/encrypt-rekey-ratio=" << brsa_decrypt_rekey.score_string(brsa_encrypt_rekey) << std::endl;

    // Signature benchmarks.
    crypto.rsa_init_sign_pss();
    crypto.rsa_init_verify_pss();

    // Perform one signature to get some valid data to verify.
    uint8_t sig[1024];
    size_t sig_len = crypto.rsa_sign(plain, sizeof(plain), sig, sizeof(sig));

    // Signature and verification performance. No rekeying.
    bench_rsa_sign brsa_sign(crypto, opt.min_usec, opt.min_iterations, plain, sizeof(plain), false);
    brsa_sign.run();
    brsa_sign.display(out, &reference, RSA_SCORE_FACTOR);

    bench_rsa_verify brsa_verify(crypto, opt.min_usec, opt.min_iterations, plain, sizeof(plain), sig, sig_len, false);
    brsa_verify.run();
    brsa_verify.display(out, &reference, RSA_SCORE_FACTOR);

    out << crypto.rsa_name() << ": sign/verify-ratio=" << brsa_sign.score_string(brsa_verify) << std::endl;

    // Signature and verification performance. With rekeying.
    bench_rsa_sign brsa_sign_rekey(crypto, opt.min_usec, opt.min_iterations, plain, sizeof(plain), true);
    brsa_sign_rekey.run();
    brsa_sign_rekey.display(out, &reference, RSA_SCORE_FACTOR);

    bench_rsa_verify brsa_verify_rekey(crypto, opt.min_usec, opt.min_iterations, plain, sizeof(plain), sig, sig_len, true);
    brsa_verify_rekey.run();
    brsa_verify_rekey.display(out, &reference, RSA_SCORE_FACTOR);

    out << crypto.rsa_name() << ": sign-rekey/verify-rekey-ratio=" << brsa_sign_rekey.score_string(brsa_verify_rekey) << std::endl;

    // Key loading performance.
    bytes_t der;
    sys::load_pem_file_as_der(der, pubkey);
    bench_rsa_load_public brsa_rekey_public(crypto, opt.min_usec, opt.min_iterations, der.data(), der.size());
    brsa_rekey_public.run();
    brsa_rekey_public.display(out, &reference, RSA_SCORE_FACTOR);

    sys::load_pem_file_as_der(der, privkey);
    bench_rsa_load_private brsa_rekey_private(crypto, opt.min_usec, opt.min_iterations, der.data(), der.size());
    brsa_rekey_private.run();
    brsa_rekey_private.display(out, &reference, RSA_SCORE_FACTOR);
}

//----------------------------------------------------------------------------
// Run one AES benchmark.
//----------------------------------------------------------------------------

void run_aes(std::ostream& out, const options& opt, const bench& reference, lib& crypto, size_t key_bits)
{
    crypto.aes_auto_test();
    out << crypto.name() << ": aes: auto-test-passed" << std::endl;

    bench_aes_encrypt baes_encrypt(crypto, opt.min_usec, opt.min_iterations, key_bits, opt.aes_data_size);
    baes_encrypt.run();
    baes_encrypt.display(out, &reference, AES_SCORE_FACTOR);

    bench_aes_decrypt baes_decrypt(crypto, opt.min_usec, opt.min_iterations, key_bits, opt.aes_data_size);
    baes_decrypt.run();
    baes_decrypt.display(out, &reference, AES_SCORE_FACTOR);

    out << crypto.aes_name() << ": encrypt/decrypt-ratio=" << baes_encrypt.score_string(baes_decrypt) << std::endl;
}

//----------------------------------------------------------------------------
// Run the math benchmarks. Available only with OpenSSL.
//----------------------------------------------------------------------------

void run_math(std::ostream& out, const options& opt, const bench& reference)
{
    bignum n, e, d;

    lib_openssl::load_rsa_private_key_values(opt.private_key_2048, n.ptr, e.ptr, d.ptr);

    // Same as RSA encrypt.
    bench_math_mod_exp b01(opt.min_usec, opt.min_iterations, e.ptr, n.ptr);
    b01.run();
    b01.display(out, &reference, MATH_SCORE_FACTOR);

    bench_math_mod_exp_simple b02(opt.min_usec, opt.min_iterations, e.ptr, n.ptr);
    b02.run();
    b02.display(out, &reference, MATH_SCORE_FACTOR);

    bench_math_mod_exp_mont b03(opt.min_usec, opt.min_iterations, e.ptr, n.ptr);
    b03.run();
    b03.display(out, &reference, MATH_SCORE_FACTOR);

    // Same as RSA decrypt (without using CRT).
    bench_math_mod_exp b10(opt.min_usec, opt.min_iterations, d.ptr, n.ptr);
    b10.run();
    b10.display(out, &reference, MATH_SCORE_FACTOR);

    bench_math_mod_exp_simple b11(opt.min_usec, opt.min_iterations, d.ptr, n.ptr);
    b11.run();
    b11.display(out, &reference, MATH_SCORE_FACTOR);

    bench_math_mod_exp_mont b13(opt.min_usec, opt.min_iterations, d.ptr, n.ptr);
    b13.run();
    b13.display(out, &reference, MATH_SCORE_FACTOR);
}

//----------------------------------------------------------------------------
// Run all benchmarks.
//----------------------------------------------------------------------------

void run_all_benchmarks(std::ostream& out, const options& opt)
{
    out << std::endl;

    // All supported libraries.
    lib_openssl openssl;
    lib_mbedtls mbedtls;
    lib_gnutls gnutls;
    lib_nettle nettle;
    lib_tomcrypt tomcrypt(false);
    lib_tomcrypt tomcrypt_gmp(true);
    lib_arm64 arm64;

    std::vector<lib*> tested_libs;
    if (opt.openssl) {
        tested_libs.push_back(&openssl);
    }
    if (opt.mbedtls) {
        tested_libs.push_back(&mbedtls);
    }
    if (opt.gnutls) {
        tested_libs.push_back(&gnutls);
    }
    if (opt.nettle) {
        tested_libs.push_back(&nettle);
    }
    if (opt.tomcrypt) {
        tested_libs.push_back(&tomcrypt);
    }
    if (opt.tomcrypt_gmp) {
        tested_libs.push_back(&tomcrypt_gmp);
    }
    if (opt.arm64) {
        tested_libs.push_back(&arm64);
    }

    // Library versions.
    if (!tested_libs.empty()) {
        for (auto tl : tested_libs) {
            out << tl->name() << ": version=" << tl->version() << std::endl;
        }
        out << std::endl;
    }

    // Reference benchmark for the system.
    bench_reference bref(opt.min_usec, opt.min_iterations);
    if (opt.reference) {
        bref.run();
        bref.display(out);
        out << std::endl;
    }

    // Generate RSA key pairs, 2048 and 4096 bits.
    if (opt.rsa || opt.math) {
        const int64_t gen_2048_usec = lib_openssl::generate_rsa_key(2048, opt.private_key_2048, opt.public_key_2048);
        const int64_t gen_4096_usec = lib_openssl::generate_rsa_key(4096, opt.private_key_4096, opt.public_key_4096);
        out << sys::format("openssl: rsa-2048: generate-key-usec=%'" PRId64, gen_2048_usec) << std::endl;
        out << sys::format("openssl: rsa-4096: generate-key-usec=%'" PRId64, gen_4096_usec) << std::endl;
        out << std::endl;
    }

    // Run all RSA tests.
    if (opt.rsa) {
        for (auto tl : tested_libs) {
            if (tl->rsa_available()) {
                run_rsa(out, opt, bref, *tl, opt.private_key_2048, opt.public_key_2048);
                run_rsa(out, opt, bref, *tl, opt.private_key_4096, opt.public_key_4096);
                out << std::endl;
            }
        }
    }

    // Run all AES tests.
    if (opt.aes) {
        for (auto tl : tested_libs) {
            if (tl->aes_available()) {
                run_aes(out, opt, bref, *tl, 128);
                run_aes(out, opt, bref, *tl, 256);
                out << std::endl;
            }
        }
    }

    // Run the math tests.
    if (opt.math) {
        run_math(out, opt, bref);
        out << std::endl;
    }
}

//----------------------------------------------------------------------------
// Application entry point, encapsulate initialization / cleanup.
//----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    sys::init();
    lib_openssl::init();
    lib_mbedtls::init();
    lib_gnutls::init();
    lib_tomcrypt::init();

    run_all_benchmarks(std::cout, options(argc, argv));

    lib_tomcrypt::cleanup();
    lib_gnutls::cleanup();
    lib_mbedtls::cleanup();
    lib_openssl::cleanup();
    return EXIT_SUCCESS;
}
