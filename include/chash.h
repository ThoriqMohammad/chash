#ifndef CHASH_H
#define CHASH_H

#include <stddef.h>

/* Opaque handle. Callers never see the struct internals. */
typedef struct chash chash;

/* Create a hash table with the given initial capacity.
 * Returns NULL on allocation failure or capacity == 0. */
chash *chash_create(size_t initial_capacity);

/* Destroy the table and free every key it owns.
 * Values are NOT freed — caller owns value lifetime. */
void chash_destroy(chash *ht);

/* Insert or update. Returns 1 on success, 0 on allocation failure.
 * The table copies the key (caller retains ownership of the input). */
int chash_put(chash *ht, const char *key, void *value);

/* Look up. Returns the stored value, or NULL if absent.
 * Note: NULL is ambiguous with "stored a NULL value" — document this. */
void *chash_get(const chash *ht, const char *key);

/* Remove. Returns 1 if the key was present, 0 otherwise. */
int chash_delete(chash *ht, const char *key);

/* Number of live entries. */
size_t chash_size(const chash *ht);

#endif /* CHASH_H */
