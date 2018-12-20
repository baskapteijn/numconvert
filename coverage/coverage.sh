#!/bin/sh
# Prerequisites
# Install lcov (f.e. for Ubuntu use "sudo apt-get install lcov")

exit_on_error()
{
    arg1=$1

    echo "Failed to execute with error "$arg1". Exiting..."
    exit $arg1
}

# Compile the program with coverage options
{
    gcc -Wall -fprofile-arcs -ftest-coverage ../src/main.c
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Execute the program and try to cover as much as possible
# Ignore return codes, this is not a component test but purely for coverage
{
    ./a.out 1111111111111111111111111111111111111111111111111111111111111111b
    ./a.out 111111111111111111111111111111111111111111111111111111111111111b
    ./a.out 18446744073709551615
    ./a.out 0xffffffffffffffff
    ./a.out 0xfffffffffffffffff
    ./a.out 0x0
    ./a.out b
    ./a.out h
    ./a.out 111h
    ./a.out 111111111111111111111111111111111111111111111111111111111111111111
    ./a.out 112b
    ./a.out Aah
    ./a.out 18446744073709551616
    ./a.out 0xg
    ./a.out
    ./a.out ""
    ./a.out 0b
    ./a.out -1
    ./a.out 0x/
} &> /dev/null

# Output the coverage result
{
    gcov main.c
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Create gcov files
{
    gcov -abcfu main.c
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Generate lcov data info
{
    lcov --rc lcov_branch_coverage=1 --directory . --capture --output-file coverage.info
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Generate html representation
genhtml --branch-coverage coverage.info
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

echo "Script completed. See index.html for results."

exit 0
