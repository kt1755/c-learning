#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>

struct passwd *getpwnam(const char *name)
{
    struct passwd *pwd;
    struct passwd *selectedPwd;
    while ((pwd = getpwent()) != NULL)
        if (strcmp(pwd->pw_name, name) == 0)
        {
            selectedPwd = pwd;
            break;
        }
    endpwent();

    return selectedPwd;
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
