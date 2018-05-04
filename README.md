# Numconvert

An easy-to-use tool for numeric conversions between Binary, Decimal and Hexadecimal.

## Prerequisites

This readme assumes a Linux Debian based host machine. It should also be possible to build and run 
the tool with a different Linux distribution or a Windows based machine when the prerequisites are
met.

* CMake 2.5.1 or higher
* GNU 5.4.0 C compiler or compatible

## Building

From the build directory:
```
user@host:/src/numconvert/build$ cmake ..
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
user@host:/src/numconvert/build$ make
Scanning dependencies of target numconvert
[ 50%] Building C object CMakeFiles/numconvert.dir/src/main.c.o
[100%] Linking C executable numconvert
[100%] Built target numconvert
```

## Usage

### Binary conversion

```
user@host:~$ numconvert 0110000101001110b

Decimal:
  24910
Binary:
  0000 0000 0000 0000 0000 0000 0000 0000 
  0000 0000 0000 0000 0110 0001 0100 1110 
Hexadecimal:
  0x614e
```

### Decimal conversion

```
user@host:~$ numconvert 12345678

Decimal:
  12345678
Binary:
  0000 0000 0000 0000 0000 0000 0000 0000 
  0000 0000 1011 1100 0110 0001 0100 1110 
Hexadecimal:
  0xbc614e
```

### Hexadecimal conversion

```
user@host:~$ numconvert 0x12345678

Decimal:
  305419896
Binary:
  0000 0000 0000 0000 0000 0000 0000 0000 
  0001 0010 0011 0100 0101 0110 0111 1000 
Hexadecimal:
  0x12345678
```
