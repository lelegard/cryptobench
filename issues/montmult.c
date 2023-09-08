//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Standalone Montgomery multiplication test.
//----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/resource.h>
#include <openssl/bn.h>

#define USECPERSEC          1000000  // microseconds per second
#define ADD_ITERATIONS      1000000  // number of iterations for BN_mod_add()
#define MUL_ITERATIONS       500000  // number of iterations for BN_mod_mul()
#define MONT_MUL_ITERATIONS 1000000  // number of iterations for BN_mod_mul_montgomery()

// A 2048-bit number which is an RSA modulus.
static const uint8_t bin_mod[] = {
    0x00,
    0xDE, 0xA0, 0xB7, 0xA4, 0xC2, 0x7C, 0xD3, 0xB2, 0x66, 0x85, 0xAF, 0xAF, 0xB8, 0x01, 0x83, 0xC7,
    0xF0, 0x9F, 0x3D, 0x0D, 0x23, 0x6A, 0xFE, 0xEB, 0xBF, 0x59, 0x84, 0x37, 0xD8, 0xDD, 0xA6, 0xB1,
    0xE4, 0x17, 0x9C, 0xA9, 0xE5, 0x41, 0x7E, 0x73, 0x11, 0xB2, 0xA7, 0xA5, 0x87, 0x32, 0xFA, 0x16,
    0x1D, 0xD2, 0xC8, 0x5B, 0x0A, 0xAC, 0x76, 0x53, 0x34, 0xE6, 0x83, 0xC7, 0x7F, 0x64, 0xD6, 0x6E,
    0x2A, 0x54, 0xD6, 0xEE, 0xC7, 0x2A, 0x8D, 0xF6, 0x4C, 0xA8, 0x9D, 0x42, 0xD8, 0x37, 0xDC, 0x1A,
    0xF0, 0xAD, 0x7D, 0x6D, 0xA1, 0x57, 0x17, 0x60, 0xA0, 0x57, 0xD1, 0x0D, 0xC5, 0x6C, 0xFB, 0xF5,
    0xD4, 0xA3, 0x03, 0xB6, 0xD4, 0xA0, 0xB3, 0xC3, 0x65, 0x04, 0x02, 0x34, 0xDD, 0xC1, 0x3A, 0x6B,
    0x8E, 0xDA, 0x38, 0x56, 0x41, 0xCD, 0xE2, 0x64, 0x46, 0xC5, 0x7E, 0x77, 0x92, 0xCA, 0x26, 0x5C,
    0x2F, 0x75, 0xE6, 0xC4, 0x50, 0x85, 0x04, 0xC2, 0xD9, 0x47, 0xD5, 0x2E, 0xB2, 0x3A, 0x7C, 0x39,
    0x88, 0xF6, 0x0A, 0xF3, 0x5E, 0xAB, 0x80, 0x71, 0xD6, 0x3E, 0xEF, 0x33, 0x36, 0x83, 0xDA, 0xAE,
    0x42, 0xE0, 0x50, 0xB9, 0xF3, 0x17, 0x75, 0x64, 0x27, 0x49, 0xEC, 0x1F, 0x38, 0x42, 0x04, 0xD2,
    0xFF, 0x10, 0xCD, 0xE8, 0x66, 0xC8, 0xBB, 0x07, 0x3A, 0x2F, 0x92, 0xB9, 0xD1, 0x4A, 0x05, 0xF8,
    0x2E, 0xF1, 0x2A, 0xD3, 0x2E, 0x90, 0x4D, 0x20, 0xD7, 0x44, 0x4D, 0xBC, 0x31, 0x87, 0xBC, 0xA5,
    0xA6, 0x79, 0x3D, 0x9A, 0x50, 0x61, 0xAF, 0xFD, 0xD2, 0xF0, 0x18, 0xBF, 0x5D, 0xDE, 0x7A, 0xE8,
    0x27, 0x9C, 0xA1, 0x32, 0xDC, 0xA5, 0x53, 0x9C, 0x27, 0x79, 0xC6, 0x15, 0x3D, 0x0B, 0x68, 0x7F,
    0x6B, 0xA2, 0xFF, 0xE5, 0xD7, 0x4A, 0x8F, 0xF1, 0x8F, 0x63, 0xE5, 0x39, 0x5D, 0xE0, 0xC5, 0xF5
};

