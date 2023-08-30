# Cryptographic libraries comparative benchmarks

Prerequisites:
- Ubuntu/Debian: `sudo apt install libssl-dev libmbedtls-dev libgnutls28-dev libtomcrypt-dev libtommath-dev libgmp-dev`
- Fedora/Redhat: `sudo dnf install openssl-devel mbedtls-devel gnutls-devel libtomcrypt-devel libtommath-devel gmp-devel`
- macOS: `brew install openssl mbedtls gnutls libtomcrypt libtommath gmp`

Note: On Ubuntu, the package `libgnutls28-dev` is probably misnamed.
It is the gnutls development environment for the latest versions of gnutls,
3.7.8 as of this writing. It is not (or no longer) gnutls version 2.8.
