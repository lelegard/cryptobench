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
  * [Multiplication and carry on Neoverse V1](#multiplication-and-carry-on-neoverse-v1)

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

The answer is the difference of CPU core and especially the implementation of the
64-bit multiplication in the Ampere Altra compared to the AWS Graviton 3 or the
Apple M1.

Ampere Altra is based on an Arm Neoverse N1 core. AWS Graviton 3 is based on a more
recent Arm Neoverse V1 core. Apple M1 is based on Apple Firestorm/Icestorm cores.
In the tested systems, the Ampere Altra is the only one with a Neoverse N1 core
(note: the older Graviton 2 is also based on Neoverse N1 but I did not have access to any).

To justify this conclusion, let's investigate how OpenSSL implements the Montgomery
algorithm. Because it is highly critical for asymmetric cryptography performances,
this algorithm is implemented in assembly code
([source code for Armv8 here](https://github.com/openssl/openssl/blob/master/crypto/bn/asm/armv8-mont.pl)).

This implementation makes a heavy usage of the MUL and UMULH instructions. Based
on the public documentation from Arm, we see that the performance of these instructions
is significantly lower on the Neoverse N1 and much better on the Neoverse V1.

Reference public documents:
- [Arm Neoverse N1 Core Software Optimization Guide](https://developer.arm.com/documentation/pjdoc466751330-9707/latest/)
- [Arm Neoverse V1 Software Optimization Guide](https://developer.arm.com/documentation/pjdoc466751330-9685/latest/)

These documents precisely describe the performances of each instruction. Let's check
the latency of the MUL and UMULH instructions. The following table gives the number
of cycles per instruction. The additional number in parenthesis, when present, is
described as follow in the N1 documentation: _"Multiply high operations stall the
multiplier pipeline for N extra cycles before any other type M uop can be issued to
that pipeline, with N shown in parentheses."_

| Latency                     | Neoverse N1 | Neoverse V1 |
| --------------------------- | :---------: | :---------: |
| MADD, MUL, W-form (32 bits) | 2(1)        | 2(1)        |
| MADD, MUL, X-form (64 bits) | 4(3)        | 2(1)        |
| UMULH                       | 5(3)        | 3           |

Surprisingly, on the N1, the 32-bit multiplication (MUL) is much more expensive than
the 64-bit one. On the V1, they have the same cost. In short 64-bit MUL and UMULH
are twice as expensive on the N1, compared to the V1.

Looking the OpenSSL assembly source code
[armv8-mont](https://github.com/openssl/openssl/blob/master/crypto/bn/asm/armv8-mont.pl)
for the Montgomery algorithm, we see sequences of adjacent instructions such as _"UMULH,
MUL, UMULH, MUL, MOV, UMULH, MUL, SUBS, UMULH, ADC"_. All these instructions - except MOV -
use the M pipeline which consequently becomes a bottleneck. Each pair of adjacent MUL, UMULH
costs 15 cycles, especially because of the extra stall.

This bottleneck does not exist on the Arm Neoverse V1. Each pair of adjacent MUL, UMULH
only costs 6 cycles (instead of 15).

There is no equivalent public document about the Apple Firestorm/Icestorm cores but we can
assume that there is no such issue either.

To confirm this diagnostic, we have tweaked the OpenSSL assembly code for the Montgomery
algorithm. First, we replaced all UMULH instructions with MUL. Then, we replaced all
MUL and UMULH instructions with NOP. The result is of course no longer functional
but we focus only on the execution time.

The table below evaluates the execution time in microseconds of the Montgomery
algorithm on the various CPU cores, with and without MUL or UMULH instructions.
Thus, we evaluate the marginal cost of these specific instructions.

| Execution time (microseconds)      | Neoverse N1 | Neoverse V1 | Apple M1 |
| ---------------------------------- | :---------: | :---------: | :------: |
| OpenSSL original source code       | 4.82        | 1.45        | 1.04     |
| Replace all UMULH with MUL         | 4.13        | 1.17        | 1.04     |
| Replace all UMULH and MUL with NOP | 1.06        | 1.01        | 1.00     |

We can see that the MUL and UMULH instructions have a huge cost on the Neoverse N1,
much higher that on other CPU cores.

This explains why RSA is so slow on CPU's which are based on the Arm Neoverse N1 core.
This problem no longer exists on Arm Neoverse V1 and more recent Arm cores.

### Multiplication and carry on Neoverse V1

The investigation in the previous section started from a visible effect: poor RSA
performance on Ampere Altra (Neoverse N1) compared to Graviton 3 (Neoverse V1),
as well as compared to Intel CPU's, relatively to the standard performance of each
CPU. The identified reason is the sequence of interspersed MUL and MULH.

Out of curiosity, let's look at the OpenSSL assembly code for the Montgomery
multiplication again and let's try to find other characteristic sequences of
instructions involving multiplications.

We easily spot another long sequence of ADCS MUL ADCS UMULH instructions.

For the record, ADCS is an addition with input carry (C) and setting the
carry on output (S). Variants ADC, ADDS and ADD exist with carry on input
only, output only or no carry at all.

This time, we try to evaluate the mean time of an instruction inside a given
sequence:

1. We start with a long sequence of NOP to have a reference.
2. MUL instructions.
3. Interspersed MUL and UMULH (issue from the previous section).
4. Interspersed MUL ADCS UMULH ADCS (from the new instruction pattern).
5. Same sequence with ADD instead of ADCS.
6. Full long sequence from OpenSSL, with many MUL ADCS UMULH ADCS.
   There are also some ADC and ADDS.
7. Same sequence with ADD instead of ADCS, ADC, ADDS.

In cases number 2, 3, 4, 5, the output registers of the instructions are all
distinct to avoid execution dependencies. The input registers are the same in
all instructions. In cases number 6 and 7, the register allocation from OpenSSL
code was not modified.

See the [test source code](issues/mult_arm.S) for the details.

The results are summarized below:

| Mean instruction time (nanoseconds) | Neoverse N1 | Neoverse V1 | Apple M1 |
| ----------------------------------- | :---------: | :---------: | :------: |
| NOP                                 | 0.083       | 0.032       | 0.048    |
| MUL                                 | 0.918       | 0.144       | 0.145    |
| MUL UMULH                           | 1.085       | 0.144       | 0.145    |
| MUL ADCS UMULH ADCS                 | 0.501       | 0.248       | 0.152    |
| MUL ADD UMULH ADD                   | 0.501       | 0.092       | 0.086    |
| Full OpenSSL sequence (MUL & ADCS)  | 0.525       | 0.130       | 0.086    |
| Same sequence with ADD only         | 0.525       | 0.106       | 0.081    |

The results are quite surprising when comparing the Neoverse N1 and V1.

On the Neoverse N1, interleaving MUL and UMULH is not good (we already knew that).
Having said that, the 64-bit multiplication alone is not that good either.
Replacing the interspersed UMULH with MUL is slightly faster but not that much
(0.918 ns/inst instead of 1.085).

When we intersperse ADCS between MUL and/or UMULH, the time per instruction
drops by a half (0.501 ns instead of 1.085). According to the Arm Neoverse N1
and V1 Software Optimization Guides, the MUL and UMULH use the pipeline M or M0
and all forms of ADxx use the pipeline I. The two pipelines are independent in
the two cores, which explains the performance boost. Moreover, the latency of
ADCS is only 1 in the two cores. ADCS is a very cheap instruction.

On the Neoverse V1, MUL and UMULH are much faster for the reasons which were
explained before. And MUL and UMULH have the same execution time. The situation
was clearly improved in the V1.

However... when we intersperse ADCS between MUL and/or UMULH, the time per
instruction almost DOUBLES (0.248 instead of 0.144). The interspersed ADCS
use an independent pipeline and are very cheap. Nonetheless, the mean time
per instruction in the sequence doubles.

However, if we replace ADCS with ADD (no use of carry), the mean time per
instruction drops to 0.092 (compared to 0.248 with ADCS and 0.144 with MUL).

Consequently, it is safe to assume that the usage of the carry on addition
(and maybe other instructions, still to be tested) has a nefarious impact
on the instruction throughput, even though the other instructions execute
on a distinct pipeline and do not use the carry.

The impact on the real code sequence in OpenSSL is slightly less important
but still by 30%. This sequence is likely executed less often than the sequence
of MUL UMULH. Thus, the macroscopic effect on RSA is less noticeable.
