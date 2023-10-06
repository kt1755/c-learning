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
    struct passwd *userID;
    userID = userFromName(user);
    if (userID == NULL)
    {
        return -1;
    }

    userID->pw_gid;

    printf("Start looking in group\n");

    struct group *grp;
    char **grMem;
    while ((grp = getgrent()) != NULL)
    {
        grMem = grp->gr_mem;
        if (*grMem != NULL)
        {
            printf("Value group (%s) mem: %s \n", grp->gr_name, *grMem);
        }

        // while (*grMem != NULL)
        // {
        //     if (strcmp(**grMem, user) == 0)
        //     {
        //         printf("Catched you in group %s", grp->gr_name);
        //     }
        // }
    }
    endgrent();

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
    char *username;
    username = getlogin();
    n = initgroups_impl(username, 0);
    printf("%d", n);
    return 0;
}