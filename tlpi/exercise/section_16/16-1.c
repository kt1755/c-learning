#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    char *eaStr;
    char *filePath;
    int opt;
    while ((opt = getopt(argc, argv, ":e::f:r")) != -1)
    {
        switch (opt)
        {
        case 'e':
            eaStr = optarg;
            break;

        case 'f':
            filePath = optarg;
        }
    }



    return 0;
}
