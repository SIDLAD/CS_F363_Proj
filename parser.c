#include "parserDef.h"
#include <stdio.h>
#include <string.h>

FirstAndFollow computeFirstAndFollowSets (grammar G)
{
    //TODO
}

TerminalBucketSet createSet()
{
    TerminalBucketSet set = (TerminalBucketSet)calloc(1,sizeof(struct terminalBucketSet));  //calloc used instead of malloc so that all bits are initialised to zero
    return set;
}

FirstAndFollow initializeFirstAndFollow(FirstAndFollow _firstAndFollow){
    _firstAndFollow = (FirstAndFollow)malloc(sizeof(struct firstAndFollow));
    for(int i = 0; i < N_TERMINALS_COUNT ; i++)
    {
        _firstAndFollow->follow[i] = createSet();
        _firstAndFollow->first[i] = NULL;
        _firstAndFollow->ruleCount[i] = 0;
    }
    return _firstAndFollow;
}

TerminalBucketSet* increaseSetArraySize(TerminalBucketSet* setArray,int* currentSize) // &ruleCount[i] should be passed as currentSize-+
{
    setArray = realloc(setArray, ((*currentSize)++ + 1) * sizeof(struct terminalBucketSet));
    memset(setArray + *currentSize - 1,0, sizeof(struct terminalBucketSet));        //initialising all bits of struct to zero ie array is initialised to all false
    return setArray;
}

void freeParseTree(parseTree PT)
{
    //TODO: Required to measure the lexing and parsing time in case of multiple executions within while loop of driver code
}