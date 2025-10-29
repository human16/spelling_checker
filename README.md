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

//insert documentation

## parser.c

parser.c will go over the 

## scanner.c

//insert documentation

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