## 3.5.1

### Build system
- **Removed Conan** — deleted `conanfile.py`, `deploy.py`, `dockerpack.yml`, `publish_local.sh`, `test_package/`, `modules/conan_provider.cmake`, `modules/conan_fallback.cmake`. Dependencies are now fetched via CMake `FetchContent` only.
- **Removed legacy scripts** — deleted `cfg/package_make.sh`, `cfg/package_upload.sh`, `cfg/version.in`, `modules/FindLinuxPlatform.cmake`.
- **CMakeLists.txt rewrite**:
  - Minimum CMake version bumped from 3.19 to 3.21.
  - Added `HOMEPAGE_URL`, `CMAKE_CXX_STANDARD_REQUIRED`, `CMAKE_CXX_EXTENSIONS OFF`.
  - Added `CMP0092` policy (no default `/W3` on MSVC).
  - `toolbox_BUILD_SHARED_LIBS` now defaults to `BUILD_SHARED_LIBS` instead of hardcoded `OFF`.
  - Added namespaced alias `toolbox::toolbox` so FetchContent and find_package consumers use the same target.
  - `target_compile_features(PUBLIC cxx_std_17)` propagates standard requirement to consumers.
  - `POSITION_INDEPENDENT_CODE ON` always set (safe static→shared linking).
  - `_CRT_SECURE_NO_WARNINGS` moved from global `add_definitions` to MSVC-only generator expression.
  - `toolbox_config.h` now generated into `CMAKE_CURRENT_BINARY_DIR/include/` instead of source tree.
  - Shared library gets proper `VERSION`/`SOVERSION` properties; Windows DLL versioning with post-build copy.
  - Test target links `GTest::gtest` (imported target) instead of plain `gtest`.
  - Benchmarks split into separate `toolbox-bench` target (excluded from `ALL`).
  - `INSTALL_GTEST OFF` to avoid polluting consumer installs.
- **CMakePresets.json** — added presets: `debug`, `release`, `debug-shared`, `release-shared`.
- **CI** — replaced multi-file GitHub Actions (`build-cmake-target`, `run-tests`, `setup-conan`, `setup-python`, `build_conan.yml`, `check_pr.yml`) with a single `build.yml` workflow using CMake presets on Linux/macOS/Windows.
- **cfg/toolbox-config.cmake.in** — simplified: now just includes the exported targets file + `check_required_components`, removed manual `set_and_check` variables.
- **cfg/toolbox.pc.in** — fixed variables: `@INSTALL_LIB_DIR@` → `@CMAKE_INSTALL_LIBDIR@`, `include` → `@CMAKE_INSTALL_INCLUDEDIR@`, `@CMAKE_PROJECT_VERSION@` → `@PROJECT_VERSION@`.
- **modules/package.cmake** — rewritten for modern CMake install/export.
- **modules/compilers_flags.cmake** — refactored.

### Bug fixes
- **UB fix**: `to_num_any` and `to_num_any_size` — `uint8_t` was shifted by ≥32 bits without cast, causing undefined behavior due to `int` promotion. Added explicit `static_cast` before shifts.
- **UB fix**: `write_batch` — incorrect resize calculation (`minPos + vals.size()` instead of `maxPos + 1`) caused out-of-bounds writes with sparse positions.
- **UB fix**: `write(pos, T val)` — resized to `size() + 1` instead of `pos + 1`, causing out-of-bounds write when `pos >> size()`.
- **UB fix**: `push_back(const T*, len)` and `write(pos, const T*, length)` — replaced `memcpy` with `std::copy`/`insert` to fix incorrect byte/element math for `sizeof(T) > 1` and UB on non-trivially-copyable types.
- **Bug fix**: `bytes_data::clear()` now calls `m_data.clear()` after secure erase, matching the `basic_data::clear()` contract (size goes to 0). Previously only zeroed memory without shrinking.
- **Bug fix**: `operator>>` — reordered `clear()`/`resize()` to avoid reading into an invalidated buffer.
- **Bug fix**: `converter::operator()` / `converter_vec::operator()` — added missing `return Out{}` to avoid UB on non-void function without return.
- **Bug fix**: `operator[]` called throwing `at()` while marked `noexcept`, causing `std::terminate` on out-of-bounds. Now uses unchecked `m_data[]` (standard STL convention).

### Improvements
- `bytes_array::clear()` — added override that zero-fills data while keeping fixed size N.
- `slice::m_debug` — wrapped in `#ifndef NDEBUG` so release builds don't copy data in a non-owning view.
- `bytes_data::to_hex()` — split into non-virtual overload (no args) + virtual method with `prefix` param to avoid default arguments on virtual methods.
- `bytes_data::clear()` — marked `override`, removed redundant `virtual`.
- `basic_data::clear()` — made `virtual` (was non-virtual).
- `basic_data::empty()` — marked `[[nodiscard]]`.
- Doxygen-style documentation across all public headers.

## 3.5.0
- C++ standard now is 17
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
  - added template `push_back(T)` to `bytes_data` for any integral type and removed other overloads
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
