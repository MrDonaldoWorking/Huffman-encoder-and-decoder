#ifndef HUFFMAN_ENCODE_H
#define HUFFMAN_ENCODE_H

#include <cstddef>
#include "tree.h"

struct encoder {
    encoder() = default;
    explicit encoder(counter &);
    ~encoder();

    void encode(uint8_t, std::vector<uint8_t> &);

    uint64_t get_len();
    std::vector<uint8_t> get_letters();
    std::vector<uint8_t> get_tr();
    size_t get_not_used();
    uint8_t get_last();

private:
    uint8_t last;
    size_t not_used;
    uint64_t len;
    std::vector<uint64_t> freq;
    tree *t;
    std::vector<tree *> trees;
    std::vector<std::vector<uint8_t>> code;
    std::vector<uint8_t> tr;
    std::vector<uint8_t> letters;

    void gen_code();
    void gen_tree();
};

#endif //HUFFMAN_ENCODE_H
