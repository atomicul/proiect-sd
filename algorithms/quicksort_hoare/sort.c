#include <stddef.h>
#include <stdint.h>
#include <stddef.h>

static void quick_sort(int64_t *nums, ptrdiff_t lo, ptrdiff_t hi) {
    while (lo < hi) {
        int64_t pivot = nums[lo + (hi - lo) / 2];
        ptrdiff_t i = lo, j = hi;
        while (i <= j) {
            while (nums[i] < pivot) i++;
            while (nums[j] > pivot) j--;
            if (i <= j) {
                int64_t tmp = nums[i];
                nums[i] = nums[j];
                nums[j] = tmp;
                i++;
                j--;
            }
        }

        if (j - lo < hi - i) {
            if (lo < j) quick_sort(nums, lo, j);
            lo = i;
        } else {
            if (i < hi) quick_sort(nums, i, hi);
            hi = j;
        }
    }
}

void sort(int64_t *nums, size_t size) {
    if (size > 1) {
        quick_sort(nums, 0, (ptrdiff_t)size - 1);
    }
}
