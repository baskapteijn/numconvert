/*
 * This file is part of the numconvert distribution (https://github.com/baskapteijn/numconvert).
 * Copyright (c) 2018 Bas Kapteijn.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "version.h"

#define BINARY_STRING_LEN_MAX           65u
#define DECIMAL_STRING_LEN_MAX          20u
#define HEXADECIMAL_H_STRING_LEN_MAX    17u
#define HEXADECIMAL_0X_STRING_LEN_MAX   18u
#define BUFFER_SIZE                     (BINARY_STRING_LEN_MAX + 1u) /* + string terminator. */

/* UINT64_MAX equivalent string. */
const char *DecimalStringValueMax = "18446744073709551615";

static bool IsBinaryChar(char c);
static bool IsDecimalChar(char c);
static bool IsHexadecimalChar(char c);
static void PrintBinary(uint64_t number);
static void PrintDecimal(uint64_t number);
static void PrintHexadecimal(uint64_t number);
static int ParseBinary(const char *string, size_t len, uint64_t *number);
static int ParseDecimal(const char *string, size_t len, uint64_t *number);
static int ParseHexadecimal(const char *string, size_t len, uint64_t *number);
static bool IsBinary(const char *string, size_t len);
static bool IsDecimal(const char *string, size_t len);
static bool IsHexadecimal(const char *string, size_t len);
static int PrintNumerals(uint64_t number);
static int ParseString(const char *string, size_t len, uint64_t *number, char base);
static int CheckString(const char *string, size_t len, char *base);
static void PrintHelp(void);

/*!
 * \brief Convert a character to it's lowercase equivalent (when required).
 * \param c
 *      The character to convert.
 * \returns
 *      The lowercase equivalent of c or c when no conversion was required.
 */
static char ToLower(char c)
{
    if ((c >= 'A') && (c <= 'Z')) {
        c = (char)(c + 0x20);
    }

    return c;
}

/*!
 * \brief Indicate if a character represents a valid Binary character.
 * \param c
 *      The character to verify.
 * \returns
 *      true if the character represents a valid Binary character, otherwise false.
 */
static bool IsBinaryChar(char c)
{
    bool isBinaryChar = false;

    if ((c == '0') || (c == '1')) {
        isBinaryChar = true;
    }

    return isBinaryChar;
}

/*!
 * \brief Indicate if a character represents a valid Decimal character.
 * \param c
 *      The character to verify.
 * \returns
 *      true if the character represents a valid Decimal character, otherwise false.
 */
static bool IsDecimalChar(char c)
{
    bool isDecimalChar = false;

    if ((c >= '0') && (c <= '9')) {
        isDecimalChar = true;
    }

    return isDecimalChar;
}

/*!
 * \brief Indicate if a character represents a valid Hexadecimal character.
 * \param c
 *      The character to verify.
 * \returns
 *      true if the character represents a valid Hexadecimal character, otherwise false.
 */
static bool IsHexadecimalChar(char c)
{
    bool isHexaDecimalChar = false;

    if ((IsDecimalChar(c) == true) ||
        ((c >= 'a') && (c <= 'f'))) {
        isHexaDecimalChar = true;
    }

    return isHexaDecimalChar;
}

/*!
 * \brief Print a number in Binary format to the console.
 * \param number
 *      The number to print.
 */
static void PrintBinary(uint64_t number)
{
    int64_t i = 0;

    printf("Binary:\n");
    printf("  ");

    for (i = 63; i >= 0; i--) {
        if (((number >> i) & 0x1) == 0x1) {
            printf("1");
        } else {
            printf("0");
        }

        if (i % 4 == 0) {
            printf(" ");
        }

        if (i % 32 == 0) {
            printf("\n");
            printf("  ");
        }
    }

    printf("\r");
}

/*!
 * \brief Print a number in Decimal format to the console.
 * \param number
 *      The number to print.
 */
static void PrintDecimal(uint64_t number)
{
    printf("Decimal:\n");
    printf("  %llu\n", (unsigned long long)number);
}

/*!
 * \brief Print a number in Hexadecimal format to the console.
 * \param number
 *      The number to print.
 */
static void PrintHexadecimal(uint64_t number)
{
    printf("Hexadecimal:\n");
    printf("  0x%llx\n", (long long unsigned int)number);
}

/*!
 * \brief Parse a string that represents a Binary value and returns its value.
 * \param string
 *      The string to parse.
 * \param len
 *      The length of the input string (excluding the string terminator).
 * \number[out]
 *      The number represented by the numeric string.
 * \returns
 *      Always 0 (successful).
 */
static int ParseBinary(const char *string, size_t len, uint64_t *number)
{
    int64_t i = 0;

    *number = 0;

    for (i = ((int64_t)len - 1); i >= 0; i--) {
        if (string[i] == '1') {
            *number += (uint64_t)1 << (len - 2 - i);
        }
    }

    return 0;
}

