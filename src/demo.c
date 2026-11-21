#include "chash.h"
#include <stdio.h>

int main(void) {
    printf("Creating hash table...\n");
    chash *ht = chash_create(16);
    
    int value1 = 100;
    int value2 = 200;
    
    printf("Inserting 'apple' and 'banana'...\n");
    chash_put(ht, "apple", &value1);
    chash_put(ht, "banana", &value2);
    
    printf("Size of table: %zu\n", chash_size(ht));
    
    int *found = chash_get(ht, "apple");
    if (found) {
        printf("Found 'apple' with value: %d\n", *found);
    }
    
    printf("Deleting 'apple'...\n");
    chash_delete(ht, "apple");
    
    if (chash_get(ht, "apple") == NULL) {
        printf("Successfully deleted 'apple'.\n");
    }
    
    printf("Destroying table.\n");
    chash_destroy(ht);
    return 0;
}
