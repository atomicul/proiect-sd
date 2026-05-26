#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

// This will be linked with the test candidate, and acts as the interface for
// all algorithms
void sort(int64_t *nums, size_t size);

typedef struct {
    int64_t *buff;
    size_t   size;
    bool     done;
    clock_t  ticks;
    pthread_mutex_t lock;
    pthread_cond_t  cond;
} SortContext;

void *sort_worker(void *arg);
