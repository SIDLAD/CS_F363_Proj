//Group Number 31
//Siddharth Shah F2021A7PS2428P
//Vedang Bhupesh Shenvi Nadkarni F2020B5A70897P
//Shai Pranesh S F2020B2A70731P
//Krteyu Pillai F2021A7PS2522P
//Aryan Seth F2021A7PS2221P

#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexer.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LINE_SIZE_GRAMMAR 128
#define grammarFile "grammar.txt"
#define ENDOFFILE ((int)EPS)        // this is only for convenience of follow-set computation, as follow-set can never contain epsilon
#define STARTSYMBOL program

typedef struct grammar* grammar;
typedef struct table* table;
typedef struct parseTree* parseTree;
typedef struct firstAndFollow* FirstAndFollow;
typedef struct terminalBucketSet* TerminalBucketSet;
typedef struct linkedList* LinkedList;
typedef struct treeNode* TreeNode;
// typedef struct stackNode* StackNode;
// typedef struct stack* Stack;

extern parseTree _parseTree;                //to be initialized via function call
extern grammar _grammar;                    //to be initialized via function call
extern table _table;                        //to be initialized via function call
extern FirstAndFollow _firstAndFollow;      //to be initialized via function call

extern char* predictiveParsingTableCache;

// struct stackNode
// {
//     void* data;
//     StackNode next;
// };

// struct stack
// {
//     StackNode top;
// };

struct linkedList
{
    LinkedList next;
    void* data;
};

struct treeNode{
    TreeNode parent;
    TreeNode nextBrother;
    TreeNode firstChild;
    void* data;
};

struct table{
    int isErrorCell[N_TERMINALS_COUNT][TERMINALS_COUNT];     //if error not sync, 1. if error & syn, 2. if no error, 0. 
    LinkedList cells[N_TERMINALS_COUNT][TERMINALS_COUNT];
};

struct parseTree{
    TreeNode root;
};

struct firstAndFollow{
    TerminalBucketSet follow[N_TERMINALS_COUNT];
    TerminalBucketSet* first[N_TERMINALS_COUNT];
    int ruleCount[N_TERMINALS_COUNT];
};

struct grammar{
    LinkedList* NT[N_TERMINALS_COUNT];
    int ruleCount[N_TERMINALS_COUNT];
};

struct terminalBucketSet{
    bool val[TERMINALS_COUNT];
};

#endif