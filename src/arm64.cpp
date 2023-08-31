//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// From public code from Arm and LibTomCrypt.
// Accelerated cryptographic algorithms on Arm64.
//----------------------------------------------------------------------------

#include "arm64.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

// A few helper macros.
#if defined(__aarch64__) || defined(__arm64__)
    #define ARM64 1
#endif
#if defined(__linux__) && defined(ARM64)
    #define LINUX_ARM64 1
#endif
#if defined(__APPLE__) && defined(ARM64)
    #define MAC_ARM64 1
#endif

//----------------------------------------------------------------------------
// Stubs when compiled without AES instruction support.
//----------------------------------------------------------------------------

#if !defined(__ARM_FEATURE_CRYPTO)

bool arm64::has_aes_instructions() { return false; }
arm64::aes::aes() {}
arm64::aes::~aes() {}
void arm64::aes::wipe_key() {}
bool arm64::aes::set_key(const uint8_t*, size_t) { return false; }
size_t arm64::aes::encrypt(const uint8_t*, size_t, uint8_t*, size_t) { return 0; }
size_t arm64::aes::decrypt(const uint8_t*, size_t, uint8_t*, size_t) { return 0; }
size_t arm64::aes::encrypt_cbc(uint8_t*, size_t, const uint8_t*, size_t, uint8_t*, size_t) { return 0; }
size_t arm64::aes::decrypt_cbc(uint8_t*, size_t, const uint8_t*, size_t, uint8_t*, size_t) { return 0; }

#else

//----------------------------------------------------------------------------
// Check if an Arm64 feature is supported on the current CPU.
// This is a macro which takes Linux and macOS arguments.
// Return non-zero if the feature is supported.
//----------------------------------------------------------------------------

#include <arm_neon.h>

#if defined(LINUX_ARM64)

    #include <sys/auxv.h>

    // On Linux, use an integer arg for getauxval(3) and the bitmask to test.
    #define armfeature(linux_arg, linux_mask, macos_sysctl) (getauxval(linux_arg) & (linux_mask))

#elif defined(MAC_ARM64)

    #include <sys/param.h>
    #include <sys/sysctl.h>

    // On macOS, use a string arg for sysctlbyname(3).
    #define armfeature(linux_arg, linux_mask, macos_sysctl) armfeature_macos(macos_sysctl)

    namespace {
        inline int armfeature_macos(const char* arg)
        {
            int val = 0;
            size_t len = sizeof(val);
            return ::sysctlbyname(arg, &val, &len, NULL, 0) == 0 && val != 0;
        }
    }

#else

    // Other system or architecture: the feature is not supported.
    #define armfeature(linux_arg, linux_mask, macos_sysctl) (0)

#endif

//----------------------------------------------------------------------------
// Check if the CPU supports acccelerated instructions.
//----------------------------------------------------------------------------

bool arm64::has_aes_instructions()
{
    static volatile bool aes_checked = false;
    static volatile bool aes_supported = false;

    if (!aes_checked) {
        aes_supported = armfeature(AT_HWCAP, HWCAP_AES, "hw.optional.arm.FEAT_AES");
        aes_checked = true;
    }
    return aes_supported;
}

//----------------------------------------------------------------------------
// Structure of the secret data.
//----------------------------------------------------------------------------

struct arm64::aes::key_data
{
    size_t     kbits;      // key size in bits
    uint8x16_t aekey[15];  // scheduled encryption keys in Arm64 format
    uint8x16_t adkey[15];  // scheduled decryption keys in Arm64 format
};

//----------------------------------------------------------------------------
// AES constructor/destructor/wiper.
//----------------------------------------------------------------------------

