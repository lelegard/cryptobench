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
| i7-8565U                     | Linux | 5.1     | 10.1    | 5.0    | 14.5        | 5.7               |
| Cortex A53 (R.Pi3)           | Linux | 10.3    | 17.7    | 8.9    | 25          | 9.3               |
| Cortex A72 (R.Pi4)           | Linux | 8.6     | 11.5    | 7.1    | 16.5        | 7.7               |
| Apple M1                     | macOS | 5.2     | 11.9    | 5.6    | 17.8        | 7.2               |
| Apple M1                     | Linux | 3.2     | 7.3     | 3.9    | 10.2        | 4.6               |
| Ampere Altra (Neoverse N1)   | Linux | 7.8     | 11.2    | 7.1    | 14.9        | 7.5               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.3     | 7.4     | 3.5    | 9.1         | 3.9               |

### RSA-2048 decryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 160     | 724     | 480    | 683         | 228               |
| i7-5775R                     | Linux | 117     | 1087    | 388    | 421         | 182               |
| i7-8565U                     | Linux | 102     | 925     | 350    | 377         | 158               |
| Cortex A53 (R.Pi3)           | Linux | 355     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 278     | 918     | 370    | 427         | 242               |
| Apple M1                     | macOS | 189     | 488     | 355    | 480         | 205               |
| Apple M1                     | Linux | 117     | 723     | 238    | 278         | 135               |
| Ampere Altra (Neoverse N1)   | Linux | 273     | 434     | 345    | 393         | 243               |
| AWS Graviton 3 (Neoverse V1) | Linux | 116     | 301     | 201    | 246         | 114               |

### RSA-2048 decryption/encryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 27      | 43      | 66     | 25          | 26                |
| i7-5775R                     | Linux | 27      | 95      | 74     | 25          | 25                |
| i7-8565U                     | Linux | 20      | 91      | 70     | 25          | 28                |
| Cortex A53 (R.Pi3)           | Linux | 34      | 44      | 53     | 26          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 32      | 79      | 52     | 25          | 31                |
| Apple M1                     | macOS | 36      | 40      | 63     | 27          | 28                |
| Apple M1                     | Linux | 36      | 99      | 60     | 27          | 29                |
| Ampere Altra (Neoverse N1)   | Linux | 35      | 38      | 48     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 35      | 40      | 57     | 27          | 29                |

### RSA-2048 encryption with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 6.7     | 16.8    | 7.1    | 26          | 8.6               |
| i7-5775R                     | Linux | 5.2     | 11.3    | 5.2    | 16.5        | 6.0               |
| i7-8565U                     | Linux | 6.0     | 10.2    | 4.8    | 14.3        | 5.8               |
| Cortex A53 (R.Pi3)           | Linux | 11.0    | 17.7    | 8.9    | 25          | 9.3               |
| Cortex A72 (R.Pi4)           | Linux | 9.5     | 11.5    | 7.1    | 16.4        | 7.7               |
| Apple M1                     | macOS | 5.7     | 12.0    | 5.6    | 17.8        | 7.2               |
| Apple M1                     | Linux | 3.6     | 7.2     | 3.9    | 10.1        | 4.4               |
| Ampere Altra (Neoverse N1)   | Linux | 8.2     | 11.2    | 7.1    | 14.9        | 7.5               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.6     | 7.4     | 3.5    | 9.1         | 3.9               |

### RSA-2048 decryption with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 161     | 728     | 475    | 686         | 233               |
| i7-5775R                     | Linux | 122     | 1055    | 390    | 415         | 166               |
| i7-8565U                     | Linux | 105     | 935     | 348    | 384         | 162               |
| Cortex A53 (R.Pi3)           | Linux | 357     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 280     | 919     | 370    | 427         | 242               |
| Apple M1                     | macOS | 189     | 488     | 355    | 480         | 205               |
| Apple M1                     | Linux | 120     | 721     | 234    | 275         | 135               |
| Ampere Altra (Neoverse N1)   | Linux | 273     | 434     | 345    | 394         | 243               |
| AWS Graviton 3 (Neoverse V1) | Linux | 116     | 301     | 201    | 246         | 114               |

