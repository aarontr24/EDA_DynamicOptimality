#ifndef TANGOTREE_H
#define TANGOTREE_H

#include <string>
#include <vector>
#include <cmath>
#include <assert.h>
#include <tangonode.h>
#include <binarysearchbasic.h>
#include <binarysearchtree.h>

class TangoTree : virtual public BinarySearchBasic, virtual public BinarySearchTree
{
public:
    BSTnode *insert(int key, void *value) override;
    void *remove(int key) override;
    void *search(int key) override;

    void initializePerfectTree(int levels) override;

private:
    TangoNode *findMarkedPredecessor(TangoNode *root, int key);
    TangoNode *tangoCutAndJoin(TangoNode *n);
    TangoNode *tangoCutAt(TangoNode *n);
    TangoNode *tangoCut(TangoNode *vRoot, int cutDepth);
    TangoNode *findMinWithDepth(TangoNode *x, int cutDepth);
    TangoNode *findMaxWithDepth(TangoNode *x, int cutDepth);
    TangoNode *tangoJoin(TangoNode *topPath, TangoNode *n, int cutDepth);
    TangoNode *tangoSplit(TangoNode *n, BSTnode *vRoot);
    TangoNode *tangoMerge(TangoNode *n);
    TangoNode *createPerfectSubtree(int levels, int depth, int nextId);
    void updateMinMax(BSTnode *n);
    void fillInOrderElements(BSTnode *n, std::vector<int> &elements);

protected:
    virtual TangoNode *tangoSplitImpl(TangoNode *n, BSTnode *vRoot) = 0;
    virtual TangoNode *tangoMergeImpl(TangoNode *n) = 0;

    TangoNode *createNode(int key, void *value) override;
    virtual TangoNode *createNil();
    virtual TangoNode *createNil(BSTnode *parent);

    void rotateLeft(BSTnode *n) override;
    void rotateRight(BSTnode *n) override;

    virtual void updateMinMax(TangoNode *n);
    virtual void updateMinMaxPath(BSTnode *n);

    virtual bool isTangoLeaf(BSTnode *n);

    bool isRoot(BSTnode *n) override;

    bool isLeaf(BSTnode *n) override;

    bool hasLeftChild(BSTnode *n) override;

    bool hasRightChild(BSTnode *n) override;

    bool isLeftChild(BSTnode *n) override;

    bool isRightChild(BSTnode *n) override;

    virtual bool isMarked(TangoNode *n);

    virtual void markNode(TangoNode *n);

    virtual void unmarkNode(TangoNode *n);

    BSTnode *getMinimumChild(BSTnode *n) override;

    BSTnode *getMaximumChild(BSTnode *n) override;

    BSTnode *getPredecessorByNode(BSTnode *n) override;

    BSTnode *getSuccessorByNode(BSTnode *n) override;

    virtual bool isMarked(BSTnode *n);

    virtual void markNode(BSTnode *n);

    virtual void unmarkNode(BSTnode *n);

};

#endif // TANGOTREE_H
