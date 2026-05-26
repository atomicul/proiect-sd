#include <stddef.h>
#include <stdint.h>

uint64_t checksum(const int64_t *buff, size_t size) {
    uint64_t sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += (uint64_t)buff[i];
    }
    return sum;
}
