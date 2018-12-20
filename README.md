# Numconvert
[![Build Status](https://travis-ci.org/baskapteijn/numconvert.svg?branch=master)](https://travis-ci.org/baskapteijn/numconvert)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/baskapteijn/numconvert.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/baskapteijn/numconvert/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/baskapteijn/numconvert.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/baskapteijn/numconvert/context:cpp)  
An easy-to-use tool for numeric conversions between Binary, Decimal and Hexadecimal.

## Prerequisites

This readme assumes a Linux or Windows based host machine.

* CMake 3.2.3 or higher
* GNU 4.8.4 C compiler or compatible

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
Make sure that the coverage.sh has execute permission and that [LCOV](http://ltp.sourceforge.net/coverage/lcov.php) 1.12 or compatible has been installed.

From the coverage directory:

```bash
$ ./coverage.sh 
Reading data file coverage.info
Found 2 entries.
Found common filename prefix "/src/numconvert"
Writing .css and .png files.
Generating output.
Processing file src/main.c
Processing file src/version.h
Writing directory view page.
Overall coverage rate:
  lines......: 97.2% (205 of 211 lines)
  functions..: 100.0% (19 of 19 functions)
  branches...: 93.4% (114 of 122 branches)
Script completed. See index.html for results.
```
Your default browser will automatically display the results.

## Profiling

Profiling with the bash script has only been tested on a Linux based host machine.  
Make sure that the profile.sh has execute permission and that [valgrind](http://www.valgrind.org/) 3.11.0 and [kcachegrind](https://kcachegrind.github.io/html/Home.html) 0.7.4kde or compatible have been installed.  
The script will handle up to 1 argument and will pass this into numconvert_prof for profiling.

From the profiling directory:

```bash
$ ./profiling.sh 0x12345678
Script completed.
```
KCachegrind will automatically display the results.

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
