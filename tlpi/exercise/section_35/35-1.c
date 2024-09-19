#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    /* code */
    int niceValue;
    errno = 0;
    if (argc == 1)
    {

        niceValue = getpriority(PRIO_PROCESS, getpid());
        if (niceValue == -1)
        {
            if (errno != 0)
            {
                perror("Get nice value of current process error");
                return EXIT_FAILURE;
            }
        }

        printf("Nice value of current process is %d\n", niceValue);
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
    // return execl();
}
