#ifndef REDBLACKNODE_H
#define REDBLACKNODE_H

#include <iostream>
#include <string>
#include <tangonode.h>

using namespace std;

class RedBlackNode : public TangoNode
{
public:
    string color;

    int blackHeight = 0;

    RedBlackNode(int key, void *value) : TangoNode(key, value)
    {
        this->color = "RED";
    }
};

#endif // REDBLACKNODE_H
