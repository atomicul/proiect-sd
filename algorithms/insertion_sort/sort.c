#include <stddef.h>
#include <stdint.h>

void sort(int64_t *nums, size_t size) {
    for (int i = 1; i < size; i++) {
        int x = nums[i];
        int j = i - 1;

        while (j >= 0 && nums[j] > x) {
            nums[j + 1] = nums[j];
            j--;
        }
        nums[j + 1] = x;
    }
}