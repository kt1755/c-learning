#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//

#ifndef STD_BUFF_SIZE
#define STD_BUFF_SIZE 1024
#endif

#ifndef BUF_SIZE
#define BUF_SIZE 128
#endif

int main(int argc, char *argv[])
{
    printf("Argc %d\n", argc);

    int opt;
    char *numOfLineStr;
    char *fileName;

    while ((opt = getopt(argc, argv, ":n:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            numOfLineStr = optarg;
            break;
        }
    }

    fileName = argv[optind];
    printf("File to be tailed: %s \n", fileName);
    if (fileName == NULL)
    {
        printf("File name missing\n");
        exit(EXIT_FAILURE);
    }

    int numOfLine = abs(atoi(numOfLineStr));
    printf("Line need to be retreived: %d \n", numOfLine);

    int openFileFlags = O_RDONLY;
    int filePermission = S_IRUSR | S_IRGRP | S_IROTH;

    errno = 0;
    int fd = open(fileName, openFileFlags, filePermission);
    if (fd == -1)
    {
        close(fd);
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    off_t offset = lseek(fd, 0, SEEK_END);
    if (offset == -1)
    {
        if (errno != 0)
        {
            close(fd);
            perror("Cannot seek to end of file");
            exit(EXIT_FAILURE);
        }
    }

    off_t oldOffset = offset;
    int newLineCount = 0;

    char *buff;
    buff = malloc(sizeof(char) * BUF_SIZE);
    while (newLineCount < numOfLine)
    {
        oldOffset = lseek(fd, -1 * BUF_SIZE, SEEK_CUR); // Seek lùi lại từ cuối file
        if (oldOffset == -1)
        {
            if (errno != 0)
            {

                if (errno != EINVAL)
                {
                    close(fd);
                    perror("Error when lseek file file\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    perror("Reach start of file. Show all file content\n");
                    oldOffset = lseek(fd, 0, SEEK_SET);
                    break;
                }
            }
            else
            {
                printf("Old offset = -1. Show all file\n");
                oldOffset = lseek(fd, 0, SEEK_SET);
                break;
            }
        }

        if ((read(fd, buff, BUF_SIZE)) != -1)
        {
            printf("buff: %s \n", buff);
            for (size_t i = 0;; i++)
            {

                if (buff[i] == '\0')
                {
                    printf("Break when null character\n");
                    break;
                }
                if (buff[i] == '\n')
                {
                    printf("Buffer in position %lu is newline \n", i);
                    write(1, buff, BUF_SIZE);
                    newLineCount += 1;
                    break;
                }
            }

            lseek(fd, -BUF_SIZE, SEEK_CUR);
        }

        offset = oldOffset;
    }

    printf("Start write file from offset found! \n");
    lseek(fd, 0, oldOffset);
    size_t readSize;
    while ((readSize = read(fd, buff, BUF_SIZE)) != -1)
    {
        write(1, buff, readSize);
    }

    printf("Reading done\n");
    close(fd);
    /* code */
    return 0;
}
