#include "redblacktango.h"

RedBlackNode *RedBlackTango::createNode(int key, void *value)
{
    return new RedBlackNode(key, value);
}

RedBlackNode *RedBlackTango::createNil()
{
    return createNil(nullptr);
}

RedBlackNode *RedBlackTango::createNil(BSTnode *parent)
{
    RedBlackNode *nil = createNode(NIL_KEY, nullptr);

    nil->color = "BLACK";
    nil->parent = parent;
    nil->blackHeight = 1;

    return nil;
}

void RedBlackTango::initializePerfectTree(int levels)
{

    _root = createPerfectSubtree(levels - 1, 0, 1);

}

RedBlackNode *RedBlackTango::createPerfectSubtree(int levels, int depth, int nextId)
{
    if ((levels - depth) == 0)
    {
        RedBlackNode *n = createNode(nextId, reinterpret_cast<void*>(nextId));
        ++nextId;

        TangoNode *nl = createNil(n);
        nl->depth = depth + 1;
        n->left = nl;

        TangoNode *nr = createNil(n);
        nr->depth = depth + 1;
        n->right = nr;

        n->size = 3;
        n->color = "BLACK";
        n->blackHeight = 2;
        n->depth = depth;
        n->marked = true;
        n->minDepth = depth;
        n->maxDepth = depth;

        return n;
    }
    else
    {
        /* construir subtrees */
        RedBlackNode *l = createPerfectSubtree(levels, depth + 1, nextId);
        nextId += std::pow(2, (levels - depth)) - 1;

        RedBlackNode *p = createNode(nextId, reinterpret_cast<void*>(nextId));
        ++nextId;

        RedBlackNode *r = createPerfectSubtree(levels, depth + 1, nextId);

        l->parent = p;
        p->left = l;

        r->parent = p;
        p->right = r;

        p->size = l->size + r->size + 1;
        p->color = "BLACK";
        p->blackHeight = 2;
        p->depth = depth;
        p->marked = true;
        p->minDepth = depth;
        p->maxDepth = depth;

        return p;
    }
}

RedBlackNode *RedBlackTango::tangoSplitImpl(TangoNode *tn, BSTnode *vRoot)
{
    assert(tn != nullptr);

    RedBlackNode *n = static_cast<RedBlackNode*>(tn);

    /*
     * prepara el arbol para split
     */

    BSTnode *vpar = vRoot->parent;

    if (vpar != nullptr)
    {
        detach(vRoot, vpar);
    }

    // marca
    bool vMark = isMarked(vRoot);

    if (vMark)
    {
        unmarkNode(vRoot);
    }

    /*
     * Red Black Split
     */

    RedBlackNode *k = static_cast<RedBlackNode*>(vRoot);
    RedBlackNode *tl = nullptr; // tree izquierdo
    RedBlackNode *vl = nullptr; // pivot para arbol izquierdo
    RedBlackNode *tr = nullptr; // tree derecho
    RedBlackNode *vr = nullptr; // pivot para arbol derecho

    while (!isTangoLeaf(k))
    {

        assert(isRoot(k));

        // prepare para merge
        RedBlackNode *kl = static_cast<RedBlackNode*>(k->left);
        RedBlackNode *kr = static_cast<RedBlackNode*>(k->right);

        detach(kl, k);
        detach(kr, k);


        {
            kl->color = "BLACK";
            updateBlackHeight(kl);

            kr->color = "BLACK";
            updateBlackHeight(kr);
        }

        if (n->key < k->key)
        {
            tr = tangoMergeRedBlack(kr, vr, tr);
            assert(tr->color == "BLACK");

            vr = k;
            k = kl;

        }
        else if (n->key > k->key)
        {
            tl = tangoMergeRedBlack(tl, vl, kl);
            assert(tl->color == "BLACK");

            vl = k;
            k = kr;

        }
        else
        {

            tl = tangoMergeRedBlack(tl, vl, kl);
            assert(tl->color == "BLACK");
            vl = nullptr;

            tr = tangoMergeRedBlack(kr, vr, tr);
            assert(tr->color == "BLACK");
            vr = nullptr;

            attachLeft(tl, k);
            attachRight(tr, k);

            break;
        }
    }

    assert(vl == nullptr);
    assert(vr == nullptr);

    assert(isRoot(k));
    assert(n == k);

    /*
     * Restaurar árbol después de split
     */

    if (vpar == nullptr)
    {
        this->_root = n;
    }
    else
    {
        attachUp(n, vpar);
    }

    // restaurar mark
    if (vMark)
    {
        markNode(n);
    }

    return n;
}

