#!/bin/bash

exit_on_error()
{
    arg1=$1

    echo "Failed to execute with error "$arg1". Exiting..."
    exit $arg1
}

# Compile numconvert
{
    gcc -O3 ../../src/main.c -o numconvert
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Compile the functional test program
{
    gcc -D_BSD_SOURCE -O3 main.c -o functional
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "./numconvert" "0x1ff2" "0" "1"
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

echo "Script completed."

exit 0
