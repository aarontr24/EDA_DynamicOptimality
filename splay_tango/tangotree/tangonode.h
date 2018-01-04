#ifndef TANGONODE_H
#define TANGONODE_H

#include <iostream>
#include <string>
#include <bstnode.h>
#include <assert.h>

class TangoNode : public BSTnode
{
public:
    bool marked = false;
    int depth = 0;
    int minDepth = 0;
    int maxDepth = 0;

    TangoNode(int key, void *value) : BSTnode(key, value)
    {
        marked = false;
    }

protected:
    virtual bool isMarked(TangoNode *n)
    {
        assert(n != nullptr);

        return (n->marked == true);
    }
};

#endif // TANGONODE_H
