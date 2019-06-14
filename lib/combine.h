#ifndef HUFFMAN_COMBINE_H
#define HUFFMAN_COMBINE_H

#include <iostream>
#include <string>
#include "tree.h"

void encode(std::string const& in, std::string const& out);
void encode(std::istream &input, std::ostream &output);

void decode(std::string const& in, std::string const& out);
void decode(std::istream &input, std::ostream &output);


#endif //HUFFMAN_COMBINE_H
