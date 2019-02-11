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
    // unicode test
    ASSERT_TRUE(hasSubstringIgnoreCase("Ю", "ЮЮЮ"));
    ASSERT_FALSE(hasSubstringIgnoreCase("ЯЯЯ", "Л"));
}

TEST(Strings, HasSubstring) {
    ASSERT_TRUE(hasSubstring("AAA", "AAA"));
    ASSERT_TRUE(hasSubstring("A", "AAA"));
    ASSERT_FALSE(hasSubstring("a", "AAA"));
    ASSERT_FALSE(hasSubstring("bbb", "AAA"));
    ASSERT_FALSE(hasSubstring("aaaa", "A"));
    ASSERT_FALSE(hasSubstring("1", "A"));
    ASSERT_FALSE(hasSubstring("HTTP/1.1 400 Bad Request", "HTTP"));
    // unicode test
    ASSERT_TRUE(hasSubstring("Привет", "Привет! Как дела?"));
    ASSERT_FALSE(hasSubstring("Как дела?", "Привет"));
}

TEST(Strings, HasSubstringChar) {
    ASSERT_TRUE(hasSubstring('A', "AAA"));
    ASSERT_FALSE(hasSubstring('a', "AAA"));
    ASSERT_FALSE(hasSubstring('d', "{a,b,c}"));
}

#ifdef HAVE_REGEX_H
TEST(Strings, HasRegex) {
    ASSERT_TRUE(hasRegex("[0-9]", "abc123"));
    ASSERT_TRUE(hasRegex(rxns::regex("[a-z0-9]"), "abc123"));
    ASSERT_TRUE(hasRegex("HTTP", "HTTP/1.1 400 Bad Request"));
    ASSERT_FALSE(hasRegex("[!@#$]", "abc123"));
    ASSERT_TRUE(hasRegex("[а-я]", "привет"));
    ASSERT_FALSE(hasRegex("[а-я]", "hello"));
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

    auto result2 = matchAllRegexp(rxns::regex(R"((\[[a-z]+\]))"), "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");
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

    auto result2 = matchRegexpFirst(rxns::regex(R"((\[[a-z]+\]))"), "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");

    ASSERT_STREQ(result2.c_str(), mustBeFounded.c_str());
}

TEST(Strings, MatchRegexp) {
    const char *mustBeFounded = "[abc]";

    auto result1 = matchRegexp(R"((\[[a-z]+\]))", "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");
    ASSERT_EQ(2UL, result1.size());
    ASSERT_FALSE(result1.empty());
    ASSERT_STREQ(result1[1].c_str(), mustBeFounded);

    auto result2 = matchRegexp(rxns::regex(R"((\[[a-z]+\]))"), "[abc]\n\n[def]gagag[ghi]rg45w5[jkl]");
    ASSERT_EQ(2UL, result2.size());
    ASSERT_FALSE(result2.empty());
    ASSERT_STREQ(result2[1].c_str(), mustBeFounded);

    auto result3 = matchRegexp(R"(HTTP\/\d\.\d.(\d+).(.*))", "HTTP/1.1 400 Bad Request");
    ASSERT_EQ(3UL, result3.size());
    ASSERT_STREQ(result3[1].c_str(), "400");
    ASSERT_STREQ(result3[2].c_str(), "Bad Request");
}

TEST(Strings, MatchEmptyStrangeSmatchBehavior) {
    const std::string urlParseRegex =
        R"(([a-zA-Z]+)\:\/\/([a-zA-Z0-9\.\-_]+):?([0-9]{1,5})?(\/[a-zA-Z0-9\/\+\-\.\%\/_]*)\??([a-zA-Z0-9\-_\+\=\&\%\.]*))";
    auto res = toolboxpp::strings::matchRegexp(urlParseRegex, "http://wtf");

    ASSERT_EQ(0, res.size());
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

    auto result8 = splitPair("привет! как дела?", '!');
    ASSERT_STREQ("привет", result8.first.c_str());
    ASSERT_STREQ(" как дела?", result8.second.c_str());
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

    std::vector<std::string> mustBeInList3{
        "abc/", "ghi/jkl"
    };
    auto result3 = split("abc/def/ghi/jkl", "def/");
    ASSERT_EQ(result3.size(), mustBeInList3.size());

    size_t found3 = 0;
    for (auto &t: result3) {
        for (auto &m: mustBeInList3) {
            if (m == t) {
                found3++;
                break;
            }
        }
    }
    ASSERT_EQ(found3, mustBeInList3.size());
}

