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

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 5.9     | 16.6    | 7.2    | 26          | 8.7               |
| i7-5775R           | 4.3     | 11.4    | 5.2    | 16.5        | 7.2               |
| i7-8565U           | 5.1     | 10.1    | 5.0    | 14.5        | 5.7               |
| Cortex A53 (R.Pi3) | 10.3    | 17.7    | 8.9    | 25          | 9.3               |
| Cortex A72 (R.Pi4) | 8.6     | 11.5    | 7.1    | 16.5        | 7.7               |
| Apple M1           | 5.2     | 11.9    | 5.6    | 17.8        | 7.2               |
| Apple M1           | 3.2     | 7.3     | 3.9    | 10.2        | 4.6               |

### RSA-2048 decryption

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 160     | 724     | 480    | 683         | 228               |
| i7-5775R           | 117     | 1087    | 388    | 421         | 182               |
| i7-8565U           | 102     | 925     | 350    | 377         | 158               |
| Cortex A53 (R.Pi3) | 355     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4) | 278     | 918     | 370    | 427         | 242               |
| Apple M1           | 189     | 488     | 355    | 480         | 205               |
| Apple M1           | 117     | 723     | 238    | 278         | 135               |

### RSA-2048 decryption/encryption ratio

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 27      | 43      | 66     | 25          | 26                |
| i7-5775R           | 27      | 95      | 74     | 25          | 25                |
| i7-8565U           | 20      | 91      | 70     | 25          | 28                |
| Cortex A53 (R.Pi3) | 34      | 44      | 53     | 26          | 33                |
| Cortex A72 (R.Pi4) | 32      | 79      | 52     | 25          | 31                |
| Apple M1           | 36      | 40      | 63     | 27          | 28                |
| Apple M1           | 36      | 99      | 60     | 27          | 29                |

### RSA-2048 encryption with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 6.7     | 16.8    | 7.1    | 26          | 8.6               |
| i7-5775R           | 5.2     | 11.3    | 5.2    | 16.5        | 6.0               |
| i7-8565U           | 6.0     | 10.2    | 4.8    | 14.3        | 5.8               |
| Cortex A53 (R.Pi3) | 11.0    | 17.7    | 8.9    | 25          | 9.3               |
| Cortex A72 (R.Pi4) | 9.5     | 11.5    | 7.1    | 16.4        | 7.7               |
| Apple M1           | 5.7     | 12.0    | 5.6    | 17.8        | 7.2               |
| Apple M1           | 3.6     | 7.2     | 3.9    | 10.1        | 4.4               |

### RSA-2048 decryption with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 161     | 728     | 475    | 686         | 233               |
| i7-5775R           | 122     | 1055    | 390    | 415         | 166               |
| i7-8565U           | 105     | 935     | 348    | 384         | 162               |
| Cortex A53 (R.Pi3) | 357     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4) | 280     | 919     | 370    | 427         | 242               |
| Apple M1           | 189     | 488     | 355    | 480         | 205               |
| Apple M1           | 120     | 721     | 234    | 275         | 135               |

### RSA-2048 decryption/encryption ratio, with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 24      | 43      | 66     | 26          | 27                |
| i7-5775R           | 23      | 93      | 75     | 25          | 27                |
| i7-8565U           | 17.6    | 91      | 72     | 26          | 27                |
| Cortex A53 (R.Pi3) | 32      | 44      | 53     | 26          | 33                |
| Cortex A72 (R.Pi4) | 29      | 79      | 52     | 26          | 31                |
| Apple M1           | 33      | 40      | 63     | 27          | 28                |
| Apple M1           | 33      | 100     | 59     | 27          | 30                |

### RSA-2048 signature

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 159     | 719     | 472    | 683         | 232               |
| i7-5775R           | 114     | 1067    | 388    | 423         | 165               |
| i7-8565U           | 103     | 949     | 357    | 377         | 157               |
| Cortex A53 (R.Pi3) | 356     | 790     | 476    | 666         | 309               |
| Cortex A72 (R.Pi4) | 279     | 919     | 370    | 427         | 242               |
| Apple M1           | 188     | 487     | 355    | 480         | 204               |
| Apple M1           | 117     | 711     | 236    | 277         | 132               |

