CC = gcc
CFLAGS = -g -Wall -Wvla -std=c99 -fsanitize=address,undefined

default: checker.o parser.o scanner.o
	$(CC) $(CFLAGS) $^ -o spell

%.o: %.c
	$(CC) $(CFLAGS) -c $<

checker.o: parser.h
parser.o: scanner.h
scanner.o: 

clean:
	rm -f *.o spell