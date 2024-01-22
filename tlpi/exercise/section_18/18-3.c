#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>

char *realpathImpl(const char *pathname, char *resolved_path)
{
    struct stat sb;

    errno = 0;
    if (lstat(pathname, &sb) == -1)
    {
        perror("Cannot lstat");
        return NULL;
    }

    if  (!S_ISLNK (sb.st_mode)) {
        printf("Not symlink\n");
        return NULL;
    }

    if (resolved_path == NULL)
    {
        resolved_path == malloc(sizeof(char) * _PC_PATH_MAX);
    }

    bool isSymlink = true;

    while (isSymlink)
    {
        size_t size = readlink(pathname, resolved_path, _PC_PATH_MAX);
        if (size == -1)
        {
            return NULL;
        }

        pathname = resolved_path;
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
