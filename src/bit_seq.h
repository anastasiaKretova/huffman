#ifndef LIB_BITSEQ_H
#define LIB_BITSEQ_H

#include <iostream>
#include <vector>

class bit_seq {
    size_t _size;

public:
    bit_seq();
    ~bit_seq() = default;
    std::vector <unsigned long long> bits;
    size_t size();
    void add_bit(bool a);
    void add(std::pair<int, long long int> p);
    void append(bit_seq x);
    void pop();
    unsigned char* write(bool f);
    void clear();
};

#endif //LIB_BITSEQ_H