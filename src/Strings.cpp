/**
 * cpphelpers
 * StringHelper.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include "cpphelpers.h"

bool cpphelpers::strings::hasSubstring(const_string source, const_string substring) {
	return source.find(substring) != std::string::npos;
}

bool cpphelpers::strings::hasSubstring(const_string source, const char substring) {
	return source.find(substring) != std::string::npos;
}

std::pair<std::string, std::string> cpphelpers::strings::splitPair(const_string source, const_string delimiter) {
	if (delimiter.empty()) {
		return std::pair<std::string, std::string>(std::string(), std::string());
	}
	return splitPair(source, delimiter.c_str()[0]);
}

std::pair<std::string, std::string> cpphelpers::strings::splitPair(const_string source, const char &delimiter) {
	std::vector<std::string> elements = split(source, delimiter);

	if (elements.empty()) {
		return std::pair<std::string, std::string>(std::string(), std::string());
	}

	if (elements.size() == 1) {
		return std::pair<std::string, std::string>(elements.at(0), std::string());
	}

	return std::pair<std::string, std::string>(elements.at(0), elements.at(1));
}

std::vector<std::string> cpphelpers::strings::split(const_string source, const_string delimiter) {
	if (delimiter.empty()) {
		return std::vector<std::string>(0);
	}

	return split(source, delimiter.c_str()[0]);
}
std::vector<std::string> cpphelpers::strings::split(const_string source, const char &delimiter) {
	std::stringstream ss;
	ss.str(source);
	std::string item;
	std::vector<std::string> elements;
	while (std::getline(ss, item, delimiter)) {
		elements.push_back(item);
	}

	return elements;
}

void cpphelpers::strings::removeSubstrings(std::string &source, const_string removable) {
	size_t n = removable.length();

	for (size_t i = source.find(removable); i != std::string::npos; i = source.find(removable)) {
		source.erase(i, n);
	}
}

void cpphelpers::strings::removeSubstrings(std::string &source, std::vector<std::string> removables) {
	for (auto &replace : removables) {
		removeSubstrings(source, replace);
	}
}

std::string cpphelpers::strings::toString(std::ifstream &inputStream) {
	return std::string((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
}

std::vector<std::vector<std::string>> cpphelpers::strings::matchAllRegexp(const std::regex &rx, const_string s) {
	std::vector<std::vector<std::string>> capturedGroups;
	std::vector<std::string> capturedSubgroups;
	const std::sregex_token_iterator endIterator;
	for (std::sregex_token_iterator it(s.cbegin(), s.cend(), rx); it != endIterator; ++it) {
		capturedSubgroups.clear();
		std::string group = *it;
		std::smatch res;
		if (std::regex_search(group, res, rx)) {
			for (auto r : res) {
				capturedSubgroups.push_back(std::move(r));
			}

			if (!capturedSubgroups.empty()) {
				capturedGroups.push_back(capturedSubgroups);
			}
		}
	}
	capturedGroups.push_back(capturedSubgroups);
	return capturedGroups;
}

std::vector<std::vector<std::string>> cpphelpers::strings::matchAllRegexp(const_string rxPattern, const_string s) {
	return matchAllRegexp(std::regex(rxPattern, std::regex_constants::icase), s);
}

std::string cpphelpers::strings::matchRegexpFirst(const_string rxPattern, const_string source) {
	return matchRegexpFirst(std::regex(rxPattern, std::regex_constants::icase), source);
}

std::string cpphelpers::strings::matchRegexpFirst(const std::regex &rxPattern, const_string source) {
	std::smatch results;
	std::string result;
	bool found = std::regex_search(source, results, rxPattern);
	if (!found || results.size() < 2) {
		return std::string();
	}

	return results[1];
}

std::smatch cpphelpers::strings::matchRegexp(const_string rxPattern, const_string source) {
	return matchRegexp(std::regex(rxPattern, std::regex_constants::icase), source);
}

std::smatch cpphelpers::strings::matchRegexp(const std::regex &rxPattern, const_string source) {
	std::smatch result;
	std::regex_search(source, result, rxPattern);
	return result;
}

bool cpphelpers::strings::equalsIgnoreCase(const_string s1, const_string s2) {
	if (s1.length() != s2.length()) return false;

	return std::equal(
		s2.begin(), s2.end(),
		s1.begin(),
		[](unsigned char a, unsigned char b) {
		  return __CHAR_TO_LOWER(a) == __CHAR_TO_LOWER(b);
		}
	);
}

bool cpphelpers::strings::hasRegex(const std::regex &pattern, const_string source) {
	std::smatch match;
	return std::regex_search(source, match, pattern);
}

bool cpphelpers::strings::hasRegex(const_string pattern, const_string source) {
	return hasRegex(std::regex(pattern), source);
}

std::string cpphelpers::strings::glue(const_string glue, const std::vector<std::string> &strings) {
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

std::string cpphelpers::strings::substringReplace(const_string search, const_string replace, const_string source) {
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
cpphelpers::strings::substringReplaceAll(const std::vector<std::string> &search,
                                         const std::vector<std::string> &replace,
                                         const_string source) {
	std::vector<std::string> toReplace;
	if (replace.size() != search.size() && replace.size() == 1) {
		for (int i = 0; i < search.size(); i++) {
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

std::string cpphelpers::strings::substringReplaceAll(const_string search,
                                                     const_string replace,
                                                     const_string source) {

	std::string result = substringReplace(search, replace, source);
	while (result.find(search) != std::string::npos) {
		result = substringReplace(search, replace, result);
	}

	return result;
}

void cpphelpers::strings::replace(const_string search, const_string replace, std::string &source) {
	source = substringReplace(search, replace, source);
}
void cpphelpers::strings::replaceAll(const_string search, const_string replace, std::string &source) {
	source = substringReplaceAll(search, replace, source);
}
void
cpphelpers::strings::replaceAll(const std::vector<std::string> &search,
                                const std::vector<std::string> &replace,
                                std::string &source) {
	source = substringReplaceAll(search, replace, source);
}

std::string cpphelpers::strings::substrInverse(const_string source, char whence) {
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

std::string cpphelpers::strings::substrInverse(const_string source, char begin, char end, long offset) {
	return substrInverse(source, std::string(1, begin), std::string(1, end), offset);
}

std::string cpphelpers::strings::substrInverse(const_string source, const_string begin) {
	strlen_t from = source.find(begin);
	if (from == std::string::npos) {
		from = 0;
	}
	return source.substr(0, from);
}

std::string cpphelpers::strings::substrInverse(const_string source, const_string begin, const_string end, long offset) {
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
bool cpphelpers::strings::hasSubstringIgnoreCase(const_string source, const_string substring) {
	size_t cmp = stringCompare(source, substring);
	return cmp != std::string::npos;
}

std::string cpphelpers::strings::clipSubstring(const_string source,
                                               const_string search,
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




