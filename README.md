netid: by240, elp95

# Content

[Testing](#testing)

[Functions](#functions)

[Planning](#planning)

# Testing


# Functions

[read_inputs()](#read_inputs)

[count_char()](#count_char)

## read_inputs()

read_inputs will take 5 arguments
* The amount of arguments to main (`int argc`)
* The arguments to main (`char *argv[]`)
* The suffix (initialized to `.txt` but could be changed in the function) (`char *suffix`)
* The location where the array of files will be located by the function (`char **files`)
* The location where the array of directories will be located by the function (`char **directories`)

read_inputs will return an `int`, which will be 0 unless an error occured.




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