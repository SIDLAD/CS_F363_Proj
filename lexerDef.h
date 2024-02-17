#define BUFFER_SIZE 256
#define VOCABULARY_COUNT 107
#define TERMINALS_COUNT 57
#define N_TERMINALS_COUNT (VOCABULARY_COUNT-TERMINALS_COUNT)
#define MAX_LEXEME_LENGTH 20
#define MAX_FILENAME_LENGTH 20

typedef struct tokenInfo* tokenInfo;
typedef struct twinBuffer* twinBuffer;
typedef enum vocabulary Vocabulary;

int currentlineNumber = 0;
twinBuffer buffer = NULL;   //buffer to be initialised via initialiseBuffer(buffer) function call

struct tokenInfo
{
    char tokenName[100];
    char lexeme[20];
    int lineNumber;
};

struct twinBuffer
{
    char first[BUFFER_SIZE];
    char second[BUFFER_SIZE];
};

enum vocabulary{
    TK_ENDUNION,
    TK_EPS,
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
    idsList,
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
    idList
};