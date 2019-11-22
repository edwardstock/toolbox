//
// Created by edward on 13.11.2019.
//

#include <gtest/gtest.h>
#include "toolboxpp.hpp"

using namespace toolboxpp::data;

TEST(ByteData, Write) {
    bytes_data d1;
    d1.write(0, (uint8_t) 0x05);
    ASSERT_EQ(1, d1.size());
    ASSERT_EQ(0x05, d1[0]);

    d1.write(1, (uint16_t) 0x0101);
    ASSERT_EQ(3, d1.size());
    ASSERT_EQ((uint8_t) 0x01, d1[1]);
    ASSERT_EQ((uint8_t) 0x01, d1[2]);

    d1.write(3, (uint32_t) 20);
    ASSERT_EQ(3 + 4, d1.size());
    ASSERT_EQ(20u >> 24u, d1[3]);
    ASSERT_EQ(20u >> 16u, d1[4]);
    ASSERT_EQ(20u >> 8u, d1[5]);
    ASSERT_EQ(20u & 0xFFu, d1[6]);

    d1.write(7, (uint64_t) 40ULL);
    ASSERT_EQ(3 + 4 + 8, d1.size());
    ASSERT_EQ(40ULL >> 56u, d1[7]);
    ASSERT_EQ(40ULL >> 48u, d1[8]);
    ASSERT_EQ(40ULL >> 40u, d1[9]);
    ASSERT_EQ(40ULL >> 32u, d1[10]);
    ASSERT_EQ(40ULL >> 24u, d1[11]);
    ASSERT_EQ(40ULL >> 16u, d1[12]);
    ASSERT_EQ(40ULL >> 8u, d1[13]);
    ASSERT_EQ(40ULL & 0xFFu, d1[14]);

    size_t n = d1.size();
    std::vector<uint8_t> tmp1{0x0, 0x01, 0x02, 0x03};
    d1.write(15, tmp1);
    ASSERT_EQ(n + tmp1.size(), d1.size());
    ASSERT_EQ(0x00u, d1[15]);
    ASSERT_EQ(0x01u, d1[16]);
    ASSERT_EQ(0x02u, d1[17]);
    ASSERT_EQ(0x03u, d1[18]);

    // 19 items now
//    size_t s2 = n + tmp1.size();

    std::vector<uint8_t> tmp2(64);
    std::for_each(tmp2.begin(), tmp2.end(), [](uint8_t &v) {
      v = 0xFFu;
    });

    ASSERT_EQ(64, tmp2.size());

    // ok, let write 64 byte vector into the 9th d index
    d1.write(10, tmp2);

    ASSERT_EQ(74, d1.size());
    ASSERT_EQ(0xFFu, d1[10]);
    ASSERT_EQ(0xFFu, d1[73]);

    // now replace tail with new data
    d1.write_tail(10, tmp2);
    ASSERT_EQ(74, d1.size());
    ASSERT_EQ(0xFFu, d1[10]);
    ASSERT_EQ(0xFFu, d1[73]);
    // now we've removed old tail with new
    // scheme:
    // [ 0 - 18 old data ]
    // we're inserted into the 10 index 64 bytes, and now we have
    // [ 0 - 9 old data, 10 - 73 new 64 bytes data ]
    // and size of ByteData was changed to 74

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

    bool threw = false;
    try {
        d1.write(150, t2, 4);
    } catch (const std::out_of_range &e) {
        threw = true;
    }
    ASSERT_TRUE(threw);
}

