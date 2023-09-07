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
"""

# Reference test description.
out_reference_test = """
The "reference test" is a set of basic integer operations which are typically used in
cryptography. There is no floating point operation. This test serves two purposes:
- The absolute time of the reference test is a comparision between processors.
- Each cryptographic algorithm is compared to the reference test on the same machine.
"""

# Description texts.
out_execution_time = """
Execution time in microseconds (the lower, the better):

"""
out_score = """
Relative performance score (the lower, the better):

"""

# Output body text.
out_body = ''

# Full name of cryptographic libraries: table columns.
lib_names = {
    'openssl': 'OpenSSL',
    'mbedtls': 'MbedTLS',
    'gnutls': 'GnuTLS',
    'nettle': 'Nettle',
    'tomcrypt': 'Libtomcrypt',
    'tomcrypt-gmp': 'Libtomcrypt / GMP',
    'arm64': 'Arm64 accel'
}

# Full names of cryptographic algorithms: level 2 sections
crypto_algo_names = {
    'rsa-2048': 'RSA-2048',
    'rsa-4096': 'RSA-4096',
    'aes-128': 'AES-128-CBC',
    'aes-256': 'AES-256-CBC'
}

# Full names of cryptographic operations: level 3 sections.
crypto_oper_names = {
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

# Full names of math algorithms: level 2 sections
math_algo_names = {
    'mod-2048': '2048-bit modular arithmetic'
}

# Names of mathematical categories: level 3 sections, and names of operations: table columns.
math_names = {
    'basic operations': {
        'add': 'Add',
        'mul': 'Multiply',
        'mul-montgomery': 'Multiply<br/>(Montgomery)',
        'mul-reciprocal': 'Multiply<br/>(reciprocal)',
        'sqr': 'Square',
        'div-reciprocal': 'Divide<br/>(reciprocal)',
        'inv': 'Inverse',
        'sqrt': 'Square<br/>Root'
    },
    'exponentiations on public (short) exponents': {
        'exp-public': 'Exponent',
        'exp-public-montgomery': 'Exponent<br/>(Montgomery)',
        'exp-public-montgomery-word': 'Exponent<br/>(Montgomery)<br/>(word)',
        'exp-public-reciprocal': 'Exponent<br/>(reciprocal)',
        'exp-public-simple': 'Exponent<br/>(simple)',
    },
    'exponentiations on private (large) exponents': {
        'exp-private': 'Exponent',
        'exp-private-montgomery': 'Exponent<br/>(Montgomery)',
        'exp-private-montgomery-word': 'Exponent<br/>(Montgomery)<br/>(word)',
        'exp-private-reciprocal': 'Exponent<br/>(reciprocal)',
        'exp-private-simple': 'Exponent<br/>(simple)'
    }
}

#----------------------------------------------------------------------------

# Get/set in nested dictionaries without throwing exceptions.
# Return None if non existent. Create intermediate levels if necessary.

def get_nested(obj, key_list):
    for key in key_list:
        if not type(obj) is dict or key not in obj:
            return None
        obj = obj[key]
    return obj

def set_nested(obj, key_list, value):
    for key in key_list[:-1]:
        if key not in obj:
            obj[key] = {}
        obj = obj[key]
    obj[key_list[-1]] = value

#----------------------------------------------------------------------------

# Definition of a line in a report file.
#
# Examples:
#   system: os: Linux
#   reference: iterations=598: usec-total=5,017,580: usec/iter=8,391
#   openssl: rsa-2048: verify: iterations=198,637: usec-total=4,999,943: usec/iter=25: score=3.0
#   openssl: rsa-2048: sign/verify-ratio=38
#   math: mod-2048: mul: iterations=841,842: usec-total=4,999,525: usec/iter=5.9: score=0.71

class report_line:
    def __init__(self, text):
        # Public fields
        self.system_item = ''
        self.system_value = ''
        self.library = ''
        self.algo = ''
        self.oper = ''
        self.ratio = -1.0
        self.utime = -1.0 # microseconds per operation
        self.score = -1.0
        # Analyze the line.
        fields = [s.strip() for s in text.split(':')] + ['', '', '', '']
        if fields[0] == 'system':
            self.system_item = fields[1]
            self.system_value = text.split(':', 2)[-1].strip()
        else:
            self.library = fields[0]
            if self.library == 'reference':
                self.algo = 'reference'
                self.oper = 'reference'
                start_index = 1
            else:
                self.algo = fields[1]
                f2 = [s.strip() for s in fields[2].split('=',1)]
                self.oper = f2[0]
                if self.oper.endswith('-ratio') and len(f2) > 1:
                    self.ratio = float(f2[1])
                start_index = 3
            iterations = 0
            utotal = 0
            for pair in [f.split('=') for f in fields[start_index:]]:
                name = pair[0].strip()
                value = '' if len(pair) < 2 else pair[1].strip().replace(',', '')
                if name == 'score':
                    self.score = float(value)
                elif name == 'usec/iter':
                    self.utime = float(value)
                elif name == 'iterations':
                    iterations = int(value)
                elif name == 'usec-total':
                    utotal = int(value)
            if iterations > 0:
                self.utime = utotal / iterations

#----------------------------------------------------------------------------

# Read a report file and return a structure as follow:
# {
#   'name': 'filename',
#   'cpu': 'cpuname';
#   'os': 'osname';
#   library: { algo: { oper: {'score': nn, 'utime': nn, 'ratio': nn}, ...}, ...}, ...
# }

def read_file(filename):
    fd = {'name': filename, 'cpu':'', 'os':''}
    with open(filename, 'r') as input:
        for line in input:
            rep = report_line(line)
            if rep.system_item == 'cpu' and rep.system_value != '' and fd['cpu'] == '':
                fd['cpu'] = rep.system_value
            elif rep.system_item == 'cpu-index' and rep.system_value != '':
                fd['cpu'] = rep.system_value
            elif rep.system_item == 'os' and rep.system_value != '':
                fd['os'] = rep.system_value
            else:
                if rep.ratio >= 0.0:
                    set_nested(fd, [rep.library, rep.algo, rep.oper, 'ratio'], rep.ratio)
                if rep.utime >= 0.0:
                    set_nested(fd, [rep.library, rep.algo, rep.oper, 'utime'], rep.utime)
                if rep.score >= 0.0:
                    set_nested(fd, [rep.library, rep.algo, rep.oper, 'score'], rep.score)
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

    # Check if the table is empty.
    def empty(self):
        return self.__empty

    # Add a new line in the table.
    def add_line(self, cpu, os):
        if self.__open:
            self.__lines.append([cpu, os] + ['' for i in range(len(self.__lines[0]) - 2)])

    # Add a data cell in the last line.
    # The column is an index into self.column_names.
    # If value is None, it is ignored.
    def set_data(self, col, value):
        if value is not None and self.__open and len(self.__lines) > 2 and col in self.__column_names:
            if type(value) is not float:
                value = str(value)
            elif value < 0.1:
                value = '%.3f' % value
            elif value < 1.0:
                value = '%.2f' % value
            elif value < 20.0:
                value = '%.1f' % value
            else:
                value = '%d' % value
            self.__lines[-1][2 + list(self.__column_names).index(col)] = value
            self.__empty = False

    # Fill all data from a list of file data.
    # One of the 4 lists must have more than one element and will be used as column index.
    def fill_data(self, filedata, library, algo, oper, index):
        # print('@@@ lib:%s, algo:%s, oper:%s, index: %s\n\n' % (library, algo, oper, index))
        col_algo = len(algo) > 1
        col_oper = len(oper) > 1
        col_index = len(index) > 1
        for fd in filedata:
            self.add_line(fd['cpu'], fd['os'])
            for l in library:
                for a in algo:
                    for o in oper:
                        for i in index:
                            value = get_nested(fd, [l, a, o, i])
                            if col_index:
                                self.set_data(i, value)
                            elif col_oper:
                                self.set_data(o, value)
                            elif col_algo:
                                self.set_data(a, value)
                            else:
                                self.set_data(l, value)

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
# Start with the reference test.
tab = table({'reference': 'Reference test'})
tab.fill_data(filedata, ['reference'], ['reference'], ['reference'], ['utime'])
add_section_header(2, 'Reference test')
out_body += out_reference_test
out_body += out_execution_time
out_body += tab.to_markdown()

# Then, all cryptographic algorithms.
for algo in crypto_algo_names:
    add_section_header(2, crypto_algo_names[algo])
    for oper in crypto_oper_names:
        tab_utime = table(lib_names)
        tab_utime.fill_data(filedata, lib_names, [algo], [oper], ['utime'])
        tab_score = table(lib_names)
        tab_score.fill_data(filedata, lib_names, [algo], [oper], ['score'])
        tab_ratio = table(lib_names)
        tab_ratio.fill_data(filedata, lib_names, [algo], [oper], ['ratio'])
        if not tab_utime.empty() or not tab_score.empty() or not tab_ratio.empty():
            add_section_header(3, crypto_algo_names[algo] + ' ' + crypto_oper_names[oper])
        if not tab_utime.empty():
            out_body += out_execution_time
            out_body += tab_utime.to_markdown()
        if not tab_score.empty():
            out_body += out_score
            out_body += tab_score.to_markdown()
        if not tab_ratio.empty():
            out_body += '\n'
            out_body += tab_ratio.to_markdown()

# Then, all math operations.
for algo in math_algo_names:
    add_section_header(2, math_algo_names[algo])
    for math_type in math_names:
        tab_utime = table(math_names[math_type])
        tab_utime.fill_data(filedata, ['math'], [algo], math_names[math_type], ['utime'])
        tab_score = table(math_names[math_type])
        tab_score.fill_data(filedata, ['math'], [algo], math_names[math_type], ['score'])
        if not tab_utime.empty() or not tab_score.empty():
            add_section_header(3, math_algo_names[algo] + ' ' + math_type)
        if not tab_utime.empty():
            out_body += out_execution_time
            out_body += tab_utime.to_markdown()
        if not tab_score.empty():
            out_body += out_score
            out_body += tab_score.to_markdown()

# Finally, output the markdown file.
print(out_header)
print(out_intro)
print(out_body)
