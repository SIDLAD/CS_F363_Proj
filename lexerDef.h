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
// twinBuffer buffer = NULL;   //buffer to be initialised via initialiseBuffer(buffer) function call
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
extern twinBuffer buffer;// = NULL;   //buffer to be initialised via initialiseBuffer(buffer) function call
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
    TK_NUM,
    TK_GT,
    TK_END,
    TK_AND,
    TK_RECORD,
    TK_DIV,
    TK_INPUT,
    TK_WITH,
    TK_ID,
    TK_IF,
    TK_CALL,
    TK_ASSIGNOP,
    TK_EQ,
    TK_ENDWHILE,
    TK_PARAMETERS,
    TK_ELSE,
    TK_FUNID,
    TK_SQL,
    TK_LE,
    TK_ENDRECORD,
    TK_AS,
    TK_TYPE,
    TK_GE,
    TK_COLON,
    TK_MAIN,
    TK_PLUS,
    TK_DEFINETYPE,
    TK_THEN,
    TK_NE,
    TK_ENDUNION,
    TK_RUID,
    TK_OR,
    TK_SEM,
    TK_OUTPUT,
    TK_WRITE,
    TK_GLOBAL,
    TK_DOT,
    TK_LT,
    TK_MUL,
    TK_COMMA,
    TK_INT,
    TK_CL,
    TK_LIST,
    TK_ENDIF,
    TK_RETURN,
    TK_MINUS,
    TK_REAL,
    TK_PARAMETER,
    TK_SQR,
    TK_FIELDID,
    TK_RNUM,
    TK_OP,
    EPS,
    TK_WHILE,
    TK_NOT,
    TK_UNION,
    TK_READ,
    TOTAL_TERMINALS_COUNT,
    assignmentStmt,
    recId,
    fieldDefinition,
    highPrecedenceOperators,
    primitiveDatatype,
    A,
    typeDefinitions,
    ioStmt,
    input_par,
    fieldDefinitions,
    moreFields,
    idList,
    factor,
    optionalReturn,
    otherStmts,
    mainFunction,
    global_or_not,
    constructedDatatype,
    actualOrRedefined,
    oneExpansion,
    relationalOp,
    constructedVariable,
    remaining_list,
    moreExpansions,
    termPrime,
    declarations,
    expPrime,
    fieldType,
    more_ids,
    parameter_list,
    iterativeStmt,
    arithmeticExpression,
    otherFunctions,
    output_par,
    outputParameters,
    lowPrecedenceOperators,
    dataType,
    declaration,
    stmt,
    program,
    conditionalStmt,
    function,
    singleOrRecId,
    logicalOp,
    elsePart,
    option_single_constructed,
    definetypestmt,
    returnStmt,
    inputParameters,
    var,
    term,
    highPrecedenceOperator,
    booleanExpression,
    funCallStmt,
    fieldtype,
    typeDefinition,
    stmts,
    TOTAL_VOCAB_SIZE
};

struct tokenInfo
{
    Vocabulary tokenName;
    char lexeme[MAX_LEXEME_LENGTH];
    int lineNumber;
};

#endif