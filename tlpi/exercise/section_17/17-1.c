#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/xattr.h>
#include <stdlib.h>
#include <string.h>
#include <sys/acl.h> // $ sudo apt-get install libacl1-dev
#include "ugid_functions.h"

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
    printf("Identity check: %s\n", idStr);

    char *fileName = argv[optind];

    gid_t groupID;
    uid_t userID;

    switch (type)
    {
    case 'u':
        userID = userIdFromName(idStr);
        break;

    case 'g':
        groupID = groupIdFromName(idStr);
        break;

    default:
        break;
    }

    getchar

    return 0;
}
