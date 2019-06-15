#include <utility>
#include <cstddef>
#include <stdexcept>

#include "tree.h"

tree::tree() : left(nullptr), right(nullptr), parent(nullptr), weight(0) {}

tree::tree(tree *_l, tree *_r, std::vector<uint8_t> _sym_arr, uint64_t _w) :
        left(_l), right(_r), parent(nullptr),
        str(std::move(_sym_arr)), weight(_w) {}

tree::~tree() {
    if (left != nullptr) {
        delete left;
    }
    if (right != nullptr) {
        delete right;
    }
}

void tree::merge(tree *x, tree *y) {
    str = x->str;
    for (uint8_t sym : y->str) {
        str.push_back(sym);
    }

    y->parent = x->parent = this;
    left = x;
    right = y;
    weight = x->weight + y->weight;
}

void dfs(tree *t, std::vector<uint8_t> &curr, std::vector<uint8_t> &letters,
         std::vector<std::vector<uint8_t>> &codes) {
    if (t->left != nullptr) {
        curr.push_back(0);
        dfs(t->left, curr, letters, codes);

        curr.back() = 1;
        dfs(t->right, curr, letters, codes);

        curr.pop_back();
    } else {
        codes[t->str[0]] = curr;
        letters.push_back(t->str[0]);
        return;
    }
}

void travel(tree *t, std::vector<uint8_t> &res) {
    if (t->left != nullptr) {
        res.push_back(0);
        travel(t->left, res);

        res.push_back(1);
        travel(t->right, res);
    }
}

void recover(tree *t, std::vector<uint8_t> &ls, std::vector<uint8_t> &str) {
    if (ls.empty()) {
        throw std::runtime_error("letter arr is empty");
    }

    size_t pos = 0;
    for (bool q : str) {
        if (!q) {
            t->left = new tree();
            t->left->parent = t;
            t = t->left;
        } else {
            if (pos == ls.size()) {
                throw std::runtime_error("wrong tree, or letters,  pos == arr "
                                         "size");
            }
            t->str.push_back(ls[pos++]);

            if (t->parent == nullptr) {
                throw std::runtime_error("wrong tree, parent == nullptr");
            }
            t = t->parent;
            while (t->right != nullptr && t->parent != nullptr) {
                t = t->parent;
            }
            t->right = new tree();
            t->right->parent = t;
            t = t->right;
        }
    }

    t->str.push_back(ls[pos++]);
    if (pos != ls.size()) {
        throw std::runtime_error("tree doesn't suit string");
    }
}

uint64_t tree::get_w() const {
    return weight;
}

tree *tree::get_l() const {
    return left;
}

tree *tree::get_r() const {
    return right;
}

uint8_t tree::get_sym() const {
    return str[0];
}

std::vector<uint8_t> tree::get_str() const {
    return str;
}

counter::counter() {
    cnt.resize(ALPHABET_SIZE);
}

void counter::add(std::vector<uint8_t> const& arr) {
    for (uint8_t i : arr) {
        ++cnt[i];
    }
}

std::vector<uint64_t> counter::get_cnt() const {
    return cnt;
}
