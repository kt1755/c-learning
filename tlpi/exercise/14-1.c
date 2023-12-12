#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

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

    char *fileArr[numOfFile];
    char *fileLinearArray[numOfFile];
    int generatedNum[numOfFile];
    for (int i = 0; i < numOfFile; i++)
    {
        generatedNum[i] = i + 1;

        char *fullPath = malloc(255 * sizeof(char));
        char *fileName = malloc(255 * sizeof(char));

        sprintf(fileName, "x%06d", i + 1); // segment fault
        sprintf(fullPath, "%s/%s", filePath, fileName);

        fileArr[i] = fullPath;
        fileLinearArray[i] = fullPath;
    }

    // Shuffle the array to create random order
    char *tmp;
    srand(time(NULL));
    for (int i = 0; i < numOfFile; i++)
    {
        int j = rand() % numOfFile;

        tmp = fileArr[j];
        fileArr[j] = fileArr[i];
        fileArr[i] = tmp;

        printf("Random at %d: %s\n", i, fileArr[i]);
    }

    for (int i = 0; i < numOfFile; i++)
    {
        printf("After shuffle at %d: %s\n", i, fileArr[i]);
    }

    for (int i = 0; i < numOfFile; i++)
    {
        int fd = open(fileArr[i], O_CREAT | O_RDWR, S_IWOTH | S_IROTH | S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            perror("cannot open file");
            exit(EXIT_FAILURE);
        }

        // ftruncate(fd, 1);
        write(fd, "A", 1);
        close(fd);
    }
    sync();

    struct timespec startDeleteFileTime;
    struct timespec endDeleteFileTime;
    if (clock_gettime(CLOCK_REALTIME, &startDeleteFileTime) == -1)
    {
        perror("Không thể lấy được thời điểm bắt đầu delete\n");
        exit(EXIT_FAILURE);
    }

    if (startDeleteFileTime.tv_sec == 0) {
        printf("startDeleteFileTime NULL\n");
    }

    printf("Start remove all file\n");
    for (int i = 0; i < numOfFile; i++)
    {
        printf("Remove file at %d: %s\n", i, fileLinearArray[i]);
        remove(fileLinearArray[i]);
    }
    sync();

    if (clock_gettime(CLOCK_REALTIME, &endDeleteFileTime) == -1)
    {
        printf("Error gettimeofday with endDeleteFileTime");
        perror("Không thể lấy được thời điểm kết thúc delete\n");
        exit(EXIT_FAILURE);
    }
    if (endDeleteFileTime.tv_sec == 0) {
        printf("endDeleteFileTime NULL\n");
    }

    printf("Delete all file in random created order: %5.2f nanoseconds\n", (endDeleteFileTime.tv_sec - startDeleteFileTime.tv_sec) + (endDeleteFileTime.tv_nsec - startDeleteFileTime.tv_nsec)/1e9);
    printf("OKE\n");

    /* code */
    return 0;
}
