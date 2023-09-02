# Comparative benchmarks results

The presented numbers are relative execution time (the lower, the better).

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

### Encryption without rekeying

| CPU                | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt/ltm | Libtomcrypt/gmp |
| ------------------ | :---: | :-----: | :-----: | :----: | :----: | :-------------: | :-------------: |
| i7-5775R           | macOS | 5.9     |         |        |        |                 |                 |
| i7-5775R           | Linux | 4.3     |         |        |        |                 |                 |
| i7-8565U           | Linux | 5.1     |         |        |        |                 |                 |
| Cortex A53 (R.Pi3) | Linux | 10.3    |         |        |        |                 |                 |
| Cortex A72 (R.Pi4) | Linux | 8.6     |         |        |        |                 |                 |
| Apple M1           | macOS | 5.2     |         |        |        |                 |                 |
| Apple M1           | Linux | 3.2     |         |        |        |                 |                 |

### Decryption without rekeying

| CPU                | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt/ltm | Libtomcrypt/gmp |
| ------------------ | :---: | :-----: | :-----: | :----: | :----: | :-------------: | :-------------: |
| i7-5775R           | macOS |         |         |        |        |                 |                 |
| i7-5775R           | Linux |         |         |        |        |                 |                 |
| i7-8565U           | Linux |         |         |        |        |                 |                 |
| Cortex A53 (R.Pi3) | Linux |         |         |        |        |                 |                 |
| Cortex A72 (R.Pi4) | Linux |         |         |        |        |                 |                 |
| Apple M1           | macOS |         |         |        |        |                 |                 |
| Apple M1           | Linux |         |         |        |        |                 |                 |

### Decryption/encryption ratio, without rekeying

### Encryption with rekeying

### Decryption with rekeying

### Decryption/encryption ratio, with rekeying

### Signature without rekeying

### Verification without rekeying

### Signature/Verification ratio, without rekeying

### Signature with rekeying

### Verification with rekeying

### Signature/Verification ratio, with rekeying

## RSA-4096

### Encryption without rekeying

### Decryption without rekeying

### Decryption/encryption ratio, without rekeying

### Encryption with rekeying

### Decryption with rekeying

### Decryption/encryption ratio, with rekeying

### Signature without rekeying

### Verification without rekeying

### Signature/Verification ratio, without rekeying

### Signature with rekeying

### Verification with rekeying

### Signature/Verification ratio, with rekeying

## AES-128

### Encryption

### Decryption

### Encryption/decryption ratio

## AES-256

### Encryption

### Decryption

### Encryption/decryption ratio
