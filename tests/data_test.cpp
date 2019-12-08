//
// Created by edward on 13.11.2019.
//

#include <fstream>
#include <gtest/gtest.h>
#include <stack>
#include <toolbox/data.hpp>
#include <toolbox/data/literals.h>
#include <toolbox/data/transformers.h>

using namespace toolbox::data;
using namespace toolbox::data::literals;

TEST(BytesData, Write) {
    bytes_data d1;
    d1.write(0, (uint8_t) 0x05);
    ASSERT_EQ(1, d1.size());
    ASSERT_EQ(0x05, d1.at(0));

    d1.write(1, (uint16_t) 0x0101);
    ASSERT_EQ(3, d1.size());
    ASSERT_EQ((uint8_t) 0x01, d1.at(1));
    ASSERT_EQ((uint8_t) 0x01, d1.at(2));

    d1.write(3, (uint32_t) 20);
    ASSERT_EQ(3 + 4, d1.size());
    ASSERT_EQ(20u >> 24u, d1.at(3));
    ASSERT_EQ(20u >> 16u, d1.at(4));
    ASSERT_EQ(20u >> 8u, d1.at(5));
    ASSERT_EQ(20u & 0xFFu, d1.at(6));

    d1.write(7, (uint64_t) 40ULL);
    ASSERT_EQ(3 + 4 + 8, d1.size());
    ASSERT_EQ(40ULL >> 56u, d1.at(7));
    ASSERT_EQ(40ULL >> 48u, d1.at(8));
    ASSERT_EQ(40ULL >> 40u, d1.at(9));
    ASSERT_EQ(40ULL >> 32u, d1.at(10));
    ASSERT_EQ(40ULL >> 24u, d1.at(11));
    ASSERT_EQ(40ULL >> 16u, d1.at(12));
    ASSERT_EQ(40ULL >> 8u, d1.at(13));
    ASSERT_EQ(40ULL & 0xFFu, d1.at(14));

    size_t n = d1.size();
    std::vector<uint8_t> tmp1 = {0x00, 0x01, 0x02, 0x03};
    d1.write(n, tmp1);

    ASSERT_EQ(n + tmp1.size(), d1.size());
    ASSERT_EQ(0x00u, d1.at(15));
    ASSERT_EQ(0x01u, d1.at(16));
    ASSERT_EQ(0x02u, d1.at(17));
    ASSERT_EQ(0x03u, d1.at(18));

    // 19 items now
    //    size_t s2 = n + tmp1.size();

    std::vector<uint8_t> tmp2(64);
    std::for_each(tmp2.begin(), tmp2.end(), [](uint8_t& v) { v = 0xFFu; });

    ASSERT_EQ(64, tmp2.size());

    // ok, let write 64 byte vector into the 9th d index
    d1.write(10, tmp2);

    ASSERT_EQ(74, d1.size());
    ASSERT_EQ(0xFFu, d1.at(10));
    ASSERT_EQ(0xFFu, d1.at(73));

    // now replace making new tail with new data starting at position=10
    d1.write_tail(10, tmp2);
    ASSERT_EQ(74, d1.size());
    ASSERT_EQ(0xFFu, d1.at(10));
    ASSERT_EQ(0xFFu, d1.at(73));
    // now we've removed old tail with new
    // scheme:
    // [ 0 - 18 old data ]
    // we're inserted into the 10 index 64 bytes, and now we have
    // [ 0 - 9 old data, 10 - 73 new 64 bytes data ]
    // and size of BytesData was changed to 74

    uint8_t t2[4] = {1, 2, 3, 4};
    d1.push_back(t2, 4);
    ASSERT_EQ(78, d1.size());
    ASSERT_EQ(1, d1.at(74));
    ASSERT_EQ(2, d1.at(75));
    ASSERT_EQ(3, d1.at(76));
    ASSERT_EQ(4, d1.at(77));

    d1.write(74, t2, 4);
    // if we're inserting out of bounds, than buffer resizes
    ASSERT_EQ(78, d1.size());
    ASSERT_EQ(1, d1.at(74));
    ASSERT_EQ(2, d1.at(75));
    ASSERT_EQ(3, d1.at(76));
    ASSERT_EQ(4, d1.at(77));

    const bytes_data d2 = d1;
    d1.clear();
    d1.resize(0);
    d1.write(0, d2);
    ASSERT_EQ(d1.size(), d2.size());
    for (size_t i = 0; i < d1.size(); i++) {
        ASSERT_EQ(d1.at(i), d2.at(i));
    }

    d1.clear();
    d1.resize(0);

    // write over size, container automatically resize backend to new size
    d1.write(150, t2, 4);

    bytes_data d3 = d1;
    d1.clear();
    d1.resize(0);
    d1.write(0, d3);
    ASSERT_EQ(d1.size(), d3.size());
    for (size_t i = 0; i < d1.size(); i++) {
        ASSERT_EQ(d1.at(i), d3.at(i));
    }

    bytes_data d4 = d1;
    d1.clear();
    d1.resize(0);
    d1.write(0, std::move(d3));
    ASSERT_EQ(d1.size(), d3.size());
    for (size_t i = 0; i < d1.size(); i++) {
        ASSERT_EQ(d1.at(i), d3.at(i));
    }
}

