# Numconvert
[![Build Status](https://travis-ci.org/baskapteijn/numconvert.svg?branch=master)](https://travis-ci.org/baskapteijn/numconvert)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/baskapteijn/numconvert.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/baskapteijn/numconvert/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/baskapteijn/numconvert.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/baskapteijn/numconvert/context:cpp)  
An easy-to-use tool for numeric conversions between Binary, Decimal and Hexadecimal.

## Prerequisites

This readme assumes a Linux or Windows based host machine.

### For building

* [CMake](https://cmake.org/) 3.2.3 or higher
* [GNU](https://gcc.gnu.org/) 4.8.4 C compiler or compatible

### For doxygen

* [Doxygen](http://www.doxygen.nl/) 1.8.11 or compatible
* [Doxywizard](http://www.doxygen.nl/manual/doxywizard_usage.html) 1.8.11 or compatible

### For coverage

* [LCOV](http://ltp.sourceforge.net/coverage/lcov.php) 1.12 or compatible

### For profiling

* [Valgrind](http://www.valgrind.org/) 3.11.0 or compatible
* [KCachegrind](https://kcachegrind.github.io/html/Home.html) 0.7.4kde or compatible

## Building

From the build directory:
```bash
$ cd build
$ cmake -D CMAKE_BUILD_TYPE=Release ..
-- The C compiler identification is GNU 5.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /src/numconvert/build
$ make
Scanning dependencies of target numconvert
[ 50%] Building C object CMakeFiles/numconvert.dir/src/main.c.o
[100%] Linking C executable numconvert
[100%] Built target numconvert
```
To switch to a Debug build you can specify the following option in the CMake command line:
```bash
$ cmake -D CMAKE_BUILD_TYPE=Debug ..
$ make
```

## Doxygen

Doxygen generation with the bash script has only been tested on a Linux based host machine.  
Make sure that the doxygen.sh has execute permission.

The doxygen script will generate the doxygen documentation.

From the doxygen directory:

```bash
$ ./doxygen.sh
Script completed.
```
The default browser will automatically display the results.

## Coverage

Coverage generation with the bash script has only been tested on a Linux based host machine.  
Make sure that the coverage.sh has execute permission.

The coverage script will run a predefined set of tests to get as much coverage as possible.

From the coverage directory:

```bash
$ ./coverage.sh 
Reading data file coverage.info
Found 2 entries.
Found common filename prefix "/src/numconvert"
Writing .css and .png files.
Generating output.
Processing file src/version.h
Processing file src/main.c
Writing directory view page.
Overall coverage rate:
  lines......: 96.2% (205 of 213 lines)
  functions..: 100.0% (19 of 19 functions)
  branches...: 92.7% (115 of 124 branches)
Script completed.
```
The default browser will automatically display the results.

## Profiling

### Visual profiling

Visual profiling with the bash script has only been tested on a Linux based host machine.  
Make sure that the profile.sh has execute permission.  
The script will handle up to 1 argument and will pass this into numconvert_prof for profiling.

From the profiling directory:

```bash
$ ./profiling.sh 0x12345678
Script completed.
```
KCachegrind will automatically display the results.

### Memcheck

Memcheck with the bash script has only been tested on a Linux based host machine.  
Make sure that the memcheck.sh has execute permission.  
The script will handle up to 1 argument and will pass this into numconvert_prof for memory checking.

From the profiling directory:

```bash
$ ./memcheck.sh 0x42
==6563== Memcheck, a memory error detector
==6563== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==6563== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==6563== Command: ./numconvert_prof 0x42
==6563== 

Decimal:
  66
Binary:
  0000 0000 0000 0000 0000 0000 0000 0000 
  0000 0000 0000 0000 0000 0000 0100 0010 
Hexadecimal:
  0x42

==6563== 
==6563== HEAP SUMMARY:
==6563==     in use at exit: 0 bytes in 0 blocks
==6563==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
==6563== 
==6563== All heap blocks were freed -- no leaks are possible
==6563== 
==6563== For counts of detected and suppressed errors, rerun with: -v
==6563== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
Script completed.
```

## Tests

### Functional tests

Functional tests with the bash script have only been tested on a Linux based host machine.  
Make sure that the functional_test.sh has execute permission. 

The functional test script will run a predefined set of tests to functionally test as much possible.

From the test/functional/ directory:

```bash
$ ./functional_test.sh 
Test 1:successful
Test 2:successful
Test 3:successful
Test 4:successful
Test 5:successful
Test 6:successful
Test 7:successful
Test 8:successful
Test 9:successful
Test 10:successful
Test 11:successful
Test 12:successful
Test 13:successful
Test 14:successful
Test 15:successful
Test 16:successful
Test 17:successful
Test 18:successful
Test 19:successful
Test 20:successful
Test 21:successful
Test 22:successful
Test 23:successful
Test 24:successful
Test 25:successful
Test 26:successful
Test 27:successful
Test 28:successful
Test 29:successful
Test 30:successful
Test 31:successful
Test 32:successful
Test 33:successful
Test 34:successful
Test 35:successful
Test 36:successful
Test 37:successful
Test 38:successful
Script completed.
```

## Usage

### Binary conversion

For binary input postfix 'b' can be used.

```bash
$ numconvert 0110000101001110b

Decimal:
  24910
Binary:
  0000 0000 0000 0000 0000 0000 0000 0000 
  0000 0000 0000 0000 0110 0001 0100 1110 
Hexadecimal:
  0x614e

```

### Decimal conversion

```bash
$ numconvert 12345678

Decimal:
  12345678
Binary:
  0000 0000 0000 0000 0000 0000 0000 0000 
  0000 0000 1011 1100 0110 0001 0100 1110 
Hexadecimal:
  0xbc614e

```

### Hexadecimal conversion

For hexadecimal input both prefix '0x' and postfix 'h' can be used.

```bash
$ numconvert 0x12345678

Decimal:
  305419896
Binary:
  0000 0000 0000 0000 0000 0000 0000 0000 
  0001 0010 0011 0100 0101 0110 0111 1000 
Hexadecimal:
  0x12345678

```
### Help menu

Any invalid input will cause the help menu to be printed.

```bash
$ ./numconvert
Version 1.0.4

Usage:
  numconvert [prefix]<value>[postfix]

No prefix/postfix:
            to indicate a decimal value

Prefixes:
  0x or 0X, to indicate a hexadecimal value

Postfixes:
  b or B,   to indicate a binary value
  h or H,   to indicate a hexadecimal value

```
