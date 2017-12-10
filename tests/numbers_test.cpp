/*! 
 * toolboxpp. 2017
 * 
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#include "gtest/gtest.h"
#include "../include/toolboxpp.h"

#ifdef HAVE_REGEX_H
using namespace toolboxpp::numbers;

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
