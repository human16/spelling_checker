CC = gcc
CFLAGS = -g -Wall -Wvla -std=c99 -fsanitize=address,undefined
REGULAR_OBJS = checker_reg.o parser_reg.o scanner_reg.o searcher_reg.o trie_reg.o

debug: checker.o parser.o scanner.o searcher.o
	$(CC) $(CFLAGS) $^ -o spell_debug

regular: $(REGULAR_OBJS)
	$(CC) $(CFLAGS) $^ -o spell

%_reg.o: %.c
	$(CC) $(CFLAGS) -DDEBUG=0 -c $< -o $@

trie_reg.o: trie/trie.c trie/trie.h
	$(CC) $(CFLAGS) -DDEBUG=0 -c $< -o $@

# Regular object dependencies
checker_reg.o: checker.c parser.h
parser_reg.o: parser.c scanner.h
scanner_reg.o: scanner.c searcher.h
searcher_reg.o: searcher.c searcher.h


%.o: %.c
	$(CC) $(CFLAGS) -c $<


checker.o: parser.h
parser.o: scanner.h
scanner.o: searcher.h
searcher.o: 
trie.o:

clean:
	rm -f *.o spell spell_debug