/**
 * cpphelpers
 * StringHelper.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include "cpphelpers.h"

bool Strings::hasSubstring(const std::string &source, const std::string &substring) {
	return source.find(substring) != std::string::npos;
}

bool Strings::hasSubstring(const std::string &source, const char substring) {
	return source.find(substring) != std::string::npos;
}

std::pair<std::string, std::string> Strings::splitPair(const std::string &source, const std::string &delimiter) {
	if (delimiter.size() < 1) {
		return std::pair<std::string, std::string>(std::string(), std::string());
	}
	return splitPair(source, delimiter.c_str()[0]);
}

std::pair<std::string, std::string> Strings::splitPair(const std::string &source, const char &delimiter) {
	std::vector<std::string> elements = split(source, delimiter);

	if (elements.size() == 0) {
		return std::pair<std::string, std::string>(std::string(), std::string());
	} else if (elements.size() == 1) {
		return std::pair<std::string, std::string>(elements.at(0), std::string());
	}

	return std::pair<std::string, std::string>(elements.at(0), elements.at(1));
}

std::vector<std::string> Strings::split(const std::string &source, const std::string &delimiter) {
	if (delimiter.size() < 1) {
		return std::vector<std::string>(0);
	}

	return split(source, delimiter.c_str()[0]);
}
std::vector<std::string> Strings::split(const std::string &source, const char &delimiter) {
	std::stringstream ss;
	ss.str(source);
	std::string item;
	std::vector<std::string> elements;
	while (std::getline(ss, item, delimiter)) {
		elements.push_back(item);
	}

	return elements;
}

void Strings::removeSubstrings(std::string &s, const std::string &p) {
	std::string::size_type n = p.length();

	for (std::string::size_type i = s.find(p); i != std::string::npos; i = s.find(p)) {
		s.erase(i, n);
	}
}

void Strings::removeSubstrings(std::string &s, std::vector<std::string> replaces) {
	for (auto it = replaces.begin(); it != replaces.end(); ++it) {
		removeSubstrings(s, *it);
	}
}

std::string Strings::toString(int32_t in) {
	std::stringstream ss;
	ss << in;
	return ss.str();
}

std::string Strings::toString(int64_t in) {
	std::stringstream ss;
	ss << in;
	return ss.str();
}

std::string Strings::toString(std::ifstream &inputStream) {
	return std::string((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
}

std::vector<std::vector<std::string>> Strings::matchAllRegexp(const std::regex &rx, const std::string &s) {
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

std::vector<std::vector<std::string>> Strings::matchAllRegexp(const std::string &rxPattern, const std::string &s) {
	return matchAllRegexp(std::regex(rxPattern, std::regex_constants::icase), s);
}

std::string Strings::matchRegexpFirst(const std::string &rxPattern, const std::string &source) {
	return matchRegexpFirst(std::regex(rxPattern, std::regex_constants::icase), source);
}

std::string Strings::matchRegexpFirst(const std::regex &rxPattern, const std::string &source) {
	std::smatch results;
	std::string result;
	bool found = std::regex_search(source, results, rxPattern);
	if (!found || results.size() < 2) {
		return std::string();
	}

	return results[1];
}

std::smatch Strings::matchRegexp(const std::string &rxPattern, const std::string &source) {
	return matchRegexp(std::regex(rxPattern, std::regex_constants::icase), source);
}

std::smatch Strings::matchRegexp(const std::regex &rxPattern, const std::string &source) {
	std::smatch result;
	std::regex_search(source, result, rxPattern);
	return result;
}

bool Strings::equals(const std::string &s1, const std::string &s2) {
	if (s2.length() != s1.length()) return false;
	return std::strncmp(s1.c_str(), s2.c_str(), s2.size()) == 0;
}

bool Strings::equalsIgnoreCase(const std::string &s1, const std::string &s2) {
	if (s1.length() != s2.length()) return false;

	return std::equal(
		s2.begin(), s2.end(),
		s1.begin(),
		[](unsigned char a, unsigned char b) {
		  return std::tolower(a) == std::tolower(b);
		}
	);
}

bool Strings::hasRegex(const std::regex &pattern, const std::string &source) {
	std::smatch match;
	return std::regex_search(source, match, pattern);
}

bool Strings::hasRegex(const std::string &pattern, const std::string &source) {
	return hasRegex(std::regex(pattern), source);
}

bool Strings::replace(const std::string &search, const std::string &replace, std::string &source) {
	if (search.empty())
		return false;

	size_t startPos = 0;
	while ((startPos = source.find(search, startPos)) != std::string::npos) {
		source.replace(startPos, search.length(), replace);
		startPos += replace.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}

	return true;
}

std::string Strings::glue(const std::string &glue, const std::vector<std::string> &strings) {
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
