#include "tree.h"

#include <iostream>
#include <set>
#include "tree.h"

Tree::Node::Node(unsigned char s, size_t si, Tree::Node *ls, Tree::Node *rs) {
    _size = si;
    symbol = s;
    sons[0] = ls;
    sons[1] = rs;
}

Tree::Node::~Node() {
    sons[0] = nullptr;
    sons[1] = nullptr;
}

size_t Tree::Node::size() {
    return _size;
}

Tree::Node *Tree::Node::son(bool f) {
    return sons[(int) f];
}

unsigned char Tree::Node::getSymbol() {
    return symbol;
}

Tree::Tree() {
    root = nullptr;
    cur = nullptr;
}

Tree::~Tree() {
    delete_node(root);
}

void Tree::build_tree(unsigned long long *count) {
    auto cmp = [](Node *a, Node *b) { return a->size() < b->size(); };
    std::multiset<Node *, decltype(cmp)> s(cmp);
    for (size_t i = 0; i < 256; ++i) {
        if (count[i] != 0) {
            s.insert(new Node((unsigned char) i, count[i], nullptr, nullptr));
        }
    }
    if (s.size() < 2) {
        if (count[1] == 0) {
            s.insert(new Node((unsigned char) 1, count[1], nullptr, nullptr));
        }
        if (count[0] == 0){
            s.insert(new Node((unsigned char) 0, count[0], nullptr, nullptr));
        }
    }
    while (s.size() > 1) {
        Node *node1 = *s.begin();
        s.erase(s.begin());
        Node *node2 = *s.begin();
        s.erase(s.begin());
        s.insert(new Node(0, node1->size() + node2->size(), node1, node2));
    }
    root = *s.begin();
    cur = root;
    std::pair<int, long long int> cod;
    cod.first = 0;
    cod.second = 0;
    create_codes(root, cod);
}

void Tree::create_codes(Tree::Node *v, std::pair<int, long long int> cod) {
    if (v->son(false) == nullptr && v->son(true) == nullptr) {
        unsigned char c = v->getSymbol();
        codes[c] = cod;
        return;
    }
    ++(cod.first);
    cod.second *= 2;
    create_codes(v->son(false), cod);
    ++cod.second;
    create_codes(v->son(true), cod);
    --cod.first;
    cod.second /= 2;
    --cod.second;
}

void Tree::delete_node(Tree::Node *v) {
    if (v == nullptr) return;
    delete_node(v->son(false));
    delete_node(v->son(true));
    delete v;
}

bool Tree::is_leaf() {
    return cur->son(false) == nullptr && cur->son(true) == nullptr;
}

bool Tree::is_root() {
    return cur == root;
}

void Tree::to_root() {
    cur = root;
}

void Tree::move(bool f) {
    cur = cur->son(f);
}

unsigned char Tree::get_cur_symbol() {
    return cur->getSymbol();
}



