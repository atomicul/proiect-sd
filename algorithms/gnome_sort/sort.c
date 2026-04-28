#include <stddef.h>
#include <stdint.h>

void sort(int64_t *nums, size_t size) {
    size_t i = 0;
    while (i < size) {
        if (i == 0 || nums[i - 1] <= nums[i]) {
            i++;
        } else {
            int64_t tmp = nums[i];
            nums[i] = nums[i - 1];
            nums[i - 1] = tmp;
            i--;
        }
    }
}
