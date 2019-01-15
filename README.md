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
./ssfi -t 4 test/test-2-big
```

# Errors

The program will print some error text and return a non-zero error code if any of the following are true:

1. No path was given.
2. The given path does not exist.
3. No value was provided for -t
4. The value given to -t is not a number

Extraneous options will be silently ignored.

Example command to demonstrate these errors:

```
[afelsher@localhost ssfi]$ ./ssfi ; ./ssfi -t ; ./ssfi -t a ; ./ssfi badpath
Error: No path provided
Error: No value provided for "-t"
Error: Error parsing value for option "-t": "a". Value must be a number.
Error: Path does not exist: "badpath"
```

# Output

The program will print up to 10 of the most common words in the given file path. Each will be printed on a separate line, followed by the number of occurrences. If the provided path contains no words (for example, it points to an empty file), no output will be printed, but this is not considered an error.

Example 1:

```
[afelsher@localhost ssfi]$ ./ssfi -t 4 test/test-2-big
the 310791
and 167838
of 161726
to 132119
a 114973
I 105301
in 91662
that 74262
was 55715
it 55143
```

Example 2:

```
[afelsher@localhost ssfi]$ ./ssfi -t 4 test/test-1-small
word8 6
0word5 4
word1 4
word2 4
0word4 2
word3 2
word6 2
word7 2
word9 2
```

# Testing

FYI, here's an example command I used for testing:

`date ; ./ssfi -t 4 test/test-2-big ; date ; echo "Exit Code: $?"`

I've included some test data and a test script in the `test` directory.

The `test-1-small` directory contains two small files useful for manual verification of the ouptut and for confirming that the character set for "word"s is defined correctly.

The `test-2-big` directory contains numerous copies of dictionary files, some small text files containing snippets from books, several complete copies of a few public domain novels, and a few non-'.txt' files, sub-directories, and a JRE tarball for good measure.

The test script (`ssfi.bash`) performs the same function as the C++ code, but using Bash commands. On my system, for the `test/test-2-big` directory, it takes around 18 seconds to complete (compared to around 1-2 seconds for the native code running with 4 worker threads). Example:

`date ; ./ssfi.bash test-2-big/ ; date`

<b>NOTE:</b> The order is undefined for top 10 words which have the same number of occurrences. Therefore, output for the ssfi.bash script may differ from the native code if any of the top 10 words have the same number of occurrences. This occurs for the `test-1-small` dataset, but not the `test-2-big` dataset.

<b>NOTE:</b> Running against `test-2-big` in debug mode will produce a <b>LOT</b> of log output (including one log message each time a word is found in a file).