TEST(BytesData, Resize) {
    bytes_data d(32);
    for (int i = 0; i < 32; i++) {
        if (i < 16) {
            d.write(i, (uint8_t) 0x7F);
        } else {
            d.write(i, (uint8_t) 0xFF);
        }
    }

    bytes_data target(d.take_range_from(d.size() / 2));
    ASSERT_EQ(16, target.size());
    d.resize(16);
    ASSERT_EQ(16, d.size());
    ASSERT_EQ(0xFF_byte, target.at(0));
    ASSERT_EQ(0xFF_byte, target.at(15));
    ASSERT_EQ(0x7F_byte, d.at(0));
    ASSERT_EQ(0x7F_byte, d.at(15));
}

TEST(BytesData, PopBackTo) {
    bytes_buffer d(32);
    for (int i = 0; i < 32; i++) {
        if (i < 16) {
            d.write(i, (uint8_t) 0x7F);
        } else {
            d.write(i, (uint8_t) 0xFF);
        }
    }
    bytes_data target(16);
    d.pop_back_to(target);
    ASSERT_EQ(16, target.size());
    ASSERT_EQ(16, d.size());
    ASSERT_EQ(0xFF_byte, target.at(0));
    ASSERT_EQ(0xFF_byte, target.at(15));
    ASSERT_EQ(0x7F_byte, d.at(0));
    ASSERT_EQ(0x7F_byte, d.at(15));

    bytes_data target2(8);
    d.pop_back_to(8, target2);
    ASSERT_EQ(8, target2.size());
    ASSERT_EQ(8, d.size());

    bytes_data target3(16);
    d.pop_back_to(target3);
    ASSERT_EQ(0, d.size());
    ASSERT_EQ(16, target3.size());
    ASSERT_EQ(0x7F_byte, target3.at(0));
    ASSERT_EQ(0x7F_byte, target3.at(7));
    ASSERT_EQ(0x00_byte, target3.at(15));
}

TEST(BytesData, InsertIterator) {
    bytes_data d;
    std::vector<uint8_t> data = {1, 2, 3, 4};
    size_t n = 0;
    n = d.write(d.begin(), data.begin(), data.end());
    ASSERT_EQ(4, d.size());
    ASSERT_EQ(4, n);

    std::vector<uint8_t> data2 = {5, 6, 7, 8};
    n = d.write(2, data2.begin(), data2.end());
    ASSERT_EQ(6, d.size());
    ASSERT_EQ(4, n);

    d.clear();
    d.resize(32);
    std::fill(d.begin(), d.end(), 0x80_byte);
    n = d.write(4, data2.begin(), data2.end());
    ASSERT_EQ(4, n);
    ASSERT_EQ(32, d.size());
}

