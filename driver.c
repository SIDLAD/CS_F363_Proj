#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"

void printMenu()
{
    printf("Enter your choice:\n");
    printf("0 : Exit\n");
    printf("1 : Remove comments and create a comment-free code file\n");
    printf("2 : Print the token list (and creates file) generated by the lexer on the console\n");
    printf("3 : Parse to verify the syntactic correctness of the input source code, print the parse tree and all errors\n");
    printf("4 : Print the total time taken by your project code of lexer and parser to verify the syntactic correctness\n");
}

void case_cleanComments()
{
    removeComments(testcaseFile, commentFreeFile);
}

void case_printTokenList()
{
    initializeSymbolTable();
    initializeTwinBuffer();
    tokenInfo _tokenInfo;
    fptrsLen = 1;
    fptrs = calloc(fptrsLen,sizeof(FILE*));

    fptrs[0]=fopen(printTokenListFile,"w");

    char token[100];
    while(_tokenInfo = getNextToken(buffer))
    { 
        enumToStr(_tokenInfo->tokenName,token);
        printf("Line no. %d\t Lexeme %s\t Token %s\n", _tokenInfo->lineNumber, _tokenInfo->lexeme, token);
        fprintf(fptrs[0],"Line no. %d\t Lexeme %s\t Token %s\n", _tokenInfo->lineNumber, _tokenInfo->lexeme, token);
    }

    for(int i=0;i<fptrsLen;i++)fclose(fptrs[i]);
    free(fptrs);

    freeTwinBuffer();
    freeSymbolTable();
}

void case_generateParseTree()
{
    //lexer and parser will be invoked
    _table = createParseTable(_firstAndFollow, _table);
    parseInputSourceCode(testcaseFile, _table);
    //lexer and parser have been invoked
    
    printParseTree(_parseTree, parseTreeOutFile);
    freeParseTree(_parseTree);  //freeing the parse tree is a must, otherwise there will be memory leaks
}

void case_calculateTime()
{
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();

    //lexer and parser will be invoked
    createParseTable(_firstAndFollow, _table);
    // parseInputSourceCode(testcaseFile, _table);
    // //lexer and parser have been invoked

    // end_time = clock();
    // total_CPU_time = (double) (end_time - start_time);
    // total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
    // printf("Total time taken by the project code of lexer and parser to verify the syntactic correctness: %f\n", total_CPU_time_in_seconds);
    
    // freeParseTree(_parseTree);
}

#ifndef MAIN_FILE
#define MAIN_FILE

int main(int argc, char* argv[])
{
    strcpy(testcaseFile,argv[1]);
    // strcpy(parseTreeOutFile,argv[2]);
    while(1)
    {
        printMenu();
        
        int choice;
        scanf("%d", &choice);

        switch(choice)
        {
            case 0:
                return 0;
            case 1:
                case_cleanComments();
                break;
            case 2:
                case_printTokenList();
                break;
            case 3:
                case_generateParseTree();
                break;
            case 4:
                case_calculateTime();
                break;
            default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}

#endif