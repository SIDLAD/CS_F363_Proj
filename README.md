# CS_F363_Proj
Compiler Construction

- For ease of working and debugging, currently files are importing lexer.c and parser.c instead of the corresponding .h files.

- Run: ```gcc driver.c && ./a.out``` to see the working and output

- TODO:
- - Write code for creating First.txt and Follow.txt files. Note that you need to use the function firstOfNT to get the first of Non-Terminal. Also, note that while printing the follow of a non-terminal, in the terminalBucketSet corresponding to the followset of that terminal, the position of EPS is replaced with ENDOFFILE, as EPS cannot be part of any followset