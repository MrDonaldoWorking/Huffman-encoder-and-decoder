#include "gtest/gtest.h"
#include <sstream>
#include <random>
#include <fstream>
#include <reader.h>
#include "combine.h"

static std::string IN = "in.txt";
static std::string OUT = "out.txt";

void check(std::string const &name) {
    EXPECT_NO_THROW(encode(name + ".in", name + ".encoded"));
    EXPECT_NO_THROW(decode(name + ".encoded", name + ".out"));

    reader r_in(name + ".in"), r_out(name + ".out");
    while (!r_in.eof() && !r_out.eof()) {
        EXPECT_EQ(r_in.get_next(), r_out.get_next());
    }
    EXPECT_EQ(r_in.eof(), r_out.eof());
}

TEST(correctness, empty) {
    check("test/files/empty");
}

TEST(correctness, one_symbol) {
    check("test/files/one_symbol");
}

TEST(correctness, hello) {
    check("test/files/hello");
}

TEST(correctness, all_chars) {
    std::ofstream tmp("test/files/all_chars.in", std::ios::binary);
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        tmp << (char) i;
    }

    check("test/files/all_chars");
}

TEST(correctness, aaaaa) {
    std::ofstream tmp("test/files/aaaaa.in", std::ios::binary);
    for (size_t i = 0; i < 1024 * 32; ++i) {
        tmp << "aaaaa";
    }

    check("test/files/aaaaa");
}

TEST(correctness, mathan) {
    check("test/files/mathan");
}

TEST(correctness, overlord) {
    check("test/files/overlord");
}

TEST(correctness, bad_input) {
    std::string s("#include <stdio.h>\n"
                  "\n"
                  "using namespace std;"
                  "\n"
                  "int main() {"
                  "    printf(\"Hello, World!\\n\");"
                  "\n"
                  "    return 0;"
                  "}\n");

    std::ofstream tmp("test/files/bad_input.encoded", std::ios::binary);
    tmp << s;
    tmp.close();

    EXPECT_THROW(decode("test/files/bad_input.encoded",
                        "test/files/bad_input.out"), std::runtime_error);
}

TEST(correctness, small_random) {
    for (size_t tests = 0; tests < 100; ++tests) {
        std::ofstream tmp("test/files/small_random.in", std::ios::binary);
        for (size_t i = 0; i < 1024; ++i) {
            tmp << (char) (rand() % 256);
        }

        check("test/files/small_random");
    }
}

TEST(correctness, large_random) {
    for (size_t tests = 0; tests < 5; ++tests) {
        std::ofstream tmp("test/files/large_random.in", std::ios::binary);
        for (size_t i = 0; i < 1024 * 1024; ++i) {
            tmp << (char) (rand() % ALPHABET_SIZE);
        }

        check("test/files/large_random");
    }
}

TEST(correctness, 20MB) {
    check("test/files/rus_gre_20MB");
}

TEST(correctness, twice) {
    for (size_t tests = 0; tests < 5; ++tests) {
        std::ofstream tmp("test/files/twice.in", std::ios::binary);
        for (size_t i = 0; i < 1024 * 1024; ++i) {
            tmp << (char) (rand() % ALPHABET_SIZE);
        }
        tmp.close();

        EXPECT_NO_THROW(encode("test/files/twice.in",
                               "test/files/twice_tmp.encoded"));
        EXPECT_NO_THROW(encode("test/files/twice_tmp.encoded",
                               "test/files/twice_tmp2.encoded"));
        EXPECT_NO_THROW(decode("test/files/twice_tmp2.encoded",
                               "test/files/twice_tmp3.decoded"));
        EXPECT_NO_THROW(decode("test/files/twice_tmp3.decoded",
                               "test/files/twice.out"));
        check("test/files/twice");
    }
}