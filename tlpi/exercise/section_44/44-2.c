#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

int pipeFd[2];
FILE *fildHolder[1024];
pid_t childPidHolder[1024];

FILE *popenimpl(const char *command, const char *mode)
{

    pid_t childPid;

    if (mode[0] != 'r' && mode[0] != 'w' && mode[1] != '\0')
    {
        errno = EINVAL;
        return NULL;
    }

    if (pipe(pipeFd) == -1)
    {
        return NULL;
    }

    switch (childPid = fork())
    {
    case -1:
        // errno will be set by fork
        return NULL;


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

    struct FILE *stream = fdopen(fd, mode);
    if (stream == NULL)
    {
        close(fd);
        return NULL;
    }


    for(int i = 0; i < 1024; i++)
    {
        if(fildHolder[i] == NULL)
        {
            fildHolder[i] = stream;
            childPidHolder[i] = childPid;
            break;
        }
    }


    return stream;
}

int pcloseimpl(FILE *stream)
{
    if (stream == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    int *status;
    status = malloc(sizeof(int));

    for(int i = 0; i < 1024; i++)
    {
        if(fildHolder[i] == stream)
        {
            if (fclose(stream) == -1) {
                return -1;
            }
            fildHolder[i] = NULL;
            kill(childPidHolder[i], SIGKILL);

            if ( waitpid(childPidHolder[i], status, 0) == -1) {
                return -1;
            }
            
            break;
        }
    }

    return *status;
}

int main(int argc, char const *argv[])
{

    return 0;
}
