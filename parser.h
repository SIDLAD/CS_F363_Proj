#include "parserDef.h"

FirstAndFollow computeFirstAndFollowSets (grammar G);
table createParseTable(FirstAndFollow F, table T);
void parseInputSourceCode(char *testcaseFile, table T);
void printParseTree(parseTree PT, char *outfile);       //lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol