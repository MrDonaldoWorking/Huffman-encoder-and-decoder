#include "gtest/gtest.h"
#include <sstream>
#include "combine.h"

TEST(correctness, empty) {
    std::stringstream in("");
    std::stringstream tmp;
    std::stringstream out;

    EXPECT_NO_THROW(encode(in, tmp));
    EXPECT_NO_THROW(decode(tmp, out));
    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, one_symbol) {
    std::stringstream in("a");
    std::stringstream tmp;
    std::stringstream out;

    EXPECT_NO_THROW(encode(in, tmp));
    EXPECT_NO_THROW(decode(tmp, out));
    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, hello) {
    std::stringstream in("Hello, World!\n");
    std::stringstream tmp;
    std::stringstream out;

    EXPECT_NO_THROW(encode(in, tmp));
    EXPECT_NO_THROW(decode(tmp, out));
    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, all_chars) {
    std::stringstream in;
    std::stringstream tmp;
    std::stringstream out;

    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        in << (char) i;
    }

    EXPECT_NO_THROW(encode(in, tmp));
    EXPECT_NO_THROW(decode(tmp, out));
    EXPECT_EQ(in.str(), out.str());
}

TEST(correctness, aaaaa) {
    std::stringstream in;
    std::stringstream tmp;
    std::stringstream out;

    for (size_t i = 0; i < 1024 * 32; ++i) {
        in << "aaaaa";
    }

    EXPECT_NO_THROW(encode(in, tmp));
    EXPECT_NO_THROW(decode(tmp, out));
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

        EXPECT_NO_THROW(encode(in, tmp));
        EXPECT_NO_THROW(decode(tmp, out));
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

        EXPECT_NO_THROW(encode(in, tmp));
        EXPECT_NO_THROW(decode(tmp, out));
        EXPECT_EQ(in.str(), out.str());
    }
}

TEST(correctness, twice) {
    for (size_t tests = 0; tests < 5; ++tests) {
        std::stringstream in;
        std::stringstream tmp;
        std::stringstream tmp1;
        std::stringstream out1;
        std::stringstream out;

        for (size_t i = 0; i < 1024 * 1024; ++i) {
            in << (char) (rand() % ALPHABET_SIZE);
        }

        EXPECT_NO_THROW(encode(in, tmp));
        EXPECT_NO_THROW(encode(tmp, tmp1));
        EXPECT_NO_THROW(decode(tmp1, out1));
        EXPECT_NO_THROW(decode(out1, out));
        EXPECT_EQ(in.str(), out.str());
    }
}