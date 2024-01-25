#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

char *realpathImpl(const char *pathname, char *resolved_path)
{
    struct stat sb;

    errno = 0;
    if (lstat(pathname, &sb) == -1)
    {
        perror("Cannot lstat");
        return NULL;
    }

    if (!S_ISLNK(sb.st_mode))
    {
        printf("Not symlink\n");
        return NULL;
    }

    if (resolved_path == NULL)
    {
        resolved_path = malloc(sizeof(char) * 255);
    }

    bool isSymlink = true;
    printf("%s is symlink %d \n", pathname, isSymlink);

    while (isSymlink)
    {
        size_t size = readlink(pathname, resolved_path, 255);
        if (size == -1)
        {
            return NULL;
        }

        printf("pathname %s - resolve_path %s\n", pathname, resolved_path);

        if (lstat(resolved_path, &sb) == -1)
        {
            printf("%s is not symlink anymore\n", resolved_path);
            return NULL;
        }

        if (!S_ISLNK(sb.st_mode))
        {
            isSymlink = false;
        }
        else
        {
            char *newPathName = calloc(255, sizeof(char));
            memcpy(newPathName, resolved_path, 255);
            printf("newPathName = %s\n", newPathName);
            pathname = newPathName;
            memset(resolved_path, 0, 255);
        }
    }

    printf("%s is symlink %d \n", pathname, isSymlink);

    return resolved_path;
}

int main(int argc, char const *argv[])
{
    char *file = malloc(sizeof(char) * 255);

        realpathImpl(argv[1], file);
    printf("symlink: %s\n", argv[1]);
    printf("real path: %s\n", file);

    return EXIT_SUCCESS;
}
