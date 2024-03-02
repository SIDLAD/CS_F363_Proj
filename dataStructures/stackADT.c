#ifndef STACKADT_MACRO
#define STACKADT_MACRO

//TODO
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct {
    void* data[MAX_SIZE];
    int top;
} Stack;

Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = -1;
    return stack;
}

bool isEmpty(Stack* stack) {
    return stack->top == -1;
}

bool isFull(Stack* stack) {
    return stack->top == MAX_SIZE - 1;
}

void push(Stack* stack, void* value) {
    if (isFull(stack)) {
        printf("Stack is full. Cannot push.\n");
        return;
    }
    stack->data[++stack->top] = value;
}

void* pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty. Cannot pop.\n");
        return NULL;
    }
    return stack->data[stack->top--];
}

void* peek(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty. Cannot peek.\n");
        return NULL;
    }
    return stack->data[stack->top];
}

void freeStack(Stack* stack) {
    free(stack);
}


//tester code
// int main() {
//     Stack* stack = createStack();

//     int a = 10;
//     double b = 3.14;
//     char c = 'A';

//     push(stack, &a);
//     push(stack, &b);
//     push(stack, &c);

//     printf("Top element: %c\n", *(char*)peek(stack));

//     while (!isEmpty(stack)) {
//         void* value = pop(stack);
//         printf("Popped element: %c\n", *(char*)value);
//     }

//     freeStack(stack);

//     return 0;
// }


#endif