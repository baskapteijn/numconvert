#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define BINARY_STRING_LEN_MAX           65
#define DECIMAL_STRING_LEN_MAX          20
#define HEXADECIMAL_H_STRING_LEN_MAX    17
#define HEXADECIMAL_0X_STRING_LEN_MAX   18
#define BUFFER_SIZE                     (65 + 1) /* Binary max + string terminator. */

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

static char ToLower(char c)
{
    if ((c >= 'A') && (c <= 'Z')) {
        c = (char)(c + 0x20);
    }

    return c;
}

static bool IsBinaryChar(char c)
{
    bool isBinaryChar = false;

    if ((c == '0') || (c == '1')) {
        isBinaryChar = true;
    }

    return isBinaryChar;
}

static bool IsDecimalChar(char c)
{
    bool isDecimalChar = false;

    if ((c >= '0') && (c <= '9')) {
        isDecimalChar = true;
    }

    return isDecimalChar;
}

static bool IsHexadecimalChar(char c)
{
    bool isHexaDecimalChar = false;

    if ((IsDecimalChar(c) == true) ||
        ((c >= 'a') && (c <= 'f'))) {
        isHexaDecimalChar = true;
    }

    return isHexaDecimalChar;
}

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

static void PrintDecimal(uint64_t number)
{
    printf("Decimal:\n");
    printf("  %llu\n", (unsigned long long)number);
}

static void PrintHexadecimal(uint64_t number)
{
    printf("Hexadecimal:\n");
    printf("  0x%llx\n", (long long unsigned int)number);
}

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
            assert(false);
            break;
    }

    return retval;
}

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

static void PrintHelp(void)
{
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
 * \brief
 * \details
 *      Expected arguments (lower and uppercase allowed):
 *          Decimal value, f.e.: 102245
 *          Hexadecimal value, f.e.: 12345h or 0x12345
 *          Binary value, f.e.: 011010110b
 * \param argc
 * \param argv
 * \return
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
