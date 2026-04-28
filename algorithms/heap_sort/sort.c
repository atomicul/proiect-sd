#include <stddef.h>
#include <stdint.h>

static void sift_down(int64_t *nums, size_t start, size_t end) {
    size_t root = start;
    while (root * 2 + 1 <= end) {
        size_t child = root * 2 + 1;
        if (child + 1 <= end && nums[child] < nums[child + 1]) {
            child++;
        }
        if (nums[root] >= nums[child]) {
            return;
        }
        int64_t tmp = nums[root];
        nums[root] = nums[child];
        nums[child] = tmp;
        root = child;
    }
}

void sort(int64_t *nums, size_t size) {
    if (size < 2) {
        return;
    }

    for (size_t start = size / 2; start-- > 0; ) {
        sift_down(nums, start, size - 1);
    }

    for (size_t end = size - 1; end > 0; end--) {
        int64_t tmp = nums[0];
        nums[0] = nums[end];
        nums[end] = tmp;
        sift_down(nums, 0, end - 1);
    }
}
