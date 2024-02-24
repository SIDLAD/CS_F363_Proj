driver: driver.o lexer.o
	gcc driver.o -o driver

driver.o: driver.c
	gcc -c driver.c

clean:
	rm -f driver.o driver
