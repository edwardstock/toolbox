## 3.5.0

- Replaced sprintf with snprintf everywhere
- A bit cleanup in code style and naming
- Fixed installation of cmake-related files from \$prefix/lib/cmake/toolbox/*.cmake -> \$prefix/lib/cmake/*.cmake
- Added regex helpers:
  - `std::string remove_first_pattern(const rxns::regex& pattern, const std::string& source)`
  - `std::string remove_last_pattern(const rxns::regex& pattern, const std::string& source)`
  - `std::string find_last_pattern(const rxns::regex& pattern, const std::string& source)`
- Added string helpers:
  - `bool starts_with(const std::string& source, const std::string& prefix)`
  - `bool starts_with_any(const std::string& source, const Iterable& prefixes)`
  - `bool ends_with(const std::string& source, const std::string& suffix)`
  - `bool ends_with_any(const std::string& source, const Iterable& suffix)`
  - `std::string remove_prefix(const std::string& source, const std::string& prefix)`
  - `std::string remove_suffix(const std::string& source, const std::string& suffix)`
  - `std::string substring_before(const std::string& source, const std::string& delimiter)`
  - `std::string substring_after(const std::string& source, const std::string& delimiter)`
  - `std::string remove_substring_before(const std::string& source, const std::string& delimiter)`
  - `std::string remove_substring_after(const std::string& source, const std::string& delimiter)`
  - ```
    std::string join(
        const Iterable& source,
        const std::string& delimiter,
        const std::string& prefix,
        const std::string& suffix,
        const std::string& truncated,
        size_t limit,
        const Transformer& transformer
    )
    ```
    and multiple overloads
- Regex: unified function names, don't know why I've not done this before:
  - `find_all_pattern` -> `find_all_patterns`
  - `find_pattern_first` -> `find_first_pattern`
  - added new function `find_last_pattern`
- Data
  - added `slice<T>` class to work with slices of data (based on vector's const iterators)
  - added `basic_data<T>.take_slice*` methods to get slices of data
  - now `bytes_data` can parse hex string with prefix 0x or 0X
  - `bytes_data` now can parse and interpret correctly hex string with uneven length, for example 0xFFF won't be parsed as 0xFFFF; now it's 0x0FFF
  - added `prefix` argument to `bytes_data::to_hex` function
  - added `push_back(size_t)` to `bytes_data` to push back `size_t` value as bytes
  - added `push_back(const std::string&)` to `bytes_data` to push back string as bytes (not hex string)
  - added `push_back(const char*, size_t)` to `bytes_data` to push back string as bytes with defined length

## 3.4.0

- Migrated to conan 2.0
- Removed versioned library directory, now it's installing directly to `libdir`
- Removed deprecated `set_[min|max]_fractions` methods from `decimal_formatter`
- A few optimizations
- Fixed windows DLL building

## 3.3.0

- Added `take_range_n` with arguments (`from`, `length`)

## 3.2.4

- Fixed `basic_data.push_back(basic_data<T>&&)` behaviour, as it hadn't actually pushed data except first element
- Renamed `version` file to `version.info` as it have been conflicting with some windows headers

## 3.2.3

- Added calculating real precision for decimal_formatter to format more accurate result
- Marked as deprecated methods `set_[min|max]_fractions`, use instead `set_[min|max]_precision` in `decimal_formatter`
- TODO: add rounding modes to `decimal_formatter`

## 3.2.2

- Fixed conanfile package_info

## 3.2.1

- Cmake-style install targets, now supports `find_package(Toolbox)`,
  linking: `target_link_libraries(my_lib toolbox::toolbox)`

## 3.2.0

- Now boost is required always if cmake can't find `<regex>`

## 3.1.3
- Added support for compiling on gcc 4.8.5
- Disabled helper bytes literal operators for gcc < 5 (didn't tested on other compilers)
- Using boost if cmake can't find <regex> in include path (on test machines it's gcc < 6)
- Added strings methods for replacing string vector with just one string (old substr_replace_all supports it too, but second argument should be a vector with 1 element):
    - `void substr_replace_all(const std::vector<std::string>, const std::string&, std::string&)`
    - `std::string substr_replace_all_ret(const std::vector<std::string>, const std::string&, const std::string&)`

## 3.1.2
- Added support for compiling on gcc 4.9
- Added strings `trim` and `trim_ref` methods to remove characters "\t\n\r\x0B"
- Added generating pc.in file
