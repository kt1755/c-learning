#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// 

int main(int argc, char *argv[])
{

    int opt;
    char *numOfLineStr;
    int numOfLine = 0;
    while ((opt = getopt(argc, argv, ":n:")) != -1)
    {
        switch (opt)
        {
        case 'n':
        numOfLineStr = optarg;
            break;
        }
    }

    atoi(numOfLineStr);

    /* code */
    return 0;
}
