//Group Number 31
//Siddharth Shah 2021A7PS2428P
//Vedang Bhupesh Shenvi Nadkarni 2020B5A70897P
//Shai Pranesh S 2020B2A70731P
//Krteyu Pillai 2021A7PS2522P
//Aryan Seth 2021A7PS2221P

#ifndef LEXER_H
#define LEXER_H

#include "lexerDef.h"

extern FILE *getStream(FILE *fp);      //re-populates the next of twin buffers with the next BUFFERSIZE amount of bytes
extern tokenInfo getNextToken(twinBuffer B);
extern void removeComments(char *testcaseFile, char *cleanFile);

extern void initializeTwinBuffer();     //initialize the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
extern void freeTwinBuffer();       //free the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
extern void initializeSymbolTable();                   
extern void freeSymbolTable();                         //similar to initializeBuffer and freeBuffer use-cases
extern tokenInfo gettoken();
extern tokenInfo createTokenInfo(Vocabulary v, char *lexeme, int lineNumber);

#endif