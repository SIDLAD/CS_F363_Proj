#include <stdio.h>
#include "lexerDef.h"

FILE *getStream(FILE *fp);      //re-populates the next of twin buffers with the next BUFFERSIZE amount of bytes
tokenInfo getNextToken();
void removeComments(char *testcaseFile, char *cleanFile);