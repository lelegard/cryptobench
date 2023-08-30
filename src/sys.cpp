//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Common system utilities.
//----------------------------------------------------------------------------

#include "sys.h"
#include <locale.h>
#include <sys/resource.h>
#include <unistd.h>
#if defined(__APPLE__)
    #include <libproc.h>
#endif

//----------------------------------------------------------------------------
// Some useful common system initialization.
//----------------------------------------------------------------------------

void sys::init()
{
    // Make sure printf knows how to format integers.
    ::setlocale(LC_ALL, "en_US.UTF-8");
}

//----------------------------------------------------------------------------
// Printf-like formatting.
//----------------------------------------------------------------------------

std::string sys::format(const char* fmt, ...)
{
    va_list ap;

    // Get required output size.
    va_start(ap, fmt);
    int len = ::vsnprintf(nullptr, 0, fmt, ap);
    va_end(ap);

    if (len < 0) {
        return std::string(); // error
    }

    // Actual formatting.
    std::string buf(len + 1, '\0');
    va_start(ap, fmt);
    len = ::vsnprintf(&buf[0], buf.size(), fmt, ap);
    va_end(ap);

    buf.resize(std::max(0, len));
    return buf;
}

//----------------------------------------------------------------------------
// Integer division with rounding to nearest.
//----------------------------------------------------------------------------

int64_t sys::div64(int64_t num, int64_t den)
{
    return ((num < 0) == (den < 0)) ? ((num + den / 2) / den) : ((num - den / 2) / den);
}

//----------------------------------------------------------------------------
// Get the file path of the current executable.
//----------------------------------------------------------------------------

fs::path sys::exec_file()
{
    // Don't use fatal() here, it would recurse forever.
#if defined(__linux__)
    // Linux implementation.
    return fs::read_symlink("/proc/self/exe");
#elif defined(__APPLE__)
    // macOS implementation.
    int length = 0;
    char name[PROC_PIDPATHINFO_MAXSIZE];
    if ((length = ::proc_pidpath(::getpid(), name, sizeof(name))) < 0) {
        ::perror("proc_pidpath");
        std::exit(EXIT_FAILURE);
    }
    return fs::path(name);
#else
    #error "exec_file() not implemented on this O/S"
#endif
}

//----------------------------------------------------------------------------
// Handle the result of strerror_r().
// Depending on GNU vs. POSIX, strerror_r() returns an int or a char*.
// There are two short functions to handle the strerror_r result.
// The C++ compiler will automatically invoke the right one.
//----------------------------------------------------------------------------

// POSIX version, strerror_r() returns an int, leave result unmodified.
inline void handle_strerror_r(bool& found, char*& result, int strerror_t_ret)
{
    found = strerror_t_ret == 0; // success
}

// GNU version, strerror_r() returns char*, not necessarily in buffer.
inline void handle_strerror_r(bool& found, char*& result, char* strerror_t_ret)
{
    result = strerror_t_ret; // actual message
    found = result != nullptr;
}

//----------------------------------------------------------------------------
// Transform an errno value into an error message string.
//----------------------------------------------------------------------------

std::string sys::error_string(int err)
{
    std::string message(1024, '\0');
    char* result = &message[0];
    bool found = false;
    handle_strerror_r(found, result, ::strerror_r(err, &message[0], message.size()));

    if (found) {
        // Make sure message is nul-terminated (POSIX variant).
        message.back() = '\0';
        // Remove trailing newlines (if any). May not be inside message string (GNU variant).
        size_t len = ::strlen(result);
        while (len > 0 && ::isspace(result[len-1])) {
            len--;
        }
        return std::string(result, len);
    }
    else {
        // Message is not found.
        return format("system error %d (0x%X)", err, err);
    }
}

//----------------------------------------------------------------------------
// Display an error message and exit.
//----------------------------------------------------------------------------

void sys::fatal(const std::string& message, int err)
{
    std::cerr << std::string(exec_file().stem()) << ": " << message;
    if (err != 0) {
        std::cerr << ": " << error_string(err);
    }
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
}

//----------------------------------------------------------------------------
// Get current CPU time resource usage in microseconds.
//----------------------------------------------------------------------------

int64_t sys::cpu_time()
{
    ::rusage ru;
    if (::getrusage(RUSAGE_SELF, &ru) < 0) {
        fatal("getrusage", errno);
    }
    return ((int64_t)(ru.ru_utime.tv_sec) * USECPERSEC) + ru.ru_utime.tv_usec +
           ((int64_t)(ru.ru_stime.tv_sec) * USECPERSEC) + ru.ru_stime.tv_usec;
}

//----------------------------------------------------------------------------
// Copy a memory area into a byte vector.
//----------------------------------------------------------------------------

void sys::copy(bytes_t& dest, const void* src, size_t size)
{
    if (src == nullptr || size == 0) {
        dest.clear();
    }
    else {
        dest.resize(size);
        ::memcpy(&dest[0], src, size);
    }
}

