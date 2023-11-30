#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{

    char *numOfFileStr;
    char *filePath;
    int opt;
    while ((opt = getopt(argc, argv, ":n:p:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            numOfFileStr = optarg;
            break;

            case 'p':
            filePath = optarg;
        }
    }

    /* code */
    return 0;
}
