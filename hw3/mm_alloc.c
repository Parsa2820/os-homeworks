/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */

#include "mm_alloc.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Your final implementation should comment out this macro. */
#undef MM_USE_STUBS

s_block_ptr heap_start = NULL;

void *mm_malloc(size_t size)
{
#ifdef MM_USE_STUBS
    return c_a_l_l_o_c(1, size);
#else
    if (size == 0)
    {
        return NULL;
    }

    s_block_ptr current_block = heap_start;
    s_block_ptr last_block = NULL;

    while (current_block != NULL)
    {
        if (current_block->_free == 1 && current_block->size >= size)
        {
            split_block(current_block, size);
            current_block->_free = 0;
            return current_block->ptr;
        }
        last_block = current_block;
        current_block = current_block->next;
    }

    s_block_ptr new_block = extend_heap(last_block, size);

    if (new_block == NULL)
    {
        return NULL;
    }

    new_block->_free = 0;
    memset(new_block->ptr, 0, size);
    return new_block->ptr;
#endif
}

void *mm_realloc(void *ptr, size_t size)
{
#ifdef MM_USE_STUBS
    return r_e_a_l_l_o_c(ptr, size);
#else
    if (ptr == NULL)
    {
        return mm_malloc(size);
    }

    if (size == 0)
    {
        mm_free(ptr);
        return NULL;
    }

    s_block_ptr current_block = get_block(ptr);

    if (current_block == NULL)
    {
        return NULL;
    }

    void *new_ptr = mm_malloc(size);

    if (new_ptr == NULL)
    {
        return NULL;
    }

    size_t min = (size < current_block->size) ? size : current_block->size;
    memcpy(new_ptr, ptr, min);
    mm_free(ptr);
    return new_ptr;
#endif
}

void mm_free(void *ptr)
{
#ifdef MM_USE_STUBS
    f_r_e_e(ptr);
#else
    if (ptr == NULL)
    {
        return;
    }

    s_block_ptr block = get_block(ptr);

    if (block != NULL)
    {
        block->_free = 1;
        memset(block->ptr, 0, block->size);
        fusion(block);
    }
#endif
}

void split_block(s_block_ptr b, size_t s)
{
    if (b->size - s > BLOCK_SIZE)
    {
        s_block_ptr new_block = (s_block_ptr)((char *)b + BLOCK_SIZE + s);
        new_block->size = b->size - BLOCK_SIZE - s;
        new_block->_free = 1;
        new_block->next = b->next;
        new_block->prev = b;
        new_block->ptr = (char *)new_block + BLOCK_SIZE;
        // b->next->prev = new_block;
        b->next = new_block;
        b->size = s;
    }
}

void fusion(s_block_ptr b)
{
    if (b == NULL)
    {
        return;
    }

    if (b->prev != NULL && b->prev->_free == 1)
    {
        b->prev->size += b->size + BLOCK_SIZE;
        b->prev->next = b->next;

        if (b->next != NULL)
        {
            b->next->prev = b->prev;
        }

        b = b->prev;
    }

    if (b->next != NULL && b->next->_free == 1)
    {
        b->size += b->next->size + BLOCK_SIZE;
        b->next = b->next->next;

        if (b->next != NULL)
        {
            b->next->prev = b;
        }
    }

    memset(b->ptr, 0, b->size);
}

s_block_ptr get_block(void *p)
{
    s_block_ptr current_block = heap_start;

    while (current_block != NULL)
    {
        if (current_block->ptr == p)
        {
            return current_block;
        }
        current_block = current_block->next;
    }

    return NULL;
}

s_block_ptr extend_heap(s_block_ptr last, size_t s)
{
    s_block_ptr new_block = (s_block_ptr)sbrk(BLOCK_SIZE + s);

    if (new_block == (void *)-1)
    {
        return NULL;
    }

    new_block->size = s;

    if (last == NULL)
    {
        new_block->prev = NULL;
        heap_start = new_block;
    }
    else
    {
        last->next = new_block;
        new_block->prev = last;
    }

    new_block->next = NULL;
    new_block->_free = 1;
    new_block->ptr = new_block + 1;
    return new_block;
}