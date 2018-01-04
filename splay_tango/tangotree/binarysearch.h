#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include <iostream>
#include <string>
#include <bstnode.h>

class BinarySearch
{
public:
    static const int NIL_KEY = std::numeric_limits<int>::min();
    virtual BSTnode *insert(int key, void *value) = 0;
    virtual void *remove(int key) = 0;
    virtual void *search(int key) = 0;
    virtual BSTnode *getRoot() = 0;
    virtual void initializePerfectTree(int levels) = 0;
};

#endif // BINARYSEARCH_H
