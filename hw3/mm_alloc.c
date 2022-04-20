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
            current_block->_free = 0;
            return current_block->ptr;
        }
        current_block = current_block->next;
        last_block = current_block;
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
    return NULL;
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
    block->_free = 1;
    // block = fusion(block);
#endif
}

void split_block (s_block_ptr b, size_t s)
{

}

s_block_ptr fusion(s_block_ptr b)
{
    return NULL;
}

s_block_ptr get_block (void *p)
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