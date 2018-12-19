#!/bin/sh
# Prerequisites
# Install lcov (f.e. for Ubuntu use "sudo apt-get install lcov")

# Compile the program with coverage options
gcc -Wall -fprofile-arcs -ftest-coverage ../src/main.c

# Execute the program and try to cover as much as possible
./a.out 1111111111111111111111111111111111111111111111111111111111111111b
./a.out 111111111111111111111111111111111111111111111111111111111111111b
./a.out 18446744073709551615
./a.out 0xffffffffffffffff
./a.out 0xfffffffffffffffff
./a.out 0x0
./a.out b
./a.out h
./a.out 111h
./a.out 111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
./a.out 112b
./a.out Aah
./a.out 18446744073709551616
./a.out 0xg
./a.out
./a.out ""

# Output the coverage result
gcov main.c

# Create gcov files
gcov -abcfu main.c

# Generate lcov data info
lcov --rc lcov_branch_coverage=1 --directory . --capture --output-file coverage.info

# Generate html representation
genhtml --branch-coverage coverage.info

