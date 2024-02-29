# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -g

# Build target executable
TARGET = program

# Object files
OBJS = lexer.o parser.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

lexer.o: lexerDef.h lexer.h lexer.c
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parserDef.h parser.h parser.c
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm $(OBJS)
