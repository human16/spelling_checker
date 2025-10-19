netid: by240, elp95

# Testing


# Planning

## Reading files/directories

To read the file names, the directory to the dictionary and the suffix of files, we will first make a few assumptions

* There's only one or no occurance of `-s`
* `-s` must occur as the first argument
* without `-s`, `spell` will assume the suffix `.txt`
* If `-s` occurs, the next argument will be the suffix, following the format: `.???` for whatever the suffix is for scanning files
* The next argument after that must be the name of the dictionary file
* The rest of the argument will be either files or directories
* Files will not have the suffix included in the argument
* Directories will be serated with `/`