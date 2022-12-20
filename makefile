CC=gcc 
CFLAGS=-Wall -std=c99 -pedantic

q1: q1.c
	$(CC) $(CFLAGS) q1.c -o q1
clean:
	rm *.o q1.exe