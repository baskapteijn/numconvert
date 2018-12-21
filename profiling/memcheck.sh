#!/bin/bash
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

# Compile the program with profiling options
{
    gcc -Wall -O0 -g ../src/main.c -o numconvert_prof
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Perform a memory check on the program 
# Errorcode 42 overrides the numconvert returncode if any leak is detected
valgrind --leak-check=yes --error-exitcode=42 ./numconvert_prof $NUMBER
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

echo "Script completed."

exit 0
