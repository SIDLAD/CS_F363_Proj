#include <stdio.h>
#include <string.h>
#include "lexerDef.h"

// function prototypes//
twinBuffer initialiseBuffer(twinBuffer _buffer);     //initialise the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
twinBuffer freeBuffer(twinBuffer _buffer);       //free the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
void initialiseSymbolTable();
void freeSymbolTable();
FILE *getStream(FILE *fp);
void incrementBufferForward(twinBuffer _buffer);
void retract(twinBuffer _buffer);
void doubleRetract(twinBuffer _buffer);
char* retractAndReturnLexeme(char* Dest,twinBuffer _buffer);
//------------------//

// function definitions

twinBuffer initialiseBuffer(twinBuffer _buffer)     //initialise the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
{
    _buffer = (twinBuffer)malloc(sizeof(struct twinBuffer));
    _buffer->forward = _buffer->lexemeBegin = 0;
    _buffer->fp = getStream(NULL);
    return _buffer;
}

twinBuffer freeBuffer(twinBuffer _buffer)       //free the buffer before starting the next iteration, in case_printTokenList() of driver.c and createParseTree()
{
    fclose(_buffer->fp);
    free(_buffer);

    return NULL;
}

void initialiseSymbolTable()
{
    symbolTable = createTrieNode();
    //char arr[][] = {"while","if","endif"}; use conversion-type logic
    //for(){

    //}
}

void freeSymbolTable()
{
    freeTrieNodeRecursive(symbolTable);
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
    if(ptr < 256)
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

char* retractAndReturnLexeme(char* Dest,twinBuffer _buffer)     //Dest character array should have enough space for storing the returned lexeme
{
    char* tmp = Dest;
    do
    {
        *tmp = characterReadFromBuffer(_buffer->lexemeBegin,_buffer);
        tmp ++ ;
    }while(_buffer->lexemeBegin!=_buffer->forward);

    return Dest;
}

void resetBufferPtrs(twinBuffer _buffer)
{
    incrementBufferForward(_buffer);
    _buffer->lexemeBegin = _buffer->forward;
}