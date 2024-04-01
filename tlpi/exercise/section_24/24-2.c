#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{

    pid_t childPid;
    char fileName[] = "/tmp/testXXXXXX";
    setbuf(stdout, NULL);

    int fd = mkstemp(fileName);
    if (fd == -1)
    {
        printf("mkstemp failed\n");
        exit(EXIT_FAILURE);
    }

    switch (childPid = vfork())
    {
    case -1:
        printf("Fork failed...\n");
        exit(EXIT_FAILURE);
        break;

    case 0: // child process
        printf("Closing descriptor..\n");
        close(fd);
        break;

    default:
        if (wait(NULL) == -1) {
            printf("Wait error\n");
            exit(EXIT_FAILURE);
        }

        printf("Parent resume\n");

        if ((fcntl(fd, F_GETFD)) == -1)
        {
            printf("Cannot write to closed fd (closed by child process)\n");
        }
        else
        {
            printf("Still write to file closed by child process\n");
        }
        
        break;
    }

    return 0;
}
