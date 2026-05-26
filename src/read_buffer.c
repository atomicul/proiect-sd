#include "bufftools.h"
#include <stdio.h>

size_t read_buffer(int64_t *nums, size_t max_count) {
    size_t size = 0;
    for(; size < max_count; size++) {
        if (scanf("%lld", nums+size) == EOF) {
            break;
        }
    }
    return size;
}
