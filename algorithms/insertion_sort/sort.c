#include <stddef.h>
#include <stdint.h>

void sort(int64_t *nums, size_t size) {
    for (size_t i = 1; i < size; i++) {
        int64_t x = nums[i];
        size_t j = i - 1;

        while (j >= 0 && nums[j] > x) {
            nums[j + 1] = nums[j];
            j--;
        }
        nums[j + 1] = x;
    }
}
