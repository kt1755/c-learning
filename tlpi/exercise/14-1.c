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

    int numOfFile = atoi(numOfFileStr);
    int fileNameNum = 0;
    char *fullPath;
    char *fileName;

    srand(time(NULL));
    for (int i = 0; i < numOfFile; i++)
    {
        fileNameNum = rand() % numOfFile;
        sprintf(fileName, "x%6d", fileNameNum);
        sprintf(fullPath,"%s/%s", filePath, fileName);

        int fd = open(fullPath, O_CREAT | O_RDWR);
    }

    /* code */
    return 0;
}