/*!
 * \brief Parse a string that represents a Decimal value and returns its value.
 * \param string
 *      The string to parse.
 * \param len
 *      The length of the input string (excluding the string terminator).
 * \number[out]
 *      The number represented by the numeric string.
 * \returns
 *      Always 0 (successful).
 */
static int ParseDecimal(const char *string, size_t len, uint64_t *number)
{
    uint64_t val = 0;
    int64_t i = 0;
    uint64_t value = 0;

    *number = 0;

    val = 1;
    for (i = ((int64_t)len - 1); i >= 0; i--) {
        value = (string[i] - 0x30) * val;
        *number += value;
        val *= 10;
    }

    return 0;
}

/*!
 * \brief Parse a string that represents a Hexadecimal value and returns its value.
 * \param string
 *      The string to parse.
 * \param len
 *      The length of the input string (excluding the string terminator).
 * \number[out]
 *      The number represented by the numeric string.
 * \returns
 *      Always 0 (successful).
 */
static int ParseHexadecimal(const char *string, size_t len, uint64_t *number)
{
    int64_t i = 0;
    uint64_t value = 0;
    uint64_t start = 0;
    int64_t end = 0;

    *number = 0;

    if (string[len - 1] == 'h') {
        start = len - 2;
        end = 0;
    } else { /* starts with 0x */
        start = len - 1;
        end = 2;
    }

    for (i = (int64_t)start; i >= end; i--) {
        switch (string[i]) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
                /* No break. */
            case 'f':
                value = (uint64_t)string[i] - 0x61 + 10;
                break;
            default:
                value = (uint64_t)string[i] - 0x30;
                break;
        }
        *number |= value << ((start - i) * 4);
    }

    return 0;
}

/*!
 * \brief Indicate if a string represents a valid Binary value.
 * \param string
 *      The string to verify.
 * \param len
 *      The length of the input string (excluding the string terminator).
 * \returns
 *      true if the string represents a valid Binary value, otherwise false.
 */
static bool IsBinary(const char *string, size_t len)
{
    bool isBinary = false;
    uint64_t i = 0;

    if (string[len - 1] == 'b') {
        isBinary = true;
        for (i = 0; i < (len - 1); i++) {
            if (IsBinaryChar(string[i]) == false) {
                isBinary = false;
                break;
            }
        }
    }

    if (isBinary == true) {
        if ((len < 2) ||
            (len > BINARY_STRING_LEN_MAX)) {/* 64 characters + b */
            /* This isn't gonna fit in UINT64_MAX. */
            isBinary = false;
        }
    }

    return isBinary;
}

/*!
 * \brief Indicate if a string represents a valid Decimal value.
 * \param string
 *      The string to verify.
 * \param len
 *      The length of the input string (excluding the string terminator).
 * \returns
 *      true if the string represents a valid Decimal value, otherwise false.
 */
static bool IsDecimal(const char *string, size_t len)
{
    bool isDecimal = true;
    uint64_t i = 0;

    for (i = 0; i < len; i++) {
        if (IsDecimalChar(string[i]) == false) {
            isDecimal = false;
            break;
        }
    }

    if (isDecimal == true) {
        if (len == 0) {
            isDecimal = false;
        } else if (len == DECIMAL_STRING_LEN_MAX) {
            for (i = 0; i < DECIMAL_STRING_LEN_MAX; i++) {
                if ((string[i] - 0x30) > (DecimalStringValueMax[i] - 0x30)) {
                    /* This isn't gonna fit in UINT64_MAX. */
                    isDecimal = false;
                    break;
                } else if ((string[i] - 0x30) == (DecimalStringValueMax[i] - 0x30)) {
                    /* Continue checking the lower digits, it can still become too large. */
                    continue;
                } else {
                    /* No need to continue checking the lower digits, it cannot become too large. */
                    break;
                }
            }
        }
    }

    return isDecimal;
}

/*!
 * \brief Indicate if a string represents a valid Hexadecimal value.
 * \param string
 *      The string to verify.
 * \param len
 *      The length of the input string (excluding the string terminator).
 * \returns
 *      true if the string represents a valid Hexadecimal value, otherwise false.
 */
static bool IsHexadecimal(const char *string, size_t len)
{
    bool isHexaDecimal = false;
    uint64_t i = 0;
    uint64_t start = 0;
    uint64_t end = 0;
    uint64_t minLen = 0;
    uint64_t maxLen = 0;

    if (string[len - 1] == 'h') {
        start = 0;
        end = len - 1;
        minLen = 2;
        maxLen = HEXADECIMAL_H_STRING_LEN_MAX;
    } else if ((string[0] == '0') && (string[1] == 'x')) {
        start = 2;
        end = len;
        minLen = 3;
        maxLen = HEXADECIMAL_0X_STRING_LEN_MAX;
    }

    for (i = start; i < end; i++) {
        isHexaDecimal = true;
        if (IsHexadecimalChar(string[i]) == false) {
            isHexaDecimal = false;
            break;
        }
    }

    if (isHexaDecimal == true) {
        if ((len < minLen) ||
            (len > maxLen)) {/* 0x + 16 characters */
            /* This isn't gonna fit in UINT64_MAX. */
            isHexaDecimal = false;
        }
    }

    return isHexaDecimal;
}

