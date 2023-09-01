# Cryptographic libraries comparative benchmarks

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

## Test results

Test reports are stored in the subdirectory `reports` and saved in the git
repository for reference.

The script `tools/run-test.sh` produces a benchmarck report on the current system.
All tests are run during 5 seconds, so the execution may take some time, 10 minutes or more.
It also produces a description of the current system for reference.

Typical usage:
~~~
$ tools/run-test.sh >reports/system-cpu-os.txt
~~~

On some systems, the CPU is not correctly identified by the script.
Be sure to verify the line containing `system: cpu:` in the report
file and manually fix it when necessary before committing to git.

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
