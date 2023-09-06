//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Command line options for cryptobench app.
//----------------------------------------------------------------------------

#pragma once
#include "sys.h"

class options
{
public:
    // Constructor.
    options() = delete;
    options(int argc, char* argv[]);

    // Display help on stderr and exit.
    void usage() const;

    // Print a fatal error and exit.
    void fatal(const std::string& message) const;

    std::string appname; // executable name part
    std::string private_key_2048;
    std::string public_key_2048;
    std::string private_key_4096;
    std::string public_key_4096;
    int64_t     min_usec = 2 * sys::USECPERSEC; // minimum run time per test, in microseconds
    int64_t     min_iter = 10; // minimum number of iterations per test
    size_t      aes_data_size = 1000000;
    bool        reference = false;
    bool        all_algos = true;
    bool        aes = false;
    bool        rsa = false;
    bool        math = false;
    bool        all_libs = true;
    bool        openssl = false;
    bool        mbedtls = false;
    bool        gnutls = false;
    bool        nettle = false;
    bool        tomcrypt = false;
    bool        tomcrypt_gmp = false;
    bool        arm64 = false;
};
