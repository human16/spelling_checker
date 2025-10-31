CC = gcc
CFLAGS = -g -Wall -Wvla -std=c99 -fsanitize=address,undefined
DEBUG_OBJS = checker.o parser.o scanner.o searcher.o read_dict.o trie/trie.o
REGULAR_OBJS = checker_reg.o parser_reg.o scanner_reg.o searcher_reg.o read_dict_reg.o trie_reg.o

debug: $(DEBUG_OBJS)
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

#files that are in directories have special rules
searcher_reg.o: searcher/searcher.c searcher/searcher.h
	$(CC) $(CFLAGS) -DDEBUG=0 -c searcher/searcher.c -o searcher_reg.o

searcher.o: searcher/searcher.c searcher/searcher.h
	$(CC) $(CFLAGS) -c $< -o $@

trie/trie.o: trie/trie.c trie/trie.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

checker.o: parser.h
parser.o: scanner.h read_dict.h searcher/searcher.h
scanner.o: searcher/searcher.h
searcher.o: searcher/searcher.h trie/trie.h
read_dict.o: read_dict.h trie/trie.h


clean:
	rm -f *.o spell spell_debug
	rm -f trie/*.o 
	rm -f searcher/*.o