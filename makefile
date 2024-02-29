# Compiler to use
CC = gcc

# Compiler flags
CFLAGS =

# Build target executable
TARGET = stage1exe

# Object files
OBJS = lexer.o parser.o driver.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

lexer.o: lexerDef.h lexer.h lexer.c
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parserDef.h parser.h parser.c
	$(CC) $(CFLAGS) -c parser.c

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

clean:
	rm -f $(OBJS)
