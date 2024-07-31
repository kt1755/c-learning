#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

static pthread_once_t once = PTHREAD_ONCE_INIT;

struct  init_once_t {
    bool *inited;
    pthread_mutex_t *mu;
};

static struct init_once_t init_once = {false, PTHREAD_MUTEX_INITIALIZER};

int one_time_init(struct init_once_t control, void (*init)(void)) {
    if ( pthread_mutex_lock(control.mu) > 0) {
        perror("one_time_init pthread_mutex_lock");
        return 1;
    }

    if (!*control.inited) {
        init();
        control.inited = true;
    }
    if (pthread_mutext_unlock(control.mu) > 0) {
        perror("one_time_init pthread_mutext_unlock");
        return 1;
    };
    return 0;
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
