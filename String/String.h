/**
 * @file String.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for a heap-allocated string implementation and string
 * manipulation utilities.
 *
 * This header defines the `String` and `Str` structures, along with functions
 * for managing dynamically allocated strings. The `String` structure supports
 * heap-allocated, mutable strings, while the `Str` structure provides a view of
 * a string, often used for efficient string manipulations without ownership.
 *
 * The file provides constructors, destructors, comparison, concatenation, and
 * other utilities for working with strings and string slices. The `Allocator`
 * structure is used to manage memory for `String` objects, and custom memory
 * allocators can be used via `string_set_allocator`.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_STRING_H_
#define CMLIB_STRING_H_

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#if __STDC_VERSION__ < 202311L
#include <stdbool.h>
#endif

#include "../Allocator/Allocator.h"
#include "../Logger/Logger.h"

/**
 * @brief An empty `String` constant.
 *
 * This constant represents an empty string (with size 0) using the `String`
 * structure.
 */
#define CMLIB_EMPTY_STRING ((String){})

/**
 * @struct String
 * @brief A heap-allocated string with dynamic size and capacity.
 *
 * The `String` structure represents a heap-allocated string with support for
 * resizing and dynamic memory management. It includes a pointer to the string
 * data, the size of the string, and the total capacity of the buffer that can
 * hold the string's data.
 */
typedef struct String
{
    Allocator allocator; ///< Allocator used for memory management of the
                         ///< string.
    char* data;          ///< Pointer to the actual data (characters).
    size_t size; ///< Current size of the string (excluding null terminator).
    size_t capacity; ///< Total capacity of the string's buffer (excluding null
                     ///< terminator).
} String;

extern Allocator Current_string_allocator; /**< The global allocator used by the
                                              String structure. */

/**
 * @struct Str
 * @brief A view of a string (slice).
 *
 * The `Str` structure provides a view of a string, which is a pointer to a
 * substring of another string. It does not own the data but allows manipulation
 * of a portion of a larger string.
 */
typedef struct Str
{
    const char* data; ///< Pointer to the data of the string slice.
    size_t size;      ///< Size of the string slice.
} Str;

DECLARE_RESULT_HEADER(String);
DECLARE_RESULT_HEADER(Str);

#define STR_LITERAL(string__)                                                  \
    ((Str){.data = string__, .size = sizeof(string__) - 1}) /**< A macro for   \
                                                           creating a `Str`    \
                                                           from a literal      \
                                                           string. */

/**
 * @brief Constructor for a `Str` from a null-terminated string.
 *
 * This function creates a `Str` object from a null-terminated C string.
 *
 * @param string The null-terminated C string.
 *
 * @return A `Str` object representing the given string.
 *
 * @see Str
 */
INLINE Str str_ctor(const char* string);

/**
 * @brief Constructor for a `Str` from a string and its size.
 *
 * This function creates a `Str` object from a given string and its size.
 *
 * @param string The pointer to the string data.
 * @param size The size of the string.
 *
 * @return A `Str` object representing the given string and its size.
 *
 * @see Str
 */
INLINE Str str_ctor_size(const char* string, size_t size);

/**
 * @brief Constructor for a `Str` from a `String` object.
 *
 * This function creates a `Str` view from a `String` object.
 *
 * @param string The `String` object to create a `Str` from.
 *
 * @return A `Str` object representing the view into the `String`.
 *
 * @see Str
 * @see String
 */
INLINE Str str_ctor_string(String string);

/**
 * @brief Compares two `Str` objects.
 *
 * This function compares two `Str` objects lexicographically.
 *
 * @param lhs The first `Str` object.
 * @param rhs The second `Str` object.
 *
 * @return A negative, zero, or positive integer indicating the result of the
 * comparison.
 *
 * @see Str
 */
INLINE int str_compare(Str lhs, Str rhs);

