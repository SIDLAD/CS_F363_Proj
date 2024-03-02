// #ifndef TREEADT_MACRO
// #define TREEADT_MACRO

#include <stdlib.h>
#include <stdbool.h>

typedef struct node *Node;

struct node{
    Node parent;
    Node brother;
    Node child;
    void* data;
    
};

Node createNode(Node parent){
    Node node = (Node)malloc(sizeof(struct node));
    node->parent=parent;
    node->child=NULL;
    node->brother=NULL;
    node->data=NULL;
    return node;
}

void insertDataIntoTreeNode(void *_data, Node node){
    node->data=_data;
}

void addBrotherNode(Node current, Node brother){
    current->brother=brother;
}

void addChild(Node current, Node child){
    current->child=child;
}

void freeAllTreeNodes(Node root){
    if(root==NULL){
        return;
    }
    if(root->brother!=NULL){
        freeAllTreeNodes(root->brother);
    }
    if(root->child!=NULL){
        freeAllTreeNodes(root->child);
    }
    free(root->data);
    free(root);
}

void main(){
    Node node=createNode(NULL);
    
}





// #endif
