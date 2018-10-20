#include "huffman.h"
#include <iosfwd>
#include <cstring>
#include <iostream>

const size_t Huffman::READ_BLOCK_SIZE = 1000;
const size_t Huffman::WRITE_BLOCK_SIZE = 1000;

void Huffman::compress(std::istream &in, std::ostream &out) {
    in.seekg(0, in.end);
    auto len = (size_t) in.tellg();
    in.seekg(std::istream::beg);
    if (len == 0) return;

    unsigned long long cnt[256];
    std::fill(cnt, cnt + 256, 0);
    cnt[0] = cnt[1] = 1;
    unsigned char buff[READ_BLOCK_SIZE + 1];
    size_t copy_len = len;
    while (copy_len != 0) {
        size_t loop = std::min(copy_len, READ_BLOCK_SIZE);
        in.read((char *) (&buff), loop);
        copy_len -= loop;
        for (size_t i = 0; i < loop; i++) {
            ++cnt[buff[i]];
        }
    }

    short cnt_symbols = 2;
    for (size_t i = 2; i < 256; ++i) {
        if (cnt[i]) {
            ++cnt_symbols;
        }
    }



    out.put(cnt_symbols);
    for (size_t i = 0; i < 256; ++i) {
        if (cnt[i]) {
            out.put(i);
            for (int j = 7; j >=0; --j) {
                out.put((unsigned char)(cnt[i] >> (j * 8)));
            }
        }
    }

    Tree huffman_tree;
    huffman_tree.build_tree(cnt);
    in.seekg(in.beg);

    bit_seq bit_codes[256];
    for (int i = 0; i < 256; ++i) {
        bit_codes[i] = bit_seq();
        bit_codes[i].add(huffman_tree.codes[i]);
    }
    unsigned long long cnt_bits;
    for (size_t i = 0; i < 256; ++i) {
            cnt_bits += cnt[i] * bit_codes[i].size();
        }
        cnt_bits -= (bit_codes[0].size() + bit_codes[1].size());

    for (int i = 7; i >= 0; i--) {
        out.put((unsigned char) (cnt_bits >> (i * 8)));
    }

    bit_seq ans;
    copy_len = len;
    unsigned char *write;
    while (copy_len) {
        size_t loop = std::min(WRITE_BLOCK_SIZE, copy_len);
        copy_len -= loop;
        in.read((char*) buff, loop);
        for (size_t i = 0; i < loop; ++i) {
            ans.add({huffman_tree.codes[buff[i]]});
            if (ans.size() >= WRITE_BLOCK_SIZE * 8) {
                size_t write_size = ans.size() / 64 * 8;
                write = ans.write(true);
                out.write((char *) write, write_size);
                delete[] write;
            }
        }
    }
    if (ans.size() != 0) {
        size_t lenn = ans.size() / 8 + (ans.size() % 8 != 0);
        write = ans.write(false);
        std::cout << " " << int(write[0]) << " a\n";
        out.write((char *) write, lenn);
        delete[] write;
    }
}

bool Huffman::decompress(std::istream &in, std::ostream &out) {
    in.seekg(0, in.end);
    auto size_of_file = (unsigned long long) in.tellg();
    in.seekg(std::istream::beg);
    unsigned char not_empty_symbol = 0;
    unsigned long long cnt[256];
    std::fill(cnt, cnt + 256, 0);
    cnt[0] = cnt[1] = 1;
    if (in.peek() == EOF) return true;

    in.read((char *) &not_empty_symbol, 1);
    int cnt_symbols = (not_empty_symbol ? not_empty_symbol : 256);
    for (int i = 0; i < cnt_symbols; ++i) {
        unsigned char symbol;
        if (in.peek() == EOF) return false;
        in.read((char *) &symbol, 1);
        for (size_t j = 0; j < 8; ++j) {
            unsigned char c;
            if (in.peek() == EOF) return false;
            in.read((char *) &c, 1);
            cnt[symbol] = (cnt[symbol] << 8) + c;
        }
        if (!cnt[symbol]) return false;
    }

    Tree huffman_tree;
    huffman_tree.build_tree(cnt);
    size_t len = 0;

    unsigned char l[8];
    in.read((char *) l, 8);
    for (unsigned char i : l) {
        len = (len << 8) + i;
    }
    if (size_of_file - len / 8 - (len % 8 != 0) != in.tellg()) return false;
    if (cnt_symbols == 1) {
        for (int i = 0; i < len; i++) {
            out.put(huffman_tree.get_cur_symbol());
        }
        return true;
    }
    char w[WRITE_BLOCK_SIZE];
    char r[READ_BLOCK_SIZE];
    int pos = 0;
    size_t copy_len = len / 8 + (len % 8 != 0);
    while (copy_len != 0) {
        size_t loop = copy_len < READ_BLOCK_SIZE ? copy_len : READ_BLOCK_SIZE;
        copy_len -= loop;
        in.read(r, loop);
        for (size_t i = 0; i < loop; i++) {
            for (size_t j = 0; j < 8; j++) {
                if (i * 8 + j < len) {
                    auto flag = bool(1 & (r[i] >> (7 - j)));
                    huffman_tree.move(flag);
                    if (huffman_tree.is_leaf()) {
                        w[pos] = huffman_tree.get_cur_symbol();
                        pos++;
                        if (pos == WRITE_BLOCK_SIZE) {
                            out.write(w, WRITE_BLOCK_SIZE);
                            pos = 0;
                        }
                        huffman_tree.to_root();
                    }
                }
            }
        }
        len -= loop * 8;
    }
    if (!huffman_tree.is_root()) return false;
    if (pos != 0) {
        out.write(w, pos);
    }
    return true;
}
