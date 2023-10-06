#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}

int dup_impl(int oldfd)
{
    return fcntl(oldfd, F_DUPFD);
}

int dup2_impl(int oldfd, int newfd)
{
    if (newfd == oldfd)
    {
        if (fcntl(oldfd, F_GETFL) == -1)
        {
            errno = EBADF;
            return -1;
        }

        return oldfd;
    }

    // Close newfd nếu như có file đang sử dụng descriptor này
    if (fcntl(newfd, F_GETFL) != -1)
    {
        close(newfd);
    }

    return fcntl(oldfd, F_DUPFD, newfd);
}