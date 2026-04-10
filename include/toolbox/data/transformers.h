/// @file transformers.h
/// @brief Converter specializations and base64 helpers for byte containers.

#ifndef TOOLBOX_TRANSFORMERS_H
#define TOOLBOX_TRANSFORMERS_H

#include "base64.h"
#include "bytes_data.h"
#include "utils.h"

#include <string>

namespace toolbox {
namespace data {

/// Encode a byte vector to base64.
inline std::vector<uint8_t> to_base_64(const std::vector<uint8_t>& source) {
    return base64_encode_bytes(source);
}

/// Decode a base64-encoded byte vector.
inline std::vector<uint8_t> from_base_64(const std::vector<uint8_t>& source) {
    return base64_decode_bytes(source);
}

/// Converter: bytes -> std::string (raw copy, not hex).
template<>
template<>
struct basic_data<uint8_t>::converter<std::string> {
    std::string operator()(const basic_data<uint8_t>& source) {
        return {source.begin(), source.end()};
    }
};
using bytes_to_string = basic_data<uint8_t>::converter<std::string>;

/// Converter: bytes -> basic_data<char>.
template<>
template<>
struct basic_data<uint8_t>::converter<basic_data<char>> {
    basic_data<char> operator()(const basic_data<uint8_t>& source) {
        return std::vector<char>(source.begin(), source.end());
    }
};
using bytes_to_chars = basic_data<uint8_t>::converter<basic_data<char>>;

} // namespace data
} // namespace toolbox

#endif // TOOLBOX_TRANSFORMERS_H
