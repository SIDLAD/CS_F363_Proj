#ifndef TREEADT_MACRO
#define TREEADT_MACRO

#include <stdlib.h>
#include <stdbool.h>
//TODO
typedef struct tree Tree;
// typedef struct node Node;
typedef struct node
{
    Node* parent;
    Node** children;
    bool leaf;
    void* data;
    int numchild; // number of children
    int child_pos; //position of child relative to parent
}Node;

struct tree{
    Node* root;
};





Node* createNode(Node* parent){
    Node* node=(Node*)malloc(sizeof(Node));
    node->parent=parent;
    node->children=NULL;
    node->data=NULL;
    node->child_pos=NULL;
    return node;
}

void insertDataIntoTreeNode(void *_data,Node* node){
    node->data=_data;   
}

void createAndPopulateChildren(Node* node, int nos, void* data[nos]){
     
    node->children=(Node**)malloc(nos*sizeof(Node*));
    for(int i=0;i<nos;i++){
        node->children[i]=(Node*)malloc(sizeof(Node));
    }
    for(int i=0;i<nos;i++){
        node->children[i]->data=data[i];
        node->children[i]->child_pos=i;
    }
}

Tree* createTree(){
    Tree* tr=(Tree*)malloc(sizeof(Tree));
    tr->root=createNode(NULL);
}

void freeTreeNodeRecursive(Tree* tr)
{
    Node* root=tr->root;
    for (int i = 0; i < sizeof(root->children)/sizeof(struct node); i++)
    {
        freeTreeNodeRecursive(root->children[i]);
        free(root->children[i]);
    }

    free(root->data);
    free(root);
}









#endif