TEST(ByteData, InsertIterator) {
    bytes_data d;
    std::vector<uint8_t> data{1, 2, 3, 4};
    size_t n = 0;
    n = d.write(d.begin(), data.begin(), data.end());
    ASSERT_EQ(4, d.size());
    ASSERT_EQ(4, n);

    std::vector<uint8_t> data2{5, 6, 7, 8};
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

TEST(ByteData, Ranges) {
    bytes_data d;
    std::vector<uint8_t> tmp{1, 2, 3, 4};
    d.push_back(tmp);

    ASSERT_EQ(1, d[0]);
    ASSERT_EQ(2, d[1]);
    ASSERT_EQ(3, d[2]);
    ASSERT_EQ(4, d[3]);

    auto slice1 = d.take_first(2);
    ASSERT_EQ(2, slice1.size());
    ASSERT_EQ(1, slice1[0]);
    ASSERT_EQ(2, slice1[1]);

    auto slice2 = d.take_last(2);
    ASSERT_EQ(2, slice2.size());
    ASSERT_EQ(3, slice2[0]);
    ASSERT_EQ(4, slice2[1]);

    auto slice3 = d.take_range(0, 2);
    ASSERT_EQ(2, slice3.size());
    ASSERT_EQ(1, slice3[0]);
    ASSERT_EQ(2, slice3[1]);

    auto slice4 = d.take_range(3, 4);
    ASSERT_EQ(1, slice4.size());
    ASSERT_EQ(4, slice4[0]);

    auto slice5 = d.take_range(2, 4);
    ASSERT_EQ(2, slice5.size());
    ASSERT_EQ(3, slice5[0]);
    ASSERT_EQ(4, slice5[1]);

    auto slice6 = d.take_range(4, 4);
    ASSERT_EQ(0, slice6.size());

    bool s7threw = false;
    try {
        auto slice7 = d.take_range(5, 4);
        ASSERT_EQ(0, slice7.size());
    } catch (const std::out_of_range &e) {
        s7threw = true;
    }
    ASSERT_TRUE(s7threw);

    bool s8threw = false;
    try {
        auto slice8 = d.take_range(5, 5);
        ASSERT_EQ(0, slice8.size());
    } catch (const std::out_of_range &e) {
        s8threw = true;
    }
    ASSERT_TRUE(s8threw);

    bool s9threw = false;
    try {
        auto slice9 = d.take_range(0, 10);
        ASSERT_EQ(0, slice9.size());
    } catch (const std::out_of_range &e) {
        s9threw = true;
    }
    ASSERT_TRUE(s9threw);
}

TEST(ByteData, push_back) {
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
    std::vector<uint8_t> tmp1{0x0, 0x01, 0x02, 0x03};
    d1.push_back(tmp1);
    ASSERT_EQ(n + tmp1.size(), d1.size());

}

TEST(ByteBuffer, pop_front_to) {
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
        //stack write
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

TEST(ByteBuffer, PopFrontLimit) {
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
    //01 01 05 00 00 00 05 00 01 00 90 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
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

TEST(ByteData, WriteReadNumber) {
    bytes_data d(32);
    d.write(0, (uint16_t) 0x8080u);

    uint8_t buf[4] = {0xFFu, 0xFFu, 0xFFu, 0xFFu};

    uint32_t num = (((uint32_t) buf[0]) << 24u) | (((uint32_t) buf[1]) << 16u) | (((uint32_t) buf[2]) << 8u)
        | (((uint32_t) buf[3]));
    ASSERT_EQ(UINT32_MAX, num);

    ASSERT_EQ(32, d.size());
    ASSERT_EQ((uint16_t) 0x8080u, d.to_num<uint16_t>(0));

    d.write(2, (uint32_t) UINT32_MAX);
    ASSERT_EQ(0xFFFFFFFFU, UINT32_MAX);
    ASSERT_EQ(32, d.size());
    ASSERT_EQ(0xFFu, d[2]);
    ASSERT_EQ(0xFFu, d[3]);
    ASSERT_EQ(0xFFu, d[4]);
    ASSERT_EQ(0xFFu, d[5]);
    ASSERT_EQ(0xFFFFFFFFU, d.to_num<uint32_t>(2));

    d.write(6, (uint64_t) UINT64_MAX);
    ASSERT_EQ(32, d.size());
    ASSERT_EQ(0xFFu, d[6]);
    ASSERT_EQ(0xFFu, d[7]);
    ASSERT_EQ(0xFFu, d[8]);
    ASSERT_EQ(0xFFu, d[9]);
    ASSERT_EQ(0xFFu, d[10]);
    ASSERT_EQ(0xFFu, d[11]);
    ASSERT_EQ(0xFFu, d[12]);
    ASSERT_EQ(0xFFu, d[13]);
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
