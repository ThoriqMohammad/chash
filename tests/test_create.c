#include "chash.h"
#include "test.h"

static void test_create_destroy(void) {
    chash *ht = chash_create(16);
    ASSERT(ht != NULL);
    ASSERT_EQ_INT(chash_size(ht), 0);
    chash_destroy(ht);
}

static void test_put_get(void) {
    chash *ht = chash_create(16);
    int x = 42;
    ASSERT(chash_put(ht, "answer", &x) == 1);
    ASSERT_EQ_INT(chash_size(ht), 1);
    ASSERT(chash_get(ht, "answer") == &x);
    ASSERT(chash_get(ht, "missing") == NULL);
    chash_destroy(ht);
}

static void test_put_update(void) {
    chash *ht = chash_create(16);
    int a = 1, b = 2;
    chash_put(ht, "k", &a);
    chash_put(ht, "k", &b);
    ASSERT_EQ_INT(chash_size(ht), 1);
    ASSERT(chash_get(ht, "k") == &b);
    chash_destroy(ht);
}

int main(void) {
    test_create_destroy();
    test_put_get();
    test_put_update();
    TEST_SUMMARY();
}
