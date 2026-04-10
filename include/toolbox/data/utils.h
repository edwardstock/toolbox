/// @file utils.h
/// @brief Low-level byte/hex/number conversion utilities.

#ifndef TOOLBOXPP_UTILS_H
#define TOOLBOXPP_UTILS_H

#include "toolbox/toolbox_config.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <type_traits>
#include <vector>
#include <cstdint>

namespace toolbox {
namespace data {

/// Convert a hex string to a vector of bytes.
/// The string must have even length and contain only hex digits.
/// @param hex input hex string (no "0x" prefix)
/// @return decoded bytes
TOOLBOX_API std::vector<uint8_t> hex_to_bytes(const std::string& hex);

/// Convert a byte array to a hex string.
/// Each byte produces two lowercase hex characters.
/// @param data pointer to the byte array
/// @param len number of bytes
/// @return hex string of length len*2
TOOLBOX_API std::string bytes_to_hex(const uint8_t* data, size_t len);

/// Convert an integral value to a big-endian byte vector.
/// The output vector must already be sized to at least sizeof(NumT).
/// @tparam NumT integral type
/// @param num value to convert
/// @param out output vector (must be pre-sized)
template<typename U, typename NumT = typename std::enable_if<std::is_integral<U>::value>::type>
void num_to_bytes(const NumT num, std::vector<uint8_t>& out) {
    static_assert(std::is_integral<NumT>::value, "Only integral types can be passed");

    size_t sz = sizeof(num);
    for (size_t i = 0; i < sz; i++) {
        out[(out.size() - 1) - i] = (num >> (i * 8));
    }
}

/// Convert an integral value to a big-endian byte vector.
/// @tparam NumT integral type
/// @param num value to convert
/// @return byte vector of size sizeof(NumT)
template<typename NumT>
std::vector<uint8_t> num_to_bytes(NumT num) {
    std::vector<uint8_t> out(sizeof(num));
    num_to_bytes<NumT>(num, out);
    return out;
}

/// Convert an integral value to a big-endian byte vector with a specific output size.
/// @tparam NumT integral type
/// @param num value to convert
/// @param outSize output vector size (must be >= sizeof(NumT))
/// @return byte vector of the requested size
template<typename NumT>
std::vector<uint8_t> num_to_bytes(NumT num, size_t outSize) {
    assert(outSize >= sizeof(num));
    std::vector<uint8_t> out(outSize);
    num_to_bytes<NumT>(num, out);
    return out;
}

/// Convert a char vector to an ASCII string.
TOOLBOX_API std::string to_ascii(const std::vector<char>& input);
/// Convert a uint8_t vector to an ASCII string.
TOOLBOX_API std::string to_ascii(const std::vector<uint8_t>& input);

/// Convert a uint8_t vector to a char vector.
TOOLBOX_API std::vector<char> to_chars(const std::vector<uint8_t>& input);
/// Convert a char vector to a uint8_t vector.
TOOLBOX_API std::vector<uint8_t> to_bytes(const std::vector<char>& input);
/// Convert a raw char pointer and length to a uint8_t vector.
TOOLBOX_API std::vector<uint8_t> to_bytes(const char* data, size_t len);
/// Convert a string to a uint8_t vector (raw bytes, not hex).
TOOLBOX_API std::vector<uint8_t> to_bytes(std::string input);

} // namespace data
} // namespace toolbox

#endif // TOOLBOXPP_UTILS_H
