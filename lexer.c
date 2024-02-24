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
void lookupSymbolTable(char* lexeme);
//------------------//

// function definitions

void initialiseTwinBuffer() // initialise the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
{
    buffer = (twinBuffer)malloc(sizeof(struct twinBuffer));
    buffer->forward = buffer->lexemeBegin = 0;
    buffer->fp = getStream(NULL);
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

void lookupSymbolTable(char* lexeme)
{
    getDataFromTrie(lexeme,symbolTable);
}

FILE *getStream(FILE *fp)
{
    if (fp == NULL)
    {
        fp = fopen(testcaseFile, "r");
    }
    if (!feof(fp))
    {
        if (buffer->forward == 0)
            fread(buffer->first, BUFFER_SIZE, 1, fp);
        else if (buffer->forward == BUFFER_SIZE)
            fread(buffer->second, BUFFER_SIZE, 1, fp);
    }
    return fp;
}

char characterReadFromBuffer(int ptr, twinBuffer _buffer)
{
    if (ptr < 256)
        return _buffer->first[ptr];

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
        tmp++;
    } while (_buffer->lexemeBegin != _buffer->forward);

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
            if(c=='\n') state=15;
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
            return createTokenInfo(TK_LE,tmp_lexeme,currentlineNumber);

            case 3:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='-') state=4;
            else state = failure(); // TODO: Must implement double retract
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
            return createTokenInfo(TK_ASSIGNOP,tmp_lexeme,currentlineNumber);
            
            case 6:
            retract(buffer);
            state=0;
            strcpy(tmp_lexeme,"<");
            return createTokenInfo(TK_LT,tmp_lexeme,currentlineNumber);

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
            return createTokenInfo(TK_GE,tmp_lexeme,currentlineNumber);

            case 9:
            retract(buffer);
            state=0;
            strcpy(tmp_lexeme,">");
            return createTokenInfo(TK_GT,tmp_lexeme,currentlineNumber);

            case 10:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"+");
            return createTokenInfo(TK_PLUS,tmp_lexeme,currentlineNumber);

            case 11:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"-");
            return createTokenInfo(TK_MINUS,tmp_lexeme,currentlineNumber);

            case 12:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"*");
            return createTokenInfo(TK_MUL,tmp_lexeme,currentlineNumber);

            case 13:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"*");
            return createTokenInfo(TK_DIV,tmp_lexeme,currentlineNumber);

            case 14:
            resetBufferPtrs(buffer); 
            state=0;
            strcpy(tmp_lexeme,"~");
            return createTokenInfo(TK_NOT,tmp_lexeme,currentlineNumber);

            case 15:
            currentlineNumber++;
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='\n') state=15;
            else if(c==' ') state=16;
            else state=17;
            break;

            case 16:
            incrementBufferForward(buffer);
            c=characterReadFromBuffer(buffer->forward,buffer);
            if(c=='\n') state=15;
            else if(c==' ') state=16;
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
            return createTokenInfo(TK_AND,tmp_lexeme,currentlineNumber);

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
            return createTokenInfo(TK_OR,tmp_lexeme,currentlineNumber);

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
            return createTokenInfo(TK_EQ,tmp_lexeme,currentlineNumber);

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
            return createTokenInfo(TK_NE,tmp_lexeme,currentlineNumber);
            
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
    }
    fclose(file);
}

void removeComments(char *testcaseFile, char *cleanFile){
    char** lines = readNremove_comments(testcaseFile);
    writeCommentFreeFile(lines, cleanFile);
}
#ifndef MAIN_FILE
#define MAIN_FILE 

int main() {
    cleanComments("t2.txt");
    return 0;
}

#endif
