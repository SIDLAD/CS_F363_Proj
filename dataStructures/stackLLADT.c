#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct {
    Node* top;
} Stack;

Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack != NULL) {
        stack->top = NULL;
    }
    return stack;
}

bool isEmpty(Stack* stack){
    return stack->top==NULL;
}

void push(Stack* stack, void* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->data = data;
        newNode->next = stack->top;
        stack->top = newNode;
    } else {
        printf("Error: Memory allocation failed.\n");
    }
}

void* pop(Stack* stack) {
    if (stack->top == NULL) {
        printf("Error: Stack is empty.\n");
        return NULL;
    }

    Node* topNode = stack->top;
    void* data = topNode->data;
    stack->top = topNode->next;
    free(topNode);
    return data;
}

void freeStack(Stack* stack) {
    Node* current = stack->top;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(stack);
}


//testing
int main() {
    Stack* stack = createStack();

    // Pushing elements onto the stack
    int a = 10, b = 20, c = 30;
    push(stack, &a);
    push(stack, &b);
    push(stack, &c);

    // Popping elements from the stack
    int* popped1 = (int*)pop(stack);
    int* popped2 = (int*)pop(stack);
    int* popped3 = (int*)pop(stack);

    if (popped1 != NULL) printf("Popped: %d\n", *popped1);
    if (popped2 != NULL) printf("Popped: %d\n", *popped2);
    if (popped3 != NULL) printf("Popped: %d\n", *popped3);
    printf("%d",isEmpty(stack));
    freeStack(stack);
    return 0;
}
