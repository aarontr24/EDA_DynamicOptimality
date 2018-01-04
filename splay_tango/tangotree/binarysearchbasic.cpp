#include "binarysearchbasic.h"
#include <assert.h>
#include <iostream>

using namespace std;

BinarySearchBasic::BinarySearchBasic()
{
}

BSTnode *BinarySearchBasic::insert(int key, void *value)
{

    BSTnode *p = searchByKey(key);
    BSTnode *n;

    if (p == nullptr)
    {
        // tree esta vacio, crea nuevo root

        n = createNode(key, value);
        _root = n;
    }
    else if (p->key == key)
    {
        // key existe, actualizar

        n = p;
        n->value = value;
    }
    else
    {
        // key no existe

        n = createNode(key, value);
        n->parent = p;

        if (p != nullptr)
        {
            if (key < p->key)
            {
                p->left = n;
            }
            else
            {
                p->right = n;
            }
        }

        updateSubtreeSizePath(n->parent);
    }

    return n;
}

void *BinarySearchBasic::remove(int key)
{

    BSTnode *d = searchByKey(key);

    /*
     * no encuentra caso
     */
    if (d == nullptr || d->key != key)
    {
        return nullptr;
    }

    auto ret_payload = d->value;

    /*
     * encuentra
     */
    if (d->left == nullptr && d->right == nullptr)
    {
        /*
         * d no tiene hijo, es una hoja
         */

        if (d == _root)
        {
            _root = nullptr;
        }
        else
        {
            clearParentReference(d);
        }

    }
    else if (d->left != nullptr && d->right != nullptr)
    {
        /*
         * d tiene dos hijos, d es un predecesor
         */

        BSTnode *n = getPredecessorByNode(d);

        d->key = n->key;
        d->value = n->value;

        if (n->left == nullptr)
        {
            clearParentReference(n);
        }
        else
        {
            setParentReference(n, n->left);
            n->left->parent = n->parent;
        }

        d = n;

    }
    else
    {
        /*
         * d tiene un hijo
         */
        BSTnode *c = (d->left != nullptr ? d->left : d->right);

        c->parent = d->parent;
        if (d == _root)
        {
            _root = c;
        }
        else
        {
            setParentReference(d, c);
        }
    }

    updateSubtreeSizePath(d->parent);

    return ret_payload;
}

void *BinarySearchBasic::search(int key)
{
    BSTnode *n = searchByKey(key);

    if (n == nullptr)
    {
        return nullptr;
    }
    else
    {
        return n->value;
    }
}

BSTnode *BinarySearchBasic::getRoot()
{
    return _root;
}

void BinarySearchBasic::initializePerfectTree(int levels)
{
}

BSTnode *BinarySearchBasic::searchByKey(int key)
{
    BSTnode *n = getRoot();
    BSTnode *p = nullptr;

    while (n != nullptr)
    {
        p = n;

        if (key < n->key)
        {
            n = n->left;
        }
        else if (key > n->key)
        {
            n = n->right;
        }
        else
        {
            break;
        }
    }

    /*
     * Si la clave no está en árbol, la búsqueda devuelve lo último que vimos
     */
    return p;
}

void BinarySearchBasic::rotateLeft(BSTnode *n)
{

    assert(n != nullptr);

    BSTnode *pv = n->right;
    assert(pv != nullptr);

    // promover pv
    pv->parent = n->parent;

    if (n->parent == nullptr)
    {
        if (n == _root)
        {
            _root = pv;
        }
        else
        {
            // nada
        }
    }
    else
    {
        setParentReference(n, pv);
    }

    // mover el subárbol izquierdo de pv a la derecha de n
    n->right = pv->left;
    if (n->right != nullptr)
    {
        n->right->parent = n;
    }

    // mueve n para ser el hijo izquierdo de pv
    pv->left = n;
    n->parent = pv;

    n->size = 1 + (n->left != nullptr ? n->left->size : 0) + (n->right != nullptr ? n->right->size : 0);

    pv->size = 1 + n->size + (pv->right != nullptr ? pv->right->size : 0);
}

void BinarySearchBasic::rotateRight(BSTnode *n)
{

    assert(n != nullptr);

    BSTnode *pv = n->left;
    assert(pv != nullptr);

    pv->parent = n->parent;

    if (n->parent == nullptr)
    {
        if (n == _root)
        {
            _root = pv;
        }
        else
        {
            // nada
        }
    }
    else
    {
        setParentReference(n, pv);
    }

    // mover el subárbol derecho de pv a la izquierda
    n->left = pv->right;
    if (n->left != nullptr)
    {
        n->left->parent = n;
    }

    // mover n para ser el hijo derecho de pv
    pv->right = n;
    n->parent = pv;

    n->size = 1 + (n->left != nullptr ? n->left->size : 0) + (n->right != nullptr ? n->right->size : 0);

    pv->size = 1 + n->size + (pv->left != nullptr ? pv->left->size : 0);
}

void BinarySearchBasic::setParentReference(BSTnode *cur, BSTnode *nw)
{
    assert(cur != nullptr);
    assert(cur->parent != nullptr);
    assert(nw != nullptr);

    if (cur == cur->parent->left)
    {
        cur->parent->left = nw;
    }
    else if (cur == cur->parent->right)
    {
        cur->parent->right = nw;
    }
    else
    {
        assert(false);
    }
}

