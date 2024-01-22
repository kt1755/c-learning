#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/xattr.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *eaStr;
    char *eaValue;
    char *filePath;
    int opt;
    while ((opt = getopt(argc, argv, ":a:f:v:")) != -1)
    {
        switch (opt)
        {
        case 'a':
            eaStr = optarg;
            break;

        case 'v':
            eaValue = optarg;
            break;

        case 'f':
            filePath = optarg;
            break;

        default:
            printf("Option unknown: %c\n", opt);
        }
    }

    printf("EA: %s\n", eaStr);
    printf("EA value: %s\n", eaValue);
    printf("File: %s\n", filePath);

    #if __linux__
    if (setxattr(filePath, eaStr, eaValue, strlen(eaValue), 0) == -1) {
        perror("Set ea failed\n");
        return EXIT_FAILURE;
    }
    #elif __APPLE__
    if (setxattr(filePath, eaStr, eaValue, strlen(eaValue), 0, XATTR_NOSECURITY) == -1) {
        perror("Set ea failed\n");
        return EXIT_FAILURE;
    }
    #endif

    printf("Change attr sucessful no 1\n");
    
    return EXIT_SUCCESS;
}