RedBlackNode *RedBlackTango::tangoMergeImpl(TangoNode *tn)
{
    assert(tn != nullptr);

    RedBlackNode *n = static_cast<RedBlackNode*>(tn);

    RedBlackNode *np = static_cast<RedBlackNode*>(n->parent);
    RedBlackNode *nl = static_cast<RedBlackNode*>(n->left);
    RedBlackNode *nr = static_cast<RedBlackNode*>(n->right);
    bool root_mark = false;

    if (isMarked(n))
    {
        root_mark = true;
        unmarkNode(n);
    }

    if (np != nullptr)
    {
        detach(n, np);
    }

    detach(nl, n);
    detach(nr, n);

    {

        n->color = "BLACK";
        updateBlackHeight(n);

        // if (!isTangoLeaf(nl)) {
        nl->color = "BLACK";
        updateBlackHeight(nl);
        // }

        // if (!isTangoLeaf(nr)) {
        nr->color = "BLACK";
        updateBlackHeight(nr);
        // }
    }

    RedBlackNode *newroot = tangoMergeRedBlack(nl, n, nr);


    if (np == nullptr)
    {
        this->_root = newroot;
    }
    else
    {
        attachUp(newroot, np);
    }


    if (root_mark)
    {
        markNode(newroot);
    }

    return newroot;

}

RedBlackNode *RedBlackTango::tangoMergeRedBlack(RedBlackNode *nl, RedBlackNode *n, RedBlackNode *nr)
{


    if (n == nullptr)
    {

        if (nr != nullptr)
        {
            assert(nr->color == "BLACK");
            n = nr;

        }
        else if (nl != nullptr)
        {
            assert(nl->color == "BLACK");
            n = nl;

        }
        else
        {
            assert(false);
        }

    }
    else if (isTangoLeaf(nl) && isTangoLeaf(nr))
    {
        if (nl != nullptr)
        {
            attachLeft(nl, n);
            assert(nl->color == "BLACK");
        }

        if (nr != nullptr)
        {
            attachRight(nr, n);
            assert(nr->color == "BLACK");
        }

        n->color = "RED";
        updateBlackHeight(n);
        assert(n->blackHeight == 1);

    }
    else if (isTangoLeaf(nl))
    {
        attachAsMinimum(n, nr);


        if (nl != nullptr)
        {
            attachLeft(nl, n);
            assert(nl->color == "BLACK");
        }


        n->color = "RED";
        updateBlackHeight(n);

    }
    else if (isTangoLeaf(nr))
    {

        attachAsMaximum(n, nl);


        if (nr != nullptr)
        {
            attachRight(nr, n);
            assert(nr->color == "BLACK");
        }


        n->color = "RED";
        updateBlackHeight(n);

    }
    else
    {

        int lh = nl->blackHeight;
        int rh = nr->blackHeight;

        if (lh == rh)
        {
            attachLeft(nl, n);
            attachRight(nr, n);

            assert(nl->color == "BLACK");
            assert(nr->color == "BLACK");

            n->color = "RED";

        }
        else if (lh < rh)
        {
            TangoNode *p = findMinWithBlackHeight(nr, nl->blackHeight);
            BSTnode *pp = p->parent;

            assert(pp != nullptr);

            attachLeft(nl, n);
            assert(nl->color == "BLACK");

            detach(p, pp);
            attachRight(p, n);

            assert(p == n->right);

            attachLeft(n, pp);
            updateMinMaxPath(n);

            n->color = "RED";

        }
        else
        {
            TangoNode *p = findMaxWithBlackHeight(nl, nr->blackHeight);
            BSTnode *pp = p->parent;

            assert(pp != nullptr);

            attachRight(nr, n);
            assert(nr->color == "BLACK");

            detach(p, pp);
            attachLeft(p, n);

            assert(p == n->left);

            attachRight(n, pp);
            updateMinMaxPath(n);

            n->color = "RED";
        }
    }

    updateMinMaxPath(n);
    updateSubtreeSizePath(n);

    insertFixUpCase1(n);

    updateBlackHeightPath(n);

    RedBlackNode *newroot = n;
    while (newroot->parent != nullptr)
    {
        newroot = static_cast<RedBlackNode*>(newroot->parent);
    }

    return newroot;
}

