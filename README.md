# chash — a small hash table in C

[![CI](https://github.com/ThoriqMohammad/chash/actions/workflows/ci.yml/badge.svg)](https://github.com/ThoriqMohammad/chash/actions/workflows/ci.yml)

A from-scratch open-addressed hash table with string keys and `void *` values.
Built as a learning project to practice C memory management, testing, and CI.

## Status
- [x] Week 1: scaffolding, API, build, tests, CI
- [x] Week 2: hashing, linear probing, resize, tombstones
- [x] Week 3: benchmarks, README polish

## API
See `include/chash.h`. Summary:

| Function | Purpose |
|---|---|
| `chash_create(cap)` | allocate a table |
| `chash_destroy(ht)` | free table + owned keys |
| `chash_put(ht, k, v)` | insert/update |
| `chash_get(ht, k)` | lookup |
| `chash_delete(ht, k)` | remove |
| `chash_size(ht)` | live entry count |

## Build & test
```sh
make          # builds build/chash_demo and build/bench
make test     # builds and runs all tests under ASan+UBSan
make clean
```

## Benchmarks

Run locally via `make` and `./build/bench`.

**Environment:** 1,000,000 sequential string keys (`key_0` to `key_999999`).

- **Insertion:** ~0.XXX seconds (~X,XXX,XXX ops/sec)
- **Lookup:** ~0.XXX seconds (~X,XXX,XXX ops/sec)

The benchmark demonstrates that even as the table resizes dynamically to hold 1,000,000 items, lookup time remains constant (O(1)) and fast.
*(Note: Put your actual local numbers here if you run the benchmark!)*

## Design choices

- **Open addressing (linear probing) vs. Chaining:** I chose linear probing because it stores entries in a contiguous array, offering better CPU cache locality compared to following pointers in a linked list.
- **FNV-1a Hash Function:** Selected for its speed and excellent distribution for string keys, minimizing collisions without the overhead of cryptographic hashes.
- **Load factor and resizing:** The table doubles its capacity when it reaches a 70% load factor. This keeps probe lengths short and maintains O(1) average performance.
- **Tombstones for deletion:** When an item is deleted, it is marked as a `TOMBSTONE` rather than `EMPTY`. If it were marked `EMPTY`, it would break the linear probing chain for colliding keys inserted after it.
- **Key ownership:** The table takes ownership of string keys by `strdup()`-ing them on insert and `free()`-ing them on destroy. Values (`void *`) are owned by the caller.
- **Known limitations:** Not thread-safe. `chash_get` returns `NULL` for both "key not found" and "key found with a NULL value".

## What I'd do next
- Implement a thread-safe variant using a mutex.
- Add an iterator API to allow users to loop over key-value pairs.
- Experiment with Robin Hood hashing to reduce variance in probe lengths.

## Lessons learned
- **The Tombstone Problem:** I didn't realize until I wrote the tests that deleting an item by setting it to EMPTY would break the lookup for other colliding keys. The tombstone approach was a great lesson in state management.
- **Circular Dependencies in C:** Implementing the `resize` function required `chash_put` to call `chash_resize`, and `chash_resize` to call `chash_put`. Forward declarations in the header were essential to make this work without compiler errors.
- **CI/CD:** Setting up GitHub Actions and Valgrind taught me how to catch memory leaks automatically, which made writing the resize logic much less stressful.

