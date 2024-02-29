#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 256
#define VOCABULARY_COUNT ((int)TOTAL_VOCAB_SIZE)
#define TERMINALS_COUNT ((int)TOTAL_TERMINALS_COUNT)
#define N_TERMINALS_COUNT ((int)TOTAL_VOCAB_SIZE-TERMINALS_COUNT - 1)
#define VOCAB_STRLEN_CAP 35
#define MAX_VARIABLE_IDENTIFIER_LENGTH 20
#define MAX_FUNCTION_IDENTIFIER_LENGTH 30
#define MAX_FILENAME_LENGTH 50
#define MAX_LEXEME_LENGTH MAX_FUNCTION_IDENTIFIER_LENGTH        //as MAX_FUNCTION_IDENTIFIER_LENGTH is greater than MAX_VARIABLE_IDENTIFIER_LENGTH

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
#include <stdlib.h>

#define ALPHABET_COUNT 85

typedef struct trie *Trie;
typedef struct trieEdge *TrieEdge;

typedef struct linkedList* LinkedList;

struct linkedList
{
    LinkedList next;
    void* data;
};

#include <stdlib.h>

#define ALPHABET_COUNT 85

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
    TK_ENDUNION,
    EPS,
    TK_COMMA,
    TK_READ,
    TK_REAL,
    TK_DOT,
    TK_GE,
    TK_NE,
    TK_PLUS,
    TK_DEFINETYPE,
    TK_ASSIGNOP,
    TK_AS,
    TK_CL,
    TK_DIV,
    TK_GT,
    TK_PARAMETERS,
    TK_UNION,
    TK_RUID,
    TK_SQR,
    TK_FIELDID,
    TK_MUL,
    TK_OR,
    TK_END,
    TK_LIST,
    TK_WRITE,
    TK_TYPE,
    TK_SEM,
    TK_WITH,
    TK_AND,
    TK_IF,
    TK_LT,
    TK_MINUS,
    TK_EQ,
    TK_ELSE,
    TK_PARAMETER,
    TK_NUM,
    TK_ENDIF,
    TK_WHILE,
    TK_FUNID,
    TK_OP,
    TK_COLON,
    TK_ID,
    TK_ENDRECORD,
    TK_LE,
    TK_NOT,
    TK_RECORD,
    TK_THEN,
    TK_RNUM,
    TK_SQL,
    TK_OUTPUT,
    TK_INT,
    TK_CALL,
    TK_INPUT,
    TK_RETURN,
    TK_ENDWHILE,
    TK_GLOBAL,
    TK_MAIN,
    TOTAL_TERMINALS_COUNT,
    iterativeStmt,
    global_or_not,
    relationalOp,
    A,
    stmts,
    input_par,
    recId,
    declarations,
    arithmeticExpression2Contd,
    otherFunctions,
    definetypestmt,
    var,
    ioStmt,
    more_ids,
    assignmentStmt,
    function,
    returnStmt,
    typeDefinitions,
    otherStmts,
    declaration,
    arithmeticExpressionContd,
    operator2,
    funCallStmt,
    parameter_list,
    primitiveDatatype,
    booleanExpression,
    arithmeticExpression2,
    singleOrRecId,
    output_par,
    arithmeticExpression,
    mainFunction,
    outputParameters,
    arithmeticExpression3,
    optionalReturn,
    operator1,
    conditionalStmt,
    stmt,
    dataType,
    program,
    remaining_list,
    conditionalStmtContd,
    logicalOp,
    fieldDefinitions,
    fieldDefinition,
    typeDefinition,
    inputParameters,
    constructedDatatype,
    moreFields,
    idList,
    TOTAL_VOCAB_SIZE
};

struct tokenInfo
{
    Vocabulary tokenName;
    char lexeme[MAX_LEXEME_LENGTH];
    int lineNumber;
};

#endif