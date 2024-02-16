//depending on command line arguments, main will call the corresponding function

/*
Create a menu as per the following instructions
0 : For exit

1 : For removal of comments ‐ print the comment free code on the console

2 : For printing the token list (on the console) generated by the lexer. This option performs lexical analysis and prints all tokens and lexemes line number wise. Here, the tokens are not passed to the parser, but printed on the console only. Each token appears in a new line along with the corresponding lexeme and line number. (invoke only lexer) [Ensure pretty printing with column justifications to increase readability]

3 : For parsing to verify the syntactic correctness of the input source code and printing the parse tree appropriately. This option prints all errors - lexical and syntactic, line number wise, on the console and prints parse tree in the file as mentioned in the command line below. (Invoke both lexer and parser).

4: For printing (on the console) the total time taken by your project code of lexer and parser to verify the syntactic correctness. Use <time.h> file as follows
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lexer.h"
#include "parser.h"

void printMenu()
{
    printf("Enter your choice: ");
    printf("0 : Exit\n");
    printf("1 : Remove comments and create a comment-free code file\n");
    printf("2 : Print the token list (on the console) generated by the lexer\n");
    printf("3 : Parse to verify the syntactic correctness of the input source code, print the parse tree and all errors\n");
    printf("4 : Print the total time taken by your project code of lexer and parser to verify the syntactic correctness\n");
}

void case_cleanComments()
{
    char testcaseFile[50], cleanFile[50];
    printf("Enter the name of the file to remove comments from: ");
    scanf("%s", testcaseFile);
    printf("Enter the name of the file to write the comment free code to: ");
    scanf("%s", cleanFile);
    removeComments(testcaseFile, cleanFile);
}

void case_printTokenList()
{
    char testcaseFile[50];
    printf("Enter the name of the file to lex: ");
    scanf("%s", testcaseFile);
    // TODO: SIDLAD
    tokenInfo tokenInfo_;
    while(tokenInfo_ = getNextToken(buffer))
    {
        printf("%s %s %d\n", tokenInfo_->tokenName, tokenInfo_->lexeme, tokenInfo_->lineNumber);
    }
}

void case_generateParseTree()
{
    char testcaseFile[50], outFile[50];
    printf("Enter the name of the file to parse: ");
    scanf("%s", testcaseFile);
    printf("Enter the name of the file to write the parse tree to: ");
    scanf("%s", outFile);
    FirstAndFollow *firstAndFollow = (FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    table *T = (table *)malloc(sizeof(table));
    createParseTable(firstAndFollow, T);
    parseInputSourceCode(testcaseFile, T);
    //TODO: SIDLAD - deal with creating the parseTree. REPLACE THE BELOW CREATED pt.
    parseTree pt;
    printParseTree(pt, outFile);
}

void case_calculateTime()
{
    char testcaseFile[50];
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();
    FirstAndFollow *firstAndFollow = (FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    table *T = (table *)malloc(sizeof(table));
    createParseTable(firstAndFollow, T);
    parseInputSourceCode(testcaseFile, T);
    end_time = clock();
    total_CPU_time = (double) (end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
    printf("Total time taken by the project code of lexer and parser to verify the syntactic correctness: %f\n", total_CPU_time_in_seconds);
}

#ifndef MAIN_FILE
#define MAIN_FILE

int main()
{
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