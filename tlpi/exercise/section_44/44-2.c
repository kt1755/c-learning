#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

FILE *popenimpl(const char *command, const char *mode)
{
    int pipeFd[2];
    if (mode[0] != 'r' && mode[0] != 'w' && mode[1] != '\0')
    {
        perror("mode must be r or w\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipeFd) == -1)
    {
        perror("pipe failed\n");
        exit(EXIT_FAILURE);
    }

    switch (fork())
    {
    case -1:
        perror("fork failed\n");
        exit(EXIT_FAILURE);
        break;

    case 0:
        if (mode[0] == 'r')
        {
            close(pipeFd[0]);
            if (pipeFd[1] != STDOUT_FILENO)
            {
                dup2(pipeFd[1], STDOUT_FILENO);
                close(pipeFd[1]);
            }
        }
        else
        {
            close(pipeFd[1]);
            if (pipeFd[0] != STDIN_FILENO)
            {
                dup2(pipeFd[0], STDIN_FILENO);
                close(pipeFd[0]);
            }
        }

        execl("/bin/sh", "sh", "-c", command, (char *)NULL);

        break;

    default:
        break;
    }

    // Parent falls through to here

    int fd;

    if (mode[0] == 'r')
    {
        close(pipeFd[1]);
        if (pipeFd[0] != STDIN_FILENO)
        {
            dup2(pipeFd[0], STDIN_FILENO);
            close(pipeFd[0]);
        }

        fd = STDIN_FILENO;
    }
    else
    {
        close(pipeFd[0]);
        if (pipeFd[1] != STDOUT_FILENO)
        {
            dup2(pipeFd[1], STDOUT_FILENO);
            close(pipeFd[1]);
        }

        fd = STDOUT_FILENO;
    }

    return fdopen(fd, mode);
}

int pcloseimpl(FILE *stream)
{
    wait(NULL);
    return 0;
}

int main(int argc, char const *argv[])
{

    return 0;
}
