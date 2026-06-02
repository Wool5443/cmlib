#include "Tests.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Allocator.h"
#include "Arena.h"
#include "ArenaResource.h"
#include "Error.h"
#include "FreeList.h"
#include "FreeListResource.h"
#include "IO.h"
#include "List.h"
#include "Pool.h"
#include "String.h"
#include "Vector.h"
#include "details/CountingMalloc.h"

typedef bool (*test_func)(void);

typedef struct TestEntry
{
    test_func func;
    const char* name;
} TestEntry;

#define make_test_entry(func) ((TestEntry) {func, STRINGIFY(func)})

#define ASSERT_TRUE(expr)                                                      \
    do                                                                         \
    {                                                                          \
        bool val = expr;                                                       \
        result &= val;                                                         \
        if (!val)                                                              \
        {                                                                      \
            printf("Assertion %s on line %d failed: %d\n",                     \
                #expr,                                                         \
                __LINE__,                                                      \
                val);                                                          \
        }                                                                      \
    } while (0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))
#define ASSERT_NULL(ptr) ASSERT_TRUE((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != NULL)
#define ASSERT_NO_ERROR(expr) ASSERT_TRUE((expr) == EVERYTHING_FINE)
#define ASSERT_ERROR(expr) ASSERT_TRUE((expr) != EVERYTHING_FINE)
#define ASSERT_STRING_EQUAL(s1, s2)                                            \
    ASSERT_TRUE(str_compare(str_ctor(s1), str_ctor(s2)) == 0)
#define ASSERT_STR_EQUAL(s1, s2) ASSERT_TRUE(str_compare((s1), (s2)) == 0)

static size_t find_max_int_count(size_t size)
{
    FreeList* free_list = free_list_ctor(size);
    if (!free_list)
    {
        return 0;
    }

    size_t prev_allocations = standard_allocations_count;

    size_t max_ints = 0;

    while (prev_allocations == standard_allocations_count)
    {
        free_list_allocate_type(free_list, int);
        max_ints++;
    }
    max_ints--;

    free_list_dtor(free_list);

    return max_ints;
}

static bool test_arena(void)
{
    bool result = true;

    constexpr size_t int_count = 1000;

    size_t prev_allocations = standard_allocations_count;

    ASSERT_NULL(arena_ctor(0));

    Arena* arena = arena_ctor(int_count * sizeof(int));

    ASSERT_NOT_NULL(arena);
    ASSERT_TRUE(prev_allocations + 1 == standard_allocations_count );
    prev_allocations = standard_allocations_count;

    ASSERT_NULL(arena_allocate(arena, 100, 0));
    ASSERT_NULL(arena_allocate(arena, 0, 100));

    int* p1 = arena_allocate_type(arena, int);
    int* p2 = arena_allocate_type(arena, int);
    ASSERT_NOT_NULL(p1);
    ASSERT_NOT_NULL(p2);

    ASSERT_TRUE((char*)p2 - (char*)p1 == sizeof(*p1));

    for (size_t i = 0; i < int_count - 2; i++)
    {
        ASSERT_NOT_NULL(arena_allocate_type(arena, int));
    }

    ASSERT_TRUE(arena_allocate(arena, 1, 1) == NULL);
    ASSERT_TRUE(prev_allocations == standard_allocations_count);

    arena_flush(arena);
    for (size_t i = 0; i < int_count; i++)
    {
        ASSERT_NOT_NULL(arena_allocate_type(arena, int));
    }

    arena_dtor(arena);

    return result;
}