// Two random 2047-bit numbers.
static const uint8_t bin_1[] = {
    0x43, 0xFD, 0x87, 0xF5, 0xE1, 0x46, 0x8C, 0x41, 0x84, 0xEA, 0x56, 0x9E, 0x65, 0xE0, 0xB5, 0x96,
    0x70, 0xB9, 0xF9, 0xEB, 0xA3, 0xFF, 0x0B, 0x6B, 0x16, 0x56, 0x99, 0x96, 0x92, 0xF9, 0x9A, 0xA2,
    0x31, 0x4D, 0x77, 0x3F, 0x0D, 0x5B, 0x6D, 0xFA, 0x94, 0x03, 0xFA, 0x32, 0x2A, 0x4A, 0xA9, 0xCF,
    0xD3, 0xD1, 0x49, 0xB3, 0x73, 0xE1, 0xFE, 0xB2, 0x94, 0x5D, 0xF7, 0xC8, 0xD2, 0x91, 0xFE, 0xC0,
    0x81, 0x7F, 0xD0, 0xAD, 0x59, 0x98, 0x72, 0xEA, 0x3A, 0xEF, 0xF0, 0xF9, 0x43, 0xE2, 0xF2, 0x6F,
    0x84, 0x68, 0x7C, 0x57, 0x13, 0x36, 0x77, 0x85, 0x38, 0xDB, 0xDF, 0xBC, 0x57, 0x0D, 0x21, 0x63,
    0x78, 0x9B, 0x3A, 0x4C, 0xC8, 0x80, 0x59, 0x10, 0x56, 0xED, 0xBB, 0xFC, 0x94, 0x65, 0x88, 0xC9,
    0x70, 0xDA, 0xBB, 0x6E, 0x06, 0xA0, 0x72, 0x36, 0xD0, 0x20, 0x08, 0x8A, 0xA9, 0xF1, 0x64, 0xFA,
    0x6C, 0x3F, 0x4D, 0x0A, 0x0A, 0x42, 0xAC, 0x41, 0x43, 0x69, 0x90, 0xF8, 0x54, 0xC8, 0x03, 0xEB,
    0x92, 0x1D, 0xAE, 0x25, 0x1B, 0x46, 0xE0, 0x69, 0xD5, 0x4C, 0x52, 0x6D, 0x74, 0xC1, 0xC7, 0xE5,
    0xB2, 0xC3, 0x67, 0xB2, 0xAC, 0x4C, 0x17, 0x3A, 0x36, 0x99, 0x1A, 0x7B, 0xC6, 0xD3, 0xBB, 0xE3,
    0xC8, 0xF3, 0xAD, 0x60, 0x0A, 0x46, 0xD0, 0xDD, 0x97, 0x89, 0xDD, 0x1E, 0x59, 0x39, 0x12, 0x68,
    0x32, 0x32, 0xBC, 0xD7, 0xC6, 0xED, 0xD5, 0x04, 0x62, 0x1F, 0xAE, 0x68, 0xFA, 0xD4, 0xE1, 0x01,
    0x54, 0xD2, 0xDA, 0xED, 0x8A, 0xF5, 0xA8, 0xFB, 0x3A, 0x4F, 0x02, 0x0A, 0xE0, 0x90, 0x54, 0x04,
    0x7C, 0xAF, 0xEC, 0xC4, 0x92, 0x3D, 0x57, 0x34, 0x4D, 0xD2, 0x20, 0xBE, 0xD9, 0x6F, 0x05, 0xC1,
    0x51, 0x97, 0xE6, 0xA8, 0x0F, 0x9E, 0x4C, 0x7B, 0xFF, 0x9A, 0x8A, 0xCB, 0x41, 0x13, 0x5B, 0xA6,
};
static const uint8_t bin_2[] = {
    0x6A, 0x8B, 0xC1, 0xAD, 0x28, 0x94, 0x17, 0x86, 0xB4, 0x2F, 0x51, 0x5E, 0x52, 0xD8, 0xDE, 0x94,
    0x08, 0xAA, 0x3D, 0xD6, 0x89, 0xA5, 0x6C, 0x71, 0x65, 0xB8, 0x0F, 0x71, 0x99, 0x93, 0x89, 0x86,
    0x6E, 0x50, 0xD6, 0x17, 0x9A, 0xF8, 0x84, 0x0A, 0x9D, 0x7E, 0xE0, 0x6B, 0x64, 0xD1, 0x6E, 0x6C,
    0x5D, 0x29, 0x92, 0xC5, 0x61, 0xB6, 0x87, 0x39, 0x19, 0x7B, 0xA0, 0xB4, 0x09, 0x40, 0x65, 0x0E,
    0x43, 0xA5, 0xBB, 0x61, 0x2B, 0x8D, 0xA8, 0x8F, 0x5A, 0xD2, 0x40, 0x3F, 0xA3, 0x34, 0x4F, 0x61,
    0x9A, 0x03, 0x40, 0xBF, 0x16, 0xE3, 0xFF, 0xC9, 0x10, 0xD4, 0x5F, 0x86, 0xAB, 0x5F, 0xCF, 0x67,
    0xD8, 0xA1, 0xE3, 0xE7, 0x1B, 0xF9, 0x52, 0x87, 0x58, 0xD0, 0xEB, 0x46, 0x0E, 0x1D, 0x05, 0x83,
    0x28, 0x7C, 0xDE, 0x77, 0x80, 0x51, 0xA6, 0xB6, 0x15, 0x2A, 0x92, 0xE6, 0x91, 0x46, 0x18, 0x0A,
    0x5B, 0xCC, 0xEE, 0xBB, 0x2D, 0x6F, 0x0C, 0xB4, 0x75, 0x98, 0x52, 0x16, 0xC6, 0x7A, 0xB4, 0x46,
    0x69, 0xF8, 0x82, 0x93, 0xF9, 0x99, 0x7B, 0xA1, 0x02, 0x6C, 0x9C, 0x44, 0x4B, 0x52, 0x71, 0xDE,
    0xF2, 0x6C, 0xF3, 0x8E, 0xD7, 0x27, 0x72, 0x31, 0x5C, 0x38, 0xBC, 0x36, 0xA8, 0x27, 0x22, 0x68,
    0x5F, 0xAE, 0xBE, 0x87, 0x6B, 0x29, 0x78, 0xC3, 0x39, 0xAC, 0xAC, 0x9E, 0x58, 0x77, 0x12, 0xC7,
    0xF7, 0x92, 0x75, 0x8F, 0x09, 0xA1, 0x72, 0xFB, 0x0A, 0x68, 0x74, 0x5C, 0x58, 0x71, 0xE8, 0xFC,
    0x6C, 0x5A, 0x73, 0xA4, 0xE7, 0x38, 0xD1, 0xBF, 0xC6, 0xA0, 0x15, 0x87, 0xE3, 0xA3, 0x0D, 0xC9,
    0x4B, 0x6E, 0x43, 0x63, 0xA6, 0xB1, 0xBC, 0x36, 0xFF, 0x8A, 0x48, 0x54, 0x2C, 0x6C, 0xF3, 0x81,
    0xDD, 0x81, 0xB3, 0x00, 0x2B, 0xE0, 0x16, 0x5E, 0x4A, 0x53, 0x0D, 0x4E, 0x7A, 0xA5, 0x41, 0x85,
};