arm64::aes::aes()
{
    // Sanity check:
    if (sizeof(_secret) < sizeof(key_data)) {
        std::cerr << "internal error, size of arm64::aes secret data too small, must be at least "
                  << sizeof(key_data) << " bytes" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    wipe_key();
}

arm64::aes::~aes()
{
    wipe_key();
}

void arm64::aes::wipe_key()
{
    memset(_secret, 0, sizeof(_secret));
}

//----------------------------------------------------------------------------
// Precomputed tables for AES
//----------------------------------------------------------------------------

namespace {

    static const uint32_t Te4_0[] = {
        0x00000063UL, 0x0000007cUL, 0x00000077UL, 0x0000007bUL, 0x000000f2UL, 0x0000006bUL, 0x0000006fUL, 0x000000c5UL,
        0x00000030UL, 0x00000001UL, 0x00000067UL, 0x0000002bUL, 0x000000feUL, 0x000000d7UL, 0x000000abUL, 0x00000076UL,
        0x000000caUL, 0x00000082UL, 0x000000c9UL, 0x0000007dUL, 0x000000faUL, 0x00000059UL, 0x00000047UL, 0x000000f0UL,
        0x000000adUL, 0x000000d4UL, 0x000000a2UL, 0x000000afUL, 0x0000009cUL, 0x000000a4UL, 0x00000072UL, 0x000000c0UL,
        0x000000b7UL, 0x000000fdUL, 0x00000093UL, 0x00000026UL, 0x00000036UL, 0x0000003fUL, 0x000000f7UL, 0x000000ccUL,
        0x00000034UL, 0x000000a5UL, 0x000000e5UL, 0x000000f1UL, 0x00000071UL, 0x000000d8UL, 0x00000031UL, 0x00000015UL,
        0x00000004UL, 0x000000c7UL, 0x00000023UL, 0x000000c3UL, 0x00000018UL, 0x00000096UL, 0x00000005UL, 0x0000009aUL,
        0x00000007UL, 0x00000012UL, 0x00000080UL, 0x000000e2UL, 0x000000ebUL, 0x00000027UL, 0x000000b2UL, 0x00000075UL,
        0x00000009UL, 0x00000083UL, 0x0000002cUL, 0x0000001aUL, 0x0000001bUL, 0x0000006eUL, 0x0000005aUL, 0x000000a0UL,
        0x00000052UL, 0x0000003bUL, 0x000000d6UL, 0x000000b3UL, 0x00000029UL, 0x000000e3UL, 0x0000002fUL, 0x00000084UL,
        0x00000053UL, 0x000000d1UL, 0x00000000UL, 0x000000edUL, 0x00000020UL, 0x000000fcUL, 0x000000b1UL, 0x0000005bUL,
        0x0000006aUL, 0x000000cbUL, 0x000000beUL, 0x00000039UL, 0x0000004aUL, 0x0000004cUL, 0x00000058UL, 0x000000cfUL,
        0x000000d0UL, 0x000000efUL, 0x000000aaUL, 0x000000fbUL, 0x00000043UL, 0x0000004dUL, 0x00000033UL, 0x00000085UL,
        0x00000045UL, 0x000000f9UL, 0x00000002UL, 0x0000007fUL, 0x00000050UL, 0x0000003cUL, 0x0000009fUL, 0x000000a8UL,
        0x00000051UL, 0x000000a3UL, 0x00000040UL, 0x0000008fUL, 0x00000092UL, 0x0000009dUL, 0x00000038UL, 0x000000f5UL,
        0x000000bcUL, 0x000000b6UL, 0x000000daUL, 0x00000021UL, 0x00000010UL, 0x000000ffUL, 0x000000f3UL, 0x000000d2UL,
        0x000000cdUL, 0x0000000cUL, 0x00000013UL, 0x000000ecUL, 0x0000005fUL, 0x00000097UL, 0x00000044UL, 0x00000017UL,
        0x000000c4UL, 0x000000a7UL, 0x0000007eUL, 0x0000003dUL, 0x00000064UL, 0x0000005dUL, 0x00000019UL, 0x00000073UL,
        0x00000060UL, 0x00000081UL, 0x0000004fUL, 0x000000dcUL, 0x00000022UL, 0x0000002aUL, 0x00000090UL, 0x00000088UL,
        0x00000046UL, 0x000000eeUL, 0x000000b8UL, 0x00000014UL, 0x000000deUL, 0x0000005eUL, 0x0000000bUL, 0x000000dbUL,
        0x000000e0UL, 0x00000032UL, 0x0000003aUL, 0x0000000aUL, 0x00000049UL, 0x00000006UL, 0x00000024UL, 0x0000005cUL,
        0x000000c2UL, 0x000000d3UL, 0x000000acUL, 0x00000062UL, 0x00000091UL, 0x00000095UL, 0x000000e4UL, 0x00000079UL,
        0x000000e7UL, 0x000000c8UL, 0x00000037UL, 0x0000006dUL, 0x0000008dUL, 0x000000d5UL, 0x0000004eUL, 0x000000a9UL,
        0x0000006cUL, 0x00000056UL, 0x000000f4UL, 0x000000eaUL, 0x00000065UL, 0x0000007aUL, 0x000000aeUL, 0x00000008UL,
        0x000000baUL, 0x00000078UL, 0x00000025UL, 0x0000002eUL, 0x0000001cUL, 0x000000a6UL, 0x000000b4UL, 0x000000c6UL,
        0x000000e8UL, 0x000000ddUL, 0x00000074UL, 0x0000001fUL, 0x0000004bUL, 0x000000bdUL, 0x0000008bUL, 0x0000008aUL,
        0x00000070UL, 0x0000003eUL, 0x000000b5UL, 0x00000066UL, 0x00000048UL, 0x00000003UL, 0x000000f6UL, 0x0000000eUL,
        0x00000061UL, 0x00000035UL, 0x00000057UL, 0x000000b9UL, 0x00000086UL, 0x000000c1UL, 0x0000001dUL, 0x0000009eUL,
        0x000000e1UL, 0x000000f8UL, 0x00000098UL, 0x00000011UL, 0x00000069UL, 0x000000d9UL, 0x0000008eUL, 0x00000094UL,
        0x0000009bUL, 0x0000001eUL, 0x00000087UL, 0x000000e9UL, 0x000000ceUL, 0x00000055UL, 0x00000028UL, 0x000000dfUL,
        0x0000008cUL, 0x000000a1UL, 0x00000089UL, 0x0000000dUL, 0x000000bfUL, 0x000000e6UL, 0x00000042UL, 0x00000068UL,
        0x00000041UL, 0x00000099UL, 0x0000002dUL, 0x0000000fUL, 0x000000b0UL, 0x00000054UL, 0x000000bbUL, 0x00000016UL
    };

    static const uint32_t Te4_1[] = {
        0x00006300UL, 0x00007c00UL, 0x00007700UL, 0x00007b00UL, 0x0000f200UL, 0x00006b00UL, 0x00006f00UL, 0x0000c500UL,
        0x00003000UL, 0x00000100UL, 0x00006700UL, 0x00002b00UL, 0x0000fe00UL, 0x0000d700UL, 0x0000ab00UL, 0x00007600UL,
        0x0000ca00UL, 0x00008200UL, 0x0000c900UL, 0x00007d00UL, 0x0000fa00UL, 0x00005900UL, 0x00004700UL, 0x0000f000UL,
        0x0000ad00UL, 0x0000d400UL, 0x0000a200UL, 0x0000af00UL, 0x00009c00UL, 0x0000a400UL, 0x00007200UL, 0x0000c000UL,
        0x0000b700UL, 0x0000fd00UL, 0x00009300UL, 0x00002600UL, 0x00003600UL, 0x00003f00UL, 0x0000f700UL, 0x0000cc00UL,
        0x00003400UL, 0x0000a500UL, 0x0000e500UL, 0x0000f100UL, 0x00007100UL, 0x0000d800UL, 0x00003100UL, 0x00001500UL,
        0x00000400UL, 0x0000c700UL, 0x00002300UL, 0x0000c300UL, 0x00001800UL, 0x00009600UL, 0x00000500UL, 0x00009a00UL,
        0x00000700UL, 0x00001200UL, 0x00008000UL, 0x0000e200UL, 0x0000eb00UL, 0x00002700UL, 0x0000b200UL, 0x00007500UL,
        0x00000900UL, 0x00008300UL, 0x00002c00UL, 0x00001a00UL, 0x00001b00UL, 0x00006e00UL, 0x00005a00UL, 0x0000a000UL,
        0x00005200UL, 0x00003b00UL, 0x0000d600UL, 0x0000b300UL, 0x00002900UL, 0x0000e300UL, 0x00002f00UL, 0x00008400UL,
        0x00005300UL, 0x0000d100UL, 0x00000000UL, 0x0000ed00UL, 0x00002000UL, 0x0000fc00UL, 0x0000b100UL, 0x00005b00UL,
        0x00006a00UL, 0x0000cb00UL, 0x0000be00UL, 0x00003900UL, 0x00004a00UL, 0x00004c00UL, 0x00005800UL, 0x0000cf00UL,
        0x0000d000UL, 0x0000ef00UL, 0x0000aa00UL, 0x0000fb00UL, 0x00004300UL, 0x00004d00UL, 0x00003300UL, 0x00008500UL,
        0x00004500UL, 0x0000f900UL, 0x00000200UL, 0x00007f00UL, 0x00005000UL, 0x00003c00UL, 0x00009f00UL, 0x0000a800UL,
        0x00005100UL, 0x0000a300UL, 0x00004000UL, 0x00008f00UL, 0x00009200UL, 0x00009d00UL, 0x00003800UL, 0x0000f500UL,
        0x0000bc00UL, 0x0000b600UL, 0x0000da00UL, 0x00002100UL, 0x00001000UL, 0x0000ff00UL, 0x0000f300UL, 0x0000d200UL,
        0x0000cd00UL, 0x00000c00UL, 0x00001300UL, 0x0000ec00UL, 0x00005f00UL, 0x00009700UL, 0x00004400UL, 0x00001700UL,
        0x0000c400UL, 0x0000a700UL, 0x00007e00UL, 0x00003d00UL, 0x00006400UL, 0x00005d00UL, 0x00001900UL, 0x00007300UL,
        0x00006000UL, 0x00008100UL, 0x00004f00UL, 0x0000dc00UL, 0x00002200UL, 0x00002a00UL, 0x00009000UL, 0x00008800UL,
        0x00004600UL, 0x0000ee00UL, 0x0000b800UL, 0x00001400UL, 0x0000de00UL, 0x00005e00UL, 0x00000b00UL, 0x0000db00UL,
        0x0000e000UL, 0x00003200UL, 0x00003a00UL, 0x00000a00UL, 0x00004900UL, 0x00000600UL, 0x00002400UL, 0x00005c00UL,
        0x0000c200UL, 0x0000d300UL, 0x0000ac00UL, 0x00006200UL, 0x00009100UL, 0x00009500UL, 0x0000e400UL, 0x00007900UL,
        0x0000e700UL, 0x0000c800UL, 0x00003700UL, 0x00006d00UL, 0x00008d00UL, 0x0000d500UL, 0x00004e00UL, 0x0000a900UL,
        0x00006c00UL, 0x00005600UL, 0x0000f400UL, 0x0000ea00UL, 0x00006500UL, 0x00007a00UL, 0x0000ae00UL, 0x00000800UL,
        0x0000ba00UL, 0x00007800UL, 0x00002500UL, 0x00002e00UL, 0x00001c00UL, 0x0000a600UL, 0x0000b400UL, 0x0000c600UL,
        0x0000e800UL, 0x0000dd00UL, 0x00007400UL, 0x00001f00UL, 0x00004b00UL, 0x0000bd00UL, 0x00008b00UL, 0x00008a00UL,
        0x00007000UL, 0x00003e00UL, 0x0000b500UL, 0x00006600UL, 0x00004800UL, 0x00000300UL, 0x0000f600UL, 0x00000e00UL,
        0x00006100UL, 0x00003500UL, 0x00005700UL, 0x0000b900UL, 0x00008600UL, 0x0000c100UL, 0x00001d00UL, 0x00009e00UL,
        0x0000e100UL, 0x0000f800UL, 0x00009800UL, 0x00001100UL, 0x00006900UL, 0x0000d900UL, 0x00008e00UL, 0x00009400UL,
        0x00009b00UL, 0x00001e00UL, 0x00008700UL, 0x0000e900UL, 0x0000ce00UL, 0x00005500UL, 0x00002800UL, 0x0000df00UL,
        0x00008c00UL, 0x0000a100UL, 0x00008900UL, 0x00000d00UL, 0x0000bf00UL, 0x0000e600UL, 0x00004200UL, 0x00006800UL,
        0x00004100UL, 0x00009900UL, 0x00002d00UL, 0x00000f00UL, 0x0000b000UL, 0x00005400UL, 0x0000bb00UL, 0x00001600UL
    };

    static const uint32_t Te4_2[] = {
        0x00630000UL, 0x007c0000UL, 0x00770000UL, 0x007b0000UL, 0x00f20000UL, 0x006b0000UL, 0x006f0000UL, 0x00c50000UL,
        0x00300000UL, 0x00010000UL, 0x00670000UL, 0x002b0000UL, 0x00fe0000UL, 0x00d70000UL, 0x00ab0000UL, 0x00760000UL,
        0x00ca0000UL, 0x00820000UL, 0x00c90000UL, 0x007d0000UL, 0x00fa0000UL, 0x00590000UL, 0x00470000UL, 0x00f00000UL,
        0x00ad0000UL, 0x00d40000UL, 0x00a20000UL, 0x00af0000UL, 0x009c0000UL, 0x00a40000UL, 0x00720000UL, 0x00c00000UL,
        0x00b70000UL, 0x00fd0000UL, 0x00930000UL, 0x00260000UL, 0x00360000UL, 0x003f0000UL, 0x00f70000UL, 0x00cc0000UL,
        0x00340000UL, 0x00a50000UL, 0x00e50000UL, 0x00f10000UL, 0x00710000UL, 0x00d80000UL, 0x00310000UL, 0x00150000UL,
        0x00040000UL, 0x00c70000UL, 0x00230000UL, 0x00c30000UL, 0x00180000UL, 0x00960000UL, 0x00050000UL, 0x009a0000UL,
        0x00070000UL, 0x00120000UL, 0x00800000UL, 0x00e20000UL, 0x00eb0000UL, 0x00270000UL, 0x00b20000UL, 0x00750000UL,
        0x00090000UL, 0x00830000UL, 0x002c0000UL, 0x001a0000UL, 0x001b0000UL, 0x006e0000UL, 0x005a0000UL, 0x00a00000UL,
        0x00520000UL, 0x003b0000UL, 0x00d60000UL, 0x00b30000UL, 0x00290000UL, 0x00e30000UL, 0x002f0000UL, 0x00840000UL,
        0x00530000UL, 0x00d10000UL, 0x00000000UL, 0x00ed0000UL, 0x00200000UL, 0x00fc0000UL, 0x00b10000UL, 0x005b0000UL,
        0x006a0000UL, 0x00cb0000UL, 0x00be0000UL, 0x00390000UL, 0x004a0000UL, 0x004c0000UL, 0x00580000UL, 0x00cf0000UL,
        0x00d00000UL, 0x00ef0000UL, 0x00aa0000UL, 0x00fb0000UL, 0x00430000UL, 0x004d0000UL, 0x00330000UL, 0x00850000UL,
        0x00450000UL, 0x00f90000UL, 0x00020000UL, 0x007f0000UL, 0x00500000UL, 0x003c0000UL, 0x009f0000UL, 0x00a80000UL,
        0x00510000UL, 0x00a30000UL, 0x00400000UL, 0x008f0000UL, 0x00920000UL, 0x009d0000UL, 0x00380000UL, 0x00f50000UL,
        0x00bc0000UL, 0x00b60000UL, 0x00da0000UL, 0x00210000UL, 0x00100000UL, 0x00ff0000UL, 0x00f30000UL, 0x00d20000UL,
        0x00cd0000UL, 0x000c0000UL, 0x00130000UL, 0x00ec0000UL, 0x005f0000UL, 0x00970000UL, 0x00440000UL, 0x00170000UL,
        0x00c40000UL, 0x00a70000UL, 0x007e0000UL, 0x003d0000UL, 0x00640000UL, 0x005d0000UL, 0x00190000UL, 0x00730000UL,
        0x00600000UL, 0x00810000UL, 0x004f0000UL, 0x00dc0000UL, 0x00220000UL, 0x002a0000UL, 0x00900000UL, 0x00880000UL,
        0x00460000UL, 0x00ee0000UL, 0x00b80000UL, 0x00140000UL, 0x00de0000UL, 0x005e0000UL, 0x000b0000UL, 0x00db0000UL,
        0x00e00000UL, 0x00320000UL, 0x003a0000UL, 0x000a0000UL, 0x00490000UL, 0x00060000UL, 0x00240000UL, 0x005c0000UL,
        0x00c20000UL, 0x00d30000UL, 0x00ac0000UL, 0x00620000UL, 0x00910000UL, 0x00950000UL, 0x00e40000UL, 0x00790000UL,
        0x00e70000UL, 0x00c80000UL, 0x00370000UL, 0x006d0000UL, 0x008d0000UL, 0x00d50000UL, 0x004e0000UL, 0x00a90000UL,
        0x006c0000UL, 0x00560000UL, 0x00f40000UL, 0x00ea0000UL, 0x00650000UL, 0x007a0000UL, 0x00ae0000UL, 0x00080000UL,
        0x00ba0000UL, 0x00780000UL, 0x00250000UL, 0x002e0000UL, 0x001c0000UL, 0x00a60000UL, 0x00b40000UL, 0x00c60000UL,
        0x00e80000UL, 0x00dd0000UL, 0x00740000UL, 0x001f0000UL, 0x004b0000UL, 0x00bd0000UL, 0x008b0000UL, 0x008a0000UL,
        0x00700000UL, 0x003e0000UL, 0x00b50000UL, 0x00660000UL, 0x00480000UL, 0x00030000UL, 0x00f60000UL, 0x000e0000UL,
        0x00610000UL, 0x00350000UL, 0x00570000UL, 0x00b90000UL, 0x00860000UL, 0x00c10000UL, 0x001d0000UL, 0x009e0000UL,
        0x00e10000UL, 0x00f80000UL, 0x00980000UL, 0x00110000UL, 0x00690000UL, 0x00d90000UL, 0x008e0000UL, 0x00940000UL,
        0x009b0000UL, 0x001e0000UL, 0x00870000UL, 0x00e90000UL, 0x00ce0000UL, 0x00550000UL, 0x00280000UL, 0x00df0000UL,
        0x008c0000UL, 0x00a10000UL, 0x00890000UL, 0x000d0000UL, 0x00bf0000UL, 0x00e60000UL, 0x00420000UL, 0x00680000UL,
        0x00410000UL, 0x00990000UL, 0x002d0000UL, 0x000f0000UL, 0x00b00000UL, 0x00540000UL, 0x00bb0000UL, 0x00160000UL
    };

    static const uint32_t Te4_3[] = {
        0x63000000UL, 0x7c000000UL, 0x77000000UL, 0x7b000000UL, 0xf2000000UL, 0x6b000000UL, 0x6f000000UL, 0xc5000000UL,
        0x30000000UL, 0x01000000UL, 0x67000000UL, 0x2b000000UL, 0xfe000000UL, 0xd7000000UL, 0xab000000UL, 0x76000000UL,
        0xca000000UL, 0x82000000UL, 0xc9000000UL, 0x7d000000UL, 0xfa000000UL, 0x59000000UL, 0x47000000UL, 0xf0000000UL,
        0xad000000UL, 0xd4000000UL, 0xa2000000UL, 0xaf000000UL, 0x9c000000UL, 0xa4000000UL, 0x72000000UL, 0xc0000000UL,
        0xb7000000UL, 0xfd000000UL, 0x93000000UL, 0x26000000UL, 0x36000000UL, 0x3f000000UL, 0xf7000000UL, 0xcc000000UL,
        0x34000000UL, 0xa5000000UL, 0xe5000000UL, 0xf1000000UL, 0x71000000UL, 0xd8000000UL, 0x31000000UL, 0x15000000UL,
        0x04000000UL, 0xc7000000UL, 0x23000000UL, 0xc3000000UL, 0x18000000UL, 0x96000000UL, 0x05000000UL, 0x9a000000UL,
        0x07000000UL, 0x12000000UL, 0x80000000UL, 0xe2000000UL, 0xeb000000UL, 0x27000000UL, 0xb2000000UL, 0x75000000UL,
        0x09000000UL, 0x83000000UL, 0x2c000000UL, 0x1a000000UL, 0x1b000000UL, 0x6e000000UL, 0x5a000000UL, 0xa0000000UL,
        0x52000000UL, 0x3b000000UL, 0xd6000000UL, 0xb3000000UL, 0x29000000UL, 0xe3000000UL, 0x2f000000UL, 0x84000000UL,
        0x53000000UL, 0xd1000000UL, 0x00000000UL, 0xed000000UL, 0x20000000UL, 0xfc000000UL, 0xb1000000UL, 0x5b000000UL,
        0x6a000000UL, 0xcb000000UL, 0xbe000000UL, 0x39000000UL, 0x4a000000UL, 0x4c000000UL, 0x58000000UL, 0xcf000000UL,
        0xd0000000UL, 0xef000000UL, 0xaa000000UL, 0xfb000000UL, 0x43000000UL, 0x4d000000UL, 0x33000000UL, 0x85000000UL,
        0x45000000UL, 0xf9000000UL, 0x02000000UL, 0x7f000000UL, 0x50000000UL, 0x3c000000UL, 0x9f000000UL, 0xa8000000UL,
        0x51000000UL, 0xa3000000UL, 0x40000000UL, 0x8f000000UL, 0x92000000UL, 0x9d000000UL, 0x38000000UL, 0xf5000000UL,
        0xbc000000UL, 0xb6000000UL, 0xda000000UL, 0x21000000UL, 0x10000000UL, 0xff000000UL, 0xf3000000UL, 0xd2000000UL,
        0xcd000000UL, 0x0c000000UL, 0x13000000UL, 0xec000000UL, 0x5f000000UL, 0x97000000UL, 0x44000000UL, 0x17000000UL,
        0xc4000000UL, 0xa7000000UL, 0x7e000000UL, 0x3d000000UL, 0x64000000UL, 0x5d000000UL, 0x19000000UL, 0x73000000UL,
        0x60000000UL, 0x81000000UL, 0x4f000000UL, 0xdc000000UL, 0x22000000UL, 0x2a000000UL, 0x90000000UL, 0x88000000UL,
        0x46000000UL, 0xee000000UL, 0xb8000000UL, 0x14000000UL, 0xde000000UL, 0x5e000000UL, 0x0b000000UL, 0xdb000000UL,
        0xe0000000UL, 0x32000000UL, 0x3a000000UL, 0x0a000000UL, 0x49000000UL, 0x06000000UL, 0x24000000UL, 0x5c000000UL,
        0xc2000000UL, 0xd3000000UL, 0xac000000UL, 0x62000000UL, 0x91000000UL, 0x95000000UL, 0xe4000000UL, 0x79000000UL,
        0xe7000000UL, 0xc8000000UL, 0x37000000UL, 0x6d000000UL, 0x8d000000UL, 0xd5000000UL, 0x4e000000UL, 0xa9000000UL,
        0x6c000000UL, 0x56000000UL, 0xf4000000UL, 0xea000000UL, 0x65000000UL, 0x7a000000UL, 0xae000000UL, 0x08000000UL,
        0xba000000UL, 0x78000000UL, 0x25000000UL, 0x2e000000UL, 0x1c000000UL, 0xa6000000UL, 0xb4000000UL, 0xc6000000UL,
        0xe8000000UL, 0xdd000000UL, 0x74000000UL, 0x1f000000UL, 0x4b000000UL, 0xbd000000UL, 0x8b000000UL, 0x8a000000UL,
        0x70000000UL, 0x3e000000UL, 0xb5000000UL, 0x66000000UL, 0x48000000UL, 0x03000000UL, 0xf6000000UL, 0x0e000000UL,
        0x61000000UL, 0x35000000UL, 0x57000000UL, 0xb9000000UL, 0x86000000UL, 0xc1000000UL, 0x1d000000UL, 0x9e000000UL,
        0xe1000000UL, 0xf8000000UL, 0x98000000UL, 0x11000000UL, 0x69000000UL, 0xd9000000UL, 0x8e000000UL, 0x94000000UL,
        0x9b000000UL, 0x1e000000UL, 0x87000000UL, 0xe9000000UL, 0xce000000UL, 0x55000000UL, 0x28000000UL, 0xdf000000UL,
        0x8c000000UL, 0xa1000000UL, 0x89000000UL, 0x0d000000UL, 0xbf000000UL, 0xe6000000UL, 0x42000000UL, 0x68000000UL,
        0x41000000UL, 0x99000000UL, 0x2d000000UL, 0x0f000000UL, 0xb0000000UL, 0x54000000UL, 0xbb000000UL, 0x16000000UL
    };

    static const uint32_t Tks0[] = {
        0x00000000UL, 0x0e090d0bUL, 0x1c121a16UL, 0x121b171dUL, 0x3824342cUL, 0x362d3927UL, 0x24362e3aUL, 0x2a3f2331UL,
        0x70486858UL, 0x7e416553UL, 0x6c5a724eUL, 0x62537f45UL, 0x486c5c74UL, 0x4665517fUL, 0x547e4662UL, 0x5a774b69UL,
        0xe090d0b0UL, 0xee99ddbbUL, 0xfc82caa6UL, 0xf28bc7adUL, 0xd8b4e49cUL, 0xd6bde997UL, 0xc4a6fe8aUL, 0xcaaff381UL,
        0x90d8b8e8UL, 0x9ed1b5e3UL, 0x8ccaa2feUL, 0x82c3aff5UL, 0xa8fc8cc4UL, 0xa6f581cfUL, 0xb4ee96d2UL, 0xbae79bd9UL,
        0xdb3bbb7bUL, 0xd532b670UL, 0xc729a16dUL, 0xc920ac66UL, 0xe31f8f57UL, 0xed16825cUL, 0xff0d9541UL, 0xf104984aUL,
        0xab73d323UL, 0xa57ade28UL, 0xb761c935UL, 0xb968c43eUL, 0x9357e70fUL, 0x9d5eea04UL, 0x8f45fd19UL, 0x814cf012UL,
        0x3bab6bcbUL, 0x35a266c0UL, 0x27b971ddUL, 0x29b07cd6UL, 0x038f5fe7UL, 0x0d8652ecUL, 0x1f9d45f1UL, 0x119448faUL,
        0x4be30393UL, 0x45ea0e98UL, 0x57f11985UL, 0x59f8148eUL, 0x73c737bfUL, 0x7dce3ab4UL, 0x6fd52da9UL, 0x61dc20a2UL,
        0xad766df6UL, 0xa37f60fdUL, 0xb16477e0UL, 0xbf6d7aebUL, 0x955259daUL, 0x9b5b54d1UL, 0x894043ccUL, 0x87494ec7UL,
        0xdd3e05aeUL, 0xd33708a5UL, 0xc12c1fb8UL, 0xcf2512b3UL, 0xe51a3182UL, 0xeb133c89UL, 0xf9082b94UL, 0xf701269fUL,
        0x4de6bd46UL, 0x43efb04dUL, 0x51f4a750UL, 0x5ffdaa5bUL, 0x75c2896aUL, 0x7bcb8461UL, 0x69d0937cUL, 0x67d99e77UL,
        0x3daed51eUL, 0x33a7d815UL, 0x21bccf08UL, 0x2fb5c203UL, 0x058ae132UL, 0x0b83ec39UL, 0x1998fb24UL, 0x1791f62fUL,
        0x764dd68dUL, 0x7844db86UL, 0x6a5fcc9bUL, 0x6456c190UL, 0x4e69e2a1UL, 0x4060efaaUL, 0x527bf8b7UL, 0x5c72f5bcUL,
        0x0605bed5UL, 0x080cb3deUL, 0x1a17a4c3UL, 0x141ea9c8UL, 0x3e218af9UL, 0x302887f2UL, 0x223390efUL, 0x2c3a9de4UL,
        0x96dd063dUL, 0x98d40b36UL, 0x8acf1c2bUL, 0x84c61120UL, 0xaef93211UL, 0xa0f03f1aUL, 0xb2eb2807UL, 0xbce2250cUL,
        0xe6956e65UL, 0xe89c636eUL, 0xfa877473UL, 0xf48e7978UL, 0xdeb15a49UL, 0xd0b85742UL, 0xc2a3405fUL, 0xccaa4d54UL,
        0x41ecdaf7UL, 0x4fe5d7fcUL, 0x5dfec0e1UL, 0x53f7cdeaUL, 0x79c8eedbUL, 0x77c1e3d0UL, 0x65daf4cdUL, 0x6bd3f9c6UL,
        0x31a4b2afUL, 0x3fadbfa4UL, 0x2db6a8b9UL, 0x23bfa5b2UL, 0x09808683UL, 0x07898b88UL, 0x15929c95UL, 0x1b9b919eUL,
        0xa17c0a47UL, 0xaf75074cUL, 0xbd6e1051UL, 0xb3671d5aUL, 0x99583e6bUL, 0x97513360UL, 0x854a247dUL, 0x8b432976UL,
        0xd134621fUL, 0xdf3d6f14UL, 0xcd267809UL, 0xc32f7502UL, 0xe9105633UL, 0xe7195b38UL, 0xf5024c25UL, 0xfb0b412eUL,
        0x9ad7618cUL, 0x94de6c87UL, 0x86c57b9aUL, 0x88cc7691UL, 0xa2f355a0UL, 0xacfa58abUL, 0xbee14fb6UL, 0xb0e842bdUL,
        0xea9f09d4UL, 0xe49604dfUL, 0xf68d13c2UL, 0xf8841ec9UL, 0xd2bb3df8UL, 0xdcb230f3UL, 0xcea927eeUL, 0xc0a02ae5UL,
        0x7a47b13cUL, 0x744ebc37UL, 0x6655ab2aUL, 0x685ca621UL, 0x42638510UL, 0x4c6a881bUL, 0x5e719f06UL, 0x5078920dUL,
        0x0a0fd964UL, 0x0406d46fUL, 0x161dc372UL, 0x1814ce79UL, 0x322bed48UL, 0x3c22e043UL, 0x2e39f75eUL, 0x2030fa55UL,
        0xec9ab701UL, 0xe293ba0aUL, 0xf088ad17UL, 0xfe81a01cUL, 0xd4be832dUL, 0xdab78e26UL, 0xc8ac993bUL, 0xc6a59430UL,
        0x9cd2df59UL, 0x92dbd252UL, 0x80c0c54fUL, 0x8ec9c844UL, 0xa4f6eb75UL, 0xaaffe67eUL, 0xb8e4f163UL, 0xb6edfc68UL,
        0x0c0a67b1UL, 0x02036abaUL, 0x10187da7UL, 0x1e1170acUL, 0x342e539dUL, 0x3a275e96UL, 0x283c498bUL, 0x26354480UL,
        0x7c420fe9UL, 0x724b02e2UL, 0x605015ffUL, 0x6e5918f4UL, 0x44663bc5UL, 0x4a6f36ceUL, 0x587421d3UL, 0x567d2cd8UL,
        0x37a10c7aUL, 0x39a80171UL, 0x2bb3166cUL, 0x25ba1b67UL, 0x0f853856UL, 0x018c355dUL, 0x13972240UL, 0x1d9e2f4bUL,
        0x47e96422UL, 0x49e06929UL, 0x5bfb7e34UL, 0x55f2733fUL, 0x7fcd500eUL, 0x71c45d05UL, 0x63df4a18UL, 0x6dd64713UL,
        0xd731dccaUL, 0xd938d1c1UL, 0xcb23c6dcUL, 0xc52acbd7UL, 0xef15e8e6UL, 0xe11ce5edUL, 0xf307f2f0UL, 0xfd0efffbUL,
        0xa779b492UL, 0xa970b999UL, 0xbb6bae84UL, 0xb562a38fUL, 0x9f5d80beUL, 0x91548db5UL, 0x834f9aa8UL, 0x8d4697a3UL
    };

    static const uint32_t Tks1[] = {
        0x00000000UL, 0x0b0e090dUL, 0x161c121aUL, 0x1d121b17UL, 0x2c382434UL, 0x27362d39UL, 0x3a24362eUL, 0x312a3f23UL,
        0x58704868UL, 0x537e4165UL, 0x4e6c5a72UL, 0x4562537fUL, 0x74486c5cUL, 0x7f466551UL, 0x62547e46UL, 0x695a774bUL,
        0xb0e090d0UL, 0xbbee99ddUL, 0xa6fc82caUL, 0xadf28bc7UL, 0x9cd8b4e4UL, 0x97d6bde9UL, 0x8ac4a6feUL, 0x81caaff3UL,
        0xe890d8b8UL, 0xe39ed1b5UL, 0xfe8ccaa2UL, 0xf582c3afUL, 0xc4a8fc8cUL, 0xcfa6f581UL, 0xd2b4ee96UL, 0xd9bae79bUL,
        0x7bdb3bbbUL, 0x70d532b6UL, 0x6dc729a1UL, 0x66c920acUL, 0x57e31f8fUL, 0x5ced1682UL, 0x41ff0d95UL, 0x4af10498UL,
        0x23ab73d3UL, 0x28a57adeUL, 0x35b761c9UL, 0x3eb968c4UL, 0x0f9357e7UL, 0x049d5eeaUL, 0x198f45fdUL, 0x12814cf0UL,
        0xcb3bab6bUL, 0xc035a266UL, 0xdd27b971UL, 0xd629b07cUL, 0xe7038f5fUL, 0xec0d8652UL, 0xf11f9d45UL, 0xfa119448UL,
        0x934be303UL, 0x9845ea0eUL, 0x8557f119UL, 0x8e59f814UL, 0xbf73c737UL, 0xb47dce3aUL, 0xa96fd52dUL, 0xa261dc20UL,
        0xf6ad766dUL, 0xfda37f60UL, 0xe0b16477UL, 0xebbf6d7aUL, 0xda955259UL, 0xd19b5b54UL, 0xcc894043UL, 0xc787494eUL,
        0xaedd3e05UL, 0xa5d33708UL, 0xb8c12c1fUL, 0xb3cf2512UL, 0x82e51a31UL, 0x89eb133cUL, 0x94f9082bUL, 0x9ff70126UL,
        0x464de6bdUL, 0x4d43efb0UL, 0x5051f4a7UL, 0x5b5ffdaaUL, 0x6a75c289UL, 0x617bcb84UL, 0x7c69d093UL, 0x7767d99eUL,
        0x1e3daed5UL, 0x1533a7d8UL, 0x0821bccfUL, 0x032fb5c2UL, 0x32058ae1UL, 0x390b83ecUL, 0x241998fbUL, 0x2f1791f6UL,
        0x8d764dd6UL, 0x867844dbUL, 0x9b6a5fccUL, 0x906456c1UL, 0xa14e69e2UL, 0xaa4060efUL, 0xb7527bf8UL, 0xbc5c72f5UL,
        0xd50605beUL, 0xde080cb3UL, 0xc31a17a4UL, 0xc8141ea9UL, 0xf93e218aUL, 0xf2302887UL, 0xef223390UL, 0xe42c3a9dUL,
        0x3d96dd06UL, 0x3698d40bUL, 0x2b8acf1cUL, 0x2084c611UL, 0x11aef932UL, 0x1aa0f03fUL, 0x07b2eb28UL, 0x0cbce225UL,
        0x65e6956eUL, 0x6ee89c63UL, 0x73fa8774UL, 0x78f48e79UL, 0x49deb15aUL, 0x42d0b857UL, 0x5fc2a340UL, 0x54ccaa4dUL,
        0xf741ecdaUL, 0xfc4fe5d7UL, 0xe15dfec0UL, 0xea53f7cdUL, 0xdb79c8eeUL, 0xd077c1e3UL, 0xcd65daf4UL, 0xc66bd3f9UL,
        0xaf31a4b2UL, 0xa43fadbfUL, 0xb92db6a8UL, 0xb223bfa5UL, 0x83098086UL, 0x8807898bUL, 0x9515929cUL, 0x9e1b9b91UL,
        0x47a17c0aUL, 0x4caf7507UL, 0x51bd6e10UL, 0x5ab3671dUL, 0x6b99583eUL, 0x60975133UL, 0x7d854a24UL, 0x768b4329UL,
        0x1fd13462UL, 0x14df3d6fUL, 0x09cd2678UL, 0x02c32f75UL, 0x33e91056UL, 0x38e7195bUL, 0x25f5024cUL, 0x2efb0b41UL,
        0x8c9ad761UL, 0x8794de6cUL, 0x9a86c57bUL, 0x9188cc76UL, 0xa0a2f355UL, 0xabacfa58UL, 0xb6bee14fUL, 0xbdb0e842UL,
        0xd4ea9f09UL, 0xdfe49604UL, 0xc2f68d13UL, 0xc9f8841eUL, 0xf8d2bb3dUL, 0xf3dcb230UL, 0xeecea927UL, 0xe5c0a02aUL,
        0x3c7a47b1UL, 0x37744ebcUL, 0x2a6655abUL, 0x21685ca6UL, 0x10426385UL, 0x1b4c6a88UL, 0x065e719fUL, 0x0d507892UL,
        0x640a0fd9UL, 0x6f0406d4UL, 0x72161dc3UL, 0x791814ceUL, 0x48322bedUL, 0x433c22e0UL, 0x5e2e39f7UL, 0x552030faUL,
        0x01ec9ab7UL, 0x0ae293baUL, 0x17f088adUL, 0x1cfe81a0UL, 0x2dd4be83UL, 0x26dab78eUL, 0x3bc8ac99UL, 0x30c6a594UL,
        0x599cd2dfUL, 0x5292dbd2UL, 0x4f80c0c5UL, 0x448ec9c8UL, 0x75a4f6ebUL, 0x7eaaffe6UL, 0x63b8e4f1UL, 0x68b6edfcUL,
        0xb10c0a67UL, 0xba02036aUL, 0xa710187dUL, 0xac1e1170UL, 0x9d342e53UL, 0x963a275eUL, 0x8b283c49UL, 0x80263544UL,
        0xe97c420fUL, 0xe2724b02UL, 0xff605015UL, 0xf46e5918UL, 0xc544663bUL, 0xce4a6f36UL, 0xd3587421UL, 0xd8567d2cUL,
        0x7a37a10cUL, 0x7139a801UL, 0x6c2bb316UL, 0x6725ba1bUL, 0x560f8538UL, 0x5d018c35UL, 0x40139722UL, 0x4b1d9e2fUL,
        0x2247e964UL, 0x2949e069UL, 0x345bfb7eUL, 0x3f55f273UL, 0x0e7fcd50UL, 0x0571c45dUL, 0x1863df4aUL, 0x136dd647UL,
        0xcad731dcUL, 0xc1d938d1UL, 0xdccb23c6UL, 0xd7c52acbUL, 0xe6ef15e8UL, 0xede11ce5UL, 0xf0f307f2UL, 0xfbfd0effUL,
        0x92a779b4UL, 0x99a970b9UL, 0x84bb6baeUL, 0x8fb562a3UL, 0xbe9f5d80UL, 0xb591548dUL, 0xa8834f9aUL, 0xa38d4697UL
    };

    static const uint32_t Tks2[] = {
        0x00000000UL, 0x0d0b0e09UL, 0x1a161c12UL, 0x171d121bUL, 0x342c3824UL, 0x3927362dUL, 0x2e3a2436UL, 0x23312a3fUL,
        0x68587048UL, 0x65537e41UL, 0x724e6c5aUL, 0x7f456253UL, 0x5c74486cUL, 0x517f4665UL, 0x4662547eUL, 0x4b695a77UL,
        0xd0b0e090UL, 0xddbbee99UL, 0xcaa6fc82UL, 0xc7adf28bUL, 0xe49cd8b4UL, 0xe997d6bdUL, 0xfe8ac4a6UL, 0xf381caafUL,
        0xb8e890d8UL, 0xb5e39ed1UL, 0xa2fe8ccaUL, 0xaff582c3UL, 0x8cc4a8fcUL, 0x81cfa6f5UL, 0x96d2b4eeUL, 0x9bd9bae7UL,
        0xbb7bdb3bUL, 0xb670d532UL, 0xa16dc729UL, 0xac66c920UL, 0x8f57e31fUL, 0x825ced16UL, 0x9541ff0dUL, 0x984af104UL,
        0xd323ab73UL, 0xde28a57aUL, 0xc935b761UL, 0xc43eb968UL, 0xe70f9357UL, 0xea049d5eUL, 0xfd198f45UL, 0xf012814cUL,
        0x6bcb3babUL, 0x66c035a2UL, 0x71dd27b9UL, 0x7cd629b0UL, 0x5fe7038fUL, 0x52ec0d86UL, 0x45f11f9dUL, 0x48fa1194UL,
        0x03934be3UL, 0x0e9845eaUL, 0x198557f1UL, 0x148e59f8UL, 0x37bf73c7UL, 0x3ab47dceUL, 0x2da96fd5UL, 0x20a261dcUL,
        0x6df6ad76UL, 0x60fda37fUL, 0x77e0b164UL, 0x7aebbf6dUL, 0x59da9552UL, 0x54d19b5bUL, 0x43cc8940UL, 0x4ec78749UL,
        0x05aedd3eUL, 0x08a5d337UL, 0x1fb8c12cUL, 0x12b3cf25UL, 0x3182e51aUL, 0x3c89eb13UL, 0x2b94f908UL, 0x269ff701UL,
        0xbd464de6UL, 0xb04d43efUL, 0xa75051f4UL, 0xaa5b5ffdUL, 0x896a75c2UL, 0x84617bcbUL, 0x937c69d0UL, 0x9e7767d9UL,
        0xd51e3daeUL, 0xd81533a7UL, 0xcf0821bcUL, 0xc2032fb5UL, 0xe132058aUL, 0xec390b83UL, 0xfb241998UL, 0xf62f1791UL,
        0xd68d764dUL, 0xdb867844UL, 0xcc9b6a5fUL, 0xc1906456UL, 0xe2a14e69UL, 0xefaa4060UL, 0xf8b7527bUL, 0xf5bc5c72UL,
        0xbed50605UL, 0xb3de080cUL, 0xa4c31a17UL, 0xa9c8141eUL, 0x8af93e21UL, 0x87f23028UL, 0x90ef2233UL, 0x9de42c3aUL,
        0x063d96ddUL, 0x0b3698d4UL, 0x1c2b8acfUL, 0x112084c6UL, 0x3211aef9UL, 0x3f1aa0f0UL, 0x2807b2ebUL, 0x250cbce2UL,
        0x6e65e695UL, 0x636ee89cUL, 0x7473fa87UL, 0x7978f48eUL, 0x5a49deb1UL, 0x5742d0b8UL, 0x405fc2a3UL, 0x4d54ccaaUL,
        0xdaf741ecUL, 0xd7fc4fe5UL, 0xc0e15dfeUL, 0xcdea53f7UL, 0xeedb79c8UL, 0xe3d077c1UL, 0xf4cd65daUL, 0xf9c66bd3UL,
        0xb2af31a4UL, 0xbfa43fadUL, 0xa8b92db6UL, 0xa5b223bfUL, 0x86830980UL, 0x8b880789UL, 0x9c951592UL, 0x919e1b9bUL,
        0x0a47a17cUL, 0x074caf75UL, 0x1051bd6eUL, 0x1d5ab367UL, 0x3e6b9958UL, 0x33609751UL, 0x247d854aUL, 0x29768b43UL,
        0x621fd134UL, 0x6f14df3dUL, 0x7809cd26UL, 0x7502c32fUL, 0x5633e910UL, 0x5b38e719UL, 0x4c25f502UL, 0x412efb0bUL,
        0x618c9ad7UL, 0x6c8794deUL, 0x7b9a86c5UL, 0x769188ccUL, 0x55a0a2f3UL, 0x58abacfaUL, 0x4fb6bee1UL, 0x42bdb0e8UL,
        0x09d4ea9fUL, 0x04dfe496UL, 0x13c2f68dUL, 0x1ec9f884UL, 0x3df8d2bbUL, 0x30f3dcb2UL, 0x27eecea9UL, 0x2ae5c0a0UL,
        0xb13c7a47UL, 0xbc37744eUL, 0xab2a6655UL, 0xa621685cUL, 0x85104263UL, 0x881b4c6aUL, 0x9f065e71UL, 0x920d5078UL,
        0xd9640a0fUL, 0xd46f0406UL, 0xc372161dUL, 0xce791814UL, 0xed48322bUL, 0xe0433c22UL, 0xf75e2e39UL, 0xfa552030UL,
        0xb701ec9aUL, 0xba0ae293UL, 0xad17f088UL, 0xa01cfe81UL, 0x832dd4beUL, 0x8e26dab7UL, 0x993bc8acUL, 0x9430c6a5UL,
        0xdf599cd2UL, 0xd25292dbUL, 0xc54f80c0UL, 0xc8448ec9UL, 0xeb75a4f6UL, 0xe67eaaffUL, 0xf163b8e4UL, 0xfc68b6edUL,
        0x67b10c0aUL, 0x6aba0203UL, 0x7da71018UL, 0x70ac1e11UL, 0x539d342eUL, 0x5e963a27UL, 0x498b283cUL, 0x44802635UL,
        0x0fe97c42UL, 0x02e2724bUL, 0x15ff6050UL, 0x18f46e59UL, 0x3bc54466UL, 0x36ce4a6fUL, 0x21d35874UL, 0x2cd8567dUL,
        0x0c7a37a1UL, 0x017139a8UL, 0x166c2bb3UL, 0x1b6725baUL, 0x38560f85UL, 0x355d018cUL, 0x22401397UL, 0x2f4b1d9eUL,
        0x642247e9UL, 0x692949e0UL, 0x7e345bfbUL, 0x733f55f2UL, 0x500e7fcdUL, 0x5d0571c4UL, 0x4a1863dfUL, 0x47136dd6UL,
        0xdccad731UL, 0xd1c1d938UL, 0xc6dccb23UL, 0xcbd7c52aUL, 0xe8e6ef15UL, 0xe5ede11cUL, 0xf2f0f307UL, 0xfffbfd0eUL,
        0xb492a779UL, 0xb999a970UL, 0xae84bb6bUL, 0xa38fb562UL, 0x80be9f5dUL, 0x8db59154UL, 0x9aa8834fUL, 0x97a38d46UL
    };

    static const uint32_t Tks3[] = {
        0x00000000UL, 0x090d0b0eUL, 0x121a161cUL, 0x1b171d12UL, 0x24342c38UL, 0x2d392736UL, 0x362e3a24UL, 0x3f23312aUL,
        0x48685870UL, 0x4165537eUL, 0x5a724e6cUL, 0x537f4562UL, 0x6c5c7448UL, 0x65517f46UL, 0x7e466254UL, 0x774b695aUL,
        0x90d0b0e0UL, 0x99ddbbeeUL, 0x82caa6fcUL, 0x8bc7adf2UL, 0xb4e49cd8UL, 0xbde997d6UL, 0xa6fe8ac4UL, 0xaff381caUL,
        0xd8b8e890UL, 0xd1b5e39eUL, 0xcaa2fe8cUL, 0xc3aff582UL, 0xfc8cc4a8UL, 0xf581cfa6UL, 0xee96d2b4UL, 0xe79bd9baUL,
        0x3bbb7bdbUL, 0x32b670d5UL, 0x29a16dc7UL, 0x20ac66c9UL, 0x1f8f57e3UL, 0x16825cedUL, 0x0d9541ffUL, 0x04984af1UL,
        0x73d323abUL, 0x7ade28a5UL, 0x61c935b7UL, 0x68c43eb9UL, 0x57e70f93UL, 0x5eea049dUL, 0x45fd198fUL, 0x4cf01281UL,
        0xab6bcb3bUL, 0xa266c035UL, 0xb971dd27UL, 0xb07cd629UL, 0x8f5fe703UL, 0x8652ec0dUL, 0x9d45f11fUL, 0x9448fa11UL,
        0xe303934bUL, 0xea0e9845UL, 0xf1198557UL, 0xf8148e59UL, 0xc737bf73UL, 0xce3ab47dUL, 0xd52da96fUL, 0xdc20a261UL,
        0x766df6adUL, 0x7f60fda3UL, 0x6477e0b1UL, 0x6d7aebbfUL, 0x5259da95UL, 0x5b54d19bUL, 0x4043cc89UL, 0x494ec787UL,
        0x3e05aeddUL, 0x3708a5d3UL, 0x2c1fb8c1UL, 0x2512b3cfUL, 0x1a3182e5UL, 0x133c89ebUL, 0x082b94f9UL, 0x01269ff7UL,
        0xe6bd464dUL, 0xefb04d43UL, 0xf4a75051UL, 0xfdaa5b5fUL, 0xc2896a75UL, 0xcb84617bUL, 0xd0937c69UL, 0xd99e7767UL,
        0xaed51e3dUL, 0xa7d81533UL, 0xbccf0821UL, 0xb5c2032fUL, 0x8ae13205UL, 0x83ec390bUL, 0x98fb2419UL, 0x91f62f17UL,
        0x4dd68d76UL, 0x44db8678UL, 0x5fcc9b6aUL, 0x56c19064UL, 0x69e2a14eUL, 0x60efaa40UL, 0x7bf8b752UL, 0x72f5bc5cUL,
        0x05bed506UL, 0x0cb3de08UL, 0x17a4c31aUL, 0x1ea9c814UL, 0x218af93eUL, 0x2887f230UL, 0x3390ef22UL, 0x3a9de42cUL,
        0xdd063d96UL, 0xd40b3698UL, 0xcf1c2b8aUL, 0xc6112084UL, 0xf93211aeUL, 0xf03f1aa0UL, 0xeb2807b2UL, 0xe2250cbcUL,
        0x956e65e6UL, 0x9c636ee8UL, 0x877473faUL, 0x8e7978f4UL, 0xb15a49deUL, 0xb85742d0UL, 0xa3405fc2UL, 0xaa4d54ccUL,
        0xecdaf741UL, 0xe5d7fc4fUL, 0xfec0e15dUL, 0xf7cdea53UL, 0xc8eedb79UL, 0xc1e3d077UL, 0xdaf4cd65UL, 0xd3f9c66bUL,
        0xa4b2af31UL, 0xadbfa43fUL, 0xb6a8b92dUL, 0xbfa5b223UL, 0x80868309UL, 0x898b8807UL, 0x929c9515UL, 0x9b919e1bUL,
        0x7c0a47a1UL, 0x75074cafUL, 0x6e1051bdUL, 0x671d5ab3UL, 0x583e6b99UL, 0x51336097UL, 0x4a247d85UL, 0x4329768bUL,
        0x34621fd1UL, 0x3d6f14dfUL, 0x267809cdUL, 0x2f7502c3UL, 0x105633e9UL, 0x195b38e7UL, 0x024c25f5UL, 0x0b412efbUL,
        0xd7618c9aUL, 0xde6c8794UL, 0xc57b9a86UL, 0xcc769188UL, 0xf355a0a2UL, 0xfa58abacUL, 0xe14fb6beUL, 0xe842bdb0UL,
        0x9f09d4eaUL, 0x9604dfe4UL, 0x8d13c2f6UL, 0x841ec9f8UL, 0xbb3df8d2UL, 0xb230f3dcUL, 0xa927eeceUL, 0xa02ae5c0UL,
        0x47b13c7aUL, 0x4ebc3774UL, 0x55ab2a66UL, 0x5ca62168UL, 0x63851042UL, 0x6a881b4cUL, 0x719f065eUL, 0x78920d50UL,
        0x0fd9640aUL, 0x06d46f04UL, 0x1dc37216UL, 0x14ce7918UL, 0x2bed4832UL, 0x22e0433cUL, 0x39f75e2eUL, 0x30fa5520UL,
        0x9ab701ecUL, 0x93ba0ae2UL, 0x88ad17f0UL, 0x81a01cfeUL, 0xbe832dd4UL, 0xb78e26daUL, 0xac993bc8UL, 0xa59430c6UL,
        0xd2df599cUL, 0xdbd25292UL, 0xc0c54f80UL, 0xc9c8448eUL, 0xf6eb75a4UL, 0xffe67eaaUL, 0xe4f163b8UL, 0xedfc68b6UL,
        0x0a67b10cUL, 0x036aba02UL, 0x187da710UL, 0x1170ac1eUL, 0x2e539d34UL, 0x275e963aUL, 0x3c498b28UL, 0x35448026UL,
        0x420fe97cUL, 0x4b02e272UL, 0x5015ff60UL, 0x5918f46eUL, 0x663bc544UL, 0x6f36ce4aUL, 0x7421d358UL, 0x7d2cd856UL,
        0xa10c7a37UL, 0xa8017139UL, 0xb3166c2bUL, 0xba1b6725UL, 0x8538560fUL, 0x8c355d01UL, 0x97224013UL, 0x9e2f4b1dUL,
        0xe9642247UL, 0xe0692949UL, 0xfb7e345bUL, 0xf2733f55UL, 0xcd500e7fUL, 0xc45d0571UL, 0xdf4a1863UL, 0xd647136dUL,
        0x31dccad7UL, 0x38d1c1d9UL, 0x23c6dccbUL, 0x2acbd7c5UL, 0x15e8e6efUL, 0x1ce5ede1UL, 0x07f2f0f3UL, 0x0efffbfdUL,
        0x79b492a7UL, 0x70b999a9UL, 0x6bae84bbUL, 0x62a38fb5UL, 0x5d80be9fUL, 0x548db591UL, 0x4f9aa883UL, 0x4697a38dUL
    };

    static const uint32_t rcon[] = {
        0x01000000UL, 0x02000000UL, 0x04000000UL, 0x08000000UL, 0x10000000UL, 0x20000000UL, 0x40000000UL, 0x80000000UL,
        0x1B000000UL, 0x36000000UL,
    };
}


//----------------------------------------------------------------------------
// Various support elementary functions.
//----------------------------------------------------------------------------

namespace {
    
    #define BYTE(x,n) (((x) >> (8 * (n))) & 0xFF)

    inline __attribute__((always_inline)) uint32_t setup_mix(uint32_t temp)
    {
        return (Te4_3[BYTE(temp, 2)]) ^ (Te4_2[BYTE(temp, 1)]) ^ (Te4_1[BYTE(temp, 0)]) ^ (Te4_0[BYTE(temp, 3)]);
    }

    inline __attribute__((always_inline)) uint32_t byte_swap_32(uint32_t x)
    {
        asm("rev %w0, %w0" : "+r" (x)); return x;
    }

    inline __attribute__((always_inline)) uint32_t get_uint32_msb(const void* p)
    {
        // Assume little endian
        return byte_swap_32(*(static_cast<const uint32_t*>(p)));
    }

    inline __attribute__((always_inline)) uint32_t ror_32(uint32_t word, const int i)
    {
        asm("ror %w0, %w0, %w1" : "+r" (word) : "r" (i));
        return word;
    }

    inline __attribute__((always_inline)) void copy_16(uint8_t* dest, const uint8_t* value)
    {
        *reinterpret_cast<uint64_t*>(dest) = *reinterpret_cast<const uint64_t*>(value);
        *reinterpret_cast<uint64_t*>(dest + 8) = *reinterpret_cast<const uint64_t*>(value + 8);
    }

    inline __attribute__((always_inline)) void xor_16(uint8_t* update, const uint8_t* value)
    {
        *reinterpret_cast<uint64_t*>(update) ^= *reinterpret_cast<const uint64_t*>(value);
        *reinterpret_cast<uint64_t*>(update + 8) ^= *reinterpret_cast<const uint64_t*>(value + 8);
    }
}

//----------------------------------------------------------------------------
// Set a new key.
//----------------------------------------------------------------------------

bool arm64::aes::set_key(const uint8_t* key, size_t key_size)
{
    // Validate AES key size.
    if (key_size != 16 && key_size != 24 && key_size != 32) {
        return false;
    }

    // Expected number of rounds for key size
    key_data* kd = reinterpret_cast<key_data*>(_secret);
    const int rounds = int(10 + ((key_size / 8) - 2) * 2);
    kd->kbits = key_size * 8;

    int j;
    uint32_t temp;
    uint32_t* rk;
    uint32_t* rrk;
    uint32_t ekey[60];   // scheduled encryption keys
    uint32_t dkey[60];   // scheduled decryption keys

    // Setup the forward key
    int i = 0;
    rk = ekey;
    rk[0] = get_uint32_msb(key);
    rk[1] = get_uint32_msb(key +  4);
    rk[2] = get_uint32_msb(key +  8);
    rk[3] = get_uint32_msb(key + 12);

    if (key_size == 16) {
        j = 44;
        for (;;) {
            temp  = rk[3];
            rk[4] = rk[0] ^ setup_mix(temp) ^ rcon[i];
            rk[5] = rk[1] ^ rk[4];
            rk[6] = rk[2] ^ rk[5];
            rk[7] = rk[3] ^ rk[6];
            if (++i == 10) {
                break;
            }
            rk += 4;
        }
    }
    else if (key_size == 24) {
        j = 52;
        rk[4] = get_uint32_msb(key + 16);
        rk[5] = get_uint32_msb(key + 20);
        for (;;) {
            temp = rk[5];
            rk[ 6] = rk[ 0] ^ setup_mix(temp) ^ rcon[i];
            rk[ 7] = rk[ 1] ^ rk[ 6];
            rk[ 8] = rk[ 2] ^ rk[ 7];
            rk[ 9] = rk[ 3] ^ rk[ 8];
            if (++i == 8) {
                break;
            }
            rk[10] = rk[ 4] ^ rk[ 9];
            rk[11] = rk[ 5] ^ rk[10];
            rk += 6;
        }
    }
    else if (key_size == 32) {
        j = 60;
        rk[4] = get_uint32_msb(key + 16);
        rk[5] = get_uint32_msb(key + 20);
        rk[6] = get_uint32_msb(key + 24);
        rk[7] = get_uint32_msb(key + 28);
        for (;;) {
            temp = rk[7];
            rk[ 8] = rk[ 0] ^ setup_mix(temp) ^ rcon[i];
            rk[ 9] = rk[ 1] ^ rk[ 8];
            rk[10] = rk[ 2] ^ rk[ 9];
            rk[11] = rk[ 3] ^ rk[10];
            if (++i == 7) {
                break;
            }
            temp = rk[11];
            rk[12] = rk[ 4] ^ setup_mix(ror_32(temp, 8));
            rk[13] = rk[ 5] ^ rk[12];
            rk[14] = rk[ 6] ^ rk[13];
            rk[15] = rk[ 7] ^ rk[14];
            rk += 8;
        }
    }
    else {
        // Should not get there
        assert (false);
        return false;
    }

    // Setup the inverse key
    rk = dkey;
    rrk = ekey + j - 4;

    // Apply the inverse MixColumn transform to all round keys but the first and the last:
    // Copy first
    *rk++ = *rrk++;
    *rk++ = *rrk++;
    *rk++ = *rrk++;
    *rk   = *rrk;
    rk -= 3; rrk -= 3;

    for (i = 1; i < rounds; i++) {
        rrk -= 4;
        rk  += 4;
        temp = rrk[0];
        rk[0] = Tks0[BYTE(temp, 3)] ^ Tks1[BYTE(temp, 2)] ^ Tks2[BYTE(temp, 1)] ^ Tks3[BYTE(temp, 0)];
        temp = rrk[1];
        rk[1] = Tks0[BYTE(temp, 3)] ^ Tks1[BYTE(temp, 2)] ^ Tks2[BYTE(temp, 1)] ^ Tks3[BYTE(temp, 0)];
        temp = rrk[2];
        rk[2] = Tks0[BYTE(temp, 3)] ^ Tks1[BYTE(temp, 2)] ^ Tks2[BYTE(temp, 1)] ^ Tks3[BYTE(temp, 0)];
        temp = rrk[3];
        rk[3] = Tks0[BYTE(temp, 3)] ^ Tks1[BYTE(temp, 2)] ^ Tks2[BYTE(temp, 1)] ^ Tks3[BYTE(temp, 0)];
    }

    // Copy last
    rrk -= 4;
    rk  += 4;
    *rk++ = *rrk++;
    *rk++ = *rrk++;
    *rk++ = *rrk++;
    *rk   = *rrk;

    // Assume little endian here.
    // HW AES on little endian needs the subkeys to be byte reversed
    int max = (rounds + 1) * 4;
    for (int i = 0; i < max; ++i) {
        ekey[i] = byte_swap_32(ekey[i]);
        dkey[i] = byte_swap_32(dkey[i]);
    }
    
    const uint8_t* ek = reinterpret_cast<const uint8_t*>(ekey);
    const uint8_t* dk = reinterpret_cast<const uint8_t*>(dkey);
    for (int i = 0; i <= rounds; ++i) {
        kd->aekey[i] = vld1q_u8(ek + 16 * i);
        kd->adkey[i] = vld1q_u8(dk + 16 * i);
    }
    return true;
}

//----------------------------------------------------------------------------
// Encrypt one block.
//----------------------------------------------------------------------------

size_t arm64::aes::encrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    if (input_size != BLOCK_SIZE || output_maxsize < BLOCK_SIZE) {
        return 0;
    }

    const key_data* kd = reinterpret_cast<const key_data*>(_secret);
    uint8x16_t B = vld1q_u8(reinterpret_cast<const uint8_t*>(input));
    B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[0]));
    B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[1]));
    B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[2]));
    B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[3]));
    B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[4]));
    B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[5]));
    B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[6]));
    B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[7]));
    B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[8]));
    if (kd->kbits == 128) {
        B = veorq_u8(vaeseq_u8(B, kd->aekey[9]), kd->aekey[10]);
    }
    else {
        B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[9]));
        B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[10]));
        if (kd->kbits == 192) {
            B = veorq_u8(vaeseq_u8(B, kd->aekey[11]), kd->aekey[12]);
        }
        else {
            B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[11]));
            B = vaesmcq_u8(vaeseq_u8(B, kd->aekey[12]));
            B = veorq_u8(vaeseq_u8(B, kd->aekey[13]), kd->aekey[14]);
        }
    }
    vst1q_u8(reinterpret_cast<uint8_t*>(output), B);
    return BLOCK_SIZE;
}

