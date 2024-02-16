#include "parserDef.h"
#include <stdbool.h>

#define total_token 53
#define Terminals 37
#define N_Terminals (total_token-Terminals)

typedef struct firstandfollow* FirstAndFollow;

typedef struct bucket{
    bool val[Terminals];
}Bucket;

struct firstandfollow{
    Bucket follow[N_Terminals];
    Bucket* first[N_Terminals];
};

FirstAndFollow initializeFirstAndFollow(){
    FirstAndFollow list = (FirstAndFollow)malloc(sizeof(struct firstandfollow));
    return list;
}

Bucket* createDyanamicBucket(int len){
    // creates a dyanmic bucket(boolean array) of size len and returns it.
    Bucket* tmp = (Bucket*)calloc(len, sizeof(Bucket));
    return tmp;
}

FirstAndFollow computeFirstAndFollowSets (grammar G);
table createParseTable(FirstAndFollow F, table T);
void parseInputSourceCode(char *testcaseFile, table T);
void printParseTree(parseTree PT, char *outfile);       //lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol