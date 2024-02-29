#include <stdio.h>
#include <string.h>
#include "lexerDef.h"

// function prototypes//
void initialiseTwinBuffer(); // initialise the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
void freeTwinBuffer();       // free the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
void initialiseSymbolTable();
void freeSymbolTable();
FILE *getStream(FILE *fp);
void incrementBufferForward(twinBuffer _buffer);
void retract(twinBuffer _buffer);
void doubleRetract(twinBuffer _buffer);
char *retractAndReturnLexeme(char *Dest, twinBuffer _buffer);
tokenInfo createTokenInfo(Vocabulary v, char *lexeme, int lineNumber);
char* resetBufferPtrsAndReturnLexeme(char* Dest, twinBuffer _buffer);
void insertIntoSymbolTable(tokenInfo tkinf,char* lexeme);
void* lookupSymbolTable(char* lexeme);
//------------------//

// function definitions

void initialiseTwinBuffer() // initialise the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
{
    buffer = (twinBuffer)malloc(sizeof(struct twinBuffer));
    buffer->forward = buffer->lexemeBegin = 0;
    buffer->fp = getStream(NULL);
    buffer->fileEndsAtBufferIndex = __INT_MAX__;
    return;
}

void freeTwinBuffer() // free the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
{
    fclose(buffer->fp);
    free(buffer);
    buffer = NULL;
    return;
}

tokenInfo createTokenInfo(Vocabulary v, char *lexeme, int lineNumber)
{
    tokenInfo tkinf = (tokenInfo)malloc(sizeof(struct tokenInfo));
    tkinf->tokenName = v;
    strcpy(tkinf->lexeme,lexeme);
    tkinf->lineNumber = lineNumber;
}

void initialiseSymbolTable()
{
    symbolTable = createTrieNode();
    const static struct
    {
        char *str;
        Vocabulary v;
    } initialEntries[] = {
        {"with", TK_WITH},
        {"parameters", TK_PARAMETERS},
        {"end", TK_END},
        {"while", TK_WHILE},
        {"union", TK_UNION},
        {"endunion", TK_ENDUNION},
        {"definetype", TK_DEFINETYPE},
        {"as", TK_AS},
        {"type", TK_TYPE},
        {"_main", TK_MAIN},
        {"global", TK_GLOBAL},
        {"parameter", TK_PARAMETER},
        {"list", TK_LIST},
        {"input", TK_INPUT},
        {"output", TK_OUTPUT},
        {"int", TK_INT},
        {"real", TK_REAL},
        {"endwhile", TK_ENDWHILE},
        {"if", TK_IF},
        {"then", TK_THEN},
        {"endif", TK_ENDIF},
        {"read", TK_READ},
        {"write", TK_WRITE},
        {"return", TK_RETURN},
        {"call", TK_CALL},
        {"record", TK_RECORD},
        {"endrecord", TK_ENDRECORD},
        {"else", TK_ELSE},
    };
    for (int i = 0; i < sizeof(initialEntries) / sizeof(initialEntries[0]); i++)
    {
        insertIntoTrie(createTokenInfo(initialEntries[i].v,initialEntries[i].str,-1),initialEntries[i].str,symbolTable);
    }
}

void freeSymbolTable()
{
    freeTrieNodeRecursive(symbolTable);
    symbolTable = NULL;
}

void insertIntoSymbolTable(tokenInfo tkinf,char* lexeme)
{
    insertIntoTrie((void*)tkinf,lexeme,symbolTable);
}

void* lookupSymbolTable(char* lexeme)
{
    return getDataFromTrie(lexeme,symbolTable);
}

FILE *getStream(FILE *fp)
{
    if (fp == NULL)
    {
        fp = fopen(testcaseFile, "r");
    }
    if (!feof(fp))
    {
        int tmp;
        if (buffer->forward == 0)
        {
            if((tmp = fread(buffer->first, sizeof(char), BUFFER_SIZE, fp))<BUFFER_SIZE){
                buffer->fileEndsAtBufferIndex = tmp - 1;
            }
        }
        else if (buffer->forward == BUFFER_SIZE)
        {
            if((tmp = fread(buffer->second, sizeof(char), BUFFER_SIZE, fp))<BUFFER_SIZE){
                buffer->fileEndsAtBufferIndex = tmp - 1 + BUFFER_SIZE;
            }
        }
    }
    return fp;
}

char characterReadFromBuffer(int ptr, twinBuffer _buffer)
{
    if(ptr >= _buffer->fileEndsAtBufferIndex)
        return '\0';
    if (ptr < 256)
    {
        return _buffer->first[ptr];
    }

    return _buffer->second[ptr - 256];
}

void incrementBufferForward(twinBuffer _buffer)
{
    _buffer->forward++;
    _buffer->forward %= 2 * BUFFER_SIZE;

    if (_buffer->forward % BUFFER_SIZE == 0)
        _buffer->fp = getStream(_buffer->fp);
}

void retract(twinBuffer _buffer)
{
    _buffer->lexemeBegin = _buffer->forward;
}

void doubleRetract(twinBuffer _buffer)
{
    _buffer->lexemeBegin = _buffer->forward = (_buffer->forward + 2 * BUFFER_SIZE - 1) % (2 * BUFFER_SIZE);
}

char *retractAndReturnLexeme(char *Dest, twinBuffer _buffer) // Dest character array should have enough space for storing the returned lexeme
{
    char *tmp = Dest;
    do
    {
        *tmp = characterReadFromBuffer(_buffer->lexemeBegin, _buffer);
        _buffer->lexemeBegin++;
        _buffer->lexemeBegin %= 2 * BUFFER_SIZE;
        
        tmp++;
    } while (_buffer->lexemeBegin != _buffer->forward);

    *tmp = '\0';
    return Dest;
}

void resetBufferPtrs(twinBuffer _buffer)
{
    incrementBufferForward(_buffer);
    _buffer->lexemeBegin = _buffer->forward;
}

char* resetBufferPtrsAndReturnLexeme(char* Dest, twinBuffer _buffer)
{
    incrementBufferForward(_buffer);
    return retractAndReturnLexeme(Dest,_buffer);
}

int failure(){
    // currently acting as a place holder until we create error.h and error.c files
    
    return 0;
}

