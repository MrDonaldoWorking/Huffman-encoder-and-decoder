#include <iostream>
#include <cstring>
#include <fstream>

#include "../lib/encode.h"
#include "../lib/decode.h"

template <typename T>
void write_arr_info(std::vector<T> const& arr, std::ofstream &output) {
    size_t sz = arr.size();
    output.write((char *) &sz, sizeof(uint32_t));
    output.write((char *) arr.data(), sizeof(T) * sz);
}

void encode(std::string const& in, std::string const& out) {
    std::ifstream input(in, std::ios::binary);
    if (!input.is_open()) {
        throw std::runtime_error("input file not found");
    }
    input.seekg(0, std::ios::end);
    uint64_t sz = (uint64_t)(input.tellg());
    if (sz == 0) {
        std::ofstream output(out, std::ios::binary);
        output.close();
        return;
    }

    std::ofstream output(out, std::ios::binary);
    input.seekg(0, std::ios::beg);

    counter t;
    std::vector<uint8_t> curr_letter(1);
    for (size_t i = 0; i < sz; ++i) {
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
    for (size_t i = 0; i < sz; ++i) {
        input.read((char *) curr_letter.data(), curr_letter.size());
        enc.encode(curr_letter[0], curr_block);
        output.write((char *)curr_block.data(), sizeof(uint8_t) * curr_block.size());
        curr_block.clear();
    }
    if (enc.get_not_used() > 0) {
        output << enc.get_last();
    }
}

void decode(std::string const& in, std::string const& out) {
    std::ifstream input(in, std::ios::binary);
    if (!input.is_open()) {
        throw std::runtime_error("input file not found");
    }

    input.seekg(0, std::ios::end);
    uint64_t sz = input.tellg();
    if (sz == 0) {
        std::ofstream output(out, std::ios::binary);
        output.close();
        return;
    }
    if (sz < 21) {
        throw std::runtime_error("file has bad encode (not enough)");
    }

    std::ofstream output(out, std::ios::binary);
    input.seekg(0, std::ios::beg);
    uint32_t sz1;
    input.read((char *) &sz1, sizeof(sz1));
    std::vector<uint8_t> str(sz1);
    input.read((char *) str.data(), sizeof(uint8_t) * sz1);

    for (size_t i = 0; i < sz1; ++i) {
        if (str[i] != 0 && str[i] != 1) {
            throw std::runtime_error("file without 0 or 1 at the very begin");
        }
    }

    uint32_t sz2;
    input.read((char *) &sz2, sizeof(sz2));
    std::vector<uint8_t> letters(sz2);
    input.read((char *) letters.data(), sizeof(uint8_t) * letters.size());
    uint64_t left;
    input.read((char *) &left, sizeof(left));
    uint64_t read = input.tellg();
    std::vector<uint8_t> res;
    std::vector<uint8_t> curr;
    decoder dec(letters, str);
    dec.set_need(read);
    uint64_t curr_read = 0;
    while (read < sz) {
        curr.resize(std::min(1ul, sz - read));
        input.read((char *) curr.data(), sizeof(uint8_t) * curr.size());
        read += curr.size();
        dec.decode(curr, res);
        output.write((char *) res.data(), sizeof(uint8_t) * res.size());
        res.clear();
        curr_read += curr.size();
    }
    if (dec.get_need() != dec.get_decoded()) {
        throw std::runtime_error("decoded not all text");
    }
}

int main(int argc, char *argv[]) {
    // std::cout << "Please input encode/decode inputFile outputFile\n";
    if (argc != 4) {
        std::cerr << "Expected 3 args, but found " << argc - 1;
        return 0;
    }
    if (strcmp(argv[1], "encode") != 0 && strcmp(argv[1], "decode") != 0) {
        std::cerr << "Please input 'encode' or 'decode' at first arg";
        return 0;
    } else {
        try {
            std::string in = argv[2];
            std::string out = argv[3];
            switch (argv[1][0]) {
                case 'e':
                    encode(in, out);
                    std::cout << "Successfully encoded\n";
                    break;
                case 'd':
                    decode(in, out);
                    std::cout << "Successfully decoded\n";
                    break;
            }
        } catch (std::runtime_error &e) {
            std::cerr << e.what();
        }
    }

    return 0;
}