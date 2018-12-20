#!/bin/sh
# Prerequisites
# Install valgrind (f.e. for Ubuntu use "sudo apt-get install valgrind")
# Install kcachegrind (f.e. for Ubuntu use "sudo apt-get install kcachegrind")

exit_on_error()
{
    arg1=$1

    echo "Failed to execute with error "$arg1". Exiting..."
    exit $arg1
}

# Store the first argument into NUMBER because that is what we use to profile numconvert
NUMBER="$1"

# Remove old profiling data
{
    rm callgrind.out*
} &> /dev/null

# Compile the program with profiling options
{
    gcc -Wall -O0 -g ../src/main.c -o numconvert_prof
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Execute the program with the given argument number
# Ignore return codes, we might want to profile with invalid input
{
    valgrind --tool=callgrind ./numconvert_prof $NUMBER
} &> /dev/null

# Show the profiling result
{
     kcachegrind
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

echo "Script completed."

exit 0