TangoNode *RedBlackTango::findMinWithBlackHeight(RedBlackNode *r, int blackHeight)
{

    assert(r != nullptr);
    assert(r->blackHeight > blackHeight);

    while (!isTangoLeaf(r))
    {
        if (r->color == "BLACK" && r->blackHeight == blackHeight)
        {
            break;
        }

        r = static_cast<RedBlackNode*>(r->left);

        ////_stats::incOtherTraversals();
    }

    assert(r->color == "BLACK");
    assert(r->blackHeight == blackHeight);

    return r;
}

TangoNode *RedBlackTango::findMaxWithBlackHeight(RedBlackNode *r, int blackHeight)
{

    assert(r != nullptr);
    assert(r->blackHeight > blackHeight);

    while (!isTangoLeaf(r))
    {
        if (r->color == "BLACK" && r->blackHeight == blackHeight)
        {
            break;
        }

        r = static_cast<RedBlackNode*>(r->right);

        ////_stats::incOtherTraversals();
    }

    assert(r->color == "BLACK");
    assert(r->blackHeight == blackHeight);

    return r;
}

void RedBlackTango::attachAsMaximum(BSTnode *n, BSTnode *t)
{
    assert(n != nullptr);
    assert(t != nullptr);

    BSTnode *a = getMaximumChild(t);
    BSTnode *ar = a->right;

    assert(a->key < n->key);
    assert(a->right != nullptr);
    assert(n->left == nullptr);

    detach(ar, a);
    attachLeft(ar, n);

    attachRight(n, a);

    updateMinMaxPath(n);
}

void RedBlackTango::attachAsMinimum(BSTnode *n, BSTnode *t)
{
    assert(n != nullptr);
    assert(t != nullptr);

    BSTnode *a = getMinimumChild(t);
    BSTnode *al = a->left;

    assert(n->key < a->key);
    assert(a->left != nullptr);
    assert(n->right == nullptr);

    detach(al, a);
    attachRight(al, n);

    attachLeft(n, a);

    updateMinMaxPath(n);
}

void RedBlackTango::updateBlackHeightPath(RedBlackNode *n)
{
    assert(n != nullptr);

    updateBlackHeight(n);

    while (!isRoot(n))
    {
        n = static_cast<RedBlackNode*>(n->parent);
        updateBlackHeight(n);
    }
}

void RedBlackTango::updateBlackHeight(RedBlackNode *n)
{
    assert(n != nullptr);

    if (isNilLeaf(n))
    {
        assert(n->blackHeight == 1);
        return;
    }

    int lh = 1;
    int rh = 1;

    if (hasLeftChild(n))
    {
        lh = (static_cast<RedBlackNode*>(n->left))->blackHeight;
    }

    if (hasRightChild(n))
    {
        rh = (static_cast<RedBlackNode*>(n->right))->blackHeight;
    }

    assert(lh == rh);

    n->blackHeight = lh;

    if (n->color == "BLACK")
    {
        n->blackHeight += 1;
    }
}

