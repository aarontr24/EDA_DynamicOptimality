#include "tangotree.h"
#include <limits.h>
#include <cstdint>
#include <typeinfo>
#include <iostream>

using namespace std;

BSTnode *TangoTree::insert(int key, void *value)
{
}

void *TangoTree::remove(int key)
{
}

void *TangoTree::search(int key)
{

    TangoNode *n = static_cast<TangoNode*>(getRoot());
    assert(isMarked(n));

    while (!isNilLeaf(n))
    {
        if (key < n->key)
        {
            n = static_cast<TangoNode*>(n->left);
        }
        else if (key > n->key)
        {
            n = static_cast<TangoNode*>(n->right);
        }
        else
        {
            break;
        }

        if (isMarked(n))
        {

            n = tangoCutAndJoin(n);
        }
    }

    assert(n != nullptr);

    TangoNode *r;
    r = tangoCutAt(n);

    TangoNode *p;
    p = findMarkedPredecessor(r, n->key);

    if (p != nullptr)
    {
        tangoJoin(r, p, n->depth);
    }
    cout <<"Found: "<<key<<endl;
    if (isNilLeaf(n))
    {
        return n->parent->value;
    }
    else
    {
        return n->value;
    }
}

TangoNode *TangoTree::findMarkedPredecessor(TangoNode *root, int key)
{

    key = key - 1;

    TangoNode *n = root;

    while (!isNilLeaf(n))
    {
        if (key < n->key)
        {
            n = static_cast<TangoNode*>(n->left);
        }
        else if (key > n->key)
        {
            n = static_cast<TangoNode*>(n->right);
        }
        else
        {
            return nullptr;
        }

        if (isMarked(n))
        {
            return n;
        }
    }

    return nullptr;
}

TangoNode *TangoTree::tangoCutAndJoin(TangoNode *n)
{

    TangoNode *topPath = static_cast<TangoNode*>(n->parent);

    while (!isMarked(topPath))
    {
        assert(topPath->parent != nullptr);
        topPath = static_cast<TangoNode*>(topPath->parent);
    }

    int cutDepth = n->minDepth - 1;

    topPath = tangoCut(topPath, cutDepth);

    topPath = tangoJoin(topPath, n, cutDepth);

    return topPath;
}

TangoNode *TangoTree::tangoCutAt(TangoNode *n)
{

    TangoNode *topPath = static_cast<TangoNode*>(n);

    while (!isMarked(topPath))
    {
        assert(topPath->parent != nullptr);
        topPath = static_cast<TangoNode*>(topPath->parent);
    }

    int cutDepth = n->depth;

    topPath = tangoCut(topPath, cutDepth);

    return topPath;
}

TangoNode *TangoTree::tangoCut(TangoNode *vRoot, int cutDepth)
{

    TangoNode *nRoot;

    TangoNode *l = findMinWithDepth(vRoot, cutDepth);
    TangoNode *r = findMaxWithDepth(vRoot, cutDepth);

    TangoNode *lp = nullptr;
    if (l != nullptr)
    {
        lp = static_cast<TangoNode*>(getPredecessorByNode(l));
    }

    TangoNode *rp = nullptr;
    if (r != nullptr)
    {
        rp = static_cast<TangoNode*>(getSuccessorByNode(r));
    }

    if ((lp == nullptr) && (rp == nullptr))
    {

        assert(isMarked(vRoot));

        nRoot = vRoot;

    }
    else if (rp == nullptr)
    {
        tangoSplit(lp, vRoot);

        assert(!isTangoLeaf(lp->right));

        markNode(lp->right);
        updateMinMaxPath(lp);

        nRoot = tangoMerge(lp);

    }
    else if (lp == nullptr)
    {

        tangoSplit(rp, vRoot);

        assert(!isTangoLeaf(rp->left));

        markNode(rp->left);
        updateMinMaxPath(rp);

        nRoot = tangoMerge(rp);

    }
    else
    {

        tangoSplit(lp, vRoot);
        tangoSplit(rp, lp->right);

        assert(!isTangoLeaf(rp->left));

        markNode(rp->left);
        updateMinMaxPath(rp);
        tangoMerge(rp);
        nRoot = tangoMerge(lp);
    }

    return nRoot;
}

TangoNode *TangoTree::findMinWithDepth(TangoNode *x, int cutDepth)
{
    assert(x != nullptr);

    while (true)
    {
        TangoNode *xl = static_cast<TangoNode*>(x->left);
        TangoNode *xr = static_cast<TangoNode*>(x->right);

        if (!isTangoLeaf(xl) && xl->maxDepth > cutDepth)
        {
            x = xl;
        }
        else if (x->depth > cutDepth)
        {
            break;
        }
        else if (!isTangoLeaf(xr))
        {
            x = xr;
        }
        else
        {
            return nullptr;
        }
    }

    return x;
}

