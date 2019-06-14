#include <iostream>
#include <cstring>

#include "combine.h"

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