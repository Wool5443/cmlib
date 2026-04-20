/**
 * @file String.h
 * @brief Owned string and borrowed string slice utilities.
 */

#ifndef CMLIB_STRING_H_
#define CMLIB_STRING_H_

#include <stdarg.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __STDC_VERSION__ < 202311L
#include <stdbool.h>
#endif

#include "Allocator.h"
#include "Result.h"

/**
 * @brief Zero-initialized `String` value.
 */
#define CMLIB_EMPTY_STRING ((String) {})

/**
 * @brief Mutable owned byte string.
 */
typedef struct String
{
    MemoryResource* memory_resource; /**< Resource used for storage. */
    char* data;                      /**< Owned byte buffer. */
    size_t size;                     /**< Byte count excluding terminator. */
    size_t capacity;                 /**< Capacity excluding terminator. */
} String;

/**
 * @brief Borrowed string slice.
 */
typedef struct Str
{
    const char* data; /**< Borrowed bytes. */
    size_t size;      /**< Byte count. */
} Str;

DECLARE_RESULT_HEADER(String);
DECLARE_RESULT_HEADER(Str);

/**
 * @brief Creates a borrowed slice from a string literal.
 */
#define STR_LITERAL(cmlib_macroarg_string)                                     \
    ((Str) {.data = cmlib_macroarg_string,                                    \
        .size = sizeof(cmlib_macroarg_string) - 1})

/**
 * @brief Creates a borrowed slice from a null-terminated C string.
 * @param string Null-terminated string, or NULL.
 * @return Borrowed slice, or empty slice for NULL.
 */
Str str_ctor(const char* string);

/**
 * @brief Creates a borrowed slice from pointer and size.
 * @param string Byte pointer, or NULL.
 * @param size Byte count.
 * @return Borrowed slice, or empty slice for NULL or zero size.
 */
Str str_ctor_size(const char* string, size_t size);

/**
 * @brief Creates a borrowed view into an owned string.
 * @param string Owned string.
 * @return Borrowed slice over string data.
 */
Str str_ctor_string(String string);

/**
 * @brief Compares two borrowed slices lexicographically.
 * @param lhs Left slice.
 * @param rhs Right slice.
 * @return `memcmp`-style sign value.
 */
int str_compare(Str lhs, Str rhs);

/**
 * @brief Writes a borrowed slice to a stream.
 * @param string Slice to write.
 * @param out Destination stream.
 */
void str_print(Str string, FILE* out);

/**
 * @brief Returns sub-slice `[start, end)`.
 * @param string Source slice.
 * @param start Start index.
 * @param end End index.
 * @return Slice result, or `ERROR_BAD_ARGS`.
 */
Result_Str str_slice(Str string, size_t start, size_t end);

/**
 * @brief Creates an owned copy of a null-terminated C string.
 * @param memory_resource Resource pointer; must be non-NULL.
 * @param string Source string, or NULL.
 * @return Owned string, empty string for NULL, or error.
 */
Result_String string_ctor(void* memory_resource, const char* string);

/**
 * @brief Creates an empty owned string with explicit capacity.
 * @param memory_resource Resource pointer; must be non-NULL.
 * @param capacity Usable byte capacity excluding trailing null.
 * @return Owned empty string, or error.
 */
Result_String string_ctor_capacity(void* memory_resource, size_t capacity);

/**
 * @brief Creates an owned copy of a borrowed slice.
 * @param memory_resource Resource pointer; must be non-NULL.
 * @param string Source slice.
 * @return Owned string, empty string for empty input, or error.
 */
Result_String string_ctor_str(void* memory_resource, Str string);

/**
 * @brief Releases owned storage and clears string state.
 * @param this String pointer; NULL is accepted.
 */
void string_dtor(String* this);

/**
 * @brief Deep-copies an owned string into a resource.
 * @param memory_resource Resource pointer; must be non-NULL.
 * @param string Source string.
 * @return Owned copy, or error.
 */
Result_String string_copy(void* memory_resource, String string);

/**
 * @brief Appends printf-formatted text to a string.
 * @param this Destination string.
 * @param format `printf`-style format string.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code string_printf(String* this, const char* format, ...)
    __attribute__((format(__printf__, 2, 3)));

/**
 * @brief Appends printf-formatted text from a `va_list`.
 * @param this Destination string.
 * @param format `printf`-style format string.
 * @param args Format arguments.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code string_vprintf(String* this, const char* format, va_list args)
    __attribute__((format(__printf__, 2, 0)));

/**
 * @brief Creates a formatted owned string.
 * @param memory_resource Resource pointer; must be non-NULL.
 * @param format `printf`-style format string.
 * @return Owned string, or error.
 */
Result_String string_ctor_printf(void* memory_resource,
    const char* format,
    ...) __attribute__((format(__printf__, 2, 3)));

/**
 * @brief Creates a formatted owned string from a `va_list`.
 * @param memory_resource Resource pointer; must be non-NULL.
 * @param format `printf`-style format string.
 * @param args Format arguments.
 * @return Owned string, or error.
 */
Result_String string_ctor_vprintf(void* memory_resource,
    const char* format,
    va_list args) __attribute__((format(__printf__, 2, 0)));

/**
 * @brief Appends one byte and keeps trailing null terminator.
 * @param this Destination string.
 * @param ch Byte to append.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code string_append_char(String* this, char ch);

/**
 * @brief Appends another owned string.
 * @param this Destination string.
 * @param string Source string.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code string_append_string(String* this, String string);

/**
 * @brief Appends a null-terminated C string.
 * @param this Destination string.
 * @param string Source string, or NULL for no-op.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code string_append(String* this, const char* string);

/**
 * @brief Appends a borrowed slice.
 * @param this Destination string.
 * @param string Source slice.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code string_append_str(String* this, Str string);

/**
 * @brief Replaces all occurrences of one slice with another.
 * @param this Destination string.
 * @param from Needle slice.
 * @param to Replacement slice.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code string_replace_all(String* this, Str from, Str to);

/**
 * @brief Returns a borrowed slice from an owned string.
 * @param this Source string.
 * @param start Start index.
 * @param end End index.
 * @return Slice result, or `ERROR_BAD_ARGS`.
 */
Result_Str string_slice(String this, size_t start, size_t end);

/**
 * @brief Clears content while keeping capacity.
 * @param this String pointer.
 */
void string_clear(String* this);

/**
 * @brief Compares two owned strings lexicographically.
 * @param lhs Left string.
 * @param rhs Right string.
 * @return `memcmp`-style sign value.
 */
int string_compare(String lhs, String rhs);

/**
 * @brief Ensures string capacity is at least `new_capacity`.
 * @param this String pointer.
 * @param new_capacity Requested capacity excluding terminator.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code string_realloc(String* this, size_t new_capacity);

#endif // CMLIB_STRING_H_
