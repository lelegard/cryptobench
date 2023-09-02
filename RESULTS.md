# Comparative benchmarks results

**Contents:**

* [Results overview](#results-overview)
* [RSA-2048](#rsa-2048)
  * [RSA-2048 encryption without rekeying](#rsa-2048-encryption-without-rekeying)
  * [RSA-2048 decryption without rekeying](#rsa-2048-decryption-without-rekeying)
  * [RSA-2048 decryption/encryption ratio, without rekeying](#rsa-2048-decryptionencryption-ratio-without-rekeying)
  * [RSA-2048 encryption with rekeying](#rsa-2048-encryption-with-rekeying)
  * [RSA-2048 decryption with rekeying](#rsa-2048-decryption-with-rekeying)
  * [RSA-2048 decryption/encryption ratio, with rekeying](#rsa-2048-decryptionencryption-ratio-with-rekeying)
  * [RSA-2048 signature without rekeying](#rsa-2048-signature-without-rekeying)
  * [RSA-2048 verification without rekeying](#rsa-2048-verification-without-rekeying)
  * [RSA-2048 signature/verification ratio, without rekeying](#rsa-2048-signatureverification-ratio-without-rekeying)
  * [RSA-2048 signature with rekeying](#rsa-2048-signature-with-rekeying)
  * [RSA-2048 verification with rekeying](#rsa-2048-verification-with-rekeying)
  * [RSA-2048 signature/verification ratio, with rekeying](#rsa-2048-signatureverification-ratio-with-rekeying)
* [RSA-4096](#rsa-4096)
  * [RSA-4096 encryption without rekeying](#rsa-4096-encryption-without-rekeying)
  * [RSA-4096 decryption without rekeying](#rsa-4096-decryption-without-rekeying)
  * [RSA-4096 decryption/encryption ratio, without rekeying](#rsa-4096-decryptionencryption-ratio-without-rekeying)
  * [RSA-4096 encryption with rekeying](#rsa-4096-encryption-with-rekeying)
  * [RSA-4096 decryption with rekeying](#rsa-4096-decryption-with-rekeying)
  * [RSA-4096 decryption/encryption ratio, with rekeying](#rsa-4096-decryptionencryption-ratio-with-rekeying)
  * [RSA-4096 signature without rekeying](#rsa-4096-signature-without-rekeying)
  * [RSA-4096 verification without rekeying](#rsa-4096-verification-without-rekeying)
  * [RSA-4096 signature/verification ratio, without rekeying](#rsa-4096-signatureverification-ratio-without-rekeying)
  * [RSA-4096 signature with rekeying](#rsa-4096-signature-with-rekeying)
  * [RSA-4096 verification with rekeying](#rsa-4096-verification-with-rekeying)
  * [RSA-4096 signature/verification ratio, with rekeying](#rsa-4096-signatureverification-ratio-with-rekeying)
* [AES-128](#aes-128)
  * [AES-128-CBC encryption](#aes-128-cbc-encryption)
  * [AES-128-CBC decryption](#aes-128-cbc-decryption)
  * [AES-128-CBC encryption/decryption ratio](#aes-128-cbc-encryptiondecryption-ratio)
* [AES-256](#aes-256)
  * [AES-256-CBC encryption](#aes-256-cbc-encryption)
  * [AES-256-CBC decryption](#aes-256-cbc-decryption)
  * [AES-256-CBC encryption/decryption ratio](#aes-256-cbc-encryptiondecryption-ratio)

## Results overview

The presented numbers are relative execution times (the lower, the better).

The values have no unit and their absolute value is meaningless. They are
relative to the performance of the CPU. A standard reference test is run on
each CPU before running the cryptographic operations. The presented number
is the ratio of the execution time of the evaluated cryptographic operation
over the execution time of the standard test.

Thus, if all CPU's had the same implementation and only differed in speed,
the score of one operation would be the same on all CPU's.

A lower relative execution time on a CPU maybe due to several reasons:
- Better performance of the instructions which are characteristic of that operation.
- Dedicated accelerated instructions for that operation (e.g. AES and SHA-x).
- More efficient compiler (gcc is used on Linux, clang on macOS).
- More efficient code generation options to produce the binary of the library.

Note: In addition to relative execution times, we also present ratio between
encryption and decryption or signature and verification.

Due to the level of support of the different operating systems, it was not
possible to use the same version of each library on all systems. Therefore,
if it possible that some differences of performance are due to different
implementations in different versions of the same library. See the corresponding
[report file](reports) for the exact version of each library.

The tested cryptographic libraries are:
- OpenSSL (mostly version 3, version 1 on older systems).
- MbedTLS
- GnuTLS
- Nettle
- Libtomcrypt with libtommath backend
- Libtomcrypt with GMP backend (only relevant for asymmetric cryptography).
- Arm64, an implementation of AES using Arm64 specialized accelerated instructions
  through C/C++ intrinsics. It is run only on Arm64 processors supporting FEAT_AES.
  Note that some cryptographic libraries also uses such an implementation.

In the result tables, Intel/AMD processors come first, them Arm processors.

## RSA-2048

### RSA-2048 encryption without rekeying

| CPU                | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt/ltm | Libtomcrypt/gmp |
| ------------------ | :---: | :-----: | :-----: | :----: | :----: | :-------------: | :-------------: |
| i7-5775R           | macOS | 5.9     |         |        |        |                 |                 |
| i7-5775R           | Linux | 4.3     |         |        |        |                 |                 |
| i7-8565U           | Linux | 5.1     |         |        |        |                 |                 |
| Cortex A53 (R.Pi3) | Linux | 10.3    |         |        |        |                 |                 |
| Cortex A72 (R.Pi4) | Linux | 8.6     |         |        |        |                 |                 |
| Apple M1           | macOS | 5.2     |         |        |        |                 |                 |
| Apple M1           | Linux | 3.2     |         |        |        |                 |                 |

### RSA-2048 decryption without rekeying

| CPU                | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt/ltm | Libtomcrypt/gmp |
| ------------------ | :---: | :-----: | :-----: | :----: | :----: | :-------------: | :-------------: |
| i7-5775R           | macOS |         |         |        |        |                 |                 |
| i7-5775R           | Linux |         |         |        |        |                 |                 |
| i7-8565U           | Linux |         |         |        |        |                 |                 |
| Cortex A53 (R.Pi3) | Linux |         |         |        |        |                 |                 |
| Cortex A72 (R.Pi4) | Linux |         |         |        |        |                 |                 |
| Apple M1           | macOS |         |         |        |        |                 |                 |
| Apple M1           | Linux |         |         |        |        |                 |                 |

### RSA-2048 decryption/encryption ratio, without rekeying

### RSA-2048 encryption with rekeying

### RSA-2048 decryption with rekeying

### RSA-2048 decryption/encryption ratio, with rekeying

### RSA-2048 signature without rekeying

### RSA-2048 verification without rekeying

### RSA-2048 signature/verification ratio, without rekeying

### RSA-2048 signature with rekeying

### RSA-2048 verification with rekeying

### RSA-2048 signature/verification ratio, with rekeying

## RSA-4096

### RSA-4096 encryption without rekeying

### RSA-4096 decryption without rekeying

### RSA-4096 decryption/encryption ratio, without rekeying

### RSA-4096 encryption with rekeying

### RSA-4096 decryption with rekeying

### RSA-4096 decryption/encryption ratio, with rekeying

### RSA-4096 signature without rekeying

### RSA-4096 verification without rekeying

### RSA-4096 signature/verification ratio, without rekeying

### RSA-4096 signature with rekeying

### RSA-4096 verification with rekeying

### RSA-4096 signature/verification ratio, with rekeying

## AES-128

### AES-128-CBC encryption

### AES-128-CBC decryption

### AES-128-CBC encryption/decryption ratio

## AES-256

### AES-256 encryption

### AES-256 decryption

### AES-256 encryption/decryption ratio
