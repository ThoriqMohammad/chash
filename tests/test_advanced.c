#include "chash.h"
#include "test.h"

static void test_collision(void) {
    chash *ht = chash_create(4); /* Small capacity to force collisions */
    int a = 1, b = 2, c = 3;
    
    /* Put 3 items into a table of size 4. 
       They will likely hash to the same few slots and force linear probing. */
    chash_put(ht, "key1", &a);
    chash_put(ht, "key2", &b);
    chash_put(ht, "key3", &c);
    
    ASSERT(chash_get(ht, "key1") == &a);
    ASSERT(chash_get(ht, "key2") == &b);
    ASSERT(chash_get(ht, "key3") == &c);
    chash_destroy(ht);
}

static void test_resize(void) {
    chash *ht = chash_create(2); /* Tiny capacity to force resizing */
    int a = 1;
    
    /* Insert 5 items. The table will hit 70% capacity and resize multiple times. */
    chash_put(ht, "one", &a); 
    chash_put(ht, "two", &a); 
    chash_put(ht, "three", &a); 
    chash_put(ht, "four", &a); 
    chash_put(ht, "five", &a);
    
    ASSERT_EQ_INT(chash_size(ht), 5);
    
    /* Prove the old keys are still findable after moving to the new array */
    ASSERT(chash_get(ht, "one") == &a);
    ASSERT(chash_get(ht, "five") == &a);
    chash_destroy(ht);
}

static void test_tombstone(void) {
    chash *ht = chash_create(4);
    int a = 1;
    chash_put(ht, "A", &a);
    chash_put(ht, "B", &a);
    
    /* Delete A. It should become a TOMBSTONE, not EMPTY. */
    chash_delete(ht, "A");
    ASSERT(chash_get(ht, "A") == NULL);
    ASSERT(chash_get(ht, "B") == &a);
    
    /* Reuse the tombstone slot with a new key */
    chash_put(ht, "C", &a);
    ASSERT(chash_get(ht, "C") == &a);
    
    chash_destroy(ht);
}

int main(void) {
    test_collision();
    test_resize();
    test_tombstone();
    TEST_SUMMARY();
}
