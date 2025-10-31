CC = gcc
CFLAGS = -g -Wall -Wvla -std=c99 -fsanitize=address,undefined
REGULAR_OBJS = checker_reg.o parser_reg.o scanner_reg.o searcher_reg.o trie_reg.o read_dict_reg.o

debug: checker.o parser.o scanner.o searcher.o
	$(CC) $(CFLAGS) $^ -o spell_debug

regular: $(REGULAR_OBJS)
	$(CC) $(CFLAGS) $^ -o spell

%_reg.o: %.c
	$(CC) $(CFLAGS) -DDEBUG=0 -c $< -o $@

trie_reg.o: trie/trie.c trie/trie.h
	$(CC) $(CFLAGS) -DDEBUG=0 -c $< -o $@

checker_reg.o: checker.c parser.h
parser_reg.o: parser.c scanner.h
scanner_reg.o: scanner.c searcher/searcher.h
read_dict_reg.o: read_dict.c read_dict.h trie/trie.h
searcher_reg.o: searcher/searcher.c searcher/searcher.h
	$(CC) $(CFLAGS) -DDEBUG=0 -c searcher/searcher.c -o searcher_reg.o


searcher.o: searcher/searcher.h
	$(CC) $(CFLAGS) -DDEBUG=0 -c searcher/searcher.c -o searcher.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<


checker.o: parser.h
parser.o: scanner.h read_dict.h
scanner.o: searcher/searcher.h
read_dict.o: trie/trie.h
trie/trie.o:


clean:
	rm -f *.o spell spell_debug