tokenInfo getNextToken(twinBuffer B)
{
    char c;
    char tmp_lexeme[MAX_LEXEME_LENGTH]; 
    tokenInfo tmp;
    while(1){
        switch(state){
            case 0:
            c=characterReadFromBuffer(buffer->forward,buffer); 
            if(c == '\0') return NULL;
            else if(c=='\n') state=15;
            else if(c==' ') state=16;
            else if(c=='<') state=1;
            else if(c=='>') state=7;
            else if(c=='+') state=10;
            else if(c=='-') state=11;
            else if(c=='*') state=12;
            else if(c=='/') state=13;
            else if(c=='~') state=14;
            else if(c=='&') state=18;
            else if(c=='@') state=21;
            else if(c=='=') state=24;
            else if(c=='!') state=26;
            else if(c>='0' && c<='9') state=28;
            else if(c=='(') state=37;
            else if(c==')') state=38;
            else if(c=='[') state=39;
            else if(c==']') state=40;
            else if(c=='.') state=41;
            else if(c==':') state=42;
            else if(c==',') state=43;
            else if(c=='_') state=44;
            else if(c==';') state=48;
            else if(c=='%') state=49;
            else if(c=='#') state=51;
            else if(c=='a'||(c>='e'&&c<='z')) state=54;
            else if(c>='b'&&c<='d') state=56;
            

            // else if 
            else state = failure();
            break;

            case 1:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer); 
            if(c=='=') state=2; 
            else if(c=='-') state=3;
            else state=6;
            break;

            case 2:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,"<=");
            return createTokenInfo(TK_LE,tmp_lexeme,currentLineNumber);

            case 3:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='-') state=4;
            else {doubleRetract(buffer);state = 0;} // TODO: Must implement double retract
            break;

            case 4:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='-') state=5;
            else state = failure();
            break;

            case 5:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"<---");
            return createTokenInfo(TK_ASSIGNOP,tmp_lexeme,currentLineNumber);
            
            case 6:
            retract(buffer);
            state=0;
            strcpy(tmp_lexeme,"<");
            return createTokenInfo(TK_LT,tmp_lexeme,currentLineNumber);

            case 7:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='=') state=8;
            else state=9;
            break;

            case 8:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,">=");
            return createTokenInfo(TK_GE,tmp_lexeme,currentLineNumber);

            case 9:
            retract(buffer);
            state=0;
            strcpy(tmp_lexeme,">");
            return createTokenInfo(TK_GT,tmp_lexeme,currentLineNumber);

            case 10:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"+");
            return createTokenInfo(TK_PLUS,tmp_lexeme,currentLineNumber);

            case 11:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"-");
            return createTokenInfo(TK_MINUS,tmp_lexeme,currentLineNumber);

            case 12:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"*");
            return createTokenInfo(TK_MUL,tmp_lexeme,currentLineNumber);

            case 13:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"*");
            return createTokenInfo(TK_DIV,tmp_lexeme,currentLineNumber);

            case 14:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"~");
            return createTokenInfo(TK_NOT,tmp_lexeme,currentLineNumber);

            case 15:
            currentLineNumber++;
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='\n') state=15;
            else if(c==' '||c=='\t') state=16;
            else state=17;
            break;

            case 16:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='\n') state=15;
            else if(c==' '||c=='\t') state=16;
            else state=17;
            break;

            case 17:
            retract(buffer);
            state=0;
            break;

            case 18:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='&') state=19;
            else state=failure();
            break;

            case 19:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='&') state=20;
            else state=failure();
            break;

            case 20:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,"&&&");
            return createTokenInfo(TK_AND,tmp_lexeme,currentLineNumber);

            case 21:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='@') state=22;
            else state=failure();
            break;

            case 22:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='@') state=23;
            else state=failure();
            break;

            case 23:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,"@@@");
            return createTokenInfo(TK_OR,tmp_lexeme,currentLineNumber);

            case 24:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='=') state=25;
            else state=failure();
            break;

            case 25:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,"==");
            return createTokenInfo(TK_EQ,tmp_lexeme,currentLineNumber);

            case 26:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='=') state=27;
            else state=failure();
            break;

            case 27:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,"!=");
            return createTokenInfo(TK_NE,tmp_lexeme,currentLineNumber);

            case 28:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='0' && c<='9') state=28;
            else if(c=='.') state=29;
            else state=36;
            break;

            case 29:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='0' && c<='9') state=30;
            else state=failure();
            break;

            case 30:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='0' && c<='9') state=31;
            else state=failure();
            break;

            case 31:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='E') state=32;
            else state=35;
            break;

            case 32:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='+'||c=='-') state=33;
            else if(c>='0' && c<='9') state=34;
            else state=failure();
            break;

            case 33:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='0' && c<='9') state=34;
            else state=failure();
            break;

            case 34:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='0' && c<='9') {incrementBufferForward(buffer);state=35;}
            else state=failure();
            break;
            
            case 35:
            retractAndReturnLexeme(tmp_lexeme, buffer);
            state=0;
            return createTokenInfo(TK_RNUM,tmp_lexeme,currentLineNumber);

            case 36:
            retractAndReturnLexeme(tmp_lexeme, buffer);
            state=0;
            return createTokenInfo(TK_NUM,tmp_lexeme,currentLineNumber);

            case 37:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,"(");
            return createTokenInfo(TK_OP,tmp_lexeme,currentLineNumber);

            case 38:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,")");
            return createTokenInfo(TK_OP,tmp_lexeme,currentLineNumber);

            case 39:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,"[");
            return createTokenInfo(TK_SQL,tmp_lexeme,currentLineNumber);

            case 40:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,"]");
            return createTokenInfo(TK_SQR,tmp_lexeme,currentLineNumber);

            case 41:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,".");
            return createTokenInfo(TK_DOT,tmp_lexeme,currentLineNumber);

            case 42:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,":");
            return createTokenInfo(TK_COLON,tmp_lexeme,currentLineNumber);

            case 43:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,",");
            return createTokenInfo(TK_COMMA,tmp_lexeme,currentLineNumber);

            case 44:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if((c>='A'&&c<='Z')||(c>='a'&&c<='z')) state=45;
            else state=failure();
            break;

            case 45:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if((c>='A'&&c<='Z')||(c>='a'&&c<='z')) state=45;
            else if(c>='0' && c<='9') state=46;
            else state=47;
            break;

            case 46:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='0' && c<='9') state=46;
            else state=47;
            break;

            case 47:
            retractAndReturnLexeme(tmp_lexeme,buffer);
            state=0;
            if(tmp = lookupSymbolTable(tmp_lexeme))
                return createTokenInfo(tmp->tokenName,tmp->lexeme,currentLineNumber);
            else
                return createTokenInfo(TK_FUNID, tmp_lexeme, currentLineNumber);

            case 48:
            resetBufferPtrs(buffer);
            state=0;
            strcpy(tmp_lexeme,";");
            return createTokenInfo(TK_SEM,tmp_lexeme,currentLineNumber);
            
            case 49:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='\n') state=50;
            else state=49;
            break;

            case 50:
            resetBufferPtrs(buffer);
            state=0;
            return createTokenInfo(TK_COMMENT,"%",currentLineNumber++);

            case 51:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='a'&&c<='z') state=52;
            else state=failure();
            break;

            case 52:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='a'&&c<='z') state=52;
            else state=53;
            break;

            case 53:
            retractAndReturnLexeme(tmp_lexeme,buffer);
            state=0;
            return createTokenInfo(TK_RUID,tmp_lexeme,currentLineNumber);

            case 54:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='a'&&c<='z') state=54;
            else state=55;
            break;

            case 55:
            retractAndReturnLexeme(tmp_lexeme,buffer);
            state=0;
            if(tmp = lookupSymbolTable(tmp_lexeme))
                return createTokenInfo(tmp->tokenName,tmp->lexeme,currentLineNumber);
            else
                return createTokenInfo(TK_FIELDID, tmp_lexeme, currentLineNumber);

            case 56:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='a'&&c<='z') state=54;
            else if(c>='2' && c<='7') state =57;
            else state=failure();
            break;

            case 57:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>=98&&c<=100) state=57;
            else if(c>='2' && c<='7') state =58;
            else state=59;
            break;

            case 58:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c>='2' && c<='7') state=58;
            else state=59;
            break;

            case 59:
            retractAndReturnLexeme(tmp_lexeme,buffer);
            state=0;
            if(tmp = lookupSymbolTable(tmp_lexeme))
                return createTokenInfo(tmp->tokenName,tmp->lexeme,currentLineNumber);
            else
                return createTokenInfo(TK_ID, tmp_lexeme, currentLineNumber);

        }
    }
} 

char** remove_comments(FILE* file) {
    char** lines = NULL;
    char* line = NULL;
    size_t len = 0;
    int read;
    int i = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        char* comment = strchr(line, '%');
        if (comment != NULL) {
            *comment = '\n';
            *(comment + 1)= '\0';
        }
        lines = realloc(lines, (i + 1) * sizeof(char*));
        lines[i] = line;
        line = NULL;
        i++;
    }
    lines = realloc(lines, (i + 1) * sizeof(char*));
    lines[i] = NULL;
    return lines;
}

char **readNremove_comments(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: file not found\n");
        exit(1);
    }
    char** lines = remove_comments(file);
    fclose(file);
    return lines;
}

void writeCommentFreeFile(char** lines, char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: file could not be created\n");
        exit(1);
    }
    for (int i = 0; lines[i] != NULL; i++) {
        fprintf(file, "%s", lines[i]);
        printf("%s", lines[i]);
    }
    fclose(file);
}

void removeComments(char *testcaseFile, char *cleanFile){
    char** lines = readNremove_comments(testcaseFile);
    writeCommentFreeFile(lines, cleanFile);
}