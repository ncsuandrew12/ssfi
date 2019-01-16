# Building

Building is simple:

```
make clean
make
```

To enable debug compiler flags and log messages, run make like so:

```
make BUILD_MODE=debug
```

# Running

To run, execute a command like this:

```
./ssfi [-t <int>] <some-path>
```

For example:

```
./ssfi -t 4 mypath
```

# Errors

The program will print some error text and return a non-zero error code if any of the following are true:

1. No path was given.
2. The given path does not exist.
3. No value was provided for -t
4. The value given to -t is not a number
5. There is an IO error with the given path or its contents.

Extraneous options will be silently ignored.

Example command to demonstrate the more likely errors:

```
[afelsher@localhost ssfi]$ ./ssfi ; ./ssfi -t ; ./ssfi -t a ; ./ssfi badpath
no path provided
no value provided for '-t'
invalid value for option '-t' ('a'). Value must be a number >= 1.
cannot access badpath: No such file or directory
```

# Output

The program will print up to 10 of the most common words in the given file path. Each will be printed on a separate line, followed by the number of occurrences. If the provided path contains no words (for example, it points to an empty file), no output will be printed, but this is not considered an error.

Example 1:

```
[afelsher@localhost ssfi]$ ./ssfi -t 4 test/test-1-small
word8   6
word2   5
0word5  4
word1   4
0word4  2
word3   2
word6   2
word7   2
word9   2
```

Example 2:

```
[afelsher@localhost ssfi]$ ./ssfi -t 4 test/test-2-big
the     332634
and     177910
of      163383
to      134443
a       119486
i       105483
in      96710
that    77187
it      66586
his     57423
```

