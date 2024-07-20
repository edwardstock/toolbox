/*!
 * toolbox.
 * regex.cpp
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "toolbox/strings/regex.h"

#include <sys/types.h>
#include <vector>

#ifdef HAVE_REGEX_H

bool toolbox::strings::matches_pattern(const rxns::regex& pattern, const std::string& source) {
    rxns::smatch match;
    return rxns::regex_search(source, match, pattern);
}

bool toolbox::strings::matches_pattern(const std::string& pattern, const std::string& source) {
    return matches_pattern(rxns::regex(pattern, rxns::regex_constants::icase), source);
}

std::vector<std::vector<std::string>> toolbox::strings::find_all_patterns(
    const rxns::regex& pattern,
    const std::string& source
) {
    std::vector<std::vector<std::string>> captured_groups;
    std::vector<std::string> captured_subgroups;
    const rxns::sregex_token_iterator end_iterator;

    for (rxns::sregex_token_iterator it(source.cbegin(), source.cend(), pattern); it != end_iterator; ++it) {
        captured_subgroups.clear();
        std::string group = *it;
        rxns::smatch result;
        if (rxns::regex_search(group, result, pattern)) {
            for (const auto& result_item : result) {
                // NOLINT(modernize-loop-convert), sometimes foreach has
                // strange effect - size is 0, but iterator have != 0 items
                captured_subgroups.push_back(result_item);
            }

            if (!captured_subgroups.empty()) {
                captured_groups.push_back(captured_subgroups);
            }
        }
    }

    return captured_groups;
}

std::vector<std::vector<std::string>> toolbox::strings::find_all_patterns(
    const std::string& pattern,
    const std::string& source
    ) {
    return find_all_patterns(rxns::regex(pattern, rxns::regex_constants::icase), source);
}

std::string toolbox::strings::find_first_pattern(const rxns::regex& pattern, const std::string& source) {
    rxns::smatch results;
    std::string result;
    bool found = rxns::regex_search(source, results, pattern);
    if (!found) {
        return {};
    }

    if (results.size() == 1) {
        return results[0];
    }
    if (results.size() >= 2) {
        return results[1];
    }
    return {};
}

std::string toolbox::strings::find_first_pattern(const std::string& pattern, const std::string& source) {
    return find_first_pattern(rxns::regex(pattern, rxns::regex_constants::icase), source);
}

std::string toolbox::strings::find_last_pattern(const rxns::regex& pattern, const std::string& source) {
    std::smatch match, last_match;
    const auto search_start(source.cbegin());
    int64_t next_search_pos = 0;

    // reduce searchable area after each match
    while (regex_search(search_start + next_search_pos, source.cend(), match, pattern)) {
        next_search_pos += match.position() + match.length();
        last_match = match;
    }

    if (last_match.size() == 1) {
        return last_match[0];
    }
    if (last_match.size() >= 2) {
        return last_match[1];
    }
    return {};
}

std::string toolbox::strings::find_last_pattern(const std::string& pattern, const std::string& source) {
    return find_last_pattern(rxns::regex(pattern, rxns::regex_constants::icase), source);
}

std::vector<std::string> toolbox::strings::find_pattern(const rxns::regex& pattern, const std::string& source) {
    rxns::smatch result;
    rxns::regex_search(source, result, pattern);

    std::vector<std::string> out(result.size());
    const size_t cnt = result.size();
    for (size_t i = 0; i < cnt; i++) {
        out[i] = result[i];
    }

    return out;
}

std::vector<std::string> toolbox::strings::find_pattern(const std::string& pattern, const std::string& source) {
    return find_pattern(rxns::regex(pattern, rxns::regex_constants::icase), source);
}

bool toolbox::strings::num_is_integer(const std::string& input) {
    return rxns::regex_match(input, rxns::regex(R"(^[-]?[0-9eE+]+?$)"));
}

bool toolbox::strings::num_is_real(const std::string& input) {
    return rxns::regex_match(input, rxns::regex(R"(^[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$)"));
}

std::string toolbox::strings::remove_first_pattern(const rxns::regex& pattern, const std::string& source) {
    std::smatch match;

    // Find the first occurrence of the pattern
    if (rxns::regex_search(source, match, pattern)) {
        // match.position(0) gives the start position of the match
        // match.length(0) gives the length of the matched string
        std::string result = source;
        result.erase(match.position(0), match.length(0));
        return result;
    }

    // If no match was found, return the original string
    return source;
}

std::string toolbox::strings::remove_last_pattern(const rxns::regex& pattern, const std::string& source) {
    std::smatch match;
    const auto search_start(source.cbegin());
    int64_t next_search_pos = 0;
    std::smatch::difference_type last_matched_pos = 0;
    std::smatch::difference_type last_matched_len = 0;

    // reduce searchable area after each match
    while (regex_search(search_start + next_search_pos, source.cend(), match, pattern)) {
        last_matched_pos = next_search_pos + match.position();
        last_matched_len = match.length();
        next_search_pos += match.position() + match.length();
    }

    if (last_matched_pos != 0 && last_matched_len != 0) {
        std::string result = source;
        result.erase(last_matched_pos, last_matched_len);
        return result;
    }

    return source; // No match found, return the original string
}

#endif //HAVE_REGEX_H
