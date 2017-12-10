/*! 
 * toolboxpp. 2017
 * 
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#include "gtest/gtest.h"
#include "../include/toolboxpp.h"

using namespace toolboxpp::strings;

TEST(Strings, HasSubstringIgnoreCase) {
    ASSERT_TRUE(hasSubstringIgnoreCase("AAA", "AAA"));
    ASSERT_TRUE(hasSubstringIgnoreCase("A", "AAA"));
    ASSERT_TRUE(hasSubstringIgnoreCase("a", "AAA"));
    ASSERT_FALSE(hasSubstringIgnoreCase("bbb", "AAA"));
    ASSERT_FALSE(hasSubstringIgnoreCase("aaaa", "A"));
    ASSERT_FALSE(hasSubstringIgnoreCase("1", "A"));
}

TEST(Strings, HasSubstring) {
    ASSERT_TRUE(hasSubstring("AAA", "AAA"));
    ASSERT_TRUE(hasSubstring("A", "AAA"));
    ASSERT_FALSE(hasSubstring("a", "AAA"));
    ASSERT_FALSE(hasSubstring("bbb", "AAA"));
    ASSERT_FALSE(hasSubstring("aaaa", "A"));
    ASSERT_FALSE(hasSubstring("1", "A"));
}

TEST(Strings, HasSubstringChar) {
    ASSERT_TRUE(hasSubstring('A', "AAA"));
    ASSERT_FALSE(hasSubstring('a', "AAA"));
    ASSERT_FALSE(hasSubstring('d', "{a,b,c}"));
}

#ifdef HAVE_REGEX_H
TEST(Strings, HasRegex) {
    ASSERT_TRUE(hasRegex("[0-9]", "abc123"));
    ASSERT_TRUE(hasRegex(std::regex("[a-z0-9]"), "abc123"));
    ASSERT_FALSE(hasRegex("[!@#$]", "abc123"));
}

TEST(Strings, MatchAllRegex) {
    auto result = matchAllRegexp(R"((\[[a-z]+\]))", "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");
    std::vector<std::string> mustBeFounded{
        "[abc]",
        "[def]",
        "[ghi]",
        "[jkl]"
    };
    size_t found = 0;
    for (auto &matches: result) {
        ASSERT_EQ(2UL, matches.size());
        const std::string match = matches[1];
        for (auto &f: mustBeFounded) {
            if (f == match) {
                found++;
                break;
            }
        }
    }

    ASSERT_EQ(mustBeFounded.size(), found);

    auto result2 = matchAllRegexp(std::regex(R"((\[[a-z]+\]))"), "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");
    size_t found2 = 0;
    for (auto &matches: result2) {
        ASSERT_EQ(2UL, matches.size());
        const std::string match = matches[1];
        for (auto &f: mustBeFounded) {
            if (f == match) {
                found2++;
                break;
            }
        }
    }

    ASSERT_EQ(mustBeFounded.size(), found2);
}

TEST(Strings, MatchRegexpFirst) {
    auto result1 = matchRegexpFirst(R"((\[[a-z]+\]))", "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");
    std::string mustBeFounded = "[abc]";

    ASSERT_STREQ(result1.c_str(), mustBeFounded.c_str());

    auto result2 = matchRegexpFirst(std::regex(R"((\[[a-z]+\]))"), "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");

    ASSERT_STREQ(result2.c_str(), mustBeFounded.c_str());
}

TEST(Strings, MatchRegexp) {
    const char *mustBeFounded = "[abc]";

    auto result1 = matchRegexp(R"((\[[a-z]+\]))", "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");
    ASSERT_EQ(2UL, result1.size());
    ASSERT_FALSE(result1.empty());
    ASSERT_STREQ(result1[1].c_str(), mustBeFounded);

    auto result2 = matchRegexp(std::regex(R"((\[[a-z]+\]))"), "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");
    ASSERT_EQ(2UL, result2.size());
    ASSERT_FALSE(result2.empty());
    ASSERT_STREQ(result2[1].c_str(), mustBeFounded);
}

#endif

TEST(Strings, SplitPair) {
    auto result1 = splitPair("abc,def", ",");
    ASSERT_STREQ("abc", result1.first.c_str());
    ASSERT_STREQ("def", result1.second.c_str());

    auto result2 = splitPair("abc, def", ",");
    ASSERT_STREQ("abc", result2.first.c_str());
    ASSERT_STREQ(" def", result2.second.c_str());

    auto result3 = splitPair("abc, ", ',');
    ASSERT_STREQ("abc", result3.first.c_str());
    ASSERT_STREQ(" ", result3.second.c_str());

    auto result4 = splitPair("abc", ",");
    ASSERT_STREQ("abc", result4.first.c_str());
    ASSERT_STREQ("", result4.second.c_str());

    auto result5 = splitPair("", ',');
    ASSERT_STREQ("", result5.first.c_str());
    ASSERT_STREQ("", result5.second.c_str());

    auto result6 = splitPair(",", ",");
    ASSERT_STREQ("", result6.first.c_str());
    ASSERT_STREQ("", result6.second.c_str());

    auto result7 = splitPair("abcdef", ',');
    ASSERT_STREQ("abcdef", result7.first.c_str());
    ASSERT_STREQ("", result7.second.c_str());
}

TEST(Strings, Split) {
    std::vector<std::string> mustBeInList{
        "abc", "def", "ghi", "jkl"
    };

    auto result1 = split("abc/def/ghi/jkl", '/');
    ASSERT_EQ(4UL, result1.size());
    size_t found1 = 0;
    for (auto &result: result1) {
        for (auto &m: mustBeInList) {
            if (result == m) {
                found1++;
                break;
            }
        }
    }

    ASSERT_EQ(found1, mustBeInList.size());

    auto result2 = split("abc/def/ghi/jkl", "/");
    ASSERT_EQ(4UL, result2.size());
    size_t found2 = 0;
    for (auto &result: result2) {
        for (auto &m: mustBeInList) {
            if (result == m) {
                found2++;
                break;
            }
        }
    }

    ASSERT_EQ(found2, mustBeInList.size());

    bool failed = false;
    try {
        // only one character can be passed
        auto result3 = split("abc/def/ghi/jkl", "def/");
    } catch (const std::runtime_error &) {
        failed = true;
    }

    ASSERT_TRUE(failed);

}
