# Comparative benchmarks results

**Contents:**
* [Results overview](#results-overview)
* [Reference test](#reference-test)
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
  * [2048-bit modular arithmetic basic operations](#2048-bit-modular-arithmetic-basic-operations)
  * [2048-bit modular arithmetic exponentiations on public (short) exponents](#2048-bit-modular-arithmetic-exponentiations-on-public-short-exponents)
  * [2048-bit modular arithmetic exponentiations on private (large) exponents](#2048-bit-modular-arithmetic-exponentiations-on-private-large-exponents)

## Results overview

This page compares the performannce of some cryptographic libraries
on some processors.

Several types of results are presented:
- Execution time in microseconds. Used to compare processors on the same
  operation.
- Relative performance factor (the lower, the better). This the ratio between
  the execution time of an algorithm and a "reference test" on the same
  system.
- Ratio between encryption and decryption (or signature and verification)
  on the same algorithm.

The relative performance factor values have no unit and their absolute value
is meaningless. They are relative to the performance of the CPU. A standard
reference test is run on each CPU before running the cryptographic operations.
The presented number is the ratio of the execution time of the evaluated
cryptographic operation over the execution time of the standard test.

Thus, if all CPU's had the same implementation and only differed in speed,
the score of one operation would be the same on all CPU's.

A lower relative execution time on a CPU maybe due to several reasons:
- Better performance of the instructions which are characteristic of that operation.
- Dedicated accelerated instructions for that operation (e.g. AES and SHA-x).
- More efficient compiler (gcc is used on Linux, clang on macOS).
- More efficient code generation options to produce the binary of the library.

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

Additional results are provided for elementary operations on large numbers,
as used in asymmetric cryptography.

In the result tables, Intel/AMD processors come first, them Arm processors.

For systems with known CPU core frequencies, the "execution time" ratio is presented.
This is a performance ratio, compared to a reference system, with respect to their frequencies.

Reference system: AWS Graviton 3 (Neoverse V1)

## Reference test

The "reference test" is a set of basic integer operations which are typically used in
cryptography. There is no floating point operation. This test serves two purposes:
- The absolute time of the reference test is a comparision between processors.
- Each cryptographic algorithm is compared to the reference test on the same machine.

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | Reference test |
| ---------------------------- | ----- | :------------: |
| i7-5775R                     | macOS | 3518           |
| i7-5775R                     | Linux | 5250           |
| i7-8565U                     | Linux | 5058           |
| i7-13700H                    | Linux | 2846           |
| Xeon-6254                    | Linux | 6846           |
| Xeon-9460                    | Linux | 5261           |
| Cortex A53 (R.Pi3)           | Linux | 75122          |
| Cortex A72 (R.Pi4)           | Linux | 17928          |
| Apple M1                     | macOS | 2892           |
| Apple M1                     | Linux | 5307           |
| Apple M3                     | macOS | 2430           |
| Ampere Altra (Neoverse N1)   | Linux | 10241          |
| AWS Graviton 3 (Neoverse V1) | Linux | 8390           |
| Nvidia Grace (Neoverse V2)   | Linux | 6894           |
| RISC-V (Qemu on Apple M1)    | Linux | 19068          |
| RISC-V (Qemu on i7-8565U)    | Linux | 31487          |
| s390x (Qemu on i7-8565U)     | Linux | 42621          |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | Reference test |
| ---------------------------- | ----- | :------------: |
| Xeon-9460                    | Linux | 0.47           |
| Cortex A72 (R.Pi4)           | Linux | 3.1            |
| Apple M1                     | macOS | 0.28           |
| Apple M1                     | Linux | 0.51           |
| Apple M3                     | macOS | 0.19           |
| Ampere Altra (Neoverse N1)   | Linux | 1.1            |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0            |
| Nvidia Grace (Neoverse V2)   | Linux | 0.65           |

## RSA-2048

### RSA-2048 encryption

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 20      | 57      | 25     | 93          | 29                |
| i7-5775R                     | Linux | 21      | 59      | 28     | 95          | 31                |
| i7-8565U                     | Linux | 36      | 55      | 26     | 74          | 29                |
| i7-13700H                    | Linux | 17.2    | 35      | 14.8   | 47          | 16.7              |
| Xeon-6254                    | Linux | 19.8    | 51      | 23     | 82          |                   |
| Xeon-9460                    | Linux | 18.6    | 39      | 22     | 54          |                   |
| Cortex A53 (R.Pi3)           | Linux | 775     | 1328    | 671    | 1927        | 698               |
| Cortex A72 (R.Pi4)           | Linux | 154     | 205     | 127    | 295         | 138               |
| Apple M1                     | macOS | 15.2    | 34      | 16.5   | 52          | 21                |
| Apple M1                     | Linux | 16.7    | 38      | 20.0   | 54          | 24                |
| Apple M3                     | macOS | 11.2    | 143     | 13.0   | 40          | 17.0              |
| Ampere Altra (Neoverse N1)   | Linux | 81      | 116     | 74     | 156         | 79                |
| AWS Graviton 3 (Neoverse V1) | Linux | 26      | 61      | 29     | 74          | 32                |
| Nvidia Grace (Neoverse V2)   | Linux | 20.0    | 37      | 20     | 49          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 298     | 690     | 143    | 393         | 171               |
| RISC-V (Qemu on i7-8565U)    | Linux | 470     | 970     | 235    | 650         | 285               |
| s390x (Qemu on i7-8565U)     | Linux | 870     | 0.000   | 942    | 2406        | 0.000             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 5.8     | 16.4    | 7.2    | 26          | 8.5               |
| i7-5775R                     | Linux | 4.1     | 11.4    | 5.4    | 18.1        | 6.0               |
| i7-8565U                     | Linux | 7.2     | 11.0    | 5.3    | 14.6        | 5.7               |
| i7-13700H                    | Linux | 6.0     | 12.3    | 5.2    | 16.8        | 5.9               |
| Xeon-6254                    | Linux | 2.9     | 7.6     | 3.4    | 12.1        |                   |
| Xeon-9460                    | Linux | 3.5     | 7.5     | 4.3    | 10.4        |                   |
| Cortex A53 (R.Pi3)           | Linux | 10.3    | 17.7    | 8.9    | 25          | 9.3               |
| Cortex A72 (R.Pi4)           | Linux | 8.6     | 11.5    | 7.1    | 16.5        | 7.7               |
| Apple M1                     | macOS | 5.3     | 11.8    | 5.7    | 18.1        | 7.4               |
| Apple M1                     | Linux | 3.1     | 7.3     | 3.8    | 10.2        | 4.6               |
| Apple M3                     | macOS | 4.6     | 59      | 5.3    | 16.9        | 7.0               |
| Ampere Altra (Neoverse N1)   | Linux | 8.0     | 11.4    | 7.3    | 15.3        | 7.7               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.2     | 7.4     | 3.5    | 8.9         | 3.9               |
| Nvidia Grace (Neoverse V2)   | Linux | 2.9     | 5.5     | 3.0    | 7.2         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 15.7    | 36      | 7.5    | 20          | 9.0               |
| RISC-V (Qemu on i7-8565U)    | Linux | 15.0    | 30      | 7.5    | 20          | 9.1               |
| s390x (Qemu on i7-8565U)     | Linux | 20      | 0.000   | 22     | 56          | 0.000             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.51    | 0.47    | 0.58   | 0.55        |                   |
| Cortex A72 (R.Pi4)           | Linux | 8.3     | 4.8     | 6.4    | 5.7         | 6.1               |
| Apple M1                     | macOS | 0.46    | 0.45    | 0.46   | 0.57        | 0.53              |
| Apple M1                     | Linux | 0.50    | 0.51    | 0.56   | 0.59        | 0.61              |
| Apple M3                     | macOS | 0.27    | 1.5     | 0.29   | 0.36        | 0.34              |
| Ampere Altra (Neoverse N1)   | Linux | 2.6     | 1.6     | 2.2    | 1.8         | 2.1               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.58    | 0.48    | 0.56   | 0.53        |                   |

### RSA-2048 decryption

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 551     | 2545    | 1661   | 2373        | 803               |
| i7-5775R                     | Linux | 589     | 5625    | 2073   | 2300        | 878               |
| i7-8565U                     | Linux | 598     | 5537    | 2021   | 1976        | 823               |
| i7-13700H                    | Linux | 500     | 1479    | 979    | 1317        | 457               |
| Xeon-6254                    | Linux | 1483    | 2412    | 1548   | 2024        |                   |
| Xeon-9460                    | Linux | 1169    | 1771    | 1543   | 1435        |                   |
| Cortex A53 (R.Pi3)           | Linux | 26744   | 59438   | 35755  | 50136       | 23283             |
| Cortex A72 (R.Pi4)           | Linux | 4997    | 16383   | 6631   | 7686        | 4347              |
| Apple M1                     | macOS | 550     | 1409    | 1036   | 1407        | 593               |
| Apple M1                     | Linux | 601     | 3781    | 1240   | 1481        | 733               |
| Apple M3                     | macOS | 392     | 1108    | 773    | 1179        | 436               |
| Ampere Altra (Neoverse N1)   | Linux | 2857    | 4523    | 3617   | 4145        | 2569              |
| AWS Graviton 3 (Neoverse V1) | Linux | 965     | 2538    | 1675   | 2046        | 952               |
| Nvidia Grace (Neoverse V2)   | Linux | 1548    | 1659    | 1172   | 1363        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 11311   | 33374   | 10874  | 10760       | 5675              |
| RISC-V (Qemu on i7-8565U)    | Linux | 17508   | 46882   | 16979  | 16146       | 9388              |
| s390x (Qemu on i7-8565U)     | Linux | 20866   | 39536   | 62180  | 72203       | 22908             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 156     | 723     | 472    | 674         | 228               |
| i7-5775R                     | Linux | 112     | 1071    | 394    | 438         | 167               |
| i7-8565U                     | Linux | 118     | 1094    | 399    | 390         | 162               |
| i7-13700H                    | Linux | 175     | 519     | 344    | 462         | 160               |
| Xeon-6254                    | Linux | 216     | 352     | 226    | 295         |                   |
| Xeon-9460                    | Linux | 222     | 336     | 293    | 272         |                   |
| Cortex A53 (R.Pi3)           | Linux | 356     | 791     | 475    | 667         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 278     | 913     | 369    | 428         | 242               |
| Apple M1                     | macOS | 190     | 487     | 358    | 486         | 205               |
| Apple M1                     | Linux | 113     | 712     | 233    | 279         | 138               |
| Apple M3                     | macOS | 161     | 456     | 318    | 485         | 179               |
| Ampere Altra (Neoverse N1)   | Linux | 279     | 441     | 353    | 404         | 250               |
| AWS Graviton 3 (Neoverse V1) | Linux | 115     | 302     | 199    | 243         | 113               |
| Nvidia Grace (Neoverse V2)   | Linux | 224     | 240     | 170    | 197         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 593     | 1750    | 570    | 564         | 297               |
| RISC-V (Qemu on i7-8565U)    | Linux | 556     | 1488    | 539    | 512         | 298               |
| s390x (Qemu on i7-8565U)     | Linux | 489     | 927     | 1458   | 1694        | 537               |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.90    | 0.52    | 0.68   | 0.52        |                   |
| Cortex A72 (R.Pi4)           | Linux | 7.5     | 9.3     | 5.7    | 5.4         | 6.6               |
| Apple M1                     | macOS | 0.46    | 0.45    | 0.50   | 0.56        | 0.51              |
| Apple M1                     | Linux | 0.51    | 1.2     | 0.60   | 0.59        | 0.63              |
| Apple M3                     | macOS | 0.26    | 0.28    | 0.30   | 0.37        | 0.30              |
| Ampere Altra (Neoverse N1)   | Linux | 2.6     | 1.5     | 1.9    | 1.8         | 2.3               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 1.3     | 0.52    | 0.55   | 0.52        |                   |

### RSA-2048 decryption/encryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 27      | 44      | 65     | 25          | 26                |
| i7-5775R                     | Linux | 27      | 94      | 73     | 24          | 27                |
| i7-8565U                     | Linux | 16.3    | 99      | 76     | 26          | 28                |
| i7-13700H                    | Linux | 29      | 42      | 66     | 27          | 27                |
| Xeon-6254                    | Linux | 74      | 46      | 66     | 24          |                   |
| Xeon-9460                    | Linux | 62      | 44      | 67     | 26          |                   |
| Cortex A53 (R.Pi3)           | Linux | 34      | 44      | 53     | 26          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 32      | 79      | 51     | 25          | 31                |
| Apple M1                     | macOS | 36      | 41      | 62     | 26          | 27                |
| Apple M1                     | Linux | 36      | 97      | 62     | 27          | 29                |
| Apple M3                     | macOS | 34      | 7.7     | 59     | 28          | 25                |
| Ampere Altra (Neoverse N1)   | Linux | 35      | 38      | 48     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 35      | 40      | 57     | 27          | 29                |
| Nvidia Grace (Neoverse V2)   | Linux | 77      | 43      | 57     | 27          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 37      | 48      | 75     | 27          | 33                |
| RISC-V (Qemu on i7-8565U)    | Linux | 37      | 48      | 72     | 24          | 32                |
| s390x (Qemu on i7-8565U)     | Linux | 23      | 0.000   | 65     | 30          | 0.000             |

### RSA-2048 encryption with rekeying

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 23      | 57      | 25     | 93          | 29                |
| i7-5775R                     | Linux | 26      | 62      | 28     | 86          | 31                |
| i7-8565U                     | Linux | 41      | 56      | 25     | 74          | 29                |
| i7-13700H                    | Linux | 18.9    | 34      | 14.7   | 47          | 16.8              |
| Xeon-6254                    | Linux | 20      | 51      | 23     | 82          |                   |
| Xeon-9460                    | Linux | 20      | 39      | 22     | 54          |                   |
| Cortex A53 (R.Pi3)           | Linux | 826     | 1328    | 670    | 1927        | 698               |
| Cortex A72 (R.Pi4)           | Linux | 170     | 205     | 127    | 295         | 138               |
| Apple M1                     | macOS | 16.6    | 34      | 16.5   | 52          | 21                |
| Apple M1                     | Linux | 18.4    | 38      | 19.8   | 52          | 24                |
| Apple M3                     | macOS | 12.5    | 143     | 13.0   | 41          | 17.0              |
| Ampere Altra (Neoverse N1)   | Linux | 85      | 116     | 74     | 155         | 78                |
| AWS Graviton 3 (Neoverse V1) | Linux | 29      | 61      | 29     | 74          | 32                |
| Nvidia Grace (Neoverse V2)   | Linux | 21      | 38      | 20     | 50          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 357     | 689     | 143    | 392         | 171               |
| RISC-V (Qemu on i7-8565U)    | Linux | 612     | 992     | 234    | 769         | 301               |
| s390x (Qemu on i7-8565U)     | Linux | 1028    | 1090    | 844    | 2643        | 0.000             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 6.8     | 16.4    | 7.2    | 26          | 8.5               |
| i7-5775R                     | Linux | 5.1     | 11.8    | 5.4    | 16.5        | 5.9               |
| i7-8565U                     | Linux | 8.2     | 11.1    | 5.0    | 14.7        | 5.9               |
| i7-13700H                    | Linux | 6.6     | 12.0    | 5.2    | 16.8        | 5.9               |
| Xeon-6254                    | Linux | 2.9     | 7.5     | 3.4    | 12.1        |                   |
| Xeon-9460                    | Linux | 4.0     | 7.5     | 4.3    | 10.4        |                   |
| Cortex A53 (R.Pi3)           | Linux | 11.0    | 17.7    | 8.9    | 25          | 9.3               |
| Cortex A72 (R.Pi4)           | Linux | 9.5     | 11.5    | 7.1    | 16.5        | 7.7               |
| Apple M1                     | macOS | 5.7     | 11.8    | 5.7    | 18.1        | 7.3               |
| Apple M1                     | Linux | 3.5     | 7.3     | 3.7    | 9.9         | 4.6               |
| Apple M3                     | macOS | 5.2     | 59      | 5.3    | 16.9        | 7.0               |
| Ampere Altra (Neoverse N1)   | Linux | 8.3     | 11.4    | 7.3    | 15.2        | 7.7               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.6     | 7.4     | 3.5    | 8.9         | 3.9               |
| Nvidia Grace (Neoverse V2)   | Linux | 3.2     | 5.5     | 3.0    | 7.3         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 18.7    | 36      | 7.5    | 20          | 9.0               |
| RISC-V (Qemu on i7-8565U)    | Linux | 19.4    | 31      | 7.4    | 24          | 9.6               |
| s390x (Qemu on i7-8565U)     | Linux | 24      | 25      | 19.8   | 62          | 0.000             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.52    | 0.47    | 0.58   | 0.55        |                   |
| Cortex A72 (R.Pi4)           | Linux | 8.2     | 4.8     | 6.4    | 5.7         | 6.1               |
| Apple M1                     | macOS | 0.45    | 0.45    | 0.46   | 0.57        | 0.53              |
| Apple M1                     | Linux | 0.50    | 0.51    | 0.55   | 0.57        | 0.60              |
| Apple M3                     | macOS | 0.27    | 1.5     | 0.29   | 0.36        | 0.34              |
| Ampere Altra (Neoverse N1)   | Linux | 2.5     | 1.6     | 2.2    | 1.8         | 2.1               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.58    | 0.48    | 0.56   | 0.53        |                   |

### RSA-2048 decryption with rekeying

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 558     | 2544    | 1657   | 2363        | 802               |
| i7-5775R                     | Linux | 692     | 6200    | 2104   | 2292        | 904               |
| i7-8565U                     | Linux | 552     | 5046    | 1821   | 2002        | 841               |
| i7-13700H                    | Linux | 502     | 1515    | 977    | 1312        | 457               |
| Xeon-6254                    | Linux | 1481    | 2410    | 1550   | 2020        |                   |
| Xeon-9460                    | Linux | 1172    | 1771    | 1543   | 1433        |                   |
| Cortex A53 (R.Pi3)           | Linux | 26820   | 59495   | 35773  | 50100       | 23282             |
| Cortex A72 (R.Pi4)           | Linux | 5017    | 16382   | 6632   | 7686        | 4348              |
| Apple M1                     | macOS | 552     | 1427    | 1031   | 1406        | 592               |
| Apple M1                     | Linux | 601     | 3741    | 1253   | 1500        | 729               |
| Apple M3                     | macOS | 395     | 1109    | 776    | 1177        | 436               |
| Ampere Altra (Neoverse N1)   | Linux | 2862    | 4523    | 3617   | 4145        | 2555              |
| AWS Graviton 3 (Neoverse V1) | Linux | 968     | 2538    | 1674   | 2045        | 952               |
| Nvidia Grace (Neoverse V2)   | Linux | 1551    | 1660    | 1172   | 1367        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 11420   | 33344   | 10873  | 10763       | 5673              |
| RISC-V (Qemu on i7-8565U)    | Linux | 17576   | 46636   | 16914  | 24326       | 9631              |
| s390x (Qemu on i7-8565U)     | Linux | 22485   | 43794   | 53415  | 67351       | 19283             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 158     | 723     | 471    | 671         | 228               |
| i7-5775R                     | Linux | 131     | 1180    | 400    | 436         | 172               |
| i7-8565U                     | Linux | 109     | 997     | 360    | 395         | 166               |
| i7-13700H                    | Linux | 176     | 532     | 343    | 461         | 160               |
| Xeon-6254                    | Linux | 216     | 352     | 226    | 295         |                   |
| Xeon-9460                    | Linux | 222     | 336     | 293    | 272         |                   |
| Cortex A53 (R.Pi3)           | Linux | 357     | 791     | 476    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 279     | 913     | 369    | 428         | 242               |
| Apple M1                     | macOS | 190     | 493     | 356    | 486         | 204               |
| Apple M1                     | Linux | 113     | 704     | 236    | 282         | 137               |
| Apple M3                     | macOS | 162     | 456     | 319    | 484         | 179               |
| Ampere Altra (Neoverse N1)   | Linux | 279     | 441     | 353    | 404         | 249               |
| AWS Graviton 3 (Neoverse V1) | Linux | 115     | 302     | 199    | 243         | 113               |
| Nvidia Grace (Neoverse V2)   | Linux | 225     | 240     | 170    | 198         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 598     | 1748    | 570    | 564         | 297               |
| RISC-V (Qemu on i7-8565U)    | Linux | 558     | 1481    | 537    | 772         | 305               |
| s390x (Qemu on i7-8565U)     | Linux | 527     | 1027    | 1253   | 1580        | 452               |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.90    | 0.52    | 0.68   | 0.52        |                   |
| Cortex A72 (R.Pi4)           | Linux | 7.5     | 9.3     | 5.7    | 5.4         | 6.6               |
| Apple M1                     | macOS | 0.46    | 0.46    | 0.50   | 0.56        | 0.51              |
| Apple M1                     | Linux | 0.50    | 1.2     | 0.61   | 0.60        | 0.62              |
| Apple M3                     | macOS | 0.27    | 0.28    | 0.30   | 0.37        | 0.30              |
| Ampere Altra (Neoverse N1)   | Linux | 2.6     | 1.5     | 1.9    | 1.8         | 2.3               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 1.3     | 0.52    | 0.55   | 0.53        |                   |

### RSA-2048 decryption/encryption ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 23      | 43      | 65     | 25          | 26                |
| i7-5775R                     | Linux | 26      | 99      | 73     | 26          | 28                |
| i7-8565U                     | Linux | 13.3    | 90      | 72     | 26          | 28                |
| i7-13700H                    | Linux | 26      | 44      | 66     | 27          | 27                |
| Xeon-6254                    | Linux | 73      | 46      | 66     | 24          |                   |
| Xeon-9460                    | Linux | 56      | 44      | 67     | 26          |                   |
| Cortex A53 (R.Pi3)           | Linux | 32      | 44      | 53     | 25          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 29      | 79      | 51     | 26          | 31                |
| Apple M1                     | macOS | 33      | 41      | 62     | 26          | 27                |
| Apple M1                     | Linux | 32      | 97      | 63     | 28          | 30                |
| Apple M3                     | macOS | 31      | 7.7     | 59     | 28          | 25                |
| Ampere Altra (Neoverse N1)   | Linux | 33      | 38      | 48     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 32      | 40      | 57     | 27          | 29                |
| Nvidia Grace (Neoverse V2)   | Linux | 70      | 43      | 57     | 27          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 31      | 48      | 75     | 27          | 33                |
| RISC-V (Qemu on i7-8565U)    | Linux | 28      | 47      | 72     | 31          | 31                |
| s390x (Qemu on i7-8565U)     | Linux | 21      | 40      | 63     | 25          | 0.000             |

### RSA-2048 signature

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 547     | 2546    | 1652   | 2364        | 803               |
| i7-5775R                     | Linux | 623     | 5633    | 2102   | 2363        | 1003              |
| i7-8565U                     | Linux | 546     | 5356    | 1830   | 1958        | 846               |
| i7-13700H                    | Linux | 506     | 1735    | 979    | 1322        | 456               |
| Xeon-6254                    | Linux | 533     | 2476    | 1558   | 1999        |                   |
| Xeon-9460                    | Linux | 297     | 1771    | 1539   | 1441        |                   |
| Cortex A53 (R.Pi3)           | Linux | 26762   | 59432   | 35750  | 50109       | 23280             |
| Cortex A72 (R.Pi4)           | Linux | 5005    | 16439   | 6626   | 7728        | 4344              |
| Apple M1                     | macOS | 549     | 1430    | 1036   | 1405        | 595               |
| Apple M1                     | Linux | 598     | 3761    | 1226   | 1465        | 723               |
| Apple M3                     | macOS | 390     | 1102    | 774    | 1177        | 435               |
| Ampere Altra (Neoverse N1)   | Linux | 2856    | 4523    | 3615   | 4144        | 2568              |
| AWS Graviton 3 (Neoverse V1) | Linux | 964     | 2541    | 1674   | 2043        | 951               |
| Nvidia Grace (Neoverse V2)   | Linux | 732     | 1659    | 1170   | 1362        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 11331   | 33357   | 10883  | 10770       | 5667              |
| RISC-V (Qemu on i7-8565U)    | Linux | 18653   | 47068   | 16817  | 24189       | 9752              |
| s390x (Qemu on i7-8565U)     | Linux | 21569   | 45935   | 65504  | 63075       | 19758             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 155     | 723     | 469    | 672         | 228               |
| i7-5775R                     | Linux | 118     | 1072    | 400    | 450         | 191               |
| i7-8565U                     | Linux | 108     | 1058    | 361    | 387         | 167               |
| i7-13700H                    | Linux | 177     | 609     | 344    | 464         | 160               |
| Xeon-6254                    | Linux | 77      | 361     | 227    | 292         |                   |
| Xeon-9460                    | Linux | 56      | 336     | 292    | 273         |                   |
| Cortex A53 (R.Pi3)           | Linux | 356     | 791     | 475    | 667         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 279     | 916     | 369    | 431         | 242               |
| Apple M1                     | macOS | 189     | 494     | 358    | 485         | 205               |
| Apple M1                     | Linux | 112     | 708     | 231    | 276         | 136               |
| Apple M3                     | macOS | 160     | 453     | 318    | 484         | 179               |
| Ampere Altra (Neoverse N1)   | Linux | 278     | 441     | 353    | 404         | 250               |
| AWS Graviton 3 (Neoverse V1) | Linux | 114     | 302     | 199    | 243         | 113               |
| Nvidia Grace (Neoverse V2)   | Linux | 106     | 240     | 169    | 197         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 594     | 1749    | 570    | 564         | 297               |
| RISC-V (Qemu on i7-8565U)    | Linux | 592     | 1494    | 534    | 768         | 309               |
| s390x (Qemu on i7-8565U)     | Linux | 506     | 1077    | 1536   | 1479        | 463               |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.23    | 0.52    | 0.68   | 0.52        |                   |
| Cortex A72 (R.Pi4)           | Linux | 7.5     | 9.3     | 5.7    | 5.5         | 6.6               |
| Apple M1                     | macOS | 0.46    | 0.46    | 0.50   | 0.56        | 0.51              |
| Apple M1                     | Linux | 0.50    | 1.2     | 0.60   | 0.58        | 0.62              |
| Apple M3                     | macOS | 0.26    | 0.28    | 0.30   | 0.37        | 0.30              |
| Ampere Altra (Neoverse N1)   | Linux | 2.6     | 1.5     | 1.9    | 1.8         | 2.3               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.60    | 0.51    | 0.55   | 0.53        |                   |

### RSA-2048 verification

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 19.1    | 56      | 27     | 92          | 28                |
| i7-5775R                     | Linux | 21      | 58      | 30     | 96          | 33                |
| i7-8565U                     | Linux | 25      | 53      | 27     | 71          | 27                |
| i7-13700H                    | Linux | 16.6    | 35      | 15.9   | 46          | 16.4              |
| Xeon-6254                    | Linux | 18.0    | 50      | 25     | 82          |                   |
| Xeon-9460                    | Linux | 17.2    | 38      | 22     | 53          |                   |
| Cortex A53 (R.Pi3)           | Linux | 752     | 1316    | 685    | 1920        | 692               |
| Cortex A72 (R.Pi4)           | Linux | 140     | 202     | 131    | 294         | 135               |
| Apple M1                     | macOS | 14.4    | 33      | 17.9   | 50          | 20.0              |
| Apple M1                     | Linux | 15.3    | 37      | 19.5   | 51          | 22                |
| Apple M3                     | macOS | 10.7    | 144     | 16.1   | 39          | 17.9              |
| Ampere Altra (Neoverse N1)   | Linux | 79      | 115     | 75     | 155         | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 25      | 60      | 30     | 73          | 31                |
| Nvidia Grace (Neoverse V2)   | Linux | 18.9    | 37      | 20     | 48          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 281     | 683     | 156    | 385         | 164               |
| RISC-V (Qemu on i7-8565U)    | Linux | 434     | 965     | 259    | 939         | 274               |
| s390x (Qemu on i7-8565U)     | Linux | 559     | 898     | 997    | 2492        | 647               |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 5.4     | 16.2    | 7.7    | 26          | 8.2               |
| i7-5775R                     | Linux | 4.1     | 11.2    | 5.8    | 18.4        | 6.5               |
| i7-8565U                     | Linux | 5.1     | 10.6    | 5.4    | 14.2        | 5.5               |
| i7-13700H                    | Linux | 5.8     | 12.4    | 5.6    | 16.4        | 5.8               |
| Xeon-6254                    | Linux | 2.6     | 7.3     | 3.7    | 12.0        |                   |
| Xeon-9460                    | Linux | 3.3     | 7.3     | 4.3    | 10.2        |                   |
| Cortex A53 (R.Pi3)           | Linux | 10.0    | 17.5    | 9.1    | 25          | 9.2               |
| Cortex A72 (R.Pi4)           | Linux | 7.8     | 11.3    | 7.3    | 16.4        | 7.6               |
| Apple M1                     | macOS | 5.0     | 11.6    | 6.2    | 17.5        | 6.9               |
| Apple M1                     | Linux | 2.9     | 7.0     | 3.7    | 9.7         | 4.2               |
| Apple M3                     | macOS | 4.4     | 59      | 6.6    | 16.4        | 7.3               |
| Ampere Altra (Neoverse N1)   | Linux | 7.7     | 11.2    | 7.4    | 15.2        | 7.6               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.0     | 7.2     | 3.6    | 8.7         | 3.8               |
| Nvidia Grace (Neoverse V2)   | Linux | 2.7     | 5.4     | 3.0    | 7.1         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 14.8    | 35      | 8.2    | 20          | 8.6               |
| RISC-V (Qemu on i7-8565U)    | Linux | 13.8    | 30      | 8.2    | 29          | 8.7               |
| s390x (Qemu on i7-8565U)     | Linux | 13.1    | 21      | 23     | 58          | 15.2              |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.51    | 0.47    | 0.56   | 0.54        |                   |
| Cortex A72 (R.Pi4)           | Linux | 8.1     | 4.8     | 6.3    | 5.8         | 6.2               |
| Apple M1                     | macOS | 0.46    | 0.45    | 0.48   | 0.56        | 0.51              |
| Apple M1                     | Linux | 0.49    | 0.50    | 0.52   | 0.57        | 0.58              |
| Apple M3                     | macOS | 0.28    | 1.5     | 0.34   | 0.35        | 0.37              |
| Ampere Altra (Neoverse N1)   | Linux | 2.7     | 1.6     | 2.2    | 1.8         | 2.1               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.59    | 0.49    | 0.54   | 0.53        |                   |

### RSA-2048 signature/verification ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 28      | 44      | 60     | 25          | 28                |
| i7-5775R                     | Linux | 29      | 95      | 69     | 24          | 29                |
| i7-8565U                     | Linux | 21      | 99      | 67     | 27          | 30                |
| i7-13700H                    | Linux | 30      | 49      | 61     | 28          | 27                |
| Xeon-6254                    | Linux | 29      | 49      | 61     | 24          |                   |
| Xeon-9460                    | Linux | 17.3    | 45      | 67     | 26          |                   |
| Cortex A53 (R.Pi3)           | Linux | 35      | 45      | 52     | 26          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 35      | 81      | 50     | 26          | 32                |
| Apple M1                     | macOS | 38      | 42      | 57     | 27          | 29                |
| Apple M1                     | Linux | 39      | 101     | 63     | 28          | 32                |
| Apple M3                     | macOS | 36      | 7.6     | 48     | 29          | 24                |
| Ampere Altra (Neoverse N1)   | Linux | 36      | 39      | 47     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 38      | 41      | 55     | 27          | 30                |
| Nvidia Grace (Neoverse V2)   | Linux | 38      | 44      | 56     | 27          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 40      | 48      | 69     | 27          | 34                |
| RISC-V (Qemu on i7-8565U)    | Linux | 42      | 48      | 64     | 25          | 35                |
| s390x (Qemu on i7-8565U)     | Linux | 38      | 51      | 65     | 25          | 30                |

### RSA-2048 signature with rekeying

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 553     | 2544    | 1652   | 2362        | 804               |
| i7-5775R                     | Linux | 597     | 5892    | 2033   | 2329        | 857               |
| i7-8565U                     | Linux | 586     | 4923    | 1848   | 1965        | 851               |
| i7-13700H                    | Linux | 508     | 1503    | 974    | 1301        | 460               |
| Xeon-6254                    | Linux | 534     | 2478    | 1550   | 1999        |                   |
| Xeon-9460                    | Linux | 302     | 1771    | 1539   | 1441        |                   |
| Cortex A53 (R.Pi3)           | Linux | 26844   | 59451   | 35753  | 50104       | 23279             |
| Cortex A72 (R.Pi4)           | Linux | 5034    | 16433   | 6626   | 7739        | 4344              |
| Apple M1                     | macOS | 551     | 1432    | 1032   | 1404        | 594               |
| Apple M1                     | Linux | 599     | 3785    | 1253   | 1501        | 742               |
| Apple M3                     | macOS | 399     | 1102    | 774    | 1175        | 435               |
| Ampere Altra (Neoverse N1)   | Linux | 2863    | 4523    | 3615   | 4145        | 2568              |
| AWS Graviton 3 (Neoverse V1) | Linux | 969     | 2541    | 1674   | 2043        | 951               |
| Nvidia Grace (Neoverse V2)   | Linux | 736     | 1681    | 1180   | 1362        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 11416   | 33356   | 10897  | 10767       | 5668              |
| RISC-V (Qemu on i7-8565U)    | Linux | 17846   | 46877   | 17130  | 25144       | 9521              |
| s390x (Qemu on i7-8565U)     | Linux | 23340   | 37137   | 57571  | 59882       | 20397             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 157     | 723     | 469    | 671         | 228               |
| i7-5775R                     | Linux | 113     | 1122    | 387    | 443         | 163               |
| i7-8565U                     | Linux | 115     | 973     | 365    | 388         | 168               |
| i7-13700H                    | Linux | 178     | 528     | 342    | 457         | 161               |
| Xeon-6254                    | Linux | 78      | 362     | 226    | 292         |                   |
| Xeon-9460                    | Linux | 57      | 336     | 292    | 273         |                   |
| Cortex A53 (R.Pi3)           | Linux | 357     | 791     | 475    | 666         | 309               |
| Cortex A72 (R.Pi4)           | Linux | 280     | 916     | 369    | 431         | 242               |
| Apple M1                     | macOS | 190     | 495     | 357    | 485         | 205               |
| Apple M1                     | Linux | 112     | 713     | 236    | 282         | 139               |
| Apple M3                     | macOS | 164     | 453     | 318    | 483         | 179               |
| Ampere Altra (Neoverse N1)   | Linux | 279     | 441     | 353    | 404         | 250               |
| AWS Graviton 3 (Neoverse V1) | Linux | 115     | 302     | 199    | 243         | 113               |
| Nvidia Grace (Neoverse V2)   | Linux | 106     | 243     | 171    | 197         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 598     | 1749    | 571    | 564         | 297               |
| RISC-V (Qemu on i7-8565U)    | Linux | 566     | 1488    | 544    | 798         | 302               |
| s390x (Qemu on i7-8565U)     | Linux | 547     | 871     | 1350   | 1404        | 478               |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.23    | 0.52    | 0.68   | 0.52        |                   |
| Cortex A72 (R.Pi4)           | Linux | 7.5     | 9.3     | 5.7    | 5.5         | 6.6               |
| Apple M1                     | macOS | 0.46    | 0.46    | 0.50   | 0.56        | 0.51              |
| Apple M1                     | Linux | 0.50    | 1.2     | 0.61   | 0.60        | 0.63              |
| Apple M3                     | macOS | 0.27    | 0.28    | 0.30   | 0.37        | 0.30              |
| Ampere Altra (Neoverse N1)   | Linux | 2.6     | 1.5     | 1.9    | 1.8         | 2.3               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.60    | 0.52    | 0.56   | 0.53        |                   |

### RSA-2048 verification with rekeying

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 23      | 56      | 27     | 91          | 28                |
| i7-5775R                     | Linux | 25      | 70      | 29     | 86          | 29                |
| i7-8565U                     | Linux | 41      | 53      | 27     | 73          | 27                |
| i7-13700H                    | Linux | 19.0    | 30      | 16.0   | 46          | 16.2              |
| Xeon-6254                    | Linux | 18.0    | 49      | 25     | 82          |                   |
| Xeon-9460                    | Linux | 21      | 38      | 22     | 53          |                   |
| Cortex A53 (R.Pi3)           | Linux | 814     | 1316    | 685    | 1919        | 693               |
| Cortex A72 (R.Pi4)           | Linux | 159     | 203     | 131    | 294         | 135               |
| Apple M1                     | macOS | 16.1    | 33      | 17.9   | 50          | 20.0              |
| Apple M1                     | Linux | 17.7    | 37      | 19.5   | 51          | 22                |
| Apple M3                     | macOS | 12.4    | 144     | 16.0   | 39          | 17.9              |
| Ampere Altra (Neoverse N1)   | Linux | 83      | 115     | 75     | 154         | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 29      | 60      | 30     | 73          | 31                |
| Nvidia Grace (Neoverse V2)   | Linux | 22      | 37      | 20     | 49          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 349     | 683     | 157    | 385         | 164               |
| RISC-V (Qemu on i7-8565U)    | Linux | 593     | 960     | 256    | 967         | 272               |
| s390x (Qemu on i7-8565U)     | Linux | 711     | 751     | 1218   | 2278        | 668               |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 6.6     | 16.2    | 7.7    | 26          | 8.2               |
| i7-5775R                     | Linux | 4.9     | 13.4    | 5.6    | 16.5        | 5.7               |
| i7-8565U                     | Linux | 8.3     | 10.6    | 5.5    | 14.6        | 5.5               |
| i7-13700H                    | Linux | 6.7     | 10.8    | 5.6    | 16.3        | 5.7               |
| Xeon-6254                    | Linux | 2.6     | 7.3     | 3.7    | 12.0        |                   |
| Xeon-9460                    | Linux | 4.0     | 7.3     | 4.3    | 10.2        |                   |
| Cortex A53 (R.Pi3)           | Linux | 10.8    | 17.5    | 9.1    | 25          | 9.2               |
| Cortex A72 (R.Pi4)           | Linux | 8.9     | 11.3    | 7.3    | 16.4        | 7.6               |
| Apple M1                     | macOS | 5.6     | 11.7    | 6.2    | 17.5        | 6.9               |
| Apple M1                     | Linux | 3.3     | 7.0     | 3.7    | 9.7         | 4.3               |
| Apple M3                     | macOS | 5.1     | 59      | 6.6    | 16.4        | 7.4               |
| Ampere Altra (Neoverse N1)   | Linux | 8.2     | 11.2    | 7.4    | 15.1        | 7.6               |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.5     | 7.2     | 3.6    | 8.7         | 3.8               |
| Nvidia Grace (Neoverse V2)   | Linux | 3.3     | 5.4     | 3.0    | 7.1         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 18.4    | 35      | 8.2    | 20          | 8.6               |
| RISC-V (Qemu on i7-8565U)    | Linux | 18.8    | 30      | 8.1    | 30          | 8.7               |
| s390x (Qemu on i7-8565U)     | Linux | 16.7    | 17.6    | 28     | 53          | 15.7              |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.54    | 0.47    | 0.56   | 0.54        |                   |
| Cortex A72 (R.Pi4)           | Linux | 7.9     | 4.8     | 6.3    | 5.8         | 6.2               |
| Apple M1                     | macOS | 0.45    | 0.45    | 0.48   | 0.56        | 0.51              |
| Apple M1                     | Linux | 0.49    | 0.50    | 0.52   | 0.57        | 0.58              |
| Apple M3                     | macOS | 0.27    | 1.5     | 0.34   | 0.35        | 0.37              |
| Ampere Altra (Neoverse N1)   | Linux | 2.5     | 1.6     | 2.2    | 1.8         | 2.1               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.60    | 0.48    | 0.53   | 0.53        |                   |

### RSA-2048 signature/verification ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 23      | 44      | 60     | 25          | 28                |
| i7-5775R                     | Linux | 23      | 83      | 69     | 26          | 28                |
| i7-8565U                     | Linux | 14.0    | 92      | 66     | 26          | 30                |
| i7-13700H                    | Linux | 26      | 49      | 61     | 28          | 28                |
| Xeon-6254                    | Linux | 29      | 49      | 60     | 24          |                   |
| Xeon-9460                    | Linux | 14.3    | 45      | 67     | 26          |                   |
| Cortex A53 (R.Pi3)           | Linux | 32      | 45      | 52     | 26          | 33                |
| Cortex A72 (R.Pi4)           | Linux | 31      | 80      | 50     | 26          | 32                |
| Apple M1                     | macOS | 34      | 42      | 57     | 27          | 29                |
| Apple M1                     | Linux | 33      | 101     | 64     | 29          | 32                |
| Apple M3                     | macOS | 32      | 7.6     | 48     | 29          | 24                |
| Ampere Altra (Neoverse N1)   | Linux | 34      | 39      | 47     | 26          | 32                |
| AWS Graviton 3 (Neoverse V1) | Linux | 33      | 41      | 55     | 27          | 30                |
| Nvidia Grace (Neoverse V2)   | Linux | 32      | 45      | 57     | 27          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 32      | 48      | 69     | 27          | 34                |
| RISC-V (Qemu on i7-8565U)    | Linux | 30      | 48      | 66     | 25          | 34                |
| s390x (Qemu on i7-8565U)     | Linux | 32      | 49      | 47     | 26          | 30                |

### RSA-2048 parse public key

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 60      | 1.1     | 4.1    | 16.0        | 7.2               |
| i7-5775R                     | Linux | 149     | 0.86    | 3.7    | 15.4        | 6.8               |
| i7-8565U                     | Linux | 192     | 0.56    | 2.8    | 12.3        | 5.9               |
| i7-13700H                    | Linux | 72      | 0.45    | 2.0    | 6.2         | 3.0               |
| Xeon-6254                    | Linux | 1.2     | 0.62    | 2.8    | 14.2        |                   |
| Xeon-9460                    | Linux | 87      | 0.56    | 1.8    | 8.6         |                   |
| Cortex A53 (R.Pi3)           | Linux | 1504    | 8.1     | 34     | 144         | 43                |
| Cortex A72 (R.Pi4)           | Linux | 435     | 2.0     | 9.9    | 28          | 13.4              |
| Apple M1                     | macOS | 28      | 0.47    | 1.9    | 9.6         | 6.0               |
| Apple M1                     | Linux | 80      | 0.58    | 1.9    | 10.7        | 7.0               |
| Apple M3                     | macOS | 5.6     | 0.33    | 1.6    | 7.4         | 4.6               |
| Ampere Altra (Neoverse N1)   | Linux | 393     | 0.76    | 3.2    | 12.0        | 5.9               |
| AWS Graviton 3 (Neoverse V1) | Linux | 378     | 0.66    | 2.6    | 9.6         | 4.8               |
| Nvidia Grace (Neoverse V2)   | Linux | 89      | 0.37    | 1.7    | 6.4         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 2078    | 10.0    | 39     | 57          | 20                |
| RISC-V (Qemu on i7-8565U)    | Linux | 2787    | 16.5    | 66     | 149         | 31                |
| s390x (Qemu on i7-8565U)     | Linux | 3222    | 0.000   | 0.000  | 140         | 0.000             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 17.3    | 0.30    | 1.2    | 4.6         | 2.0               |
| i7-5775R                     | Linux | 28      | 0.16    | 0.71   | 2.9         | 1.3               |
| i7-8565U                     | Linux | 38      | 0.11    | 0.55   | 2.4         | 1.2               |
| i7-13700H                    | Linux | 25      | 0.16    | 0.72   | 2.2         | 1.1               |
| Xeon-6254                    | Linux | 0.18    | 0.091   | 0.40   | 2.1         |                   |
| Xeon-9460                    | Linux | 16.7    | 0.11    | 0.34   | 1.6         |                   |
| Cortex A53 (R.Pi3)           | Linux | 20      | 0.11    | 0.47   | 1.9         | 0.58              |
| Cortex A72 (R.Pi4)           | Linux | 24      | 0.11    | 0.55   | 1.6         | 0.75              |
| Apple M1                     | macOS | 10.0    | 0.16    | 0.67   | 3.3         | 2.1               |
| Apple M1                     | Linux | 15.1    | 0.11    | 0.37   | 2.0         | 1.3               |
| Apple M3                     | macOS | 2.3     | 0.14    | 0.68   | 3.0         | 1.9               |
| Ampere Altra (Neoverse N1)   | Linux | 38      | 0.075   | 0.31   | 1.2         | 0.58              |
| AWS Graviton 3 (Neoverse V1) | Linux | 45      | 0.079   | 0.31   | 1.1         | 0.57              |
| Nvidia Grace (Neoverse V2)   | Linux | 12.9    | 0.054   | 0.25   | 0.93        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 109     | 0.52    | 2.1    | 3.0         | 1.1               |
| RISC-V (Qemu on i7-8565U)    | Linux | 88      | 0.52    | 2.1    | 4.7         | 1.0               |
| s390x (Qemu on i7-8565U)     | Linux | 75      | 0.000   | 0.000  | 3.3         | 0.000             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.17    | 0.62    | 0.51   | 0.67        |                   |
| Cortex A72 (R.Pi4)           | Linux | 1.7     | 4.3     | 5.5    | 4.4         | 4.1               |
| Apple M1                     | macOS | 0.062   | 0.58    | 0.60   | 0.81        | 1.0               |
| Apple M1                     | Linux | 0.17    | 0.71    | 0.60   | 0.91        | 1.2               |
| Apple M3                     | macOS | 0.010   | 0.33    | 0.41   | 0.50        | 0.63              |
| Ampere Altra (Neoverse N1)   | Linux | 0.90    | 0.99    | 1.0    | 1.1         | 1.1               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.19    | 0.44    | 0.52   | 0.53        |                   |

### RSA-2048 parse private key

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 65      | 39      | 35     | 34          | 5.1               |
| i7-5775R                     | Linux | 151     | 36      | 41     | 34          | 4.9               |
| i7-8565U                     | Linux | 198     | 27      | 30     | 28          | 5.5               |
| i7-13700H                    | Linux | 74      | 18.2    | 26     | 13.2        | 2.1               |
| Xeon-6254                    | Linux | 3.7     | 30      | 24     | 34          |                   |
| Xeon-9460                    | Linux | 90      | 21      | 19.9   | 18.5        |                   |
| Cortex A53 (R.Pi3)           | Linux | 1562    | 662     | 317    | 381         | 39                |
| Cortex A72 (R.Pi4)           | Linux | 445     | 97      | 87     | 67          | 11.3              |
| Apple M1                     | macOS | 31      | 22      | 17.8   | 15.0        | 2.6               |
| Apple M1                     | Linux | 83      | 21      | 20     | 16.9        | 3.0               |
| Apple M3                     | macOS | 8.5     | 18.0    | 15.3   | 11.6        | 2.2               |
| Ampere Altra (Neoverse N1)   | Linux | 400     | 35      | 32     | 26          | 4.5               |
| AWS Graviton 3 (Neoverse V1) | Linux | 383     | 24      | 23     | 20          | 3.4               |
| Nvidia Grace (Neoverse V2)   | Linux | 90      | 18.0    | 17.9   | 13.6        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 2061    | 421     | 307    | 170         | 26                |
| RISC-V (Qemu on i7-8565U)    | Linux | 2863    | 651     | 549    | 418         | 37                |
| s390x (Qemu on i7-8565U)     | Linux | 3194    | 926     | 1417   | 336         | 0.000             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 18.7    | 11.2    | 10.2   | 9.8         | 1.4               |
| i7-5775R                     | Linux | 28      | 7.0     | 7.9    | 6.6         | 0.93              |
| i7-8565U                     | Linux | 39      | 5.4     | 6.0    | 5.6         | 1.1               |
| i7-13700H                    | Linux | 26      | 6.4     | 9.4    | 4.6         | 0.74              |
| Xeon-6254                    | Linux | 0.54    | 4.5     | 3.6    | 5.0         |                   |
| Xeon-9460                    | Linux | 17.2    | 4.0     | 3.8    | 3.5         |                   |
| Cortex A53 (R.Pi3)           | Linux | 20      | 8.8     | 4.2    | 5.1         | 0.53              |
| Cortex A72 (R.Pi4)           | Linux | 24      | 5.5     | 4.9    | 3.7         | 0.63              |
| Apple M1                     | macOS | 11.0    | 7.9     | 6.1    | 5.2         | 0.89              |
| Apple M1                     | Linux | 15.8    | 4.0     | 3.8    | 3.2         | 0.56              |
| Apple M3                     | macOS | 3.5     | 7.4     | 6.3    | 4.8         | 0.89              |
| Ampere Altra (Neoverse N1)   | Linux | 39      | 3.5     | 3.1    | 2.6         | 0.44              |
| AWS Graviton 3 (Neoverse V1) | Linux | 45      | 3.0     | 2.8    | 2.4         | 0.40              |
| Nvidia Grace (Neoverse V2)   | Linux | 13.1    | 2.6     | 2.6    | 2.0         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 108     | 22      | 16.1   | 9.0         | 1.4               |
| RISC-V (Qemu on i7-8565U)    | Linux | 90      | 20      | 17.5   | 13.3        | 1.2               |
| s390x (Qemu on i7-8565U)     | Linux | 74      | 21      | 33     | 7.9         | 0.000             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.18    | 0.63    | 0.63   | 0.67        |                   |
| Cortex A72 (R.Pi4)           | Linux | 1.7     | 5.7     | 5.4    | 4.7         | 4.8               |
| Apple M1                     | macOS | 0.067   | 0.74    | 0.62   | 0.59        | 0.61              |
| Apple M1                     | Linux | 0.18    | 0.69    | 0.71   | 0.67        | 0.72              |
| Apple M3                     | macOS | 0.014   | 0.47    | 0.43   | 0.37        | 0.42              |
| Ampere Altra (Neoverse N1)   | Linux | 0.91    | 1.3     | 1.2    | 1.1         | 1.1               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.19    | 0.57    | 0.60   | 0.52        |                   |

## RSA-4096

### RSA-4096 encryption

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 62      | 198     | 82     | 310         | 90                |
| i7-5775R                     | Linux | 69      | 214     | 91     | 276         | 98                |
| i7-8565U                     | Linux | 129     | 185     | 84     | 245         | 92                |
| i7-13700H                    | Linux | 60      | 130     | 54     | 158         | 57                |
| Xeon-6254                    | Linux | 61      | 179     | 80     | 276         |                   |
| Xeon-9460                    | Linux | 64      | 159     | 80     | 174         |                   |
| Cortex A53 (R.Pi3)           | Linux | 2856    | 4936    | 2245   | 6881        | 2296              |
| Cortex A72 (R.Pi4)           | Linux | 480     | 738     | 393    | 1015        | 412               |
| Apple M1                     | macOS | 53      | 126     | 60     | 179         | 69                |
| Apple M1                     | Linux | 58      | 143     | 79     | 167         | 79                |
| Apple M3                     | macOS | 40      | 583     | 45     | 134         | 52                |
| Ampere Altra (Neoverse N1)   | Linux | 253     | 433     | 220    | 563         | 227               |
| AWS Graviton 3 (Neoverse V1) | Linux | 94      | 223     | 94     | 238         | 101               |
| Nvidia Grace (Neoverse V2)   | Linux | 71      | 138     | 65     | 160         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 847     | 2445    | 481    | 1062        | 531               |
| RISC-V (Qemu on i7-8565U)    | Linux | 1304    | 3373    | 785    | 3477        | 863               |
| s390x (Qemu on i7-8565U)     | Linux | 1425    | 4839    | 3546   | 7484        | 2226              |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 17.9    | 56      | 23     | 88          | 25                |
| i7-5775R                     | Linux | 13.3    | 40      | 17.5   | 52          | 18.9              |
| i7-8565U                     | Linux | 25      | 36      | 16.7   | 48          | 18.3              |
| i7-13700H                    | Linux | 21      | 45      | 19.1   | 55          | 20                |
| Xeon-6254                    | Linux | 8.9     | 26      | 11.7   | 40          |                   |
| Xeon-9460                    | Linux | 12.3    | 30      | 15.2   | 33          |                   |
| Cortex A53 (R.Pi3)           | Linux | 38      | 65      | 29     | 91          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 26      | 41      | 21     | 56          | 23                |
| Apple M1                     | macOS | 18.5    | 43      | 20     | 62          | 23                |
| Apple M1                     | Linux | 11.0    | 27      | 14.9   | 31          | 14.9              |
| Apple M3                     | macOS | 16.6    | 239     | 18.7   | 55          | 21                |
| Ampere Altra (Neoverse N1)   | Linux | 24      | 42      | 21     | 55          | 22                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.3    | 26      | 11.3   | 28          | 12.1              |
| Nvidia Grace (Neoverse V2)   | Linux | 10.4    | 20      | 9.5    | 23          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 44      | 128     | 25     | 55          | 27                |
| RISC-V (Qemu on i7-8565U)    | Linux | 41      | 107     | 24     | 110         | 27                |
| s390x (Qemu on i7-8565U)     | Linux | 33      | 113     | 83     | 175         | 52                |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.51    | 0.53    | 0.63   | 0.55        |                   |
| Cortex A72 (R.Pi4)           | Linux | 7.3     | 4.8     | 6.0    | 6.2         | 5.9               |
| Apple M1                     | macOS | 0.46    | 0.46    | 0.52   | 0.61        | 0.56              |
| Apple M1                     | Linux | 0.50    | 0.52    | 0.68   | 0.57        | 0.64              |
| Apple M3                     | macOS | 0.28    | 1.7     | 0.31   | 0.37        | 0.34              |
| Ampere Altra (Neoverse N1)   | Linux | 2.3     | 1.7     | 2.0    | 2.1         | 1.9               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.59    | 0.49    | 0.55   | 0.53        |                   |

### RSA-4096 decryption

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 3640    | 14755   | 9000   | 12502       | 5556              |
| i7-5775R                     | Linux | 4473    | 23882   | 10585  | 13504       | 6024              |
| i7-8565U                     | Linux | 8661    | 20292   | 9796   | 10669       | 5483              |
| i7-13700H                    | Linux | 3647    | 9209    | 5601   | 7107        | 3208              |
| Xeon-6254                    | Linux | 7260    | 13781   | 9852   | 10608       |                   |
| Xeon-9460                    | Linux | 7407    | 11113   | 9232   | 7468        |                   |
| Cortex A53 (R.Pi3)           | Linux | 186535  | 378405  | 214376 | 312961      | 158954            |
| Cortex A72 (R.Pi4)           | Linux | 34813   | 72849   | 49520  | 45186       | 29517             |
| Apple M1                     | macOS | 3497    | 8798    | 6179   | 7253        | 3732              |
| Apple M1                     | Linux | 3878    | 14981   | 7477   | 7644        | 4860              |
| Apple M3                     | macOS | 2603    | 7021    | 5023   | 5638        | 2910              |
| Ampere Altra (Neoverse N1)   | Linux | 20248   | 28560   | 28455  | 25751       | 17497             |
| AWS Graviton 3 (Neoverse V1) | Linux | 6295    | 15070   | 11590  | 10845       | 6575              |
| Nvidia Grace (Neoverse V2)   | Linux | 7935    | 10243   | 8444   | 7296        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 61093   | 192521  | 59195  | 48199       | 33403             |
| RISC-V (Qemu on i7-8565U)    | Linux | 96142   | 267158  | 82392  | 140225      | 53760             |
| s390x (Qemu on i7-8565U)     | Linux | 96877   | 269989  | 345333 | 369270      | 142696            |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1034    | 4193    | 2557   | 3553        | 1579              |
| i7-5775R                     | Linux | 852     | 4548    | 2015   | 2571        | 1147              |
| i7-8565U                     | Linux | 1712    | 4011    | 1936   | 2109        | 1084              |
| i7-13700H                    | Linux | 1281    | 3235    | 1968   | 2497        | 1127              |
| Xeon-6254                    | Linux | 1060    | 2013    | 1439   | 1549        |                   |
| Xeon-9460                    | Linux | 1408    | 2112    | 1754   | 1419        |                   |
| Cortex A53 (R.Pi3)           | Linux | 2483    | 5037    | 2853   | 4166        | 2115              |
| Cortex A72 (R.Pi4)           | Linux | 1941    | 4063    | 2762   | 2520        | 1646              |
| Apple M1                     | macOS | 1209    | 3041    | 2136   | 2507        | 1290              |
| Apple M1                     | Linux | 730     | 2822    | 1408   | 1440        | 915               |
| Apple M3                     | macOS | 1071    | 2888    | 2066   | 2319        | 1197              |
| Ampere Altra (Neoverse N1)   | Linux | 1977    | 2788    | 2778   | 2514        | 1708              |
| AWS Graviton 3 (Neoverse V1) | Linux | 750     | 1796    | 1381   | 1292        | 783               |
| Nvidia Grace (Neoverse V2)   | Linux | 1150    | 1485    | 1224   | 1058        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 3203    | 10096   | 3104   | 2527        | 1751              |
| RISC-V (Qemu on i7-8565U)    | Linux | 3053    | 8484    | 2616   | 4453        | 1707              |
| s390x (Qemu on i7-8565U)     | Linux | 2272    | 6334    | 8102   | 8663        | 3347              |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.87    | 0.55    | 0.59   | 0.51        |                   |
| Cortex A72 (R.Pi4)           | Linux | 8.0     | 7.0     | 6.2    | 6.0         | 6.5               |
| Apple M1                     | macOS | 0.45    | 0.47    | 0.43   | 0.54        | 0.46              |
| Apple M1                     | Linux | 0.50    | 0.81    | 0.52   | 0.57        | 0.60              |
| Apple M3                     | macOS | 0.27    | 0.30    | 0.28   | 0.34        | 0.29              |
| Ampere Altra (Neoverse N1)   | Linux | 2.8     | 1.6     | 2.1    | 2.1         | 2.3               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.99    | 0.54    | 0.57   | 0.53        |                   |

### RSA-4096 decryption/encryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 57      | 74      | 109    | 40          | 61                |
| i7-5775R                     | Linux | 64      | 111     | 115    | 48          | 60                |
| i7-8565U                     | Linux | 67      | 109     | 116    | 43          | 59                |
| i7-13700H                    | Linux | 60      | 70      | 102    | 44          | 55                |
| Xeon-6254                    | Linux | 118     | 76      | 122    | 38          |                   |
| Xeon-9460                    | Linux | 114     | 69      | 115    | 42          |                   |
| Cortex A53 (R.Pi3)           | Linux | 65      | 76      | 95     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 72      | 98      | 125    | 44          | 71                |
| Apple M1                     | macOS | 65      | 69      | 102    | 40          | 53                |
| Apple M1                     | Linux | 66      | 104     | 94     | 45          | 61                |
| Apple M3                     | macOS | 64      | 12.0    | 110    | 41          | 55                |
| Ampere Altra (Neoverse N1)   | Linux | 79      | 65      | 128    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 66      | 67      | 122    | 45          | 64                |
| Nvidia Grace (Neoverse V2)   | Linux | 111     | 73      | 128    | 45          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 72      | 78      | 122    | 45          | 62                |
| RISC-V (Qemu on i7-8565U)    | Linux | 73      | 79      | 104    | 40          | 62                |
| s390x (Qemu on i7-8565U)     | Linux | 67      | 55      | 97     | 49          | 64                |

### RSA-4096 encryption with rekeying

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 66      | 197     | 81     | 310         | 90                |
| i7-5775R                     | Linux | 77      | 217     | 90     | 313         | 99                |
| i7-8565U                     | Linux | 188     | 181     | 84     | 242         | 90                |
| i7-13700H                    | Linux | 63      | 130     | 54     | 158         | 57                |
| Xeon-6254                    | Linux | 60      | 178     | 80     | 276         |                   |
| Xeon-9460                    | Linux | 67      | 159     | 79     | 174         |                   |
| Cortex A53 (R.Pi3)           | Linux | 2908    | 4934    | 2245   | 6916        | 2296              |
| Cortex A72 (R.Pi4)           | Linux | 497     | 738     | 392    | 1015        | 412               |
| Apple M1                     | macOS | 54      | 126     | 60     | 179         | 69                |
| Apple M1                     | Linux | 60      | 143     | 81     | 174         | 82                |
| Apple M3                     | macOS | 41      | 546     | 45     | 134         | 52                |
| Ampere Altra (Neoverse N1)   | Linux | 257     | 433     | 219    | 563         | 227               |
| AWS Graviton 3 (Neoverse V1) | Linux | 98      | 223     | 94     | 237         | 101               |
| Nvidia Grace (Neoverse V2)   | Linux | 73      | 136     | 65     | 161         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 916     | 2424    | 481    | 1058        | 532               |
| RISC-V (Qemu on i7-8565U)    | Linux | 1418    | 3361    | 783    | 4922        | 858               |
| s390x (Qemu on i7-8565U)     | Linux | 1792    | 2872    | 3365   | 7866        | 2225              |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 18.9    | 56      | 23     | 88          | 25                |
| i7-5775R                     | Linux | 14.7    | 41      | 17.3   | 59          | 19.0              |
| i7-8565U                     | Linux | 37      | 35      | 16.7   | 47          | 18.0              |
| i7-13700H                    | Linux | 22      | 45      | 19.2   | 55          | 20                |
| Xeon-6254                    | Linux | 8.9     | 26      | 11.7   | 40          |                   |
| Xeon-9460                    | Linux | 12.8    | 30      | 15.2   | 33          |                   |
| Cortex A53 (R.Pi3)           | Linux | 38      | 65      | 29     | 92          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 27      | 41      | 21     | 56          | 23                |
| Apple M1                     | macOS | 18.9    | 43      | 20     | 61          | 23                |
| Apple M1                     | Linux | 11.5    | 27      | 15.3   | 32          | 15.4              |
| Apple M3                     | macOS | 17.2    | 224     | 18.6   | 55          | 21                |
| Ampere Altra (Neoverse N1)   | Linux | 25      | 42      | 21     | 55          | 22                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.7    | 26      | 11.3   | 28          | 12.1              |
| Nvidia Grace (Neoverse V2)   | Linux | 10.6    | 19.9    | 9.5    | 23          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 48      | 127     | 25     | 55          | 27                |
| RISC-V (Qemu on i7-8565U)    | Linux | 45      | 106     | 24     | 156         | 27                |
| s390x (Qemu on i7-8565U)     | Linux | 42      | 67      | 78     | 184         | 52                |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.51    | 0.53    | 0.63   | 0.55        |                   |
| Cortex A72 (R.Pi4)           | Linux | 7.3     | 4.8     | 6.0    | 6.2         | 5.9               |
| Apple M1                     | macOS | 0.45    | 0.46    | 0.52   | 0.61        | 0.56              |
| Apple M1                     | Linux | 0.50    | 0.52    | 0.70   | 0.60        | 0.66              |
| Apple M3                     | macOS | 0.28    | 1.6     | 0.31   | 0.37        | 0.34              |
| Ampere Altra (Neoverse N1)   | Linux | 2.3     | 1.7     | 2.0    | 2.1         | 1.9               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.59    | 0.48    | 0.55   | 0.54        |                   |

### RSA-4096 decryption with rekeying

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 3641    | 14754   | 8994   | 12526       | 5556              |
| i7-5775R                     | Linux | 4176    | 24461   | 10711  | 11867       | 5861              |
| i7-8565U                     | Linux | 7029    | 23341   | 9711   | 10505       | 5566              |
| i7-13700H                    | Linux | 3659    | 9280    | 5575   | 7095        | 3221              |
| Xeon-6254                    | Linux | 7243    | 13776   | 9884   | 10626       |                   |
| Xeon-9460                    | Linux | 7410    | 11110   | 9241   | 7460        |                   |
| Cortex A53 (R.Pi3)           | Linux | 187251  | 379170  | 214359 | 311307      | 158941            |
| Cortex A72 (R.Pi4)           | Linux | 34867   | 72833   | 49520  | 45177       | 29516             |
| Apple M1                     | macOS | 3499    | 8798    | 6179   | 7253        | 3732              |
| Apple M1                     | Linux | 3916    | 15090   | 7411   | 7499        | 4770              |
| Apple M3                     | macOS | 2650    | 7045    | 5024   | 5609        | 2920              |
| Ampere Altra (Neoverse N1)   | Linux | 20263   | 28559   | 28557  | 25629       | 17501             |
| AWS Graviton 3 (Neoverse V1) | Linux | 6299    | 15072   | 11591  | 10844       | 6574              |
| Nvidia Grace (Neoverse V2)   | Linux | 7925    | 10130   | 8412   | 7296        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 61343   | 192813  | 59115  | 48175       | 33408             |
| RISC-V (Qemu on i7-8565U)    | Linux | 93131   | 269093  | 82632  | 137372      | 53696             |
| s390x (Qemu on i7-8565U)     | Linux | 107973  | 241173  | 351911 | 346849      | 142998            |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1034    | 4193    | 2556   | 3559        | 1579              |
| i7-5775R                     | Linux | 795     | 4658    | 2040   | 2260        | 1116              |
| i7-8565U                     | Linux | 1389    | 4614    | 1919   | 2076        | 1100              |
| i7-13700H                    | Linux | 1285    | 3260    | 1958   | 2493        | 1131              |
| Xeon-6254                    | Linux | 1057    | 2012    | 1443   | 1552        |                   |
| Xeon-9460                    | Linux | 1408    | 2111    | 1756   | 1417        |                   |
| Cortex A53 (R.Pi3)           | Linux | 2492    | 5047    | 2853   | 4143        | 2115              |
| Cortex A72 (R.Pi4)           | Linux | 1944    | 4062    | 2762   | 2519        | 1646              |
| Apple M1                     | macOS | 1209    | 3041    | 2135   | 2507        | 1290              |
| Apple M1                     | Linux | 737     | 2843    | 1396   | 1412        | 898               |
| Apple M3                     | macOS | 1090    | 2898    | 2067   | 2307        | 1201              |
| Ampere Altra (Neoverse N1)   | Linux | 1978    | 2788    | 2788   | 2502        | 1708              |
| AWS Graviton 3 (Neoverse V1) | Linux | 750     | 1796    | 1381   | 1292        | 783               |
| Nvidia Grace (Neoverse V2)   | Linux | 1149    | 1469    | 1220   | 1058        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 3217    | 10111   | 3100   | 2526        | 1752              |
| RISC-V (Qemu on i7-8565U)    | Linux | 2957    | 8546    | 2624   | 4362        | 1705              |
| s390x (Qemu on i7-8565U)     | Linux | 2533    | 5658    | 8256   | 8137        | 3355              |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.87    | 0.55    | 0.59   | 0.51        |                   |
| Cortex A72 (R.Pi4)           | Linux | 8.0     | 7.0     | 6.2    | 6.0         | 6.5               |
| Apple M1                     | macOS | 0.45    | 0.47    | 0.43   | 0.54        | 0.46              |
| Apple M1                     | Linux | 0.51    | 0.81    | 0.52   | 0.56        | 0.59              |
| Apple M3                     | macOS | 0.27    | 0.30    | 0.28   | 0.34        | 0.29              |
| Ampere Altra (Neoverse N1)   | Linux | 2.8     | 1.6     | 2.1    | 2.0         | 2.3               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.99    | 0.53    | 0.57   | 0.53        |                   |

### RSA-4096 decryption/encryption ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 54      | 74      | 109    | 40          | 61                |
| i7-5775R                     | Linux | 54      | 112     | 117    | 37          | 58                |
| i7-8565U                     | Linux | 37      | 128     | 115    | 43          | 61                |
| i7-13700H                    | Linux | 58      | 71      | 102    | 44          | 55                |
| Xeon-6254                    | Linux | 119     | 77      | 123    | 38          |                   |
| Xeon-9460                    | Linux | 110     | 69      | 115    | 42          |                   |
| Cortex A53 (R.Pi3)           | Linux | 64      | 76      | 95     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 70      | 98      | 126    | 44          | 71                |
| Apple M1                     | macOS | 63      | 69      | 102    | 40          | 53                |
| Apple M1                     | Linux | 64      | 105     | 91     | 42          | 58                |
| Apple M3                     | macOS | 63      | 12.9    | 110    | 41          | 55                |
| Ampere Altra (Neoverse N1)   | Linux | 78      | 65      | 129    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 64      | 67      | 122    | 45          | 64                |
| Nvidia Grace (Neoverse V2)   | Linux | 108     | 73      | 127    | 45          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 66      | 79      | 122    | 45          | 62                |
| RISC-V (Qemu on i7-8565U)    | Linux | 65      | 80      | 105    | 27          | 62                |
| s390x (Qemu on i7-8565U)     | Linux | 60      | 83      | 104    | 44          | 64                |

### RSA-4096 signature

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 3636    | 14716   | 8973   | 12516       | 5534              |
| i7-5775R                     | Linux | 4464    | 23671   | 10773  | 11314       | 5775              |
| i7-8565U                     | Linux | 7816    | 27146   | 9718   | 10258       | 5632              |
| i7-13700H                    | Linux | 4037    | 8906    | 5588   | 7073        | 3216              |
| Xeon-6254                    | Linux | 3724    | 13886   | 9806   | 10610       |                   |
| Xeon-9460                    | Linux | 3864    | 11145   | 9223   | 7471        |                   |
| Cortex A53 (R.Pi3)           | Linux | 187189  | 379076  | 214369 | 311659      | 158949            |
| Cortex A72 (R.Pi4)           | Linux | 34822   | 72850   | 49495  | 45169       | 29500             |
| Apple M1                     | macOS | 3495    | 8806    | 6183   | 7250        | 3741              |
| Apple M1                     | Linux | 3929    | 15137   | 7496   | 7722        | 4709              |
| Apple M3                     | macOS | 2618    | 7040    | 5022   | 5641        | 2924              |
| Ampere Altra (Neoverse N1)   | Linux | 20251   | 28563   | 28445  | 25749       | 17499             |
| AWS Graviton 3 (Neoverse V1) | Linux | 6292    | 15072   | 11591  | 10853       | 6569              |
| Nvidia Grace (Neoverse V2)   | Linux | 4770    | 10126   | 8406   | 7284        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 61217   | 192723  | 59295  | 48211       | 33395             |
| RISC-V (Qemu on i7-8565U)    | Linux | 94018   | 267837  | 82538  | 125473      | 53400             |
| s390x (Qemu on i7-8565U)     | Linux | 114340  | 256951  | 354929 | 341691      | 143775            |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1033    | 4182    | 2550   | 3557        | 1572              |
| i7-5775R                     | Linux | 850     | 4508    | 2051   | 2154        | 1100              |
| i7-8565U                     | Linux | 1545    | 5366    | 1921   | 2027        | 1113              |
| i7-13700H                    | Linux | 1418    | 3129    | 1963   | 2485        | 1130              |
| Xeon-6254                    | Linux | 543     | 2028    | 1432   | 1549        |                   |
| Xeon-9460                    | Linux | 734     | 2118    | 1753   | 1420        |                   |
| Cortex A53 (R.Pi3)           | Linux | 2491    | 5046    | 2853   | 4148        | 2115              |
| Cortex A72 (R.Pi4)           | Linux | 1942    | 4063    | 2760   | 2519        | 1645              |
| Apple M1                     | macOS | 1208    | 3044    | 2137   | 2506        | 1293              |
| Apple M1                     | Linux | 740     | 2851    | 1412   | 1454        | 887               |
| Apple M3                     | macOS | 1077    | 2896    | 2066   | 2320        | 1203              |
| Ampere Altra (Neoverse N1)   | Linux | 1977    | 2788    | 2777   | 2514        | 1708              |
| AWS Graviton 3 (Neoverse V1) | Linux | 749     | 1796    | 1381   | 1293        | 783               |
| Nvidia Grace (Neoverse V2)   | Linux | 692     | 1468    | 1219   | 1056        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 3210    | 10107   | 3109   | 2528        | 1751              |
| RISC-V (Qemu on i7-8565U)    | Linux | 2985    | 8506    | 2621   | 3984        | 1695              |
| s390x (Qemu on i7-8565U)     | Linux | 2682    | 6028    | 8327   | 8016        | 3373              |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.46    | 0.55    | 0.59   | 0.51        |                   |
| Cortex A72 (R.Pi4)           | Linux | 8.0     | 7.0     | 6.2    | 6.0         | 6.5               |
| Apple M1                     | macOS | 0.45    | 0.47    | 0.43   | 0.54        | 0.46              |
| Apple M1                     | Linux | 0.51    | 0.82    | 0.53   | 0.58        | 0.58              |
| Apple M3                     | macOS | 0.27    | 0.30    | 0.28   | 0.34        | 0.29              |
| Ampere Altra (Neoverse N1)   | Linux | 2.8     | 1.6     | 2.1    | 2.1         | 2.3               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.60    | 0.53    | 0.57   | 0.53        |                   |

### RSA-4096 verification

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 61      | 195     | 85     | 306         | 87                |
| i7-5775R                     | Linux | 69      | 220     | 96     | 268         | 92                |
| i7-8565U                     | Linux | 132     | 195     | 87     | 245         | 90                |
| i7-13700H                    | Linux | 67      | 130     | 56     | 156         | 56                |
| Xeon-6254                    | Linux | 58      | 175     | 84     | 273         |                   |
| Xeon-9460                    | Linux | 62      | 157     | 79     | 171         |                   |
| Cortex A53 (R.Pi3)           | Linux | 2827    | 4921    | 2277   | 6868        | 2284              |
| Cortex A72 (R.Pi4)           | Linux | 464     | 733     | 399    | 1009        | 407               |
| Apple M1                     | macOS | 51      | 124     | 64     | 177         | 68                |
| Apple M1                     | Linux | 57      | 142     | 75     | 169         | 83                |
| Apple M3                     | macOS | 39      | 544     | 45     | 130         | 49                |
| Ampere Altra (Neoverse N1)   | Linux | 250     | 431     | 222    | 561         | 225               |
| AWS Graviton 3 (Neoverse V1) | Linux | 92      | 221     | 96     | 234         | 99                |
| Nvidia Grace (Neoverse V2)   | Linux | 68      | 135     | 66     | 158         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 819     | 2412    | 502    | 1049        | 519               |
| RISC-V (Qemu on i7-8565U)    | Linux | 1237    | 3368    | 823    | 2863        | 850               |
| s390x (Qemu on i7-8565U)     | Linux | 1346    | 4026    | 3646   | 7198        | 2317              |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 17.4    | 55      | 24     | 87          | 25                |
| i7-5775R                     | Linux | 13.1    | 41      | 18.4   | 51          | 17.7              |
| i7-8565U                     | Linux | 26      | 38      | 17.3   | 48          | 17.9              |
| i7-13700H                    | Linux | 23      | 45      | 19.8   | 55          | 19.7              |
| Xeon-6254                    | Linux | 8.6     | 25      | 12.3   | 39          |                   |
| Xeon-9460                    | Linux | 11.9    | 29      | 15.1   | 32          |                   |
| Cortex A53 (R.Pi3)           | Linux | 37      | 65      | 30     | 91          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 25      | 40      | 22     | 56          | 22                |
| Apple M1                     | macOS | 18.0    | 43      | 22     | 61          | 23                |
| Apple M1                     | Linux | 10.9    | 26      | 14.3   | 31          | 15.7              |
| Apple M3                     | macOS | 16.4    | 224     | 18.9   | 53          | 20                |
| Ampere Altra (Neoverse N1)   | Linux | 24      | 42      | 21     | 54          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.0    | 26      | 11.5   | 27          | 11.8              |
| Nvidia Grace (Neoverse V2)   | Linux | 10.0    | 19.6    | 9.6    | 23          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 42      | 126     | 26     | 55          | 27                |
| RISC-V (Qemu on i7-8565U)    | Linux | 39      | 106     | 26     | 90          | 27                |
| s390x (Qemu on i7-8565U)     | Linux | 31      | 94      | 85     | 168         | 54                |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.50    | 0.53    | 0.61   | 0.54        |                   |
| Cortex A72 (R.Pi4)           | Linux | 7.2     | 4.8     | 6.0    | 6.2         | 5.9               |
| Apple M1                     | macOS | 0.46    | 0.46    | 0.54   | 0.61        | 0.56              |
| Apple M1                     | Linux | 0.51    | 0.52    | 0.64   | 0.59        | 0.68              |
| Apple M3                     | macOS | 0.28    | 1.6     | 0.31   | 0.36        | 0.32              |
| Ampere Altra (Neoverse N1)   | Linux | 2.3     | 1.7     | 2.0    | 2.1         | 2.0               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.59    | 0.48    | 0.54   | 0.53        |                   |

### RSA-4096 signature/verification ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 59      | 75      | 105    | 40          | 62                |
| i7-5775R                     | Linux | 64      | 107     | 111    | 42          | 62                |
| i7-8565U                     | Linux | 58      | 139     | 111    | 41          | 62                |
| i7-13700H                    | Linux | 59      | 68      | 99     | 45          | 57                |
| Xeon-6254                    | Linux | 63      | 79      | 116    | 38          |                   |
| Xeon-9460                    | Linux | 61      | 70      | 115    | 43          |                   |
| Cortex A53 (R.Pi3)           | Linux | 66      | 77      | 94     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 74      | 99      | 124    | 44          | 72                |
| Apple M1                     | macOS | 67      | 70      | 95     | 40          | 54                |
| Apple M1                     | Linux | 67      | 106     | 99     | 45          | 56                |
| Apple M3                     | macOS | 65      | 12.9    | 109    | 43          | 59                |
| Ampere Altra (Neoverse N1)   | Linux | 80      | 66      | 127    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 67      | 67      | 119    | 46          | 66                |
| Nvidia Grace (Neoverse V2)   | Linux | 69      | 74      | 127    | 45          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 74      | 79      | 117    | 45          | 64                |
| RISC-V (Qemu on i7-8565U)    | Linux | 75      | 79      | 100    | 43          | 62                |
| s390x (Qemu on i7-8565U)     | Linux | 84      | 63      | 97     | 47          | 62                |

### RSA-4096 signature with rekeying

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 3644    | 14713   | 8977   | 12508       | 5533              |
| i7-5775R                     | Linux | 4249    | 23898   | 11284  | 11382       | 5623              |
| i7-8565U                     | Linux | 6083    | 21523   | 9845   | 10463       | 5765              |
| i7-13700H                    | Linux | 4097    | 8865    | 5614   | 7055        | 3219              |
| Xeon-6254                    | Linux | 3700    | 13923   | 9808   | 10619       |                   |
| Xeon-9460                    | Linux | 3868    | 11113   | 9224   | 7474        |                   |
| Cortex A53 (R.Pi3)           | Linux | 187283  | 378629  | 214372 | 311857      | 158939            |
| Cortex A72 (R.Pi4)           | Linux | 34884   | 72824   | 49495  | 45175       | 29507             |
| Apple M1                     | macOS | 3497    | 8802    | 6186   | 7271        | 3743              |
| Apple M1                     | Linux | 3943    | 15398   | 7429   | 7523        | 4620              |
| Apple M3                     | macOS | 2638    | 7045    | 5029   | 5632        | 2921              |
| Ampere Altra (Neoverse N1)   | Linux | 20254   | 28563   | 28549  | 25757       | 17499             |
| AWS Graviton 3 (Neoverse V1) | Linux | 6297    | 15074   | 11591  | 10845       | 6573              |
| Nvidia Grace (Neoverse V2)   | Linux | 4806    | 10127   | 8417   | 7288        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 61290   | 192573  | 59315  | 48211       | 33538             |
| RISC-V (Qemu on i7-8565U)    | Linux | 93959   | 270152  | 82190  | 121945      | 52905             |
| s390x (Qemu on i7-8565U)     | Linux | 103769  | 388794  | 361854 | 344166      | 150272            |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 1035    | 4181    | 2551   | 3554        | 1572              |
| i7-5775R                     | Linux | 809     | 4551    | 2148   | 2167        | 1070              |
| i7-8565U                     | Linux | 1202    | 4254    | 1946   | 2068        | 1139              |
| i7-13700H                    | Linux | 1439    | 3114    | 1972   | 2478        | 1131              |
| Xeon-6254                    | Linux | 540     | 2033    | 1432   | 1551        |                   |
| Xeon-9460                    | Linux | 735     | 2112    | 1753   | 1420        |                   |
| Cortex A53 (R.Pi3)           | Linux | 2493    | 5040    | 2853   | 4151        | 2115              |
| Cortex A72 (R.Pi4)           | Linux | 1945    | 4062    | 2760   | 2519        | 1645              |
| Apple M1                     | macOS | 1209    | 3042    | 2138   | 2513        | 1293              |
| Apple M1                     | Linux | 743     | 2901    | 1399   | 1417        | 870               |
| Apple M3                     | macOS | 1085    | 2898    | 2069   | 2317        | 1202              |
| Ampere Altra (Neoverse N1)   | Linux | 1977    | 2788    | 2787   | 2514        | 1708              |
| AWS Graviton 3 (Neoverse V1) | Linux | 750     | 1796    | 1381   | 1292        | 783               |
| Nvidia Grace (Neoverse V2)   | Linux | 697     | 1469    | 1220   | 1057        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 3214    | 10099   | 3110   | 2528        | 1758              |
| RISC-V (Qemu on i7-8565U)    | Linux | 2984    | 8579    | 2610   | 3872        | 1680              |
| s390x (Qemu on i7-8565U)     | Linux | 2434    | 9121    | 8489   | 8074        | 3525              |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.46    | 0.55    | 0.59   | 0.51        |                   |
| Cortex A72 (R.Pi4)           | Linux | 8.0     | 7.0     | 6.2    | 6.0         | 6.5               |
| Apple M1                     | macOS | 0.45    | 0.47    | 0.43   | 0.54        | 0.46              |
| Apple M1                     | Linux | 0.51    | 0.83    | 0.52   | 0.56        | 0.57              |
| Apple M3                     | macOS | 0.27    | 0.30    | 0.28   | 0.34        | 0.29              |
| Ampere Altra (Neoverse N1)   | Linux | 2.8     | 1.6     | 2.1    | 2.1         | 2.3               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.60    | 0.53    | 0.57   | 0.53        |                   |

### RSA-4096 verification with rekeying

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 65      | 196     | 85     | 305         | 88                |
| i7-5775R                     | Linux | 74      | 211     | 101    | 270         | 100               |
| i7-8565U                     | Linux | 112     | 184     | 88     | 243         | 90                |
| i7-13700H                    | Linux | 70      | 117     | 56     | 156         | 56                |
| Xeon-6254                    | Linux | 58      | 176     | 83     | 272         |                   |
| Xeon-9460                    | Linux | 66      | 157     | 79     | 171         |                   |
| Cortex A53 (R.Pi3)           | Linux | 2888    | 4921    | 2277   | 6866        | 2285              |
| Cortex A72 (R.Pi4)           | Linux | 484     | 733     | 399    | 1010        | 406               |
| Apple M1                     | macOS | 53      | 124     | 64     | 177         | 68                |
| Apple M1                     | Linux | 60      | 142     | 75     | 170         | 83                |
| Apple M3                     | macOS | 41      | 544     | 45     | 130         | 49                |
| Ampere Altra (Neoverse N1)   | Linux | 255     | 431     | 222    | 561         | 225               |
| AWS Graviton 3 (Neoverse V1) | Linux | 96      | 221     | 96     | 235         | 99                |
| Nvidia Grace (Neoverse V2)   | Linux | 73      | 135     | 66     | 158         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 893     | 2412    | 503    | 1050        | 521               |
| RISC-V (Qemu on i7-8565U)    | Linux | 1428    | 3371    | 836    | 2830        | 864               |
| s390x (Qemu on i7-8565U)     | Linux | 1868    | 3329    | 3508   | 7246        | 2456              |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 18.7    | 55      | 24     | 86          | 25                |
| i7-5775R                     | Linux | 14.2    | 40      | 19.3   | 51          | 19.1              |
| i7-8565U                     | Linux | 22      | 36      | 17.5   | 48          | 17.9              |
| i7-13700H                    | Linux | 24      | 41      | 19.8   | 54          | 19.8              |
| Xeon-6254                    | Linux | 8.6     | 25      | 12.2   | 39          |                   |
| Xeon-9460                    | Linux | 12.7    | 29      | 15.2   | 32          |                   |
| Cortex A53 (R.Pi3)           | Linux | 38      | 65      | 30     | 91          | 30                |
| Cortex A72 (R.Pi4)           | Linux | 27      | 40      | 22     | 56          | 22                |
| Apple M1                     | macOS | 18.6    | 43      | 22     | 61          | 23                |
| Apple M1                     | Linux | 11.4    | 26      | 14.3   | 32          | 15.7              |
| Apple M3                     | macOS | 17.2    | 224     | 18.9   | 53          | 20                |
| Ampere Altra (Neoverse N1)   | Linux | 24      | 42      | 21     | 54          | 21                |
| AWS Graviton 3 (Neoverse V1) | Linux | 11.5    | 26      | 11.5   | 28          | 11.8              |
| Nvidia Grace (Neoverse V2)   | Linux | 10.7    | 19.6    | 9.6    | 22          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 46      | 126     | 26     | 55          | 27                |
| RISC-V (Qemu on i7-8565U)    | Linux | 45      | 107     | 26     | 89          | 27                |
| s390x (Qemu on i7-8565U)     | Linux | 43      | 78      | 82     | 170         | 57                |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.51    | 0.53    | 0.61   | 0.54        |                   |
| Cortex A72 (R.Pi4)           | Linux | 7.2     | 4.8     | 6.0    | 6.2         | 5.9               |
| Apple M1                     | macOS | 0.45    | 0.46    | 0.54   | 0.61        | 0.56              |
| Apple M1                     | Linux | 0.51    | 0.52    | 0.64   | 0.59        | 0.68              |
| Apple M3                     | macOS | 0.28    | 1.6     | 0.31   | 0.36        | 0.32              |
| Ampere Altra (Neoverse N1)   | Linux | 2.3     | 1.7     | 2.0    | 2.1         | 2.0               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.60    | 0.48    | 0.54   | 0.53        |                   |

### RSA-4096 signature/verification ratio, with rekeying

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 55      | 74      | 105    | 40          | 62                |
| i7-5775R                     | Linux | 56      | 112     | 111    | 42          | 56                |
| i7-8565U                     | Linux | 54      | 116     | 111    | 42          | 63                |
| i7-13700H                    | Linux | 58      | 75      | 99     | 45          | 57                |
| Xeon-6254                    | Linux | 62      | 79      | 117    | 38          |                   |
| Xeon-9460                    | Linux | 57      | 70      | 115    | 43          |                   |
| Cortex A53 (R.Pi3)           | Linux | 64      | 76      | 94     | 45          | 69                |
| Cortex A72 (R.Pi4)           | Linux | 72      | 99      | 124    | 44          | 72                |
| Apple M1                     | macOS | 64      | 70      | 95     | 41          | 54                |
| Apple M1                     | Linux | 65      | 108     | 97     | 44          | 55                |
| Apple M3                     | macOS | 63      | 12.9    | 109    | 43          | 59                |
| Ampere Altra (Neoverse N1)   | Linux | 79      | 66      | 128    | 45          | 77                |
| AWS Graviton 3 (Neoverse V1) | Linux | 64      | 67      | 119    | 46          | 66                |
| Nvidia Grace (Neoverse V2)   | Linux | 65      | 74      | 127    | 46          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 68      | 79      | 117    | 45          | 64                |
| RISC-V (Qemu on i7-8565U)    | Linux | 65      | 80      | 98     | 43          | 61                |
| s390x (Qemu on i7-8565U)     | Linux | 55      | 116     | 103    | 47          | 61                |

### RSA-4096 parse public key

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 60      | 1.1     | 4.7    | 44          | 12.6              |
| i7-5775R                     | Linux | 173     | 0.87    | 6.8    | 44          | 12.4              |
| i7-8565U                     | Linux | 144     | 0.60    | 4.5    | 33          | 10.9              |
| i7-13700H                    | Linux | 81      | 0.43    | 3.4    | 16.2        | 5.5               |
| Xeon-6254                    | Linux | 1.4     | 0.69    | 3.3    | 39          |                   |
| Xeon-9460                    | Linux | 88      | 0.58    | 2.1    | 22          |                   |
| Cortex A53 (R.Pi3)           | Linux | 1515    | 8.4     | 40     | 407         | 75                |
| Cortex A72 (R.Pi4)           | Linux | 434     | 2.0     | 12.2   | 74          | 24                |
| Apple M1                     | macOS | 29      | 0.51    | 2.3    | 25          | 11.1              |
| Apple M1                     | Linux | 85      | 0.61    | 2.5    | 24          | 13.3              |
| Apple M3                     | macOS | 5.9     | 0.35    | 1.9    | 19.4        | 8.4               |
| Ampere Altra (Neoverse N1)   | Linux | 395     | 0.75    | 3.8    | 30          | 10.8              |
| AWS Graviton 3 (Neoverse V1) | Linux | 379     | 0.67    | 3.1    | 24          | 8.7               |
| Nvidia Grace (Neoverse V2)   | Linux | 89      | 0.42    | 2.0    | 16.7        |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 2006    | 12.0    | 44     | 126         | 32                |
| RISC-V (Qemu on i7-8565U)    | Linux | 2768    | 19.0    | 78     | 266         | 54                |
| s390x (Qemu on i7-8565U)     | Linux | 4776    | 0.000   | 0.000  | 339         | 0.000             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 17.2    | 0.30    | 1.3    | 12.7        | 3.6               |
| i7-5775R                     | Linux | 33      | 0.16    | 1.3    | 8.5         | 2.4               |
| i7-8565U                     | Linux | 28      | 0.12    | 0.89   | 6.5         | 2.2               |
| i7-13700H                    | Linux | 28      | 0.15    | 1.2    | 5.7         | 1.9               |
| Xeon-6254                    | Linux | 0.20    | 0.10    | 0.48   | 5.8         |                   |
| Xeon-9460                    | Linux | 16.7    | 0.11    | 0.41   | 4.2         |                   |
| Cortex A53 (R.Pi3)           | Linux | 20      | 0.11    | 0.54   | 5.4         | 1.0               |
| Cortex A72 (R.Pi4)           | Linux | 24      | 0.11    | 0.68   | 4.1         | 1.3               |
| Apple M1                     | macOS | 10.1    | 0.18    | 0.78   | 8.7         | 3.8               |
| Apple M1                     | Linux | 16.1    | 0.11    | 0.47   | 4.7         | 2.5               |
| Apple M3                     | macOS | 2.4     | 0.14    | 0.80   | 8.0         | 3.5               |
| Ampere Altra (Neoverse N1)   | Linux | 38      | 0.073   | 0.37   | 3.0         | 1.1               |
| AWS Graviton 3 (Neoverse V1) | Linux | 45      | 0.080   | 0.37   | 2.9         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 13.0    | 0.061   | 0.28   | 2.4         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 105     | 0.63    | 2.3    | 6.6         | 1.7               |
| RISC-V (Qemu on i7-8565U)    | Linux | 87      | 0.60    | 2.5    | 8.4         | 1.7               |
| s390x (Qemu on i7-8565U)     | Linux | 112     | 0.000   | 0.000  | 8.0         | 0.000             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.17    | 0.64    | 0.51   | 0.69        |                   |
| Cortex A72 (R.Pi4)           | Linux | 1.7     | 4.4     | 5.7    | 4.5         | 4.0               |
| Apple M1                     | macOS | 0.063   | 0.62    | 0.59   | 0.85        | 1.0               |
| Apple M1                     | Linux | 0.18    | 0.74    | 0.66   | 0.85        | 1.2               |
| Apple M3                     | macOS | 0.010   | 0.34    | 0.41   | 0.53        | 0.63              |
| Ampere Altra (Neoverse N1)   | Linux | 0.90    | 0.97    | 1.1    | 1.1         | 1.1               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.19    | 0.49    | 0.50   | 0.55        |                   |

### RSA-4096 parse private key

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 66      | 116     | 65     | 113         | 7.7               |
| i7-5775R                     | Linux | 168     | 88      | 99     | 111         | 7.7               |
| i7-8565U                     | Linux | 185     | 70      | 56     | 86          | 8.9               |
| i7-13700H                    | Linux | 87      | 47      | 44     | 39          | 3.2               |
| Xeon-6254                    | Linux | 4.5     | 75      | 44     | 106         |                   |
| Xeon-9460                    | Linux | 91      | 54      | 35     | 54          |                   |
| Cortex A53 (R.Pi3)           | Linux | 1567    | 1824    | 622    | 1170        | 60                |
| Cortex A72 (R.Pi4)           | Linux | 451     | 265     | 164    | 191         | 18.0              |
| Apple M1                     | macOS | 33      | 62      | 34     | 49          | 4.3               |
| Apple M1                     | Linux | 89      | 71      | 35     | 43          | 4.7               |
| Apple M3                     | macOS | 9.9     | 51      | 26     | 37          | 3.6               |
| Ampere Altra (Neoverse N1)   | Linux | 406     | 100     | 67     | 75          | 7.2               |
| AWS Graviton 3 (Neoverse V1) | Linux | 384     | 71      | 40     | 59          | 5.3               |
| Nvidia Grace (Neoverse V2)   | Linux | 91      | 53      | 30     | 40          |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 2055    | 1118    | 422    | 386         | 32                |
| RISC-V (Qemu on i7-8565U)    | Linux | 2838    | 1917    | 772    | 606         | 50                |
| s390x (Qemu on i7-8565U)     | Linux | 4178    | 2199    | 1896   | 833         | 0.000             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| i7-5775R                     | macOS | 19.0    | 32      | 18.5   | 32          | 2.2               |
| i7-5775R                     | Linux | 32      | 16.8    | 18.9   | 21          | 1.5               |
| i7-8565U                     | Linux | 36      | 14.0    | 11.2   | 17.0        | 1.8               |
| i7-13700H                    | Linux | 30      | 16.6    | 15.6   | 13.9        | 1.1               |
| Xeon-6254                    | Linux | 0.65    | 11.1    | 6.4    | 15.6        |                   |
| Xeon-9460                    | Linux | 17.4    | 10.4    | 6.8    | 10.4        |                   |
| Cortex A53 (R.Pi3)           | Linux | 20      | 24      | 8.3    | 15.6        | 0.81              |
| Cortex A72 (R.Pi4)           | Linux | 25      | 14.8    | 9.2    | 10.7        | 1.0               |
| Apple M1                     | macOS | 11.4    | 21      | 11.8   | 17.2        | 1.5               |
| Apple M1                     | Linux | 16.8    | 13.5    | 6.8    | 8.3         | 0.89              |
| Apple M3                     | macOS | 4.1     | 21      | 10.8   | 15.6        | 1.5               |
| Ampere Altra (Neoverse N1)   | Linux | 39      | 9.8     | 6.6    | 7.4         | 0.70              |
| AWS Graviton 3 (Neoverse V1) | Linux | 45      | 8.5     | 4.9    | 7.1         | 0.64              |
| Nvidia Grace (Neoverse V2)   | Linux | 13.3    | 7.7     | 4.4    | 5.8         |                   |
| RISC-V (Qemu on Apple M1)    | Linux | 107     | 58      | 22     | 20          | 1.7               |
| RISC-V (Qemu on i7-8565U)    | Linux | 90      | 60      | 24     | 19.3        | 1.6               |
| s390x (Qemu on i7-8565U)     | Linux | 98      | 51      | 44     | 19.5        | 0.000             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Libtomcrypt | Libtomcrypt / GMP |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :---------: | :---------------: |
| Xeon-9460                    | Linux | 0.18    | 0.57    | 0.66   | 0.69        |                   |
| Cortex A72 (R.Pi4)           | Linux | 1.7     | 5.4     | 5.8    | 4.7         | 4.9               |
| Apple M1                     | macOS | 0.070   | 0.71    | 0.68   | 0.68        | 0.65              |
| Apple M1                     | Linux | 0.19    | 0.81    | 0.72   | 0.60        | 0.72              |
| Apple M3                     | macOS | 0.017   | 0.47    | 0.42   | 0.42        | 0.44              |
| Ampere Altra (Neoverse N1)   | Linux | 0.92    | 1.2     | 1.4    | 1.1         | 1.2               |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0         | 1.0               |
| Nvidia Grace (Neoverse V2)   | Linux | 0.19    | 0.58    | 0.59   | 0.53        |                   |

## AES-128-CBC

### AES-128-CBC encryption

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 1180    | 1679    | 1177   | 1723   | 4491        |             |
| i7-5775R                     | Linux | 1244    | 2290    | 1227   | 1553   | 3692        |             |
| i7-8565U                     | Linux | 667     | 1436    | 677    | 956    | 3587        |             |
| i7-13700H                    | Linux | 429     | 932     | 447    | 782    | 2994        |             |
| Xeon-6254                    | Linux | 678     | 1411    | 679    | 947    | 3474        |             |
| Xeon-9460                    | Linux | 595     | 1289    | 592    | 879    | 3201        |             |
| Cortex A53 (R.Pi3)           | Linux | 21010   | 50739   | 56339  | 54874  | 37195       |             |
| Cortex A72 (R.Pi4)           | Linux | 19761   | 10909   | 12656  | 12638  | 12095       |             |
| Apple M1                     | macOS | 636     | 1159    | 3254   | 3265   | 3430        | 975         |
| Apple M1                     | Linux | 723     | 4849    | 720    | 1317   | 4179        | 1126        |
| Apple M3                     | macOS | 422     | 729     | 2602   | 2573   | 2729        | 717         |
| Ampere Altra (Neoverse N1)   | Linux | 424     | 4648    | 423    | 4976   | 4738        | 730         |
| AWS Graviton 3 (Neoverse V1) | Linux | 488     | 4266    | 492    | 4605   | 4273        | 850         |
| Nvidia Grace (Neoverse V2)   | Linux | 382     | 3131    | 374    | 728    | 3220        | 602         |
| RISC-V (Qemu on Apple M1)    | Linux | 19301   | 28468   | 27597  | 27583  | 38513       |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 30125   | 41928   | 38709  | 38793  | 57255       |             |
| s390x (Qemu on i7-8565U)     | Linux | 50705   | 54921   | 60871  | 58705  | 48596       |             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 335     | 477     | 334    | 489    | 1276        |             |
| i7-5775R                     | Linux | 237     | 436     | 233    | 295    | 703         |             |
| i7-8565U                     | Linux | 131     | 283     | 133    | 189    | 709         |             |
| i7-13700H                    | Linux | 150     | 327     | 157    | 275    | 1051        |             |
| Xeon-6254                    | Linux | 99      | 206     | 99     | 138    | 507         |             |
| Xeon-9460                    | Linux | 113     | 245     | 112    | 167    | 608         |             |
| Cortex A53 (R.Pi3)           | Linux | 279     | 675     | 749    | 730    | 495         |             |
| Cortex A72 (R.Pi4)           | Linux | 1102    | 608     | 705    | 704    | 674         |             |
| Apple M1                     | macOS | 220     | 400     | 1125   | 1128   | 1186        | 337         |
| Apple M1                     | Linux | 136     | 913     | 135    | 248    | 787         | 212         |
| Apple M3                     | macOS | 173     | 300     | 1070   | 1058   | 1123        | 295         |
| Ampere Altra (Neoverse N1)   | Linux | 41      | 453     | 41     | 485    | 462         | 71          |
| AWS Graviton 3 (Neoverse V1) | Linux | 58      | 508     | 58     | 548    | 509         | 101         |
| Nvidia Grace (Neoverse V2)   | Linux | 55      | 454     | 54     | 105    | 467         | 87          |
| RISC-V (Qemu on Apple M1)    | Linux | 1012    | 1493    | 1447   | 1446   | 2019        |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 956     | 1331    | 1229   | 1232   | 1818        |             |
| s390x (Qemu on i7-8565U)     | Linux | 1189    | 1288    | 1428   | 1377   | 1140        |             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| Xeon-9460                    | Linux | 0.91    | 0.22    | 0.89   | 0.14   | 0.56        |             |
| Cortex A72 (R.Pi4)           | Linux | 58      | 3.7     | 37     | 4.0    | 4.1         |             |
| Apple M1                     | macOS | 1.1     | 0.22    | 5.4    | 0.58   | 0.65        | 0.93        |
| Apple M1                     | Linux | 1.2     | 0.92    | 1.2    | 0.23   | 0.79        | 1.1         |
| Apple M3                     | macOS | 0.56    | 0.11    | 3.4    | 0.36   | 0.42        | 0.55        |
| Ampere Altra (Neoverse N1)   | Linux | 0.75    | 0.94    | 0.75   | 0.94   | 0.96        | 0.74        |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0    | 1.0         | 1.0         |
| Nvidia Grace (Neoverse V2)   | Linux | 0.62    | 0.58    | 0.60   | 0.12   | 0.59        | 0.56        |

### AES-128-CBC decryption

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 168     | 915     | 168    | 226    | 4136        |             |
| i7-5775R                     | Linux | 176     | 1512    | 174    | 234    | 3761        |             |
| i7-8565U                     | Linux | 163     | 1190    | 161    | 209    | 3469        |             |
| i7-13700H                    | Linux | 66      | 792     | 68     | 116    | 2541        |             |
| Xeon-6254                    | Linux | 161     | 1019    | 161    | 279    | 3333        |             |
| Xeon-9460                    | Linux | 92      | 851     | 90     | 116    | 2652        |             |
| Cortex A53 (R.Pi3)           | Linux | 22138   | 45732   | 47236  | 47203  | 39899       |             |
| Cortex A72 (R.Pi4)           | Linux | 14152   | 11032   | 11686  | 11682  | 12878       |             |
| Apple M1                     | macOS | 77      | 478     | 2619   | 2624   | 3190        | 142         |
| Apple M1                     | Linux | 97      | 4676    | 97     | 150    | 3643        | 190         |
| Apple M3                     | macOS | 50      | 139     | 1869   | 1881   | 2346        | 107         |
| Ampere Altra (Neoverse N1)   | Linux | 234     | 4495    | 259    | 4668   | 4900        | 390         |
| AWS Graviton 3 (Neoverse V1) | Linux | 114     | 4009    | 124    | 3993   | 4336        | 260         |
| Nvidia Grace (Neoverse V2)   | Linux | 89      | 2917    | 95     | 105    | 3173        | 154         |
| RISC-V (Qemu on Apple M1)    | Linux | 19085   | 35403   | 16119  | 16127  | 57693       |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 29662   | 49837   | 27297  | 26578  | 76965       |             |
| s390x (Qemu on i7-8565U)     | Linux | 31146   | 55083   | 39750  | 39514  | 98029       |             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 47      | 260     | 47     | 64     | 1175        |             |
| i7-5775R                     | Linux | 33      | 288     | 33     | 44     | 716         |             |
| i7-8565U                     | Linux | 32      | 235     | 31     | 41     | 685         |             |
| i7-13700H                    | Linux | 23      | 278     | 24     | 40     | 893         |             |
| Xeon-6254                    | Linux | 23      | 148     | 23     | 40     | 486         |             |
| Xeon-9460                    | Linux | 17.7    | 161     | 17.2   | 22     | 504         |             |
| Cortex A53 (R.Pi3)           | Linux | 294     | 608     | 628    | 628    | 531         |             |
| Cortex A72 (R.Pi4)           | Linux | 789     | 615     | 651    | 651    | 718         |             |
| Apple M1                     | macOS | 26      | 165     | 905    | 907    | 1102        | 49          |
| Apple M1                     | Linux | 18.3    | 881     | 18.4   | 28     | 686         | 35          |
| Apple M3                     | macOS | 20      | 57      | 769    | 774    | 965         | 44          |
| Ampere Altra (Neoverse N1)   | Linux | 22      | 438     | 25     | 455    | 478         | 38          |
| AWS Graviton 3 (Neoverse V1) | Linux | 13.7    | 477     | 14.9   | 475    | 516         | 31          |
| Nvidia Grace (Neoverse V2)   | Linux | 13.0    | 423     | 13.9   | 15.3   | 460         | 22          |
| RISC-V (Qemu on Apple M1)    | Linux | 1000    | 1856    | 845    | 845    | 3025        |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 942     | 1582    | 866    | 844    | 2444        |             |
| s390x (Qemu on i7-8565U)     | Linux | 730     | 1292    | 932    | 927    | 2299        |             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| Xeon-9460                    | Linux | 0.60    | 0.16    | 0.54   | 0.022  | 0.45        |             |
| Cortex A72 (R.Pi4)           | Linux | 177     | 4.0     | 135    | 4.2    | 4.3         |             |
| Apple M1                     | macOS | 0.55    | 0.097   | 17.0   | 0.53   | 0.60        | 0.45        |
| Apple M1                     | Linux | 0.69    | 0.95    | 0.64   | 0.031  | 0.68        | 0.60        |
| Apple M3                     | macOS | 0.29    | 0.023   | 9.7    | 0.31   | 0.35        | 0.27        |
| Ampere Altra (Neoverse N1)   | Linux | 1.8     | 0.97    | 1.8    | 1.0    | 0.98        | 1.3         |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0    | 1.0         | 1.0         |
| Nvidia Grace (Neoverse V2)   | Linux | 0.62    | 0.57    | 0.60   | 0.021  | 0.58        | 0.47        |

### AES-128-CBC encryption/decryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 7.0     | 1.8     | 7.0    | 7.6    | 1.1         |             |
| i7-5775R                     | Linux | 7.1     | 1.5     | 7.0    | 6.6    | 0.98        |             |
| i7-8565U                     | Linux | 4.1     | 1.2     | 4.2    | 4.6    | 1.0         |             |
| i7-13700H                    | Linux | 6.4     | 1.2     | 6.5    | 6.7    | 1.2         |             |
| Xeon-6254                    | Linux | 4.2     | 1.4     | 4.2    | 3.4    | 1.0         |             |
| Xeon-9460                    | Linux | 6.4     | 1.5     | 6.5    | 7.5    | 1.2         |             |
| Cortex A53 (R.Pi3)           | Linux | 0.95    | 1.1     | 1.2    | 1.2    | 0.93        |             |
| Cortex A72 (R.Pi4)           | Linux | 1.4     | 0.99    | 1.1    | 1.1    | 0.94        |             |
| Apple M1                     | macOS | 8.2     | 2.4     | 1.2    | 1.2    | 1.1         | 6.8         |
| Apple M1                     | Linux | 7.4     | 1.0     | 7.4    | 8.7    | 1.1         | 5.9         |
| Apple M3                     | macOS | 8.3     | 5.2     | 1.4    | 1.4    | 1.2         | 6.7         |
| Ampere Altra (Neoverse N1)   | Linux | 1.8     | 1.0     | 1.6    | 1.1    | 0.97        | 1.9         |
| AWS Graviton 3 (Neoverse V1) | Linux | 4.2     | 1.1     | 3.9    | 1.2    | 0.99        | 3.3         |
| Nvidia Grace (Neoverse V2)   | Linux | 4.3     | 1.1     | 3.9    | 6.9    | 1.0         | 3.9         |
| RISC-V (Qemu on Apple M1)    | Linux | 1.0     | 0.80    | 1.7    | 1.7    | 0.67        |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 1.0     | 0.84    | 1.4    | 1.5    | 0.74        |             |
| s390x (Qemu on i7-8565U)     | Linux | 1.6     | 1.0     | 1.5    | 1.5    | 0.50        |             |

## AES-256-CBC

### AES-256-CBC encryption

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 1643    | 2139    | 1646   | 2142   | 6009        |             |
| i7-5775R                     | Linux | 1742    | 2676    | 1719   | 2081   | 5112        |             |
| i7-8565U                     | Linux | 979     | 1737    | 927    | 1199   | 4814        |             |
| i7-13700H                    | Linux | 583     | 1089    | 613    | 981    | 4011        |             |
| Xeon-6254                    | Linux | 935     | 1661    | 934    | 1204   | 4604        |             |
| Xeon-9460                    | Linux | 811     | 1504    | 808    | 1095   | 4202        |             |
| Cortex A53 (R.Pi3)           | Linux | 27673   | 65286   | 72289  | 70831  | 47898       |             |
| Cortex A72 (R.Pi4)           | Linux | 27705   | 13703   | 16892  | 16825  | 15757       |             |
| Apple M1                     | macOS | 890     | 1411    | 4323   | 4319   | 4498        | 1276        |
| Apple M1                     | Linux | 1020    | 5845    | 1026   | 1613   | 5373        | 1470        |
| Apple M3                     | macOS | 574     | 920     | 3429   | 3449   | 3562        | 997         |
| Ampere Altra (Neoverse N1)   | Linux | 593     | 5957    | 592    | 6641   | 6144        | 949         |
| AWS Graviton 3 (Neoverse V1) | Linux | 679     | 5567    | 682    | 6147   | 5580        | 1081        |
| Nvidia Grace (Neoverse V2)   | Linux | 523     | 4081    | 523    | 875    | 4200        | 790         |
| RISC-V (Qemu on Apple M1)    | Linux | 24467   | 33076   | 33149  | 33143  | 43845       |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 37893   | 50247   | 47783  | 47619  | 65983       |             |
| s390x (Qemu on i7-8565U)     | Linux | 64662   | 65498   | 71239  | 72580  | 62154       |             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 466     | 608     | 467    | 608    | 1707        |             |
| i7-5775R                     | Linux | 331     | 509     | 327    | 396    | 973         |             |
| i7-8565U                     | Linux | 193     | 343     | 183    | 237    | 951         |             |
| i7-13700H                    | Linux | 205     | 382     | 215    | 344    | 1409        |             |
| Xeon-6254                    | Linux | 136     | 242     | 136    | 175    | 672         |             |
| Xeon-9460                    | Linux | 154     | 285     | 153    | 208    | 798         |             |
| Cortex A53 (R.Pi3)           | Linux | 368     | 869     | 962    | 942    | 637         |             |
| Cortex A72 (R.Pi4)           | Linux | 1545    | 764     | 942    | 938    | 878         |             |
| Apple M1                     | macOS | 307     | 487     | 1494   | 1493   | 1554        | 441         |
| Apple M1                     | Linux | 192     | 1101    | 193    | 303    | 1012        | 277         |
| Apple M3                     | macOS | 236     | 378     | 1411   | 1419   | 1465        | 410         |
| Ampere Altra (Neoverse N1)   | Linux | 57      | 581     | 57     | 648    | 599         | 92          |
| AWS Graviton 3 (Neoverse V1) | Linux | 80      | 663     | 81     | 732    | 665         | 128         |
| Nvidia Grace (Neoverse V2)   | Linux | 75      | 592     | 75     | 127    | 609         | 114         |
| RISC-V (Qemu on Apple M1)    | Linux | 1283    | 1734    | 1738   | 1738   | 2299        |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 1203    | 1595    | 1517   | 1512   | 2095        |             |
| s390x (Qemu on i7-8565U)     | Linux | 1517    | 1536    | 1671   | 1702   | 1458        |             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| Xeon-9460                    | Linux | 0.89    | 0.20    | 0.88   | 0.13   | 0.56        |             |
| Cortex A72 (R.Pi4)           | Linux | 58      | 3.6     | 35     | 4.0    | 4.1         |             |
| Apple M1                     | macOS | 1.1     | 0.21    | 5.1    | 0.57   | 0.65        | 0.96        |
| Apple M1                     | Linux | 1.2     | 0.85    | 1.2    | 0.21   | 0.78        | 1.1         |
| Apple M3                     | macOS | 0.55    | 0.11    | 3.3    | 0.36   | 0.41        | 0.60        |
| Ampere Altra (Neoverse N1)   | Linux | 0.76    | 0.93    | 0.75   | 0.94   | 0.95        | 0.76        |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0    | 1.0         | 1.0         |
| Nvidia Grace (Neoverse V2)   | Linux | 0.61    | 0.58    | 0.60   | 0.11   | 0.59        | 0.58        |

### AES-256-CBC decryption

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 234     | 1186    | 234    | 315    | 5730        |             |
| i7-5775R                     | Linux | 246     | 1668    | 245    | 328    | 4982        |             |
| i7-8565U                     | Linux | 230     | 1342    | 224    | 269    | 4746        |             |
| i7-13700H                    | Linux | 96      | 938     | 121    | 154    | 2859        |             |
| Xeon-6254                    | Linux | 225     | 1159    | 226    | 353    | 4431        |             |
| Xeon-9460                    | Linux | 129     | 1023    | 126    | 154    | 3616        |             |
| Cortex A53 (R.Pi3)           | Linux | 28485   | 58836   | 63194  | 63130  | 51241       |             |
| Cortex A72 (R.Pi4)           | Linux | 19975   | 13948   | 15877  | 15883  | 16647       |             |
| Apple M1                     | macOS | 143     | 558     | 3680   | 3947   | 4254        | 189         |
| Apple M1                     | Linux | 171     | 5860    | 153    | 174    | 5035        | 252         |
| Apple M3                     | macOS | 116     | 176     | 2711   | 2968   | 3176        | 131         |
| Ampere Altra (Neoverse N1)   | Linux | 317     | 5786    | 307    | 6313   | 6220        | 537         |
| AWS Graviton 3 (Neoverse V1) | Linux | 155     | 5283    | 175    | 5683   | 5621        | 347         |
| Nvidia Grace (Neoverse V2)   | Linux | 121     | 3850    | 123    | 133    | 4141        | 202         |
| RISC-V (Qemu on Apple M1)    | Linux | 24193   | 40043   | 21814  | 21842  | 71284       |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 37198   | 57469   | 35277  | 34749  | 94678       |             |
| s390x (Qemu on i7-8565U)     | Linux | 42516   | 66216   | 51480  | 53434  | 111634      |             |

Relative performance score (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 66      | 337     | 66     | 89     | 1628        |             |
| i7-5775R                     | Linux | 46      | 317     | 46     | 62     | 948         |             |
| i7-8565U                     | Linux | 45      | 265     | 44     | 53     | 938         |             |
| i7-13700H                    | Linux | 33      | 329     | 42     | 54     | 1004        |             |
| Xeon-6254                    | Linux | 32      | 169     | 33     | 51     | 647         |             |
| Xeon-9460                    | Linux | 24      | 194     | 24     | 29     | 687         |             |
| Cortex A53 (R.Pi3)           | Linux | 379     | 783     | 841    | 840    | 682         |             |
| Cortex A72 (R.Pi4)           | Linux | 1114    | 778     | 885    | 885    | 928         |             |
| Apple M1                     | macOS | 49      | 193     | 1272   | 1364   | 1470        | 65          |
| Apple M1                     | Linux | 32      | 1104    | 28     | 32     | 948         | 47          |
| Apple M3                     | macOS | 47      | 72      | 1115   | 1221   | 1306        | 53          |
| Ampere Altra (Neoverse N1)   | Linux | 31      | 565     | 30     | 616    | 607         | 52          |
| AWS Graviton 3 (Neoverse V1) | Linux | 18.6    | 629     | 20     | 677    | 669         | 41          |
| Nvidia Grace (Neoverse V2)   | Linux | 17.6    | 558     | 18.0   | 19.4   | 600         | 29          |
| RISC-V (Qemu on Apple M1)    | Linux | 1268    | 2099    | 1144   | 1145   | 3738        |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 1181    | 1825    | 1120   | 1103   | 3006        |             |
| s390x (Qemu on i7-8565U)     | Linux | 997     | 1553    | 1207   | 1253   | 2619        |             |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| Xeon-9460                    | Linux | 0.61    | 0.14    | 0.54   | 0.020  | 0.48        |             |
| Cortex A72 (R.Pi4)           | Linux | 185     | 3.8     | 130    | 4.0    | 4.3         |             |
| Apple M1                     | macOS | 0.75    | 0.086   | 17.0   | 0.56   | 0.62        | 0.44        |
| Apple M1                     | Linux | 0.90    | 0.90    | 0.71   | 0.025  | 0.73        | 0.59        |
| Apple M3                     | macOS | 0.48    | 0.022   | 10.0   | 0.34   | 0.37        | 0.25        |
| Ampere Altra (Neoverse N1)   | Linux | 1.8     | 0.95    | 1.5    | 0.96   | 0.96        | 1.3         |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0     | 1.0     | 1.0    | 1.0    | 1.0         | 1.0         |
| Nvidia Grace (Neoverse V2)   | Linux | 0.61    | 0.57    | 0.56   | 0.019  | 0.58        | 0.46        |

### AES-256-CBC encryption/decryption ratio

| CPU                          | OS    | OpenSSL | MbedTLS | GnuTLS | Nettle | Libtomcrypt | Arm64 accel |
| ---------------------------- | ----- | :-----: | :-----: | :----: | :----: | :---------: | :---------: |
| i7-5775R                     | macOS | 7.0     | 1.8     | 7.0    | 6.8    | 1.0         |             |
| i7-5775R                     | Linux | 7.1     | 1.6     | 7.0    | 6.3    | 1.0         |             |
| i7-8565U                     | Linux | 4.2     | 1.3     | 4.1    | 4.5    | 1.0         |             |
| i7-13700H                    | Linux | 6.0     | 1.2     | 5.0    | 6.3    | 1.4         |             |
| Xeon-6254                    | Linux | 4.2     | 1.4     | 4.1    | 3.4    | 1.0         |             |
| Xeon-9460                    | Linux | 6.3     | 1.5     | 6.4    | 7.1    | 1.2         |             |
| Cortex A53 (R.Pi3)           | Linux | 0.97    | 1.1     | 1.1    | 1.1    | 0.93        |             |
| Cortex A72 (R.Pi4)           | Linux | 1.4     | 0.98    | 1.1    | 1.1    | 0.95        |             |
| Apple M1                     | macOS | 6.2     | 2.5     | 1.2    | 1.1    | 1.1         | 6.7         |
| Apple M1                     | Linux | 5.9     | 1.0     | 6.7    | 9.3    | 1.1         | 5.8         |
| Apple M3                     | macOS | 4.9     | 5.2     | 1.3    | 1.2    | 1.1         | 7.6         |
| Ampere Altra (Neoverse N1)   | Linux | 1.9     | 1.0     | 1.9    | 1.1    | 0.99        | 1.8         |
| AWS Graviton 3 (Neoverse V1) | Linux | 4.4     | 1.1     | 3.9    | 1.1    | 0.99        | 3.1         |
| Nvidia Grace (Neoverse V2)   | Linux | 4.3     | 1.1     | 4.2    | 6.5    | 1.0         | 3.9         |
| RISC-V (Qemu on Apple M1)    | Linux | 1.0     | 0.83    | 1.5    | 1.5    | 0.62        |             |
| RISC-V (Qemu on i7-8565U)    | Linux | 1.0     | 0.87    | 1.4    | 1.4    | 0.70        |             |
| s390x (Qemu on i7-8565U)     | Linux | 1.5     | 0.99    | 1.4    | 1.4    | 0.56        |             |

## 2048-bit modular arithmetic

### 2048-bit modular arithmetic basic operations

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | Add   | Multiply | Multiply<br/>(Montgomery) | Multiply<br/>(reciprocal) | Square | Divide<br/>(reciprocal) | Inverse | Square<br/>Root |
| ---------------------------- | ----- | :---: | :------: | :------: | :------: | :----: | :----: | :-----: | :----: |
| i7-5775R                     | macOS | 0.32  | 4.1      | 0.97     | 2.8      | 4.0    | 0.030  | 255     | 1626   |
| i7-5775R                     | Linux | 0.28  | 4.2      | 1.0      | 3.0      | 3.9    | 0.036  | 253     | 3424   |
| i7-8565U                     | Linux | 0.26  | 3.8      | 1.5      | 2.7      | 3.6    | 0.031  | 259     | 2664   |
| i7-13700H                    | Linux | 0.14  | 1.7      | 0.93     | 1.5      | 1.7    | 0.019  | 147     | 1581   |
| Xeon-6254                    | Linux | 0.26  | 4.0      | 0.92     | 2.5      | 3.7    | 0.029  | 238     | 1616   |
| Xeon-9460                    | Linux | 0.18  | 2.6      | 0.99     | 2.0      | 2.4    | 0.021  | 194     | 3372   |
| Cortex A53 (R.Pi3)           | Linux | 3.5   | 130      | 35       | 87       | 123    | 0.43   | 3180    | 160771 |
| Cortex A72 (R.Pi4)           | Linux | 0.84  | 16.3     | 8.1      | 14.6     | 15.0   | 0.10   | 679     | 15433  |
| Apple M1                     | macOS | 0.22  | 4.8      | 0.88     | 3.2      | 4.8    | 0.000  | 228     | 2869   |
| Apple M1                     | Linux | 0.24  | 5.3      | 1.1      | 3.7      | 5.2    | 0.022  | 252     | 1793   |
| Apple M3                     | macOS | 0.17  | 3.7      | 0.59     | 2.3      | 3.6    | 0.000  | 172     | 1131   |
| Ampere Altra (Neoverse N1)   | Linux | 0.38  | 8.3      | 4.9      | 7.5      | 7.6    | 0.034  | 302     | 9234   |
| AWS Graviton 3 (Neoverse V1) | Linux | 0.30  | 5.9      | 1.5      | 4.3      | 5.8    | 0.025  | 273     | 2624   |
| Nvidia Grace (Neoverse V2)   | Linux | 0.19  | 3.6      | 1.1      | 2.9      | 3.6    | 0.018  | 173     | 3714   |
| RISC-V (Qemu on Apple M1)    | Linux | 2.2   | 33       | 16.0     | 43       | 28     | 0.62   | 1878    | 27965  |
| RISC-V (Qemu on i7-8565U)    | Linux | 3.4   | 53       | 25       | 70       | 44     | 0.78   | 2557    | 42683  |
| s390x (Qemu on i7-8565U)     | Linux | 0.000 | 0.000    | 0.000    | 0.000    | 0.000  | 0.000  | 2966    | 76952  |

Relative performance score (the lower, the better):

| CPU                          | OS    | Add   | Multiply | Multiply<br/>(Montgomery) | Multiply<br/>(reciprocal) | Square | Divide<br/>(reciprocal) | Inverse | Square<br/>Root |
| ---------------------------- | ----- | :---: | :------: | :------: | :------: | :----: | :----: | :-----: | :----: |
| i7-5775R                     | macOS | 0.091 | 1.2      | 0.27     | 0.81     | 1.1    | 0.009  | 72      | 462    |
| i7-5775R                     | Linux | 0.054 | 0.79     | 0.20     | 0.57     | 0.74   | 0.007  | 48      | 652    |
| i7-8565U                     | Linux | 0.051 | 0.76     | 0.29     | 0.54     | 0.71   | 0.006  | 51      | 526    |
| i7-13700H                    | Linux | 0.049 | 0.61     | 0.33     | 0.54     | 0.61   | 0.007  | 51      | 555    |
| Xeon-6254                    | Linux | 0.038 | 0.58     | 0.13     | 0.36     | 0.54   | 0.004  | 34      | 236    |
| Xeon-9460                    | Linux | 0.035 | 0.49     | 0.19     | 0.37     | 0.46   | 0.004  | 36      | 640    |
| Cortex A53 (R.Pi3)           | Linux | 0.046 | 1.7      | 0.48     | 1.2      | 1.6    | 0.006  | 42      | 2140   |
| Cortex A72 (R.Pi4)           | Linux | 0.047 | 0.91     | 0.45     | 0.82     | 0.84   | 0.006  | 37      | 860    |
| Apple M1                     | macOS | 0.077 | 1.7      | 0.30     | 1.1      | 1.7    | 0.000  | 78      | 991    |
| Apple M1                     | Linux | 0.045 | 1.0      | 0.20     | 0.70     | 0.98   | 0.004  | 47      | 337    |
| Apple M3                     | macOS | 0.070 | 1.5      | 0.24     | 0.95     | 1.5    | 0.000  | 70      | 465    |
| Ampere Altra (Neoverse N1)   | Linux | 0.037 | 0.81     | 0.48     | 0.73     | 0.74   | 0.003  | 29      | 901    |
| AWS Graviton 3 (Neoverse V1) | Linux | 0.036 | 0.71     | 0.17     | 0.52     | 0.70   | 0.003  | 32      | 312    |
| Nvidia Grace (Neoverse V2)   | Linux | 0.028 | 0.53     | 0.16     | 0.41     | 0.52   | 0.003  | 25      | 538    |
| RISC-V (Qemu on Apple M1)    | Linux | 0.12  | 1.8      | 0.84     | 2.3      | 1.5    | 0.033  | 98      | 1466   |
| RISC-V (Qemu on i7-8565U)    | Linux | 0.11  | 1.7      | 0.82     | 2.3      | 1.4    | 0.025  | 81      | 1355   |
| s390x (Qemu on i7-8565U)     | Linux | 0.000 | 0.000    | 0.000    | 0.000    | 0.000  | 0.000  | 69      | 1805   |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | Add  | Multiply | Multiply<br/>(Montgomery) | Multiply<br/>(reciprocal) | Square | Divide<br/>(reciprocal) | Inverse | Square<br/>Root |
| ---------------------------- | ----- | :--: | :------: | :------: | :------: | :----: | :----: | :-----: | :----: |
| Xeon-9460                    | Linux | 0.45 | 0.32     | 0.50     | 0.34     | 0.31   | 0.64   | 0.53    | 0.95   |
| Cortex A72 (R.Pi4)           | Linux | 4.1  | 4.0      | 8.0      | 4.9      | 3.7    | 5.9    | 3.6     | 8.5    |
| Apple M1                     | macOS | 0.60 | 0.66     | 0.49     | 0.61     | 0.67   | 0.000  | 0.68    | 0.89   |
| Apple M1                     | Linux | 0.65 | 0.73     | 0.59     | 0.70     | 0.72   | 0.72   | 0.75    | 0.56   |
| Apple M3                     | macOS | 0.37 | 0.40     | 0.26     | 0.35     | 0.41   | 0.000  | 0.41    | 0.28   |
| Ampere Altra (Neoverse N1)   | Linux | 1.1  | 1.2      | 2.9      | 1.5      | 1.1    | 1.2    | 0.96    | 3.0    |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0  | 1.0      | 1.0      | 1.0      | 1.0    | 1.0    | 1.0     | 1.0    |
| Nvidia Grace (Neoverse V2)   | Linux | 0.51 | 0.48     | 0.59     | 0.52     | 0.48   | 0.58   | 0.50    | 1.1    |

### 2048-bit modular arithmetic exponentiations on public (short) exponents

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | Exponent | Exponent<br/>(Montgomery) | Exponent<br/>(Montgomery)<br/>(word) | Exponent<br/>(reciprocal) | Exponent<br/>(simple) |
| ---------------------------- | ----- | :------: | :------: | :------: | :------: | :------: |
| i7-5775R                     | macOS | 21       | 14.3     | 12.7     | 53       | 71       |
| i7-5775R                     | Linux | 22       | 14.9     | 13.3     | 57       | 72       |
| i7-8565U                     | Linux | 28       | 22       | 19.5     | 49       | 62       |
| i7-13700H                    | Linux | 16.7     | 13.4     | 10.9     | 28       | 29       |
| Xeon-6254                    | Linux | 20       | 14.2     | 12.5     | 46       | 64       |
| Xeon-9460                    | Linux | 19.5     | 15.2     | 12.6     | 38       | 44       |
| Cortex A53 (R.Pi3)           | Linux | 857      | 708      | 636      | 1523     | 2117     |
| Cortex A72 (R.Pi4)           | Linux | 152      | 128      | 104      | 246      | 261      |
| Apple M1                     | macOS | 19.2     | 12.8     | 10.9     | 60       | 83       |
| Apple M1                     | Linux | 22       | 15.4     | 12.9     | 65       | 92       |
| Apple M3                     | macOS | 15.2     | 9.8      | 8.6      | 44       | 63       |
| Ampere Altra (Neoverse N1)   | Linux | 87       | 76       | 61       | 125      | 131      |
| AWS Graviton 3 (Neoverse V1) | Linux | 30       | 22       | 19.0     | 78       | 101      |
| Nvidia Grace (Neoverse V2)   | Linux | 21       | 16.7     | 14.0     | 51       | 63       |
| RISC-V (Qemu on Apple M1)    | Linux | 314      | 230      | 183      | 690      | 486      |
| RISC-V (Qemu on i7-8565U)    | Linux | 487      | 361      | 294      | 1156     | 789      |
| s390x (Qemu on i7-8565U)     | Linux | 0.000    | 349      | 0.000    | 764      | 669      |

Relative performance score (the lower, the better):

| CPU                          | OS    | Exponent | Exponent<br/>(Montgomery) | Exponent<br/>(Montgomery)<br/>(word) | Exponent<br/>(reciprocal) | Exponent<br/>(simple) |
| ---------------------------- | ----- | :------: | :------: | :------: | :------: | :------: |
| i7-5775R                     | macOS | 6.2      | 4.1      | 3.6      | 15.2     | 20       |
| i7-5775R                     | Linux | 4.3      | 2.8      | 2.5      | 11.0     | 13.7     |
| i7-8565U                     | Linux | 5.7      | 4.4      | 3.9      | 9.9      | 12.3     |
| i7-13700H                    | Linux | 5.9      | 4.7      | 3.8      | 10.1     | 10.4     |
| Xeon-6254                    | Linux | 3.0      | 2.1      | 1.8      | 6.7      | 9.5      |
| Xeon-9460                    | Linux | 3.7      | 2.9      | 2.4      | 7.2      | 8.4      |
| Cortex A53 (R.Pi3)           | Linux | 11.4     | 9.4      | 8.5      | 20       | 28       |
| Cortex A72 (R.Pi4)           | Linux | 8.5      | 7.2      | 5.8      | 13.7     | 14.6     |
| Apple M1                     | macOS | 6.6      | 4.4      | 3.8      | 21       | 28       |
| Apple M1                     | Linux | 4.2      | 2.9      | 2.4      | 12.4     | 17.5     |
| Apple M3                     | macOS | 6.2      | 4.0      | 3.5      | 18.4     | 26       |
| Ampere Altra (Neoverse N1)   | Linux | 8.5      | 7.5      | 6.1      | 12.3     | 12.9     |
| AWS Graviton 3 (Neoverse V1) | Linux | 3.6      | 2.7      | 2.3      | 9.4      | 12.1     |
| Nvidia Grace (Neoverse V2)   | Linux | 3.1      | 2.4      | 2.0      | 7.4      | 9.1      |
| RISC-V (Qemu on Apple M1)    | Linux | 16.5     | 12.1     | 9.6      | 36       | 25       |
| RISC-V (Qemu on i7-8565U)    | Linux | 15.5     | 11.5     | 9.4      | 36       | 25       |
| s390x (Qemu on i7-8565U)     | Linux | 0.000    | 8.2      | 0.000    | 17.9     | 15.7     |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | Exponent | Exponent<br/>(Montgomery) | Exponent<br/>(Montgomery)<br/>(word) | Exponent<br/>(reciprocal) | Exponent<br/>(simple) |
| ---------------------------- | ----- | :------: | :------: | :------: | :------: | :------: |
| Xeon-9460                    | Linux | 0.47     | 0.50     | 0.49     | 0.36     | 0.33     |
| Cortex A72 (R.Pi4)           | Linux | 7.2      | 8.3      | 7.9      | 4.5      | 3.7      |
| Apple M1                     | macOS | 0.51     | 0.46     | 0.47     | 0.63     | 0.67     |
| Apple M1                     | Linux | 0.60     | 0.56     | 0.55     | 0.68     | 0.74     |
| Apple M3                     | macOS | 0.32     | 0.28     | 0.29     | 0.37     | 0.41     |
| Ampere Altra (Neoverse N1)   | Linux | 2.5      | 3.0      | 2.8      | 1.4      | 1.1      |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0      | 1.0      | 1.0      | 1.0      | 1.0      |
| Nvidia Grace (Neoverse V2)   | Linux | 0.56     | 0.58     | 0.58     | 0.51     | 0.49     |

### 2048-bit modular arithmetic exponentiations on private (large) exponents

Execution time in microseconds (the lower, the better):

| CPU                          | OS    | Exponent | Exponent<br/>(Montgomery) | Exponent<br/>(Montgomery)<br/>(word) | Exponent<br/>(reciprocal) | Exponent<br/>(simple) |
| ---------------------------- | ----- | :------: | :------: | :------: | :------: | :------: |
| i7-5775R                     | macOS | 1652     | 1640     | 1660     | 6630     | 9672     |
| i7-5775R                     | Linux | 1851     | 1705     | 1832     | 7138     | 9391     |
| i7-8565U                     | Linux | 2610     | 2684     | 2388     | 6300     | 8666     |
| i7-13700H                    | Linux | 1593     | 1582     | 1417     | 3893     | 4069     |
| Xeon-6254                    | Linux | 1595     | 1597     | 1639     | 5935     | 9039     |
| Xeon-9460                    | Linux | 1783     | 1783     | 1627     | 5017     | 6030     |
| Cortex A53 (R.Pi3)           | Linux | 84912    | 84760    | 78166    | 196706   | 295110   |
| Cortex A72 (R.Pi4)           | Linux | 15397    | 15371    | 13614    | 32410    | 36296    |
| Apple M1                     | macOS | 1471     | 1464     | 1429     | 7894     | 11609    |
| Apple M1                     | Linux | 1742     | 1737     | 1701     | 8719     | 13003    |
| Apple M3                     | macOS | 1119     | 1113     | 1103     | 5735     | 8706     |
| Ampere Altra (Neoverse N1)   | Linux | 9189     | 9180     | 8052     | 16628    | 18248    |
| AWS Graviton 3 (Neoverse V1) | Linux | 2617     | 2609     | 2441     | 10329    | 14145    |
| Nvidia Grace (Neoverse V2)   | Linux | 1951     | 1949     | 1808     | 6793     | 9234     |
| RISC-V (Qemu on Apple M1)    | Linux | 27633    | 27523    | 24490    | 92583    | 67942    |
| RISC-V (Qemu on i7-8565U)    | Linux | 42673    | 42464    | 38362    | 153789   | 109387   |
| s390x (Qemu on i7-8565U)     | Linux | 42387    | 42138    | 42543    | 101567   | 94984    |

Relative performance score (the lower, the better):

| CPU                          | OS    | Exponent | Exponent<br/>(Montgomery) | Exponent<br/>(Montgomery)<br/>(word) | Exponent<br/>(reciprocal) | Exponent<br/>(simple) |
| ---------------------------- | ----- | :------: | :------: | :------: | :------: | :------: |
| i7-5775R                     | macOS | 469      | 466      | 471      | 1884     | 2748     |
| i7-5775R                     | Linux | 352      | 324      | 348      | 1359     | 1788     |
| i7-8565U                     | Linux | 516      | 530      | 472      | 1245     | 1713     |
| i7-13700H                    | Linux | 559      | 556      | 498      | 1367     | 1429     |
| Xeon-6254                    | Linux | 233      | 233      | 239      | 866      | 1320     |
| Xeon-9460                    | Linux | 339      | 339      | 309      | 953      | 1146     |
| Cortex A53 (R.Pi3)           | Linux | 1130     | 1128     | 1040     | 2618     | 3928     |
| Cortex A72 (R.Pi4)           | Linux | 858      | 857      | 759      | 1807     | 2024     |
| Apple M1                     | macOS | 508      | 506      | 494      | 2728     | 4013     |
| Apple M1                     | Linux | 328      | 327      | 320      | 1642     | 2449     |
| Apple M3                     | macOS | 460      | 458      | 454      | 2359     | 3582     |
| Ampere Altra (Neoverse N1)   | Linux | 897      | 896      | 786      | 1623     | 1781     |
| AWS Graviton 3 (Neoverse V1) | Linux | 311      | 310      | 290      | 1231     | 1685     |
| Nvidia Grace (Neoverse V2)   | Linux | 283      | 282      | 262      | 985      | 1339     |
| RISC-V (Qemu on Apple M1)    | Linux | 1449     | 1443     | 1284     | 4855     | 3563     |
| RISC-V (Qemu on i7-8565U)    | Linux | 1355     | 1348     | 1218     | 4884     | 3474     |
| s390x (Qemu on i7-8565U)     | Linux | 994      | 988      | 998      | 2382     | 2228     |

Execution time ratio, compared to reference system, frequencies included (the lower, the better):

| CPU                          | OS    | Exponent | Exponent<br/>(Montgomery) | Exponent<br/>(Montgomery)<br/>(word) | Exponent<br/>(reciprocal) | Exponent<br/>(simple) |
| ---------------------------- | ----- | :------: | :------: | :------: | :------: | :------: |
| Xeon-9460                    | Linux | 0.51     | 0.51     | 0.50     | 0.36     | 0.32     |
| Cortex A72 (R.Pi4)           | Linux | 8.5      | 8.5      | 8.1      | 4.5      | 3.7      |
| Apple M1                     | macOS | 0.46     | 0.46     | 0.48     | 0.62     | 0.67     |
| Apple M1                     | Linux | 0.54     | 0.54     | 0.57     | 0.69     | 0.75     |
| Apple M3                     | macOS | 0.28     | 0.28     | 0.29     | 0.36     | 0.40     |
| Ampere Altra (Neoverse N1)   | Linux | 3.0      | 3.0      | 2.9      | 1.4      | 1.1      |
| AWS Graviton 3 (Neoverse V1) | Linux | 1.0      | 1.0      | 1.0      | 1.0      | 1.0      |
| Nvidia Grace (Neoverse V2)   | Linux | 0.59     | 0.59     | 0.58     | 0.52     | 0.51     |

