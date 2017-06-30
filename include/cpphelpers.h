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

#include <chrono>
#include <string>
#include <iosfwd>
#include <vector>
#include <map>
#include <deque>
#include <unordered_map>
#include <istream>
#include <ostream>
#include <vector>
#include <regex>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>

class _CPPHELPERS_EXPORT SimpleProfiler {
public:
	/**
	 * Begin profile record
	 * @param tag
	 */
	static void begin(const std::string& tag);
	/**
	 * End profile recording for last "begin" tag
	 */
	static void end();
	/**
	 * Finish profile recording for certain tag
	 * @param tag
	 */
	static void end(const std::string& tag);

	static void printProfile(std::ostream& ostream, bool clear = true);

	/**
	 * Clear profile data and last tag
	 */
	static void clear();

private:
	typedef std::chrono::milliseconds __millis;
	typedef std::chrono::system_clock __sys_clock;

	static std::unordered_map<std::string, __millis> profile;
	static std::string& lastTag;
};

class _CPPHELPERS_EXPORT Collections {
public:
	template<typename K, typename V>
	static std::vector<K> mapKeysToVector(const std::map<K, V>& map);

	template<typename K, typename V>
	static std::vector<V> mapValuesToVector(const std::map<K, V>& map);
};

class _CPPHELPERS_EXPORT Console {
public:
	static bool confirm(std::istream& in, std::ostream& out, const std::string& message, bool defValue = false);
	static std::string prompt(std::istream& in, std::ostream& out, const std::string& message,
			bool required = false, const std::string& defValue = "");
};

class _CPPHELPERS_EXPORT Numbers {
public:
	static bool isInteger(const std::string& input);
	static bool isReal(const std::string& input);
};

class _CPPHELPERS_EXPORT Strings {
public:
	typedef size_t strlen_t;
	typedef const std::string& const_string;

	/**
	 * Search substring in string
	 * @param source
	 * @param substring
	 * @return
	 */
	static bool hasSubstring(const_string source, const_string substring);
	/**
	 * Serch character in string
	 * @param source
	 * @param substring
	 * @return
	 */
	static bool hasSubstring(const_string source, char substring);
	/**
	 * Match regex pattern
	 * @param pattern
	 * @param source
	 * @return
	 */
	static bool hasRegex(const std::regex& pattern, const_string source);
	/**
	 * Match regex pattern.
	 * @param pattern Just string. By default: search with flag std::regex_constants::icase
	 * @param source
	 * @return
	 */
	static bool hasRegex(const_string pattern, const_string source);
	/**
	 * Returns all found groups in source with pattern
	 * @param rx custom regex object
	 * @param s
	 * @return
	 */
	static std::vector<std::vector<std::string>> matchAllRegexp(const std::regex& pattern, const_string source);
	/**
	 * Returns all found groups in source with pattern
	 * @param pattern
	 * @param source
	 * @return
	 */
	static std::vector<std::vector<std::string>> matchAllRegexp(const_string pattern, const_string source);
	/**
	 * Take first regex match and return it (with custom regex object)
	 * @param pattern
	 * @param source
	 * @return
	 */
	static std::string matchRegexpFirst(const_string pattern, const_string source);
	/**
	 * Take first regex match and return it
	 * @param pattern
	 * @param source
	 * @return
	 */
	static std::string matchRegexpFirst(const std::regex& pattern, const_string source);
	/**
	 * boilerplate to std::regex
	 * @param pattern
	 * @param source
	 * @return
	 */
	static std::smatch matchRegexp(const_string pattern, const_string source);
	/**
	 * Less boilerplate to std::regex
	 * @param rxPattern
	 * @param source
	 * @return
	 */
	static std::smatch matchRegexp(const std::regex& rxPattern, const_string source);

	/**
	 * Splits string by delimiter to pair
	 * @param source
	 * @param delimiter
	 * @return
	 */
	static std::pair<std::string, std::string> splitPair(const_string source, const_string delimiter);
	/**
	 * Splits string by char delimiter to pair
	 * @param source
	 * @param delimiter
	 * @return
	 */
	static std::pair<std::string, std::string> splitPair(const_string source, const char& delimiter);
	/**
	 * Splits string by char delimiter to vector list
	 * @param source
	 * @param delimiter
	 * @return
	 */
	static std::vector<std::string> split(const_string source, const_string delimiter);
	/**
	 * Splits string by delimiter to vector list
	 * @param source
	 * @param delimiter
	 * @return
	 */
	static std::vector<std::string> split(const_string source, const char& delimiter);

	/**
	 * @see substringReplace(const_string search,
	                                    const_string replace,
	                                    const_string source) but mutable behavior
	 * @param search
	 * @param replace
	 * @param source Mutable
	 * @return
	 */
	static void replace(const_string search, const_string replace, std::string& source);

	/**
	 * @see substringReplaceAll(const_string search,
	                                       const_string replace,
	                                       const_string source)
	 * @param search
	 * @param replace
	 * @param source Mutable
	 */
	static void replaceAll(const_string search, const_string replace, std::string& source);

	/**
	 * @see substringReplaceAll(const std::vector<std::string> &search,
	                                       const std::vector<std::string> &replace,
	                                       const_string source)
	 * @param search
	 * @param replace
	 * @param source Mutable
	 */
	static void replaceAll(const std::vector<std::string>& search,
			const std::vector<std::string>& replace,
			std::string& source);

	/**
	 * Replaces first occurrence "search" and replaces with "replace" parameter in "source" string.
	 * @param search
	 * @param replace
	 * @param source Immutable
	 * @return If search string were not found, function will return source string.
	 */
	static std::string substringReplace(const_string search,
			const_string replace,
			const_string source);
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
	static std::string substringReplaceAll(const std::vector<std::string>& search,
			const std::vector<std::string>& replace,
			const_string source);
	/**
	 * Works like std::string substringReplace(const_string search, const_string replace, const_string source);
	 * but replaces all "search" occurrences with "replace" string
	 * @param search
	 * @param replace
	 * @param source
	 * @return
	 */
	static std::string substringReplaceAll(const_string search,
			const_string replace,
			const_string source);
	/**
	 * Find "removable" string in "source" and remove it
	 * @param source Mutable value!
	 * @param removable
	 */
	static void removeSubstrings(std::string& source, const_string removable);
	/**
	 * Find all "removables" strings in "source" and remove it
	 * @param source
	 * @param removables
	 */
	static void removeSubstrings(std::string& source, std::vector<std::string> removables);

	/**
	 *
	 * @param source
	 * @param whence Whence start to cut. You can inverse character to cut from the end of "source"
	 * Example:
	 *  from start:
	 *      string s = "abcdef";
			Strings::substrInverse(s, 'c');
			result: "ab"

		from end:
	        string s = "abcdef";
			Strings::substrInverse(s, ~'c');
			result: "def"
	 * @return
	 */
	static std::string substrInverse(const_string source, char whence);
	static std::string substrInverse(const_string source, char begin, char end, long offset = 0);
	static std::string substrInverse(const_string source, const_string begin);
	static std::string substrInverse(const_string source, const_string begin, const_string end, long offset = 0);

	/**
	 * Concatenates strings by glue
	 * @param glue
	 * @param strings
	 * @return
	 */
	static std::string glue(const_string glue, const std::vector<std::string>& strings);

	/**
	 * Input file stream to string
	 * @param inputStream
	 * @return
	 */
	static std::string toString(std::ifstream& inputStream);

	/**
	 *
	 * @param s1
	 * @param s2
	 * @return
	 */
	static bool equalsIgnoreCase(const_string s1, const_string s2);
};

#endif // CPPHELPERS_H