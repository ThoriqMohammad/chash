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

static void chash_resize(chash *ht, size_t new_capacity);

/* FNV-1a hash function */
static uint64_t hash(const char *str) {
    uint64_t hash = 14695981039346656037ULL;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 1099511628211ULL;
    }
    return hash;
}

static void chash_resize(chash *ht, size_t new_capacity) {
    entry *old_entries = ht->entries;
    size_t old_capacity = ht->capacity;

    /* Allocate new array */
    ht->entries = calloc(new_capacity, sizeof *ht->entries);
    if (!ht->entries) return; /* Handle allocation failure (silently for now) */
    
    ht->capacity = new_capacity;
    ht->size = 0; /* Reset size. The put() calls below will increment it again. */

    /* Re-insert all ACTIVE entries from the old array */
    for (size_t i = 0; i < old_capacity; i++) {
        if (old_entries[i].state == ACTIVE) {
            chash_put(ht, old_entries[i].key, old_entries[i].value);
            free(old_entries[i].key); /* put() duplicates the key, so free the old one */
        }
    }
    
    free(old_entries); /* Free the old array */
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
    /* Check load factor (70%) */
    if (ht->size * 10 >= ht->capacity * 7) {
        chash_resize(ht, ht->capacity * 2);
    }

    size_t index = hash(key) % ht->capacity;
    entry *first_tombstone = NULL;

    for (size_t i = 0; i < ht->capacity; i++) {
        size_t current = (index + i) % ht->capacity;
        entry *e = &ht->entries[current];

        if (e->state == ACTIVE && strcmp(e->key, key) == 0) {
            /* Found the key! Update the value. */
            e->value = value;
            return 1;
        }
        
        if (e->state == TOMBSTONE && first_tombstone == NULL) {
            /* Remember the first tombstone we see, but KEEP LOOKING */
            first_tombstone = e; 
        }
        
        if (e->state == EMPTY) {
            /* Hit an empty slot. We know the key isn't in the table.
               If we saw a tombstone, insert there. Otherwise, insert here. */
            entry *target = (first_tombstone != NULL) ? first_tombstone : e;
            
            target->key = strdup(key);
            if (!target->key) return 0;
            target->value = value;
            target->state = ACTIVE;
            ht->size++;
            return 1;
        }
    }
    return 0; /* Table is full */
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

int chash_delete(chash *ht, const char *key) {
    size_t index = hash(key) % ht->capacity;
    
    for (size_t i = 0; i < ht->capacity; i++) {
        size_t current = (index + i) % ht->capacity;
        entry *e = &ht->entries[current];
        
        if (e->state == EMPTY) {
            return 0; /* Hit an empty slot, key isn't here */
        } else if (e->state == ACTIVE && strcmp(e->key, key) == 0) {
            free(e->key);
            e->key = NULL;
            e->value = NULL;
            e->state = TOMBSTONE; /* Mark as deleted, don't make it EMPTY */
            ht->size--;
            return 1;
        }
    }
    return 0;
}
