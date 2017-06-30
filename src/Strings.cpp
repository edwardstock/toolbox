/**
 * cpphelpers
 * StringHelper.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include "cpphelpers.h"

bool Strings::hasSubstring(const_string source, const_string substring)
{
	return source.find(substring)!=std::string::npos;
}

bool Strings::hasSubstring(const_string source, const char substring)
{
	return source.find(substring)!=std::string::npos;
}

std::pair<std::string, std::string> Strings::splitPair(const_string source, const_string delimiter)
{
	if (delimiter.empty()) {
		return std::pair<std::string, std::string>(std::string(), std::string());
	}
	return splitPair(source, delimiter.c_str()[0]);
}

std::pair<std::string, std::string> Strings::splitPair(const_string source, const char& delimiter)
{
	std::vector<std::string> elements = split(source, delimiter);

	if (elements.empty()) {
		return std::pair<std::string, std::string>(std::string(), std::string());
	}

	if (elements.size()==1) {
		return std::pair<std::string, std::string>(elements.at(0), std::string());
	}

	return std::pair<std::string, std::string>(elements.at(0), elements.at(1));
}

std::vector<std::string> Strings::split(const_string source, const_string delimiter)
{
	if (delimiter.empty()) {
		return std::vector<std::string>(0);
	}

	return split(source, delimiter.c_str()[0]);
}
std::vector<std::string> Strings::split(const_string source, const char& delimiter)
{
	std::stringstream ss;
	ss.str(source);
	std::string item;
	std::vector<std::string> elements;
	while (std::getline(ss, item, delimiter)) {
		elements.push_back(item);
	}

	return elements;
}

void Strings::removeSubstrings(std::string& source, const_string removable)
{
	size_t n = removable.length();

	for (size_t i = source.find(removable); i!=std::string::npos; i = source.find(removable)) {
		source.erase(i, n);
	}
}

void Strings::removeSubstrings(std::string& source, std::vector<std::string> removables)
{
	for (auto& replace : removables) {
		removeSubstrings(source, replace);
	}
}

std::string Strings::toString(std::ifstream& inputStream)
{
	return std::string((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
}

std::vector<std::vector<std::string>> Strings::matchAllRegexp(const std::regex& rx, const_string s)
{
	std::vector<std::vector<std::string>> capturedGroups;
	std::vector<std::string> capturedSubgroups;
	const std::sregex_token_iterator endIterator;
	for (std::sregex_token_iterator it(s.cbegin(), s.cend(), rx); it!=endIterator; ++it) {
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

std::vector<std::vector<std::string>> Strings::matchAllRegexp(const_string rxPattern, const_string s)
{
	return matchAllRegexp(std::regex(rxPattern, std::regex_constants::icase), s);
}

std::string Strings::matchRegexpFirst(const_string rxPattern, const_string source)
{
	return matchRegexpFirst(std::regex(rxPattern, std::regex_constants::icase), source);
}

std::string Strings::matchRegexpFirst(const std::regex& rxPattern, const_string source)
{
	std::smatch results;
	std::string result;
	bool found = std::regex_search(source, results, rxPattern);
	if (!found || results.size()<2) {
		return std::string();
	}

	return results[1];
}

std::smatch Strings::matchRegexp(const_string rxPattern, const_string source)
{
	return matchRegexp(std::regex(rxPattern, std::regex_constants::icase), source);
}

std::smatch Strings::matchRegexp(const std::regex& rxPattern, const_string source)
{
	std::smatch result;
	std::regex_search(source, result, rxPattern);
	return result;
}

bool Strings::equalsIgnoreCase(const_string s1, const_string s2)
{
	if (s1.length()!=s2.length()) return false;

	return std::equal(
			s2.begin(), s2.end(),
			s1.begin(),
			[](unsigned char a, unsigned char b) {
			  return std::tolower(a)==std::tolower(b);
			}
	);
}

bool Strings::hasRegex(const std::regex& pattern, const_string source)
{
	std::smatch match;
	return std::regex_search(source, match, pattern);
}

bool Strings::hasRegex(const_string pattern, const_string source)
{
	return hasRegex(std::regex(pattern), source);
}

std::string Strings::glue(const_string glue, const std::vector<std::string>& strings)
{
	std::string out;

	size_t size = strings.size();
	size_t i = 0;
	for (auto& s: strings) {
		if (i==0 || i==size) {
			out.append(s);
		}
		else {
			out.append(glue).append(s);
		}

		i++;
	}

	return out;
}

std::string Strings::substringReplace(const_string search, const_string replace, const_string source)
{
	if (source.empty() || source.length()<search.length()) {
		return source;
	}

	size_t found = source.find(search);
	if (found==std::string::npos) {
		return source;
	}

	std::string begin, end;
	begin = source.substr(0, found);
	end = source.substr(found+search.length(), std::string::npos);
	std::stringstream ss;
	ss << begin << replace << end;

	return ss.str();
}

std::string
Strings::substringReplaceAll(const std::vector<std::string>& search, const std::vector<std::string>& replace,
		const_string source)
{
	if (search.size()!=replace.size() || search.empty() || replace.empty()) {
		throw std::invalid_argument("search & replace vectors must be equal size and not empty!");
	}

	std::string result;

	for (size_t i = 0; i<search.size(); i++) {
		result = substringReplaceAll(search[i], replace[i], i==0 ? source : result);
	}

	return result;
}

std::string Strings::substringReplaceAll(const_string search,
		const_string replace,
		const_string source)
{

	std::string result = substringReplace(search, replace, source);
	while (result.find(search)!=std::string::npos) {
		result = substringReplace(search, replace, result);
	}

	return result;
}

void Strings::replace(const_string search, const_string replace, std::string& source)
{
	source = substringReplace(search, replace, source);
}
void Strings::replaceAll(const_string search, const_string replace, std::string& source)
{
	source = substringReplaceAll(search, replace, source);
}
void
Strings::replaceAll(const std::vector<std::string>& search,
		const std::vector<std::string>& replace,
		std::string& source)
{
	source = substringReplaceAll(search, replace, source);
}

std::string Strings::substrInverse(const_string source, char whence)
{
	strlen_t from;
	std::string out;
	if (~whence>=0) {
		from = source.rfind(~whence);
		out = source.substr(from+1, std::string::npos);
	}
	else {
		from = source.find(whence);
		out = source.substr(0, from);
	}

	return out;
}

std::string Strings::substrInverse(const_string source, char begin, char end, long offset)
{
	return substrInverse(source, std::string(1, begin), std::string(1, end), offset);
}

std::string Strings::substrInverse(const_string source, const_string begin)
{
	strlen_t from = source.find(begin);
	if (from==std::string::npos) {
		from = 0;
	}
	return source.substr(0, from);
}

std::string Strings::substrInverse(const_string source, const_string begin, const_string end, long offset)
{
	strlen_t to, from = source.find(begin);
	if (from==std::string::npos) {
		from = 0;
	}
	to = source.rfind(end);

	from += -(offset);
	to += 1+offset;
	std::string out = source.substr(0, from);
	out.append(source.substr(to, std::string::npos));
	return out;
}

