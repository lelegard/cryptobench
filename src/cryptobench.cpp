//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Main application for cryptographic benchmark.
//----------------------------------------------------------------------------

#include "options.h"
#include "bench_reference.h"
#include "bench_rsa.h"
#include "bench_aes.h"
#include "lib_openssl.h"
#include "lib_mbedtls.h"
#include "lib_gnutls.h"
#include "lib_nettle.h"
#include "lib_tomcrypt.h"

// Score adjustment factors, to give readable results.
#define RSA_SCORE_FACTOR 1000.0
#define AES_SCORE_FACTOR 1000.0

//----------------------------------------------------------------------------
// Run one RSA benchmark.
//----------------------------------------------------------------------------

void run_rsa(std::ostream& out, const options& opt, const bench& reference, lib& crypto, const std::string& privkey, const std::string& pubkey)
{
    crypto.load_rsa_public_key(pubkey);
    crypto.load_rsa_private_key(privkey);

    crypto.rsa_auto_test();
    out << crypto.name() << ": " << crypto.rsa_name() << ": auto-test passed" << std::endl;

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

    // Encryption and decryption performance.
    bench_rsa_encrypt brsa_encrypt(crypto, opt.min_usec, opt.min_iterations, plain, sizeof(plain));
    brsa_encrypt.run();
    brsa_encrypt.display(out, &reference, RSA_SCORE_FACTOR);

    bench_rsa_decrypt brsa_decrypt(crypto, opt.min_usec, opt.min_iterations, encrypted, encrypted_len);
    brsa_decrypt.run();
    brsa_decrypt.display(out, &reference, RSA_SCORE_FACTOR);

    out << crypto.name() << ": " << crypto.rsa_name() << ": decrypt/encrypt ratio: " << brsa_decrypt.score_string(brsa_encrypt) << std::endl;
}

//----------------------------------------------------------------------------
// Run one AES benchmark.
//----------------------------------------------------------------------------

void run_aes(std::ostream& out, const options& opt, const bench& reference, lib& crypto, size_t key_bits)
{
    crypto.aes_auto_test();
    out << crypto.name() << ": aes: auto-test passed" << std::endl;

    bench_aes_encrypt baes_encrypt(crypto, opt.min_usec, opt.min_iterations, key_bits, opt.aes_data_size);
    baes_encrypt.run();
    baes_encrypt.display(out, &reference, AES_SCORE_FACTOR);

    bench_aes_decrypt baes_decrypt(crypto, opt.min_usec, opt.min_iterations, key_bits, opt.aes_data_size);
    baes_decrypt.run();
    baes_decrypt.display(out, &reference, AES_SCORE_FACTOR);

    out << crypto.name() << ": " << crypto.aes_name() << ": encrypt/decrypt ratio: " << baes_encrypt.score_string(baes_decrypt) << std::endl;
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

    // Library versions.
    if (!tested_libs.empty()) {
        for (auto tl : tested_libs) {
            out << tl->name() << ": " << tl->version() << std::endl;
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

    // Run all RSA tests.
    if (opt.rsa) {
        // Generate RSA key pairs, 2048 and 4096 bits.
        const int64_t gen_2048_usec = lib_openssl::generate_rsa_key(2048, opt.private_key_2048, opt.public_key_2048);
        const int64_t gen_4096_usec = lib_openssl::generate_rsa_key(4096, opt.private_key_4096, opt.public_key_4096);
        out << sys::format("generate-rsa-2048: %'" PRId64 " usec", gen_2048_usec) << std::endl;
        out << sys::format("generate-rsa-4096: %'" PRId64 " usec", gen_4096_usec) << std::endl;
        out << std::endl;

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