//----------------------------------------------------------------------------
// Decrypt one block.
//----------------------------------------------------------------------------

size_t arm64::aes::decrypt(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    if (input_size != BLOCK_SIZE || output_maxsize < BLOCK_SIZE) {
        return 0;
    }

    const key_data* kd = reinterpret_cast<const key_data*>(_secret);
    uint8x16_t B = vld1q_u8(reinterpret_cast<const uint8_t*>(input));
    B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[0]));
    B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[1]));
    B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[2]));
    B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[3]));
    B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[4]));
    B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[5]));
    B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[6]));
    B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[7]));
    B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[8]));
    if (kd->kbits == 128) {
        B = veorq_u8(vaesdq_u8(B, kd->adkey[9]), kd->adkey[10]);
    }
    else {
        B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[9]));
        B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[10]));
        if (kd->kbits == 192) {
            B = veorq_u8(vaesdq_u8(B, kd->adkey[11]), kd->adkey[12]);
        }
        else {
            B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[11]));
            B = vaesimcq_u8(vaesdq_u8(B, kd->adkey[12]));
            B = veorq_u8(vaesdq_u8(B, kd->adkey[13]), kd->adkey[14]);
        }
    }
    vst1q_u8(reinterpret_cast<uint8_t*>(output), B);
    return BLOCK_SIZE;
}