/**
 * @brief Prints a `Str` slice to an output stream.
 *
 * This function prints a `Str` object to the specified output stream.
 *
 * @param string The `Str` object to print.
 * @param [out] out The output stream to print to.
 *
 * @see Str
 */
INLINE void str_print(Str string, FILE* out);

/**
 * @brief Slices a `Str` object.
 *
 * This function returns a new `Str` object that represents a slice of the
 * original `Str` object.
 *
 * @param string The `Str` object to slice.
 * @param start_idx The start index of the slice (inclusive).
 * @param end_idx The end index of the slice (exclusive).
 *
 * @return A `Result_Str` object representing the sliced string.
 *
 * @see Str
 * @see Error_code
 */
INLINE Result_Str str_slice(Str string, size_t start_idx, size_t end_idx);

/**
 * @brief Sets a custom allocator for the `String` structure.
 *
 * This function allows setting a custom memory allocator for the `String`
 * structure.
 *
 * @param allocator The custom allocator to use for the `String` objects.
 */
INLINE void string_set_allocator(Allocator allocator);

/**
 * @brief Resets the `String` allocator to the default allocator (using
 * `calloc`).
 *
 * This function resets the allocator used for `String` objects to the default
 * `calloc` allocator.
 */
INLINE void string_reset_allocator();

/**
 * @brief Constructs a `String` from a null-terminated C string.
 *
 * This function creates a `String` object from a null-terminated C string.
 *
 * @param string The null-terminated C string to convert into a `String`.
 *
 * @return A `Result_String` containing the created `String` or an error code.
 *
 * @see String
 * @see Error_code
 */
INLINE Result_String string_ctor(const char* string);

/**
 * @brief Constructs a `String` with a specified capacity.
 *
 * This function creates a `String` with the given capacity, allowing space for
 * future growth.
 *
 * @param capacity The capacity of the string.
 *
 * @return A `Result_String` containing the created `String` or an error code.
 *
 * @see String
 * @see Error_code
 */
Result_String string_ctor_capacity(size_t capacity);

/**
 * @brief Constructs a `String` from a `Str` view.
 *
 * This function creates a `String` from a `Str` view, making a copy of the
 * data.
 *
 * @param string The `Str` object to convert into a `String`.
 *
 * @return A `Result_String` containing the created `String` or an error code.
 *
 * @see String
 * @see Str
 * @see Error_code
 */
INLINE Result_String string_ctor_str(Str string);

/**
 * @brief Destructor for a `String` object.
 *
 * This function safely deallocates the memory used by the `String` object. It
 * is safe to call multiple times or on a `NULL` pointer.
 *
 * @param [out] string The `String` object to deallocate.
 *
 * @see String
 */
INLINE void string_dtor(String* this);

/**
 * @brief Copies a `String`.
 *
 * This function creates a new `String` object by copying the contents of the
 * provided `String`.
 *
 * @param string The `String` to copy.
 *
 * @return A `Result_String` containing the copied `String` or an error code.
 *
 * @see String
 * @see Error_code
 */
INLINE Result_String string_copy(String string);

/**
 * @brief Prints a formatted string into a `String`.
 *
 * This function uses `printf`-like formatting and appends the result to the
 * `String`.
 *
 * @param this The `String` to append to.
 * @param format The format string.
 *
 * @return An error code indicating success or failure.
 *
 * @see String
 * @see Error_code
 */
INLINE Error_code string_printf(String* this, const char* format, ...);
Error_code string_vprintf(String* this, const char* format, va_list args);

/**
 * @brief Constructs a `String` from a formatted string.
 *
 * This function creates a new `String` by formatting the given `format` string
 * using `printf`-like syntax.
 *
 * @param format The format string.
 *
 * @return A `Result_String` containing the created `String` or an error code.
 *
 * @see String
 * @see Error_code
 */
INLINE Result_String string_ctor_printf(const char* format, ...);
INLINE Result_String string_ctor_vprintf(const char* format, va_list args);