### RSA-2048 verification

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 5.7     | 16.1    | 7.9    | 27          | 8.4               |
| i7-5775R           | 3.9     | 10.9    | 5.7    | 17.6        | 5.9               |
| i7-8565U           | 5.0     | 10.0    | 5.2    | 13.7        | 5.3               |
| Cortex A53 (R.Pi3) | 10.0    | 17.5    | 9.1    | 25          | 9.2               |
| Cortex A72 (R.Pi4) | 7.9     | 11.4    | 7.3    | 16.3        | 7.6               |
| Apple M1           | 4.9     | 11.7    | 6.1    | 17.4        | 6.8               |
| Apple M1           | 3.0     | 6.9     | 4.1    | 9.8         | 4.3               |

### RSA-2048 signature/verification ratio

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 28      | 44      | 60     | 25          | 27                |
| i7-5775R           | 29      | 97      | 68     | 24          | 27                |
| i7-8565U           | 20      | 94      | 68     | 27          | 29                |
| Cortex A53 (R.Pi3) | 35      | 45      | 52     | 25          | 33                |
| Cortex A72 (R.Pi4) | 35      | 80      | 50     | 26          | 32                |
| Apple M1           | 38      | 41      | 57     | 27          | 30                |
| Apple M1           | 39      | 102     | 58     | 28          | 30                |

### RSA-2048 signature with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 207     | 717     | 473    | 716         | 229               |
| i7-5775R           | 114     | 1052    | 388    | 425         | 167               |
| i7-8565U           | 107     | 925     | 355    | 371         | 159               |
| Cortex A53 (R.Pi3) | 357     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4) | 281     | 920     | 370    | 427         | 242               |
| Apple M1           | 189     | 487     | 355    | 480         | 204               |
| Apple M1           | 120     | 731     | 329    | 277         | 136               |

### RSA-2048 verification with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 6.7     | 16.3    | 7.8    | 28          | 8.3               |
| i7-5775R           | 4.9     | 10.7    | 5.6    | 15.9        | 5.9               |
| i7-8565U           | 5.9     | 9.8     | 5.3    | 14.0        | 5.4               |
| Cortex A53 (R.Pi3) | 10.9    | 17.5    | 9.1    | 25          | 9.2               |
| Cortex A72 (R.Pi4) | 9.0     | 11.3    | 7.3    | 16.3        | 7.6               |
| Apple M1           | 5.5     | 11.7    | 6.1    | 17.4        | 6.8               |
| Apple M1           | 3.5     | 6.9     | 4.2    | 9.8         | 4.3               |

### RSA-2048 signature/verification ratio, with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 30      | 43      | 60     | 24          | 27                |
| i7-5775R           | 23      | 97      | 69     | 26          | 28                |
| i7-8565U           | 18.4    | 94      | 66     | 26          | 29                |
| Cortex A53 (R.Pi3) | 32      | 45      | 52     | 26          | 33                |
| Cortex A72 (R.Pi4) | 31      | 81      | 50     | 26          | 32                |
| Apple M1           | 34      | 41      | 57     | 27          | 30                |
| Apple M1           | 34      | 105     | 78     | 28          | 31                |

### RSA-2048 parse public key

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 18.0    | 0.30    | 1.2    | 5.4         | 2.1               |
| i7-5775R           | 29      | 0.15    | 0.60   | 3.1         | 1.3               |
| i7-8565U           | 24      | 0.11    | 0.55   | 2.4         | 1.1               |
| Cortex A53 (R.Pi3) | 20      | 0.11    | 0.47   | 1.9         | 0.58              |
| Cortex A72 (R.Pi4) | 24      | 0.12    | 0.54   | 1.6         | 0.74              |
| Apple M1           | 9.7     | 0.17    | 0.67   | 3.3         | 2.1               |
| Apple M1           | 16.0    | 0.11    | 0.37   | 2.0         | 1.3               |

### RSA-2048 parse private key

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 20      | 11.5    | 10.1   | 9.9         | 1.5               |
| i7-5775R           | 30      | 6.1     | 9.0    | 7.0         | 0.92              |
| i7-8565U           | 25      | 5.3     | 4.7    | 5.6         | 1.0               |
| Cortex A53 (R.Pi3) | 20      | 8.4     | 4.2    | 5.0         | 0.53              |
| Cortex A72 (R.Pi4) | 26      | 5.0     | 4.8    | 3.7         | 0.63              |
| Apple M1           | 10.9    | 7.8     | 6.2    | 5.1         | 0.88              |
| Apple M1           | 16.8    | 4.0     | 3.6    | 3.1         | 0.55              |

## RSA-4096

