#include <libgen.h>
#include <stdio.h>
#include <pthread.h>

#define MAX_ERROR_LEN 1 << 12

static pthread_key_t key;
static pthread_once_t once = PTHREAD_ONCE_INIT;

static void destructBuffer(void *buf)
{
    free(buf);
}

static void createKey(void)
{
    int s;
    /* Maximum length of string in per-thread buffer returned by strerror() */
    /* Free thread-specific data buffer */
    /* One-time key creation function */
    s = pthread_key_create(&key, destructBuffer);
    if (s != 0)
    {
        perror("pthread_key_create");
    }
}

char *basename_r(char *pathname)
{
    int s = pthread_once(&once, createKey);
    if (s != 0)
    {
        perror("pthread_once");
        return NULL;
    }

    char *buf = pthread_getspecific(key);
    if (buf == NULL)
    {
        buf = malloc(MAX_ERROR_LEN);
        if (buf == NULL)
            errExit("malloc");
        s = pthread_setspecific(key, buf);
        if (s != 0)
            errExitEN(s, "pthread_setspecific");
    }

    buf = basename(pathname);
    return buf;
}
