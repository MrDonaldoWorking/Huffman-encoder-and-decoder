#include "reader.h"

reader::reader(std::string const& s) : in(s, std::ios::binary), name(s),
    buffer(new uint8_t[BUF_LEN]), curr_pos(0), end_pos(0), already_read(0) {
    if (!in.is_open()) {
        throw std::runtime_error("file not found");
    }
}

reader::~reader() {
    delete[] buffer;
    in.close();
}

bool reader::eof() {
    return curr_pos == end_pos && in.peek() == EOF;
}

uint8_t reader::get_next() {
    if (curr_pos == end_pos) {
        if (eof()) {
            throw std::runtime_error("reached end of file");
        }
        in.read((char *) buffer, BUF_LEN);
        end_pos = in.gcount();
        if (end_pos == 0) {
            throw std::runtime_error("reached end of file");
        }
        curr_pos = 0;
    }
    ++already_read;
    return buffer[curr_pos++];
}

uint64_t reader::get_file_size() {
    in.seekg(0, std::ios::end);
    auto res = (uint64_t)(in.tellg());
    move_to_beg();
    return res;
}

void reader::move_to_beg() {
    in.close();
    in.open(name, std::ios::binary);
    already_read = curr_pos = end_pos = 0;
}

void reader::get_next_arr(std::vector<uint8_t> &to, size_t sz) {
    for (size_t i = 0; i < sz; ++i) {
        to[i] = get_next();
    }
}

uint64_t reader::get_next_int(size_t sz) {
    uint64_t res = 0;
    std::vector<uint8_t> bytes(sz);
    get_next_arr(bytes, sz);
    for (size_t i = sz - 1; i != 0; --i) {
        res += bytes[i];
        res <<= 8u;
    }

    return res + bytes[0];
}

uint32_t reader::get_next_32() {
    return get_next_int(sizeof(uint32_t));
}

uint64_t reader::get_next_64() {
    return get_next_int(sizeof(uint64_t));
}

uint64_t reader::get_pos() {
    return already_read;
}
