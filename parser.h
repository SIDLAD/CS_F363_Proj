#include "parserDef.h"

extern grammar populateGrammar(grammar g, char* grammarFileName); //function to parse the grammar file and populate the grammar ds
extern FirstAndFollow computeFirstAndFollowSets (grammar G);
extern table createParseTable(FirstAndFollow F, table T);      //if firstAndFollow is null,then call computeFirstAndFollowSets function internally. table should be passed as a dummy variable with NULL
extern void parseInputSourceCode(char *testcaseFile, table T);
extern void printParseTree(parseTree PT, char *outfile);       //lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol

extern void freeParseTree(parseTree PT);
extern void enumToStr(Vocabulary v, char *dest);