TEST(BytesData, PushBackIterators) {
    bytes_data d;
    std::vector<uint8_t> src = {5, 6, 7, 8};
    d.clear();
    d.resize(0);
    d.push_back(src.begin(), src.end());
    ASSERT_EQ(4, d.size());
    ASSERT_EQ(5, d.at(0));
    ASSERT_EQ(6, d.at(1));
    ASSERT_EQ(7, d.at(2));
    ASSERT_EQ(8, d.at(3));

    const auto data3 = src;
    d.clear();
    d.resize(0);
    d.push_back(data3.begin(), data3.end());
    ASSERT_EQ(4, d.size());
    ASSERT_EQ(5, d.at(0));
    ASSERT_EQ(6, d.at(1));
    ASSERT_EQ(7, d.at(2));
    ASSERT_EQ(8, d.at(3));

    bytes_data nd;
    nd.push_back(d);

    ASSERT_EQ(d.size(), nd.size());
    ASSERT_EQ(d, nd);
    ASSERT_TRUE(d == nd);

    const bytes_data another("aaff");
    d.clear();
    d.resize(0);

    d.push_back(another);
    ASSERT_EQ(2, d.size());
    ASSERT_EQ(0xAA, d.at(0));
    ASSERT_EQ(0xFF, d.at(1));

    const bytes_data another_move("aaff");
    d.clear();
    d.resize(0);

    d.push_back(std::move(another));
    ASSERT_EQ(2, d.size());
    ASSERT_EQ(0xAA, d.at(0));
    ASSERT_EQ(0xFF, d.at(1));
}

TEST(BytesData, WriteBatch) {
    bytes_data d;
    std::map<size_t, uint8_t> empty_data;

    d.write_batch(std::move(empty_data));
    ASSERT_EQ(0, d.size());
}

TEST(BytesData, Ranges) {
    bytes_data d;
    std::vector<uint8_t> tmp = {1, 2, 3, 4};
    d.push_back(tmp);

    ASSERT_EQ(1, d.at(0));
    ASSERT_EQ(2, d.at(1));
    ASSERT_EQ(3, d.at(2));
    ASSERT_EQ(4, d.at(3));

    auto slice1 = d.take_first(2);
    ASSERT_EQ(2, slice1.size());
    ASSERT_EQ(1, slice1.at(0));
    ASSERT_EQ(2, slice1.at(1));

    auto slice2 = d.take_last(2);
    ASSERT_EQ(2, slice2.size());
    ASSERT_EQ(3, slice2.at(0));
    ASSERT_EQ(4, slice2.at(1));

    auto slice3 = d.take_range(0, 2);
    ASSERT_EQ(2, slice3.size());
    ASSERT_EQ(1, slice3.at(0));
    ASSERT_EQ(2, slice3.at(1));

    auto slice4 = d.take_range(3, 4);
    ASSERT_EQ(1, slice4.size());
    ASSERT_EQ(4, slice4.at(0));

    auto slice5 = d.take_range(2, 4);
    ASSERT_EQ(2, slice5.size());
    ASSERT_EQ(3, slice5.at(0));
    ASSERT_EQ(4, slice5.at(1));

    auto slice6 = d.take_range(4, 4);
    ASSERT_EQ(0, slice6.size());

    bool s7threw = false;
    try {
        auto slice7 = d.take_range(5, 4);
        ASSERT_EQ(0, slice7.size());
    } catch (const std::out_of_range& e) {
        s7threw = true;
    }
    ASSERT_TRUE(s7threw);

    bool s8threw = false;
    try {
        auto slice8 = d.take_range(5, 5);
        ASSERT_EQ(0, slice8.size());
    } catch (const std::out_of_range& e) {
        s8threw = true;
    }
    ASSERT_TRUE(s8threw);

    bool s9threw = false;
    try {
        auto slice9 = d.take_range(0, 10);
        ASSERT_EQ(0, slice9.size());
    } catch (const std::out_of_range& e) {
        s9threw = true;
    }
    ASSERT_TRUE(s9threw);

    bool s10threw = false;
    try {
        auto slice10 = d.take_range(500, 0);
        ASSERT_EQ(0, slice10.size());
    } catch (const std::out_of_range& e) {
        s10threw = true;
    }
    ASSERT_TRUE(s10threw);
}

TEST(BytesData, push_back) {
    bytes_data d1;
    d1.push_back((uint8_t) 0x05);
    ASSERT_EQ(1, d1.size());

    d1.push_back((uint16_t) 0x0101);
    ASSERT_EQ(3, d1.size());

    d1.push_back((uint32_t) 0x20);
    ASSERT_EQ(3 + 4, d1.size());

    d1.push_back((uint64_t) 0x40ULL);
    ASSERT_EQ(3 + 4 + 8, d1.size());

    size_t n = d1.size();
    std::vector<uint8_t> tmp1 = {0x0, 0x01, 0x02, 0x03};
    d1.push_back(tmp1);
    ASSERT_EQ(n + tmp1.size(), d1.size());
}

TEST(Buffer, PopFrontTo) {
    bytes_buffer buffer(256);
    ASSERT_EQ(256, buffer.size());
    std::fill(buffer.begin(), buffer.end(), 0xFFu);

    bytes_buffer exData(74);
    std::fill(exData.begin(), exData.begin() + 10, 0x80u);

    ASSERT_EQ(0x80u, exData.at(0));
    ASSERT_EQ(0x80u, exData.at(9));
    ASSERT_EQ(0x0u, exData.at(exData.size() - 1));

    size_t seq = 1;
    size_t rlen = 64;
    size_t left = 256;
    size_t written = 0;

    while (!buffer.empty()) {
        // stack write
        // getting from first
        // writing to end
        written += buffer.pop_front_to(rlen, exData.begin() + 10, exData);
        left -= rlen;
        ASSERT_EQ(left, buffer.size());
        ASSERT_EQ(74, exData.size());
        seq++;
    }

    ASSERT_EQ(256, written);
    ASSERT_EQ(0, buffer.size());
    ASSERT_EQ(74, exData.size());
}

TEST(Buffer, PopFrontLimit) {
    bytes_data chunk(64);
    std::fill(chunk.begin(), chunk.end(), 0x80_byte);

    bytes_buffer buffer(256);
    std::fill(buffer.begin(), buffer.end(), 0xFF_byte);

    uint16_t seq = 0;

    ASSERT_EQ(64, chunk.size());
    ASSERT_EQ(256, buffer.size());

    size_t left = 256;

    while (!buffer.empty()) {
        chunk.write(3, seq);
        size_t n = buffer.pop_front_to(5, chunk);
        std::cout << "Written to chunk: " << n << std::endl;
        std::cout << "Size of chunk: " << chunk.size() << std::endl;

        left -= n;

        ASSERT_EQ(left, buffer.size());
        ASSERT_EQ(64, chunk.size());

        seq++;
    }
}

TEST(BytesData, RealCase1) {
    bytes_data buffer(64);
    // 01 01 05 00 00 00 05 00 01 00 90 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    // 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    buffer.write(0, 0x01_byte);
    buffer.write(1, 0x01_byte);
    buffer.write(2, 0x05_byte);
    buffer.write(3, 0x00_byte);
    buffer.write(4, 0x00_byte);
    buffer.write(5, 0x00_byte);
    buffer.write(6, 0x05_byte);
    buffer.write(7, 0x00_byte);
    buffer.write(8, 0x01_byte);
    buffer.write(9, 0x00_byte);
    buffer.write(10, 0x90_byte);
    buffer.write(11, 0x00_byte);
    for (int i = 12; i < 64; i++) {
        buffer.write(i, 0x0_byte);
    }

    // parse header 5 bytes
    // 2 bytes
    auto channelId = buffer.to_num<uint16_t>(0);
    // 1 byte
    uint8_t commandTag = buffer.at(2);
    // 2 bytes
    auto cseq = buffer.to_num<uint16_t>(3);

    ASSERT_EQ(0x0101_dbyte, channelId);
    ASSERT_EQ(0x05_dbyte, commandTag);
    ASSERT_EQ(0x00, cseq);

    // data len
    ASSERT_EQ(0x05_dbyte, buffer.at(6));

    // version
    ASSERT_EQ(0x00, buffer.at(7));
    ASSERT_EQ(0x01_dbyte, buffer.at(8));
    ASSERT_EQ(0x00, buffer.at(9));

    uint16_t statusCode = buffer.to_num<uint16_t>(10);
    ASSERT_EQ(0x9000_dbyte, statusCode);
}

