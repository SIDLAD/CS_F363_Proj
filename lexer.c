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