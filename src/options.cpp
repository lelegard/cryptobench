//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Command line options for cryptobench app.
//----------------------------------------------------------------------------

#include "options.h"

//----------------------------------------------------------------------------
// Display help on stderr and exit.
//----------------------------------------------------------------------------

void options::usage() const
{
    std::cerr << std::endl
              << "Command line options:" << std::endl
              << "  -h, --help : display this help text" << std::endl
              << "  -m sec, --min-time sec : minimum run time of each test in seconds" << std::endl
              << "  --min-iterations count : minimum number of iteration for each test" << std::endl
              << "  --aes-data-size count : data size for AES-CBC tests" << std::endl
              << "  -r, --reference : run reference benchmark only" << std::endl
              << std::endl
              << "Algorithms selection (all by default):" << std::endl
              << "  --aes, --rsa" << std::endl
              << std::endl
              << "Crytographic libraries selection (all by default):" << std::endl
              << "  --gnutls, --mbedtls, --openssl, --nettle, --tomcrypt, --tomcrypt-gmp" << std::endl
              << std::endl;
    std::exit(EXIT_FAILURE);
}

//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

options::options(int argc, char* argv[])
{
    // Key files are currently hardcoded to same directory as executable file.
    const std::string exec_dir(sys::exec_file().parent_path());
    private_key_2048 = exec_dir + "/priv2048.pem";
    public_key_2048  = exec_dir + "/pub2048.pem";
    private_key_4096 = exec_dir + "/priv4096.pem";
    public_key_4096  = exec_dir + "/pub4096.pem";

    if (argc > 0) {
        appname = fs::path(argv[0]).stem();
        for (int i = 1; i < argc; ++i) {
            const std::string arg(argv[i]);
            if (arg == "--help" || arg == "-h") {
                usage();
            }
            else if ((arg == "-m" || arg == "--min-time") && i+1 < argc) {
                min_usec = std::atoll(argv[++i]) * sys::USECPERSEC;
            }
            else if (arg == "--min-iterations" && i+1 < argc) {
                min_iterations = std::atoll(argv[++i]);
            }
            else if (arg == "--aes-data-size" && i+1 < argc) {
                aes_data_size = std::atoll(argv[++i]);
            }
            else if (arg == "-r" || arg == "--reference") {
                all_algos = all_libs = false;
                reference = true;
            }
            else if (arg == "--aes") {
                all_algos = false;
                aes = true;
            }
            else if (arg == "--rsa") {
                all_algos = false;
                rsa = true;
            }
            else if (arg == "--gnutls") {
                all_libs = false;
                gnutls = true;
            }
            else if (arg == "--mbedtls") {
                all_libs = false;
                mbedtls = true;
            }
            else if (arg == "--nettle") {
                all_libs = false;
                nettle = true;
            }
            else if (arg == "--openssl") {
                all_libs = false;
                openssl = true;
            }
            else if (arg == "--tomcrypt") {
                all_libs = false;
                tomcrypt = true;
            }
            else if (arg == "--tomcrypt-gmp") {
                all_libs = false;
                tomcrypt_gmp = true;
            }
            else {
                fatal("invalid option '" + arg + "', try --help");
            }
        }
    }

    if (all_libs && all_algos) {
        reference = true;
    }
    if (all_algos) {
        aes = rsa = true;
    }
    if (all_libs) {
        gnutls = mbedtls = nettle = openssl = tomcrypt = tomcrypt_gmp = true;
    }
}

//----------------------------------------------------------------------------
// Print a fatal error and exit.
//----------------------------------------------------------------------------

void options::fatal(const std::string& message) const
{
    std::cerr << appname << ": " << message << std::endl;
    std::exit(EXIT_FAILURE);
}
