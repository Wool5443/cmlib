#ifndef CMLIB_STRING_H_
#define CMLIB_STRING_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#if __STDC_VERSION__ < 202311L
    #include <stdbool.h>
#endif

#include "../Logger/Logger.h"
#include "../Allocator/Allocator.h"

#define CMLIB_EMPTY_STRING ((String){})

/**
 * @struct String
 *
 * @brief A heap-allocated string
 */
typedef struct String
{
    Allocator allocator; ///< allocator
    char*  data; ///< data
    size_t size; ///< size
    size_t capacity; ///< capacity number of symbols it can store not including 0. So real capacity is capacity + 1
} String;

extern Allocator Current_string_allocator;

/**
 * @struct Str
 *
 * @brief A view on a string
 */
typedef struct Str
{
    const char* data; ///< data
    size_t size; ///< size
} Str;
#define STR_LITERAL(string) ((Str){ .data = string, .size = sizeof(string) - 1 })

DECLARE_RESULT_HEADER(String);
DECLARE_RESULT_HEADER(Str);

/**
 * @brief Str constructor
 *
 * @param [in] string
 *
 * @return Str
 *
 * @see Str
 */
INLINE Str str_ctor(const char* string);

/**
 * @brief Str constructor knowing string size
 *
 * @param [in] string
 * @param [in] size
 *
 * @return Str
 *
 * @see Str
 */
INLINE Str str_ctor_size(const char* string, size_t size);

/**
 * @brief Str constructor from String
 *
 * @param [in] string
 *
 * @return Str
 *
 * @see Str
 * @see String
 */
INLINE Str str_ctor_string(String string);

/**
 * @brief Compares 2 strs
 *
 * @param [in] lhs
 * @param [in] rhs
 *
 * @return int
 *
 * @see Str
 */
INLINE int str_compare(Str lhs, Str rhs);

/**
 * @brief Prints str slice to out
 *
 * @param [in] string
 * @param [out] out
 *
 * @see Str
 */
INLINE void str_print(Str string, FILE* out);

/**
 * @brief Slice str
 *
 * @param [in] string
 * @param [in] start_idx
 * @param [in] end_idx
 *
 * @return Result_Str
 *
 * @see Str
 * @see Error_code
 */
INLINE Result_Str str_slice(Str string, size_t start_idx, size_t end_idx);

/**
 * @brief Set string allocator
 *
 * @param [in] allocator
 */
INLINE void string_set_allocator(Allocator allocator);

/**
 * @brief Reset default string allocator(calloc)
 */
INLINE void string_reset_allocator();

/**
 * @brief String constructor from c-style string
 *
 * @param [in] string
 *
 * @return Result_String
 *
 * @see String
 * @see Error_code
 */
INLINE Result_String string_ctor(const char* string);

/**
 * @brief String constructor with sufficient capacity
 *
 * @param [in] capacity
 *
 * @return Result_String
 *
 * @see String
 * @see Error_code
 */
Result_String string_ctor_capacity(size_t capacity);

/**
 * @brief String constructor from Str
 *
 * @param [in] string
 *
 * @return Result_String
 *
 * @see String
 * @see Str
 * @see Error_code
 */
INLINE Result_String string_ctor_str(Str string);

/**
 * @brief String destructor
 *
 * Safe to call twice or on a NULL
 *
 * @param [out] string
 *
 * @see String
 */
INLINE void string_dtor(String* this);

/**
 * @brief Copies a string
 *
 * @param [in] string
 *
 * @return Result_String
 *
 * @see String
 * @see Error_code
 */
INLINE Result_String string_copy(String string);


/**
 * @brief Prints to string like printf
 *
 * @param [in] this
 * @param [in] format
 *
 * @return Error_code
 *
 * @see String
 * @see Error_code
 */
INLINE Error_code string_printf(String* this, const char* format, ...);
Error_code string_vprintf(String* this, const char* format, va_list args);

