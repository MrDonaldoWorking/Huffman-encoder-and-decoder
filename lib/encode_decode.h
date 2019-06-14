#ifndef HUFFMAN_ENCODE_DECODE_H
#define HUFFMAN_ENCODE_DECODE_H

#include <iostream>
#include <string>
#include "tree.h"

void encode(std::string const& in, std::string const& out);
void encode(std::istream &input, std::ostream &output);

void decode(std::string const& in, std::string const& out);
void decode(std::istream &input, std::ostream &output);


#endif //HUFFMAN_ENCODE_DECODE_H
