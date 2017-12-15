/*!
 * toolboxpp. 2017
 *
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_TOOLBOXCONFIG_H
#define TOOLBOXPP_TOOLBOXCONFIG_H

#cmakedefine HAVE_REGEX_H 1
#cmakedefine HAVE_STL_REGEX 1
#cmakedefine HAVE_BOOST_REGEX 1

#define HAVE_REGEX_H 1
#define HAVE_BOOST_REGEX 1
//#define HAVE_STL_REGEX 1

#ifndef HAVE_REGEX_H
#warning "Regex is not supported by your compiler (GCC < 4.9)"
#else
#ifdef HAVE_STL_REGEX
 #include <regex>
namespace rxns = std;

#elif HAVE_BOOST_REGEX
 #include <boost/regex.hpp>
namespace rxns = boost;

#else
 #error "STL and Boost Regex not found on your system"
#endif
#endif

#endif //TOOLBOXPP_TOOLBOXCONFIG_H
