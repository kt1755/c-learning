#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    printf("Start...\n");
    __pid_t childId, gid;
    int setGrErr;
    setbuf(stdout, NULL);

    gid = getpgrp();

    switch (childId = fork())
    {
    case -1:
        perror("Fork");
    case 0: // Child
        printf("Call in child\n");

        if ( setpgid(childId, gid) == -1) {
            perror("setpgid");
            return EXIT_FAILURE;
        }

        // become a group leader
        if (argc > 1)
        {
            setpgrp();
        }

        if (setsid() == -1) {
            perror("setsid failed");
        } else {
            printf("Create new session successful\n");
        }
        break;

    default: // parent
        sleep(3);
    }

    return EXIT_SUCCESS;
}
