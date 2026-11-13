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

size_t chash_size(const chash *ht) { return ht ? ht->size : 0; }

int chash_put(chash *ht, const char *key, void *value) {
    /* Note: We will add resizing here tomorrow (Day 4) */
    size_t index = hash(key) % ht->capacity;
    
    for (size_t i = 0; i < ht->capacity; i++) {
        size_t current = (index + i) % ht->capacity;
        entry *e = &ht->entries[current];
        
        if (e->state == EMPTY || e->state == TOMBSTONE) {
            /* Found an empty slot (or a reusable tombstone) */
            e->key = strdup(key);
            if (!e->key) return 0;
            e->value = value;
            e->state = ACTIVE;
            ht->size++;
            return 1;
        } else if (e->state == ACTIVE && strcmp(e->key, key) == 0) {
            /* Key already exists, update its value */
            e->value = value;
            return 1;
        }
    }
    return 0; /* Table is completely full */
}

void *chash_get(const chash *ht, const char *key) {
    size_t index = hash(key) % ht->capacity;
    
    for (size_t i = 0; i < ht->capacity; i++) {
        size_t current = (index + i) % ht->capacity;
        entry *e = &ht->entries[current];
        
        if (e->state == EMPTY) {
            /* We hit an empty slot. If the key was in the table, 
               it would have been placed before this empty slot. */
            return NULL; 
        } else if (e->state == ACTIVE && strcmp(e->key, key) == 0) {
            return e->value; /* Found it! */
        }
        /* If it's a TOMBSTONE, just keep looking */
    }
    return NULL; /* Looked through the whole table and didn't find it */
}

int chash_delete(chash *ht, const char *key) { return 0; (void)ht; (void)key; }
