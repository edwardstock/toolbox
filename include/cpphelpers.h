/**
 * uedit
 * cpphelpers.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef CPPHELPERS_H
#define CPPHELPERS_H

#ifndef _CPPHELPERS_EXPORT
#define _CPPHELPERS_EXPORT __attribute__ ((visibility ("default")))
#endif

#include <string>
#include <vector>
#include <map>
#include <istream>
#include <ostream>
#include <vector>
#include <regex>
#include <sstream>
#include <iostream>
#include <fstream>

class _CPPHELPERS_EXPORT Collections {
  public:
	template<typename K, typename V>
	static std::vector<K> mapKeysToVector(const std::map<K, V> &map);

	template<typename K, typename V>
	static std::vector<V> mapValuesToVector(const std::map<K, V> &map);
};

class _CPPHELPERS_EXPORT Console {
  public:
	static bool confirm(std::istream &in, std::ostream &out, const std::string &message, bool defValue = false);
	static std::string prompt(std::istream &in, std::ostream &out, const std::string &message,
	                          bool required = false, const std::string &defValue = "");
};

class _CPPHELPERS_EXPORT Numbers {
  public:
	static bool isInteger(const std::string &input);
	static bool isReal(const std::string &input);
};

class _CPPHELPERS_EXPORT Strings {
  public:
	static bool hasSubstring(const std::string &source, const std::string &substring);
	static bool hasSubstring(const std::string &source, char substring);
	static bool hasRegex(const std::regex &pattern, const std::string &source);
	static bool hasRegex(const std::string &pattern, const std::string &source);
	static std::vector<std::vector<std::string>> matchAllRegexp(const std::regex &rx, const std::string &s);
	static std::vector<std::vector<std::string>> matchAllRegexp(const std::string &rxPattern, const std::string &s);
	static std::string matchRegexpFirst(const std::string &rxPattern, const std::string &source);
	static std::string matchRegexpFirst(const std::regex &rxPattern, const std::string &source);
	static std::smatch matchRegexp(const std::string &rxPattern, const std::string &source);
	static std::smatch matchRegexp(const std::regex &rxPattern, const std::string &source);

	static std::pair<std::string, std::string> splitPair(const std::string &source, const std::string &delimiter);
	static std::pair<std::string, std::string> splitPair(const std::string &source, const char &delimiter);
	static std::vector<std::string> split(const std::string &source, const std::string &delimiter);
	static std::vector<std::string> split(const std::string &source, const char &delimiter);

	static void removeSubstrings(std::string &s, const std::string &p);
	static void removeSubstrings(std::string &s, std::vector<std::string> replaces);
	static bool replace(const std::string &search, const std::string &replace, std::string &source);

	static std::string glue(const std::string &glue, const std::vector<std::string> &strings);

	static std::string toString(int32_t in);
	static std::string toString(int64_t in);
	static std::string toString(std::ifstream &inputStream);

	static bool equals(const std::string &s1, const std::string &s2);
	static bool equalsIgnoreCase(const std::string &s1, const std::string &s2);
};

#endif // CPPHELPERS_H