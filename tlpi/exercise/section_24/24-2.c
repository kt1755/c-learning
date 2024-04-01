#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{

    pid_t childPid;
    char *fileName = "~/tmp.txt";


    int fd = mkstemp(fileName);
    if (fd == -1)
    {
        printf("mkstemp failed");
        exit(EXIT_FAILURE);
    }

    switch (childPid = vfork())
    {
    case -1:
        printf("Fork failed...");
        exit(EXIT_FAILURE);
        break;

    case 0: // child process
        write(STDOUT_FILENO, "Executing", 9);
        close(fd);
        break;

    default:
        wait(NULL);
        int size = write(fd, "aaa", 3);
        if (size == -1) {
            printf("Cannot write to closed fd (closed by child process)");
        } else {
            printf("Still write to file closed by child process");
            close(fd);

            unlink(fileName);
        }
        break;
    }

    return 0;
}