TEST(BytesData, WriteReadNumber) {
    bytes_data d(32);
    d.write(0, (uint16_t) 0x8080u);

    uint8_t buf[4] = {0xFFu, 0xFFu, 0xFFu, 0xFFu};

    uint32_t num = (((uint32_t) buf[0]) << 24u) | (((uint32_t) buf[1]) << 16u) | (((uint32_t) buf[2]) << 8u) |
                   (((uint32_t) buf[3]));
    ASSERT_EQ(UINT32_MAX, num);

    ASSERT_EQ(32, d.size());
    ASSERT_EQ((uint16_t) 0x8080u, d.to_num<uint16_t>(0));

    d.write(2, (uint32_t) UINT32_MAX);
    ASSERT_EQ(0xFFFFFFFFU, UINT32_MAX);
    ASSERT_EQ(32, d.size());
    ASSERT_EQ(0xFFu, d.at(2));
    ASSERT_EQ(0xFFu, d.at(3));
    ASSERT_EQ(0xFFu, d.at(4));
    ASSERT_EQ(0xFFu, d.at(5));
    ASSERT_EQ(0xFFFFFFFFU, d.to_num<uint32_t>(2));

    d.write(6, (uint64_t) UINT64_MAX);
    ASSERT_EQ(32, d.size());
    ASSERT_EQ(0xFFu, d.at(6));
    ASSERT_EQ(0xFFu, d.at(7));
    ASSERT_EQ(0xFFu, d.at(8));
    ASSERT_EQ(0xFFu, d.at(9));
    ASSERT_EQ(0xFFu, d.at(10));
    ASSERT_EQ(0xFFu, d.at(11));
    ASSERT_EQ(0xFFu, d.at(12));
    ASSERT_EQ(0xFFu, d.at(13));
    ASSERT_EQ((uint64_t) UINT64_MAX, d.to_num<uint64_t>(6));

    d.write(14, (uint8_t) 0xFFu);
    ASSERT_EQ(32, d.size());
    ASSERT_EQ((uint8_t) 0xFFu, d.to_num<uint8_t>(14));

    d.clear();
    d.resize(2);
    d.write(0, (uint16_t) 257);
    ASSERT_EQ(2, d.size());
    ASSERT_EQ((uint16_t) 257, d.to_num<uint16_t>());
}

TEST(BytesData, ToNumAny) {
    bytes_data d;
    d.write(0, UINT64_MAX);

    uint64_t r1 = d.to_num_any(0, sizeof(uint64_t));
    ASSERT_EQ(UINT64_MAX, r1);

    d.clear();
    d.write(0, 0xFF_byte);

    auto r2 = d.to_num_any_size<uint8_t>(0, 0 + 1);
    ASSERT_EQ(0xFF_byte, r2);

    d.clear();
    d.write(0, 250);

    auto r3 = d.to_num_any_size<uint32_t>(0);
    ASSERT_EQ(250_byte, r3);

    d.clear();
    d.write(0, (uint32_t) UINT16_MAX);
    auto r5 = d.to_num_any_size<uint16_t>(0, 0 + sizeof(uint32_t));
    ASSERT_EQ(UINT16_MAX, r5);

    d.clear();
    d.write(0, (uint32_t) UINT16_MAX);
    auto r6 = d.to_num_any_size<uint32_t, uint16_t>(0);
    ASSERT_EQ(UINT16_MAX, r6);

    d.clear();
    d.write(0, 250_byte);
    auto r4 = d.to_num_any_size<uint32_t>(0);
    ASSERT_NE((uint32_t) 250, r4);
    // r4 case works, only if data container has only 1 byte size, but there already 8,
    // so, if we would try to convert 0-4 indexes to uint32_t, we'll get little-endian encoding, toolbox does not
    // support little endian. If you need to convert between values, set 2 types explicitly, like below

    auto r7 = d.to_num_any_size<uint8_t, uint32_t>(0);
    ASSERT_EQ((uint32_t) 250, r7);

    auto r8 = d.to_num_any(0, 8);
    // the same issue, use explicit types
    ASSERT_NE(250ull, r8);

    d.clear();
    d.resize(0);

    d.write_back((uint8_t) 0xFF);
    d.write_back((uint8_t) 0xFF);

    uint64_t val = d.to_num_any(0, 2);
    ASSERT_EQ(2, d.size());
    ASSERT_EQ(0xFFFF, val);

    uint64_t val_no_bounds = d.to_num_any();
    ASSERT_EQ(0xFFFF, val_no_bounds);
}

TEST(BytesData, CopyCtor) {
    std::stack<bytes_data> merge;
    bytes_data d1;
    d1.write_back((uint8_t) 1);
    d1.write_back((uint8_t) 2);
    d1.write_back((uint8_t) 3);

    merge.push(d1);
    std::cout << "Size of stack:" << merge.size() << "\n";
    std::cout << "Top of stack: " << merge.top().size() << "\n";

    const bytes_data a("ff00ff00");
    bytes_data b(a);

    ASSERT_EQ(4, a.size());
    ASSERT_EQ(4, b.size());
    ASSERT_EQ(a, b);
    ASSERT_TRUE(a == b);
}

