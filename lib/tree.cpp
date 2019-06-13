#include <utility>
#include <cstddef>
#include <stdexcept>

#include "tree.h"

tree::tree() : left(nullptr), right(nullptr), parent(nullptr), weight(0) {}

tree::tree(tree *_l, tree *_r, std::vector<uint8_t> _sym_arr, uint64_t _w) :
        left(_l), right(_r), parent(nullptr),
        sym_arr(std::move(_sym_arr)), weight(_w) {}

tree::~tree() {
    if (left != nullptr) {
        delete left;
    }
    if (right != nullptr) {
        delete right;
    }
}

void tree::merge(tree *x, tree *y) {
    sym_arr = x->sym_arr;
    for (uint8_t sym : y->sym_arr) {
        sym_arr.push_back(sym);
    }

    y->parent = x->parent = this;
    left = x;
    right = y;
    weight = x->weight + y->weight;
}

void dfs(tree *t, std::vector<uint8_t> &curr, std::vector<uint8_t> &order,
         std::vector<std::vector<uint8_t>> &codes) {
    if (t->left != nullptr) {
        curr.push_back(0);
        dfs(t->left, curr, order, codes);

        curr.back() = 1;
        dfs(t->right, curr, order, codes);

        curr.pop_back();
    } else {
        codes[t->sym_arr[0]] = curr;
        order.push_back(t->sym_arr[0]);
        return;
    }
}

void cod(tree *t, std::vector<uint8_t> &curr) {
    if (t->left != nullptr) {
        curr.push_back(1);
        cod(t->left, curr);

        curr.push_back(0);
        cod(t->right, curr);
    }
}

void build(tree *t, std::vector<uint8_t> &ls, std::vector<uint8_t> &str) {
    size_t pos = 0;
    for (bool q : str) {
        if (q) {
            t->left = new tree();
            t->left->parent = t;
            t = t->left;
        } else {
            if (pos == ls.size()) {
                throw std::runtime_error("wrong tree, pos == arr size");
            }
            t->sym_arr.push_back(ls[pos++]);

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

    t->sym_arr.push_back(ls[pos++]);
    if (pos != ls.size()) {
        throw std::runtime_error("wrong symbol array");
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
    return sym_arr[0];
}

std::vector<uint8_t> tree::get_arr() const {
    return sym_arr;
}

counter::counter() {
    cnt.resize(256);
}

void counter::add(std::vector<uint8_t> const& arr) {
    for (uint8_t i : arr) {
        ++cnt[i];
    }
}

std::vector<uint64_t> counter::get_cnt() const {
    return cnt;
}