void RedBlackTango::insertFixUpCase1(RedBlackNode *n)
{
    if (isRoot(n))
    {
        n->color = "BLACK";
        updateBlackHeight(n);
    }
    else
    {
        updateBlackHeight(n);
        insertFixUpCase2(n);
    }
}

void RedBlackTango::insertFixUpCase2(RedBlackNode *n)
{
    assert(n != nullptr);
    RedBlackNode *p = static_cast<RedBlackNode*>(n->parent);

    if (isBlack(p))
    {
        updateBlackHeight(p);
        return;
    }
    else
    {
        insertFixUpCase3(n);
    }
}

void RedBlackTango::insertFixUpCase3(RedBlackNode *n)
{

    assert(n != nullptr);

    RedBlackNode *p = static_cast<RedBlackNode*>(n->parent);
    assert(p != nullptr);

    RedBlackNode *g = static_cast<RedBlackNode*>(p->parent);
    assert(g != nullptr);

    RedBlackNode *u = static_cast<RedBlackNode*>(getSibling(p));

    if ((u != nullptr) && isRed(u))
    {
        p->color = "BLACK";
        updateBlackHeight(p);
        u->color = "BLACK";
        updateBlackHeight(u);

        g->color = "RED";
        updateBlackHeight(g);

        insertFixUpCase1(g);
    }
    else
    {
        insertFixUpCase4(n);
    }
}

void RedBlackTango::insertFixUpCase4(RedBlackNode *n)
{

    assert(n != nullptr);

    RedBlackNode *p = static_cast<RedBlackNode*>(n->parent);
    assert(p != nullptr);

    assert(p->parent != nullptr);

    if (isLeftChild(p))
    {
        if (isRightChild(n))
        {
            rotateLeft(p);

            updateBlackHeight(p);
            updateBlackHeight(n);

            n = p;
        }
    }
    else
    {
        if (isLeftChild(n))
        {
            rotateRight(p);

            updateBlackHeight(p);
            updateBlackHeight(n);

            n = p;
        }
    }

    insertFixUpCase5(n);
}

void RedBlackTango::insertFixUpCase5(RedBlackNode *n)
{
    assert(n != nullptr);

    RedBlackNode *p = static_cast<RedBlackNode*>(n->parent);
    assert(p != nullptr);

    RedBlackNode *g = static_cast<RedBlackNode*>(p->parent);
    assert(g != nullptr);

    p->color = "BLACK";
    g->color = "RED";

    if (isLeftChild(p))
    {
        rotateRight(g);
    }
    else
    {
        rotateLeft(g);
    }

    updateBlackHeight(g);
    updateBlackHeight(p);
}

void RedBlackTango::deleteFixUpCase1(RedBlackNode *n)
{
    assert(n != nullptr);
    assert(n->color == "BLACK");

    if (isRoot(n))
    {
        // done
    }
    else
    {
        deleteFixUpCase2(n);
    }
}

void RedBlackTango::deleteFixUpCase2(RedBlackNode *n)
{
    assert(n != nullptr);
    RedBlackNode *p = static_cast<RedBlackNode*>(n->parent);
    RedBlackNode *s = static_cast<RedBlackNode*>(getSibling(n));

    if (isRed(s))
    {
        assert(p->color == "BLACK");

        p->color = "RED";
        s->color = "BLACK";

        if (isLeftChild(n))
        {
            rotateLeft(p);
        }
        else
        {
            rotateRight(p);
        }

    }

    deleteFixUpCase3(n);
}

