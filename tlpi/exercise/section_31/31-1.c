#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

static pthread_once_t once = PTHREAD_ONCE_INIT;

struct  init_once_t {
    bool inited;
    pthread_mutex_t mu;
};

static struct init_once_t init_once;

int one_time_init(struct init_once_t control, void (*init)(void)) {

    return 0;
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
