/*! 
 * toolbox. 2017
 * 
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#include "gtest/gtest.h"
#include <toolbox/strings/decimal_formatter.h>

#ifdef HAVE_REGEX_H
using namespace toolbox::numbers;

TEST(Numbers, IsReal) {
    std::string num1 = "256.464646958";
    std::string num2 = "1e-4";
    std::string num3 = "12E-4";
    std::string num4 = "-137,29";
    std::string num5 = "-225.29";
    std::string num6 = "NaN";
    std::string num7 = "111";
    std::string num8 = "192.168.1.1";

    ASSERT_TRUE(isReal(num1));
    ASSERT_TRUE(isReal(num2));
    ASSERT_TRUE(isReal(num3));
    ASSERT_FALSE(isReal(num4));
    ASSERT_TRUE(isReal(num5));
    ASSERT_FALSE(isReal(num6));
    ASSERT_TRUE(isReal(num7)); // integer is real too
    ASSERT_FALSE(isReal(num8));
}

TEST(Numbers, IsInteger) {
    std::string num1 = "256.958";
    std::string num2 = "1e-4";
    std::string num3 = "12E-4";
    std::string num4 = "-137";
    std::string num5 = "-225.29";
    std::string num6 = "NaN";
    std::string num7 = "111";
    std::string num8 = "1e+19";

    ASSERT_FALSE(isInteger(num1));
    ASSERT_FALSE(isInteger(num2));
    ASSERT_FALSE(isInteger(num3));
    ASSERT_TRUE(isInteger(num4));
    ASSERT_FALSE(isInteger(num5));
    ASSERT_FALSE(isInteger(num6));
    ASSERT_TRUE(isInteger(num7));
    ASSERT_TRUE(isInteger(num8));
}

#endif

TEST(Numbers, DecimalFormatter) {
    toolbox::strings::decimal_formatter form("1567948125.105");

    form.set_delimiter('\'');
    form.set_min_fractions(4);
    form.set_max_fractions(18);

    ASSERT_STREQ("1'567'948'125.1050", form.format().c_str());

    form.set_delimiter(' ');
    ASSERT_STREQ("1 567 948 125.1050", form.format().c_str());
}

TEST(Numbers, DecimalFormatterFractions) {
    toolbox::strings::decimal_formatter form("1567948125.10203040506070809080706050");

    form.set_delimiter('\'');
    form.set_min_fractions(4);
    form.set_max_fractions(18);

    ASSERT_STREQ("1'567'948'125.102030405060708090", form.format().c_str());
}

TEST(Numbers, SimpleFormatter) {
    ASSERT_STREQ("1 567 948 125.1050", toolbox::strings::decimal_formatter()("1567948125.105").c_str());
}

TEST(Numbers, SimpleFormatterMinFracts) {
    ASSERT_STREQ("1 567 948 125.1000", toolbox::strings::decimal_formatter()("1567948125.1").c_str());
}

TEST(Numbers, SimpleFormatterNoFractions) {
    ASSERT_STREQ("1 567 948 125.0000", toolbox::strings::decimal_formatter()("1567948125").c_str());
}