### RSA-2048 decryption/encryption ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 24      | 43      | 66     | 26          | 27                |
| i7-5775R                     | Linux | 23      | 93      | 75     | 25          | 27                |
| i7-8565U                     | Linux | 17.6    | 91      | 72     | 26          | 27                |
| Cortex A53 (R.Pi3)           | Linux | 32      | 44      | 53     | 26          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 29      | 79      | 52     | 26          | 31                |
| Apple M1                     | macOS | 33      | 40      | 63     | 27          | 28                |
| Apple M1                     | Linux | 33      | 100     | 59     | 27          | 30                |
| Ampere Altra (Neoverse N1)   | Linux | 33      | 38      | 48     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 32      | 40      | 57     | 27          | 29                |

### RSA-2048 signature

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 159     | 719     | 472    | 683         | 232               |
| i7-5775R                     | Linux | 114     | 1067    | 388    | 423         | 165               |
| i7-8565U                     | Linux | 103     | 949     | 357    | 377         | 157               |
| Cortex A53 (R.Pi3)           | Linux | 356     | 790     | 476    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 279     | 919     | 370    | 427         | 242               |
| Apple M1                     | macOS | 188     | 487     | 355    | 480         | 204               |
| Apple M1                     | Linux | 117     | 711     | 236    | 277         | 132               |
| Ampere Altra (Neoverse N1)   | Linux | 273     | 434     | 345    | 392         | 243               |
| AWS Graviton 3 (Neoverse V1) | Linux | 116     | 303     | 201    | 246         | 114               |

### RSA-2048 verification

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 5.7     | 16.1    | 7.9    | 27          | 8.4               |
| i7-5775R                     | Linux | 3.9     | 10.9    | 5.7    | 17.6        | 5.9               |
| i7-8565U                     | Linux | 5.0     | 10.0    | 5.2    | 13.7        | 5.3               |
| Cortex A53 (R.Pi3)           | Linux | 10.0    | 17.5    | 9.1    | 25          | 9.2               |
| Cortex A72 (R.Pi4)           | Linux | 7.9     | 11.4    | 7.3    | 16.3        | 7.6               |
| Apple M1                     | macOS | 4.9     | 11.7    | 6.1    | 17.4        | 6.8               |
| Apple M1                     | Linux | 3.0     | 6.9     | 4.1    | 9.8         | 4.3               |
| Ampere Altra (Neoverse N1)   | Linux | 7.6     | 11.0    | 7.3    | 14.8        | 7.4               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.0     | 7.2     | 3.6    | 8.9         | 3.8               |

### RSA-2048 signature/verification ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 28      | 44      | 60     | 25          | 27                |
| i7-5775R                     | Linux | 29      | 97      | 68     | 24          | 27                |
| i7-8565U                     | Linux | 20      | 94      | 68     | 27          | 29                |
| Cortex A53 (R.Pi3)           | Linux | 35      | 45      | 52     | 25          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 35      | 80      | 50     | 26          | 32                |
| Apple M1                     | macOS | 38      | 41      | 57     | 27          | 30                |
| Apple M1                     | Linux | 39      | 102     | 58     | 28          | 30                |
| Ampere Altra (Neoverse N1)   | Linux | 36      | 39      | 47     | 26          | 33                |
| AWS Graviton 3 (Neoverse V1) | Linux | 38      | 41      | 55     | 27          | 30                |

### RSA-2048 signature with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 207     | 717     | 473    | 716         | 229               |
| i7-5775R                     | Linux | 114     | 1052    | 388    | 425         | 167               |
| i7-8565U                     | Linux | 107     | 925     | 355    | 371         | 159               |
| Cortex A53 (R.Pi3)           | Linux | 357     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 281     | 920     | 370    | 427         | 242               |
| Apple M1                     | macOS | 189     | 487     | 355    | 480         | 204               |
| Apple M1                     | Linux | 120     | 731     | 329    | 277         | 136               |
| Ampere Altra (Neoverse N1)   | Linux | 273     | 434     | 345    | 392         | 243               |
| AWS Graviton 3 (Neoverse V1) | Linux | 116     | 303     | 201    | 246         | 114               |

