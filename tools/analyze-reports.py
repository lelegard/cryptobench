#!/usr/bin/env python
#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Analyze report files and produce an analysis in markdown format.
#----------------------------------------------------------------------------

# Output header.
out_header = """# Comparative benchmarks results

**Contents:**
"""

# Output introduction text.
out_intro = """## Results overview

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
"""

# Full name of libraries from index in report files, in order of appearance in columns.
lib_names = {
    'openssl': 'OpenSSL',
    'mbedtls': 'MbedTLS',
    'gnutls': 'GnuTLS',
    'nettle': 'Nettle',
    'tomcrypt': 'Libtomcrypt',
    'tomcrypt-gmp': 'Libtomcrypt / GMP',
    'arm64': 'Arm64 accel'
}

# Full names of algorithms from index in report files, in order of level-1 sections.
algo_names = {
    'rsa-2048': 'RSA-2048',
    'rsa-4096': 'RSA-4096',
    'aes-128': 'AES-128-CBC',
    'aes-256': 'AES-256-CBC'
}

# Operation names from index in report files, in order of level-2 sections.
oper_names = {
    'encrypt': 'encryption', # RSA + AES
    'decrypt': 'decryption', # RSA + AES
    'decrypt/encrypt-ratio': 'decryption/encryption ratio', # RSA
    'encrypt/decrypt-ratio': 'encryption/decryption ratio', # AES
    'encrypt-rekey': 'encryption with rekeying',
    'decrypt-rekey': 'decryption with rekeying',
    'decrypt-rekey/encrypt-rekey-ratio': 'decryption/encryption ratio, with rekeying',
    'sign': 'signature',
    'verify': 'verification',
    'sign/verify-ratio': 'signature/verification ratio',
    'sign-rekey': 'signature with rekeying',
    'verify-rekey': 'verification with rekeying',
    'sign-rekey/verify-rekey-ratio': 'signature/verification ratio, with rekeying',
    'load-public': 'parse public key',
    'load-private': 'parse private key'
}

#----------------------------------------------------------------------------

import os, sys

# Get input files from command line.
if len(sys.argv) < 2:
    print('usage: %s report-file ...' % os.path.basename(__file__), file=sys.stderr)
    exit(1)

# Split a line into a list of tuples (tag, value).
# Make sure that there are at least 3 tuples in the line, possibly empty.
def split_input_line(line):
    fields = []
    for f in line.split(':'):
        subfields = []
        for sf in f.split('=', 1):
            subfields.append(sf.strip())
        while len(subfields) < 2:
            subfields.append('')
        fields.append(subfields)
    while len(fields) < 3:
        fields.append(('', ''))
    return fields

# Build a list of results, one element per file:
# filedata = [
#   {
#       'name': 'filename',
#       'cpu': 'cpuname';
#       'lib': {
#           'openssl': {
#               'rsa-2048': {'encrypt': '5.9', 'decrypt': '160', ...},
#               'rsa-4096': {'encrypt': '17.9', 'decrypt': '1058', ...},
#               ...
#            },
#            ....
#        }
#   },
#   ...
# ]
filedata = []
for filename in sys.argv[1:]:
    with open(filename, 'r') as input:
        fd = {'name': filename, 'cpu':'', 'lib':{}}
        for line in input:
            fields = split_input_line(line)
            lib = fields[0][0]
            algo = fields[1][0]
            oper = fields[2][0]
            if lib == 'system' and algo == 'cpu' and oper != '' and fd['cpu'] == '':
                fd['cpu'] = line.split(':', 2)[2].strip()
            elif lib == 'system' and algo == 'cpu-index' and oper != '':
                fd['cpu'] = line.split(':', 2)[2].strip()
            elif lib in lib_names and algo in algo_names and oper in oper_names:
                if fields[-1][0] == 'score':
                    value = fields[-1][1]
                elif oper.endswith('-ratio'):
                    value = fields[2][1]
                else:
                    value = ''
                if lib not in fd['lib']:
                    fd['lib'][lib] = {}
                if algo not in fd['lib'][lib]:
                    fd['lib'][lib][algo] = {}
                fd['lib'][lib][algo][oper] = value
        filedata.append(fd)
