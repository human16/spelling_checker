netid: by240, elp95

# Content

[Testing](#testing)

[Files](#files)

[Planning](#planning)

# Testing


# Files

[checker.c](#checker)

[parser.c](#parserc)

[scanner.c](#scannerc)

[trie.c](#triec)

## checker.c

checker.c will take in all the files and directories that need to be opened and checked as provided by parser.c

### functions

[check_file()](#check_file)

[check_directory()](#check_directory)

[check_input()](#check_inpur)

#### check_file()

`check_file()` will open the provided file and traverse all words in a given file and compare them to the dictionary with the function `_____`

#### check_directory()

`check_directory()` will recursively call itself for ever directory inside of it, and call `check_file()` for every file in it that has `suffix` as its file extension

#### check_input()

will be almost equivalent to [open_file](#check_file), except is uses user input instead of a file

## parser.c

parser.c will go over the given arguments, check if a suffix exists, and provide the dictionary file name to the `build function`, and then call the appropriate `scanner.c` function for each argument.

## trie.c

//insert documentation

# Planning

## Reading files/directories

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
* user input will be read similarly to `scan_file` implementation 