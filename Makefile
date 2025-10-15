CC = gcc
CFLAGS = -g -Wall -Wvla -std=c99 -fsanitize=address,undefined

default: checker.o
	$(CC) $(CFLAGS) $^ -o spell

%.o: %.c
	$(CC) $(CFLAGS) -c $<

checker.o:

clean:
	rm -f *.o spell