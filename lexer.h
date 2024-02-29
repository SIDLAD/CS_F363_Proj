#ifndef LEXER_H
#define LEXER_H

#include "lexerDef.h"

extern FILE *getStream(FILE *fp);      //re-populates the next of twin buffers with the next BUFFERSIZE amount of bytes
extern tokenInfo getNextToken(twinBuffer B);
extern void removeComments(char *testcaseFile, char *cleanFile);

extern void initialiseTwinBuffer();     //initialise the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
extern void freeTwinBuffer();       //free the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
extern void initialiseSymbolTable();                   
extern void freeSymbolTable();                         //similar to initialiseBuffer and freeBuffer use-cases
extern tokenInfo gettoken();

#endif