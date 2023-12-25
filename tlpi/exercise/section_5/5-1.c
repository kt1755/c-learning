#define _FILE_OFFSET_BITS 64

#include <stdio.h>
// #include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    off_t off;
    int errnum;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        errnum = errno;
        printf("Current errnum %d\n", errnum);
        exit(EXIT_FAILURE);
    }

    // Open file
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        exit(EXIT_FAILURE);

    off = atoll(argv[2]); // Convert string to long long integer

    if (lseek(fd, off, SEEK_SET) == -1)
        exit(EXIT_FAILURE);

    if (write(fd, "test", 4) == -1)
        exit(EXIT_FAILURE);

    exit(EXIT_SUCCESS);
}
