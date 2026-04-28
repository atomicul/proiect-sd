#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void sort(int64_t *nums, size_t size) {
    if (size < 2) return;

    int64_t *buf = malloc(size * sizeof(int64_t));
    if (!buf) return;

    int64_t *src = nums;
    int64_t *dst = buf;

    for (int byte = 0; byte < 8; byte++) {
        size_t count[256] = {0};
        unsigned shift = (unsigned)byte * 8;
        uint8_t flip = (byte == 7) ? 0x80 : 0;

        for (size_t i = 0; i < size; i++) {
            uint8_t key = (uint8_t)((uint64_t)src[i] >> shift) ^ flip;
            count[key]++;
        }

        size_t total = 0;
        for (int i = 0; i < 256; i++) {
            size_t c = count[i];
            count[i] = total;
            total += c;
        }

        for (size_t i = 0; i < size; i++) {
            uint8_t key = (uint8_t)((uint64_t)src[i] >> shift) ^ flip;
            dst[count[key]++] = src[i];
        }

        int64_t *tmp = src;
        src = dst;
        dst = tmp;
    }

    if (src != nums) {
        memcpy(nums, src, size * sizeof(int64_t));
    }

    free(buf);
}
