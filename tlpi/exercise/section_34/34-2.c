#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    printf("Start...\n");
    __pid_t childId, newGrID;
    int setGrErr;
    setbuf(stdout, NULL);

    // Need call fork
    switch (childId = fork())
    {
    case -1:
        perror("Fork");
    case 0:       // Child
        printf("Call in child\n");
        sleep(2); // Give parent chance to execute setpgid before or after child call exec
        execl("/bin/bash", "echo", "Echo exec", NULL);
        sleep(10);
        break;

    default: // parent
        if (argc > 1) {
            sleep(3);
        }
        // Change the process group id
        if (setpgid(childId, newGrID) == -1)
        {
            printf("setpgid errno: %d ; error: %s\n",errno, strerror(errno));
        }
        else
        {
            printf("setpgid success\n");
        }
    }

    
}
