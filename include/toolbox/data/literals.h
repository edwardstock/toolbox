/// @file literals.h
/// @brief User-defined literals for byte types: 0xFF_byte, 0xFFFF_dbyte.

#ifndef TOOLBOXPP_LITERALS_H
#define TOOLBOXPP_LITERALS_H

#include <cstdint>

namespace toolbox {
namespace data {
namespace literals {

/// Literal suffix for a single byte (uint8_t).  Example: 0xFF_byte
inline uint8_t operator""_byte(unsigned long long val) {
    return (uint8_t) val;
}
/// Plural alias for _byte.  Example: 255_bytes
inline uint8_t operator""_bytes(unsigned long long val) {
    return (uint8_t) val;
}
/// Literal suffix for a double byte (uint16_t).  Example: 0xFFFF_dbyte
inline uint16_t operator""_dbyte(unsigned long long val) {
    return (uint16_t) val;
}
/// Plural alias for _dbyte.  Example: 0xFFFF_dbytes
inline uint16_t operator""_dbytes(unsigned long long val) {
    return (uint16_t) val;
}


} // namespace literals
} // namespace data
} // namespace toolbox

#endif // TOOLBOXPP_LITERALS_H
