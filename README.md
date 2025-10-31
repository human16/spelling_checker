netid: by240, elp95

# Content

[Testing](#testing)

[Files](#files)

[Planning](#planning)

# Testing

Most function in every file are equipped with a `DEBUG` variable, which can be changed in compuiling with `make debug` instead of the default `make`

Along those

# Files

[checker.c](#checker)

[parser.c](#parserc)

[scanner.c](#scannerc)

[trie.c](#triec)

[searcher.c](#searcherc)

## checker.c

`checker.c` will take in all the files and directories that need to be opened and checked as provided by `parser.c`

### functions

[check_file()](#check_file)

[check_directory()](#check_directory)

[check_input()](#check_inpur)

#### check_file()

`check_file()` will open the provided file and traverse all words in a given file and compare them to the dictionary with the function `searcher.c`

#### check_directory()

`check_directory()` will recursively call itself for ever directory inside of it, and call `check_file()` for every file in it that has `suffix` as its file extension

#### check_input()

will be almost equivalent to [open_file](#check_file), except is uses user input instead of a file

## parser.c

parser.c will go over the given arguments, check if a suffix exists, and provide the dictionary file name to the `build function`, and then call the appropriate `scanner.c` function for each argument.

## scanner.c

will look at every file or directory and sort them correctly

### functions

[scan_file()](#scan_file)

[scan_directory()](#scan_directory)

[scan_input()](#scan_input)

#### scan_file()

will take in a file and read it with an initial buffer of size 16 bytes and then pass it to the searcher.

#### scan_directory()

will recursively scan a directory and pass every file with extension `suffix` to `scan_file()`

#### scan_input()

same functionality as `scan_file()` but with `stdin`

## trie.c

trie creates a trie tree to be able to check if a word is included in a dictionary fast

### functions

[make_trienode()](#make_trienode)

[free_trienode()](#free_trienode)

[insert_trie()](#insert_trienode)

[serach_trie()](#search_trie)

#### make_trienode()

creates a trienode with `ALPHABET_SIZE` number of child trienodes

#### free_trienode()

frees a trienode and all of its children

#### insert_trienode()

inserts a trienode into the trienode tree

#### search_trie()

searches the trie tree for a specific given word

## searcher.c 

# Planning

the file path

`checker.c` -> `parser.c` -> `scanner.c` -> `searcher.c`\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |\
&nbsp;&nbsp;&nbsp;&nbsp;  V&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/\\ \
 `trie.c` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_______________________________________J \
 `read_dict.c`


To read the file names, the directory to the dictionary and the suffix of files, we will first make a few assumptions

* There's only one or no occurance of `-s`
* `-s` must occur as the first argument
* without `-s`, `spell` will assume the suffix `.txt`
* If `-s` occurs, the next argument will be the suffix, following the format: `.???` for whatever the suffix is for scanning files
* The next argument after that must be the name of the dictionary file, the rest of the argument will be either files or directories
* Directories will be serated with `/`
* If files have file extensions, the default suffix will not be added to them

Each argument will then be forwarded to the appropriate `scanner.c` function based on it's type (file, dictionary, dictionary, or user input)

in `scanner.c`:
* every file will be opened, traversed, and each word will be compared to the dictionary. 
* each directory will be opened, and in it, each file with the extensions `suffix` will be sent to `scan_file()`, and each directory will be recursively opened.
* user input will be read similarly to `scan_file()` implementation 

Then, every word in a file will be sent to `seracher.c`, which will be able to strip each word of access symbols at the edges, normalize the capitalization and compare it and then compare the words to the trie tree and flag any wrong words