/**
 * @brief Creates a string like printf
 *
 * @param [in] format
 *
 * @return Error_code
 *
 * @see String
 * @see Error_code
 */
INLINE Result_String string_ctor_printf(const char* format, ...);
INLINE Result_String string_ctor_vprintf(const char* format, va_list args);

/**
 * @brief Replaces count occurences of from to to
 * If count == 0, replace all occurences
 *
 * @param [in] this
 * @param [in] from
 * @param [in] to
 *
 * @return Error_code
 *
 * @see String
 * @see Error_code
 */
Error_code string_replace_all(String* this, Str from, Str to);

/**
 * @brief Reads file's contents to a String
 *
 * @param [in] path
 *
 * @return Result_String
 *
 * @see String
 * @see Error_code
 */
Result_String read_file(const char* path);

/**
 * @brief Slices a String
 *
 * Unsafe to append to NULL
 *
 * @param [in] this
 * @param [in] startIdx
 * @param [in] endIdx
 *
 * @return Result_Str
 *
 * @see String
 * @see Str
 * @see Error_code
 */
INLINE Result_Str string_slice(String this, size_t start_idx, size_t end_idx);

/**
 * @brief Appends a char to this
 *
 * Unsafe to append to NULL
 *
 * @param [in, out] this
 * @param [in] string
 *
 * @return Error_code
 *
 * @see String
 * @see Str
 * @see Error_code
 */
INLINE Error_code string_append_char(String* this, char ch);

/**
 * @brief Appends a String string to this
 *
 * Unsafe to append to NULL
 *
 * @param [in, out] this
 * @param [in] string
 *
 * @return Error_code
 *
 * @see String
 * @see Str
 * @see Error_code
 */
INLINE Error_code string_append_string(String* this, String string);

/**
 * @brief Appends a c-style string string to this
 *
 * Unsafe to append to NULL
 *
 * @param [in, out] this
 * @param [in] string
 *
 * @return Error_code
 *
 * @see String
 * @see Str
 * @see Error_code
 */
INLINE Error_code string_append(String* this, const char* string);

/**
 * @brief Appends a Str string to this
 *
 * Unsafe to append to NULL
 *
 * @param [in, out] this
 * @param [in] string
 *
 * @return Error_code
 *
 * @see String
 * @see Str
 * @see Error_code
 */
Error_code string_append_str(String* this, Str string);

/**
 * @brief Clears a stting
 *
 * @param [out] this
 *
 * @see String
 */
INLINE void string_clear(String* this);

/**
 * @brief Compares 2 strings
 *
 * @param [in] lhs
 * @param [in] rhs
 *
 * @return int
 *
 * @see Str
 */
INLINE int string_compare(String lhs, String rhs);

/**
 * @brief Reallocs a String to new capacity
 *
 * Unsafe to realloc NULL
 *
 * @param [in, out] this
 * @param [in] newCapacity
 *
 * @return Error_code
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
    if (!string) return (Str){};
    if (size == 0) return (Str){};

    return (Str)
    {
        .data = string,
        .size = size,
    };
}

INLINE Str str_ctor_string(String string)
{
    return (Str)
    {
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
        || end_idx < start_idx) {
        err = ERROR_BAD_ARGS;
        log_error("Failed to create slice:\n"
                  "startIdx: %zu, endIdx: %zu, size: %zu",
                  start_idx, end_idx, string.size);
        return Result_Str_ctor((Str){}, err);
    }

    return Result_Str_ctor(
        (Str) { .data = string.data + start_idx, .size = end_idx - start_idx },
        err
    );
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
    if (!string.data) return (Result_String){};
    if (string.size == 0) return (Result_String){};

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
    if (this->data) this->data[0] = '\0';
    this->size = 0;
}

INLINE Error_code string_append(String* this, const char* string)
{
    if (!string) return EVERYTHING_FINE;
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
    return (Result_String) {
        .error_code = string_vprintf(&s, format, args),
        .value = s,
    };
}

#endif // CMLIB_STRING_H_
