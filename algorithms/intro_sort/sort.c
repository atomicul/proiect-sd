#include <stddef.h>
#include <stdint.h>

#define INSERTION_THRESHOLD 16

static void insertion_sort(int64_t *nums, size_t size) {
    for (size_t i = 1; i < size; i++) {
        int64_t key = nums[i];
        size_t j = i;
        while (j > 0 && nums[j - 1] > key) {
            nums[j] = nums[j - 1];
            j--;
        }
        nums[j] = key;
    }
}

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

static void heap_sort(int64_t *nums, size_t size) {
    if (size < 2) return;
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

static int floor_log2(size_t n) {
    int log = 0;
    while (n > 1) {
        n >>= 1;
        log++;
    }
    return log;
}

static void intro_sort(int64_t *nums, size_t size, int depth_limit) {
    while (size >= INSERTION_THRESHOLD) {
        if (depth_limit == 0) {
            heap_sort(nums, size);
            return;
        }
        depth_limit--;

        int64_t pivot = nums[size / 2];
        size_t i = 0, j = size - 1;
        while (1) {
            while (nums[i] < pivot) i++;
            while (nums[j] > pivot) j--;
            if (i >= j) break;
            int64_t tmp = nums[i];
            nums[i] = nums[j];
            nums[j] = tmp;
            i++;
            j--;
        }

        size_t left_size = j + 1;
        size_t right_start = j + 1;
        intro_sort(nums, left_size, depth_limit);
        nums += right_start;
        size -= right_start;
    }
    insertion_sort(nums, size);
}

void sort(int64_t *nums, size_t size) {
    if (size < 2) return;
    intro_sort(nums, size, 2 * floor_log2(size));
}
