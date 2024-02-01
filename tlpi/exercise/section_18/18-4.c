#if defined(__APPLE__)
        /* Darwin requires this header before including <dirent.h> */
#include <sys/types.h>
#endif
#include <dirent.h>
#include "tlpi_hdr.h"

static void             /* List all files in directory 'dirpath' */
listFiles_r(const char *dirpath)
{
    DIR *dirp;
    struct dirent *dp = malloc(sizeof(struct dirent));
    struct dirent *nextDp;

    Boolean isCurrent;          /* True if 'dirpath' is "." */

    isCurrent = strcmp(dirpath, ".") == 0;

    dirp = opendir(dirpath);
    if (dirp  == NULL) {
        errMsg("opendir failed on '%s'", dirpath);
        return;
    }

    /* For each entry in this directory, print directory + filename */

    for (;;) {
        errno = 0;              /* To distinguish error from end-of-directory */
        // dp = readdir(dirp);
        if (readdir_r(dirp, dp, nextDp) != 0) {
            errExit("readdir_r");
        }
        if (nextDp == NULL)
            break;

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;           /* Skip . and .. */

        if (!isCurrent)
            printf("%s/", dirpath);
        printf("%s\n", dp->d_name);
    }

    if (errno != 0)
        errExit("readdir");

    if (closedir(dirp) == -1)
        errMsg("closedir");
}

int
main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [dir-path...]\n", argv[0]);

    if (argc == 1)              /* No arguments - use current directory */
        listFiles_r(".");
    else
        for (argv++; *argv; argv++)
            listFiles(*argv);

    exit(EXIT_SUCCESS);
}