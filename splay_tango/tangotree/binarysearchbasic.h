#ifndef BINARYSEARCHBASIC_H
#define BINARYSEARCHBASIC_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <binarysearchtree.h>
#include <bstnode.h>

class BSTnode;

class BinarySearchBasic : virtual public BinarySearchTree
{
protected:
    BSTnode *_root;

public:
    virtual ~BinarySearchBasic()
    {
        delete _root;
    }

protected:
    BinarySearchBasic();

public:

    BSTnode *insert(int key, void *value) override;
    void *remove(int key) override;
    void *search(int key) override;
    BSTnode *getRoot() override;
    void initializePerfectTree(int levels) override;

protected:
    virtual BSTnode *createNode(int key, void *value) = 0;

    virtual BSTnode *searchByKey(int key);
    virtual void rotateLeft(BSTnode *n);
    virtual void rotateRight(BSTnode *n);
    virtual void setParentReference(BSTnode *cur, BSTnode *nw);
    virtual void clearParentReference(BSTnode *n);


    virtual bool isRoot(BSTnode *n);
    virtual bool isLeaf(BSTnode *n);
    virtual bool isNilLeaf(BSTnode *n);
    virtual bool hasLeftChild(BSTnode *n);
    virtual bool hasRightChild(BSTnode *n);
    virtual bool isLeftChild(BSTnode *n);
    virtual bool isRightChild(BSTnode *n);
    virtual BSTnode *getMaximumChild(BSTnode *n);
    virtual BSTnode *getMinimumChild(BSTnode *n);
    virtual BSTnode *getPredecessorByNode(BSTnode *n);
    virtual BSTnode *getSuccessorByNode(BSTnode *n);
    virtual BSTnode *getSibling(BSTnode *n);


    virtual void updateSubtreeSizePath(BSTnode *n);
    virtual void updateSubtreeSize(BSTnode *n);
    virtual void detach(BSTnode *n, BSTnode *par);
    virtual void attachUp(BSTnode *n, BSTnode *par);
    virtual void attachLeft(BSTnode *n, BSTnode *par);
    virtual void attachRight(BSTnode *n, BSTnode *par);

private:
    void fillInOrderElements(BSTnode *n, std::vector<int> &elements);
};

#endif // BINARYSEARCHBASIC_H
