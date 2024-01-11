/***************/
// gcc 17-1.c ugid_functions.c -o 17-1.out -lacl
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
#include <acl/libacl.h>

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

    int id;

    acl_tag_t tagSelected;
    bool showMask = false;

    struct stat fileStat;
    if ((stat(fileName, &fileStat)) == -1)
    {
        perror("Cannot get stat of file");
        return EXIT_FAILURE;
    }

    switch (*type)
    {
    case 'u':
        id = userIdFromName(idStr);

        if (fileStat.st_uid != id)
        {
            tagSelected = ACL_USER;
            showMask = true;
        }
        else
        {
            tagSelected = ACL_USER_OBJ;
        }
        break;

    case 'g':
        id = groupIdFromName(idStr);

        if (fileStat.st_uid != userID)
        {
            tagSelected = ACL_GROUP_OBJ;
            showMask = true;
        }
        else
        {
            tagSelected = ACL_GROUP;
        }
        showMask = true;
        break;

    default:
        break;
    }

    acl_t acl = acl_get_file(fileName, ACL_TYPE_ACCESS);
    acl_entry_t entry;
    int r;
    bool found = false;
    acl_permset_t permset;
    for (r = acl_get_entry(acl, ACL_FIRST_ENTRY, &entry); r > 0; r = acl_get_entry(acl, ACL_NEXT_ENTRY, &entry))
    {
        acl_tag_t tag;
        if (acl_get_tag_type(entry, &tag) == -1)
        {
            return errno;
        }

        if (tag == tagSelected)
        {
            if (tag == ACL_USER_OBJ || tag == ACL_GROUP_OBJ) {
                
                acl_get_permset(entry, &permset);
            } else if (tag == ACL_USER || tag == ACL_GROUP) {

            }
        }
    }

    if (acl_free(acl) == -1) {
        perror("acl_free");
        return EXIT_FAILURE;
    }

    return 0;
}