TangoNode *TangoTree::findMaxWithDepth(TangoNode *x, int cutDepth)
{
    assert(x != nullptr);

    while (true)
    {
        TangoNode *xr = static_cast<TangoNode*>(x->right);
        TangoNode *xl = static_cast<TangoNode*>(x->left);

        if (!isTangoLeaf(xr) && xr->maxDepth > cutDepth)
        {
            x = xr;
        }
        else if (x->depth > cutDepth)
        {
            break;
        }
        else if (!isTangoLeaf(xl))
        {
            x = xl;
        }
        else
        {
            return nullptr;
        }
    }

    return x;
}

TangoNode *TangoTree::tangoJoin(TangoNode *topPath, TangoNode *n, int cutDepth)
{
    assert(topPath != nullptr);
    assert(n != nullptr);
    assert(isRoot(topPath));
    assert(isRoot(n));
    assert(topPath->maxDepth < n->minDepth);
    assert(cutDepth == topPath->maxDepth);
    assert(cutDepth == (n->minDepth - 1));

    TangoNode *newRoot = nullptr;

    TangoNode *lp = nullptr;
    TangoNode *rp = nullptr;


    TangoNode *x = topPath;

    while (x != n)
    {
        if (x->key > n->key)
        {
            rp = x;
            x = static_cast<TangoNode*>(x->left);
        }
        else
        {
            lp = x;
            x = static_cast<TangoNode*>(x->right);
        }
    }


    if ((lp == nullptr) && (rp == nullptr))
    {
        assert(false);

    }
    else if (rp == nullptr)
    {
        tangoSplit(lp, topPath);

        assert(isTangoLeaf(lp->right));

        unmarkNode(lp->right);
        updateMinMaxPath(lp->right);

        newRoot = tangoMerge(lp);

    }
    else if (lp == nullptr)
    {

        tangoSplit(rp, topPath);

        assert(isTangoLeaf(rp->left));

        unmarkNode(rp->left);
        updateMinMaxPath(rp->left);

        newRoot = tangoMerge(rp);

    }
    else
    {

        tangoSplit(lp, topPath);
        tangoSplit(rp, lp->right);

        assert(rp == lp->right);
        assert(isTangoLeaf(rp->left));

        unmarkNode(rp->left);
        updateMinMaxPath(rp->left);
        tangoMerge(rp);
        newRoot = tangoMerge(lp);

    }

    assert(newRoot != nullptr);

    return newRoot;
}

TangoNode *TangoTree::tangoSplit(TangoNode *n, BSTnode *vRoot)
{
    assert(n != nullptr);
    assert(vRoot != nullptr);

    TangoNode *ret;

    ret = tangoSplitImpl(n, vRoot);

    assert(ret == n);

    return ret;
}

TangoNode *TangoTree::tangoMerge(TangoNode *n)
{
    assert(n != nullptr);

    TangoNode *ret;

    ret = tangoMergeImpl(n);

    return ret;
}

TangoNode *TangoTree::createNode(int key, void *value)
{
    return new TangoNode(key, value);
}

TangoNode *TangoTree::createNil()
{
    return createNil(nullptr);
}

TangoNode *TangoTree::createNil(BSTnode *parent)
{
    TangoNode *nil = createNode(NIL_KEY, nullptr);

    nil->parent = parent;

    return nil;
}

void TangoTree::initializePerfectTree(int levels)
{

    _root = createPerfectSubtree(levels - 1, 0, 1);

}

TangoNode *TangoTree::createPerfectSubtree(int levels, int depth, int nextId)
{
    if ((levels - depth) == 0)
    {

        TangoNode *n = createNode(nextId, reinterpret_cast<void*>(nextId));
        ++nextId;


        TangoNode *nl = createNil(n);
        nl->depth = depth + 1;
        n->left = nl;

        TangoNode *nr = createNil(n);
        nr->depth = depth + 1;
        n->right = nr;


        n->size = 3;
        n->depth = depth;
        n->marked = true;
        n->minDepth = depth;
        n->maxDepth = depth;

        return n;
    }
    else
    {

        TangoNode *l = createPerfectSubtree(levels, depth + 1, nextId);
        nextId += std::pow(2, (levels - depth)) - 1;

        TangoNode *p = createNode(nextId, reinterpret_cast<void*>(nextId));
        ++nextId;

        TangoNode *r = createPerfectSubtree(levels, depth + 1, nextId);

        l->parent = p;
        p->left = l;

        r->parent = p;
        p->right = r;


        p->size = l->size + r->size + 1;
        p->depth = depth;
        p->marked = true;
        p->minDepth = depth;
        p->maxDepth = depth;

        return p;
    }
}

void TangoTree::rotateLeft(BSTnode *n)
{

    BinarySearchBasic::rotateLeft(n);

    assert(!isMarked(n->parent));

    if (isMarked(n))
    {
        markNode(n->parent);
        unmarkNode(n);
    }

    updateMinMax(n);
    updateMinMax(n->parent);
}

void TangoTree::rotateRight(BSTnode *n)
{

    BinarySearchBasic::rotateRight(n);

    assert(!isMarked(n->parent));

    if (isMarked(n))
    {
        markNode(n->parent);
        unmarkNode(n);
    }

    updateMinMax(n);
    updateMinMax(n->parent);
}

