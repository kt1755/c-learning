/***************/
// gcc 17-1.c ugid_functions.c -o 17-1.out
/***************/


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

    

    char *fileName = argv[optind];

    printf("Type affected: %s\n", type);
    printf("Identity check: %s\n", idStr);
    printf("File check: %s\n", fileName);

    gid_t groupID;
    uid_t userID;

    switch (*type)
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

    acl_t acl = acl_get_file(fileName, ACL_TYPE_ACCESS);
    acl_entry_t entry;
    int r;
    for (r = acl_get_entry(acl, ACL_FIRST_ENTRY, &entry); r > 0; r = acl_get_entry(acl, ACL_NEXT_ENTRY, &entry))
    {
        acl_tag_t tag;
        if (acl_get_tag_type(entry, &tag) == -1) {
            return errno;
        }
    }
    

 

    acl_free(acl)

    return 0;
}
