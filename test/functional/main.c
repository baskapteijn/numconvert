#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define STRING_LENGTH_MAX       100u /* 99 + string terminator */
#define DECIMAL_STRING_LEN_MAX  3u

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

int main(int argc, char *argv[])
{
    char *line = 0;
    char stdoutString[512];
    char string[STRING_LENGTH_MAX];
    int retval = 0;
    FILE *fp = 0;

    /* Parse the arguments.
     *
     * argv[0] is this executable path
     * argv[1] is the test executable system path, f.e. "./numconvert"
     * argv[2] is the test input value, f.e. "0x12" or ""
     * argv[3] is the test expected return value, f.e. "0" between 0 and 255
     * argv[4] is the test file that contains the expected (stdout)output, f.e. "tests/1.txt"
     *
     * TODO: add verbosity argument to enable printing of return codes on error and stdout on error
     */

    /* Check arguments. */
    if (argc != 5) {
        /* Missing or too many arguments. */
        return -1;
    }

    if (IsDecimal(argv[3], strlen(argv[3])) == false) {
        /* The given return value is not a decimal character. */
        return -1;
    }

    snprintf(string, STRING_LENGTH_MAX, "%s %s", argv[1], argv[2]);

    fp = popen(string, "r");
    while (1) {
        line = fgets(stdoutString, sizeof(stdoutString), fp);
        if (line == NULL) {
            break;
        }
        printf("%s", line); //TODO: remove when test works
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
    retval = WEXITSTATUS(pclose(fp));
    if (retval != atoi(argv[3])) {
        return RETURN_VALUE_FAILURE_CODE;
    }

    return 0;
}