### RSA-4096 encryption

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 17.9    | 57      | 23     | 89          | 26                |
| i7-5775R           | 13.1    | 39      | 17.3   | 53          | 19.3              |
| i7-8565U           | 17.8    | 38      | 16.0   | 45          | 17.8              |
| Cortex A53 (R.Pi3) | 38      | 65      | 29     | 92          | 30                |
| Cortex A72 (R.Pi4) | 26      | 41      | 21     | 56          | 22                |
| Apple M1           | 18.3    | 43      | 20     | 61          | 23                |
| Apple M1           | 11.5    | 26      | 14.4   | 31          | 17.3              |

### RSA-4096 decryption

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 1058    | 4279    | 2555   | 3591        | 1614              |
| i7-5775R           | 770     | 4219    | 2496   | 2175        | 1160              |
| i7-8565U           | 1098    | 3944    | 1864   | 1966        | 1093              |
| Cortex A53 (R.Pi3) | 2481    | 5044    | 2853   | 4140        | 2113              |
| Cortex A72 (R.Pi4) | 1937    | 4073    | 2765   | 2517        | 1641              |
| Apple M1           | 1200    | 3028    | 2119   | 2483        | 1281              |
| Apple M1           | 754     | 2892    | 1418   | 1475        | 864               |

### RSA-4096 decryption/encryption ratio

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 59      | 75      | 109    | 40          | 61                |
| i7-5775R           | 58      | 107     | 144    | 40          | 60                |
| i7-8565U           | 61      | 103     | 116    | 43          | 61                |
| Cortex A53 (R.Pi3) | 65      | 76      | 95     | 45          | 69                |
| Cortex A72 (R.Pi4) | 72      | 98      | 127    | 44          | 71                |
| Apple M1           | 65      | 70      | 101    | 40          | 53                |
| Apple M1           | 65      | 108     | 98     | 47          | 49                |

### RSA-4096 encryption with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 19.2    | 56      | 23     | 88          | 26                |
| i7-5775R           | 14.0    | 38      | 20     | 52          | 19.6              |
| i7-8565U           | 19.2    | 37      | 16.1   | 47          | 17.5              |
| Cortex A53 (R.Pi3) | 38      | 65      | 29     | 91          | 30                |
| Cortex A72 (R.Pi4) | 27      | 41      | 21     | 56          | 22                |
| Apple M1           | 18.8    | 43      | 20     | 61          | 23                |
| Apple M1           | 12.0    | 27      | 13.8   | 31          | 17.7              |

### RSA-4096 decryption with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 1064    | 4192    | 2555   | 3553        | 1604              |
| i7-5775R           | 761     | 4223    | 2207   | 2180        | 1167              |
| i7-8565U           | 1097    | 3951    | 1851   | 1930        | 1057              |
| Cortex A53 (R.Pi3) | 2491    | 5056    | 2853   | 4139        | 2112              |
| Cortex A72 (R.Pi4) | 1944    | 4073    | 2766   | 2517        | 1641              |
| Apple M1           | 1200    | 3029    | 2119   | 2482        | 1281              |
| Apple M1           | 763     | 2847    | 1431   | 1412        | 833               |

### RSA-4096 decryption/encryption ratio, with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 55      | 74      | 109    | 40          | 61                |
| i7-5775R           | 54      | 108     | 108    | 41          | 59                |
| i7-8565U           | 57      | 106     | 114    | 40          | 60                |
| Cortex A53 (R.Pi3) | 64      | 76      | 95     | 45          | 69                |
| Cortex A72 (R.Pi4) | 69      | 98      | 127    | 44          | 71                |
| Apple M1           | 63      | 70      | 101    | 40          | 53                |
| Apple M1           | 63      | 104     | 103    | 44          | 47                |

### RSA-4096 signature

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 1044    | 4295    | 2579   | 3620        | 1604              |
| i7-5775R           | 798     | 4213    | 2068   | 2185        | 1225              |
| i7-8565U           | 1093    | 3820    | 1882   | 1975        | 1041              |
| Cortex A53 (R.Pi3) | 2489    | 5049    | 2853   | 4141        | 2113              |
| Cortex A72 (R.Pi4) | 1938    | 4106    | 2764   | 2516        | 1641              |
| Apple M1           | 1199    | 3030    | 2119   | 2482        | 1275              |
| Apple M1           | 759     | 2899    | 1415   | 1520        | 870               |

