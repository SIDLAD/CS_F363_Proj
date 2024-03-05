//Group Number 31
//Siddharth Shah F2021A7PS2428P
//Vedang Bhupesh Shenvi Nadkarni F2020B5A70897P
//Shai Pranesh S F2020B2A70731P
//Krteyu Pillai F2021A7PS2522P
//Aryan Seth F2021A7PS2221P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"

void printMenu()
{
    printf("\nEnter your choice:\n");
    printf("0 : Exit\n");
    printf("1 : Remove comments and create a comment-free code file\n");
    printf("2 : Print the token list (and creates file) generated by the lexer on the console\n");
    printf("3 : Parse to verify the syntactic correctness of the input source code, print all errors, and print the parse tree if there is no syntax error\n");
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
    fptrsLen = 2;
    fptrs = calloc(fptrsLen,sizeof(FILE*));

    fptrs[0]=fopen(printTokenListFile,"w");
    fptrs[1]=fopen(listOfErrorsFile,"w");

    char token[MAX_LEXEME_LENGTH];
    while(_tokenInfo = getNextToken(buffer))
    { 
        enumToStr(_tokenInfo->tokenName,token);
        printf("Line no. %d\t Lexeme %s\t Token %s\n", _tokenInfo->lineNumber, _tokenInfo->lexeme, token);
        fprintf(fptrs[0],"Line no. %d\t Lexeme %s\t Token %s\n", _tokenInfo->lineNumber, _tokenInfo->lexeme, token);
        free(_tokenInfo);
    }

    for(int i=0;i<fptrsLen;i++)fclose(fptrs[i]);
    free(fptrs);
    fptrsLen = 0;

    freeTwinBuffer();
    freeSymbolTable();
}

void case_generateParseTree()
{
    fptrsLen = 1;
    fptrs = calloc(fptrsLen,sizeof(FILE*));
    fptrs[0] = fopen(listOfErrorsFile,"w");
    //lexer and parser will be invoked
    _table = createParseTable(_firstAndFollow, _table);
    bool noSyntaxErrors = parseInputSourceCode(testcaseFile, _table);
    //lexer and parser have been invoked
    if(!noSyntaxErrors){
        printf("Syntax errors in program. Parse tree generated but not printed on");
    }
    else{
        printf("Parse tree generated, checkout output file path");
    }
    if(noSyntaxErrors)printParseTree(_parseTree, parseTreeOutFile);

    freeParseTree();  //freeing the parse tree is a must, otherwise there will be memory leaks
    freeSymbolTable();          //if the backend had to be built, then the symbol table would persist beyond this point

    for(int i=0;i<fptrsLen;i++)fclose(fptrs[i]);
    free(fptrs);
    fptrsLen = 0;
}

void case_calculateTime()
{
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();

    //lexer and parser will be invoked
    _table = createParseTable(_firstAndFollow, _table);
    parseInputSourceCode(testcaseFile, _table);
    // //lexer and parser have been invoked

    end_time = clock();
    total_CPU_time = (double) (end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
    printf("Total time taken by the project code of lexer and parser to verify the syntactic correctness: %f seconds\n", total_CPU_time_in_seconds);
    
    freeParseTree();
    freeSymbolTable();
}

#ifndef MAIN_FILE
#define MAIN_FILE

int main(int argc, char* argv[])
{
    // strcpy(testcaseFile,"Test Cases/t2.txt");
    
    strcpy(testcaseFile,argv[1]);
    strcpy(parseTreeOutFile,argv[2]);
    while(1)
    {
        printMenu();
        
        char choice[10];
        scanf("%s", choice);
        printf("FIRST and FOLLOW sets have been automated\n");
        printf("Both Lexical and Syntax Analysis Modules implemented\n");
        printf("Our compiler works with all provided testcases\n");
        

        switch(choice[0])
        {
            case '0':
                return 0;
            case '1':
                case_cleanComments();
                break;
            case '2':
                case_printTokenList();
                break;
            case '3':
                case_generateParseTree();
                break;
            case '4':
                case_calculateTime();
                break;
            default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}

#endif