#include <stdio.h>
#include "lexerDef.h"

FILE *getStream(FILE *fp);      //re-populates the next of twin buffers with the next BUFFERSIZE amount of bytes
tokenInfo getNextToken(twinBuffer B);
void removeComments(char *testcaseFile, char *cleanFile);

void initialiseTwinBuffer();     //initialise the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
void freeTwinBuffer();       //free the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
void initialiseSymbolTable();                   
void freeSymbolTable();                         //similar to initialiseBuffer and freeBuffer use-cases
tokenInfo gettoken();