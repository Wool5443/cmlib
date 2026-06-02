#include "Pool.h"

#include <stdint.h>

#include "Allocator.h"
#include "details/CountingMalloc.h"

typedef struct PoolFreeBlock PoolFreeBlock;
struct PoolFreeBlock
{
    PoolFreeBlock* next;
};

typedef struct SubPool SubPool;
struct SubPool
{
    PoolFreeBlock* free_block;
};

typedef struct SecondLevelPool SecondLevelPool;
struct SecondLevelPool
{
    SubPool base;
    SecondLevelPool* next_pool_same_size;
};

typedef struct FirstLevelPool FirstLevelPool;
struct FirstLevelPool
{
    SecondLevelPool base;
    FirstLevelPool* next_pool_dif_size;
    size_t elem_size;
};

struct Pool
{
    size_t count;
    FirstLevelPool* pool;
};

typedef struct FindResult
{
    FirstLevelPool *prev, *cur;
} FindResult;

static SubPool* sub_pool_ctor(size_t count, size_t elem_size, size_t meta_size);
static void* sub_pool_allocate(SubPool* pool);
static void sub_pool_deallocate(SubPool* pool, void* ptr);

static SecondLevelPool*
second_level_ctor(size_t count, size_t elem_size, size_t meta_size);
static FirstLevelPool* first_level_ctor(size_t count, size_t elem_size);

static void*
second_level_allocate(SecondLevelPool* pool, size_t count, size_t elem_size);
static void* first_level_allocate(FirstLevelPool* pool, size_t count);

static SubPool* find_sub_pool_containing_ptr(Pool* pool, void* ptr);

static FindResult find_first_level_by_size(FirstLevelPool* pool,
    size_t elem_size);

// static void subpool_deallocate()

Pool* pool_ctor(size_t count)
{
    Pool* pool = cmlib_details_malloc(sizeof(Pool));
    if (!pool)
    {
        return NULL;
    }

    *pool = (Pool) {
        .count = count,
        .pool = NULL,
    };

    return pool;
}

void pool_dtor(Pool* pool)
{
    cmlib_details_free(pool);
}

void* pool_allocate(Pool* pool, size_t size, size_t alignment)
{
    if (!pool || size == 0 || alignment == 0)
    {
        return NULL;
    }

    alignment = MAX(alignment, alignof(PoolFreeBlock));
    size_t aligned_size = align_size(size, alignment);

    if (!pool->pool)
    {
        pool->pool = first_level_ctor(pool->count, aligned_size);
        return first_level_allocate(pool->pool, pool->count);
    }

    FindResult needed_pool = find_first_level_by_size(pool->pool, aligned_size);

    if (!needed_pool.cur)
    {
        needed_pool.cur = first_level_ctor(pool->count, aligned_size);
        if (needed_pool.prev)
        {
            needed_pool.prev->next_pool_dif_size = needed_pool.cur;
        }
    }

    FirstLevelPool* fp = needed_pool.cur;
    if (!fp)
    {
        return NULL;
    }

    return first_level_allocate(fp, pool->count);
}

void pool_deallocate(Pool* pool, void* ptr)
{
    if (!pool || !ptr)
    {
        return;
    }

    SubPool* sp = find_sub_pool_containing_ptr(pool, ptr);

    if (!sp)
    {
        return;
    }

    sub_pool_deallocate(sp, ptr);
}

static SubPool* sub_pool_ctor(size_t count, size_t elem_size, size_t meta_size)
{
    size_t size = count * elem_size;
    SubPool* pool = (SubPool*)cmlib_details_malloc(size + meta_size);
    if (!pool)
    {
        return NULL;
    }

    char* start = (char*)pool + meta_size;
    char* end = start + size;

    char* cur = start;

    while (cur < end)
    {
        ((PoolFreeBlock*)cur)->next = (PoolFreeBlock*)(cur + elem_size);
        cur += elem_size;
    }
    cur -= elem_size;
    ((PoolFreeBlock*)cur)->next = NULL;

    *pool = (SubPool) {
        .free_block = (PoolFreeBlock*)start,
    };

    return pool;
}

static void* sub_pool_allocate(SubPool* pool)
{
    if (!pool)
    {
        return NULL;
    }

    if (!pool->free_block)
    {
        return NULL;
    }

    void* ret = pool->free_block;
    pool->free_block = pool->free_block->next;

    return ret;
}

static void sub_pool_deallocate(SubPool* pool, void* ptr)
{
    if (!pool)
    {
        return;
    }
    PoolFreeBlock* fblock = (PoolFreeBlock*)ptr;
    fblock->next = pool->free_block;
    pool->free_block = fblock;
}

static SecondLevelPool*
second_level_ctor(size_t count, size_t elem_size, size_t meta_size)
{
    SecondLevelPool* pool = (SecondLevelPool*)sub_pool_ctor(count,
        elem_size,
        meta_size ? meta_size : sizeof(*pool));

    if (!pool)
    {
        return NULL;
    }

    pool->next_pool_same_size = NULL;

    return pool;
}

static FirstLevelPool* first_level_ctor(size_t count, size_t elem_size)
{
    FirstLevelPool* pool =
        (FirstLevelPool*)second_level_ctor(count, elem_size, sizeof(*pool));

    if (!pool)
    {
        return NULL;
    }

    pool->next_pool_dif_size = NULL;
    pool->elem_size = elem_size;

    return pool;
}

static void*
second_level_allocate(SecondLevelPool* pool, size_t count, size_t elem_size)
{
    SecondLevelPool* prev = NULL;
    SecondLevelPool* cur = pool;

    void* ret = sub_pool_allocate(&cur->base);

    while (cur && !ret)
    {
        prev = cur;
        cur = (SecondLevelPool*)cur->next_pool_same_size;
        ret = sub_pool_allocate((SubPool*)cur);
    }

    if (ret)
    {
        return ret;
    }

    SecondLevelPool* new_pool = second_level_ctor(count, elem_size, 0);

    ret = sub_pool_allocate((SubPool*)new_pool);
    prev->next_pool_same_size = new_pool;

    return ret;
}

static void* first_level_allocate(FirstLevelPool* pool, size_t count)
{
    return second_level_allocate(&pool->base, count, pool->elem_size);
}

static SubPool* find_sub_pool_containing_ptr(Pool* pool, void* ptr)
{
    FirstLevelPool* cur_f = pool->pool;

    char* p = (char*)ptr;

    while (cur_f)
    {
        char* start = (char*)(cur_f + 1);
        char* end = start + pool->count * cur_f->elem_size;

        if (start <= p && p < end)
        {
            return (SubPool*)cur_f;
        }

        SecondLevelPool* cur_s =
            (SecondLevelPool*)cur_f->base.next_pool_same_size;

        while (cur_s)
        {
            char* start = (char*)(cur_s + 1);
            char* end = start + pool->count * cur_f->elem_size;

            if (start <= p && p < end)
            {
                return (SubPool*)cur_s;
            }

            cur_s = cur_s->next_pool_same_size;
        }

        cur_f = cur_f->next_pool_dif_size;
    }

    return NULL;
}

static FindResult find_first_level_by_size(FirstLevelPool* pool,
    size_t elem_size)
{
    FirstLevelPool* prev = NULL;
    FirstLevelPool* cur = pool;

    while (cur && cur->elem_size != elem_size)
    {
        prev = cur;
        cur = cur->next_pool_dif_size;
    }

    return (FindResult) {prev, cur};
}
