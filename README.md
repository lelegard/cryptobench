# Cryptographic libraries comparative benchmarks

**Contents:**

* [Purpose](#purpose)
* [Software support](#software-support)
* [Building and prerequisites](#building-and-prerequisites)
* [Command line usage](#command-line-usage)
* [Test results](#test-results)
* [Bugs and limitations](#bugs-and-limitations)
* [Miscelaneous observations](#miscelaneous-observations)
  * [RSA performances on old Arm CPU cores](#rsa-performances-on-old-arm-cpu-cores)

## Purpose

This project evaluates a comparison of cryptographic libraries from two perspectives:

- Comparing libraries: On each system, the time of each operation is given in micro-seconds.
- Comparing processors efficiency: For each processor, a "standard" performance test is
  run first. Then, for each cryptographic operation, a "score" is reported, based on the
  standard test of the processor.

The "score" compares the efficiency of an algorithm between processors. It is the ratio
of the processing time of a cryptographic operation over the "standard" performance test.

If a given cryptographic operation had the same efficiency on all processors, its
score would be the same on all processors, slow low-end processors or fast high-end
processors.

If a score on processor A is higher than on a processor B, this means that the corresponding
cryptographic operation is less efficient on A (it takes more time, relatively to the
"standard" performances of the processor).

For a given cryptographic library, if an operation is implemented with the same
source code on distinct processors, the scores for that operation are direct
indications of the relative efficiencies of the two processors. However, if the
two scores are extremely different, this may be an indication that the operation
may have been compiled with accelerated instructions on the processor with the lower
score and with portable source code on the processor with the higher score.

The "scores" are relative values within a given cryptographic operation.
The absolute values have no significance. Comparing the scores of different
cryptographic operations is pointless.

The "standard" test uses integer arithmetics and bitwise operations. It is supposed to
be representative of the basic operations in symmetric and asymmetric cryptography.
There is no floating point tests since this type of data is not used in cryptography.
Currently, this test is very basic (see file `src/bench_reference.cpp`). Ideas to
improve it are welcome.

The results are presented in the file [RESULTS.md](RESULTS.md).

## Software support

The `cryptobench` program can be built on:
- Linux (tested on Ubuntu, Debian, Fedora, Redhat)
- macOS

Tested architectures:
- Intel x86_64 (aka amd64)
- Arm64 (aka aarch64)

Tested cryptographic libraries:
- OpenSSL
- GnuTLS
- MbedTLS
- Nettle (also used through GnuTLS)
- Libtomcrypt (with libtommath and/or GMP as math backends)

Tested algorithms:
- RSA (2048 and 4096 bits, encryption and signature)
- AES (128 and 256 bits, CBC mode on large data)
- 2048-bit modular arithmetic using OpenSSL

The modular arithmetic tests give performance indicators on the various elementary
operations which are used by RSA. These tests may help to understand the differences
in RSA performances between systems.

## Building and prerequisites

Software prerequisites to build `cryptobench`:

- Ubuntu/Debian: `sudo apt install libssl-dev libmbedtls-dev libgnutls28-dev nettle-dev libtomcrypt-dev libtommath-dev libgmp-dev`
- Fedora/Redhat: `sudo dnf install openssl-devel mbedtls-devel gnutls-devel nettle-devel libtomcrypt-devel libtommath-devel gmp-devel`
- macOS: `brew install openssl mbedtls gnutls nettle libtomcrypt libtommath gmp`

Note: On Ubuntu/Debian, the package `libgnutls28-dev` is probably misnamed.
It is the gnutls development environment for the latest versions of gnutls,
3.x.x as of this writing. It is not (or no longer) gnutls version 2.8.

Just run `make` to build the `cryptobench` executable. All objects are
produced in subdirectory `build`.

## Command line usage

Without parameter, the `cryptobench` program runs all tests for
all libraries. Each cryptographic operation is repeatedly run during
at least 2 seconds.

Using command line options, it is possible to run only selected tests
on selected libraries. Any combination is possible.
~~~
$ cryptobench --help

Command line options:
  -h, --help : display this help text
  -m sec, --min-time sec : minimum run time of each test in seconds
  --min-iterations count : minimum number of iteration for each test
  --aes-data-size count : data size for AES-CBC tests
  -r, --reference : run reference benchmark only

Algorithms selection (all by default):
  --aes, --rsa

Crytographic libraries selection (all by default):
  --gnutls, --mbedtls, --openssl, --nettle, --tomcrypt, --tomcrypt-gmp --arm64
~~~

The `arm64` pseudo-library is an implementation of AES using Arm64 specialized
accelerated instructions, through C/C++ intrinsics. It is run only on Arm64
processors supporting FEAT_AES.

## Test results

Test reports are stored in the subdirectory `reports` and saved in the git
repository for reference.

The script `tools/run-test.sh` produces a benchmark report on the current system.
All tests are run during 5 seconds, so the execution may take some time, 12 minutes
or more. It also produces a description of the current system for reference.

Typical usage:
~~~
$ tools/run-test.sh >reports/system-cpu-os.txt
~~~

On some systems, the CPU is not correctly identified by the script.
Be sure to verify the line containing `system: cpu:` in the report
file and manually fix it when necessary before committing to git.

To regenerate the file [RESULTS.md](RESULTS.md), use the following script:
~~~
$ tools/analyze-reports.py reports/*.txt >RESULTS.md
~~~

## Bugs and limitations

GnuTLS: It is unclear which type of padding is used for RSA encrypt/decrypt.
How can we do OAEP with GnuTLS? What kind of padding does `gnutls_pubkey_encrypt_data()` use?
With sign/verify, using PSS is explicit. But there is no equivalent parameters
in RSA encryption functions.

MbedTLS: PKCS#1 v1.5 padding is used on RSA signature instead of PSS.
With MbedTLS v3, trying to sign with PSS returns the error code
`MBEDTLS_ERR_PK_BAD_INPUT_DATA`. With MbedTLS v2, the function `mbedtls_pk_sign_ext()`
which includes a padding argument does not exist.

Nettle: RSA tests are currently disabled. There is an issue loading public and private
key files, as created by OpenSSL. The parsing of the DER sequence fails. See source
file `src/lib_nettle.cpp` for a fix. To be investigated...

## Miscelaneous observations

### RSA performances on old Arm CPU cores

Looking at [RESULTS.md](RESULTS.md), we see that the RSA performances are surprisingly low
on Ampere Altra CPU's. By "low", we mean that the RSA operations (encryption, decryption,
signature, verification) are slow, relatively to any other cryptographic operation on the
same CPU. On other CPU's, the RSA operations run much faster, again relatively to the
performance of their respective CPU. This is where the "relative performance score"
indicators are useful.

No need to blame the Arm ISA, other Arm CPU's such as the AWS Graviton 3 or the Apple M1
do not have this performance bias on RSA. They execute RSA operations with the same
relative efficiency as any other cryptographic operation.

Looking at the OpenSSL modular arithmetic operations benchmarks at the end of
[RESULTS.md](RESULTS.md), we see the exact same bias on the Ampere Altra only,
on the modular multiplication (Montgomery algorithm only), the modular square
root and the modular exponentiation.

This means that the root cause is the implementation of the Montgomery algorithm for
the multiplication. The four basic RSA operations are based on the modular exponentiation
which in turn uses the Montgomery multiplication. And the modular square root is based
on the modular exponentiation too.

So, what is different in the Ampere Altra CPU to impact the Montgomery algorithm
and nothing else?

The answer is in the CPU core. Ampere Altra is based on an Arm Neoverse N1 core.
AWS Graviton 3 is based on a more recent Arm Neoverse V1 core. Apple M1 is based
on Apple Firestorm/Icestorm cores. In the tested systems, the Ampere Altra is the
only one with a Neoverse N1 core (note: the older Graviton 2 is also based on
Neoverse N1 but I did not have access to any).

To justify this conclusion, let's investigate how OpenSSL implements the Montgomery
algorithm. Because it is highly critical for asymmetric cryptography performances,
this algorithm is implemented in assembly code
([source code for Armv8 here](https://github.com/openssl/openssl/blob/master/crypto/bn/asm/armv8-mont.pl)).

This implementation makes a heavy usage of the `umulh` instruction. Based on the public
documentation from Arm, we see that the performance of this instruction is quite bad
on the Neoverse N1 and much better on the Neoverse V1.

Reference public documents:
- [Arm Neoverse N1 Core Software Optimization Guide](https://developer.arm.com/documentation/pjdoc466751330-9707/latest/)
- [Arm Neoverse V1 Software Optimization Guide](https://developer.arm.com/documentation/pjdoc466751330-9685/latest/)

These documents precisely describe the performances of each instruction. Let's check
the latency of the `umulh` instruction. On the N1, the latency is described as "5(3)"
cycles, while it is only 3 cycles on the V1.

In the N1 guide, we also read about `umulh` (and `smulh`): _"Multiply high operations stall
the multiplier pipeline for N extra cycles before any other type M uop can be issued to
that pipeline, with N shown in parentheses."_

So the actual stall is 8 cycles in the "Integer single/multicycle" pipeline (M) on the N1.
Looking the OpenSSL assembly source code
[armv8-mont](https://github.com/openssl/openssl/blob/master/crypto/bn/asm/armv8-mont.pl)
for the Montgomery algorithm, we see sequences of adjacent instructions such as _"umulh,
mul, umulh, mul, mov, umulh, mul, subs, umulh, adc"_. All these instructions - except `mov` -
use the M pipeline which consequently becomes a bottleneck.

This bottleneck does not exist on the Arm Neoverse V1. There is no equivalent public
document about the Apple Firestorm/Icestorm cores but we can assume that there is
no such issue either.

This explains why RSA is so slow on CPU's which are based on the Arm Neoverse N1 core.
This problem no longer exists on Arm Neoverse V1 and more recent Arm cores.
