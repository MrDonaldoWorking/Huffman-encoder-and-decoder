#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <vector>

const size_t ALPHABET_SIZE = 256;
const size_t BLOCK_SIZE = 8;

struct tree {
    tree();
    tree(tree *, tree *, std::vector<uint8_t>, uint64_t);
    tree(tree &t) = delete;
    tree &operator=(tree &t) = delete;
    ~tree();

    void merge(tree *, tree *);

    uint64_t get_w() const;
    tree *get_l() const;
    tree *get_r() const;
    uint8_t get_sym() const;
    std::vector<uint8_t> get_str() const;

private:
    tree *left;
    tree *right;
    tree *parent;
    std::vector<uint8_t> str;
    uint64_t weight;

    friend void dfs(tree *, std::vector<uint8_t> &, std::vector<uint8_t> &,
                    std::vector<std::vector<uint8_t>> &);
    friend void travel(tree *, std::vector<uint8_t> &);
    friend void recover(tree *t, std::vector<uint8_t> &ls,
                        std::vector<uint8_t> &str);
};

struct counter {
    counter();
    counter(counter &c) = delete;
    counter &operator=(counter &c) = delete;
    void add(std::vector<uint8_t> const&);
    std::vector<uint64_t> get_cnt() const;

private:
    std::vector<uint64_t> cnt;
};

#endif //HUFFMAN_TREE_H
