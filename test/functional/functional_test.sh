#!/bin/bash

exit_on_error()
{
    arg1=$1

    echo "Failed to execute with error "$arg1". Exiting..."
    exit $arg1
}

# Compile numconvert
{
    gcc -Wall -O3 ../../src/main.c -o numconvert
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Compile the functional test program
{
    gcc -Wall -D_BSD_SOURCE -O3 main.c -o functional
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Perform a number of tests decimal tests
./functional "1" "./numconvert" "0" "0" "tests/1.txt"                       #lower decimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "2" "./numconvert" "18446744073709551615" "0" "tests/2.txt"    #upper decimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "3" "./numconvert" "-1" "255" "tests/3.txt"                    #invalid lower decimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "4" "./numconvert" "18446744073709551616" "255" "tests/4.txt"  #invalid upper decimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

echo "Script completed."

exit 0
