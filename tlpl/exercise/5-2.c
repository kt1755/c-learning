#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {

        printf("Current errnum %d\n", errno);
        exit(EXIT_FAILURE);
    }

    int fd;
    // Open file
    fd = open(argv[1], O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
        exit(EXIT_FAILURE);

    // Không kéo offset file về đầu file do cờ O_APPEND đã đảm bảo việc luôn ghi vào cuối cùng của file
    if (lseek(fd, 0, SEEK_SET) == -1)
        exit(EXIT_FAILURE);

    if (write(fd, argv[2], 4) == -1)
        exit(EXIT_FAILURE);

    return 0;
}
