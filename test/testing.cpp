#include "gtest/gtest.h"
#include <sstream>
#include "../lib/encode_decode.h"

TEST(correctness, empty) {
    std::stringstream in("");
    std::stringstream tmp;
    std::stringstream out;

    encode(in, tmp);
    decode(tmp, out);
    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, one_symbol) {
    std::stringstream in("a");
    std::stringstream tmp;
    std::stringstream out;

    encode(in, tmp);
    decode(tmp, out);
    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, hello) {
    std::stringstream in("Hello, World!\n");
    std::stringstream tmp;
    std::stringstream out;

    encode(in, tmp);
    decode(tmp, out);
    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, all_chars) {
    std::stringstream in;
    std::stringstream tmp;
    std::stringstream out;

    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        in << (char) i;
    }

    encode(in, tmp);
    decode(tmp, out);
    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, small_random) {
    for (size_t tests = 0; tests < 100; ++tests) {
        std::stringstream in;
        std::stringstream tmp;
        std::stringstream out;

        for (size_t i = 0; i < 1024; ++i) {
            in << (char) (rand() % 256);
        }

        encode(in, tmp);
        decode(tmp, out);
        EXPECT_EQ(in.str(), out.str());
    }
}

TEST(correctness, large_random) {
    for (size_t tests = 0; tests < 5; ++tests) {
        std::stringstream in;
        std::stringstream tmp;
        std::stringstream out;

        for (size_t i = 0; i < 1024 * 1024; ++i) {
            in << (char) (rand() % ALPHABET_SIZE);
        }

        encode(in, tmp);
        decode(tmp, out);
        EXPECT_EQ(in.str(), out.str());
    }
}

TEST(correctness, twice) {
    for (size_t tests = 0; tests < 5; ++tests) {
        std::stringstream in;
        std::stringstream tmp1;
        std::stringstream tmp2;
        std::stringstream out1;
        std::stringstream out2;

        for (size_t i = 0; i < 1024 * 1024; ++i) {
            in << (char) (rand() % ALPHABET_SIZE);
        }

        encode(in, tmp1);
        encode(tmp1, tmp2);
        decode(tmp2, out1);
        decode(out1, out2);
        EXPECT_EQ(in.str(), out2.str());
    }
}