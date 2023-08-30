# Cryptographic libraries comparative benchmarks

## Prerequisites

- Ubuntu/Debian: `sudo apt install libssl-dev libmbedtls-dev libgnutls28-dev nettle-dev libtomcrypt-dev libtommath-dev libgmp-dev`
- Fedora/Redhat: `sudo dnf install openssl-devel mbedtls-devel gnutls-devel nettle-devel libtomcrypt-devel libtommath-devel gmp-devel`
- macOS: `brew install openssl mbedtls gnutls nettle libtomcrypt libtommath gmp`

Note: On Ubuntu/Debian, the package `libgnutls28-dev` is probably misnamed.
It is the gnutls development environment for the latest versions of gnutls,
3.x.x as of this writing. It is not (or no longer) gnutls version 2.8.

## Bugs

RSA tests are disabled with nettle. There is an issue loading public and private
key files, as created by OpenSSL. The parsing of the DER sequence fails.
To be investigated...
