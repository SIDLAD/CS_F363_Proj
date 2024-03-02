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
	rm -f printTokenListFile.txt

clearCache:
	rm -f *Cache
	rm -f *.o
	rm -f stage1exe
	rm -f printTokenListFile.txt