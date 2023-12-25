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
#include <ctype.h>

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

    printf("Username %s have id %d\n", username, euid);

    // Định nghĩa và đọc directory
    DIR *procDir;
    DIR *pidDir;

    errno = 0;
    if ((procDir = opendir("/proc")) == NULL)
    {
        perror("Không thể mở thư mục /proc \n");
        exit(EXIT_FAILURE);
    }

    struct dirent *procDirent;
    struct dirent *pidDirent;

    while ((procDirent = readdir(procDir)) != NULL)
    {
        if (!IsDir(procDirent))
        {
            continue;
        }

        printf("File %s inside /proc is directory \n", procDirent->d_name);
        char *pidDirName;
        pidDirName = calloc(256, sizeof(char));
        strcat(pidDirName, "/proc/");
        strcat(pidDirName, procDirent->d_name);
        printf("Full path file: %s \n", pidDirName);

        errno = 0;
        pidDir = opendir(pidDirName);
        if (pidDir == NULL)
        {
            if (errno == ENOENT)
            {
                printf("File %s not exist, ignore\n", pidDirName);
                continue;
            }
            else
            {
                perror("Không thể mở file");
            }

            continue;
        }

        while ((pidDirent = readdir(pidDir)) != NULL)
        {
            // printf("Found file %s inside %s\n", pidDirent->d_name, pidDirName);
            if (strcasecmp(pidDirent->d_name, "status") == 0)
            {
                char *statusPath;
                statusPath = calloc(256, sizeof(char));
                strcat(statusPath, pidDirName);
                strcat(statusPath, "/");
                strcat(statusPath, pidDirent->d_name);

                FILE *statusFile;
                statusFile = fopen(statusPath, "r");
                if (statusFile == NULL)
                {
                    printf("file %s inside %s not found, ignore parse.\n", statusPath, pidDirName);
                    continue;
                }

                printf("Start read file %s inside %s \n", statusPath, pidDirName);
                // Đọc file, line by line
                char line[100];
                while (fgets(line, sizeof(line), statusFile) != NULL)
                {
                    char *tail;
                    char *key;
                    char *value;
                    tail = strchr(line, '\n');
                    if (tail != NULL)
                        *tail = '\0'; /* remove the trailing '\n' */
                    tail = strchr(line, ':');
                    if (tail != NULL)
                    {
                        tail[0] = '\0';
                        key = strdup(line);
                        if (key == NULL)
                            continue;
                        tail += 1;
                        while ((tail[0] != '\0') && (isspace((int)tail[0]) != 0))
                            tail++;
                        value = strdup(tail);
                        if (value != NULL)
                        {
                            fprintf(stderr, "%s --> %s\n", key, value);
                            /* You could do something now with key/value */
                            free(value);
                        }
                        free(key);
                    }
                }
            }
        }

        closedir(pidDir);
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
