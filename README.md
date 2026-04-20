# cmlib

A collection of C utility libraries for dynamic containers, strings, allocators, logging, and path helpers.

## What is included

`cmlib` is split into small static libraries that can be linked independently:

- `cmlib_vector`: dynamic array macros and helpers.
- `cmlib_list`: intrusive doubly linked list with sentinel node.
- `cmlib_string`: owned string (`String`) and string view (`Str`) APIs.
- `cmlib_allocator`: polymorphic memory resources, arena/free-list resources, and resource destructor helpers.
- `cmlib_error`: error types, error-handling macros, and result wrappers.
- `cmlib_logger`: logging helpers built on top of `cmlib_error`.
- `cmlib_scratch_buffer`: global reusable temporary string buffer.
- `cmlib_IO`: filesystem path helpers built on top of `String`.

## Requirements

- CMake `>= 4.3`
- A C23-capable compiler

## Build

```bash
cmake -S . -B build
cmake --build build -j
```

This produces static libraries under `build/<module>/` and test binaries under `build/tests/`.

## Run tests

```bash
ctest --test-dir build --output-on-failure
```

## Using cmlib from CMake

`cmlib` is intended to be consumed with `add_subdirectory(...)` and linked by target.

```cmake
add_subdirectory(path/to/cmlib)

add_executable(my_app main.c)
target_link_libraries(my_app PRIVATE cmlib_vector cmlib_string)
```

Headers are exposed by each library target, so consumers typically include short header names:

```c
#include "Vector.h"
#include "String.h"
```

## Module quick reference

| Module         | Main header                                                                     | CMake target           | Notes                                                                  |
| -------------- | ------------------------------------------------------------------------------- | ---------------------- | ---------------------------------------------------------------------- |
| Error          | `Error.h`, `Result.h`                                                           | `cmlib_error`          | Error codes, error helpers, and result wrappers.                       |
| Allocator      | `Allocator.h`, `Arena.h`, `ArenaResource.h`, `FreeList.h`, `FreeListResource.h` | `cmlib_allocator`      | Polymorphic memory resources and custom allocators.                    |
| Logger         | `Logger.h`                                                                      | `cmlib_logger`         | Logging utilities.                                                     |
| Vector         | `Vector.h`                                                                      | `cmlib_vector`         | Macro-based generic dynamic array.                                     |
| List           | `List.h`                                                                        | `cmlib_list`           | Circular doubly linked list with `ListNode` links and inline payloads. |
| String         | `String.h`                                                                      | `cmlib_string`         | Heap string + slices (`Str`), formatting and replacement helpers.      |
| Scratch Buffer | `Scratch_buf.h`                                                                 | `cmlib_scratch_buffer` | Global temporary string buffer for fast staged formatting.             |
| IO             | `IO.h`                                                                          | `cmlib_IO`             | Path normalization and filename/folder extraction.                     |

## Usage examples

### Vector

```c
#include "Vector.h"

int main(void) {
    int* numbers = vec_ctor(get_malloc_resource(), int);
    if (!numbers) {
        return 1;
    }

    for (int i = 0; i < 10; ++i) {
        if (vec_add(numbers, i) != EVERYTHING_FINE) {
            return 1;
        }
    }

    VEC_ITER(numbers, i) {
        // use numbers[i]
    }

    vec_dtor(numbers);
    return 0;
}
```

### String

```c
#include "String.h"

int main(void) {
    Result_String res = string_ctor(get_calloc_resource(), "Hello");
    if (res.error_code != EVERYTHING_FINE) {
        return 1;
    }

    String s = res.value;
    string_append(&s, ", world");
    string_printf(&s, " (%d)", 42);

    // s.data => "Hello, world (42)"

    string_dtor(&s);
    return 0;
}
```

### List

```c
#include "List.h"

int main(void) {
    List* list = list_ctor(get_malloc_resource());
    if (!list) {
        return 1;
    }

    list_insert_after(list, list_end(list), 10);
    list_insert_before(list, list_end(list), 20);

    LIST_ITER(list, it) {
        int value = *list_node_get_value(it, int);
        // use value
    }

    list_dtor(list);
    return 0;
}
```

Detached list nodes may be moved between lists only when both lists use the
same `MemoryResource`.

### Arena and FreeList

```c
#include "Arena.h"
#include "ArenaResource.h"
#include "FreeList.h"
#include "FreeListResource.h"
#include "Vector.h"
#include "List.h"

int main(void) {
    Arena direct_arena = arena_ctor(128).value;
    int* direct = arena_allocate_type(&direct_arena, int);
    *direct = 42;
    arena_dtor(&direct_arena);

    Arena vector_arena = arena_ctor(4096).value;
    ArenaResource arena_resource = arena_to_resource(&vector_arena);
    // or
    ArenaResource arena_resource = arena_resource_ctor(4096).value;

    int* values = vec_ctor(&arena_resource, int);
    vec_add(values, 42);

    FreeList free_list = free_list_ctor(4096).value;

    void* block = free_list_allocate(&free_list, 64, alignof(max_align_t));
    free_list_deallocate(&free_list, block);

    FreeListResource free_list_resource = free_list_to_resource(&free_list);
    List* list = list_ctor(&free_list_resource);
    // use list

    free_list_resource_dtor(&free_list_resource);
    arena_resource_dtor(&arena_resource);
    return 0;
}
```

`arena_to_resource` and `free_list_to_resource` move allocator state into the
resource wrapper and clear the source object.

## Error-handling conventions

Some non-container APIs use an internal `err` variable and macros from `Error.h`:

- `ERROR_CHECKING()` initializes `err`
- `CHECK_ERROR(expr)` propagates failure via `goto`
- `ERROR_CASE` marks common cleanup path

Result-bearing APIs use `Result_<T>` structures declared with `DECLARE_RESULT_HEADER(T)` in headers and DECLARE_RESULT_SOURCE(T) in one .c file.

## Notes

- There is no install/export packaging yet; consumption is via source subtree (`add_subdirectory`).
- Containers and allocator resources return errors/NULL without logging side effects.
- If you define `DISABLE_LOGGING`, logger output macros become no-ops.

## Additional docs

- [Development Notes](docs/DEVELOPMENT.md) for module dependency and contributor-focused conventions.
