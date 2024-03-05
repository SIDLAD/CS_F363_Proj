# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -g

# Build target executable
TARGET = stage1exe

# Object files
OBJS = lexer.o parser.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) driver.c $(OBJS) -o $(TARGET)

lexer.o: lexerDef.h lexer.h lexer.c
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parserDef.h parser.h parser.c
	$(CC) $(CFLAGS) -c parser.c

# driver.o: driver.c
# 	$(CC) $(CFLAGS) -c driver.c

clean:
	rm -f *.o
	rm -f stage1exe

# make clearCache performs the functionalities of make clean, along with deleting the predictiveParsingTableCache
# Note that any change in the compiler-codes necessitates running "make clearCache" on the terminal.
clearCache:
	rm -f *Cache
	rm -f *.o
	rm -f stage1exe