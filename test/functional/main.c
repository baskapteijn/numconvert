#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define STRING_LENGTH_MAX           100u /* 99 + string terminator */
#define DECIMAL_STRING_LEN_MAX      3u

#define STDOUT_STRING_LENGTH_MAX    512u

#define RETURN_VALUE_FAILURE_CODE   100u
#define STDIO_OUTPUT_FAILURE_CODE   200u

/* UINT64_MAX equivalent string. */
const char *DecimalStringValueMax = "255";

static bool IsDecimalChar(char c);
static bool IsDecimal(const char *string, size_t len);

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
 * \brief This is the program entry.
 * \details
 *      Test an executable with a list of input arguments and compare its returned value and stdout
 *      data to a predefined set.
 *
 *      Expected arguments:
 *          argv[0] is this executable path
 *          argv[1] is the test name, f.e. "1" or "1a"
 *          argv[2] is the test executable system path, f.e. "./numconvert"
 *          argv[3] is the test input value, f.e. "0x12" or "" or even "1 2"
 *          argv[4] is the test expected return value, f.e. "0" between 0 and 255
 *          argv[5] is the test file that contains the expected (stdout)output, f.e. "tests/1.txt"
 *
 *      argv[3] accepts a special input 'NOARG' with allows the program under test to be called
 *      without any arguments.
 * \param argc
 *      The number of string pointed to by argv (argument count).
 * \param argv
 *      A list of strings (argument vector).
 * \returns
 *      0 in case of successful completion or any other value in case of an error.
 */
int main(int argc, char *argv[])
{
    char *stdoutLine = 0;
    char *stdoutExpLine = 0;
    char stdoutString[STDOUT_STRING_LENGTH_MAX];
    char stdoutExpString[STDOUT_STRING_LENGTH_MAX];
    char string[STRING_LENGTH_MAX];
    int retval = 0;
    int stdoutStrLen = 0;
    int stdoutExpStrLen = 0;
    uint32_t i = 0;
    uint32_t lineNumber = 0;
    FILE *stdoutFp = 0;
    FILE *stdoutExpFp = 0;

    if (argc >= 2) {
        printf("Test %s:", argv[1]);
    }

    /* Check arguments. */
    if (argc != 6) {
        /* Missing or too many arguments. */
        printf("argument mismatch\n");
        return -1;
    }

    if (IsDecimal(argv[4], strlen(argv[4])) == false) {
        /* The given return value is not a decimal character. */
        printf("invalid expected return value\n");
        return -1;
    }

    if (strncmp(argv[3], "NOARG", 5) != 0) {
        snprintf(string, STRING_LENGTH_MAX, "%s %s", argv[2], argv[3]);
    } else {
        snprintf(string, STRING_LENGTH_MAX, "%s", argv[2]);
    }
   // printf("popen!\n");
    stdoutFp = popen(string, "r");
    if (stdoutFp == NULL) {
        printf("unable to open test executable\n");
        return -1;
    }
    stdoutExpFp = fopen(argv[5], "r");
    if (stdoutExpFp == NULL) {
        printf("unable to open file with expected stdout output\n");
        return -1;
    }

    lineNumber = 0;
    while (1) {
        lineNumber++;

        stdoutLine = fgets(stdoutString, sizeof(stdoutString), stdoutFp);
        if (stdoutLine == NULL) {
            stdoutExpLine = fgets(stdoutExpString, sizeof(stdoutExpString), stdoutExpFp);
            if (stdoutExpLine != NULL) {
                printf("stdout mismatch\n");

                printf("  line %u (stdout strlen < stdout expected strlen)\n", lineNumber);
                printf("  output:\n");
                printf("  expected output: %s\n", stdoutExpLine);
                return STDIO_OUTPUT_FAILURE_CODE;
            }
            break;
        }

        stdoutExpLine = fgets(stdoutExpString, sizeof(stdoutExpString), stdoutExpFp);
        if (stdoutExpLine == NULL) {
            printf("stdout mismatch\n");

            printf("  line %u (stdout output > stdout expected output)\n", lineNumber);
            printf("  output:       %s", stdoutLine);
            printf("  check output:\n");
            return STDIO_OUTPUT_FAILURE_CODE;
        }

        /* Check string length. */
        stdoutStrLen = strlen(stdoutLine);
        stdoutExpStrLen = strlen(stdoutExpLine);
        if (stdoutStrLen > stdoutExpStrLen) {
            printf("stdout mismatch\n");

            printf("  line %u (stdout strlen > stdout expected strlen)\n", lineNumber);
            printf("  output:       %s", stdoutLine);
            printf("  expected output: %s\n", stdoutExpLine);
            return STDIO_OUTPUT_FAILURE_CODE;
        } else if (stdoutStrLen < stdoutExpStrLen) {
            printf("stdout mismatch\n");

            printf("  line %u (stdout strlen < stdout expected strlen)\n", lineNumber);
            printf("  output:       %s", stdoutLine);
            printf("  expected output: %s\n", stdoutExpLine);
            return STDIO_OUTPUT_FAILURE_CODE;
        }

        /* Check characters. */
        for (i = 0; i < strlen(stdoutLine); i++) {
            if (stdoutLine[i] != stdoutExpLine[i]) {
                printf("stdout mismatch\n");

                printf("  line %u:%u\n", lineNumber, i + 1);
                printf("  output:       %s", stdoutLine);
                printf("  expected output: %s\n", stdoutExpLine);
                return STDIO_OUTPUT_FAILURE_CODE;
            }
        }
    }

    /* Close the process and get the return value.
     *
     * Regarding the return value conversion:
     *
     * To quote from bash man page on EXIT STATUS
     * The exit status of an executed command is the value returned by the waitpid system call or
     * equivalent function.
     * Exit statuses fall between 0 and 255, though, as explained below, the shell may use values
     * above 125 specially.
     * Exit statuses from shell builtins and compound commands are also limited to this range.
     * Under certain circumstances, the shell will use special values to indicate specific failure
     * modes.
     */
    retval = WEXITSTATUS(pclose(stdoutFp));
    if (retval != atoi(argv[4])) {
        printf("return value mismatch\n");
        printf("  got %d expected %d\n", retval, atoi(argv[4]));
        return RETURN_VALUE_FAILURE_CODE;
    }

    /* Close the stdout output test file. */
    (void)fclose(stdoutExpFp);

    /* We reached the end, print the result. */
    printf("successful\n");

    return 0;
}
