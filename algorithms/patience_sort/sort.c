#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int64_t *data;
    size_t size;
    size_t cap;
} Pile;

static void pile_push(Pile *p, int64_t val) {
    if (p->size >= p->cap) {
        p->cap = p->cap ? p->cap * 2 : 8;
        p->data = realloc(p->data, p->cap * sizeof(int64_t));
    }
    p->data[p->size++] = val;
}

typedef struct {
    int64_t value;
    size_t pile;
} HeapEntry;

static void heap_sift_up(HeapEntry *heap, size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (heap[parent].value <= heap[idx].value) {
            return;
        }
        HeapEntry tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

static void heap_sift_down(HeapEntry *heap, size_t size, size_t idx) {
    while (1) {
        size_t left = idx * 2 + 1;
        size_t right = idx * 2 + 2;
        size_t smallest = idx;
        if (left < size && heap[left].value < heap[smallest].value) {
            smallest = left;
        }
        if (right < size && heap[right].value < heap[smallest].value) {
            smallest = right;
        }
        if (smallest == idx) return;
        HeapEntry tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

void sort(int64_t *nums, size_t size) {
    if (size < 2) return;

    Pile *piles = NULL;
    size_t num_piles = 0, piles_cap = 0;
    int64_t *pile_tops = NULL;

    for (size_t i = 0; i < size; i++) {
        int64_t val = nums[i];
        size_t lo = 0, hi = num_piles;
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (pile_tops[mid] >= val) hi = mid;
            else lo = mid + 1;
        }
        if (lo == num_piles) {
            if (num_piles >= piles_cap) {
                piles_cap = piles_cap ? piles_cap * 2 : 8;
                piles = realloc(piles, piles_cap * sizeof(Pile));
                pile_tops = realloc(pile_tops, piles_cap * sizeof(int64_t));
            }
            piles[num_piles] = (Pile){0};
            num_piles++;
        }
        pile_push(&piles[lo], val);
        pile_tops[lo] = val;
    }

    HeapEntry *heap = malloc(num_piles * sizeof(HeapEntry));
    size_t heap_size = 0;
    for (size_t i = 0; i < num_piles; i++) {
        heap[heap_size].value = piles[i].data[piles[i].size - 1];
        heap[heap_size].pile = i;
        heap_sift_up(heap, heap_size);
        heap_size++;
    }

    for (size_t out_idx = 0; out_idx < size; out_idx++) {
        HeapEntry top = heap[0];
        nums[out_idx] = top.value;
        piles[top.pile].size--;
        if (piles[top.pile].size > 0) {
            heap[0].value = piles[top.pile].data[piles[top.pile].size - 1];
            heap_sift_down(heap, heap_size, 0);
        } else {
            heap_size--;
            if (heap_size > 0) {
                heap[0] = heap[heap_size];
                heap_sift_down(heap, heap_size, 0);
            }
        }
    }

    for (size_t i = 0; i < num_piles; i++) {
        free(piles[i].data);
    }
    free(piles);
    free(pile_tops);
    free(heap);
}
