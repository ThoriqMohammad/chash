#include "chash.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ITEMS 1000000

int main(void) {
    printf("Benchmarking chash with %d items...\n", NUM_ITEMS);
    
    /* Create table with initial capacity of 1000 */
    chash *ht = chash_create(1000);
    if (!ht) {
        fprintf(stderr, "Failed to create table\n");
        return 1;
    }

    /* Generate keys like "key_0", "key_1", ... */
    char key[32];
    int dummy_value = 42;

    /* 1. Benchmark Insertion */
    clock_t start = clock();
    for (int i = 0; i < NUM_ITEMS; i++) {
        snprintf(key, sizeof(key), "key_%d", i);
        chash_put(ht, key, &dummy_value);
    }
    clock_t end = clock();
    double insert_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Inserted %d items in %.3f seconds (%.0f ops/sec)\n", 
           NUM_ITEMS, insert_time, NUM_ITEMS / insert_time);

    /* 2. Benchmark Lookup */
    start = clock();
    for (int i = 0; i < NUM_ITEMS; i++) {
        snprintf(key, sizeof(key), "key_%d", i);
        chash_get(ht, key);
    }
    end = clock();
    double lookup_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Looked up %d items in %.3f seconds (%.0f ops/sec)\n", 
           NUM_ITEMS, lookup_time, NUM_ITEMS / lookup_time);

    chash_destroy(ht);
    return 0;
}
