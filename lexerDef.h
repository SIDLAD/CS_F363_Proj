#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define VOCABULARY_COUNT ((int)TOTAL_VOCAB_SIZE)
#define TERMINALS_COUNT ((int)TOTAL_TERMINALS_COUNT)
#define N_TERMINALS_COUNT ((int)TOTAL_VOCAB_SIZE-TERMINALS_COUNT - 1)
#define VOCAB_STRLEN_CAP 35
#define MAX_VARIABLE_IDENTIFIER_LENGTH 20
#define MAX_FUNCTION_IDENTIFIER_LENGTH 30
#define MAX_FILENAME_LENGTH 50
#define MAX_LEXEME_LENGTH MAX_FUNCTION_IDENTIFIER_LENGTH        //as MAX_FUNCTION_IDENTIFIER_LENGTH is greater than MAX_VARIABLE_IDENTIFIER_LENGTH
#define ALPHABET_COUNT 85


typedef struct tokenInfo* tokenInfo;
typedef struct twinBuffer* twinBuffer;
typedef enum vocabulary Vocabulary;

// int state = 0; // state number for the DFA
// int currentLineNumber = 1; // current line number being read
// twinBuffer buffer = NULL;   //buffer to be initialized via initializeBuffer(buffer) function call
// Trie symbolTable = NULL;

// char testcaseFile[MAX_FILENAME_LENGTH] = "testcase.txt";
// char parseTreeOutFile[MAX_FILENAME_LENGTH] = "createParseOutFile.txt";
// char commentFreeFile[MAX_FILENAME_LENGTH] = "commentFreeFile.txt";
// char printTokenListFile[MAX_FILENAME_LENGTH] = "printTokenListFile.txt";



typedef struct trie *Trie;
typedef struct trieEdge *TrieEdge;

struct trieEdge
{
    Trie ptr;
    char ch;
};

struct trie
{
    TrieEdge *children;
    int childCount;
    void *data;
};

extern int state;// = 0; // state number for the DFA
extern int currentLineNumber;// = 1; // current line number being read
extern twinBuffer buffer;// = NULL;   //buffer to be initialized via initializeBuffer(buffer) function call
extern Trie symbolTable;// = NULL;

extern char testcaseFile[MAX_FILENAME_LENGTH];// = "testcase.txt";
extern char parseTreeOutFile[MAX_FILENAME_LENGTH];// = "createParseOutFile.txt";
extern char commentFreeFile[MAX_FILENAME_LENGTH];// = "commentFreeFile.txt";
extern char printTokenListFile[MAX_FILENAME_LENGTH];// = "printTokenListFile.txt";

extern FILE** fptrs;
extern int fptrsLen;

struct twinBuffer
{
    FILE* fp;
    char first[BUFFER_SIZE];
    char second[BUFFER_SIZE];
    int lexemeBegin;
    int forward;
    int fileEndsAtBufferIndex;
};

enum vocabulary{
    TK_COMMENT,
    TK_THEN,
    TK_ASSIGNOP,
    TK_INT,
    TK_AS,
    TK_INPUT,
    TK_OR,
    TK_ENDRECORD,
    TK_MINUS,
    TK_EQ,
    TK_PARAMETER,
    TK_TYPE,
    TK_WHILE,
    TK_GLOBAL,
    TK_SEM,
    TK_RUID,
    TK_OP,
    TK_ENDUNION,
    TK_COMMA,
    TK_MUL,
    TK_RETURN,
    TK_MAIN,
    TK_IF,
    TK_DOT,
    TK_FIELDID,
    TK_SQL,
    TK_WRITE,
    TK_DIV,
    TK_PLUS,
    TK_NUM,
    TK_GT,
    TK_DEFINETYPE,
    TK_OUTPUT,
    TK_REAL,
    TK_PARAMETERS,
    TK_WITH,
    TK_GE,
    TK_LT,
    TK_READ,
    TK_AND,
    TK_RECORD,
    TK_END,
    TK_CALL,
    TK_ENDWHILE,
    TK_ELSE,
    TK_SQR,
    TK_NOT,
    TK_ENDIF,
    TK_COLON,
    EPS,
    TK_ID,
    TK_LE,
    TK_NE,
    TK_FUNID,
    TK_UNION,
    TK_LIST,
    TK_RNUM,
    TK_CL,
    TOTAL_TERMINALS_COUNT,
    dataType,
    fieldDefinition,
    function,
    more_ids,
    output_par,
    typeDefinitions,
    stmts,
    constructedDatatype,
    definetypestmt,
    elsePart,
    relationalOp,
    booleanExpression,
    returnStmt,
    var,
    moreExpansions,
    singleOrRecId,
    fieldDefinitions,
    actualOrRedefined,
    parameter_list,
    oneExpansion,
    funCallStmt,
    fieldType,
    otherFunctions,
    otherStmts,
    lowPrecedenceOperators,
    inputParameters,
    highPrecedenceOperators,
    declaration,
    moreFields,
    typeDefinition,
    ioStmt,
    stmt,
    termPrime,
    input_par,
    A,
    assignmentStmt,
    option_single_constructed,
    optionalReturn,
    idList,
    global_or_not,
    recId,
    mainFunction,
    remaining_list,
    iterativeStmt,
    term,
    logicalOp,
    declarations,
    arithmeticExpression,
    expPrime,
    primitiveDatatype,
    outputParameters,
    factor,
    program,
    conditionalStmt,
    TOTAL_VOCAB_SIZE
};

struct tokenInfo
{
    Vocabulary tokenName;
    char lexeme[MAX_LEXEME_LENGTH];
    int lineNumber;
};

#endif