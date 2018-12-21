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

### For coverage

* [LCOV](http://ltp.sourceforge.net/coverage/lcov.php) 1.12 or compatible

### For profiling

* [Valgrind](http://www.valgrind.org/) 3.11.0 or compatible
* [KCachegrind](https://kcachegrind.github.io/html/Home.html) 0.7.4kde or compatible

## Building

From the build directory:
```bash
$ cd build
$ cmake ..
-- The C compiler identification is GNU 5.4.0
-- The CXX compiler identification is GNU 5.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /src/numconvert/build

$ make
Scanning dependencies of target numconvert
[ 50%] Building C object CMakeFiles/numconvert.dir/src/main.c.o
[100%] Linking C executable numconvert
[100%] Built target numconvert
```
To switch between Debug and Release builds you can specify the option in the CMake command line:
```bash
$ cmake -D CMAKE_BUILD_TYPE=Debug ..
$ make
```
or:
```bash
$ cmake -D CMAKE_BUILD_TYPE=Release ..
$ make
```
## Coverage

Coverage generation with the bash script has only been tested on a Linux based host machine.  
Make sure that the coverage.sh has execute permission.

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
  lines......: 96.8% (210 of 217 lines)
  functions..: 100.0% (19 of 19 functions)
  branches...: 94.3% (115 of 122 branches)
Script completed.
```
The default browser will automatically display the results.

## Profiling

### Memcheck

Memcheck with the bash script has only been tested on a Linux based host machine.  
Make sure that the memcheck.sh has execute permission.  
The script will handle up to 1 argument and will pass this into numconvert_prof for memory checking.

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

## Tests

### Functional tests

Functional tests with the bash script have only been tested on a Linux based host machine.  
Make sure that the functional_test.sh has execute permission. 

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
Script completed.
```

## Usage

### Binary conversion

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
