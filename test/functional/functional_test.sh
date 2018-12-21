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


# For the functional tests we store any error in EXIT_FAILURE and return it at the end
# This is required for the Travis CI server to detect a failed test, test execution or script failure


# Perform a number of decimal input tests
./functional "1" "./numconvert" "0" "0" "stdout/1.txt"                          #lower decimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "2" "./numconvert" "18446744073709551615" "0" "stdout/2.txt"       #upper decimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "3" "./numconvert" "-1" "255" "stdout/3.txt"                       #invalid lower decimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "4" "./numconvert" "18446744073709551616" "255" "stdout/4.txt"     #invalid upper decimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


# Perform a number of binary input tests
./functional "5" "./numconvert" "0b" "0" "stdout/5.txt"                         #lower binary boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "6" "./numconvert" "1111111111111111111111111111111111111111111111111111111111111111b" "0" "stdout/6.txt"       #upper binary boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "7" "./numconvert" "11111111111111111111111111111111111111111111111111111111111111111b" "255" "stdout/7.txt"    #invalid upper binary boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


# Perform a number of hexadecimal input tests
./functional "8" "./numconvert" "0x0" "0" "stdout/8.txt"                        #lower hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "9" "./numconvert" "0h" "0" "stdout/9.txt"                         #lower hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "10" "./numconvert" "0xffffffffffffffff" "0" "stdout/10.txt"       #upper hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "11" "./numconvert" "ffffffffffffffffh" "0" "stdout/11.txt"        #upper hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "12" "./numconvert" "0x1ffffffffffffffff" "255" "stdout/12.txt"    #invalid upper hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "13" "./numconvert" "1ffffffffffffffffh" "255" "stdout/13.txt"     #invalidupper hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


echo "Script completed."

exit $EXIT_FAILURE
