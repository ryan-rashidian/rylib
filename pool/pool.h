/*
 * pool - Pool allocator
 *
 * Wraps malloc/realloc/free from stdlib.h into a pool allocator.
 * The allocator is initialized with a given block size and number of blocks.
 * When `pool_alloc` is called, a pointer to a single block of memory is
 * returned back; when `pool_free` is called, given a block pointer as an
 * argument, it will add that block back to its pool of available memory.
 * Every call of `pool_create` should have a corresponding call of
 * `pool_destroy` for clean-up.
 *
 * Important rules:
 *     - Do not exceed the block size that is preset for a pool
 *     - Do not use pointers after freeing them, or free them more than once
 *
 *  Not following the above rules can lead to nasty bugs. This allocator has
 *  no guard rails built in.
 */

#ifndef POOL_H
#define POOL_H

#include <stddef.h>

typedef struct Pool Pool;

Pool *pool_create(size_t block_size, size_t n_blocks);
void  pool_destroy(Pool *p);

void *pool_alloc(Pool *p);
void  pool_free(Pool *p, void *block);

#endif

