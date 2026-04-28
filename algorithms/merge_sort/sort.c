#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static inline void merge(
        int64_t *dest,
        const int64_t *src1,
        size_t len1,
        const int64_t *src2,
        size_t len2) {

    int i = 0, j = 0;
    while (i < len1 && j < len2) {
        if (src1[i] < src2[j]) {
            *(dest++) = src1[i++];
        } else {

            *(dest++) = src2[j++];
        }
    }

    for(; i<len1; i++) {
        *(dest++) = src1[i];
    }

    for(; j<len2; j++) {
        *(dest++) = src2[j];
    }
}

void merge_sort(int64_t *nums, size_t size, int64_t *copy_buffer) {
    if (size < 2) {
        return;
    }

    if (size == 2) {
        if (nums[0] > nums[1]) {
            int64_t tmp = nums[0];
            nums[0] = nums[1];
            nums[1] = tmp;
        }
        return;
    }

    const size_t midpoint = size/2;
    merge_sort(nums, midpoint, copy_buffer);
    merge_sort(nums + midpoint, size-midpoint, copy_buffer);

    memcpy(copy_buffer, nums, midpoint * sizeof(int64_t));
    memcpy(copy_buffer + midpoint, nums + midpoint,
           (size - midpoint) * sizeof(int64_t));

    merge(nums, copy_buffer, midpoint, copy_buffer + midpoint, size-midpoint);
}

void sort(int64_t *nums, size_t size) {
    int64_t *copy_buffer = malloc(sizeof(int64_t)*size);

    merge_sort(nums, size, copy_buffer);

    free(copy_buffer);
}

