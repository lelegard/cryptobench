# Comparative benchmarks results

**Contents:**
* [Results overview](#results-overview)
* [RSA-2048](#rsa-2048)
  * [RSA-2048 encryption](#rsa-2048-encryption)
  * [RSA-2048 decryption](#rsa-2048-decryption)
  * [RSA-2048 decryption/encryption ratio](#rsa-2048-decryptionencryption-ratio)
  * [RSA-2048 encryption with rekeying](#rsa-2048-encryption-with-rekeying)
  * [RSA-2048 decryption with rekeying](#rsa-2048-decryption-with-rekeying)
  * [RSA-2048 decryption/encryption ratio, with rekeying](#rsa-2048-decryptionencryption-ratio-with-rekeying)
  * [RSA-2048 signature](#rsa-2048-signature)
  * [RSA-2048 verification](#rsa-2048-verification)
  * [RSA-2048 signature/verification ratio](#rsa-2048-signatureverification-ratio)
  * [RSA-2048 signature with rekeying](#rsa-2048-signature-with-rekeying)
  * [RSA-2048 verification with rekeying](#rsa-2048-verification-with-rekeying)
  * [RSA-2048 signature/verification ratio, with rekeying](#rsa-2048-signatureverification-ratio-with-rekeying)
  * [RSA-2048 parse public key](#rsa-2048-parse-public-key)
  * [RSA-2048 parse private key](#rsa-2048-parse-private-key)
* [RSA-4096](#rsa-4096)
  * [RSA-4096 encryption](#rsa-4096-encryption)
  * [RSA-4096 decryption](#rsa-4096-decryption)
  * [RSA-4096 decryption/encryption ratio](#rsa-4096-decryptionencryption-ratio)
  * [RSA-4096 encryption with rekeying](#rsa-4096-encryption-with-rekeying)
  * [RSA-4096 decryption with rekeying](#rsa-4096-decryption-with-rekeying)
  * [RSA-4096 decryption/encryption ratio, with rekeying](#rsa-4096-decryptionencryption-ratio-with-rekeying)
  * [RSA-4096 signature](#rsa-4096-signature)
  * [RSA-4096 verification](#rsa-4096-verification)
  * [RSA-4096 signature/verification ratio](#rsa-4096-signatureverification-ratio)
  * [RSA-4096 signature with rekeying](#rsa-4096-signature-with-rekeying)
  * [RSA-4096 verification with rekeying](#rsa-4096-verification-with-rekeying)
  * [RSA-4096 signature/verification ratio, with rekeying](#rsa-4096-signatureverification-ratio-with-rekeying)
  * [RSA-4096 parse public key](#rsa-4096-parse-public-key)
  * [RSA-4096 parse private key](#rsa-4096-parse-private-key)
* [AES-128-CBC](#aes-128-cbc)
  * [AES-128-CBC encryption](#aes-128-cbc-encryption)
  * [AES-128-CBC decryption](#aes-128-cbc-decryption)
  * [AES-128-CBC encryption/decryption ratio](#aes-128-cbc-encryptiondecryption-ratio)
* [AES-256-CBC](#aes-256-cbc)
  * [AES-256-CBC encryption](#aes-256-cbc-encryption)
  * [AES-256-CBC decryption](#aes-256-cbc-decryption)
  * [AES-256-CBC encryption/decryption ratio](#aes-256-cbc-encryptiondecryption-ratio)
* [2048-bit modular arithmetic](#2048-bit-modular-arithmetic)

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

### RSA-2048 encryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 5.9     | 16.6    | 7.2    | 26          | 8.7               |
| i7-5775R                     | Linux | 4.3     | 11.4    | 5.2    | 16.5        | 7.2               |
| i7-8565U                     | Linux | 4.9     | 9.7     | 4.6    | 13.7        | 5.3               |
| Xeon-6254                    | Linux | 2.9     | 7.6     | 3.4    | 12.0        |                   |
| Cortex A53 (R.Pi3)           | Linux | 10.3    | 17.7    | 8.9    | 25          | 9.3               |
| Cortex A72 (R.Pi4)           | Linux | 8.6     | 11.5    | 7.1    | 16.5        | 7.7               |
| Apple M1                     | macOS | 5.3     | 12.0    | 5.6    | 18.0        | 7.2               |
| Apple M1                     | Linux | 3.1     | 7.2     | 3.7    | 10.4        | 4.5               |
| Ampere Altra (Neoverse N1)   | Linux | 7.8     | 11.2    | 7.1    | 15.0        | 7.6               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.2     | 7.5     | 3.5    | 9.0         | 3.9               |

### RSA-2048 decryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 160     | 724     | 480    | 683         | 228               |
| i7-5775R                     | Linux | 117     | 1087    | 388    | 421         | 182               |
| i7-8565U                     | Linux | 97      | 894     | 329    | 356         | 148               |
| Xeon-6254                    | Linux | 216     | 361     | 225    | 289         |                   |
| Cortex A53 (R.Pi3)           | Linux | 355     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 278     | 922     | 369    | 427         | 242               |
| Apple M1                     | macOS | 192     | 490     | 353    | 482         | 203               |
| Apple M1                     | Linux | 112     | 707     | 231    | 280         | 139               |
| Ampere Altra (Neoverse N1)   | Linux | 273     | 436     | 346    | 396         | 245               |
| AWS Graviton 3 (Neoverse V1) | Linux | 116     | 306     | 201    | 245         | 114               |

### RSA-2048 decryption/encryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 27      | 43      | 66     | 25          | 26                |
| i7-5775R                     | Linux | 27      | 95      | 74     | 25          | 25                |
| i7-8565U                     | Linux | 19.8    | 92      | 71     | 25          | 28                |
| Xeon-6254                    | Linux | 74      | 47      | 66     | 24          |                   |
| Cortex A53 (R.Pi3)           | Linux | 34      | 44      | 53     | 26          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 32      | 80      | 52     | 25          | 31                |
| Apple M1                     | macOS | 36      | 40      | 62     | 26          | 28                |
| Apple M1                     | Linux | 36      | 98      | 63     | 27          | 30                |
| Ampere Altra (Neoverse N1)   | Linux | 34      | 38      | 48     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 35      | 40      | 57     | 27          | 29                |

### RSA-2048 encryption with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 6.7     | 16.8    | 7.1    | 26          | 8.6               |
| i7-5775R                     | Linux | 5.2     | 11.3    | 5.2    | 16.5        | 6.0               |
| i7-8565U                     | Linux | 5.5     | 9.6     | 4.7    | 13.2        | 5.5               |
| Xeon-6254                    | Linux | 2.9     | 7.6     | 3.4    | 12.0        |                   |
| Cortex A53 (R.Pi3)           | Linux | 11.0    | 17.7    | 8.9    | 25          | 9.3               |
| Cortex A72 (R.Pi4)           | Linux | 9.5     | 11.5    | 7.1    | 16.5        | 7.7               |
| Apple M1                     | macOS | 5.8     | 12.0    | 5.6    | 18.0        | 7.2               |
| Apple M1                     | Linux | 3.4     | 7.2     | 3.7    | 10.1        | 4.5               |
| Ampere Altra (Neoverse N1)   | Linux | 8.2     | 11.2    | 7.1    | 15.0        | 7.6               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.6     | 7.5     | 3.5    | 8.9         | 4.0               |

### RSA-2048 decryption with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 161     | 728     | 475    | 686         | 233               |
| i7-5775R                     | Linux | 122     | 1055    | 390    | 415         | 166               |
| i7-8565U                     | Linux | 98      | 908     | 329    | 360         | 149               |
| Xeon-6254                    | Linux | 215     | 363     | 226    | 289         |                   |
| Cortex A53 (R.Pi3)           | Linux | 357     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 279     | 923     | 369    | 427         | 242               |
| Apple M1                     | macOS | 193     | 490     | 353    | 482         | 203               |
| Apple M1                     | Linux | 113     | 707     | 235    | 281         | 140               |
| Ampere Altra (Neoverse N1)   | Linux | 273     | 436     | 346    | 394         | 244               |
| AWS Graviton 3 (Neoverse V1) | Linux | 116     | 306     | 201    | 245         | 114               |

### RSA-2048 decryption/encryption ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 24      | 43      | 66     | 26          | 27                |
| i7-5775R                     | Linux | 23      | 93      | 75     | 25          | 27                |
| i7-8565U                     | Linux | 17.8    | 94      | 70     | 27          | 26                |
| Xeon-6254                    | Linux | 74      | 47      | 66     | 24          |                   |
| Cortex A53 (R.Pi3)           | Linux | 32      | 44      | 53     | 26          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 29      | 80      | 52     | 25          | 31                |
| Apple M1                     | macOS | 33      | 40      | 62     | 26          | 28                |
| Apple M1                     | Linux | 32      | 98      | 63     | 27          | 30                |
| Ampere Altra (Neoverse N1)   | Linux | 33      | 38      | 48     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 32      | 40      | 57     | 27          | 29                |

### RSA-2048 signature

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 159     | 719     | 472    | 683         | 232               |
| i7-5775R                     | Linux | 114     | 1067    | 388    | 423         | 165               |
| i7-8565U                     | Linux | 103     | 873     | 329    | 365         | 146               |
| Xeon-6254                    | Linux | 77      | 354     | 226    | 289         |                   |
| Cortex A53 (R.Pi3)           | Linux | 356     | 790     | 476    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 279     | 920     | 369    | 427         | 242               |
| Apple M1                     | macOS | 192     | 489     | 354    | 482         | 202               |
| Apple M1                     | Linux | 112     | 708     | 232    | 278         | 134               |
| Ampere Altra (Neoverse N1)   | Linux | 273     | 436     | 345    | 395         | 245               |
| AWS Graviton 3 (Neoverse V1) | Linux | 115     | 306     | 201    | 245         | 114               |

### RSA-2048 verification

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 5.7     | 16.1    | 7.9    | 27          | 8.4               |
| i7-5775R                     | Linux | 3.9     | 10.9    | 5.7    | 17.6        | 5.9               |
| i7-8565U                     | Linux | 4.7     | 9.3     | 5.2    | 13.4        | 5.2               |
| Xeon-6254                    | Linux | 2.6     | 7.4     | 3.7    | 11.8        |                   |
| Cortex A53 (R.Pi3)           | Linux | 10.0    | 17.5    | 9.1    | 25          | 9.2               |
| Cortex A72 (R.Pi4)           | Linux | 7.8     | 11.3    | 7.3    | 16.3        | 7.6               |
| Apple M1                     | macOS | 5.0     | 11.7    | 6.1    | 17.5        | 6.8               |
| Apple M1                     | Linux | 2.9     | 6.9     | 3.7    | 9.8         | 4.3               |
| Ampere Altra (Neoverse N1)   | Linux | 7.6     | 11.1    | 7.3    | 14.9        | 7.5               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.0     | 7.3     | 3.6    | 8.8         | 3.8               |

### RSA-2048 signature/verification ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 28      | 44      | 60     | 25          | 27                |
| i7-5775R                     | Linux | 29      | 97      | 68     | 24          | 27                |
| i7-8565U                     | Linux | 22      | 93      | 63     | 27          | 28                |
| Xeon-6254                    | Linux | 29      | 47      | 61     | 24          |                   |
| Cortex A53 (R.Pi3)           | Linux | 35      | 45      | 52     | 25          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 35      | 81      | 50     | 26          | 31                |
| Apple M1                     | macOS | 38      | 41      | 57     | 27          | 29                |
| Apple M1                     | Linux | 38      | 102     | 62     | 28          | 30                |
| Ampere Altra (Neoverse N1)   | Linux | 36      | 39      | 47     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 38      | 41      | 55     | 27          | 30                |

### RSA-2048 signature with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 207     | 717     | 473    | 716         | 229               |
| i7-5775R                     | Linux | 114     | 1052    | 388    | 425         | 167               |
| i7-8565U                     | Linux | 104     | 897     | 325    | 369         | 147               |
| Xeon-6254                    | Linux | 77      | 355     | 226    | 290         |                   |
| Cortex A53 (R.Pi3)           | Linux | 357     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 280     | 919     | 369    | 427         | 241               |
| Apple M1                     | macOS | 192     | 489     | 354    | 482         | 202               |
| Apple M1                     | Linux | 113     | 710     | 236    | 283         | 138               |
| Ampere Altra (Neoverse N1)   | Linux | 273     | 436     | 346    | 395         | 245               |
| AWS Graviton 3 (Neoverse V1) | Linux | 116     | 306     | 201    | 245         | 114               |

### RSA-2048 verification with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 6.7     | 16.3    | 7.8    | 28          | 8.3               |
| i7-5775R                     | Linux | 4.9     | 10.7    | 5.6    | 15.9        | 5.9               |
| i7-8565U                     | Linux | 5.6     | 9.5     | 5.2    | 13.4        | 5.3               |
| Xeon-6254                    | Linux | 2.6     | 7.4     | 3.7    | 11.8        |                   |
| Cortex A53 (R.Pi3)           | Linux | 10.9    | 17.5    | 9.1    | 25          | 9.2               |
| Cortex A72 (R.Pi4)           | Linux | 8.9     | 11.3    | 7.3    | 16.3        | 7.6               |
| Apple M1                     | macOS | 5.6     | 11.7    | 6.1    | 17.5        | 6.8               |
| Apple M1                     | Linux | 3.4     | 6.9     | 3.7    | 9.9         | 4.3               |
| Ampere Altra (Neoverse N1)   | Linux | 8.1     | 11.1    | 7.3    | 14.9        | 7.5               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.5     | 7.3     | 3.6    | 8.8         | 3.8               |

### RSA-2048 signature/verification ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 30      | 43      | 60     | 24          | 27                |
| i7-5775R                     | Linux | 23      | 97      | 69     | 26          | 28                |
| i7-8565U                     | Linux | 18.5    | 94      | 62     | 27          | 27                |
| Xeon-6254                    | Linux | 29      | 48      | 61     | 24          |                   |
| Cortex A53 (R.Pi3)           | Linux | 32      | 45      | 52     | 26          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 31      | 81      | 50     | 26          | 31                |
| Apple M1                     | macOS | 34      | 41      | 57     | 27          | 29                |
| Apple M1                     | Linux | 33      | 102     | 64     | 28          | 32                |
| Ampere Altra (Neoverse N1)   | Linux | 34      | 39      | 47     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 33      | 41      | 55     | 27          | 30                |

### RSA-2048 parse public key

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 18.0    | 0.30    | 1.2    | 5.4         | 2.1               |
| i7-5775R                     | Linux | 29      | 0.15    | 0.60   | 3.1         | 1.3               |
| i7-8565U                     | Linux | 23      | 0.10    | 0.55   | 2.3         | 1.1               |
| Xeon-6254                    | Linux | 0.18    | 0.088   | 0.40   | 2.1         |                   |
| Cortex A53 (R.Pi3)           | Linux | 20      | 0.11    | 0.47   | 1.9         | 0.58              |
| Cortex A72 (R.Pi4)           | Linux | 24      | 0.11    | 0.54   | 1.6         | 0.74              |
| Apple M1                     | macOS | 10.1    | 0.16    | 0.67   | 3.3         | 2.1               |
| Apple M1                     | Linux | 15.4    | 0.11    | 0.35   | 2.0         | 1.3               |
| Ampere Altra (Neoverse N1)   | Linux | 36      | 0.073   | 0.30   | 1.1         | 0.57              |
| AWS Graviton 3 (Neoverse V1) | Linux | 45      | 0.080   | 0.31   | 1.1         | 0.57              |

### RSA-2048 parse private key

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 20      | 11.5    | 10.1   | 9.9         | 1.5               |
| i7-5775R                     | Linux | 30      | 6.1     | 9.0    | 7.0         | 0.92              |
| i7-8565U                     | Linux | 24      | 5.1     | 5.6    | 5.1         | 0.96              |
| Xeon-6254                    | Linux | 0.53    | 4.2     | 3.6    | 5.0         |                   |
| Cortex A53 (R.Pi3)           | Linux | 20      | 8.4     | 4.2    | 5.0         | 0.53              |
| Cortex A72 (R.Pi4)           | Linux | 24      | 5.5     | 4.9    | 3.8         | 0.63              |
| Apple M1                     | macOS | 11.2    | 7.6     | 6.2    | 5.1         | 0.88              |
| Apple M1                     | Linux | 16.0    | 4.3     | 3.8    | 3.1         | 0.56              |
| Ampere Altra (Neoverse N1)   | Linux | 36      | 3.3     | 3.0    | 2.5         | 0.42              |
| AWS Graviton 3 (Neoverse V1) | Linux | 46      | 2.9     | 2.8    | 2.5         | 0.42              |

## RSA-4096

### RSA-4096 encryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 17.9    | 57      | 23     | 89          | 26                |
| i7-5775R                     | Linux | 13.1    | 39      | 17.3   | 53          | 19.3              |
| i7-8565U                     | Linux | 17.3    | 33      | 15.8   | 45          | 16.7              |
| Xeon-6254                    | Linux | 8.8     | 25      | 11.7   | 39          |                   |
| Cortex A53 (R.Pi3)           | Linux | 38      | 65      | 29     | 92          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 26      | 41      | 21     | 56          | 22                |
| Apple M1                     | macOS | 18.6    | 43      | 20     | 61          | 23                |
| Apple M1                     | Linux | 11.1    | 27      | 14.0   | 31          | 15.3              |
| Ampere Altra (Neoverse N1)   | Linux | 24      | 41      | 21     | 54          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.4    | 26      | 11.4   | 28          | 12.2              |

### RSA-4096 decryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1058    | 4279    | 2555   | 3591        | 1614              |
| i7-5775R                     | Linux | 770     | 4219    | 2496   | 2175        | 1160              |
| i7-8565U                     | Linux | 1023    | 3661    | 1819   | 1853        | 1021              |
| Xeon-6254                    | Linux | 1055    | 2019    | 1424   | 1545        |                   |
| Cortex A53 (R.Pi3)           | Linux | 2481    | 5044    | 2853   | 4140        | 2113              |
| Cortex A72 (R.Pi4)           | Linux | 1933    | 4076    | 2763   | 2533        | 1647              |
| Apple M1                     | macOS | 1223    | 3008    | 2122   | 2480        | 1281              |
| Apple M1                     | Linux | 737     | 2912    | 1412   | 1464        | 879               |
| Ampere Altra (Neoverse N1)   | Linux | 1938    | 2725    | 2720   | 2469        | 1676              |
| AWS Graviton 3 (Neoverse V1) | Linux | 757     | 1820    | 1393   | 1305        | 792               |

### RSA-4096 decryption/encryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 59      | 75      | 109    | 40          | 61                |
| i7-5775R                     | Linux | 58      | 107     | 144    | 40          | 60                |
| i7-8565U                     | Linux | 59      | 108     | 115    | 40          | 61                |
| Xeon-6254                    | Linux | 120     | 78      | 121    | 38          |                   |
| Cortex A53 (R.Pi3)           | Linux | 65      | 76      | 95     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 72      | 98      | 127    | 44          | 71                |
| Apple M1                     | macOS | 65      | 69      | 102    | 40          | 54                |
| Apple M1                     | Linux | 66      | 105     | 100    | 46          | 57                |
| Ampere Altra (Neoverse N1)   | Linux | 79      | 65      | 129    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 66      | 67      | 122    | 45          | 64                |

### RSA-4096 encryption with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 19.2    | 56      | 23     | 88          | 26                |
| i7-5775R                     | Linux | 14.0    | 38      | 20     | 52          | 19.6              |
| i7-8565U                     | Linux | 17.8    | 34      | 15.6   | 43          | 17.0              |
| Xeon-6254                    | Linux | 8.8     | 25      | 11.7   | 39          |                   |
| Cortex A53 (R.Pi3)           | Linux | 38      | 65      | 29     | 91          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 27      | 41      | 21     | 56          | 22                |
| Apple M1                     | macOS | 19.2    | 43      | 20     | 61          | 23                |
| Apple M1                     | Linux | 11.6    | 27      | 14.3   | 32          | 15.4              |
| Ampere Altra (Neoverse N1)   | Linux | 24      | 41      | 21     | 54          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.8    | 26      | 11.4   | 28          | 12.2              |

### RSA-4096 decryption with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1064    | 4192    | 2555   | 3553        | 1604              |
| i7-5775R                     | Linux | 761     | 4223    | 2207   | 2180        | 1167              |
| i7-8565U                     | Linux | 1052    | 3636    | 1770   | 1845        | 1037              |
| Xeon-6254                    | Linux | 1052    | 2019    | 1435   | 1543        |                   |
| Cortex A53 (R.Pi3)           | Linux | 2491    | 5056    | 2853   | 4139        | 2112              |
| Cortex A72 (R.Pi4)           | Linux | 1937    | 4080    | 2763   | 2532        | 1648              |
| Apple M1                     | macOS | 1224    | 2991    | 2120   | 2482        | 1280              |
| Apple M1                     | Linux | 743     | 2904    | 1392   | 1419        | 847               |
| Ampere Altra (Neoverse N1)   | Linux | 1939    | 2725    | 2726   | 2457        | 1678              |
| AWS Graviton 3 (Neoverse V1) | Linux | 757     | 1820    | 1393   | 1305        | 792               |

### RSA-4096 decryption/encryption ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 55      | 74      | 109    | 40          | 61                |
| i7-5775R                     | Linux | 54      | 108     | 108    | 41          | 59                |
| i7-8565U                     | Linux | 59      | 104     | 113    | 42          | 61                |
| Xeon-6254                    | Linux | 119     | 77      | 122    | 38          |                   |
| Cortex A53 (R.Pi3)           | Linux | 64      | 76      | 95     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 69      | 98      | 127    | 44          | 71                |
| Apple M1                     | macOS | 63      | 69      | 102    | 40          | 54                |
| Apple M1                     | Linux | 64      | 105     | 97     | 44          | 55                |
| Ampere Altra (Neoverse N1)   | Linux | 78      | 65      | 129    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 64      | 67      | 122    | 45          | 64                |

### RSA-4096 signature

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1044    | 4295    | 2579   | 3620        | 1604              |
| i7-5775R                     | Linux | 798     | 4213    | 2068   | 2185        | 1225              |
| i7-8565U                     | Linux | 1014    | 3760    | 1759   | 1847        | 1040              |
| Xeon-6254                    | Linux | 515     | 1998    | 1429   | 1544        |                   |
| Cortex A53 (R.Pi3)           | Linux | 2489    | 5049    | 2853   | 4141        | 2113              |
| Cortex A72 (R.Pi4)           | Linux | 1933    | 4075    | 2762   | 2530        | 1647              |
| Apple M1                     | macOS | 1222    | 3009    | 2120   | 2482        | 1274              |
| Apple M1                     | Linux | 742     | 2947    | 1412   | 1474        | 882               |
| Ampere Altra (Neoverse N1)   | Linux | 1938    | 2724    | 2717   | 2468        | 1670              |
| AWS Graviton 3 (Neoverse V1) | Linux | 756     | 1820    | 1394   | 1305        | 791               |

### RSA-4096 verification

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 17.4    | 56      | 24     | 89          | 25                |
| i7-5775R                     | Linux | 13.0    | 37      | 18.0   | 53          | 20                |
| i7-8565U                     | Linux | 16.5    | 33      | 16.2   | 47          | 16.4              |
| Xeon-6254                    | Linux | 8.6     | 25      | 12.2   | 39          |                   |
| Cortex A53 (R.Pi3)           | Linux | 37      | 65      | 30     | 91          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 25      | 41      | 22     | 56          | 22                |
| Apple M1                     | macOS | 18.2    | 42      | 22     | 60          | 23                |
| Apple M1                     | Linux | 11.0    | 27      | 13.9   | 31          | 14.7              |
| Ampere Altra (Neoverse N1)   | Linux | 23      | 41      | 21     | 53          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.2    | 26      | 11.6   | 28          | 11.9              |

### RSA-4096 signature/verification ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 60      | 76      | 104    | 40          | 62                |
| i7-5775R                     | Linux | 61      | 111     | 114    | 40          | 61                |
| i7-8565U                     | Linux | 61      | 112     | 108    | 38          | 63                |
| Xeon-6254                    | Linux | 60      | 77      | 116    | 39          |                   |
| Cortex A53 (R.Pi3)           | Linux | 66      | 77      | 94     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 74      | 99      | 124    | 45          | 72                |
| Apple M1                     | macOS | 67      | 70      | 93     | 41          | 54                |
| Apple M1                     | Linux | 67      | 109     | 101    | 46          | 59                |
| Ampere Altra (Neoverse N1)   | Linux | 80      | 66      | 127    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 67      | 68      | 119    | 46          | 66                |

### RSA-4096 signature with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1075    | 4261    | 2570   | 3647        | 1731              |
| i7-5775R                     | Linux | 756     | 4160    | 2077   | 2700        | 1208              |
| i7-8565U                     | Linux | 1017    | 3677    | 1756   | 1851        | 1007              |
| Xeon-6254                    | Linux | 515     | 1991    | 1429   | 1538        |                   |
| Cortex A53 (R.Pi3)           | Linux | 2491    | 5050    | 2854   | 4177        | 2112              |
| Cortex A72 (R.Pi4)           | Linux | 1937    | 4075    | 2762   | 2531        | 1647              |
| Apple M1                     | macOS | 1222    | 3005    | 2120   | 2481        | 1274              |
| Apple M1                     | Linux | 748     | 2927    | 1388   | 1433        | 858               |
| Ampere Altra (Neoverse N1)   | Linux | 1939    | 2724    | 2725   | 2468        | 1676              |
| AWS Graviton 3 (Neoverse V1) | Linux | 757     | 1820    | 1394   | 1305        | 791               |

### RSA-4096 verification with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 19.1    | 56      | 24     | 90          | 29                |
| i7-5775R                     | Linux | 14.0    | 37      | 17.9   | 53          | 18.6              |
| i7-8565U                     | Linux | 17.8    | 34      | 16.1   | 45          | 16.1              |
| Xeon-6254                    | Linux | 8.6     | 25      | 12.2   | 39          |                   |
| Cortex A53 (R.Pi3)           | Linux | 38      | 65      | 30     | 91          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 27      | 41      | 22     | 56          | 22                |
| Apple M1                     | macOS | 18.8    | 42      | 22     | 60          | 23                |
| Apple M1                     | Linux | 11.5    | 27      | 13.7   | 32          | 15.1              |
| Ampere Altra (Neoverse N1)   | Linux | 24      | 41      | 21     | 53          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.7    | 26      | 11.6   | 28          | 11.9              |

### RSA-4096 signature/verification ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 56      | 75      | 105    | 40          | 59                |
| i7-5775R                     | Linux | 54      | 109     | 116    | 50          | 64                |
| i7-8565U                     | Linux | 57      | 108     | 109    | 41          | 62                |
| Xeon-6254                    | Linux | 59      | 77      | 117    | 39          |                   |
| Cortex A53 (R.Pi3)           | Linux | 64      | 77      | 94     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 71      | 99      | 124    | 45          | 72                |
| Apple M1                     | macOS | 65      | 70      | 93     | 41          | 54                |
| Apple M1                     | Linux | 65      | 108     | 101    | 44          | 56                |
| Ampere Altra (Neoverse N1)   | Linux | 79      | 66      | 128    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 64      | 68      | 119    | 46          | 66                |

### RSA-4096 parse public key

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 18.0    | 0.31    | 1.3    | 13.2        | 3.7               |
| i7-5775R                     | Linux | 29      | 0.15    | 1.1    | 8.6         | 2.3               |
| i7-8565U                     | Linux | 24      | 0.11    | 0.80   | 6.1         | 1.9               |
| Xeon-6254                    | Linux | 0.20    | 0.091   | 0.49   | 5.7         |                   |
| Cortex A53 (R.Pi3)           | Linux | 20      | 0.11    | 0.54   | 5.4         | 1.0               |
| Cortex A72 (R.Pi4)           | Linux | 24      | 0.12    | 0.66   | 4.1         | 1.3               |
| Apple M1                     | macOS | 10.2    | 0.17    | 0.77   | 8.6         | 3.9               |
| Apple M1                     | Linux | 16.1    | 0.12    | 0.47   | 4.6         | 2.5               |
| Ampere Altra (Neoverse N1)   | Linux | 36      | 0.073   | 0.36   | 2.9         | 1.0               |
| AWS Graviton 3 (Neoverse V1) | Linux | 45      | 0.073   | 0.37   | 2.9         | 1.0               |

### RSA-4096 parse private key

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 20      | 30      | 16.9   | 33          | 2.3               |
| i7-5775R                     | Linux | 29      | 17.0    | 10.4   | 24          | 1.5               |
| i7-8565U                     | Linux | 24      | 13.5    | 9.2    | 15.7        | 1.6               |
| Xeon-6254                    | Linux | 0.65    | 11.1    | 6.9    | 15.4        |                   |
| Cortex A53 (R.Pi3)           | Linux | 20      | 25      | 8.2    | 15.5        | 0.81              |
| Cortex A72 (R.Pi4)           | Linux | 25      | 13.4    | 9.1    | 10.7        | 1.00              |
| Apple M1                     | macOS | 11.6    | 23      | 11.7   | 16.8        | 1.5               |
| Apple M1                     | Linux | 16.8    | 12.3    | 6.4    | 8.4         | 0.93              |
| Ampere Altra (Neoverse N1)   | Linux | 36      | 10.6    | 6.4    | 7.3         | 0.69              |
| AWS Graviton 3 (Neoverse V1) | Linux | 46      | 9.3     | 4.9    | 7.1         | 0.65              |

## AES-128-CBC

### AES-128-CBC encryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 340     | 491     | 343    | 344    | 1299        |             |
| i7-5775R                     | Linux | 252     | 436     | 246    | 255    | 740         |             |
| i7-8565U                     | Linux | 126     | 261     | 125    | 175    | 660         |             |
| Xeon-6254                    | Linux | 98      | 216     | 98     | 137    | 503         |             |
| Cortex A53 (R.Pi3)           | Linux | 279     | 674     | 729    | 728    | 496         |             |
| Cortex A72 (R.Pi4)           | Linux | 1106    | 607     | 714    | 706    | 680         |             |
| Apple M1                     | macOS | 219     | 397     | 1127   | 1116   | 1176        | 340         |
| Apple M1                     | Linux | 140     | 879     | 136    | 256    | 794         | 212         |
| Ampere Altra (Neoverse N1)   | Linux | 40      | 442     | 40     | 475    | 457         | 70          |
| AWS Graviton 3 (Neoverse V1) | Linux | 58      | 513     | 58     | 553    | 514         | 103         |

### AES-128-CBC decryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 49      | 279     | 50     | 65     | 1190        |             |
| i7-5775R                     | Linux | 35      | 294     | 35     | 48     | 743         |             |
| i7-8565U                     | Linux | 29      | 218     | 29     | 37     | 661         |             |
| Xeon-6254                    | Linux | 23      | 148     | 23     | 40     | 485         |             |
| Cortex A53 (R.Pi3)           | Linux | 294     | 608     | 629    | 629    | 531         |             |
| Cortex A72 (R.Pi4)           | Linux | 791     | 648     | 655    | 656    | 719         |             |
| Apple M1                     | macOS | 26      | 164     | 898    | 899    | 1095        | 49          |
| Apple M1                     | Linux | 20      | 733     | 19.4   | 27     | 691         | 36          |
| Ampere Altra (Neoverse N1)   | Linux | 22      | 430     | 24     | 452    | 469         | 38          |
| AWS Graviton 3 (Neoverse V1) | Linux | 14.0    | 480     | 15.1   | 481    | 521         | 31          |

### AES-128-CBC encryption/decryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 6.9     | 1.8     | 6.8    | 5.2    | 1.1         |             |
| i7-5775R                     | Linux | 7.2     | 1.5     | 7.0    | 5.3    | 1.00        |             |
| i7-8565U                     | Linux | 4.2     | 1.2     | 4.2    | 4.7    | 1.00        |             |
| Xeon-6254                    | Linux | 4.2     | 1.5     | 4.2    | 3.4    | 1.0         |             |
| Cortex A53 (R.Pi3)           | Linux | 0.95    | 1.1     | 1.2    | 1.2    | 0.93        |             |
| Cortex A72 (R.Pi4)           | Linux | 1.4     | 0.94    | 1.1    | 1.1    | 0.95        |             |
| Apple M1                     | macOS | 8.2     | 2.4     | 1.3    | 1.2    | 1.1         | 6.9         |
| Apple M1                     | Linux | 6.7     | 1.2     | 7.0    | 9.2    | 1.1         | 5.9         |
| Ampere Altra (Neoverse N1)   | Linux | 1.8     | 1.0     | 1.6    | 1.1    | 0.98        | 1.8         |
| AWS Graviton 3 (Neoverse V1) | Linux | 4.2     | 1.1     | 3.9    | 1.1    | 0.99        | 3.3         |

## AES-256-CBC

### AES-256-CBC encryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 475     | 617     | 477    | 473    | 1736        |             |
| i7-5775R                     | Linux | 347     | 531     | 348    | 351    | 994         |             |
| i7-8565U                     | Linux | 171     | 305     | 170    | 219    | 882         |             |
| Xeon-6254                    | Linux | 136     | 253     | 136    | 175    | 667         |             |
| Cortex A53 (R.Pi3)           | Linux | 368     | 869     | 943    | 940    | 638         |             |
| Cortex A72 (R.Pi4)           | Linux | 1549    | 764     | 939    | 939    | 885         |             |
| Apple M1                     | macOS | 310     | 487     | 1483   | 1481   | 1543        | 462         |
| Apple M1                     | Linux | 198     | 1170    | 193    | 306    | 1029        | 278         |
| Ampere Altra (Neoverse N1)   | Linux | 56      | 569     | 56     | 632    | 590         | 90          |
| AWS Graviton 3 (Neoverse V1) | Linux | 82      | 669     | 82     | 739    | 671         | 130         |

### AES-256-CBC decryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 68      | 379     | 68     | 91     | 1639        |             |
| i7-5775R                     | Linux | 49      | 327     | 49     | 66     | 1037        |             |
| i7-8565U                     | Linux | 41      | 243     | 40     | 50     | 892         |             |
| Xeon-6254                    | Linux | 32      | 169     | 32     | 51     | 656         |             |
| Cortex A53 (R.Pi3)           | Linux | 378     | 783     | 841    | 841    | 682         |             |
| Cortex A72 (R.Pi4)           | Linux | 1113    | 818     | 890    | 892    | 929         |             |
| Apple M1                     | macOS | 49      | 191     | 1261   | 1354   | 1460        | 66          |
| Apple M1                     | Linux | 32      | 926     | 28     | 32     | 941         | 48          |
| Ampere Altra (Neoverse N1)   | Linux | 30      | 557     | 29     | 607    | 593         | 53          |
| AWS Graviton 3 (Neoverse V1) | Linux | 18.9    | 634     | 21     | 682    | 677         | 41          |

### AES-256-CBC encryption/decryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 6.9     | 1.6     | 7.0    | 5.1    | 1.1         |             |
| i7-5775R                     | Linux | 7.0     | 1.6     | 7.0    | 5.3    | 0.96        |             |
| i7-8565U                     | Linux | 4.1     | 1.3     | 4.2    | 4.4    | 0.99        |             |
| Xeon-6254                    | Linux | 4.2     | 1.5     | 4.2    | 3.4    | 1.0         |             |
| Cortex A53 (R.Pi3)           | Linux | 0.97    | 1.1     | 1.1    | 1.1    | 0.94        |             |
| Cortex A72 (R.Pi4)           | Linux | 1.4     | 0.93    | 1.1    | 1.1    | 0.95        |             |
| Apple M1                     | macOS | 6.3     | 2.5     | 1.2    | 1.1    | 1.1         | 7.0         |
| Apple M1                     | Linux | 6.0     | 1.3     | 6.9    | 9.4    | 1.1         | 5.7         |
| Ampere Altra (Neoverse N1)   | Linux | 1.9     | 1.0     | 1.9    | 1.0    | 1.00        | 1.7         |
| AWS Graviton 3 (Neoverse V1) | Linux | 4.3     | 1.1     | 3.9    | 1.1    | 0.99        | 3.2         |

## 2048-bit modular arithmetic

| CPU                          | OS    | Add   | Mult | Mult<br/>(Montgomery) | Mult<br/>(reciprocal) | Square | Div<br/>(reciprocal) | Inverse | Square<br/>Root | Exponent<br/>(public) | Exponent<br/>(public)<br/>(Montgomery) | Exponent<br/>(public)<br/>(Montgomery)<br/>(word) | Exponent<br/>(public)<br/>(reciprocal) | Exponent<br/>(public)<br/>(simple) | Exponent<br/>(private) | Exponent<br/>(private)<br/>(Montgomery) | Exponent<br/>(private)<br/>(Montgomery)<br/>(word) | Exponent<br/>(private)<br/>(reciprocal) | Exponent<br/>(private)<br/>(simple) |
| ---------------------------- | ----- | :---: | :--: | :--: | :--: | :----: | :---: | :-----: | :----: | :------: | :------: | :------: | :------: | :------: | :------: | :------: | :------: | :------: | :------: |
| i7-8565U                     | Linux | 0.048 | 0.70 | 0.27 | 0.49 | 0.66   | 0.006 | 47      | 933    | 5.3      | 4.1      | 3.5      | 9.1      | 12.2     | 478      | 481      | 456      | 1166     | 1612     |
| Xeon-6254                    | Linux | 0.038 | 0.53 | 0.13 | 0.37 | 0.52   | 0.005 | 35      | 233    | 3.0      | 2.0      | 1.8      | 6.7      | 8.9      | 231      | 231      | 232      | 866      | 1226     |
| Cortex A72 (R.Pi4)           | Linux | 0.047 | 0.88 | 0.45 | 0.82 | 0.83   | 0.006 | 38      | 1603   | 8.5      | 7.2      | 5.8      | 13.7     | 14.2     | 857      | 856      | 759      | 1804     | 1969     |
| Apple M1                     | macOS | 0.077 | 1.7  | 0.30 | 1.1  | 1.7    | 0.007 | 78      | 505    | 6.6      | 4.4      | 3.8      | 21       | 29       | 505      | 502      | 492      | 2705     | 4099     |
| Apple M1                     | Linux | 0.045 | 0.97 | 0.20 | 0.67 | 0.96   | 0.004 | 47      | 335    | 4.2      | 2.9      | 2.4      | 12.5     | 16.7     | 327      | 326      | 320      | 1623     | 2312     |
| Ampere Altra (Neoverse N1)   | Linux | 0.037 | 0.79 | 0.47 | 0.72 | 0.72   | 0.003 | 28      | 1642   | 8.3      | 7.4      | 5.9      | 12.1     | 12.6     | 881      | 876      | 772      | 1594     | 1753     |
| AWS Graviton 3 (Neoverse V1) | Linux | 0.036 | 0.71 | 0.18 | 0.51 | 0.70   | 0.003 | 31      | 314    | 3.6      | 2.7      | 2.3      | 9.5      | 12.3     | 314      | 313      | 293      | 1238     | 1714     |

