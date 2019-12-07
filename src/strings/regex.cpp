/*!
 * toolbox.
 * regex.cpp
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "toolbox/strings/regex.h"

#include <vector>

bool toolbox::strings::matches_pattern(const std::regex& pattern, const std::string& source) {
    std::smatch match;
    return std::regex_search(source, match, pattern);
}
bool toolbox::strings::matches_pattern(const std::string& pattern, const std::string& source) {
    return matches_pattern(std::regex(pattern, std::regex_constants::icase), source);
}
std::vector<std::vector<std::string>> toolbox::strings::find_all_pattern(const std::regex& pattern,
                                                                         const std::string& source) {
    std::vector<std::vector<std::string>> capturedGroups;
    std::vector<std::string> capturedSubgroups;
    const std::sregex_token_iterator endIterator;
    for (std::sregex_token_iterator it(source.cbegin(), source.cend(), pattern); it != endIterator; ++it) {
        capturedSubgroups.clear();
        std::string group = *it;
        std::smatch res;
        if (std::regex_search(group, res, pattern)) {
            for (size_t i = 0; i < res.size(); i++) { // NOLINT(modernize-loop-convert), sometimes foreach has
                // strange effect - size is 0, but iterator have != 0 items
                capturedSubgroups.push_back(res[i]);
            }

            if (!capturedSubgroups.empty()) {
                capturedGroups.push_back(capturedSubgroups);
            }
        }
    }

    return capturedGroups;
}
std::vector<std::vector<std::string>> toolbox::strings::find_all_pattern(const std::string& pattern,
                                                                         const std::string& source) {
    return find_all_pattern(std::regex(pattern, std::regex_constants::icase), source);
}
std::string toolbox::strings::find_pattern_first(const std::regex& pattern, const std::string& source) {
    std::smatch results;
    std::string result;
    bool found = std::regex_search(source, results, pattern);
    if (!found || results.size() < 2) {
        return std::string();
    }

    return results[1];
}
std::string toolbox::strings::find_pattern_first(const std::string& pattern, const std::string& source) {
    return find_pattern_first(std::regex(pattern, std::regex_constants::icase), source);
}
std::vector<std::string> toolbox::strings::find_pattern(const std::regex& rxPattern, const std::string& source) {
    std::smatch result;
    std::regex_search(source, result, rxPattern);

    std::vector<std::string> out(result.size());
    const size_t cnt = result.size();
    for (size_t i = 0; i < cnt; i++) {
        out[i] = result[i];
    }

    return out;
}
std::vector<std::string> toolbox::strings::find_pattern(const std::string& pattern, const std::string& source) {
    return find_pattern(std::regex(pattern, std::regex_constants::icase), source);
}
bool toolbox::strings::num_is_integer(const std::string& input) {
    return std::regex_match(input, std::regex(R"(^[-]?[0-9eE+]+?$)"));
}
bool toolbox::strings::num_is_real(const std::string& input) {
    return std::regex_match(input, std::regex(R"(^[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$)"));
}
