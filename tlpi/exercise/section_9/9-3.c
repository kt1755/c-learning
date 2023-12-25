#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

struct passwd *userFromName(const char *name); // Khai báo nguyên mẫu hàm, giúp C compiler đoán function không bị sai

int initgroups_impl(const char *user, gid_t group)
{
    errno = 0;
    uid_t euid = geteuid();
    if (euid != 0)
    {
        errno = EPERM;
        return -1;
    }

    gid_t *supplementalGroupIDs;
    long maxGroupSize;
    maxGroupSize = sysconf(_SC_NGROUPS_MAX);
    supplementalGroupIDs = malloc(sizeof(gid_t) * maxGroupSize);

    //  struct passwd *userID;
    // userID = userFromName(user);
    // if (userID == NULL)
    // {
    //     return -1;
    // }

    printf("Start looking in group\n");

    struct group *grp;
    char mem;
    size_t size = 0;
    int currentIndex = 0;
    setgrent();
    while ((grp = getgrent()) != NULL)
    {
        for (size_t j = 0;; j++)
        {
            if (grp->gr_mem[j] == NULL)
            {
                break;
            }

            if ((strcmp(grp->gr_mem[j], user)) == 0)
            {
                printf("Found in group id %d name %s \n", grp->gr_gid, grp->gr_name);
                supplementalGroupIDs[currentIndex] = grp->gr_gid;
                size += 1;
                currentIndex += 1;
            }
        }
    }
    endgrent();

    printf("Supplemental group ids\n");
    for (int i = 0; i < currentIndex; i++)
    {
        printf("%d ", supplementalGroupIDs[i]);
    }
    printf("\n");

    printf("Size to set group: %lu\n", size);
    if (setgroups(size, supplementalGroupIDs) == -1)
    {
        if (errno != 0)
        {
            perror("Problem");
        }
        return -1;
    }

    return 0;
}

struct passwd *userFromName(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || *name == '\0') /* On NULL or empty string */
        return NULL;                   /* return an error */

    u = strtol(name, &endptr, 10); /* As a convenience to caller */
    if (*endptr == '\0')           /* allow a numeric string */
        return NULL;

    return getpwnam(name);
}

int main(int argc, char const *argv[])
{
    int n;
    unsigned int euid;
    char *username;
    username = getlogin();
    printf("Check username: %s\n", username);

    setuid(0);

    euid = geteuid();
    printf("Current effective id: %d", euid);

    long maxGroups;
    maxGroups = sysconf(_SC_NGROUPS_MAX);
    gid_t *grouplist;
    // grouplist = malloc(sizeof(gid_t) * maxGroups);

   int currentGroupNum ;
   currentGroupNum = getgroups(0, grouplist);

   grouplist = malloc(sizeof(gid_t) * currentGroupNum);


    if (getgroups(currentGroupNum, grouplist) == -1)
    {
        printf("Error when get current group\n");
    }
    else
    {
        for (int i = 0; i < currentGroupNum; i++)
        {
            printf("%d ", grouplist[i]);
        }
        printf("\n");
    }

    n = initgroups_impl(username, 0);
    printf("%d\n", n);

    if (n == -1)
    {
        perror("initgroups_impl failed");
    }

    return 0;
}