#ifndef TREEADT_MACRO
#define TREEADT_MACRO

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

void addAndCreateBrotherNode(Node current,void* data){
    Node node=createNode(current);
    node->data=data;
    current->brother=node;
}

void addChild(Node current, void* data){
    Node node=createNode(current);
    node->data=data;
    current->child=node;
}

void free_node_data(Node node) {
    if (node == NULL) {
        return;
    }

    free(node->data);
    free_node_data(node->child);

    // Move to the next brother node and free its data
    Node current = node->brother;
    while (current != NULL) {
        Node next = current->brother;
        free_node_data(current);
        current = next;
    }

    // Free the current node itself
    free(node);
}

void main(){
    Node node=createNode(NULL);

}





#endif