### RSA-2048 verification with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 6.7     | 16.3    | 7.8    | 28          | 8.3               |
| i7-5775R                     | Linux | 4.9     | 10.7    | 5.6    | 15.9        | 5.9               |
| i7-8565U                     | Linux | 5.9     | 9.8     | 5.3    | 14.0        | 5.4               |
| Cortex A53 (R.Pi3)           | Linux | 10.9    | 17.5    | 9.1    | 25          | 9.2               |
| Cortex A72 (R.Pi4)           | Linux | 9.0     | 11.3    | 7.3    | 16.3        | 7.6               |
| Apple M1                     | macOS | 5.5     | 11.7    | 6.1    | 17.4        | 6.8               |
| Apple M1                     | Linux | 3.5     | 6.9     | 4.2    | 9.8         | 4.3               |
| Ampere Altra (Neoverse N1)   | Linux | 8.0     | 11.0    | 7.3    | 14.8        | 7.4               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.5     | 7.2     | 3.6    | 8.9         | 3.8               |

### RSA-2048 signature/verification ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 30      | 43      | 60     | 24          | 27                |
| i7-5775R                     | Linux | 23      | 97      | 69     | 26          | 28                |
| i7-8565U                     | Linux | 18.4    | 94      | 66     | 26          | 29                |
| Cortex A53 (R.Pi3)           | Linux | 32      | 45      | 52     | 26          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 31      | 81      | 50     | 26          | 32                |
| Apple M1                     | macOS | 34      | 41      | 57     | 27          | 30                |
| Apple M1                     | Linux | 34      | 105     | 78     | 28          | 31                |
| Ampere Altra (Neoverse N1)   | Linux | 34      | 39      | 47     | 26          | 33                |
| AWS Graviton 3 (Neoverse V1) | Linux | 33      | 41      | 55     | 27          | 30                |

### RSA-2048 parse public key

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 18.0    | 0.30    | 1.2    | 5.4         | 2.1               |
| i7-5775R                     | Linux | 29      | 0.15    | 0.60   | 3.1         | 1.3               |
| i7-8565U                     | Linux | 24      | 0.11    | 0.55   | 2.4         | 1.1               |
| Cortex A53 (R.Pi3)           | Linux | 20      | 0.11    | 0.47   | 1.9         | 0.58              |
| Cortex A72 (R.Pi4)           | Linux | 24      | 0.12    | 0.54   | 1.6         | 0.74              |
| Apple M1                     | macOS | 9.7     | 0.17    | 0.67   | 3.3         | 2.1               |
| Apple M1                     | Linux | 16.0    | 0.11    | 0.37   | 2.0         | 1.3               |
| Ampere Altra (Neoverse N1)   | Linux | 37      | 0.07    | 0.30   | 1.1         | 0.56              |
| AWS Graviton 3 (Neoverse V1) | Linux | 45      | 0.09    | 0.31   | 1.2         | 0.58              |

### RSA-2048 parse private key

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 20      | 11.5    | 10.1   | 9.9         | 1.5               |
| i7-5775R                     | Linux | 30      | 6.1     | 9.0    | 7.0         | 0.92              |
| i7-8565U                     | Linux | 25      | 5.3     | 4.7    | 5.6         | 1.0               |
| Cortex A53 (R.Pi3)           | Linux | 20      | 8.4     | 4.2    | 5.0         | 0.53              |
| Cortex A72 (R.Pi4)           | Linux | 26      | 5.0     | 4.8    | 3.7         | 0.63              |
| Apple M1                     | macOS | 10.9    | 7.8     | 6.2    | 5.1         | 0.88              |
| Apple M1                     | Linux | 16.8    | 4.0     | 3.6    | 3.1         | 0.55              |
| Ampere Altra (Neoverse N1)   | Linux | 38      | 3.6     | 3.0    | 2.5         | 0.43              |
| AWS Graviton 3 (Neoverse V1) | Linux | 46      | 3.0     | 2.8    | 2.5         | 0.41              |

## RSA-4096

### RSA-4096 encryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 17.9    | 57      | 23     | 89          | 26                |
| i7-5775R                     | Linux | 13.1    | 39      | 17.3   | 53          | 19.3              |
| i7-8565U                     | Linux | 17.8    | 38      | 16.0   | 45          | 17.8              |
| Cortex A53 (R.Pi3)           | Linux | 38      | 65      | 29     | 92          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 26      | 41      | 21     | 56          | 22                |
| Apple M1                     | macOS | 18.3    | 43      | 20     | 61          | 23                |
| Apple M1                     | Linux | 11.5    | 26      | 14.4   | 31          | 17.3              |
| Ampere Altra (Neoverse N1)   | Linux | 24      | 41      | 20     | 53          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.4    | 26      | 11.4   | 28          | 12.2              |

