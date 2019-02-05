/**
 * toolboxpp
 * StringHelper.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include <toolboxpp.h>

#include "toolboxpp.h"

bool toolboxpp::strings::hasSubstring(const std::string &substring, const std::string &source) {
    return source.find(substring) != std::string::npos;
}

bool toolboxpp::strings::hasSubstring(char substring, const std::string &source) {
    return source.find(substring) != std::string::npos;
}

bool toolboxpp::strings::hasWSubstring(wchar_t substring, const std::wstring &source) {
    return source.find(substring) != std::string::npos;
}

std::pair<std::string, std::string> toolboxpp::strings::splitPair(const std::string &source,
                                                                  const std::string &delimiter) {
    if (delimiter.empty()) {
        return std::pair<std::string, std::string>(std::string(), std::string());
    }
    return splitPair(source, delimiter.c_str()[0]);
}

std::pair<std::string, std::string> toolboxpp::strings::splitPair(const std::string &source, const char &delimiter) {
    std::vector<std::string> elements = split(source, delimiter);

    if (elements.empty()) {
        return std::pair<std::string, std::string>(std::string(), std::string());
    }

    if (elements.size() == 1) {
        return std::pair<std::string, std::string>(elements.at(0), std::string());
    }

    return std::pair<std::string, std::string>(elements.at(0), elements.at(1));
}

std::vector<std::string> toolboxpp::strings::split(const std::string &source, const std::string &delimiter) {
    if (delimiter.empty()) {
        return std::vector<std::string>(0);
    }

    std::string src = source;

    std::vector<std::string> result;
    size_t current = 0;
    while (current != src.npos) {
        current = src.find(delimiter);
        if (current != src.npos && src.substr(current, delimiter.length()) == delimiter) {
            result.push_back(src.substr(0, current));
            src = src.substr(current + (delimiter.length()), src.npos);
        }
    }
    if (src.length() > 0) {
        result.push_back(src);
    }

    return result;
}
std::vector<std::string> toolboxpp::strings::split(const std::string &source, const char &delimiter) {
    std::stringstream ss;
    ss.str(source);
    std::string item;
    std::vector<std::string> elements;
    while (std::getline(ss, item, delimiter)) {
        elements.push_back(item);
    }

    return elements;
}

void toolboxpp::strings::removeSubstrings(std::string &source, const std::string &removable) {
    size_t n = removable.length();

    for (size_t i = source.find(removable); i != std::string::npos; i = source.find(removable)) {
        source.erase(i, n);
    }
}

void toolboxpp::strings::removeSubstrings(std::string &source, std::vector<std::string> removables) {
    for (auto &replace : removables) {
        removeSubstrings(source, replace);
    }
}

std::string toolboxpp::strings::toString(std::ifstream &inputStream) {
    return std::string((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
}

#ifdef HAVE_REGEX_H
std::vector<std::vector<std::string>> toolboxpp::strings::matchAllRegexp(const rxns::regex &rx, const std::string &s) {
    std::vector<std::vector<std::string>> capturedGroups;
    std::vector<std::string> capturedSubgroups;
    const rxns::sregex_token_iterator endIterator;
    for (rxns::sregex_token_iterator it(s.cbegin(), s.cend(), rx); it != endIterator; ++it) {
        capturedSubgroups.clear();
        std::string group = *it;
        rxns::smatch res;
        if (rxns::regex_search(group, res, rx)) {
            for (auto r : res) {
                capturedSubgroups.push_back(std::move(r));
            }

            if (!capturedSubgroups.empty()) {
                capturedGroups.push_back(capturedSubgroups);
            }
        }
    }

    return capturedGroups;
}

std::vector<std::vector<std::string>> toolboxpp::strings::matchAllRegexp(const std::string &rxPattern,
                                                                         const std::string &s) {
    return matchAllRegexp(rxns::regex(rxPattern, rxns::regex_constants::icase), s);
}

std::string toolboxpp::strings::matchRegexpFirst(const std::string &rxPattern, const std::string &source) {
    return matchRegexpFirst(rxns::regex(rxPattern, rxns::regex_constants::icase), source);
}

std::string toolboxpp::strings::matchRegexpFirst(const rxns::regex &rxPattern, const std::string &source) {
    rxns::smatch results;
    std::string result;
    bool found = rxns::regex_search(source, results, rxPattern);
    if (!found || results.size() < 2) {
        return std::string();
    }

    return results[1];
}

const std::vector<std::string> toolboxpp::strings::matchRegexp(const std::string &rxPattern,
                                                               const std::string &source) {
    return matchRegexp(rxns::regex(rxPattern, rxns::regex_constants::icase), source);
}

const std::vector<std::string> toolboxpp::strings::matchRegexp(const rxns::regex &rxPattern,
                                                               const std::string &source) {
    rxns::smatch result;
    rxns::regex_search(source, result, rxPattern);
    std::vector<std::string> out(result.size());
    size_t i = 0;
    for (auto &r: result) {
        out[i] = r;
        i++;
    }
    return out;
}

bool toolboxpp::strings::hasRegex(const rxns::regex &pattern, const std::string &source) {
    rxns::smatch match;
    return rxns::regex_search(source, match, pattern);
}

bool toolboxpp::strings::hasRegex(const std::string &pattern, const std::string &source) {
    return hasRegex(rxns::regex(pattern), source);
}
#endif
bool toolboxpp::strings::equalsIgnoreCase(const std::string &s1, const std::string &s2) {
    if (s1.length() != s2.length()) return false;

    return std::equal(
        s2.begin(), s2.end(),
        s1.begin(),
        [](unsigned char a, unsigned char b) {
          return __CHAR_TO_LOWER(a) == __CHAR_TO_LOWER(b);
        }
    );
}

bool toolboxpp::strings::equalsIgnoreWCase(const std::wstring &s1, const std::wstring &s2) {
    if (s1.length() != s2.length()) return false;

    return std::equal(
        s2.begin(), s2.end(),
        s1.begin(),
        [](wchar_t a, wchar_t b) {
          return __WCHAR_TO_LOWER(a) == __WCHAR_TO_LOWER(b);
        }
    );
}

std::string toolboxpp::strings::glue(const std::string &glue, const std::vector<std::string> &strings) {
    std::string out;

    size_t size = strings.size();
    size_t i = 0;
    for (auto &s: strings) {
        if (i == 0 || i == size) {
            out.append(s);
        } else {
            out.append(glue).append(s);
        }

        i++;
    }

    return out;
}

std::string toolboxpp::strings::substringReplace(const std::string &search,
                                                 const std::string &replace,
                                                 const std::string &source) {
    if (source.empty() || source.length() < search.length()) {
        return source;
    }

    size_t found = source.find(search);
    if (found == std::string::npos) {
        return source;
    }

    std::string begin, end;
    begin = source.substr(0, found);
    end = source.substr(found + search.length(), std::string::npos);
    std::stringstream ss;
    ss << begin << replace << end;

    return ss.str();
}

std::string
toolboxpp::strings::substringReplaceAll(const std::vector<std::string> &search,
                                        const std::vector<std::string> &replace,
                                        const std::string &source) {
    std::vector<std::string> toReplace;
    if (replace.size() != search.size() && replace.size() == 1) {
        for (std::size_t i = 0; i < search.size(); i++) {
            toReplace.push_back(replace[0]);
        }
    } else if ((search.size() != replace.size() && replace.size() > 1) || search.empty() || replace.empty()) {
        throw std::invalid_argument("search & replace vectors must be equal size and not empty!");
    } else {
        toReplace = replace;
    }

    std::string result;

    for (size_t i = 0; i < search.size(); i++) {
        result = substringReplaceAll(search[i], toReplace[i], i == 0 ? source : result);
    }

    return result;
}

std::string toolboxpp::strings::substringReplaceAll(const std::string &search,
                                                    const std::string &replace,
                                                    const std::string &source) {

    std::string result = substringReplace(search, replace, source);
    while (result.find(search) != std::string::npos) {
        result = substringReplace(search, replace, result);
    }

    return result;
}

void toolboxpp::strings::replace(const std::string &search, const std::string &replace, std::string &source) {
    source = substringReplace(search, replace, source);
}

void toolboxpp::strings::replaceAll(const std::string &search, const std::string &replace, std::string &source) {
    source = substringReplaceAll(search, replace, source);
}

void toolboxpp::strings::replaceAll(const std::vector<std::string> &search,
                               const std::vector<std::string> &replace,
                               std::string &source) {
    source = substringReplaceAll(search, replace, source);
}

std::string toolboxpp::strings::substrInverse(const std::string &source, char whence) {
    strlen_t from;
    std::string out;
    if (~whence >= 0) {
        from = source.rfind(~whence);
        out = source.substr(from + 1, std::string::npos);
    } else {
        from = source.find(whence);
        out = source.substr(0, from);
    }

    return out;
}

std::string toolboxpp::strings::substrInverse(const std::string &source, char begin, char end, long offset) {
    return substrInverse(source, std::string(1, begin), std::string(1, end), offset);
}

std::string toolboxpp::strings::substrInverse(const std::string &source, const std::string &begin) {
    strlen_t from = source.find(begin);
    if (from == std::string::npos) {
        from = 0;
    }
    return source.substr(0, from);
}

std::string toolboxpp::strings::substrInverse(const std::string &source,
                                              const std::string &begin,
                                              const std::string &end,
                                              long offset) {
    strlen_t to, from = source.find(begin);
    if (from == std::string::npos) {
        from = 0;
    }
    to = source.rfind(end);

    from += -(offset);
    to += 1 + offset;
    std::string out = source.substr(0, from);
    out.append(source.substr(to, std::string::npos));
    return out;
}
bool toolboxpp::strings::hasSubstringIgnoreCase(const std::string &substring, const std::string &source) {
    size_t cmp = stringCompare(source, substring);
    return cmp != std::string::npos;
}

std::string toolboxpp::strings::clipSubstring(const std::string &source,
                                              const std::string &search,
                                              const size_t width,
                                              bool icase) {
    size_t pos = icase ? stringCompare(source, search) : source.find(search);
    if (pos == std::string::npos) {
        return source;
    }

    size_t searchLength = search.length();
    size_t sourceLength = source.length();

    // if width equals search length, return search string
    if (searchLength == width || sourceLength == searchLength || sourceLength == width) {
        return search;
    }

    // if width size less than search size, return part of search string
    if (width < searchLength || sourceLength < width) {
        return search.substr(0, width);
    }

    long centerOfSearch, centerOfWidth, begin;

    centerOfSearch = pos + (searchLength / 2);
    centerOfWidth = width / 2;

    if (centerOfSearch - centerOfWidth < 0) {
        begin = 0;
    } else {
        begin = centerOfSearch - centerOfWidth;
    }

    return source.substr((size_t) begin, width);
}
std::string toolboxpp::strings::toUpper(const std::string &s) {
    std::stringstream tmp;
    for (char i : s) {
        tmp << static_cast<char>(__CHAR_TO_UPPER(i));
    }

    return tmp.str();
}

std::wstring toolboxpp::strings::toWUpper(const std::wstring &s) {
    std::wstringstream tmp;
    for (wchar_t i : s) {
        tmp << static_cast<wchar_t>(__WCHAR_TO_UPPER(i));
    }

    return tmp.str();
}

std::string toolboxpp::strings::toLower(const std::string &s) {
    std::stringstream tmp;
    for (char i : s) {
        tmp << static_cast<char>(__CHAR_TO_LOWER(i));
    }

    return tmp.str();
}

std::wstring toolboxpp::strings::toWLower(const std::wstring &s) {
    std::wstringstream tmp;
    for (wchar_t i : s) {
        tmp << static_cast<wchar_t>(__WCHAR_TO_LOWER(i));
    }

    return tmp.str();
}



