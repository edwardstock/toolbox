/// @file collections.h
/// @brief Helpers for working with standard associative containers.

#ifndef TOOLBOXPP_COLLECTIONS_H
#define TOOLBOXPP_COLLECTIONS_H

#include <map>
#include <unordered_map>
#include <vector>

namespace toolbox {
namespace collections {

/// Type trait that evaluates to true for std::map, std::unordered_map,
/// std::multimap, and std::unordered_multimap.
template<typename T>
struct is_map {
    static const bool value = false;
};
template<typename K, typename V>
struct is_map<std::map<K, V>> {
    static const bool value = true;
};
template<typename K, typename V>
struct is_map<std::unordered_map<K, V>> {
    static const bool value = true;
};
template<typename K, typename V>
struct is_map<std::unordered_multimap<K, V>> {
    static const bool value = true;
};
template<typename K, typename V>
struct is_map<std::multimap<K, V>> {
    static const bool value = true;
};

/// Extract all keys from a map-like container into a vector.
/// @tparam map_type any type satisfying is_map (std::map, std::unordered_map, etc.)
/// @param map source container
/// @return vector containing copies of all keys
template<typename map_type, typename key_type = typename map_type::key_type>
std::vector<key_type> map_keys_to_vector(const map_type& map) {
    static_assert(is_map<map_type>::value, "Only map can be passed");
    std::vector<key_type> v(map.size());
    size_t i = 0;
    for (auto& it : map) {
        v[i] = it.first;
        i++;
    }

    return v;
}

/// Extract all values from a map-like container into a vector.
/// @tparam map_type any type satisfying is_map (std::map, std::unordered_map, etc.)
/// @param map source container
/// @return vector containing copies of all values
template<typename map_type, typename value_type = typename map_type::value_type::second_type>
std::vector<value_type> map_vals_to_vector(const map_type& map) {
    static_assert(is_map<map_type>::value, "Only map can be passed");
    std::vector<value_type> v(map.size());
    size_t i = 0;
    for (auto& it : map) {
        v[i] = it.second;
        i++;
    }

    return v;
}
} // namespace collections
} // namespace toolbox

#endif // TOOLBOXPP_COLLECTIONS_H
