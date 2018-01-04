#ifndef BSTNODE_H
#define BSTNODE_H

#include <iostream>
#include <string>

class BSTnode{

public:
    int key = 0;
    void *value;
    int size = 0;

    BSTnode *parent;
    BSTnode *left = nullptr;
    BSTnode *right = nullptr;

    BSTnode(int key, void *value)
    {
        this->key = key;
        this->value = value;
        this->size = 1;
    }
};


#endif // BSTNODE_H
