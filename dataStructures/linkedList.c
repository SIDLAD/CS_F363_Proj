#include<stddef.h>

typedef struct linkedList* LinkedList;

struct linkedList
{
    LinkedList next;
    void* data;
};

LinkedList createLinkedListNode()
{
    LinkedList list = (LinkedList)malloc(sizeof(struct linkedList));
    list->next = NULL;
    list->data = NULL;
    return list;
}