//----------------------------------------------------------------------------
// Load a binary file. Return malloc'ated data. Fail on error.
//----------------------------------------------------------------------------

void sys::load_file(bytes_t& data, const std::string& filename)
{
    // Open the input file.
    std::ifstream strm(filename.c_str(), std::ios::in | std::ios::binary);
    if (!strm.is_open()) {
        fatal("error opening " + filename);
    }

    // Get the file size.
    strm.seekg(0, std::ios::end);
    const std::ios::pos_type file_size = strm.tellg();
    strm.seekg(0, std::ios::beg);
    if (strm.fail()) {
        fatal("error getting file size of " + filename);
    }
    data.resize(size_t(file_size));

    // Read complete file.
    strm.read(reinterpret_cast<char*>(data.data()), std::streamsize(data.size()));
    if (strm.gcount() < std::streamsize(data.size())) {
        fatal("error reading " + filename);
    }
    strm.close();
}

//----------------------------------------------------------------------------
// Get the size in bits of a multi-precision number in MSB format.
//----------------------------------------------------------------------------

size_t sys::large_number_bits(const uint8_t* num, size_t num_bytes)
{
    size_t bits = num == nullptr ? 0 : 8 * num_bytes;
    uint8_t mask = 0x80;
    while (bits > 0 && (*num & mask) == 0) {
        bits--;
        if ((mask >>= 1) == 0) {
            mask = 0x80;
            num++;
        }
    }
    return bits;
}

//----------------------------------------------------------------------------
// Base64 utilities
//----------------------------------------------------------------------------

namespace {

    // Convert a Base64 character into a 0-63 integer. Fail on error.
    uint8_t base64_to_integer(char c)
    {
        // Base64 alphabet: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
        if (c >= 'A' && c <= 'Z') {
            return c - 'A';
        }
        else if (c >= 'a' && c <= 'z') {
            return c - 'a' + 26;
        }
        else if (c >= '0' && c <= '9') {
            return c - '0' + 52;
        }
        else if (c == '+') {
            return 62;
        }
        else if (c == '/') {
            return 63;
        }
        else {
            sys::fatal(sys::format("invalid base64 encoding, got character %d ('%c')", int(c), c));
            return 0; // unreacheable
        }
    }

    // Convert a buffer of up to 4 characters to binary data.
    void base64_update(bytes_t& data, const char* buf, size_t buf_size)
    {
        if (buf_size >= 2) {
            const uint8_t c0 = base64_to_integer(buf[0]);
            const uint8_t c1 = base64_to_integer(buf[1]);
            data.push_back((c0 << 2) | (c1 >> 4));
            if (buf_size >= 3 && buf[2] != '=') {
                const uint8_t c2 = base64_to_integer(buf[2]);
                data.push_back((c1 << 4) | (c2 >> 2));
                if (buf_size >= 4 && buf[3] != '=') {
                    const uint8_t c3 = base64_to_integer(buf[3]);
                    data.push_back((c2 << 6) | c3);
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
// Decode 64 data. Fail on error.
//----------------------------------------------------------------------------

void sys::base64_decode(bytes_t& data, const void* void_b64, size_t b64_size)
{
    data.clear();
    if (void_b64 == nullptr || b64_size == 0) {
        return;
    }
    data.reserve((b64_size * 3) / 4);

    char buf[4];
    size_t buf_size = 0;
    const char* b64 = reinterpret_cast<const char*>(void_b64);

    while (b64_size > 0) {
        while (buf_size < sizeof(buf) && b64_size > 0) {
            if (!std::isspace(*b64)) {
                buf[buf_size++] = *b64;
            }
            b64++;
            b64_size--;
        }
        base64_update(data, buf, buf_size);
        buf_size = 0;
    }
    base64_update(data, buf, buf_size);
}

//----------------------------------------------------------------------------
// Load a PEM file and decode it as DER. Fail on error.
//----------------------------------------------------------------------------

void sys::load_pem_file_as_der(bytes_t& data, const std::string& filename)
{
    // Read the PEM file.
    bytes_t pem;
    load_file(pem, filename);

    // Ignore the "-----" headers in the PEM file.
    uint8_t* b64 = pem.data();
    size_t b64_size = pem.size();
    if (b64_size > 5 && b64[0] == '-' && b64[1] == '-') {
        while (b64_size > 0 && *b64 != '\n') {
            b64++;
            b64_size--;
        }
        while (b64_size > 0 && std::isspace(*b64)) {
            b64++;
            b64_size--;
        }
    }
    while (b64_size > 0 && std::isspace(b64[b64_size - 1])) {
        b64_size--;
    }
    if (b64_size > 5 && b64[b64_size - 1] == '-' && b64[b64_size - 2] == '-') {
        while (b64_size > 0 && b64[b64_size - 1] != '\n') {
            b64_size--;
        }
        while (b64_size > 0 && isspace(b64[b64_size - 1])) {
            b64_size--;
        }
    }

    // Decode the Base64 encoding to get the DER formatting.
    base64_decode(data, b64, b64_size);
}