void BinarySearchBasic::clearParentReference(BSTnode *n)
{

    assert(n != nullptr);
    assert(n->parent != nullptr);

    if (n == n->parent->left)
    {
        n->parent->left = nullptr;
    }
    else if (n == n->parent->right)
    {
        n->parent->right = nullptr;
    }
    else
    {
        assert(false);
    }
}

bool BinarySearchBasic::isRoot(BSTnode *n)
{
    assert(n != nullptr);

    return (n->parent == nullptr);
}

bool BinarySearchBasic::isLeaf(BSTnode *n)
{
    assert(n != nullptr);

    return ((n->left == nullptr) && (n->right == nullptr));
}

bool BinarySearchBasic::isNilLeaf(BSTnode *n)
{
    assert(n != nullptr);

    if (n->key == BinarySearchBasic::NIL_KEY)
    {
        assert(this->isLeaf(n));

        return true;
    }
    else
    {
        return false;
    }
}

bool BinarySearchBasic::hasLeftChild(BSTnode *n)
{
    return (n->left != nullptr);
}

bool BinarySearchBasic::hasRightChild(BSTnode *n)
{
    return (n->right != nullptr);
}

bool BinarySearchBasic::isLeftChild(BSTnode *n)
{

    assert(n != nullptr);
    assert(n->parent != nullptr);

    return (n == n->parent->left);
}

bool BinarySearchBasic::isRightChild(BSTnode *n)
{

    assert(n != nullptr);
    assert(n->parent != nullptr);

    return (n == n->parent->right);
}

BSTnode *BinarySearchBasic::getMaximumChild(BSTnode *n)
{
    assert(n != nullptr);

    while (hasRightChild(n))
    {
        n = n->right;
    }

    return n;
}

BSTnode *BinarySearchBasic::getMinimumChild(BSTnode *n)
{
    assert(n != nullptr);

    while (hasLeftChild(n))
    {
        n = n->left;

    }

    return n;
}

BSTnode *BinarySearchBasic::getPredecessorByNode(BSTnode *n)
{
    if (hasLeftChild(n))
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

BSTnode *BinarySearchBasic::getSuccessorByNode(BSTnode *n)
{
    if (hasRightChild(n))
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

BSTnode *BinarySearchBasic::getSibling(BSTnode *n)
{
    assert(n != nullptr);

    if (isRoot(n))
    {
        // la raiz no tiene hermano
        return nullptr;
    }
    else
    {
        if (isLeftChild(n) && hasRightChild(n->parent))
        {
            return n->parent->right;
        }
        else if (isRightChild(n) && hasLeftChild(n->parent))
        {
            return n->parent->left;
        }
        else
        {
            // si el padre de n no tiene un hijo opuesto, devuelva nulo
            return nullptr;
        }
    }
}

void BinarySearchBasic::updateSubtreeSizePath(BSTnode *n)
{
    if (n == nullptr)
    {
        return;
    }

    while (true)
    {
        updateSubtreeSize(n);

        if (n->parent == nullptr)
        {
            break;
        }
        else
        {
            n = n->parent;
        }
    }
}

void BinarySearchBasic::updateSubtreeSize(BSTnode *n)
{
    n->size = 1;

    if (n->left != nullptr)
    {
        n->size += n->left->size;
    }

    if (n->right != nullptr)
    {
        n->size += n->right->size;
    }
}

void BinarySearchBasic::detach(BSTnode *n, BSTnode *par)
{
    assert(n != nullptr);
    assert(par != nullptr);
    assert(n->parent == par);
    assert((par->left == n) || (par->right == n));

    clearParentReference(n);
    n->parent = nullptr;
}

void BinarySearchBasic::attachUp(BSTnode *n, BSTnode *par)
{
    assert(n != nullptr);
    assert(par != nullptr);
    assert(n->parent == nullptr);
    assert(!isNilLeaf(n));

    if (n->key < par->key)
    {
        assert(par->left == nullptr);
        par->left = n;
    }
    else
    {
        assert(par->right == nullptr);
        par->right = n;
    }

    n->parent = par;
}

void BinarySearchBasic::attachLeft(BSTnode *n, BSTnode *par)
{
    assert(n != nullptr);
    assert(par != nullptr);
    assert(n->parent == nullptr);
    assert(par->left == nullptr);

    if (isNilLeaf(n))
    {
        // nada
    }
    else
    {
        assert(n->key < par->key);
    }

    par->left = n;
    n->parent = par;
}

void BinarySearchBasic::attachRight(BSTnode *n, BSTnode *par)
{
    assert(n != nullptr);
    assert(par != nullptr);
    assert(n->parent == nullptr);
    assert(par->right == nullptr);

    if (isNilLeaf(n))
    {
        // nada
    }
    else
    {
        assert(n->key > par->key);
    }

    par->right = n;
    n->parent = par;
}

void BinarySearchBasic::fillInOrderElements(BSTnode *n, std::vector<int> &elements)
{
    if (n->left != nullptr)
    {
        fillInOrderElements(n->left, elements);
    }

    elements.push_back(n->key);

    if (n->right != nullptr)
    {
        fillInOrderElements(n->right, elements);
    }
}

