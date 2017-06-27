/**
 * cpphelpers
 * CollectionHelper.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include "cpphelpers.h"

template<typename K, typename V>
std::vector<K> Collections::mapKeysToVector(const std::map<K, V> &map) {
	std::vector<K> v(map.size());
	for (auto &it = map.begin(); it != map.end(); ++it) {
		v.push_back(it->first);
	}

	return v;
}
template<typename K, typename V>
std::vector<V> Collections::mapValuesToVector(const std::map<K, V> &map) {
	std::vector<V> v(map.size());

	for (auto &it = map.begin(); it != map.end(); ++it) {
		v.push_back(it->first);
	}

	return v;
}

