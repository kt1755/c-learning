#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

// result: f2 file bị reset offset qua lseek nên file luôn bị ghi đè chứ ko append như f1
int main(int argc, char const *argv[])
{
    if (argc != 3 && argc != 4 || strcmp(argv[1], "--help") == 0)
    {

        printf("Current errnum %d\n", errno);
        exit(EXIT_FAILURE);
    }

    int x = 0;
    printf("Argc: %d\n", argc);
    printf("Argv[4]: %s\n", argv[3]);

    if (argc == 4 && strcmp(argv[3], "x") == 0)
    {
        x = 1;
    }
    else
    {
        printf("x not input\n");
    }

    printf("x = %d\n", x);

    int fd;
    // Open file
    int flags;
    flags = O_RDWR | O_CREAT | O_APPEND;
    fd = open(argv[1], flags, S_IRUSR | S_IWUSR);
    if (fd == -1)
        exit(EXIT_FAILURE);

    if (x != 0) // Cờ x có định nghĩa
    {
        if (fcntl(fd, F_SETFL, flags & ~O_APPEND) == -1)
        {
            perror("Cannot change file open flags");
            exit(EXIT_FAILURE);
        };

        if (lseek(fd, 0, SEEK_END) == -1)
        {
            perror("Cannot lseek file");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Lseek to head of file\n");
        }
    }

    int num;
    num = atol(argv[2]);
    int *byteWrites = (int *)malloc(sizeof(int) * num);

    if (write(fd, byteWrites, num) == -1)
    {
        perror("Cannot write to file");
        exit(EXIT_FAILURE);
    }

    return 0;
}