/**
 * @brief Replaces occurrences of one substring with another.
 *
 * This function replaces `count` occurrences of the `from` substring with the
 * `to` substring in the `String`. If `count` is 0, all occurrences are
 * replaced.
 *
 * @param this The `String` to modify.
 * @param from The substring to replace.
 * @param to The substring to replace with.
 *
 * @return An error code indicating success or failure.
 *
 * @see String
 * @see Error_code
 */
Error_code string_replace_all(String* this, Str from, Str to);

/**
 * @brief Reads the contents of a file into a `String`.
 *
 * This function reads the contents of the specified file and stores it in a
 * `String`.
 *
 * @param path The path to the file.
 *
 * @return A `Result_String` containing the contents of the file or an error
 * code.
 *
 * @see String
 * @see Error_code
 */
Result_String read_file(const char* path);

/**
 * @brief Slices a `String` into a `Str`.
 *
 * This function returns a view (`Str`) of a substring from a `String`. This is
 * unsafe if the `String` is `NULL`.
 *
 * @param this The `String` to slice.
 * @param start_idx The start index of the slice (inclusive).
 * @param end_idx The end index of the slice (exclusive).
 *
 * @return A `Result_Str` containing the sliced string or an error code.
 *
 * @see String
 * @see Str
 * @see Error_code
 */
INLINE Result_Str string_slice(String this, size_t start_idx, size_t end_idx);

/**
 * @brief Appends a character to a `String`.
 *
 * This function appends a single character to the end of the `String`.
 *
 * @param [in, out] this The `String` to append to.
 * @param ch The character to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see String
 * @see Error_code
 */
INLINE Error_code string_append_char(String* this, char ch);

/**
 * @brief Appends a `String` to another `String`.
 *
 * This function appends the contents of one `String` to another.
 *
 * @param [in, out] this The `String` to append to.
 * @param string The `String` to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see String
 * @see Error_code
 */
INLINE Error_code string_append_string(String* this, String string);

/**
 * @brief Appends a C-style string to a `String`.
 *
 * This function appends a null-terminated C-style string to the `String`.
 *
 * @param [in, out] this The `String` to append to.
 * @param string The C-style string to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see String
 * @see Error_code
 */
INLINE Error_code string_append(String* this, const char* string);

/**
 * @brief Appends a `Str` to a `String`.
 *
 * This function appends a `Str` (string slice) to the `String`.
 *
 * @param [in, out] this The `String` to append to.
 * @param string The `Str` to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see String
 * @see Str
 * @see Error_code
 */
Error_code string_append_str(String* this, Str string);

/**
 * @brief Clears a `String`.
 *
 * This function clears the contents of the `String`, resetting its size to
 * zero.
 *
 * @param [out] this The `String` to clear.
 *
 * @see String
 */
INLINE void string_clear(String* this);

/**
 * @brief Compares two `String` objects.
 *
 * This function compares two `String` objects lexicographically.
 *
 * @param lhs The first `String`.
 * @param rhs The second `String`.
 *
 * @return A negative, zero, or positive integer indicating the result of the
 * comparison.
 *
 * @see String
 */
INLINE int string_compare(String lhs, String rhs);

/**
 * @brief Reallocates a `String` to a new capacity.
 *
 * This function reallocates the memory for the `String` to a new capacity. The
 * `String` will be resized accordingly to accommodate additional data.
 *
 * @param [in, out] this The `String` to reallocate.
 * @param newCapacity The new capacity for the `String`.
 *
 * @return An error code indicating success or failure.
 *
 * @see String
 * @see Error_code
 */
Error_code string_realloc(String* this, size_t newCapacity);

INLINE Str str_ctor(const char* string)
{
    return str_ctor_size(string, strlen(string));
}

INLINE Str str_ctor_size(const char* string, size_t size)
{
    if (!string)
        return (Str){};
    if (size == 0)
        return (Str){};

    return (Str){
        .data = string,
        .size = size,
    };
}

