#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define STRING_LENGTH_MAX       100 /* 99 + string terminator */

int main(int argc, char *argv[])
{
    char *line = 0;
    char buf[512];
    FILE *fp = 0;

    /* Parse the arguments.
     *
     * argv[0] is this executable path
     * argv[1] is the test executable path + arguments list, f.e. "./numconvert 0x12"
     */

    /* Check arguments. */
    if (argc != 2) {
        /* Missing or too many arguments. */
        return -1;
    }

    fp = popen(argv[1], "r");
    while (1) {
        line = fgets(buf, sizeof(buf), fp);
        if (line == NULL) {
            break;
        }
        printf("%s", line);
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
    return WEXITSTATUS(pclose(fp));
}