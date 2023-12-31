//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Common system utilities.
//----------------------------------------------------------------------------

#pragma once
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <limits>
#include <filesystem>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cinttypes>

namespace fs = std::filesystem;

// Generic byte block.
typedef std::vector<uint8_t> bytes_t;

namespace sys {

    // Some useful common system initialization.
    void init();

    // Printf-like formatting.
    std::string format(const char* fmt, ...);

    // Integer division with rounding to nearest.
    int64_t div64(int64_t num, int64_t den);

    // Get the file path of the current executable.
    fs::path exec_file();

    // Transform an errno value into an error message string.
    std::string error_string(int);

    // Display an error message and exit. If errno node is specified, displau error message.
    void fatal(const std::string& message, int err = 0);

    // Get current CPU time resource usage in microseconds.
    int64_t cpu_time();

    // Number of microseconds per second
    constexpr int64_t USECPERSEC = 1000000;

    // Copy a memory area into a byte vector.
    void copy(bytes_t& dest, const void* src, size_t size);

    // Load a binary file. Return malloc'ated data. Fail on error.
    void load_file(bytes_t& data, const std::string& filename);

    // Get the size in bits of a multi-precision number in MSB format.
    size_t large_number_bits(const uint8_t* num, size_t num_bytes);

    // Decode 64 data. Ignore spaces. Fail on error.
    void base64_decode(bytes_t& data, const void* b64, size_t b64_size);
    inline void base64_decode(bytes_t& data, const std::string& b64) { base64_decode(data, b64.data(), b64.size()); }
    inline void base64_decode(bytes_t& data, const bytes_t& b64) { base64_decode(data, b64.data(), b64.size()); }

    // Load a PEM file and decode it as DER. Fail on error.
    void load_pem_file_as_der(bytes_t& data, const std::string& filename);
}