TEST(BytesData, Map) {
    bytes_data d = {0x01_byte, 0x02_byte, 0x01_byte, 0x02_byte};

    ASSERT_EQ(4, d.size());
    ASSERT_EQ(0x01_byte, d.at(0));
    ASSERT_EQ(0x02_byte, d.at(d.size() - 1));

    d.map([](uint8_t val) { return (uint8_t)(val * 2); });

    ASSERT_EQ(4, d.size());
    ASSERT_EQ(0x02_byte, d.at(0));
    ASSERT_EQ(0x04_byte, d.at(d.size() - 1));
}

TEST(BytesData, SwitchMap) {
    bytes_data d = {0x01_byte, 0x02_byte, 0x01_byte, 0x02_byte};

    ASSERT_EQ(4, d.size());
    ASSERT_EQ(0x01_byte, d.at(0));
    ASSERT_EQ(0x02_byte, d.at(d.size() - 1));

    d.switch_map([](std::vector<uint8_t> old) {
        std::vector<uint8_t> out;
        out.resize(old.size());
        std::transform(old.begin(), old.end(), out.begin(), [](uint8_t val) { return val * 4; });

        return out;
    });

    ASSERT_EQ(4, d.size());
    ASSERT_EQ(0x04_byte, d.at(0));
    ASSERT_EQ(0x08_byte, d.at(d.size() - 1));
}

TEST(BytesData, SwitchMapReduce) {
    bytes_data d = {0x01_byte, 0x02_byte, 0x01_byte, 0x02_byte};

    ASSERT_EQ(4, d.size());
    ASSERT_EQ(0x01_byte, d.at(0));
    ASSERT_EQ(0x02_byte, d.at(d.size() - 1));

    d.switch_map([](std::vector<uint8_t> old) {
        std::vector<uint8_t> out;
        out.resize(old.size() - 2);
        std::transform(old.begin(), old.begin() + 2, out.begin(), [](uint8_t val) { return val * 4; });

        return out;
    });

    ASSERT_EQ(2, d.size());
    ASSERT_EQ(0x04_byte, d.at(0));
    ASSERT_EQ(0x08_byte, d.at(d.size() - 1));
}

TEST(BytesData, SwitchMapCopy) {
    bytes_data d = {0x01_byte, 0x02_byte, 0x01_byte, 0x02_byte};

    ASSERT_EQ(4, d.size());
    ASSERT_EQ(0x01_byte, d.at(0));
    ASSERT_EQ(0x02_byte, d.at(3));

    auto res = d.switch_map_c([](std::vector<uint8_t> old) {
        std::vector<uint8_t> out(old.size());
        std::transform(old.begin(), old.end(), out.begin(), [](uint8_t val) {
            return (uint8_t)(val * 2);
        });

        return out;
    });

    ASSERT_EQ(4, res.size());
    ASSERT_EQ(0x02_byte, res.at(0));
    ASSERT_EQ(0x04_byte, res.at(1));
    ASSERT_EQ(0x02_byte, res.at(2));
    ASSERT_EQ(0x04_byte, res.at(3));

    ASSERT_EQ(4, d.size());
    ASSERT_EQ(0x01_byte, d.at(0));
    ASSERT_EQ(0x02_byte, d.at(1));
    ASSERT_EQ(0x01_byte, d.at(2));
    ASSERT_EQ(0x02_byte, d.at(3));
}

TEST(BytesData, MapToStrings) {
    bytes_data d = bytes_data::from_string_raw("hello map to");
    auto string_d = d.map_to<char>([](uint8_t val) {
        return (char) val;
    });
    ASSERT_EQ(d.size(), string_d.size());
    std::string res = std::string(string_d.data(), string_d.data() + string_d.size());

    ASSERT_STREQ("hello map to", res.c_str());
    std::cout << res << std::endl;
}

TEST(BytesData, InitializerVectors) {
    bytes_data d = {
        {0x0, 0x0, 0x0, 'a'},
        {0x1, 0x1, 0x1, 'b'}};

    ASSERT_EQ(8, d.size());
    ASSERT_EQ(0x0, d.at(0));
    ASSERT_EQ(0x0, d.at(2));
    ASSERT_EQ('a', d.at(3));
    ASSERT_EQ(0x1, d.at(4));
    ASSERT_EQ(0x1, d.at(6));
    ASSERT_EQ('b', d.at(7));
}