// Get current CPU time resource usage in microseconds.
static int64_t cpu_time()
{
    struct rusage ru;
    if (getrusage(RUSAGE_SELF, &ru) < 0) {
        perror("getrusage");
        exit(EXIT_FAILURE);
    }
    return ((int64_t)(ru.ru_utime.tv_sec) * USECPERSEC) + ru.ru_utime.tv_usec +
           ((int64_t)(ru.ru_stime.tv_sec) * USECPERSEC) + ru.ru_stime.tv_usec;
}

// Check a BIGNUM error.
static inline void check(int err, const char* func)
{
    if (err <= 0) {
        fprintf(stderr, "error in %s()\n", func);
        exit(EXIT_FAILURE);
    }
}

// Test program.
int main(int argc, char* argv[])
{
    // Don't check errors here, will crash later anyway in case of error...
    BIGNUM* mod = BN_bin2bn(bin_mod, sizeof(bin_mod), NULL);
    BIGNUM* n1 = BN_bin2bn(bin_1, sizeof(bin_1), NULL);
    BIGNUM* n2 = BN_bin2bn(bin_2, sizeof(bin_2), NULL);
    BIGNUM* res = BN_new();
    BN_CTX* bn_ctx = BN_CTX_new();
    BN_MONT_CTX* bn_mont_ctx = BN_MONT_CTX_new();

    check(BN_MONT_CTX_set(bn_mont_ctx, mod, bn_ctx), "BN_MONT_CTX_set");

    const uint64_t time1 = cpu_time();
    for (int i = ADD_ITERATIONS; i > 0; i--) {
        check(BN_mod_add(res, n1, n2, mod, bn_ctx), "BN_mod_add");
    }
    const uint64_t time2 = cpu_time();
    for (int i = MUL_ITERATIONS; i > 0; i--) {
        check(BN_mod_mul(res, n1, n2, mod, bn_ctx), "BN_mod_mul");
    }
    const uint64_t time3 = cpu_time();
    for (int i = MONT_MUL_ITERATIONS; i > 0; i--) {
        check(BN_mod_mul_montgomery(res, n1, n2, bn_mont_ctx, bn_ctx), "BN_mod_mul_montgomery");
    }
    const uint64_t time4 = cpu_time();

    printf("add: %.2f, mul: %.2f, mont-mul: %.2f microseconds\n",
           (double)(time2 - time1) / ADD_ITERATIONS,
           (double)(time3 - time2) / MUL_ITERATIONS,
           (double)(time4 - time3) / MONT_MUL_ITERATIONS);

    // Don't free resources, this is just a test and we exit anyway.
    return EXIT_SUCCESS;
}
