#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void sort(int64_t *nums, size_t size) {
    while (true) {
        bool sorted = true;

        for (int i = 0; i+1<size; i++) {
            if (nums[i] > nums[i+1]) {
                int64_t tmp = nums[i];
                nums[i] = nums[i+1];
                nums[i+1] = tmp;
                sorted = false;
            }
        }

        if (sorted) {
            return;
        }
    }
}