### RSA-4096 verification

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 17.4    | 56      | 24     | 89          | 25                |
| i7-5775R           | 13.0    | 37      | 18.0   | 53          | 20                |
| i7-8565U           | 17.7    | 36      | 17.0   | 48          | 17.9              |
| Cortex A53 (R.Pi3) | 37      | 65      | 30     | 91          | 30                |
| Cortex A72 (R.Pi4) | 25      | 41      | 22     | 55          | 22                |
| Apple M1           | 17.8    | 42      | 23     | 60          | 24                |
| Apple M1           | 11.3    | 26      | 13.8   | 30          | 15.1              |

### RSA-4096 signature/verification ratio

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 60      | 76      | 104    | 40          | 62                |
| i7-5775R           | 61      | 111     | 114    | 40          | 61                |
| i7-8565U           | 61      | 104     | 110    | 41          | 58                |
| Cortex A53 (R.Pi3) | 66      | 77      | 94     | 45          | 69                |
| Cortex A72 (R.Pi4) | 74      | 100     | 124    | 45          | 72                |
| Apple M1           | 67      | 70      | 89     | 40          | 51                |
| Apple M1           | 67      | 109     | 102    | 49          | 57                |

### RSA-4096 signature with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 1075    | 4261    | 2570   | 3647        | 1731              |
| i7-5775R           | 756     | 4160    | 2077   | 2700        | 1208              |
| i7-8565U           | 1108    | 3864    | 1878   | 1997        | 1067              |
| Cortex A53 (R.Pi3) | 2491    | 5050    | 2854   | 4177        | 2112              |
| Cortex A72 (R.Pi4) | 1941    | 4107    | 2763   | 2517        | 1641              |
| Apple M1           | 1201    | 3002    | 2120   | 2484        | 1275              |
| Apple M1           | 766     | 2893    | 1432   | 1406        | 833               |

### RSA-4096 verification with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 19.1    | 56      | 24     | 90          | 29                |
| i7-5775R           | 14.0    | 37      | 17.9   | 53          | 18.6              |
| i7-8565U           | 18.4    | 35      | 16.8   | 46          | 17.6              |
| Cortex A53 (R.Pi3) | 38      | 65      | 30     | 91          | 30                |
| Cortex A72 (R.Pi4) | 27      | 41      | 22     | 55          | 22                |
| Apple M1           | 18.4    | 42      | 23     | 60          | 24                |
| Apple M1           | 11.7    | 26      | 14.0   | 32          | 15.5              |

### RSA-4096 signature/verification ratio, with rekeying

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 56      | 75      | 105    | 40          | 59                |
| i7-5775R           | 54      | 109     | 116    | 50          | 64                |
| i7-8565U           | 60      | 107     | 111    | 43          | 60                |
| Cortex A53 (R.Pi3) | 64      | 77      | 94     | 45          | 69                |
| Cortex A72 (R.Pi4) | 71      | 100     | 124    | 45          | 72                |
| Apple M1           | 65      | 70      | 89     | 40          | 51                |
| Apple M1           | 65      | 108     | 102    | 43          | 53                |

### RSA-4096 parse public key

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 18.0    | 0.31    | 1.3    | 13.2        | 3.7               |
| i7-5775R           | 29      | 0.15    | 1.1    | 8.6         | 2.3               |
| i7-8565U           | 25      | 0.11    | 0.75   | 6.3         | 2.2               |
| Cortex A53 (R.Pi3) | 20      | 0.11    | 0.54   | 5.4         | 1.0               |
| Cortex A72 (R.Pi4) | 24      | 0.12    | 0.66   | 4.1         | 1.3               |
| Apple M1           | 10.0    | 0.17    | 0.77   | 8.7         | 3.9               |
| Apple M1           | 16.4    | 0.11    | 0.44   | 4.6         | 2.4               |

### RSA-4096 parse private key

| CPU                | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ------------------ | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R           | 20      | 30      | 16.9   | 33          | 2.3               |
| i7-5775R           | 29      | 17.0    | 10.4   | 24          | 1.5               |
| i7-8565U           | 25      | 14.6    | 10.2   | 17.4        | 1.7               |
| Cortex A53 (R.Pi3) | 20      | 25      | 8.2    | 15.5        | 0.81              |
| Cortex A72 (R.Pi4) | 26      | 13.8    | 9.0    | 10.7        | 1.0               |
| Apple M1           | 11.5    | 21      | 11.6   | 16.9        | 1.5               |
| Apple M1           | 17.1    | 13.1    | 6.4    | 8.2         | 0.86              |

## AES-128-CBC

### AES-128-CBC encryption

