#!/bin/bash
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
    gcc -Wall -fprofile-arcs -ftest-coverage ../src/main.c -o numconvert_cov
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Execute the program and try to cover as much as possible
# Ignore return codes, this is not a component test but purely for coverage
{
    ./numconvert_cov 1111111111111111111111111111111111111111111111111111111111111111b
    ./numconvert_cov 111111111111111111111111111111111111111111111111111111111111111b
    ./numconvert_cov 18446744073709551615
    ./numconvert_cov 0xffffffffffffffff
    ./numconvert_cov 0xfffffffffffffffff
    ./numconvert_cov 0x0
    ./numconvert_cov b
    ./numconvert_cov h
    ./numconvert_cov 111h
    ./numconvert_cov 111111111111111111111111111111111111111111111111111111111111111111
    ./numconvert_cov 112b
    ./numconvert_cov Aah
    ./numconvert_cov 18446744073709551616
    ./numconvert_cov 0xg
    ./numconvert_cov
    ./numconvert_cov ""
    ./numconvert_cov 0b
    ./numconvert_cov -1
    ./numconvert_cov 0x/
    ./numconvert_cov 0z
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
{
    genhtml --branch-coverage coverage.info
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

{
    sensible-browser index.html &
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

echo "Script completed."

exit 0
