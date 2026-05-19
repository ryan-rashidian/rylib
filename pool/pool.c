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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Pool {
    uint8_t *buffer;
    void    *next_block;
    size_t   block_size;
    size_t   n_blocks;
} Pool;

static void link_blocks(Pool *p)
{
    // "Intrusive free list structure"
    // Tracking pointers are stored inside empty memory blocks.
    size_t n = p->n_blocks;
    size_t size = p->block_size;

    for (size_t i = 0; i < n - 1; i++) {
        void **curr_block = (void **)(p->buffer + (i * size));
        void *next_block = p->buffer + ((i + 1) * size);
        *curr_block = next_block;
    }
    void **last_block = (void **)(p->buffer + ((n - 1) * size));
    *last_block = NULL;
}

Pool *pool_create(size_t block_size, size_t n_blocks)
{
    if (block_size < 8) {
        fprintf(stderr, "pool_create: block_size must be at least 8 bytes");
        return NULL;
    }
    Pool *p = malloc(sizeof(Pool));
    if (p == NULL) return NULL;

    uint8_t *buffer = malloc(block_size * n_blocks);
    if (buffer == NULL) {
        free(p);
        return NULL;
    }
    
    p->buffer = buffer;
    p->next_block = buffer;
    p->block_size = block_size;
    p->n_blocks = n_blocks;
    link_blocks(p);

    return p;
}

void pool_destroy(Pool *p)
{
    if (p == NULL) return;

    free(p->buffer);
    free(p);
}

void *pool_alloc(Pool *p)
{
    if (p->next_block == NULL) return NULL;
    void *block = p->next_block;

    p->next_block = *(void **)p->next_block;

    return block;
}

void pool_free(Pool *p, void *block)
{
    void **free_block = (void **)block;
    void *pool_head = p->next_block;

    *free_block = pool_head;
    p->next_block = block;
}

