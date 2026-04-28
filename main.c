#include <time.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define BUFF_SIZE 100000

bool check_sorted(const int64_t *nums, size_t size);
void sort(int64_t *nums, size_t size);
uint64_t checksum(const int64_t *buff, size_t size);

int64_t buff[BUFF_SIZE];

int main(void) {
    size_t size = 0;
    for(; size < BUFF_SIZE; size++) {
        if (scanf("%lld", buff+size) == EOF) {
            break;
        }
    }

    const uint64_t before_checksum = checksum(buff, size);

    clock_t begin = clock();
    sort(buff, size);
    clock_t end = clock();

    printf("%lu\n", end-begin);

    if (before_checksum != checksum(buff, size)) {
        return 1;
    }

    if (!check_sorted(buff, size)) {
        return 1;
    }

    return 0;
}
