#!/usr/bin/env python
#----------------------------------------------------------------------------
# CryptoBench - Copyright (c) 2023, Thierry Lelegard
# BSD 2-Clause License, see LICENSE file.
# Analyze report files and produce an analysis in markdown format.
#----------------------------------------------------------------------------

import os, sys, string

# Output header.
out_header = """# Comparative benchmarks results

**Contents:**
* [Results overview](#results-overview)
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

# Output body text.
out_body = ''

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
    'rsa-2048': {'name': 'RSA-2048', "math": False},
    'rsa-4096': {'name': 'RSA-4096', "math": False},
    'aes-128':  {'name': 'AES-128-CBC', "math": False},
    'aes-256':  {'name': 'AES-256-CBC', "math": False},
    'mod-2048': {'name': '2048-bit modular arithmetic', "math": True}
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

# Names of mathematical operations, in horizontal order of the math table.
math_names = {
    'add': 'Add',
    'mul': 'Mult',
    'mul-montgomery': 'Mult<br/>(Montgomery)',
    'mul-reciprocal': 'Mult<br/>(reciprocal)',
    'sqr': 'Square',
    'div-reciprocal': 'Div<br/>(reciprocal)',
    'inv': 'Inverse',
    'sqrt': 'Square<br/>Root',
    'exp-public': 'Exponent<br/>(public)',
    'exp-public-montgomery': 'Exponent<br/>(public)<br/>(Montgomery)',
    'exp-public-montgomery-word': 'Exponent<br/>(public)<br/>(Montgomery)<br/>(word)',
    'exp-public-reciprocal': 'Exponent<br/>(public)<br/>(reciprocal)',
    'exp-public-simple': 'Exponent<br/>(public)<br/>(simple)',
    'exp-private': 'Exponent<br/>(private)',
    'exp-private-montgomery': 'Exponent<br/>(private)<br/>(Montgomery)',
    'exp-private-montgomery-word': 'Exponent<br/>(private)<br/>(Montgomery)<br/>(word)',
    'exp-private-reciprocal': 'Exponent<br/>(private)<br/>(reciprocal)',
    'exp-private-simple': 'Exponent<br/>(private)<br/>(simple)',
}

#----------------------------------------------------------------------------

# Read a report file and return a structure as follow:
# {
#   'name': 'filename',
#   'cpu': 'cpuname';
#   'os': 'osname';
#   'lib': {
#     'openssl': {
#       'rsa-2048': {'encrypt': '5.9', 'decrypt': '160', ...},
#       'rsa-4096': {'encrypt': '17.9', 'decrypt': '1058', ...},
#       ...
#      },
#      ....
#   },
#   'math': {
#     'mod-2048': {'add': '0.037', 'mul': '0.79', ...},
#   }
# }
def read_file(filename):
    fd = {'name': filename, 'cpu':'', 'os':'', 'lib':{}, 'math':{}}
    with open(filename, 'r') as input:
        for line in input:
            # Split the input line into a list of tuples (tag, value).
            # Make sure that there are at least 3 tuples in the line, possibly empty.
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
            # Get the tags of the three first fields.
            lib = fields[0][0]
            algo = fields[1][0]
            oper = fields[2][0]
            # First, process system description.
            if lib == 'system' and algo == 'cpu' and oper != '' and fd['cpu'] == '':
                fd['cpu'] = line.split(':', 2)[2].strip()
            elif lib == 'system' and algo == 'cpu-index' and oper != '':
                fd['cpu'] = line.split(':', 2)[2].strip()
            elif lib == 'system' and algo == 'system' and oper != '':
                fd['os'] = oper
            elif lib == 'math' and algo in algo_names and algo_names[algo]['math'] and oper in math_names and fields[-1][0] == 'score':
                # This is a "math:" line.
                if algo not in fd['math']:
                    fd['math'][algo] = {}
                fd['math'][algo][oper] = fields[-1][1]
            elif lib in lib_names and algo in algo_names and not algo_names[algo]['math'] and oper in oper_names:
                # Now, this is a standard line "lib: algo: oper: ..."
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
    return fd

#----------------------------------------------------------------------------

# Definition of a markdown table.
# The first two columns are 'CPU' and 'OS'.
# Other columns are named from column_names.
class table:

    # Constructor.
    def __init__(self, column_names):
        self.__column_names = column_names
        # Build header lines.
        self.__lines = [['CPU', 'OS'], ['-', '-']]
        for col in self.__column_names:
            self.__lines[0].append(self.__column_names[col])
            self.__lines[1].append(':')
        # The table can be modified:
        self.__open = True
        self.__markdown = ''
        self.__empty = True

    # Add a new line in the table.
    def add_line(self, cpu, os):
        if self.__open:
            self.__lines.append([cpu, os] + ['' for i in range(len(self.__lines[0]) - 2)])

    # Add a data cell in the last line.
    # The column is an index into self.column_names.
    def set_data(self, col, value):
        if self.__open and len(self.__lines) > 2 and col in self.__column_names:
            self.__lines[-1][2 + list(self.__column_names).index(col)] = value
            self.__empty = False

    # Check if the table is empty.
    def empty(self):
        return self.__empty

    # Get a markdown printable representation.
    # The table is closed and cannot be modified.
    def to_markdown(self):
        self.__open = False
        if self.__markdown == '':
            # Remove empty lines in the table.
            lin = 2
            while lin < len(self.__lines):
                if ''.join(s for s in self.__lines[lin][2:]) == '':
                    del(self.__lines[lin])
                else:
                    lin += 1
            # Remove empty columns in the table.
            col = 2
            while col < len(self.__lines[0]):
                if ''.join(self.__lines[lin][col] for lin in range(2,len(self.__lines))) == '':
                    # Remove that column
                    for lin in range(len(self.__lines)):
                        del(self.__lines[lin][col])
                else:
                    col = col + 1
            # Justify all columns with constant width.
            for col in range(len(self.__lines[0])):
                width = 0
                for lin in range(len(self.__lines)):
                    # If the cell contains a "<br/>", use it as width.
                    line = self.__lines[lin][col]
                    br = '<br/>'
                    width = max(width, line.index(br) if br in line else len(line))
                for lin in range(len(self.__lines)):
                    self.__lines[lin][col] = self.__lines[lin][col].ljust(width)
            # Fix header line.
            for col in range(len(self.__lines[1])):
                self.__lines[1][col] = self.__lines[1][col][0] + ('-' * (len(self.__lines[1][col]) - 2)) + self.__lines[1][col][0]
            # Generate the markdown.
            for line in self.__lines:
                self.__markdown += '| ' + ' | '.join(line) + ' |\n'
        return self.__markdown

#----------------------------------------------------------------------------

# Add a section header line in out_header (ToC) and out_body.
def add_section_header(level, text):
    global out_header, out_body
    anchor = ''.join([c for c in text.lower().replace(' ', '-') if c in string.ascii_lowercase + string.digits + '-'])
    out_body += '\n%s %s\n' % ((level * '#'), text)
    out_header += '%s* [%s](#%s)\n' % (((2 * (level - 2)) * ' '), text, anchor)

#----------------------------------------------------------------------------

# Main code: Get input files from command line.
if len(sys.argv) < 2:
    print('usage: %s report-file ...' % os.path.basename(__file__), file=sys.stderr)
    exit(1)

# Analyze all input files.
filedata = []
for filename in sys.argv[1:]:
    filedata.append(read_file(filename))

# Build output body text and table of contents.
for algo in algo_names:
    add_section_header(2, algo_names[algo]['name'])
    if algo_names[algo]['math']:
        # Build a math layout: each column is an operation.
        tab = table(math_names)
        for fd in filedata:
            if algo in fd['math']:
                tab.add_line(fd['cpu'], fd['os'])
                for oper in fd['math'][algo]:
                    tab.set_data(oper, fd['math'][algo][oper])
        # Output the table for this algo/operation.
        if not tab.empty():
            out_body += '\n'
            out_body += tab.to_markdown()
    else:
        for oper in oper_names:
            # Build a crypto layout for that algo/oper.
            tab = table(lib_names)
            for fd in filedata:
                tab.add_line(fd['cpu'], fd['os'])
                for lib in fd['lib']:
                    if algo in fd['lib'][lib] and oper in fd['lib'][lib][algo]:
                        tab.set_data(lib, fd['lib'][lib][algo][oper])
            # Output the table for this algo/operation.
            if not tab.empty():
                add_section_header(3, algo_names[algo]['name'] + ' ' + oper_names[oper])
                out_body += '\n'
                out_body += tab.to_markdown()

# Finally, output the markdown file.
print(out_header)
print(out_intro)
print(out_body)
