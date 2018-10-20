#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <fstream>
#include "tree.h"
#include "bit_seq.h"

class Huffman {
public:
    static const size_t READ_BLOCK_SIZE;
    static const size_t WRITE_BLOCK_SIZE;
public:
    static void compress(std::istream &in, std::ostream &out);

    static bool decompress(std::istream &in, std::ostream &out);
};

#endif //HUFFMAN_HUFFMAN_H
