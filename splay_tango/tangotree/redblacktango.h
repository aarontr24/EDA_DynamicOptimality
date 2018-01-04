#ifndef REDBLACKTANGO_H
#define REDBLACKTANGO_H

#include <cmath>
#include <redblacknode.h>
#include <tangotree.h>

class RedBlackTango : public TangoTree
{
protected:
        RedBlackNode *createNode(int key, void *value) override;

        RedBlackNode *createNil() override;

        RedBlackNode *createNil(BSTnode *parent) override;

    public:
        void initializePerfectTree(int levels) override;

    private:
        RedBlackNode *createPerfectSubtree(int levels, int depth, int nextId);

    protected:
        RedBlackNode *tangoSplitImpl(TangoNode *tn, BSTnode *vRoot) override;

        RedBlackNode *tangoMergeImpl(TangoNode *tn) override;

    private:
        RedBlackNode *tangoMergeRedBlack(RedBlackNode *nl, RedBlackNode *n, RedBlackNode *nr);

        TangoNode *findMinWithBlackHeight(RedBlackNode *r, int blackHeight);

        TangoNode *findMaxWithBlackHeight(RedBlackNode *r, int blackHeight);

        void attachAsMaximum(BSTnode *n, BSTnode *t);

        void attachAsMinimum(BSTnode *n, BSTnode *t);

    protected:
        virtual void updateBlackHeightPath(RedBlackNode *n);

        virtual void updateBlackHeight(RedBlackNode *n);

        virtual void insertFixUpCase1(RedBlackNode *n);

    private:
        void insertFixUpCase2(RedBlackNode *n);

        void insertFixUpCase3(RedBlackNode *n);

        void insertFixUpCase4(RedBlackNode *n);

        void insertFixUpCase5(RedBlackNode *n);

    protected:
        virtual void deleteFixUpCase1(RedBlackNode *n);

    private:
        void deleteFixUpCase2(RedBlackNode *n);

        void deleteFixUpCase3(RedBlackNode *n);

        void deleteFixUpCase4(RedBlackNode *n);

        void deleteFixUpCase5(RedBlackNode *n);

        void deleteFixUpCase6(RedBlackNode *n);

    protected:
        virtual bool isRed(RedBlackNode *n);

        virtual bool isBlack(RedBlackNode *n);

        BSTnode *getMinimumChild(BSTnode *n) override;

        BSTnode *getMaximumChild(BSTnode *n) override;

        BSTnode *getPredecessorByNode(BSTnode *n) override;

        BSTnode *getSuccessorByNode(BSTnode *n) override;
};

#endif // REDBLACKTANGO_H
