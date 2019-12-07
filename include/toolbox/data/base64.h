/*!
 * toolbox.
 * base64.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#ifndef TOOLBOX_BASE64_H
#define TOOLBOX_BASE64_H

#include "utils.h"

#include <string>

namespace toolbox {
namespace data {

std::string get_base64_chars();

bool is_base64(unsigned char c);

std::string base64_encode(const std::string& raw_string);
std::string base64_decode(const std::string& encoded_string);
std::vector<uint8_t> base64_encode_bytes(const std::vector<uint8_t>& raw);
std::vector<uint8_t> base64_decode_bytes(const std::vector<uint8_t>& encoded);
std::vector<uint8_t> base64_encode_chars(const std::vector<char>& raw);
std::vector<uint8_t> base64_decode_chars(const std::vector<char>& encoded);

} // namespace data
} // namespace toolbox

#endif // TOOLBOX_BASE64_H