void TangoTree::updateMinMax(BSTnode *n)
{
    updateMinMax(static_cast<TangoNode*>(n));
}

void TangoTree::updateMinMax(TangoNode *n)
{
    assert(n != nullptr);

    int min = n->depth;
    int max = n->depth;

    TangoNode *nl = static_cast<TangoNode*>(n->left);
    TangoNode *nr = static_cast<TangoNode*>(n->right);

    if (!isTangoLeaf(nl))
    {
        min = std::min(min, nl->minDepth);
        max = std::max(max, nl->maxDepth);
    }

    if (!isTangoLeaf(nr))
    {
        min = std::min(min, nr->minDepth);
        max = std::max(max, nr->maxDepth);
    }

    n->minDepth = min;
    n->maxDepth = max;

}

void TangoTree::updateMinMaxPath(BSTnode *n)
{
    assert(n != nullptr);

    updateMinMax(n);


    while (!isRoot(n))
    {
        n = n->parent;
        updateMinMax(n);
    }
}

bool TangoTree::isTangoLeaf(BSTnode *n)
{
    // assert (n != null);

    return (n == nullptr || isNilLeaf(n) || isMarked(n));
}

bool TangoTree::isRoot(BSTnode *n)
{
    return (BinarySearchBasic::isRoot(n) || isMarked(n));
}

bool TangoTree::isLeaf(BSTnode *n)
{
    return ((!hasLeftChild(n)) && (!hasRightChild(n)));
}

bool TangoTree::hasLeftChild(BSTnode *n)
{
    assert(n != nullptr);

    if (BinarySearchBasic::hasLeftChild(n))
    {
        return !isMarked(n->left);
    }
    else
    {
        return false;
    }
}

bool TangoTree::hasRightChild(BSTnode *n)
{
    assert(n != nullptr);

    if (BinarySearchBasic::hasRightChild(n))
    {
        return !isMarked(n->right);
    }
    else
    {
        return false;
    }
}

bool TangoTree::isLeftChild(BSTnode *n)
{
    if (BinarySearchBasic::isLeftChild(n))
    {
        return !isMarked(n);
    }
    else
    {
        return false;
    }
}

bool TangoTree::isRightChild(BSTnode *n)
{
    if (BinarySearchBasic::isRightChild(n))
    {
        return !isMarked(n);
    }
    else
    {
        return false;
    }
}

bool TangoTree::isMarked(TangoNode *n)
{
    assert(n != nullptr);

    return (n->marked == true);
}

void TangoTree::markNode(TangoNode *n)
{
    assert(n != nullptr);

    n->marked = true;
}

void TangoTree::unmarkNode(TangoNode *n)
{
    assert(n != nullptr);

    n->marked = false;
}

BSTnode *TangoTree::getMinimumChild(BSTnode *n)
{
    BSTnode *m = BinarySearchBasic::getMinimumChild(n);

    if (isNilLeaf(m))
    {
        return m->parent;
    }
    else
    {
        return m;
    }
}

BSTnode *TangoTree::getMaximumChild(BSTnode *n)
{
    BSTnode *m = BinarySearchBasic::getMaximumChild(n);

    if (isNilLeaf(m))
    {
        return m->parent;
    }
    else
    {
        return m;
    }
}

BSTnode *TangoTree::getPredecessorByNode(BSTnode *n)
{
    if (hasLeftChild(n) && !isNilLeaf(n->left))
    {
        return getMaximumChild(n->left);
    }
    else
    {
        while (!isRoot(n) && isLeftChild(n))
        {
            n = n->parent;

        }

        if (isRoot(n))
        {
            return nullptr;
        }
        else
        {
            return n->parent;
        }
    }
}

BSTnode *TangoTree::getSuccessorByNode(BSTnode *n)
{
    if (hasRightChild(n) && !isNilLeaf(n->right))
    {
        return getMinimumChild(n->right);
    }
    else
    {
        while (!isRoot(n) && isRightChild(n))
        {
            n = n->parent;

        }

        if (isRoot(n))
        {
            return nullptr;
        }
        else
        {
            return n->parent;
        }
    }
}

bool TangoTree::isMarked(BSTnode *n)
{
    //assert(dynamic_cast<TangoNode*>(n) != nullptr);

    return isMarked(static_cast<TangoNode*>(n));
}

void TangoTree::markNode(BSTnode *n)
{
    //assert(typeid(BSTnode) == typeid(TangoNode));

    markNode(static_cast<TangoNode*>(n));
}

void TangoTree::unmarkNode(BSTnode *n)
{
    //assert(typeid(BSTnode) == typeid(TangoNode));

    unmarkNode(static_cast<TangoNode*>(n));
}

void TangoTree::fillInOrderElements(BSTnode *n, std::vector<int> &elements)
{
    if (n->left != nullptr && n->left->key != NIL_KEY)
    {
        fillInOrderElements(n->left, elements);
    }

    elements.push_back(n->key);

    if (n->right != nullptr && n->right->key != NIL_KEY)
    {
        fillInOrderElements(n->right, elements);
    }
}
