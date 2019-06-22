#ifndef HUFFMAN_READER_H
#define HUFFMAN_READER_H


#include <string>
#include <fstream>
#include <vector>

const size_t BUF_LEN = 256;

struct reader {
    reader() = delete;
    explicit reader(std::string const& s);
    reader(reader &r) = delete;
    reader &operator=(reader &r) = delete;
    ~reader();

    bool eof();

    uint8_t get_next();
    uint64_t get_file_size();
    void move_to_beg();
    void get_next_arr(std::vector<uint8_t> &to, size_t sz);
    uint32_t get_next_32();
    uint64_t get_next_64();
    uint64_t get_pos();

private:
    std::ifstream in;
    std::string name;
    uint8_t *buffer;
    size_t curr_pos;
    size_t end_pos;
    uint64_t already_read;

    uint64_t get_next_int(size_t sz);
};


#endif //HUFFMAN_READER_H