static bool test_free_list(void)
{
    constexpr size_t free_list_size = 20000;

    bool result = true;

    size_t prev_allocations = standard_allocations_count;

    ASSERT_NULL(free_list_ctor(0));

    FreeList* free_list = free_list_ctor(free_list_size);

    ASSERT_NOT_NULL(free_list);
    ASSERT_TRUE(prev_allocations + 1 == standard_allocations_count);

    ASSERT_NULL(free_list_allocate(free_list, UINT32_MAX, 8));

    prev_allocations = standard_allocations_count;
    int* p1 = free_list_allocate_type(free_list, int);
    int* p2 = free_list_allocate_type(free_list, int);

    ASSERT_NOT_NULL(p1);
    ASSERT_NOT_NULL(p2);
    ASSERT_TRUE(prev_allocations == standard_allocations_count);

    free_list_deallocate(free_list, p1);
    free_list_deallocate(free_list, p2);

    size_t int_count = find_max_int_count(free_list_size);

    int** ptrs = (int**)calloc(int_count, sizeof(*ptrs));

    prev_allocations = standard_allocations_count;
    for (size_t i = 0; i < int_count; i++)
    {
        ptrs[i] = free_list_allocate_type(free_list, int);
    }

    for (size_t i = 0; i < int_count / 7; i++)
    {
        size_t random_index = rand() % int_count;
        for (size_t j = 0; j < 5; j++)
        {
            free_list_deallocate(free_list, ptrs[random_index + j]);
            ptrs[random_index + j] = NULL;
        }
    }
    ASSERT_TRUE(prev_allocations == standard_allocations_count);

    free(ptrs);

    void* aligned1 = free_list_allocate(free_list, 1, 32);
    void* aligned2 = free_list_allocate(free_list, 1, 128);

    ASSERT_NOT_NULL(aligned1);
    ASSERT_NOT_NULL(aligned2);
    ASSERT_TRUE((uintptr_t)aligned1 % 32 == 0);
    ASSERT_TRUE((uintptr_t)aligned2 % 128 == 0);

    int* last_ptr = NULL;
    for (size_t i = 0; i < 100'000; i++)
    {
        if (rand() % 5 == 1)
        {
            free_list_deallocate(free_list, last_ptr);
            last_ptr = NULL;
        }
        last_ptr =
            free_list_allocate(free_list, sizeof(double), alignof(double));
        ASSERT_NOT_NULL(last_ptr);
    }

    free_list_dtor(free_list);

    return result;
}

static bool test_free_list_dump(void)
{
    bool result = true;

    FreeList* free_list = free_list_ctor(128);
    ASSERT_NOT_NULL(free_list);

    int* ptrs[20] = {};

    for (size_t i = 0; i < ARRAY_SIZE(ptrs); i++)
    {
        ptrs[i] = free_list_allocate_type(free_list, int);
        ASSERT_NOT_NULL(ptrs[i]);
    }

    FILE* dump = fopen("dump_before.dot", "w");
    ASSERT_NOT_NULL(dump);
    free_list_dump_dot(free_list, dump);
    if (dump)
    {
        fclose(dump);
    }
    dump = NULL;

    free_list_deallocate(free_list, ptrs[5]);
    ptrs[8] = NULL;
    free_list_deallocate(free_list, ptrs[10]);
    ptrs[17] = NULL;
    free_list_deallocate(free_list, ptrs[3]);
    ptrs[3] = NULL;

    dump = fopen("dump_after.dot", "w");
    ASSERT_NOT_NULL(dump);
    free_list_dump_dot(free_list, dump);
    if (dump)
    {
        fclose(dump);
    }

    free_list_dtor(free_list);

    return result;
}

static bool test_pool(void)
{
    bool result = true;

    constexpr size_t count = 200;

    Pool* pool = pool_ctor(count);
    ASSERT_NOT_NULL(pool);

    size_t prev_allocations = standard_allocations_count;

    int* ptrs[count] = {};
    for (size_t i = 0; i < count; i++)
    {
        ptrs[i] = pool_allocate_type(pool, int);
        ASSERT_NOT_NULL(ptrs[i]);
    }
    ASSERT_TRUE(prev_allocations + 1 == standard_allocations_count);

    prev_allocations = standard_allocations_count;

    for (size_t i = 0; i < 15; i++)
    {
        pool_deallocate(pool, ptrs[rand() % count]);
    }

    for (size_t i = 0; i < 15; i++)
    {
        ASSERT_NOT_NULL(pool_allocate_type(pool, int));
    }
    ASSERT_TRUE(prev_allocations == standard_allocations_count);

    struct Node
    {
        struct Node *prev, *next;
        char data[100];
    };

    for (size_t i = 0; i < count; i++)
    {
        ASSERT_NOT_NULL(pool_allocate_type(pool, struct Node));
    }
    ASSERT_TRUE(prev_allocations + 1 == standard_allocations_count);

    pool_dtor(pool);
    return result;
}

static bool test_list(void)
{
    bool result = true;

    size_t prev_allocations = standard_allocations_count;
    list_ctor(list, get_malloc_resource());
    ASSERT_TRUE(prev_allocations == standard_allocations_count);

    ASSERT_TRUE(list_begin(list) == list_end(list));

    ListNode* ten = list_insert_after(list, list_end(list), 10);
    ListNode* twenty = list_insert_after(list, ten, 20);
    ListNode* five = list_insert_before(list, ten, 5);

    ASSERT_NOT_NULL(ten);
    ASSERT_NOT_NULL(twenty);
    ASSERT_NOT_NULL(five);

    int expected[] = {5, 10, 20};
    size_t i = 0;
    LIST_ITER(list, node)
    {
        ASSERT_TRUE(i < ARRAY_SIZE(expected));
        ASSERT_TRUE(*list_node_get_value(node, int) == expected[i]);
        i++;
    }
    ASSERT_TRUE(i == ARRAY_SIZE(expected));

    list_erase(list, ten);

    int expected_after_erase[] = {5, 20};
    i = 0;
    LIST_ITER(list, node)
    {
        ASSERT_TRUE(i < ARRAY_SIZE(expected_after_erase));
        ASSERT_TRUE(*list_node_get_value(node, int) == expected_after_erase[i]);
        i++;
    }
    ASSERT_TRUE(i == ARRAY_SIZE(expected_after_erase));

    list_dtor(list);

    return result;
}

static bool test_resource_conversions(void)
{
    bool result = true;

    Arena* arena = arena_ctor(128);
    ASSERT_NOT_NULL(arena);
    ArenaResource arena_resource = arena_to_resource(arena);

    int* arena_value = arena_resource.base.allocate(&arena_resource.base,
        sizeof(int),
        alignof(int));
    ASSERT_NOT_NULL(arena_value);
    if (arena_value)
    {
        *arena_value = 42;
        ASSERT_TRUE(*arena_value == 42);
    }
    arena_resource_dtor(&arena_resource);

    FreeList* free_list = free_list_ctor(128);
    ASSERT_NOT_NULL(free_list);
    FreeListResource free_list_resource = free_list_to_resource(free_list);

    int* free_list_value =
        free_list_resource.base.allocate(&free_list_resource.base,
            sizeof(int),
            alignof(int));
    ASSERT_NOT_NULL(free_list_value);
    if (free_list_value)
    {
        *free_list_value = 24;
        ASSERT_TRUE(*free_list_value == 24);
        free_list_resource.base.deallocate(&free_list_resource.base,
            free_list_value);
    }
    free_list_resource_dtor(&free_list_resource);

    return result;
}

static bool test_string(void)
{
    bool result = true;

    const char* test_string = "hello, this is some test string!";
    Str test_str = str_ctor(test_string);

    Result_String string_res = string_ctor(get_malloc_resource(), test_string);
    ASSERT_NO_ERROR(string_res.error_code);
    String* s = &string_res.value;

    ASSERT_STRING_EQUAL(s->data, test_string);

    string_dtor(&string_res.value);

    string_res = string_ctor_str(get_malloc_resource(), test_str);
    ASSERT_NO_ERROR(string_res.error_code);
    ASSERT_STR_EQUAL(str_ctor_string(*s), test_str);
    string_dtor(&string_res.value);

    string_res = string_ctor_capacity(get_malloc_resource(), 1);
    const char* correct = "Did you know, that 25 * 31 = 775 = 307, and also "
                          "NOTHING, I LIED\n";
    ASSERT_NO_ERROR(string_printf(s,
        "Did you know, that %d * %d = %d = %x, and also %s\n",
        25,
        31,
        25 * 31,
        25 * 31,
        "NOTHING, I LIED"));
    ASSERT_STRING_EQUAL(s->data, correct);
    string_dtor(&string_res.value);

    string_res = string_ctor_capacity(get_malloc_resource(), 1);
    ASSERT_NO_ERROR(string_append_char(s, 'a'));
    ASSERT_TRUE(s->data[0] == 'a');
    ASSERT_NO_ERROR(string_append(s, "const char*"));
    ASSERT_STRING_EQUAL(s->data, "aconst char*");

    Result_String new_string_res = string_copy(get_malloc_resource(), *s);
    ASSERT_NO_ERROR(new_string_res.error_code);
    String* ns = &new_string_res.value;
    ASSERT_STRING_EQUAL(ns->data, s->data);
    ASSERT_NO_ERROR(string_append_string(s, *ns));
    ASSERT_STRING_EQUAL(s->data, "aconst char*aconst char*");
    string_dtor(ns);

    string_replace_all(s, str_ctor("const"), str_ctor("ABOBA"));
    ASSERT_STRING_EQUAL(s->data, "aABOBA char*aABOBA char*");

    Result_Str slice_res = string_slice(*s, 7, 17);
    ASSERT_NO_ERROR(slice_res.error_code);
    ASSERT_STRING_EQUAL(slice_res.value.data, "char*aABOB");

    string_dtor(s);

    return result;
}

static bool test_vector(void)
{
    bool result = true;

    int* vec = vec_ctor(get_malloc_resource(), int);
    ASSERT_NOT_NULL(vec);

    for (size_t i = 0; i < 10'000; i++)
    {
        ASSERT_NO_ERROR(vec_add(vec, i));
    }

    for (int i = 10'000 - 1; i >= 0; i--)
    {
        ASSERT_TRUE(vec_pop(vec) == i);
    }

    vec_reserve(vec, 15);
    size_t prev_allocations = standard_allocations_count;
    for (size_t i = 0; i < 15; i++)
    {
        vec_add(vec, i);
    }
    ASSERT_TRUE(prev_allocations == standard_allocations_count);

    VEC_ITER(vec, i)
    {
        ASSERT_TRUE(vec[i] == (int)i);
    }

    vec_dtor(vec);

    return result;
}

static bool test_io(void)
{
    bool result = true;

    Result_String string_res = string_ctor_capacity(get_malloc_resource(), 16);
    ASSERT_NO_ERROR(string_res.error_code);

    String string = string_res.value;

    ASSERT_NO_ERROR(read_file(&string, "tests/huge_file.txt"));

    Str orig = str_ctor(text);
    ASSERT_TRUE(str_compare(orig, str_ctor_string(string)) == 0);

    string_dtor(&string);

    return result;
}

int main()
{
    TestEntry tests[] = {
        make_test_entry(test_arena),
        make_test_entry(test_free_list),
        make_test_entry(test_free_list_dump),
        make_test_entry(test_pool),
        make_test_entry(test_list),
        make_test_entry(test_resource_conversions),
        make_test_entry(test_string),
        make_test_entry(test_vector),
        make_test_entry(test_io)
    };

    for (size_t i = 0; i < ARRAY_SIZE(tests); i++)
    {
        bool res = tests[i].func();
        if (res)
        {
            printf("Test #%zu %s passed!\n", i + 1, tests[i].name);
        }
        else
        {
            printf("Test #%zu %s did not pass!\n", i + 1, tests[i].name);
        }
    }
}
