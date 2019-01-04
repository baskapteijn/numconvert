#!/bin/bash

exit_on_error()
{
    arg1=$1

    echo "Failed to execute with error "$arg1". Exiting..."
    exit $arg1
}

# Compile numconvert
{
	cmake -D CMAKE_BUILD_TYPE=Release ../..
	make
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi

# Compile the functional test program
{
    gcc -Wall -D_BSD_SOURCE -O3 -s main.c -o functional
} &> /dev/null
rc=$?; if [[ $rc != 0 ]]; then exit_on_error $rc; fi


# For the functional tests we store any error in EXIT_FAILURE and return it at the end
# This is required for the Travis CI server to detect a failed test, test execution or script failure


# Perform a number of decimal boundary tests
./functional "1" "./numconvert" "0" "0" "stdout/1.txt"                          #lower decimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "2" "./numconvert" "18446744073709551615" "0" "stdout/2.txt"       #upper decimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "3" "./numconvert" "-1" "255" "stdout/3.txt"                       #invalid lower decimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "4" "./numconvert" "18446744073709551616" "255" "stdout/4.txt"     #invalid upper decimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


# Perform a number of binary boundary tests
./functional "5" "./numconvert" "0b" "0" "stdout/5.txt"                         #lower binary boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "6" "./numconvert" "1111111111111111111111111111111111111111111111111111111111111111b" "0" "stdout/6.txt"       #upper binary boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "7" "./numconvert" "11111111111111111111111111111111111111111111111111111111111111111b" "255" "stdout/7.txt"    #invalid upper binary boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


# Perform a number of hexadecimal boundary tests
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

./functional "13" "./numconvert" "1ffffffffffffffffh" "255" "stdout/13.txt"     #invalid upper hexadecimal boundary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


# Perform a number of ECP tests
./functional "14" "./numconvert" "9223372036854775808" "0" "stdout/14.txt"      #decimal ECP mid
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "15" "./numconvert" "1000000000000000000000000000000000000000000000000000000000000000b" "0" "stdout/15.txt"     #binary ECP mid
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "16" "./numconvert" "0x8000000000000000" "0" "stdout/16.txt"       #hexadecimal ECP mid
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


# Perform a number of tests with upper to lower case conversion

./functional "17" "./numconvert" "0xaBcDe" "0" "stdout/17.txt"                  #prefixed hexadecimal caps
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "18" "./numconvert" "0XAbCdE" "0" "stdout/18.txt"                  #prefixed caps hexadecimal caps
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "19" "./numconvert" "aBcDeh" "0" "stdout/19.txt"                   #postfixed hexadecimal caps
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "20" "./numconvert" "AbCdEH" "0" "stdout/20.txt"                   #postfixed caps hexadecimal caps
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "21" "./numconvert" "1001B" "0" "stdout/21.txt"                    #postfixed caps binary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


# Perform 0 input tests

./functional "22" "./numconvert" "0" "0" "stdout/22.txt"                        #decimal 0
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "23" "./numconvert" "0x0" "0" "stdout/23.txt"                      #prefixed hexadecimal 0
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "24" "./numconvert" "0h" "0" "stdout/24.txt"                       #postfixed hexadecimal 0
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "25" "./numconvert" "0b" "0" "stdout/25.txt"                       #postfixed binary 0
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


# Perform invalid input tests

./functional "26" "./numconvert" "3/" "255" "stdout/26.txt"                     #invalid decimal
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "27" "./numconvert" "0x3/" "255" "stdout/27.txt"                   #invalid hexadecimal
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "28" "./numconvert" "1/b" "255" "stdout/28.txt"                    #invalid binary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "29" "./numconvert" "b" "255" "stdout/29.txt"                      #invalid binary
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "30" "./numconvert" "0x" "255" "stdout/30.txt"                     #invalid hexadecimal
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "31" "./numconvert" "h" "255" "stdout/31.txt"                      #invalid hexadecimal
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "32" "./numconvert" "" "255" "stdout/32.txt"                       #invalid
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "33" "./numconvert" "0z" "255" "stdout/33.txt"                     #invalid hexadecimal
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "34" "./numconvert" " " "255" "stdout/34.txt"                      #invalid
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "35" "./numconvert" \"\" "255" "stdout/35.txt"                     #invalid
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "36" "./numconvert" "123456789012345678901" "255" "stdout/36.txt"  #invalid decimal
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

./functional "37" "./numconvert" "0xg" "255" "stdout/37.txt"                    #invalid hexadecimal
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi


# Perform additional coverage tests

./functional "38" "./numconvert" "17446744073709551616" "0" "stdout/38.txt"     #valid decimal
rc=$?; if [[ $rc != 0 ]]; then EXIT_FAILURE=$rc; fi

echo "Script completed."

exit $EXIT_FAILURE