void RedBlackTango::deleteFixUpCase3(RedBlackNode *n)
{
    assert(n != nullptr);
    RedBlackNode *p = static_cast<RedBlackNode*>(n->parent);
    RedBlackNode *s = static_cast<RedBlackNode*>(getSibling(n));

    assert(s != nullptr);
    assert(s->color == "BLACK");

    RedBlackNode *sl = static_cast<RedBlackNode*>(s->left);
    RedBlackNode *sr = static_cast<RedBlackNode*>(s->right);

    if (isBlack(p) && isBlack(s) && isBlack(sl) && isBlack(sr))
    {
        s->color = "RED";

        deleteFixUpCase1(p);
        return;

    }
    else
    {
        deleteFixUpCase4(n);
    }
}

void RedBlackTango::deleteFixUpCase4(RedBlackNode *n)
{
    assert(n != nullptr);
    RedBlackNode *p = static_cast<RedBlackNode*>(n->parent);
    RedBlackNode *s = static_cast<RedBlackNode*>(getSibling(n));

    assert(s != nullptr);

    RedBlackNode *sl = static_cast<RedBlackNode*>(s->left);
    RedBlackNode *sr = static_cast<RedBlackNode*>(s->right);

    if (isRed(p) && isBlack(s) && isBlack(sl) && isBlack(sr))
    {
        s->color = "RED";
        p->color = "BLACK";


        return;
    }
    else
    {
        deleteFixUpCase5(n);
    }
}

void RedBlackTango::deleteFixUpCase5(RedBlackNode *n)
{
    assert(n != nullptr);
    // RedBlackNode p = (RedBlackNode) n.parent;
    RedBlackNode *s = static_cast<RedBlackNode*>(getSibling(n));

    assert(s != nullptr);
    RedBlackNode *sl = static_cast<RedBlackNode*>(s->left);
    RedBlackNode *sr = static_cast<RedBlackNode*>(s->right);

    if (isLeftChild(s))
    {

        if (isBlack(sl) && isRed(sr))
        {
            s->color = "RED";
            sr->color = "BLACK";

            rotateLeft(s);
        }
    }
    else
    {

        if (isRed(sl) && isBlack(sr))
        {
            s->color = "RED";
            sl->color = "BLACK";

            rotateRight(s);
        }
    }

    deleteFixUpCase6(n);
}

void RedBlackTango::deleteFixUpCase6(RedBlackNode *n)
{
    assert(n != nullptr);
    RedBlackNode *p = static_cast<RedBlackNode*>(n->parent);
    RedBlackNode *s = static_cast<RedBlackNode*>(getSibling(n));

    assert(s != nullptr);
    RedBlackNode *sl = static_cast<RedBlackNode*>(s->left);
    RedBlackNode *sr = static_cast<RedBlackNode*>(s->right);

    s->color = p->color;
    p->color = "BLACK";

    if (isLeftChild(s))
    {
        sl->color = "BLACK";

        rotateRight(p);
    }
    else
    {
        sr->color = "BLACK";

        rotateLeft(p);
    }
}

bool RedBlackTango::isRed(RedBlackNode *n)
{
    assert(n != nullptr);

    return (n->color == "RED");
}

bool RedBlackTango::isBlack(RedBlackNode *n)
{
    assert(n != nullptr);

    return (n->color == "BLACK");
}

BSTnode *RedBlackTango::getMinimumChild(BSTnode *n)
{
    BSTnode *m = TangoTree::getMinimumChild(n);

    if (isNilLeaf(m))
    {
        return m->parent;
    }
    else
    {
        return m;
    }
}

BSTnode *RedBlackTango::getMaximumChild(BSTnode *n)
{
    BSTnode *m = TangoTree::getMaximumChild(n);

    if (isNilLeaf(m))
    {
        return m->parent;
    }
    else
    {
        return m;
    }
}

BSTnode *RedBlackTango::getPredecessorByNode(BSTnode *n)
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

BSTnode *RedBlackTango::getSuccessorByNode(BSTnode *n)
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