TEST(Strings, Replace) {
    std::string test1 = "hello world!";
    replace("hello", "hell or", test1);
    replace("world", "piece", test1);
    replace("!", "?", test1);
    ASSERT_STREQ("hell or piece?", test1.c_str());
}

TEST(Strings, ReplaceAll) {
    std::string test1 = "hello world!";
    replaceAll("o", "!", test1);
    ASSERT_STREQ("hell! w!rld!", test1.c_str());

    replaceAll("ll", "bb", test1);
    ASSERT_STREQ("hebb! w!rld!", test1.c_str());

    const std::vector<std::string> from1{"aaa", "bbb"};
    const std::vector<std::string> to1{"yyy", "zzz"};
    std::string test2 = "aaabbb";
    replaceAll(from1, to1, test2);
    ASSERT_STREQ("yyyzzz", test2.c_str());

    std::vector<std::string> from2{"aaa", "bbb"};
    std::vector<std::string> to2{"yyy", "zzz"};
    std::string test3 = "aaabbb";
    replaceAll(from2, to2, test3);
    ASSERT_STREQ("yyyzzz", test3.c_str());
}

TEST(Strings, RemoveSubstrings) {
    std::string test = "hello world!";
    removeSubstrings(test, " ");
    ASSERT_STREQ("helloworld!", test.c_str());
}

TEST(Strings, RemoveSubstringsVector) {
    std::string test = "hello world!";
    removeSubstrings(test, {"ll", "!", " "});
    ASSERT_STREQ("heoworld", test.c_str());
}

TEST(Strings, SubstringInverse) {
    std::string test = "hello world!";
    std::string res1 = substrInverse(test, ' ');
    ASSERT_STREQ("hello", res1.c_str());

    std::string res2 = substrInverse(test, ~' ');
    ASSERT_STREQ("world!", res2.c_str());
}

TEST(Strings, ToLower) {
    INIT_WCHAR_UNICODE();
    std::string test1 = "Upper Case";
    std::string res1 = toLower(test1);
    ASSERT_STREQ("upper case", res1.c_str());

    std::wstring test2 = L"Верхний Регистр";
    std::wstring res2 = toWLower(test2);
    std::wstring resp = L"верхний регистр";
    ASSERT_STREQ(resp.c_str(), res2.c_str());
}

TEST(Strings, ClipSubstring) {
    std::string test1 = "aaa bbb ccc";
    std::string res1 = clipSubstring(test1, "bbb", 7);
    ASSERT_STREQ("a bbb c", res1.c_str());

    std::string test2 = "aaabbbccc";
    std::string res2 = clipSubstring(test2, "bbb", 3);
    ASSERT_STREQ("bbb", res2.c_str());
}

TEST(Strings, Glue) {
    std::string g = ";";
    const std::vector<std::string> data1{
        "aaa",
        "bbb",
        "ccc"
    };

    std::string res1 = glue(g, data1);
    ASSERT_STREQ("aaa;bbb;ccc", res1.c_str());

    const std::vector<std::string> data2{
        "ааа",
        "ббб",
        "ввв"
    };
    std::string res2 = glue(g, data2);
    ASSERT_STREQ("ааа;ббб;ввв", res2.c_str());
}

TEST(Strings, EqualsICase) {
    const std::string t1 = "FirSt";
    const std::string t2 = "fIRsT";

    ASSERT_TRUE(equalsIgnoreCase(t1, t2));

    const std::string t3 = "ПервЫЙ";
    const std::string t4 = "пЕрВый";

    ASSERT_FALSE(equalsIgnoreCase(t3, t4));

    const std::wstring t5 = L"ПервЫЙ";
    const std::wstring t6 = L"первый";

    ASSERT_TRUE(equalsIgnoreWCase(t5, t6));

}
