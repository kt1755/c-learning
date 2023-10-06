#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[])
{

    /* code */
    char *fileName;
    char *content;

    // TODO read getopt()
    int opt;
    bool isappend = false;

    // Dấu : đầu tiên xác định rằng không cần trả lỗi khi gặp một option không xác định
    // Dấu : thứ 2 xác định răng option phía trước cần 1 tham số theo sau
    while ((opt = getopt(argc, argv, ":af:")) != -1)
    {
        /* code */
        printf("Current option check is opt %c \n", opt);
        printf("Current optind %d \n", optind);
        // printf("Next argv %s \n", argv[optind]);
        switch (opt)
        {
        case 'f':
            fileName = optarg;
            printf("Need append to file: %s\n", fileName);

            break;

        case 'a':
            printf("Need append content\n");
            isappend = true;
            break;

        // case '?':
        //     printf("Unknown option %c\n", optopt);
        //     break;
        default:
            printf("Current options %c \n", optopt);
            break;
        }
    }

    if (optind < argc)
    {
        content = argv[optind];
    }

    printf("%s\n", content);

    /* Bắt đầu xử lí file*/
    int fd;

    int fileFlags;

    fileFlags = O_CREAT | O_RDWR | O_EXCL | F_GETFL;
    if (!isappend)
    {
        fileFlags = fileFlags | O_TRUNC;
    }
    else
    {
        fileFlags = fileFlags | O_APPEND;
    }

    // S_IWOTH | S_IROTH | S_IRUSR | S_IWUSR: Quyền cho owner và other user read/write file
    fd = open(fileName, fileFlags, S_IWOTH | S_IROTH | S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }

    int writeN;
    writeN = write(fd, content, strlen(content));
    if (writeN == -1)
    {
        perror("Cannot write to file");
        // exit(EXIT_FAILURE);
    }

    int closeN;
    if (close(fd) == -1)
    {
        perror("Cannot clsoe file");
        exit(EXIT_FAILURE);
    }

    return 0;
}
