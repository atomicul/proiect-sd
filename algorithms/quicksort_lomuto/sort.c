#include <stddef.h>
#include <stdint.h>

int partition(int64_t *nums, int st, int dr) {
    int64_t piv = nums[dr]; 
    int i = st; 

    for (int j = st; j <= dr - 1; j++) {
        if (nums[j] < piv) {
            int64_t aux = nums[i];
            nums[i] = nums[j];
            nums[j] = aux;
            i++;
        }
    }
    
    int64_t aux = nums[i];
    nums[i] = nums[dr];
    nums[dr] = aux;
    return i;
}

void quickSort(int64_t *nums, int st, int dr) {
    if (st < dr) {
        int pi = partition(nums, st, dr);
        quickSort(nums, st, pi - 1);
        quickSort(nums, pi + 1, dr);
    }
}

void sort(int64_t *nums, size_t size) {
    quickSort(nums, 0, size - 1);
}