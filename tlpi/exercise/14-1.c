#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>


int main(int argc, char *argv[])
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

    printf("Generate %s file in %s\n", numOfFileStr, filePath);

    struct stat *directoryStat;
    stat(filePath, directoryStat);

    int numOfFile = atoi(numOfFileStr);
    int fileNameNum = 0;
    char *fullPath = malloc(255 *sizeof(char));
    char *fileName = malloc(255 *sizeof(char));
    char *fileArr[numOfFile];
    int generatedNum[numOfFile];
    for (int i = 0; i < numOfFile; i++) {
        generatedNum[i] = i+1;
    }

    srand(time(NULL));
    // Shuffle the array to create random order
    int tmp = 0;
    for (int i = 0; i < numOfFile; i++)
    {
        int j = rand() % numOfFile + 1;

        tmp =  generatedNum[j];
        generatedNum[j] = generatedNum[i];
        generatedNum[i] = tmp;
    }



    for (int i = 0; i < numOfFile; i++)
    {
        
        fileNameNum = generatedNum[i];
        sprintf(fileName, "x%06d", fileNameNum); //segment fault
        sprintf(fullPath,"%s/%s", filePath, fileName);

        // int fd = open(fullPath, O_CREAT | O_RDWR);
        // if (fd == -1) {
        //     perror("cannot open file");
        //     exit(EXIT_FAILURE);
        // }

        // ftruncate(fd, 1);
        // close(fd);

        // fileArr[i] = fullPath;

        printf("Random at %d: %s\n", i, fullPath);
    }

    
    /* code */
    return 0;
}

