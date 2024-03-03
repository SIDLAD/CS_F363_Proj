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

void free_stack(Stack *stack) {
    if (stack == NULL) {
        return;
    }
    // printf("%c",*(char*)stack->top);
    for (int i = 0; i <= stack->top; i++) {
        printf("%d",i);
        // printf("%c",stack->data[i]);
        free(stack->data[i]);
    }

    free(stack);
}


//tester code
int main() {
    Stack* stack = createStack();

    char a = 'C';
    char b = 'B';
    char c = 'A';

    push(stack, &a);
    push(stack, &b);
    push(stack, &c);

    // printf("Top element: %c\n", *(char*)peek(stack));

    // while (!isEmpty(stack)) {
    //     void* value = pop(stack);
    //     printf("Popped element: %c\n", *(char*)value);
    // }

    free_stack(stack);

    return 0;
}


#endif