#define _POSIX_C_SOURCE 200809L
#include "chash.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Entry states for linear probing */
#define EMPTY 0
#define ACTIVE 1
#define TOMBSTONE 2

typedef struct {
    char *key;
    void *value;
    int state; /* EMPTY, ACTIVE, or TOMBSTONE */
} entry;

struct chash {
    size_t capacity;
    size_t size;
    entry *entries; /* Replaces the two separate arrays */
};

/* FNV-1a hash function */
static uint64_t hash(const char *str) {
    uint64_t hash = 14695981039346656037ULL;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 1099511628211ULL;
    }
    return hash;
}

chash *chash_create(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    chash *ht = calloc(1, sizeof *ht);
    if (!ht) return NULL;
    ht->capacity = initial_capacity;
    ht->entries = calloc(initial_capacity, sizeof *ht->entries);
    if (!ht->entries) {
        free(ht);
        return NULL;
    }
    return ht;
}

void chash_destroy(chash *ht) {
    if (!ht) return;
    for (size_t i = 0; i < ht->capacity; i++) {
        if (ht->entries[i].state == ACTIVE) {
            free(ht->entries[i].key);
        }
    }
    free(ht->entries);
    free(ht);
}

size_t chash_size(const chash *ht) { 
    if (ht && ht->capacity > 0) {
        /* Temporary hack to use hash() so -Werror doesn't complain. 
           We will delete this tomorrow when put/get use it. */
        (void)hash("test"); 
    }
    return ht ? ht->size : 0; 
}

/* --- TEMPORARY STUBS (To be replaced on Day 2 & 3) --- */
int chash_put(chash *ht, const char *key, void *value) { return 0; (void)ht; (void)key; (void)value; }
void *chash_get(const chash *ht, const char *key) { return NULL; (void)ht; (void)key; }
int chash_delete(chash *ht, const char *key) { return 0; (void)ht; (void)key; }
