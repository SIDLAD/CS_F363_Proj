#include "parserDef.h"

grammar populateGrammar(grammar g, char* grammarFileName); //function to parse the grammar file and populate the grammar ds
FirstAndFollow computeFirstAndFollowSets (grammar G);   //grammar should be prepopulated, so need to create a function for doing that. If grammar is passed as NULL, then call that function locally instead.
table createParseTable(FirstAndFollow F, table T);      //if firstAndFollow is null,then call computeFirstAndFollowSets function internally. table should be passed as a dummy variable with NULL
void parseInputSourceCode(char *testcaseFile, table T);
void printParseTree(parseTree PT, char *outfile);       //lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol

//TODO: should we add the supporting functions locally used?
void freeParseTree(parseTree PT);
