typedef struct linkedList* LinkedList;

struct linkedList
{
    LinkedList next;
    void* data;
};