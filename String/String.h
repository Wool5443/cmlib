#ifndef CMLIB_STRING_H_
#define CMLIB_STRING_H_

#include "Allocator.h"
#include "Result.h"

#define CMLIB_EMPTY_STRING ((String) {})

typedef struct String
{
    MemoryResource* memory_resource;
    char* data;
    size_t size;
    size_t capacity;
} String;

typedef struct Str
{
    const char* data;
    size_t size;
} Str;

DECLARE_RESULT_HEADER(String);
DECLARE_RESULT_HEADER(Str);

#define STR_LITERAL(string) ((Str) {.data = string, .size = sizeof(string) - 1})

Str str_ctor(const char* string);

Str str_ctor_size(const char* string, size_t size);

Str str_ctor_string(String string);

int str_compare(Str lhs, Str rhs);

void str_print(Str string, FILE* out);

Result_Str str_slice(Str string, size_t start, size_t end);

Result_String string_ctor(void* memory_resource, const char* string);

Result_String string_ctor_capacity(void* memory_resource, size_t capacity);

Result_String string_ctor_str(void* memory_resource, Str string);

void string_dtor(String* this);

Result_String string_copy(void* memory_resource, String string);

ErrorCode string_printf(String* this, const char* format, ...)
    __attribute__((format(__printf__, 2, 3)));

ErrorCode string_vprintf(String* this, const char* format, va_list args)
    __attribute__((format(__printf__, 2, 0)));

Result_String string_ctor_printf(void* memory_resource, const char* format, ...)
    __attribute__((format(__printf__, 2, 3)));

Result_String string_ctor_vprintf(void* memory_resource,
    const char* format,
    va_list args) __attribute__((format(__printf__, 2, 0)));

ErrorCode string_append_char(String* this, char ch);

ErrorCode string_append_string(String* this, String string);

ErrorCode string_append(String* this, const char* string);

ErrorCode string_append_str(String* this, Str string);

ErrorCode string_replace_all(String* this, Str from, Str to);

Result_Str string_slice(String this, size_t start, size_t end);

void string_clear(String* this);

int string_compare(String lhs, String rhs);

ErrorCode string_realloc(String* this, size_t new_capacity);

#endif // CMLIB_STRING_H_
