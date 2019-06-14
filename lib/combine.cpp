#include <fstream>

#include "combine.h"
#include "encode.h"
#include "decode.h"

template <typename T>
void write_arr_info(std::vector<T> const& arr, std::ostream &output) {
    size_t sz = arr.size();
    output.write((char *) &sz, sizeof(uint32_t));
    output.write((char *) arr.data(), sizeof(T) * sz);
}

void encode(std::istream &input, std::ostream &output) {
//    if (!input.is_open()) {
//        throw std::runtime_error("input file not found");
//    }
    input.seekg(0, std::ios::end);
    uint64_t file_sz = (uint64_t)(input.tellg());
    if (file_sz == 0) {
//        output.close();
        return;
    }

    input.seekg(0, std::ios::beg);

    if (input.peek() == EOF) {
        throw std::runtime_error("input file is really file?");
    }

    counter t;
    std::vector<uint8_t> curr_letter(1);
    for (size_t i = 0; i < file_sz; ++i) {
        input.read((char *) curr_letter.data(), 1);
        t.add(curr_letter);
    }

    encoder enc(t);
    write_arr_info(enc.get_tr(), output);
    write_arr_info(enc.get_letters(), output);

    input.seekg(0, std::ios::beg);
    std::vector<uint8_t> curr_block;
    uint64_t code_len = enc.get_len();
    output.write((char *) &code_len, sizeof(code_len));
    for (size_t i = 0; i < file_sz; ++i) {
        input.read((char *) curr_letter.data(), curr_letter.size());
        enc.encode(curr_letter[0], curr_block);
        output.write((char *)curr_block.data(), sizeof(uint8_t) * curr_block.size());
        curr_block.clear();
    }
    if (enc.get_not_used() > 0) {
        output << enc.get_last();
    }
}

void encode(std::string const& in, std::string const& out) {
    std::ifstream input(in, std::ios::binary);
    std::ofstream output(out, std::ios::binary);
    encode(input, output);
}

void decode(std::istream &input, std::ostream &output) {
//    if (!input.is_open()) {
//        throw std::runtime_error("input file not found");
//    }

    input.seekg(0, std::ios::end);
    uint64_t file_sz = input.tellg();
    if (file_sz == 0) {
//        output.close();
        return;
    }

    input.seekg(0, std::ios::beg);

    if (input.peek() == EOF) {
        throw std::runtime_error("bad encode, can't decode it");
    }

    uint32_t tree_sz;
    input.read((char *) &tree_sz, sizeof(tree_sz));
    std::vector<uint8_t> tree_trav(tree_sz);
    input.read((char *) tree_trav.data(), sizeof(uint8_t) * tree_sz);

    for (size_t i = 0; i < tree_sz; ++i) {
        if (tree_trav[i] != 0 && tree_trav[i] != 1) {
            throw std::runtime_error("file without 0 or 1 at the very begin");
        }
    }

    uint32_t letter_qty;
    input.read((char *) &letter_qty, sizeof(letter_qty));
    std::vector<uint8_t> letters(letter_qty);
    input.read((char *) letters.data(), letters.size());

    uint64_t cipher_sz;
    input.read((char *) &cipher_sz, sizeof(cipher_sz));

    decoder dec(letters, tree_trav);
    dec.set_need(cipher_sz);

    std::vector<uint8_t> res;
    std::vector<uint8_t> curr(1);
    for (uint64_t read = input.tellg(); read < file_sz; ++read) {
        input.read((char *) curr.data(), 1);
        dec.decode(curr[0], res);
        output.write((char *) res.data(), res.size());
        res.clear();
    }
    if (cipher_sz != dec.get_decoded()) {
        throw std::runtime_error("decoded not all text");
    }
}

void decode(std::string const& in, std::string const& out) {
    std::ifstream input(in, std::ios::binary);
    std::ofstream output(out, std::ios::binary);
    decode(input, output);
}
