#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

void *threadFunc()
{
    printf("Thread executed!\n");
    return (void *)"threadFunc return";
}

int main(int argc, char const *argv[])
{
    pthread_t thr;
    /* code */
    int s = pthread_create(&thr, NULL, threadFunc, NULL);
    if (s != 0)
    {
        // pthread_exit(pthread_self());
        printf("pthread_create error!\n");
        exit(EXIT_FAILURE);
    }

    printf("Main thread call!\n");
    s = pthread_join(thr, NULL);
    if (s != 0)
    {
        printf("pthread_join error!\n");
        exit(EXIT_FAILURE);
    }

    printf("After thread join\n");

    exit(EXIT_SUCCESS);
}
