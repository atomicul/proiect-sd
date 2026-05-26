#include "sortworker.h"
#include <time.h>

static clock_t get_cpu_ticks(void) {
    struct timespec ts;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    return (clock_t)((long long)ts.tv_sec * CLOCKS_PER_SEC
                   + (long long)ts.tv_nsec * CLOCKS_PER_SEC / 1000000000LL);
}

void *sort_worker(void *arg) {
    SortContext *ctx = (SortContext*)arg;

    clock_t begin = get_cpu_ticks();
    sort(ctx->buff, ctx->size);
    clock_t end = get_cpu_ticks();

    pthread_mutex_lock(&ctx->lock);
    ctx->ticks = end - begin;
    ctx->done = true;
    pthread_cond_signal(&ctx->cond);
    pthread_mutex_unlock(&ctx->lock);
    return NULL;
}
