#define _POSIX_C_SOURCE 200809L
#include "chash.h"
#include <stdlib.h>
#include <string.h>

struct chash {
    size_t capacity;
    size_t size;
    /* Week 1: fixed array, no hashing.
     * Week 2: replace with entries[] of {char *key; void *value; int state;} */
    char **keys;
    void **values;
};

chash *chash_create(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    chash *ht = calloc(1, sizeof *ht);
    if (!ht) return NULL;
    ht->capacity = initial_capacity;
    ht->keys = calloc(initial_capacity, sizeof *ht->keys);
    ht->values = calloc(initial_capacity, sizeof *ht->values);
    if (!ht->keys || !ht->values) {
        free(ht->keys);
        free(ht->values);
        free(ht);
        return NULL;
    }
    return ht;
}

void chash_destroy(chash *ht) {
    if (!ht) return;
    for (size_t i = 0; i < ht->capacity; i++) free(ht->keys[i]);
    free(ht->keys);
    free(ht->values);
    free(ht);
}

/* Week 1 placeholder: linear scan, no hashing. Replaced in week 2. */
int chash_put(chash *ht, const char *key, void *value) {
    for (size_t i = 0; i < ht->capacity; i++) {
        if (ht->keys[i] && strcmp(ht->keys[i], key) == 0) {
            ht->values[i] = value;
            return 1;
        }
    }
    for (size_t i = 0; i < ht->capacity; i++) {
        if (!ht->keys[i]) {
            ht->keys[i] = strdup(key);
            if (!ht->keys[i]) return 0;
            ht->values[i] = value;
            ht->size++;
            return 1;
        }
    }
    return 0; /* full — week 2 adds resize */
}

void *chash_get(const chash *ht, const char *key) {
    for (size_t i = 0; i < ht->capacity; i++)
        if (ht->keys[i] && strcmp(ht->keys[i], key) == 0) return ht->values[i];
    return NULL;
}

int chash_delete(chash *ht, const char *key) {
    for (size_t i = 0; i < ht->capacity; i++) {
        if (ht->keys[i] && strcmp(ht->keys[i], key) == 0) {
            free(ht->keys[i]);
            ht->keys[i] = NULL;
            ht->values[i] = NULL;
            ht->size--;
            return 1;
        }
    }
    return 0;
}

size_t chash_size(const chash *ht) { return ht ? ht->size : 0; }
