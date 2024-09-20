#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>

extern char **environ;

int main(int argc, char *argv[])
{
    /* code */
    int niceValue;
    int opt;
    char *prog = NULL;

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

    while ((opt = getopt(argc, argv, "n:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            /* code */
            niceValue = atoi(optarg);
            break;

        default:
            break;
        }
    }

    printf("argc = %d\n", argc);
    printf("optind = %d\n", optind);

    if (nice(niceValue) == -1)
    {
        perror("Cannot set nice value myself\n");
        return EXIT_FAILURE;
    }

    niceValue = getpriority(PRIO_PROCESS, 0);
    printf("Priority after set nice value: %d\n", niceValue);

    if (optind < argc)
    {
        prog = argv[optind];
        printf("Program to execute: %s\n", prog);

        char *progArgs = malloc(sizeof(char)* 255);
        int index = 0;
        for (int i = optind + 1; i < argc; i++)
        {
            printf("Argv %d = %s\n", i, argv[i]);
            progArgs[index] = *argv[i];
            index += 1;
        }

        printf("Param %s\n", progArgs);

        if (execl(prog, progArgs) == -1)
        {
            perror("execl failed");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
    // return execl();
}