TEST(BytesData, Filter) {
    bytes_data d = {0x0, 0x0, 0x0, 0x1};
    auto filter_no_zeroes = [](uint8_t v) {
        return v != 0x0;
    };

    d.filter(filter_no_zeroes);
    ASSERT_EQ(1, d.size());
    ASSERT_EQ(0x1, d.at(0));

    bytes_data d2 = {0x1, 0x0, 0x1, 0x0};
    bytes_data res = d2.filter_c(filter_no_zeroes);
    ASSERT_EQ(4, d2.size());
    ASSERT_EQ(0x1, d2.at(0));
    ASSERT_EQ(0x0, d2.at(1));
    ASSERT_EQ(0x1, d2.at(2));
    ASSERT_EQ(0x0, d2.at(3));

    ASSERT_EQ(2, res.size());
    ASSERT_EQ(0x1, res.at(0));
    ASSERT_EQ(0x1, res.at(1));
}

TEST(BytesData, Base64Transform) {
    std::string target = "aGVsbG8gd29ybGQ=";
    std::string source = "hello world";

    bytes_data d = bytes_data::from_string_raw(source);
    d.switch_map(to_base_64);
    ASSERT_EQ(target.size(), d.size());
    ASSERT_STREQ(target.c_str(), d.to_string().c_str());

    d.switch_map(from_base_64);
    ASSERT_EQ(source.size(), d.size());
    ASSERT_STREQ(source.c_str(), d.to_string().c_str());
}

TEST(BytesData, OutputStream) {
    bytes_data data = bytes_data::from_string_raw("hello streams");
    std::stringstream ss;
    ss << data;

    ASSERT_STREQ("hello streams", ss.str().c_str());
}

TEST(BytesData, InputStream) {
    std::stringstream ss;
    ss << "abc";
    bytes_data d;
    ss >> d;

    ASSERT_EQ(3, d.size());
    ASSERT_EQ('a', d.at(0));
    ASSERT_EQ('b', d.at(1));
    ASSERT_EQ('c', d.at(2));
}

TEST(BytesData, ConverterToString) {
    bytes_data data = bytes_data::from_string_raw("hello streams");
    // this works only on c++17, as it have type deduction
    std::string result = data.convert(bytes_to_string());
}

TEST(BytesData, FromUint8PointerArray) {
    const uint8_t val[4] = {'a', 'b', 'c', 'd'};
    bytes_data data(val, 4);

    ASSERT_EQ(4, data.size());
    ASSERT_EQ('a', data.at(0));
    ASSERT_EQ('b', data.at(1));
    ASSERT_EQ('c', data.at(2));
    ASSERT_EQ('d', data.at(3));
}

TEST(BytesData, FromCharPointerArray) {
    const char val[4] = {'a', 'b', 'c', 'd'};
    bytes_data data = bytes_data::from_chars(val, sizeof(val));

    ASSERT_EQ(4, data.size());
    ASSERT_EQ('a', data.at(0));
    ASSERT_EQ('b', data.at(1));
    ASSERT_EQ('c', data.at(2));
    ASSERT_EQ('d', data.at(3));
}

TEST(BytesData, FromCharVector) {
    const std::vector<char> val = {'a', 'b', 'c', 'd'};
    bytes_data data = bytes_data::from_chars(val);

    ASSERT_EQ(4, data.size());
    ASSERT_EQ('a', data.at(0));
    ASSERT_EQ('b', data.at(1));
    ASSERT_EQ('c', data.at(2));
    ASSERT_EQ('d', data.at(3));
}

TEST(BytesData, FromHexCString) {
    const char* hex = "aaaabbbbcccc";
    bytes_data d(hex);
    ASSERT_EQ(6, d.size());
    ASSERT_EQ(0xAA, d.at(0));
    ASSERT_EQ(0xAA, d.at(1));
    ASSERT_EQ(0xBB, d.at(2));
    ASSERT_EQ(0xBB, d.at(3));
    ASSERT_EQ(0xCC, d.at(4));
    ASSERT_EQ(0xCC, d.at(5));

    ASSERT_STREQ(hex, d.to_hex().c_str());
}

TEST(BytesData, FromHexString) {
    const std::string hex = "aaaabbbbcccc";
    bytes_data d(hex);
    ASSERT_EQ(6, d.size());
    ASSERT_EQ(0xAA, d.at(0));
    ASSERT_EQ(0xAA, d.at(1));
    ASSERT_EQ(0xBB, d.at(2));
    ASSERT_EQ(0xBB, d.at(3));
    ASSERT_EQ(0xCC, d.at(4));
    ASSERT_EQ(0xCC, d.at(5));

    ASSERT_STREQ(hex.c_str(), d.to_hex().c_str());
}

