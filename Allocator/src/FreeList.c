#include "FreeList.h"

#include <stdint.h>

#include "Allocator.h"
#include "details/CountingMalloc.h"

DECLARE_RESULT_SOURCE(FreeList);

static constexpr size_t POOL_METADATA_SIZE = sizeof(FreeListMemoryPool);

static FreeListMemoryPool* free_list_pool_ctor(size_t size);
static void free_list_pool_dtor(FreeListMemoryPool* pool);
static void* free_list_pool_allocate(FreeListMemoryPool* pool,
    size_t size,
    size_t alignment);
static bool free_list_pool_check_ptr(FreeListMemoryPool* pool, void* ptr);
static bool free_list_pool_deallocate(FreeListMemoryPool* pool, void* ptr);
static size_t free_list_pool_size(const FreeListMemoryPool* pool);
static size_t free_list_required_block_size(size_t size, size_t alignment);

Result_FreeList free_list_ctor(size_t pool_size)
{
    if (pool_size == 0)
    {
        return Result_FreeList_ctor((FreeList) {}, ERROR_BAD_VALUE);
    }

    FreeListMemoryPool* pool = free_list_pool_ctor(pool_size);

    if (!pool)
    {
        return Result_FreeList_ctor((FreeList) {}, ERROR_NO_MEMORY);
    }

    return Result_FreeList_ctor((FreeList) {.pool = pool}, EVERYTHING_FINE);
}

void free_list_dtor(FreeList* free_list)
{
    if (!free_list)
    {
        return;
    }

    FreeListMemoryPool* cur_pool = free_list->pool;
    while (cur_pool)
    {
        FreeListMemoryPool* next_pool = cur_pool->next_pool;
        free_list_pool_dtor(cur_pool);
        cur_pool = next_pool;
    }

    *free_list = (FreeList) {};
}

void* free_list_allocate(FreeList* free_list, size_t size, size_t alignment)
{
    if (!free_list)
    {
        return NULL;
    }

    FreeListMemoryPool* prev_pool = NULL;
    FreeListMemoryPool* cur_pool = free_list->pool;
    void* allocated = NULL;

    while (cur_pool && !allocated)
    {
        allocated = free_list_pool_allocate(cur_pool, size, alignment);
        prev_pool = cur_pool;
        cur_pool = cur_pool->next_pool;
    }

    if (allocated)
    {
        return allocated;
    }

    if (!free_list->pool)
    {
        return NULL;
    }

    size_t required_size = free_list_required_block_size(size, alignment);
    if (!required_size)
    {
        return NULL;
    }

    size_t new_pool_size = MAX(required_size,
        free_list_pool_size(free_list->pool));
    FreeListMemoryPool* new_pool = free_list_pool_ctor(new_pool_size);
    if (!new_pool)
    {
        return NULL;
    }

    prev_pool->next_pool = new_pool;
    return free_list_pool_allocate(new_pool, size, alignment);
}

void free_list_deallocate(FreeList* free_list, void* ptr)
{
    if (!free_list || !ptr)
    {
        return;
    }

    FreeListMemoryPool* cur_pool = free_list->pool;
    while (cur_pool && !free_list_pool_deallocate(cur_pool, ptr))
    {
        cur_pool = cur_pool->next_pool;
    }
}

void free_list_dump_dot(const FreeList* free_list, FILE* out)
{
    if (!out)
    {
        return;
    }

    fprintf(out, "digraph FreeList {\n");
    fprintf(out, "    rankdir=LR;\n");
    fprintf(out, "    node [shape=record];\n");
    fprintf(out,
        "    free_list [label=\"FreeList|pool=%p\"];\n",
        free_list ? (void*)free_list->pool : NULL);

    if (!free_list)
    {
        fprintf(out, "}\n");
        return;
    }

    size_t pool_index = 0;
    for (const FreeListMemoryPool* pool = free_list->pool; pool;
        pool = pool->next_pool, pool_index++)
    {
        fprintf(out,
            "    pool_%zu [label=\"pool %zu|addr=%p|size=%zu|end=%p\"];\n",
            pool_index,
            pool_index,
            (void*)pool,
            free_list_pool_size(pool),
            pool->pool_end);

        if (pool_index == 0)
        {
            fprintf(out,
                "    free_list -> pool_%zu [label=\"pool\"];\n",
                pool_index);
        }

        if (pool->next_pool)
        {
            fprintf(out,
                "    pool_%zu -> pool_%zu [label=\"next_pool\"];\n",
                pool_index,
                pool_index + 1);
        }

        size_t block_index = 0;
        for (const FreeListFreeBlockHeader* block = pool->next_block; block;
            block = block->next, block_index++)
        {
            fprintf(out,
                "    pool_%zu_free_block_%zu "
                "[label=\"free_block %zu|addr=%p|size=%zu\"];\n",
                pool_index,
                block_index,
                block_index,
                (void*)block,
                block->size);

            if (block_index == 0)
            {
                fprintf(out,
                    "    pool_%zu -> pool_%zu_free_block_%zu "
                    "[label=\"next_block\"];\n",
                    pool_index,
                    pool_index,
                    block_index);
            }

            if (block->next)
            {
                fprintf(out,
                    "    pool_%zu_free_block_%zu -> "
                    "pool_%zu_free_block_%zu [label=\"next\"];\n",
                    pool_index,
                    block_index,
                    pool_index,
                    block_index + 1);
            }
        }
    }

    fprintf(out, "}\n");
}

