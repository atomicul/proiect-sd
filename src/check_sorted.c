#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

bool check_sorted(const int64_t *nums, size_t size) {
    for (int i = 0; i<size-1; i++) {
        if (nums[i] > nums[i+1]) {
            return false;
        }
    }
    return true;
}
