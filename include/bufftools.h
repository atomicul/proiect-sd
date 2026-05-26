#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

size_t read_buffer(int64_t *nums, size_t max_count);
bool check_sorted(const int64_t *nums, size_t size);
uint64_t checksum(const int64_t *buff, size_t size);
