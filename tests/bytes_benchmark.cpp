/*!
 * toolbox.
 * bytes_benchmark.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include <gtest/gtest.h>
#include <toolbox/data/bytes_data.h>
#include <toolbox/log.h>
#include <toolbox/time_profiler.h>

using namespace toolbox::data;

TEST(BenchmarkBytesData, PushBack) {
    bytes_data data;
    toolbox::tlog::get().set_level(toolbox::LEVEL_ALL);
    const uint8_t val = (uint8_t) 0xFF;
    using tp = toolbox::time_profiler;
    const std::string tag = "Pushing back 100m elements";

    tp::get().begin(tag);

    for (size_t i = 0; i < 100'000'000; i++) {
        data.push_back(val);
    }

    tp::get().end(tag);
}

TEST(BenchmarkBytesData, WriteBack) {
    bytes_data data;

    using tp = toolbox::time_profiler;
    const std::string tag = "Write back 100m elements";
    const uint8_t val = (uint8_t) 0xFF;
    tp::get().begin(tag);

    for (size_t i = 0; i < 100'000; i++) {
        data.write_back(val);
    }

    tp::get().end(tag);
}

TEST(BenchmarkBytesData, WriteBytePosition) {
    bytes_data data;

    using tp = toolbox::time_profiler;
    const std::string tag = "Write 100m elements";
    const uint8_t val = (uint8_t) 0xFF;
    tp::get().begin(tag);

    for (size_t i = 0; i < 100'000; i++) {
        data.write(i, val);
    }

    tp::get().end(tag);
}

TEST(BenchmarkBytesData, AllocatedWriteBytePosition) {
    bytes_data data(100'000);

    using tp = toolbox::time_profiler;
    const std::string tag = "Write 100m elements";
    const uint8_t val = (uint8_t) 0xFF;
    tp::get().begin(tag);

    for (size_t i = 0; i < 100'000; i++) {
        data.write(i, val);
    }

    tp::get().end(tag);
}

TEST(BenchmarkBytesData, WriteReservedBytePosition) {
    bytes_data data;
    data.get().reserve(100'000);

    using tp = toolbox::time_profiler;
    const std::string tag = "Write 100m elements";
    const uint8_t val = (uint8_t) 0xFF;
    tp::get().begin(tag);

    for (size_t i = 0; i < 100'000; i++) {
        data.write(i, val);
    }

    tp::get().end(tag);
}