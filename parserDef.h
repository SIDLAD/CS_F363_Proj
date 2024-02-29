#ifndef PARSERDEF_MACRO
#define PARSERDEF_MACRO

#include "dataStructures/linkedList.c"
#include "lexer.h"
#include <stdbool.h>

#define MAX_LINE_SIZE_GRAMMAR 128
#define grammarFile "grammar.txt"
#define ENDOFFILE ((int)EPS)        // this is only for convenience of follow-set computation, as follow-set can never contain epsilon
#define STARTSYMBOL program

typedef struct grammar* grammar;
typedef struct table* table;
typedef struct parseTree* parseTree;
typedef struct firstAndFollow* FirstAndFollow;
typedef struct terminalBucketSet* TerminalBucketSet;

parseTree _parseTree = NULL;                //to be initialised via function call
grammar _grammar = NULL;                    //to be initialised via function call
table _table = NULL;                        //to be initialised via function call
FirstAndFollow _firstAndFollow = NULL;      //to be initialised via function call
TerminalBucketSet _terminalBucketSet = NULL;//to be initialised via function call

struct table{
    //fill in the details of table
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