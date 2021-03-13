#ifndef TREE_H
#define TREE_H

using namespace std;

class treeNode
{
public:
    treeNode();
    ~treeNode();

    treeNode *insert(treeNode *node, int key, int *duplicate);

    void print(treeNode *node);
    treeNode *search(treeNode *root, int id);

    treeNode *left;
    treeNode *right;
    int key;

private:
    treeNode *newTreeNode(int key);
    treeNode *rightRotation(treeNode *y);
    treeNode *leftRotation(treeNode *x);

    int getBalance(treeNode *node);
    int getHeight(treeNode *node);

    int balanceHeight;
};

#endif