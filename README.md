
# Fakemake

This project was the final programming assignment for my data structures class. It makes a directed acyclic graph from the data file specified as the first argument. If a cycle is detected, the program reports a detected loop and aborts. If no cycle is detected, the user can use the commands specified below to update 'files' and their dependencies, just as the make command does. All functions have runtime requirements that were either met or exceeded in this implementation. More specific details about the project can be found in the handout.pdf.

## Running the Program

### Compilation

To compile the program, simply type:
```
make fakemake
```
and an executable will be created called 'fakemake'. A couple of test files are included in the repository and must be passed in as the first command line argument when running the executable. File 'test3' has a cycle.

### Test File Structure

The general structure of a test file must be as follows:

```
<file1>
<file2>
...
<fileN>
<target1> : <dep1> <dep2> ...
<target2> : <dep1> <dep2> ...
...
<targetK> : <dep1> <dep2> ...
```

where fileN refers to a "basic" file and targetK refers to a composite target file. Note that there is white space between the target file and the subsequent colon, as well as the colon and the first dependency. This is necessary formatting for the program to run correctly.

### User Commands

Note that after the program checks for cycles and duplicate file names, it initializes all timestamps on 'files' to be 0.

* time: This command reports the current calue of the clock.
* touch <filename>: This command changes the time stamp on the specified filename to the current clock valuie and increments the clock. This applies to only "basic" files.
* timestamp <filename>: This prints the time stamp on the specified file. This is the last time the "basic" or target file was "modified" with the touch command.
* timestamps: Prints the timestamps of all files.
* make <target>: This command takes the target and brings it up to date. "Basic" files are always up to date. This is essentially the make command.
* quit: Exits the program.

## Author

* **Kyler Boyle**

## Acknowledgments

* **John Lillis**, University of Illinois at Chicago, hashmap implementation.




Copyright 2017 Kyler Boyle. All Rights Reserved.