//----------------------------------------------------------------------------
// Encrypt data in CBC mode, without padding.
//----------------------------------------------------------------------------

size_t arm64::aes::encrypt_cbc(uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    // Validate input sizes.
    if (iv_size != BLOCK_SIZE || input_size % BLOCK_SIZE != 0 || output_maxsize < input_size) {
        return 0;
    }

    // Encrypt block after block.
    const uint8_t* const input_end = input + input_size;
    while (input < input_end) {
        xor_16(iv, input);
        encrypt(iv, BLOCK_SIZE, output, BLOCK_SIZE);
        copy_16(iv, output);
        input += BLOCK_SIZE;
        output += BLOCK_SIZE;
    }
    return input_size;
}

//----------------------------------------------------------------------------
// Decrypt data in CBC mode, without padding.
//----------------------------------------------------------------------------

size_t arm64::aes::decrypt_cbc(uint8_t* iv, size_t iv_size, const uint8_t* input, size_t input_size, uint8_t* output, size_t output_maxsize)
{
    // Validate input sizes.
    if (iv_size != BLOCK_SIZE || input_size % BLOCK_SIZE != 0 || output_maxsize < input_size) {
        return 0;
    }

    // Decrypt block after block.
    const uint8_t* const input_end = input + input_size;
    while (input < input_end) {
        decrypt(input, BLOCK_SIZE, output, BLOCK_SIZE);
        xor_16(output, iv);
        copy_16(iv, input);
        input += BLOCK_SIZE;
        output += BLOCK_SIZE;
    }
    return input_size;
}

#endif // __ARM_FEATURE_CRYPTO
