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

#ifndef HAVE_REGEX_H
#warning "Regex is not supported by your compiler (GCC < 4.9). Some features will be unavailable. Please, use modern compiler or set -DBOOST_ROOT=/path/to/boost with Boost.Regex support"
#else
#ifdef HAVE_STL_REGEX
 #include <regex>
namespace rxns = std;
#else
 #error "STL and Boost Regex not found on your system"
#endif
#endif

#endif //TOOLBOXPP_TOOLBOXCONFIG_H