INLINE Str str_ctor_string(String string)
{
    return (Str){
        .data = string.data,
        .size = string.size,
    };
}

INLINE int str_compare(Str lhs, Str rhs)
{
    if (lhs.size == rhs.size)
    {
        return memcmp(lhs.data, rhs.data, lhs.size);
    }
    return memcmp(lhs.data, rhs.data, MIN(lhs.size, rhs.size) + 1);
}

INLINE void str_print(Str string, FILE* out)
{
    if (!string.data)
    {
        return;
    }

    for (size_t i = 0; i < string.size; i++)
    {
        putc(string.data[i], out);
    }
}

INLINE Result_Str str_slice(Str string, size_t start_idx, size_t end_idx)
{
    ERROR_CHECKING();

    if (start_idx >= string.size || end_idx >= string.size
        || end_idx < start_idx)
    {
        err = ERROR_BAD_ARGS;
        log_error("Failed to create slice:\n"
                  "startIdx: %zu, endIdx: %zu, size: %zu",
                  start_idx,
                  end_idx,
                  string.size);
        return Result_Str_ctor((Str){}, err);
    }

    return Result_Str_ctor((Str){.data = string.data + start_idx,
                                 .size = end_idx - start_idx},
                           err);
}

INLINE void string_set_allocator(Allocator allocator)
{
    Current_string_allocator = allocator;
}

INLINE void string_reset_allocator()
{
    Current_string_allocator = CALLOC_ALLOCATOR;
}

INLINE Result_String string_ctor(const char* string)
{
    return string_ctor_str(str_ctor(string));
}

INLINE Result_String string_ctor_str(Str string)
{
    if (!string.data)
        return (Result_String){};
    if (string.size == 0)
        return (Result_String){};

    Result_String stringRes = string_ctor_capacity(string.size);

    if (stringRes.error_code)
        return stringRes;

    memcpy(stringRes.value.data, string.data, string.size);

    stringRes.value.size = string.size;

    return stringRes;
}

INLINE void string_dtor(String* this)
{
    if (!this)
    {
        return;
    }

    if (this->allocator.free)
    {
        this->allocator.free(this->data);
    }
    *this = (String){};
}

INLINE Result_String string_copy(String string)
{
    return string_ctor_str(str_ctor_string(string));
}

INLINE int string_compare(String lhs, String rhs)
{
    return str_compare(str_ctor_string(lhs), str_ctor_string(rhs));
}

INLINE void string_clear(String* this)
{
    if (this->data)
        this->data[0] = '\0';
    this->size = 0;
}

INLINE Error_code string_append(String* this, const char* string)
{
    if (!string)
        return EVERYTHING_FINE;
    return string_append_str(this, str_ctor(string));
}

INLINE Error_code string_append_string(String* this, String string)
{
    return string_append_str(this, str_ctor_string(string));
}

INLINE Error_code string_append_char(String* this, char ch)
{
    ERROR_CHECKING();

    if (this->size == this->capacity)
    {
        if ((err = string_realloc(this, this->capacity * 2)))
        {
            return err;
        }
    }
    this->data[this->size++] = ch;
    this->data[this->size] = '\0';

    return EVERYTHING_FINE;
}

INLINE Result_Str string_slice(String this, size_t start_idx, size_t end_idx)
{
    return str_slice(str_ctor_string(this), start_idx, end_idx);
}

INLINE Error_code string_printf(String* this, const char* format, ...)
{
    va_list args = {};
    va_start(args, format);

    return string_vprintf(this, format, args);
}

INLINE Result_String string_ctor_printf(const char* format, ...)
{
    va_list args = {};
    va_start(args, format);

    return string_ctor_vprintf(format, args);
}

INLINE Result_String string_ctor_vprintf(const char* format, va_list args)
{
    String s = {};
    return (Result_String){
        .error_code = string_vprintf(&s, format, args),
        .value = s,
    };
}

#endif // CMLIB_STRING_H_
