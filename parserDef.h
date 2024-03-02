#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexer.c"
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

extern parseTree _parseTree;                //to be initialized via function call
extern grammar _grammar;                    //to be initialized via function call
extern table _table;                        //to be initialized via function call
extern FirstAndFollow _firstAndFollow;      //to be initialized via function call
extern TerminalBucketSet _terminalBucketSet;//to be initialized via function call

extern char* predictiveParsingTableCache;


struct linkedList
{
    LinkedList next;
    void* data;
};

struct table{
    int isErrorCell[N_TERMINALS_COUNT][TERMINALS_COUNT];     //if error not sync, 1. if error & syn, 2. if no error, 0. 
    LinkedList cells[N_TERMINALS_COUNT][TERMINALS_COUNT];
};

struct parseTree{
    //fill in the details of parseTree
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