#include <cstdint>
#include <algorithm>
#include "encode.h"
#include "tree.h"

bool cmp(tree *x, tree *y) {
    return x->get_w() > y->get_w();
}

encoder::encoder(counter &b) : last(0), not_used(0), len(0) {
    freq = b.get_cnt();
    for (size_t i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            std::vector<uint8_t> v(1, (uint8_t) i);
            trees.push_back(new tree(nullptr, nullptr, v, freq[i]));
        }
    }

    std::sort(trees.begin(), trees.end(), cmp);
    make_tree();
    make_code();
    t = trees[0];
}

encoder::~encoder() {
    delete t;
}

void encoder::encode(uint8_t letter, std::vector<uint8_t> &b_out) {
    size_t sz = code[letter].size();
    size_t st = 0;
    if (sz >= 8 - not_used) {
        for (size_t i = not_used; i < 8; ++i) {
            if (code[letter][i - not_used] > 0) {
                last += (1u << (7 - i));
            }
        }

        b_out.push_back(last);
        st = static_cast<size_t>(8 - not_used);
        sz -= 8 - not_used;
        last = not_used = 0;
    }

    while (sz >= 8) {
        uint8_t curr = 0;
        for (size_t i = 0; i < 8; ++i) {
            if (code[letter][i + st] > 0) {
                curr += (1u << (7 - i));
            }
        }

        b_out.push_back(curr);
        st += 8;
        sz -= 8;
    }

    if (sz != 0) {
        for (size_t i = 0; i < sz; ++i) {
            if (code[letter][i + st] > 0) {
                last += (1u << (7 - i - not_used));
            }
        }
        not_used += sz;
    }
}

void encoder::make_code() {
    code.resize(256);
    std::vector<uint8_t> curr;
    dfs(trees[0], curr, letters, code);
    cod(trees[0], tr);
}

void encoder::make_tree() {
    if (trees.size() == 1) {
        trees.push_back(new tree(nullptr, nullptr, trees[0]->get_arr(),
                trees[0]->get_w()));
    }

    while (trees.size() > 1) {
        tree *right = trees.back();
        trees.pop_back();
        tree *left = trees.back();
        trees.pop_back();
        tree *res = new tree();
        res->merge(left, right);

        if (trees.empty()) {
            trees.push_back(res);
            continue;
        }
        if (trees[0]->get_w() < res->get_w()) {
            trees.insert(trees.begin(), res);
            continue;
        }

        auto rit = trees.end() - 1;
        bool inserted = false;
        for (; rit != trees.begin(); --rit) {
            if ((*rit)->get_w() >= res->get_w()) {
                trees.insert(rit, res);
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            trees.insert(rit + 1, res);
        }
    }
}

uint64_t encoder::get_len() {
    len = 0;
    for (size_t i = 0; i < 256; ++i) {
        len += (uint64_t) (freq[i] * code[i].size());
    }
    return len;
}

std::vector<uint8_t> encoder::get_letters() {
    return letters;
}

std::vector<uint8_t> encoder::get_tr() {
    return tr;
}

size_t encoder::get_not_used() {
    return not_used;
}

uint8_t encoder::get_last() {
    return last;
}
