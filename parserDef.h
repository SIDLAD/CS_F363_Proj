#ifndef PARSERDEF_H
#define PARSERDEF_H

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

extern parseTree _parseTree;                //to be initialised via function call
extern grammar _grammar;                    //to be initialised via function call
extern table _table;                        //to be initialised via function call
extern FirstAndFollow _firstAndFollow;      //to be initialised via function call
extern TerminalBucketSet _terminalBucketSet;//to be initialised via function call

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