### RSA-4096 decryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1058    | 4279    | 2555   | 3591        | 1614              |
| i7-5775R                     | Linux | 770     | 4219    | 2496   | 2175        | 1160              |
| i7-8565U                     | Linux | 1098    | 3944    | 1864   | 1966        | 1093              |
| Cortex A53 (R.Pi3)           | Linux | 2481    | 5044    | 2853   | 4140        | 2113              |
| Cortex A72 (R.Pi4)           | Linux | 1937    | 4073    | 2765   | 2517        | 1641              |
| Apple M1                     | macOS | 1200    | 3028    | 2119   | 2483        | 1281              |
| Apple M1                     | Linux | 754     | 2892    | 1418   | 1475        | 864               |
| Ampere Altra (Neoverse N1)   | Linux | 1940    | 2743    | 2705   | 2439        | 1659              |
| AWS Graviton 3 (Neoverse V1) | Linux | 757     | 1820    | 1394   | 1304        | 791               |

### RSA-4096 decryption/encryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 59      | 75      | 109    | 40          | 61                |
| i7-5775R                     | Linux | 58      | 107     | 144    | 40          | 60                |
| i7-8565U                     | Linux | 61      | 103     | 116    | 43          | 61                |
| Cortex A53 (R.Pi3)           | Linux | 65      | 76      | 95     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 72      | 98      | 127    | 44          | 71                |
| Apple M1                     | macOS | 65      | 70      | 101    | 40          | 53                |
| Apple M1                     | Linux | 65      | 108     | 98     | 47          | 49                |
| Ampere Altra (Neoverse N1)   | Linux | 80      | 65      | 129    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 66      | 67      | 122    | 45          | 64                |

### RSA-4096 encryption with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 19.2    | 56      | 23     | 88          | 26                |
| i7-5775R                     | Linux | 14.0    | 38      | 20     | 52          | 19.6              |
| i7-8565U                     | Linux | 19.2    | 37      | 16.1   | 47          | 17.5              |
| Cortex A53 (R.Pi3)           | Linux | 38      | 65      | 29     | 91          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 27      | 41      | 21     | 56          | 22                |
| Apple M1                     | macOS | 18.8    | 43      | 20     | 61          | 23                |
| Apple M1                     | Linux | 12.0    | 27      | 13.8   | 31          | 17.7              |
| Ampere Altra (Neoverse N1)   | Linux | 24      | 41      | 20     | 53          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.8    | 26      | 11.4   | 28          | 12.2              |

### RSA-4096 decryption with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1064    | 4192    | 2555   | 3553        | 1604              |
| i7-5775R                     | Linux | 761     | 4223    | 2207   | 2180        | 1167              |
| i7-8565U                     | Linux | 1097    | 3951    | 1851   | 1930        | 1057              |
| Cortex A53 (R.Pi3)           | Linux | 2491    | 5056    | 2853   | 4139        | 2112              |
| Cortex A72 (R.Pi4)           | Linux | 1944    | 4073    | 2766   | 2517        | 1641              |
| Apple M1                     | macOS | 1200    | 3029    | 2119   | 2482        | 1281              |
| Apple M1                     | Linux | 763     | 2847    | 1431   | 1412        | 833               |
| Ampere Altra (Neoverse N1)   | Linux | 1938    | 2743    | 2702   | 2438        | 1659              |
| AWS Graviton 3 (Neoverse V1) | Linux | 758     | 1820    | 1394   | 1304        | 792               |

### RSA-4096 decryption/encryption ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 55      | 74      | 109    | 40          | 61                |
| i7-5775R                     | Linux | 54      | 108     | 108    | 41          | 59                |
| i7-8565U                     | Linux | 57      | 106     | 114    | 40          | 60                |
| Cortex A53 (R.Pi3)           | Linux | 64      | 76      | 95     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 69      | 98      | 127    | 44          | 71                |
| Apple M1                     | macOS | 63      | 70      | 101    | 40          | 53                |
| Apple M1                     | Linux | 63      | 104     | 103    | 44          | 47                |
| Ampere Altra (Neoverse N1)   | Linux | 78      | 65      | 129    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 64      | 67      | 122    | 45          | 64                |