TEST(BytesData, WriteBackInt8) {
    bytes_data d;
    d.write_back((uint8_t) 100);

    ASSERT_EQ(1, d.size());
    ASSERT_EQ((uint8_t) 100, d.at(0));
}

TEST(BytesData, WriteBackInt16) {
    bytes_data d;
    d.write_back((uint16_t) 100);

    ASSERT_EQ(2, d.size());
    ASSERT_EQ((uint8_t) 0, d.at(0));
    ASSERT_EQ((uint8_t) 100, d.at(1));
}

TEST(BytesData, WriteBackInt32) {
    bytes_data d;
    d.write_back((uint32_t) 100);

    ASSERT_EQ(4, d.size());
    ASSERT_EQ((uint32_t) 100, d.to_num_any_size<uint32_t>(0, 4));
}

TEST(BytesData, WriteBackInt64) {
    bytes_data d;
    d.write_back((uint64_t) 100);

    ASSERT_EQ(8, d.size());
    ASSERT_EQ((uint32_t) 100, d.to_num_any_size<uint32_t>(0, 8));
}

TEST(BytesData, PushBackSingleChar) {
    bytes_data d;
    d.push_back('w');

    ASSERT_EQ(1, d.size());
    ASSERT_EQ('w', d.at(0));

    d.push_back('o');
    ASSERT_EQ(2, d.size());
    ASSERT_EQ('o', d.at(1));
}

TEST(BytesData, PushBackInt16) {
    bytes_data d;
    d.push_back((uint16_t) 100);

    ASSERT_EQ(2, d.size());
    ASSERT_EQ((uint8_t) 0, d.at(0));
    ASSERT_EQ((uint8_t) 100, d.at(1));
}

TEST(BytesData, PushBackInt32) {
    bytes_data d;
    d.push_back((uint32_t) 100);

    ASSERT_EQ(4, d.size());
    ASSERT_EQ((uint32_t) 100, d.to_num_any_size<uint32_t>(0, 4));
}

TEST(BytesData, PushBackInt64) {
    bytes_data d;
    d.push_back((uint64_t) 100);
    d.push_back((uint64_t) 200);

    ASSERT_EQ(16, d.size());
    ASSERT_EQ((uint64_t) 100, d.to_num_any_size<uint32_t>(0, 8));
    ASSERT_EQ((uint64_t) 200, d.to_num_any_size<uint32_t>(8, 16));

    uint64_t any_val_a = d.to_num_any_size<uint64_t>(0);
    uint64_t any_val_b = d.to_num_any_size<uint64_t>(8);
    ASSERT_EQ((uint64_t) 100, any_val_a);
    ASSERT_EQ((uint64_t) 200, any_val_b);
}

/// \brief to_num_any - exception-less, if data is empty, it will return 0
TEST(BytesData, ToNumAnyOnEmpty) {
    bytes_data d;
    uint64_t val = d.to_num_any(0, 8);

    ASSERT_EQ(0, d.size());
    ASSERT_EQ(0, val);
}

TEST(BytesData, ToNumOperators) {
    bytes_data d;

    d.write_back((uint8_t) 0xFF);
    ASSERT_EQ(1, d.size());
    ASSERT_EQ((uint8_t) 0xFF, (uint8_t) d);

    d.clear();
    d.resize(0);
    d.write_back((char) 'w');
    ASSERT_EQ(1, d.size());
    ASSERT_EQ('w', (char) d);

    d.clear();
    d.resize(0);
    d.write_back((uint16_t) 0xFFFF);
    ASSERT_EQ(2, d.size());
    ASSERT_EQ((uint16_t) 0xFFFF, (uint16_t) d);

    d.clear();
    d.resize(0);
    d.write_back((uint32_t) 0xFFFFFFFF);
    ASSERT_EQ(4, d.size());
    ASSERT_EQ((uint32_t) 0xFFFFFFFF, (uint32_t) d);

    d.clear();
    d.resize(0);
    d.write_back((uint64_t) UINT64_MAX);
    ASSERT_EQ(8, d.size());
    ASSERT_EQ((uint64_t) UINT64_MAX, (uint64_t) d);
}