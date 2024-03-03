/*
    Our tree datastructure is a modified version of the basic tree datastructure.
    Here, the parent points only to its first child, and other children are accessed in a manner similar to a linked list from
    the first child via nextBrother.
    The described inorder euler traversal will not be an issue either, as it is convenient to go to the first child if it exists,
    then display its own contents, followed by going to its other children's contents iteratively.
*/
#ifndef TREEADT_MACRO
#define TREEADT_MACRO

#include <stdlib.h>
#include <stdbool.h>    

typedef struct treeNode* TreeNode;

struct treeNode{
    TreeNode parent;
    TreeNode nextBrother;
    TreeNode firstChild;
    void* data;
    
};

TreeNode createTreeNode(TreeNode parent,void* data)
{
    TreeNode treeNode = (TreeNode)malloc(sizeof(struct treeNode));
    treeNode->parent=parent;
    treeNode->firstChild=NULL;
    treeNode->nextBrother=NULL;
    treeNode->data=data;
    return treeNode;
}

void createBrotherNode(TreeNode current,void* data)
{
    current->nextBrother = createTreeNode(current->parent,data);
}

void createChildNode(TreeNode current, void* data)
{
    if(current->firstChild == NULL)                     //our use case will be limited to this if-clause
    {
        TreeNode treeNode=createTreeNode(current,data);
        current->firstChild=treeNode;
        return;
    }

    current = current->firstChild;
    while(current->nextBrother != NULL)current = current->nextBrother;
    current->nextBrother = createTreeNode(current->parent,data);
}

void freeTreeNodeRecursive(TreeNode treeNode)
{
    if (treeNode == NULL) {
        return;
    }

    free(treeNode->data);
    freeTreeNodeRecursive(treeNode->firstChild);

    // Move to the next nextBrother treeNode and free its data
    TreeNode current = treeNode->nextBrother;
    while (current != NULL) {
        TreeNode next = current->nextBrother;
        freeTreeNodeRecursive(current);
        current = next;
    }

    // Free the current treeNode itself
    free(treeNode);
}

// void main()
// {
//     TreeNode node=createTreeNode(NULL,NULL);

// }

#endif