### RSA-4096 signature

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1044    | 4295    | 2579   | 3620        | 1604              |
| i7-5775R                     | Linux | 798     | 4213    | 2068   | 2185        | 1225              |
| i7-8565U                     | Linux | 1093    | 3820    | 1882   | 1975        | 1041              |
| Cortex A53 (R.Pi3)           | Linux | 2489    | 5049    | 2853   | 4141        | 2113              |
| Cortex A72 (R.Pi4)           | Linux | 1938    | 4106    | 2764   | 2516        | 1641              |
| Apple M1                     | macOS | 1199    | 3030    | 2119   | 2482        | 1275              |
| Apple M1                     | Linux | 759     | 2899    | 1415   | 1520        | 870               |
| Ampere Altra (Neoverse N1)   | Linux | 1937    | 2743    | 2704   | 2439        | 1659              |
| AWS Graviton 3 (Neoverse V1) | Linux | 757     | 1820    | 1394   | 1304        | 791               |

### RSA-4096 verification

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 17.4    | 56      | 24     | 89          | 25                |
| i7-5775R                     | Linux | 13.0    | 37      | 18.0   | 53          | 20                |
| i7-8565U                     | Linux | 17.7    | 36      | 17.0   | 48          | 17.9              |
| Cortex A53 (R.Pi3)           | Linux | 37      | 65      | 30     | 91          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 25      | 41      | 22     | 55          | 22                |
| Apple M1                     | macOS | 17.8    | 42      | 23     | 60          | 24                |
| Apple M1                     | Linux | 11.3    | 26      | 13.8   | 30          | 15.1              |
| Ampere Altra (Neoverse N1)   | Linux | 23      | 41      | 21     | 53          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.2    | 26      | 11.7   | 28          | 12.0              |

### RSA-4096 signature/verification ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 60      | 76      | 104    | 40          | 62                |
| i7-5775R                     | Linux | 61      | 111     | 114    | 40          | 61                |
| i7-8565U                     | Linux | 61      | 104     | 110    | 41          | 58                |
| Cortex A53 (R.Pi3)           | Linux | 66      | 77      | 94     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 74      | 100     | 124    | 45          | 72                |
| Apple M1                     | macOS | 67      | 70      | 89     | 40          | 51                |
| Apple M1                     | Linux | 67      | 109     | 102    | 49          | 57                |
| Ampere Altra (Neoverse N1)   | Linux | 80      | 66      | 127    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 67      | 68      | 119    | 46          | 66                |

### RSA-4096 signature with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1075    | 4261    | 2570   | 3647        | 1731              |
| i7-5775R                     | Linux | 756     | 4160    | 2077   | 2700        | 1208              |
| i7-8565U                     | Linux | 1108    | 3864    | 1878   | 1997        | 1067              |
| Cortex A53 (R.Pi3)           | Linux | 2491    | 5050    | 2854   | 4177        | 2112              |
| Cortex A72 (R.Pi4)           | Linux | 1941    | 4107    | 2763   | 2517        | 1641              |
| Apple M1                     | macOS | 1201    | 3002    | 2120   | 2484        | 1275              |
| Apple M1                     | Linux | 766     | 2893    | 1432   | 1406        | 833               |
| Ampere Altra (Neoverse N1)   | Linux | 1938    | 2743    | 2706   | 2440        | 1659              |
| AWS Graviton 3 (Neoverse V1) | Linux | 757     | 1819    | 1394   | 1304        | 792               |

### RSA-4096 verification with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 19.1    | 56      | 24     | 90          | 29                |
| i7-5775R                     | Linux | 14.0    | 37      | 17.9   | 53          | 18.6              |
| i7-8565U                     | Linux | 18.4    | 35      | 16.8   | 46          | 17.6              |
| Cortex A53 (R.Pi3)           | Linux | 38      | 65      | 30     | 91          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 27      | 41      | 22     | 55          | 22                |
| Apple M1                     | macOS | 18.4    | 42      | 23     | 60          | 24                |
| Apple M1                     | Linux | 11.7    | 26      | 14.0   | 32          | 15.5              |
| Ampere Altra (Neoverse N1)   | Linux | 24      | 41      | 21     | 53          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.7    | 26      | 11.7   | 28          | 12.0              |

