/*
    Our trie datastructure is a modified version of the basic trie data-structure.
    In our modified DS, TC = O(APLHABET_COUNT * length_of_string) in the worst case
                           = O(length_of_string) in the average (sparsely populated) test case
                    and SC = O(number of nodes in the Trie * sizeof(data item)))
*/
#ifndef TRIEADT_MACRO
#define TRIEADT_MACRO

#include <stdlib.h>

#define ALPHABET_COUNT 85

typedef struct trie *Trie;
typedef struct trieEdge *TrieEdge;

struct trieEdge
{
    Trie ptr;
    char ch;
};

struct trie
{
    TrieEdge *children;
    int childCount;
    void *data;
};

Trie createTrieNode()
{
    Trie node = (Trie)malloc(sizeof(struct trie));
    node->children = NULL;
    node->data = NULL;
    node->childCount = 0;
    return node;
}

void insertDataIntoTrieNode(void *_data, Trie node)
{
    node->data = _data;
}

Trie goToNextTrieNode(char _ch, Trie node)          //this function will return NULL if the node doesn't exist
{
    for(int i = 0; i < node->childCount; i++)
    {
        if(node->children[i]->ch == _ch)
        {
            return node->children[i]->ptr;
        }
    }
    return NULL;
}

Trie goToNextTrieNodeForced(char _ch, Trie node)    // this function will create a new node if it doesn't exist already
{
    for (int i = 0; i < node->childCount; i++)
    {
        if (node->children[i]->ch == _ch)
        {
            return node->children[i]->ptr;
        }
    }
    node->children = (TrieEdge*)realloc(node->children,sizeof(TrieEdge) * ++(node->childCount));
    node->children[node->childCount - 1] = (TrieEdge)malloc(sizeof(struct trieEdge));
    node->children[node->childCount - 1]->ch = _ch;
    node->children[node->childCount - 1]->ptr = createTrieNode();
    return node->children[node->childCount - 1]->ptr;
}

void* getDataFromTrie(char* str, Trie root)     //if NULL is returned, then there is no such entry in the trie
{
    if(root == NULL)
    {
        return NULL;
    }

    if(*str == (char)0)
    {
        return root->data;      //if NULL is returned, then there is no such entry in the trie  
    }
    
    return getDataFromTrie(str + 1,goToNextTrieNode(*str,root));
}

void* insertIntoTrie(void* data,char* str,Trie root)    
{
    /*
      will override existing data if the node already exists. Note that the returned value 
      will be that of the void pointer to the previously existing data item corresponding to the same string,
      so the calling function would have to free it manually after typecasting the void pointer returned.
      (in case existing data needs to be preserved and stored)
    */

    if(*str == (char)0)
    {
        void* tmp = root->data;
        root->data = data;
        return tmp;
    }

    Trie child = goToNextTrieNodeForced(*str,root);
    return insertIntoTrie(data,str + 1,child);
}

Trie freeTrieNodeRecursive(Trie root)
{
    for (int i = 0; i < root->childCount; i++)
    {
        freeTrieNodeRecursive(root->children[i]->ptr);
        free(root->children[i]);
    }

    free(root->data);
    free(root);
}

#endif