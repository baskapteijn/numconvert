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


# Perform a number of decimal input tests
./functional "1" "./numconvert" "0" "0" "tests/1.txt"                       #lower decimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "2" "./numconvert" "18446744073709551615" "0" "tests/2.txt"    #upper decimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "3" "./numconvert" "-1" "255" "tests/3.txt"                    #invalid lower decimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "4" "./numconvert" "18446744073709551616" "255" "tests/4.txt"  #invalid upper decimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi


# Perform a number of binary input tests
./functional "5" "./numconvert" "0b" "0" "tests/5.txt"                       #lower binary boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "6" "./numconvert" "1111111111111111111111111111111111111111111111111111111111111111b" "0" "tests/6.txt"       #upper binary boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "7" "./numconvert" "11111111111111111111111111111111111111111111111111111111111111111b" "255" "tests/7.txt"    #invalid upper binary boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi


# Perform a number of hexadecimal input tests
./functional "8" "./numconvert" "0x0" "0" "tests/8.txt"                     #lower hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "9" "./numconvert" "0h" "0" "tests/9.txt"                      #lower hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "10" "./numconvert" "0xffffffffffffffff" "0" "tests/10.txt"    #upper hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "11" "./numconvert" "ffffffffffffffffh" "0" "tests/11.txt"     #upper hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "12" "./numconvert" "0x1ffffffffffffffff" "255" "tests/12.txt"   #invalid upper hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

./functional "13" "./numconvert" "1ffffffffffffffffh" "255" "tests/13.txt"    #invalidupper hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi


echo "Script completed."

exit 0