### RSA-4096 signature/verification ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 56      | 75      | 105    | 40          | 59                |
| i7-5775R                     | Linux | 54      | 109     | 116    | 50          | 64                |
| i7-8565U                     | Linux | 60      | 107     | 111    | 43          | 60                |
| Cortex A53 (R.Pi3)           | Linux | 64      | 77      | 94     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 71      | 100     | 124    | 45          | 72                |
| Apple M1                     | macOS | 65      | 70      | 89     | 40          | 51                |
| Apple M1                     | Linux | 65      | 108     | 102    | 43          | 53                |
| Ampere Altra (Neoverse N1)   | Linux | 79      | 66      | 128    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 64      | 68      | 119    | 46          | 66                |

### RSA-4096 parse public key

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 18.0    | 0.31    | 1.3    | 13.2        | 3.7               |
| i7-5775R                     | Linux | 29      | 0.15    | 1.1    | 8.6         | 2.3               |
| i7-8565U                     | Linux | 25      | 0.11    | 0.75   | 6.3         | 2.2               |
| Cortex A53 (R.Pi3)           | Linux | 20      | 0.11    | 0.54   | 5.4         | 1.0               |
| Cortex A72 (R.Pi4)           | Linux | 24      | 0.12    | 0.66   | 4.1         | 1.3               |
| Apple M1                     | macOS | 10.0    | 0.17    | 0.77   | 8.7         | 3.9               |
| Apple M1                     | Linux | 16.4    | 0.11    | 0.44   | 4.6         | 2.4               |
| Ampere Altra (Neoverse N1)   | Linux | 37      | 0.08    | 0.36   | 2.9         | 1.0               |
| AWS Graviton 3 (Neoverse V1) | Linux | 45      | 0.08    | 0.37   | 2.9         | 1.0               |

### RSA-4096 parse private key

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 20      | 30      | 16.9   | 33          | 2.3               |
| i7-5775R                     | Linux | 29      | 17.0    | 10.4   | 24          | 1.5               |
| i7-8565U                     | Linux | 25      | 14.6    | 10.2   | 17.4        | 1.7               |
| Cortex A53 (R.Pi3)           | Linux | 20      | 25      | 8.2    | 15.5        | 0.81              |
| Cortex A72 (R.Pi4)           | Linux | 26      | 13.8    | 9.0    | 10.7        | 1.0               |
| Apple M1                     | macOS | 11.5    | 21      | 11.6   | 16.9        | 1.5               |
| Apple M1                     | Linux | 17.1    | 13.1    | 6.4    | 8.2         | 0.86              |
| Ampere Altra (Neoverse N1)   | Linux | 38      | 9.2     | 6.4    | 7.1         | 0.69              |
| AWS Graviton 3 (Neoverse V1) | Linux | 46      | 9.0     | 4.9    | 7.2         | 0.64              |

## AES-128-CBC

### AES-128-CBC encryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 340     | 491     | 343    | 344    | 1299        |             |
| i7-5775R                     | Linux | 252     | 436     | 246    | 255    | 740         |             |
| i7-8565U                     | Linux | 130     | 287     | 135    | 127    | 689         |             |
| Cortex A53 (R.Pi3)           | Linux | 279     | 674     | 729    | 728    | 496         |             |
| Cortex A72 (R.Pi4)           | Linux | 1101    | 608     | 704    | 703    | 748         |             |
| Apple M1                     | macOS | 218     | 395     | 1120   | 1116   | 1176        | 334         |
| Apple M1                     | Linux | 134     | 863     | 133    | 248    | 792         | 213         |
| Ampere Altra (Neoverse N1)   | Linux | 40      | 439     | 40     | 470    | 444         | 69          |
| AWS Graviton 3 (Neoverse V1) | Linux | 59      | 513     | 58     | 553    | 514         | 103         |

### AES-128-CBC decryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 49      | 279     | 50     | 65     | 1190        |             |
| i7-5775R                     | Linux | 35      | 294     | 35     | 48     | 743         |             |
| i7-8565U                     | Linux | 30      | 228     | 31     | 39     | 667         |             |
| Cortex A53 (R.Pi3)           | Linux | 294     | 608     | 629    | 629    | 531         |             |
| Cortex A72 (R.Pi4)           | Linux | 787     | 624     | 652    | 650    | 723         |             |
| Apple M1                     | macOS | 25      | 164     | 898    | 899    | 1093        | 49          |
| Apple M1                     | Linux | 18.6    | 738     | 19.2   | 28     | 727         | 36          |
| Ampere Altra (Neoverse N1)   | Linux | 22      | 425     | 24     | 445    | 470         | 38          |
| AWS Graviton 3 (Neoverse V1) | Linux | 14.0    | 482     | 15.1   | 482    | 522         | 31          |

