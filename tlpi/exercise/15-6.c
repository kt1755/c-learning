#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    char *filePath;
    int opt;
    while ((opt = getopt(argc, argv, ":f:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            filePath = optarg;
            break;
        }
    }

    printf("File to be granted: %s \n", filePath);

    struct stat statbuf;
    if (stat(filePath, &statbuf) == -1)
    {
        // TODO handle error more careful
        perror("Cannot check stat of file");
        return -1;
    }

    bool enableX = false;

    int mode = S_IRUSR | S_IRGRP | S_IROTH;
    if (S_ISDIR(statbuf.st_mode) ||
        (statbuf.st_mode) & X_OK == X_OK ||
        (statbuf.st_mode >> 3) & X_OK == X_OK ||
        (statbuf.st_mode >> 6) & X_OK == X_OK)
    {
        mode = mode | S_IXGRP | S_IXOTH | S_IXUSR;
    }

    mode = statbuf.st_mode | mode;

    if (chmod(filePath, mode) == -1)
    {
        perror("Chạy chmod error");
    }

    /* code */
    return 0;
}
