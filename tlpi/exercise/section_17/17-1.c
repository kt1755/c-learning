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
    char *type;
    char *idStr;

    int opt;
    while ((opt = getopt(argc, argv, ":t:i:")) != -1)
    {
        switch (opt)
        {
        case 't':
            type = optarg;
            break;

        case 'i':
            idStr = optarg;
            break;

        default:
            printf("Option unknown: %c\n", opt);
        }
    }

    printf("Type affected: %s\n", type);

    return 0;
}