### AES-128-CBC encryption/decryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: |
| i7-5775R                     | macOS | 6.9     | 1.8     | 6.8    | 5.2    | 1.1         |
| i7-5775R                     | Linux | 7.2     | 1.5     | 7.0    | 5.3    | 1.00        |
| i7-8565U                     | Linux | 4.2     | 1.3     | 4.4    | 3.3    | 1.0         |
| Cortex A53 (R.Pi3)           | Linux | 0.95    | 1.1     | 1.2    | 1.2    | 0.93        |
| Cortex A72 (R.Pi4)           | Linux | 1.4     | 0.97    | 1.1    | 1.1    | 1.0         |
| Apple M1                     | macOS | 8.4     | 2.4     | 1.2    | 1.2    | 1.1         |
| Apple M1                     | Linux | 7.2     | 1.2     | 6.9    | 8.9    | 1.1         |
| Ampere Altra (Neoverse N1)   | Linux | 1.8     | 1.0     | 1.6    | 1.1    | 0.94        |
| AWS Graviton 3 (Neoverse V1) | Linux | 4.2     | 1.1     | 3.9    | 1.1    | 0.98        |

## AES-256-CBC

### AES-256-CBC encryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 475     | 617     | 477    | 473    | 1736        |             |
| i7-5775R                     | Linux | 347     | 531     | 348    | 351    | 994         |             |
| i7-8565U                     | Linux | 175     | 323     | 177    | 176    | 913         |             |
| Cortex A53 (R.Pi3)           | Linux | 368     | 869     | 943    | 940    | 638         |             |
| Cortex A72 (R.Pi4)           | Linux | 1542    | 765     | 942    | 936    | 961         |             |
| Apple M1                     | macOS | 309     | 485     | 1481   | 1478   | 1542        | 454         |
| Apple M1                     | Linux | 192     | 1083    | 189    | 297    | 1000        | 271         |
| Ampere Altra (Neoverse N1)   | Linux | 56      | 563     | 56     | 628    | 569         | 90          |
| AWS Graviton 3 (Neoverse V1) | Linux | 82      | 670     | 82     | 739    | 671         | 130         |

### AES-256-CBC decryption

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 68      | 379     | 68     | 91     | 1639        |             |
| i7-5775R                     | Linux | 49      | 327     | 49     | 66     | 1037        |             |
| i7-8565U                     | Linux | 42      | 253     | 43     | 51     | 899         |             |
| Cortex A53 (R.Pi3)           | Linux | 378     | 783     | 841    | 841    | 682         |             |
| Cortex A72 (R.Pi4)           | Linux | 1111    | 786     | 887    | 885    | 934         |             |
| Apple M1                     | macOS | 49      | 191     | 1259   | 1354   | 1457        | 66          |
| Apple M1                     | Linux | 32      | 919     | 27     | 31     | 915         | 47          |
| Ampere Altra (Neoverse N1)   | Linux | 29      | 549     | 29     | 600    | 598         | 53          |
| AWS Graviton 3 (Neoverse V1) | Linux | 18.7    | 635     | 21     | 683    | 677         | 42          |

### AES-256-CBC encryption/decryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: |
| i7-5775R                     | macOS | 6.9     | 1.6     | 7.0    | 5.1    | 1.1         |
| i7-5775R                     | Linux | 7.0     | 1.6     | 7.0    | 5.3    | 0.96        |
| i7-8565U                     | Linux | 4.1     | 1.3     | 4.1    | 3.5    | 1.0         |
| Cortex A53 (R.Pi3)           | Linux | 0.97    | 1.1     | 1.1    | 1.1    | 0.94        |
| Cortex A72 (R.Pi4)           | Linux | 1.4     | 0.97    | 1.1    | 1.1    | 1.0         |
| Apple M1                     | macOS | 6.3     | 2.5     | 1.2    | 1.1    | 1.1         |
| Apple M1                     | Linux | 6.0     | 1.2     | 6.8    | 9.4    | 1.1         |
| Ampere Altra (Neoverse N1)   | Linux | 1.9     | 1.0     | 1.9    | 1.0    | 0.95        |
| AWS Graviton 3 (Neoverse V1) | Linux | 4.4     | 1.1     | 3.9    | 1.1    | 0.99        |