/*!
 * \brief Print a number to the console in Binary, Decimal and Hexadecimal.
 * \param number
 *      The number to print.
 * \returns
 *      0 in case of successful completion or any other value in case of an error.
 */
static int PrintNumerals(uint64_t number)
{
    printf("\n");

    /* Decimal. */
    PrintDecimal(number);

    /* Binary. */
    PrintBinary(number);

    /* Hexadecimal. */
    PrintHexadecimal(number);

    printf("\n");

    return 0;
}

/*!
 * \brief Parse an input string and return it's represented value as a number.
 * \param string
 *      The input string to parse.
 * \param len
 *      The length of the input string (excluding the string terminator).
 * \param[out] number
 *      The number represented by the numeric string in case it checked-out.
 * \param base
 *      The base of the numeric string.
 * \returns
 *      0 in case of successful completion or any other value in case of an error.
 */
static int ParseString(const char *string, size_t len, uint64_t *number, const char base)
{
    int retval = 0;

    switch (base) {
        case 'b':
            retval = ParseBinary(string, len, number);
            break;
        case 'd':
            retval = ParseDecimal(string, len, number);
            break;
        case 'h':
            retval = ParseHexadecimal(string, len, number);
            break;
        default:
            retval = -1;
            break;
    }

    return retval;
}

/*!
 * \brief Check an input string to be an expected numeric string.
 * \param string
 *      The input string to check.
 * \param len
 *      The length of the input string (excluding the string terminator).
 * \param[out] base
 *      The base of the numeric string in case it checked-out.
 * \returns
 *      0 in case of successful completion or any other value in case of an error.
 */
static int CheckString(const char *string, size_t len, char *base)
{
    /* Is the len okay? */
    if (len == 0) {
        return -1;
    }

    /* Is Binary? */
    if (IsBinary(string, len) == true) {
        *base = 'b';
        return 0;
    }

    /* Is Decimal? */
    if (IsDecimal(string, len) == true) {
        *base = 'd';
        return 0;
    }

    /* Is Hexadecimal? */
    if (IsHexadecimal(string, len) == true) {
        *base = 'h';
        return 0;
    }

    /* Is none of the above. */
    return -1;
}

/*!
 * \brief Print the help menu to the console.
 */
static void PrintHelp(void)
{
    printf("Version %s\n\n", GetVersionString());

    printf("Usage:\n");
    printf("  numconvert [prefix]<value>[postfix]\n");

    printf("\nNo prefix/postfix:\n");
    printf("            to indicate a decimal value\n");

    printf("\nPrefixes:\n");
    printf("  0x or 0X, to indicate a hexadecimal value\n");

    printf("\nPostfixes:\n");
    printf("  b or B,   to indicate a binary value\n");
    printf("  h or H,   to indicate a hexadecimal value\n");
}

/*!
 * \brief This is the program entry.
 * \details
 *      Expected arguments (lower and uppercase allowed):
 *          Decimal value, f.e.: 102245
 *          Hexadecimal value, f.e.: 12345h or 0x12345
 *          Binary value, f.e.: 011010110b
 *      Only values between 0 - 2^64 are allowed.
 * \param argc
 *      The number of string pointed to by argv (argument count).
 * \param argv
 *      A list of strings (argument vector).
 * \returns
 *      0 in case of successful completion or any other value in case of an error.
 */
int main(int argc, char *argv[])
{
    char base = '\0';
    char string[BUFFER_SIZE];
    int retval = 0;
    uint64_t number = 0;
    uint64_t i = 0;
    size_t len = 0;

    /* Check argument. */
    if (argc != 2) {
        /* Missing or too many arguments. */
        PrintHelp();
        return -1;
    }

    /* Determine string length. */
    len = strlen(argv[1]);
    if (len >= BUFFER_SIZE) {
        PrintHelp();
        return -1;
    }

    /* Copy argument 2 into our own string string while converting to lower case. */
    memset(string, 0, sizeof(string));
    for (i = 0; i < len; i++) {
        string[i] = ToLower(argv[1][i]);
    }
    /* From this point on we don't have to use ToLower any more. */

    retval = CheckString(string, len, &base);
    if (retval != 0) {
        PrintHelp();
        return retval;
    }

    retval = ParseString(string, len, &number, base);
    if (retval != 0) {
        PrintHelp();
        return retval;
    }

    retval = PrintNumerals(number);
    if (retval != 0) {
        PrintHelp();
        return retval;
    }

    return 0;
}
