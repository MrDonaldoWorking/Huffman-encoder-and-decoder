#include <utility>
#include <stdexcept>

#include "decode.h"
#include "tree.h"

decoder::decoder(std::vector<uint8_t> ls, std::vector<uint8_t> str) :
        is_f(true), decoded(0),
        letters(std::move(ls)), tree_struct(std::move(str)) {
    build(&t, letters, tree_struct);
}

void check(bool state, tree **x) {
    if (!state) {
        if ((*x)->get_l() == nullptr) {
            throw std::runtime_error("wrong tree, left == nullptr");
        }
        *x =  (*x)->get_l();
    } else {
        if ((*x)->get_r() == nullptr) {
            throw std::runtime_error("wrong tree, right == nullptr");
        }
        *x = (*x)->get_r();
    }
}

bool decoder::update(bool state, tree **x, std::vector<uint8_t> &out) {
    if (state) {
        out.push_back((*x)->get_sym());
        decoded += last.size();
        last.clear();
        *x = &t;
        return (need == decoded);
    }

    return false;
}

void decoder::decode(uint8_t b, std::vector<uint8_t> &out) {
    tree *curr = &t;
    int j = BLOCK_SIZE - 1;
    if (!last.empty()) {
        bool f = false;
        for (auto &&el : last) {
            check(el, &curr);
        }

        while (j >= 0) {
            last.push_back(static_cast<bool>((b >> j) & 1));
            check(last.back(), &curr);

            --j;

            if (curr->get_r() == nullptr) {
                f = true;
                break;
            }
        }

        if (update(f, &curr, out)) {
            return;
        }
    }

    while (j >= 0) {
        last.push_back(static_cast<bool>((b >> j) & 1));
        --j;
        check(last.back(), &curr);

        if (update((curr->get_r() == nullptr), &curr, out)) {
            return;
        }
    }
}

void decoder::set_need(uint64_t val) {
    need = val;
}

uint64_t decoder::get_decoded() {
    return decoded;
}
