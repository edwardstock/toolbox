/// @file time_profiler.h
/// @brief Simple thread-safe time-based profiler singleton.

#ifndef TOOLBOXPP_TIME_PROFILER_H
#define TOOLBOXPP_TIME_PROFILER_H

#include "toolbox/toolbox_config.h"

#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>

namespace toolbox {

/// Thread-safe singleton profiler for measuring elapsed time between
/// begin()/end() calls identified by string tags.
class TOOLBOX_API time_profiler {
public:
    using hires_clock = std::chrono::high_resolution_clock;
    using hires_time_t = std::chrono::high_resolution_clock::time_point;
    using mutex_t = std::mutex;

    /// Return the global profiler instance.
    static time_profiler& get() {
        static time_profiler p;
        return p;
    }

    time_profiler(const time_profiler&) = delete;
    time_profiler(time_profiler&&) = delete;
    ~time_profiler();

    /// Remove all recorded timings.
    void clear();
    /// Return the number of active (started but not yet ended) timings.
    size_t size() const;
    /// Start a timing measurement for the given tag.
    /// @param tag unique identifier for this measurement
    void begin(std::string tag);
    /// Stop the timing measurement for the given tag and report elapsed seconds.
    /// @param tag identifier previously passed to begin()
    /// @param result if non-null, receives the elapsed time in seconds
    void end(std::string tag, double* result = nullptr);

private:
    std::unordered_map<std::string, hires_time_t> timings;
    mutable mutex_t lock;
    time_profiler() = default;
};

} // namespace toolbox

#endif // TOOLBOXPP_TIME_PROFILER_H
