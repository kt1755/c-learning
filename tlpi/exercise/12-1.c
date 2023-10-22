#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

uid_t userIDFromName(const char *name);
_Bool IsDir(struct dirent *de);

// Step thực hiện
// 1. Loop toàn bộ thư mục /proc
//     - Lựa chọn những file có type là DT_DIR
// 2. Với mỗi thư mục con, truy cập và đọc file "status"

int main(int argc, char const *argv[])
{
    int errnum;
    unsigned int euid;
    char const *username;

    if (argc > 2)
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

    struct dirent *procDirent;
    struct dirent *pidDirent;

    while ((procDirent = readdir(procDir)) != NULL)
    {
        if(IsDir(procDirent)) {
            printf("File %s is directory \n", procDirent->d_name);

            // pidDir = opendir(procDirent->d_name);
        }
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

_Bool IsDir(struct dirent *de)
{
    _Bool is_dir;
#ifdef _DIRENT_HAVE_D_TYPE
    if (de->d_type != DT_UNKNOWN && de->d_type != DT_LNK)
    {
        is_dir = (de->d_type == DT_DIR);
    }
    else
#endif
    {
        struct stat stbuf;
        // stat follows symlinks, lstat doesn't.
        stat(de->d_name, &stbuf); // TODO: error check
        is_dir = S_ISDIR(stbuf.st_mode);
    }

    return is_dir;
}