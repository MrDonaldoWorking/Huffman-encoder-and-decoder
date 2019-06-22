#ifndef HUFFMAN_DECODE_H
#define HUFFMAN_DECODE_H

#include <vector>
#include <stdint.h>
#include "tree.h"

struct decoder {
    decoder() = delete;
    decoder(std::vector<uint8_t>, std::vector<uint8_t>);
    decoder(decoder &d) = delete;
    decoder &operator=(decoder &d) = delete;

    void decode(uint8_t, std::vector<uint8_t> &);

    void set_need(uint64_t);
    uint64_t get_decoded();

private:
    uint64_t decoded, need;
    std::vector<bool> last;
    std::vector<uint8_t> letters;
    std::vector<uint8_t> tree_struct;
    tree t;

    bool update(bool, tree **, std::vector<uint8_t> &);
};


#endif //HUFFMAN_DECODE_H
