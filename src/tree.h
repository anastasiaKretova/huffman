#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <vector>
#include <iostream>

class Tree {
    class Node {
        unsigned char symbol;
        Node *sons[2];
        size_t _size;
    public:
        explicit Node(unsigned char s = 0, size_t si = 0, Node *ls = nullptr, Node *rs = nullptr);

        ~Node();

        size_t size();

        unsigned char getSymbol();

        Node *son(bool f);
    };

    Node *root;
    Node *cur;

public:
    Tree();

    ~Tree();

    void build_tree(unsigned long long *count);

    void delete_node(Tree::Node *v);

    bool is_leaf();

    std::pair<int, long long> codes[256];

    void create_codes(Tree::Node *v, std::pair<int, long long> codes);

    bool is_root();

    void to_root();

    void move(bool f);

    unsigned char get_cur_symbol();
};

#endif //HUFFMAN_TREE_H
