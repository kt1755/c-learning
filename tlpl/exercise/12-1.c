#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>

uid_t userIDFromName(const char *name);

// Step thực hiện
// 1. Loop toàn bộ thư mục /proc
//     - Lựa chọn những file có type là DT_DIR
// 2. Với mỗi thư mục con, truy cập và đọc file "status"

int main(int argc, char const *argv[])
{
    int errnum;
    unsigned int euid;
    char const *username;

    if (argc >= 2 || strcmp(argv[1], "--help") == 0)
    {
        errnum = errno;
        printf("Current errnum %d\n", errnum);
        exit(EXIT_FAILURE);
    }

    if (argc == 1)
    {
        username = getlogin();
    }
    else
    {
        username = argv[1];
    }

    printf("Check username: %s\n", username);

    euid = userIDFromName(username);
    if (euid == -1)
    {
        printf("Error when get userID from username %s\n", username);
        exit(EXIT_FAILURE);
    }

    // Định nghĩa và đọc directory
    DIR *procDir;
    DIR *pidDir;

    if ((procDir = opendir("/proc")) == NULL)
    {
        printf("Không thể mở thư mục /proc \n");
        exit(EXIT_FAILURE);
    }

    closedir(procDir);

    /* code */
    return 0;
}

uid_t userIDFromName(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || *name == '\0') /* On NULL or empty string */
        return -1;                     /* return an error */

    u = strtol(name, &endptr, 10); /* As a convenience to caller */
    if (*endptr == '\0')           /* allow a numeric string */
        return -1;

    pwd = getpwnam(name);

    return pwd->pw_uid;
}