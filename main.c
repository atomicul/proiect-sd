#include "sortworker.h"
#include "bufftools.h"

#include <time.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define BUFF_SIZE    10000007
#define TIMEOUT_SECS 5

int64_t buff[BUFF_SIZE];

int main(void) {
    const size_t size = read_buffer(buff, BUFF_SIZE);

    if (size >= BUFF_SIZE) {
        fprintf(stderr, "Input length too big, exceeds buffer limit of %d\n", BUFF_SIZE);
        return 2;
    }

    const uint64_t before_checksum = checksum(buff, size);

    SortContext ctx = {
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

    printf("%ld\n", (long)ctx.ticks);

    if (before_checksum != checksum(buff, size)) {
        return 1;
    }

    if (!check_sorted(buff, size)) {
        return 1;
    }

    return 0;
}
