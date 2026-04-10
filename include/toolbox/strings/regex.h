/// @file regex.h
/// @brief Regex-based string matching and extraction utilities.
///        Requires HAVE_REGEX_H to be defined (auto-detected at configure time).

#ifndef TOOLBOXPP_STRINGS_REGEX_H
#define TOOLBOXPP_STRINGS_REGEX_H

#include "toolbox/toolbox_config.h"

#ifdef HAVE_REGEX_H

#if defined(HAVE_STD_REGEX)
#include <regex>
namespace rxns = std;
#elif defined(HAVE_BOOST_REGEX)
#include <boost/regex.hpp>
namespace rxns = boost;
#endif //#if defined(HAVE_STD_REGEX)

#include <string>
#include <vector>

namespace toolbox {
namespace strings {

/// Result type for find_all_patterns(): vector of match groups, each group is a vector of strings.
using match_all_res_t = std::vector<std::vector<std::string>>;

/// Check whether @p source matches the given compiled regex @p pattern.
/// @param pattern compiled regex
/// @param source string to test
/// @return true if the pattern matches
TOOLBOX_API bool matches_pattern(const rxns::regex& pattern, const std::string& source);

/// Check whether @p source matches the given regex @p pattern string.
/// The pattern is compiled with std::regex_constants::icase by default.
/// @param pattern regex pattern string
/// @param source string to test
/// @return true if the pattern matches
TOOLBOX_API bool matches_pattern(const std::string& pattern, const std::string& source);

/// Find all regex match groups in @p source.
/// @param pattern compiled regex
/// @param source string to search
/// @return vector of match groups (each group is a vector of captured strings)
TOOLBOX_API match_all_res_t find_all_patterns(const rxns::regex& pattern, const std::string& source);

/// Find all regex match groups in @p source.
/// @param pattern regex pattern string
/// @param source string to search
/// @return vector of match groups (each group is a vector of captured strings)
TOOLBOX_API match_all_res_t find_all_patterns(const std::string& pattern, const std::string& source);

/// Return the first regex match (or the first captured group) from @p source.
/// @param pattern compiled regex
/// @param source string to search
/// @return matched string, or empty if not found
TOOLBOX_API std::string find_first_pattern(const rxns::regex& pattern, const std::string& source);

/// Return the first regex match (or the first captured group) from @p source.
/// @param pattern regex pattern string
/// @param source string to search
/// @return matched string, or empty if not found
TOOLBOX_API std::string find_first_pattern(const std::string& pattern, const std::string& source);

/// Return the last regex match (or the first captured group of the last match) from @p source.
/// @param pattern compiled regex
/// @param source string to search
/// @return matched string, or empty if not found
TOOLBOX_API std::string find_last_pattern(const rxns::regex& pattern, const std::string& source);

/// Return the last regex match (or the first captured group of the last match) from @p source.
/// @param pattern regex pattern string
/// @param source string to search
/// @return matched string, or empty if not found
TOOLBOX_API inline std::string find_last_pattern(const std::string& pattern, const std::string& source);

/// Return all captured groups from the first regex match.
/// Note: do not use this to check whether a pattern exists in a string;
/// use matches_pattern() instead, as results vary across compilers.
/// @param pattern compiled regex
/// @param source string to search
/// @return vector of captured group strings
TOOLBOX_API std::vector<std::string> find_pattern(const rxns::regex& pattern, const std::string& source);

/// Return all captured groups from the first regex match.
/// Note: do not use this to check whether a pattern exists in a string;
/// use matches_pattern() instead, as results vary across compilers.
/// @param pattern regex pattern string
/// @param source string to search
/// @return vector of captured group strings
TOOLBOX_API std::vector<std::string> find_pattern(const std::string& pattern, const std::string& source);

/// Check whether the input string represents a valid integer (including exponential notation).
/// @param input string to check
/// @return true if the string is a valid integer
TOOLBOX_API bool num_is_integer(const std::string& input);

/// Check whether the input string represents a real (floating-point) number.
/// @param input string to check
/// @return true if the string is a valid real number
TOOLBOX_API bool num_is_real(const std::string& input);

/// Remove the first occurrence of @p pattern from @p source.
/// @param pattern compiled regex
/// @param source string to modify
/// @return string with the first match removed
TOOLBOX_API std::string remove_first_pattern(const rxns::regex& pattern, const std::string& source);

/// Remove the last occurrence of @p pattern from @p source.
/// @param pattern compiled regex
/// @param source string to modify
/// @return string with the last match removed
TOOLBOX_API std::string remove_last_pattern(const rxns::regex& pattern, const std::string& source);

} // namespace strings
} // namespace toolbox

#endif //#ifdef HAVE_REGEX_H

#endif // TOOLBOXPP_STRINGS_REGEX_H