static FreeListMemoryPool* free_list_pool_ctor(size_t size)
{
    ERROR_CHECKING();

    size = MAX(size, sizeof(FreeListFreeBlockHeader));

    auto pool = (FreeListMemoryPool*)cmlib_details_malloc(
        POOL_METADATA_SIZE + size);

    if (!pool)
    {
        return NULL;
    }

    FreeListFreeBlockHeader* block = (FreeListFreeBlockHeader*)(pool + 1);

    *block = (FreeListFreeBlockHeader) {
        .size = size,
        .next = NULL,
    };

    *pool = (FreeListMemoryPool) {
        .next_pool = NULL,
        .next_block = block,
        .pool_end = (char*)block + size,
    };

    return pool;
}

static void free_list_pool_dtor(FreeListMemoryPool* pool)
{
    free(pool);
}

static void*
free_list_pool_allocate(FreeListMemoryPool* pool, size_t size, size_t alignment)
{
    if (!pool)
    {
        return NULL;
    }

    if (size == 0 || size > UINT32_MAX)
    {
        return NULL;
    }

    alignment = MAX(alignment, alignof(FreeListOccupiedBlockHeader));

    FreeListFreeBlockHeader* prev_block = NULL;
    FreeListFreeBlockHeader* cur_block = pool->next_block;

    FreeListOccupiedBlockHeader* occupied_block = NULL;
    size_t occupied_size = 0;
    size_t occupied_padding = 0;

    FreeListFreeBlockHeader* split_block = NULL;
    size_t split_size = 0;

    while (cur_block)
    {
        char* block_start = (char*)cur_block;
        char* block_end = block_start + cur_block->size;
        char* payload =
            align_ptr(block_start + sizeof(FreeListOccupiedBlockHeader),
                alignment);
        occupied_block =
            (FreeListOccupiedBlockHeader*)(payload - sizeof(*occupied_block));
        occupied_padding = (size_t)((char*)occupied_block - block_start);

        if (payload <= block_end && payload + size <= block_end)
        {
            char* split = align_ptr(payload + size,
                alignof(FreeListFreeBlockHeader));

            if (split + sizeof(FreeListFreeBlockHeader) < block_end)
            {
                occupied_size = split - block_start;
                split_size = block_end - split;
                split_block = (FreeListFreeBlockHeader*)split;
            }
            else
            {
                occupied_size = cur_block->size;
            }
            break;
        }

        prev_block = cur_block;
        cur_block = cur_block->next;
    }

    if (!cur_block)
    {
        return NULL;
    }

    FreeListFreeBlockHeader** prev_block_next = NULL;
    if (prev_block)
    {
        prev_block_next = &prev_block->next;
    }
    else
    {
        prev_block_next = &pool->next_block;
    }

    if (split_block)
    {
        *split_block = (FreeListFreeBlockHeader) {
            .next = cur_block->next,
            .size = split_size,
        };
        *prev_block_next = split_block;
    }
    else
    {
        *prev_block_next = cur_block->next;
    }

    occupied_block->size = (uint32_t)occupied_size;
    occupied_block->padding = (uint32_t)occupied_padding;

    void* allocated = occupied_block + 1;

    return allocated;
}

static bool free_list_pool_check_ptr(FreeListMemoryPool* pool, void* ptr)
{
    if (!pool)
    {
        return false;
    }

    if (!ptr)
    {
        return false;
    }

    void* pool_start = pool + 1;

    // TODO: range/alignment checks still accept interior pointers that were
    // never returned by free_list_pool_allocate.
    // add bit to block sizes that tells it was really allocated
    return pool_start <= ptr && ptr < pool->pool_end;
}
static bool free_list_pool_deallocate(FreeListMemoryPool* pool, void* ptr)
{
    if (!pool)
    {
        return false;
    }

    if (!free_list_pool_check_ptr(pool, ptr))
    {
        return false;
    }

    FreeListOccupiedBlockHeader* header = (FreeListOccupiedBlockHeader*)ptr - 1;
    FreeListFreeBlockHeader* block =
        (FreeListFreeBlockHeader*)((char*)header - header->padding);
    block->size = header->size;
    block->next = pool->next_block;

    pool->next_block = block;

    return true;
}

static size_t free_list_pool_size(const FreeListMemoryPool* pool)
{
    assert(pool);

    ptrdiff_t size = pool->pool_end - (void*)(pool + 1);

    return size;
}

static size_t free_list_required_block_size(size_t size, size_t alignment)
{
    if (alignment == 0 || size == 0)
    {
        return 0;
    }

    alignment = MAX(alignment, alignof(FreeListOccupiedBlockHeader));

    if (size > SIZE_MAX - sizeof(FreeListOccupiedBlockHeader))
    {
        return 0;
    }

    size_t required_size = sizeof(FreeListOccupiedBlockHeader) + size;
    if (required_size > SIZE_MAX - (alignment - 1))
    {
        return 0;
    }

    required_size += alignment - 1;

    if (required_size > (size_t)UINT32_MAX)
    {
        return 0;
    }

    return required_size;
}
