#include <time.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define BUFF_SIZE    100000
#define TIMEOUT_SECS 5

bool check_sorted(const int64_t *nums, size_t size);
void sort(int64_t *nums, size_t size);
uint64_t checksum(const int64_t *buff, size_t size);

int64_t buff[BUFF_SIZE];

struct sort_ctx {
    int64_t *buff;
    size_t   size;
    bool     done;
    clock_t  ticks;
    pthread_mutex_t lock;
    pthread_cond_t  cond;
};

static clock_t get_cpu_ticks(void) {
    struct timespec ts;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    return (clock_t)((long long)ts.tv_sec * CLOCKS_PER_SEC
                   + (long long)ts.tv_nsec * CLOCKS_PER_SEC / 1000000000LL);
}

static void *sort_worker(void *arg) {
    struct sort_ctx *ctx = arg;

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

int main(void) {
    size_t size = 0;
    for(; size < BUFF_SIZE; size++) {
        if (scanf("%lld", buff+size) == EOF) {
            break;
        }
    }

    const uint64_t before_checksum = checksum(buff, size);

    struct sort_ctx ctx = {
        .buff = buff,
        .size = size,
        .done = false,
        .lock = PTHREAD_MUTEX_INITIALIZER,
        .cond = PTHREAD_COND_INITIALIZER,
    };

    struct timespec deadline;
    clock_gettime(CLOCK_REALTIME, &deadline);
    deadline.tv_sec += TIMEOUT_SECS;

    pthread_t thread;
    if (pthread_create(&thread, NULL, sort_worker, &ctx)) {
        perror("Failed to create thread");
        return 2;
    }

    pthread_mutex_lock(&ctx.lock);
    int rc = 0;
    while (!ctx.done && rc != ETIMEDOUT) {
        rc = pthread_cond_timedwait(&ctx.cond, &ctx.lock, &deadline);
    }

    if (!ctx.done) {
        _exit(1); // OS exit preferred here so both threads die together
                  // The usual userspace cleanup might have, for example,
                  // destroyed the main stack while the worker thread is
                  // still reading the ctx object
    }
    pthread_mutex_unlock(&ctx.lock);
    pthread_join(thread, NULL);

    printf("%lu\n", ctx.ticks);

    if (before_checksum != checksum(buff, size)) {
        return 1;
    }

    if (!check_sorted(buff, size)) {
        return 1;
    }

    return 0;
}
