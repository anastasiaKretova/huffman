#include "bit_seq.h"

bit_seq::bit_seq() {
    _size = 0;
}

size_t bit_seq::size() {
    return _size;
}

void bit_seq::add_bit(bool a) {
    if ((_size & 63) == 0) bits.push_back(0);
    bits.back() += ((unsigned long long)(a) << (63 - (_size & 63)));
    _size++;
}

void bit_seq::add(std::pair<int, long long int> p) {
    //std::cout << p.second << "   f\n";
    for (int i = p.first - 1; i >= 0; --i) {
        auto x = static_cast<int>((p.second >> i) & 1);
        add_bit(static_cast<bool>(x));
    }
}

void bit_seq::pop() {
    if (_size == 0) return;
    _size -= 64;
    bits.pop_back();
}

unsigned char *bit_seq::write(bool f) {
    size_t new_size = 0;
    unsigned long long ost = 0;
    if (_size == 0 || bits.empty()) return nullptr;
    if (f && (_size & 63) != 0) {
        ost = bits.back();
        new_size = (_size & 63);
        _size -= new_size;
        bits.pop_back();
    }
    size_t len = _size / 8 + ((_size % 8) != 0);
    auto ans = new unsigned char[len];
    for (size_t i = 0; i < bits.size(); i++) {
        for (int j = 7; j >= 0; j--) {
            if (i * 8 + (7-j) < len) ans[i*8 + 7 - j] = (unsigned char)(bits[i] >> (j*8));
            else break;
        }
    }
    clear();
    if (new_size != 0) {
        bits.push_back(ost);
        _size = new_size;
    }
    return ans;
}

void bit_seq::clear() {
    _size = 0;
    bits.clear();
}