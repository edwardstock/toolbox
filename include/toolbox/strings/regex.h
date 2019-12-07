/*!
 * toolbox.
 * regex.h
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_STRINGS_REGEX_H
#define TOOLBOXPP_STRINGS_REGEX_H

#include <regex>
#include <string>
#include <vector>

namespace toolbox {
namespace strings {

using match_all_res_t = std::vector<std::vector<std::string>>;

/**
 * Match regex pattern
 * @param pattern
 * @param source
 * @return
 */
bool matches_pattern(const std::regex& pattern, const std::string& source);
/**
 * Match regex pattern.
 * @param pattern Just string. By default: search with flag
 * std::regex_constants::icase
 * @param source
 * @return
 */
bool matches_pattern(const std::string& pattern, const std::string& source);
/**
 * Returns all found groups in source with pattern
 * @param rx custom regex object
 * @param s
 * @return vector of vectors of strings
 */
match_all_res_t find_all_pattern(const std::regex& pattern, const std::string& source);
/**
 * Returns all found groups in source with pattern
 * @param pattern
 * @param source
 * @return vector of vectors of strings
 */
match_all_res_t find_all_pattern(const std::string& pattern, const std::string& source);
/**
 * Take first regex match and return it
 * @param pattern
 * @param source
 * @return
 */
std::string find_pattern_first(const std::regex& pattern, const std::string& source);
/**
 * Take first regex match and return it (with custom regex object)
 * @param pattern
 * @param source
 * @return
 */
std::string find_pattern_first(const std::string& pattern, const std::string& source);

/**
 * Less boilerplate for std::regex
 * Attention! Don't use this method to check if regex pattern has in string, use
 * {@code hasRegex()} instead, because different compilers compiles different
 * results
 * @param rxPattern
 * @param source
 * @return
 */
std::vector<std::string> find_pattern(const std::regex& rxPattern, const std::string& source);

/**
 * Less boilerplate for std::regex
 * Attention! Don't use this method to check if regex pattern has in string, use
 * {@code hasRegex()} instead, because different compilers compiles different
 * results
 * @param pattern
 * @param source
 * @return
 */
std::vector<std::string> find_pattern(const std::string& pattern, const std::string& source);

/// \brief Check input string has a valid integer value (including exponential values)
/// \param input
/// \return
bool num_is_integer(const std::string& input);

/// \brief Check input string is a real value
/// \param input
/// \return
bool num_is_real(const std::string& input);

} // namespace strings
} // namespace toolbox

#endif // TOOLBOXPP_STRINGS_REGEX_H
