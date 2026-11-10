# chash — a small hash table in C

[![CI](https://github.com/ThoriqMohammad/chash/actions/workflows/ci.yml/badge.svg)](https://github.com/ThoriqMohammad/chash/actions/workflows/ci.yml)

A from-scratch open-addressed hash table with string keys and `void *` values.
Built as a learning project to practice C memory management, testing, and CI.

## Status
- [x] Week 1: scaffolding, API, build, tests, CI
- [ ] Week 2: hashing, linear probing, resize, tombstones
- [ ] Week 3: benchmarks, README polish

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
make          # builds build/chash_demo
make test     # builds and runs all tests under ASan+UBSan
make clean
