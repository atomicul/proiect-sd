#include "bufftools.h"
#include <stdio.h>
#include <inttypes.h>

size_t read_buffer(int64_t *nums, size_t max_count) {
    size_t size = 0;
    for(; size < max_count; size++) {
        if (scanf("%" SCNd64, nums+size) != 1) {
            break;
        }
    }
    return size;
}
