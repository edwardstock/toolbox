/// @file base64.h
/// @brief Base64 encoding and decoding for strings and byte vectors.

#ifndef TOOLBOX_BASE64_H
#define TOOLBOX_BASE64_H

#include "toolbox/toolbox_config.h"
#include "utils.h"

#include <string>

namespace toolbox {
namespace data {

/// Return the standard base64 character set.
TOOLBOX_API std::string get_base64_chars();
/// Check whether the given character is a valid base64 character.
TOOLBOX_API bool is_base64(unsigned char c);

/// Encode a raw string to a base64 string.
TOOLBOX_API std::string base64_encode(const std::string& raw_string);
/// Decode a base64 string back to raw bytes.
TOOLBOX_API std::string base64_decode(const std::string& encoded_string);

/// Encode a byte vector to base64, returning the result as bytes.
TOOLBOX_API std::vector<uint8_t> base64_encode_bytes(const std::vector<uint8_t>& raw);
/// Decode a base64 byte vector back to raw bytes.
TOOLBOX_API std::vector<uint8_t> base64_decode_bytes(const std::vector<uint8_t>& encoded);

/// Encode a char vector to base64, returning the result as bytes.
TOOLBOX_API std::vector<uint8_t> base64_encode_chars(const std::vector<char>& raw);
/// Decode a base64 char vector back to raw bytes.
TOOLBOX_API std::vector<uint8_t> base64_decode_chars(const std::vector<char>& encoded);

} // namespace data
} // namespace toolbox

#endif // TOOLBOX_BASE64_H
