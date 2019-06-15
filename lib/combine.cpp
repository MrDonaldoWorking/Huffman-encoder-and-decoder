#include <fstream>

#include "combine.h"
#include "encode.h"
#include "decode.h"
#include "reader.h"

template <typename T>
void write_arr_info(std::vector<T> const& arr, std::ostream &output) {
    size_t sz = arr.size();
    output.write((char *) &sz, sizeof(uint32_t));
    output.write((char *) arr.data(), sizeof(T) * sz);
}

void encode(std::string const& in, std::ofstream &output) {
    reader input(in);
    uint64_t file_sz = input.get_file_size();
    if (file_sz == 0) {
        output.close();
        return;
    }

    counter t;
    std::vector<uint8_t> curr_letter(1);
    for (size_t i = 0; i < file_sz; ++i) {
        input.get_next_arr(curr_letter, 1);
        t.add(curr_letter);
    }

    encoder enc(t);

    write_arr_info(enc.get_tr(), output);
    write_arr_info(enc.get_letters(), output);

    input.move_to_beg();
    std::vector<uint8_t> curr_block;
    uint64_t code_len = enc.get_len();
    output.write((char *) &code_len, sizeof(code_len));
    for (size_t i = 0; i < file_sz; ++i) {
        input.get_next_arr(curr_letter, 1);
        enc.encode(curr_letter[0], curr_block);
        output.write((char *)curr_block.data(), sizeof(uint8_t) * curr_block.size());
        curr_block.clear();
    }
    if (enc.get_not_used() > 0) {
        output << enc.get_last();
    }

    output.close();
}

void encode(std::string const& in, std::string const& out) {
    std::ofstream output(out, std::ios::binary);
    encode(in, output);
}

void decode(reader &input, std::ofstream &output) {
    uint64_t file_sz = input.get_file_size();
    if (file_sz == 0) {
        output.close();
        return;
    }
    if (file_sz < 21) {
        throw std::runtime_error("bad encode, can't decode it");
    }

    uint32_t tree_sz = input.get_next_32();
    std::vector<uint8_t> tree_trav(tree_sz);
    input.get_next_arr(tree_trav, tree_sz);

    for (size_t i = 0; i < tree_sz; ++i) {
        if (tree_trav[i] != 0 && tree_trav[i] != 1) {
            throw std::runtime_error("file without 0 or 1 at the very begin");
        }
    }

    uint32_t letter_qty = input.get_next_32();
    std::vector<uint8_t> letters(letter_qty);
    input.get_next_arr(letters, letter_qty);

    uint64_t cipher_sz = input.get_next_64();

    decoder dec(letters, tree_trav);
    dec.set_need(cipher_sz);

    std::vector<uint8_t> res;
    std::vector<uint8_t> curr(1);
    for (uint64_t read = input.get_pos(); read < file_sz; ++read) {
        input.get_next_arr(curr, 1);
        dec.decode(curr[0], res);
        output.write((char *) res.data(), res.size());
        res.clear();
    }
    if (cipher_sz != dec.get_decoded()) {
        throw std::runtime_error("decoded not all text");
    }

    output.close();
}

void decode(std::string const& in, std::string const& out) {
    reader input(in);
    std::ofstream output(out, std::ios::binary);
    decode(input, output);
}