| CPU                | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ------------------ | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R           | 340     | 491     | 343    | 344    | 1299        |             |
| i7-5775R           | 252     | 436     | 246    | 255    | 740         |             |
| i7-8565U           | 130     | 287     | 135    | 127    | 689         |             |
| Cortex A53 (R.Pi3) | 279     | 674     | 729    | 728    | 496         |             |
| Cortex A72 (R.Pi4) | 1101    | 608     | 704    | 703    | 748         |             |
| Apple M1           | 218     | 395     | 1120   | 1116   | 1176        | 334         |
| Apple M1           | 134     | 863     | 133    | 248    | 792         | 213         |

### AES-128-CBC decryption

| CPU                | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ------------------ | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R           | 49      | 279     | 50     | 65     | 1190        |             |
| i7-5775R           | 35      | 294     | 35     | 48     | 743         |             |
| i7-8565U           | 30      | 228     | 31     | 39     | 667         |             |
| Cortex A53 (R.Pi3) | 294     | 608     | 629    | 629    | 531         |             |
| Cortex A72 (R.Pi4) | 787     | 624     | 652    | 650    | 723         |             |
| Apple M1           | 25      | 164     | 898    | 899    | 1093        | 49          |
| Apple M1           | 18.6    | 738     | 19.2   | 28     | 727         | 36          |

### AES-128-CBC encryption/decryption ratio

| CPU                | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt |
| ------------------ | :-----: | :-----: | :----: | :----: | :---------: |
| i7-5775R           | 6.9     | 1.8     | 6.8    | 5.2    | 1.1         |
| i7-5775R           | 7.2     | 1.5     | 7.0    | 5.3    | 1.00        |
| i7-8565U           | 4.2     | 1.3     | 4.4    | 3.3    | 1.0         |
| Cortex A53 (R.Pi3) | 0.95    | 1.1     | 1.2    | 1.2    | 0.93        |
| Cortex A72 (R.Pi4) | 1.4     | 0.97    | 1.1    | 1.1    | 1.0         |
| Apple M1           | 8.4     | 2.4     | 1.2    | 1.2    | 1.1         |
| Apple M1           | 7.2     | 1.2     | 6.9    | 8.9    | 1.1         |

## AES-256-CBC

### AES-256-CBC encryption

| CPU                | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ------------------ | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R           | 475     | 617     | 477    | 473    | 1736        |             |
| i7-5775R           | 347     | 531     | 348    | 351    | 994         |             |
| i7-8565U           | 175     | 323     | 177    | 176    | 913         |             |
| Cortex A53 (R.Pi3) | 368     | 869     | 943    | 940    | 638         |             |
| Cortex A72 (R.Pi4) | 1542    | 765     | 942    | 936    | 961         |             |
| Apple M1           | 309     | 485     | 1481   | 1478   | 1542        | 454         |
| Apple M1           | 192     | 1083    | 189    | 297    | 1000        | 271         |

### AES-256-CBC decryption

| CPU                | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ------------------ | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R           | 68      | 379     | 68     | 91     | 1639        |             |
| i7-5775R           | 49      | 327     | 49     | 66     | 1037        |             |
| i7-8565U           | 42      | 253     | 43     | 51     | 899         |             |
| Cortex A53 (R.Pi3) | 378     | 783     | 841    | 841    | 682         |             |
| Cortex A72 (R.Pi4) | 1111    | 786     | 887    | 885    | 934         |             |
| Apple M1           | 49      | 191     | 1259   | 1354   | 1457        | 66          |
| Apple M1           | 32      | 919     | 27     | 31     | 915         | 47          |

### AES-256-CBC encryption/decryption ratio

| CPU                | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt |
| ------------------ | :-----: | :-----: | :----: | :----: | :---------: |
| i7-5775R           | 6.9     | 1.6     | 7.0    | 5.1    | 1.1         |
| i7-5775R           | 7.0     | 1.6     | 7.0    | 5.3    | 0.96        |
| i7-8565U           | 4.1     | 1.3     | 4.1    | 3.5    | 1.0         |
| Cortex A53 (R.Pi3) | 0.97    | 1.1     | 1.1    | 1.1    | 0.94        |
| Cortex A72 (R.Pi4) | 1.4     | 0.97    | 1.1    | 1.1    | 1.0         |
| Apple M1           | 6.3     | 2.5     | 1.2    | 1.1    | 1.1         |
| Apple M1           | 6.0     | 1.2     | 6.8    | 9.4    | 1.1         |

