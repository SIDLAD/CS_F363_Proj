//Group Number 31
//Siddharth Shah 2021A7PS2428P
//Vedang Bhupesh Shenvi Nadkarni 2020B5A70897P
//Shai Pranesh S 2020B2A70731P
//Krteyu Pillai 2021A7PS2522P
//Aryan Seth 2021A7PS2221P

#ifndef PARSER_H
#define PARSER_H
#include "parserDef.h"

extern grammar populateGrammar(grammar g, char* grammarFileName); //function to parse the grammar file and populate the grammar ds
extern FirstAndFollow computeFirstAndFollowSets (grammar G);
extern table createParseTable(FirstAndFollow F, table T);      //if firstAndFollow is null,then call computeFirstAndFollowSets function internally. table should be passed as a dummy variable with NULL
extern bool parseInputSourceCode(char *testcaseFile, table T);
extern void printParseTree(parseTree PT, char *outfile);       //lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol

extern void freeParseTree();
extern void enumToStr(Vocabulary v, char *dest);
#endif