#include <stdio.h>
#include "parserDef.h"

FirstAndFollow computeFirstAndFollowSets (Grammar G);
Table createParseTable(FirstAndFollow F, Table T);
void parseInputSourceCode(char *testcaseFile, Table T);
void printParseTree(ParseTree PT, char *outfile);       //lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol