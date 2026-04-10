/// @file decimal_formatter.h
/// @brief Human-readable decimal number formatting with grouping and precision control.

#ifndef TOOLBOXPP_STRINGS_DECIMAL_FORMATTER_H
#define TOOLBOXPP_STRINGS_DECIMAL_FORMATTER_H
#include "toolbox/toolbox_config.h"

#include <ostream>
#include <string>
namespace toolbox {
namespace strings {

/// Formatter that converts numeric values into human-readable decimal strings
/// with configurable digit grouping (e.g. "1 000 000.0000") and precision.
class TOOLBOX_API decimal_formatter {
public:
    decimal_formatter() = default;
    explicit decimal_formatter(int num);
    explicit decimal_formatter(double num);
    explicit decimal_formatter(float num);
    explicit decimal_formatter(std::string num);
    explicit decimal_formatter(const char* num);

    /// Format the given number string and return the result.
    std::string operator()(const std::string& num);

    /// Set the group delimiter string (default: " ").
    decimal_formatter& set_delimiter(const std::string& delimiter);
    /// Set the group delimiter character.
    decimal_formatter& set_delimiter(char delimiter);
    /// Set the number of integer digits per group (default: 3).
    decimal_formatter& set_decimals_group(size_t num);

    /// Set the minimum number of fractional digits to keep (default: 4).
    decimal_formatter& set_min_precision(size_t min_precision);
    /// Set the maximum number of fractional digits to keep (default: 18).
    decimal_formatter& set_max_precision(size_t max_precision);

    /// Return the formatted string.
    [[nodiscard]] std::string format() const;
    friend std::ostream& operator<<(std::ostream& os, const decimal_formatter& formatter);

private:
    std::string m_input;
    size_t m_decimals = 3;
    size_t m_max_precision = 18;
    size_t m_min_precision = 4;
    std::string m_delimiter = " ";
};

} // namespace strings
} // namespace toolbox

TOOLBOX_API std::ostream& operator<<(std::ostream& os, const toolbox::strings::decimal_formatter& formatter);

#endif // TOOLBOXPP_STRINGS_DECIMAL_FORMATTER_H
