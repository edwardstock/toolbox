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
#include <exception>

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
	/**
	 * Search substring in string
	 * @param source
	 * @param substring
	 * @return
	 */
	static bool hasSubstring(const std::string &source, const std::string &substring);
	/**
	 * Serch character in string
	 * @param source
	 * @param substring
	 * @return
	 */
	static bool hasSubstring(const std::string &source, char substring);
	/**
	 * Match regex pattern
	 * @param pattern
	 * @param source
	 * @return
	 */
	static bool hasRegex(const std::regex &pattern, const std::string &source);
	/**
	 * Match regex pattern.
	 * @param pattern Just string. By default: search with flag std::regex_constants::icase
	 * @param source
	 * @return
	 */
	static bool hasRegex(const std::string &pattern, const std::string &source);
	/**
	 * Returns all found groups in source with pattern
	 * @param rx custom regex object
	 * @param s
	 * @return
	 */
	static std::vector<std::vector<std::string>> matchAllRegexp(const std::regex &pattern, const std::string &source);
	/**
	 * Returns all found groups in source with pattern
	 * @param pattern
	 * @param source
	 * @return
	 */
	static std::vector<std::vector<std::string>> matchAllRegexp(const std::string &pattern, const std::string &source);
	/**
	 * Take first regex match and return it (with custom regex object)
	 * @param pattern
	 * @param source
	 * @return
	 */
	static std::string matchRegexpFirst(const std::string &pattern, const std::string &source);
	/**
	 * Take first regex match and return it
	 * @param pattern
	 * @param source
	 * @return
	 */
	static std::string matchRegexpFirst(const std::regex &pattern, const std::string &source);
	/**
	 * boilerplate to std::regex
	 * @param pattern
	 * @param source
	 * @return
	 */
	static std::smatch matchRegexp(const std::string &pattern, const std::string &source);
	/**
	 * Less boilerplate to std::regex
	 * @param rxPattern
	 * @param source
	 * @return
	 */
	static std::smatch matchRegexp(const std::regex &rxPattern, const std::string &source);

	/**
	 * Splits string by delimiter to pair
	 * @param source
	 * @param delimiter
	 * @return
	 */
	static std::pair<std::string, std::string> splitPair(const std::string &source, const std::string &delimiter);
	/**
	 * Splits string by char delimiter to pair
	 * @param source
	 * @param delimiter
	 * @return
	 */
	static std::pair<std::string, std::string> splitPair(const std::string &source, const char &delimiter);
	/**
	 * Splits string by char delimiter to vector list
	 * @param source
	 * @param delimiter
	 * @return
	 */
	static std::vector<std::string> split(const std::string &source, const std::string &delimiter);
	/**
	 * Splits string by delimiter to vector list
	 * @param source
	 * @param delimiter
	 * @return
	 */
	static std::vector<std::string> split(const std::string &source, const char &delimiter);

	/**
	 * @see substringReplace(const std::string &search,
	                                    const std::string &replace,
	                                    const std::string &source) but mutable behavior
	 * @param search
	 * @param replace
	 * @param source Mutable
	 * @return
	 */
	static void replace(const std::string &search, const std::string &replace, std::string &source);

	/**
	 * @see substringReplaceAll(const std::string &search,
	                                       const std::string &replace,
	                                       const std::string &source)
	 * @param search
	 * @param replace
	 * @param source Mutable
	 */
	static void replaceAll(const std::string &search, const std::string &replace, std::string &source);

	/**
	 * @see substringReplaceAll(const std::vector<std::string> &search,
	                                       const std::vector<std::string> &replace,
	                                       const std::string &source)
	 * @param search
	 * @param replace
	 * @param source Mutable
	 */
	static void replaceAll(const std::vector<std::string> &search,
	                       const std::vector<std::string> &replace,
	                       std::string &source);

	/**
	 * Replaces first occurrence "search" and replaces with "replace" parameter in "source" string.
	 * @param search
	 * @param replace
	 * @param source Immutable
	 * @return If search string were not found, function will return source string.
	 */
	static std::string substringReplace(const std::string &search,
	                                    const std::string &replace,
	                                    const std::string &source);
	/**
	 * Replaces list search strings with replace list. Vectors size must be equals!
	 * Example:
	 *
	 * substringReplaceAll({"before1","before2"}, {"after1","after2"}, "Source string with before1 and before2")
	 *
	 *
	 * @param search
	 * @param replace
	 * @param source
	 * @return
	 */
	static std::string substringReplaceAll(const std::vector<std::string> &search,
	                                       const std::vector<std::string> &replace,
	                                       const std::string &source);
	/**
	 * Works like std::string substringReplace(const std::string &search, const std::string &replace, const std::string &source);
	 * but replaces all "search" occurrences with "replace" string
	 * @param search
	 * @param replace
	 * @param source
	 * @return
	 */
	static std::string substringReplaceAll(const std::string &search,
	                                       const std::string &replace,
	                                       const std::string &source);
	/**
	 * Find "removable" string in "source" and remove it
	 * @param source Mutable value!
	 * @param removable
	 */
	static void removeSubstrings(std::string &source, const std::string &removable);
	/**
	 * Find all "removables" strings in "source" and remove it
	 * @param source
	 * @param removables
	 */
	static void removeSubstrings(std::string &source, std::vector<std::string> removables);

	/**
	 * Concatenates strings by glue
	 * @param glue
	 * @param strings
	 * @return
	 */
	static std::string glue(const std::string &glue, const std::vector<std::string> &strings);

	/**
	 * Input file stream to string
	 * @param inputStream
	 * @return
	 */
	static std::string toString(std::ifstream &inputStream);

	/**
	 *
	 * @param s1
	 * @param s2
	 * @return
	 */
	static bool equalsIgnoreCase(const std::string &s1, const std::string &s2);
};

#endif // CPPHELPERS_H