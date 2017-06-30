/**
 * cpphelpers
 * SimpleProfiler.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include "cpphelpers.h"

void SimpleProfiler::begin(const std::string &tag) {
#ifdef CPPHELPERS_PROFILE_DISABLE
	return;
#endif
	if (profile.count(tag) > 0) return;

	lastTag = tag;
	profile[lastTag] = std::chrono::duration_cast<__millis>(__sys_clock::now().time_since_epoch());
}
void SimpleProfiler::end() {
#ifdef CPPHELPERS_PROFILE_DISABLE
	return;
#endif
}
void SimpleProfiler::end(const std::string &tag) {
#ifdef CPPHELPERS_PROFILE_DISABLE
	return;
#endif
}

void SimpleProfiler::clear() {
	profile.clear();
	lastTag.clear();
}

void SimpleProfiler::printProfile(std::ostream &out, bool clr) {
	std::deque<std::string> results;
	for (auto &p: profile) {
		std::stringstream ss;
		ss << p.first << ": " << p.second.count() << "ms";
		results.push_front(ss.str());
	}

	for (auto &p: results) {
		out << p << std::endl;
	}

	if (clr) clear();

}
