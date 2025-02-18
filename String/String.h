#ifndef CMLIB_STRING_H_
#define CMLIB_STRING_H_

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#if __STDC_VERSION__ < 202311L
    #include <stdbool.h>
#endif

#include "../Logger/Logger.h"

#define CMLIB_EMPTY_STRING ((String){})

/**
 * @struct String
 *
 * @brief A heap-allocated string
 */
typedef struct String
{
    char*  data; ///< data
    size_t size; ///< size
    size_t capacity; ///< capacity number of symbols it can store not including 0. So real capacity is capacity + 1
} String;

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
INLINE Str str_ctor(const char* restrict string);

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
INLINE Str str_ctor_size(const char* restrict string, size_t size);

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
INLINE Str str_ctor_string(const String string);

/**
 * @brief Prints str slice to out
 *
 * @param [in] string
 * @param [out] out
 *
 * @see Str
 */
INLINE void str_print(const Str string, FILE* out);

/**
 * @brief String constructor from c-style string
 *
 * @param [in] string
 *
 * @return Result_String
 *
 * @see String
 * @see ErrorCode
 */
INLINE Result_String string_ctor(const char* restrict string);

/**
 * @brief String constructor with sufficient capacity
 *
 * @param [in] capacity
 *
 * @return Result_String
 *
 * @see String
 * @see ErrorCode
 */
INLINE Result_String string_ctor_capacity(size_t capacity);

/**
 * @brief String constructor from Str
 *
 * @param [in] string
 *
 * @return Result_String
 *
 * @see String
 * @see Str
 * @see ErrorCode
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
INLINE void string_dtor(String* restrict this);

/**
 * @brief Copies a string
 *
 * @param [in] string
 *
 * @return Result_String
 *
 * @see String
 * @see ErrorCode
 */
INLINE Result_String string_copy(const String string);


/**
 * @brief Prints to string like printf
 *
 * @param [in] format
 *
 * @return Result_String
 *
 * @see String
 * @see ErrorCode
 */
INLINE Result_String string_printf(const char* restrict format, ...);
INLINE Result_String string_vprintf(const char* restrict format, va_list args);

/**
 * @brief Reads file's contents to a String
 *
 * @param [in] path
 *
 * @return Result_String
 *
 * @see String
 * @see ErrorCode
 */
INLINE Result_String read_file(const char* restrict path);

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
 * @see ErrorCode
 */
INLINE Result_Str string_slice(const String* restrict this, size_t startIdx, size_t endIdx);

/**
 * @brief Appends a char to this
 *
 * Unsafe to append to NULL
 *
 * @param [in, out] this
 * @param [in] string
 *
 * @return ErrorCode
 *
 * @see String
 * @see Str
 * @see ErrorCode
 */
INLINE Error_code string_append_char(String* restrict this, char ch);

/**
 * @brief Appends a String string to this
 *
 * Unsafe to append to NULL
 *
 * @param [in, out] this
 * @param [in] string
 *
 * @return ErrorCode
 *
 * @see String
 * @see Str
 * @see ErrorCode
 */
INLINE Error_code string_append_string(String* restrict this, const String string);

/**
 * @brief Appends a c-style string string to this
 *
 * Unsafe to append to NULL
 *
 * @param [in, out] this
 * @param [in] string
 *
 * @return ErrorCode
 *
 * @see String
 * @see Str
 * @see ErrorCode
 */
INLINE Error_code string_append(String* restrict this, const char* restrict string);

/**
 * @brief Appends a Str string to this
 *
 * Unsafe to append to NULL
 *
 * @param [in, out] this
 * @param [in] string
 *
 * @return ErrorCode
 *
 * @see String
 * @see Str
 * @see ErrorCode
 */
INLINE Error_code string_append_str(String* restrict this, Str string);

/**
 * @brief Clears a stting
 *
 * @param [out] this
 *
 * @see String
 */
INLINE void string_clear(String* restrict this);

/**
 * @brief Reallocs a String to new capacity
 *
 * Unsafe to realloc NULL
 *
 * @param [in, out] this
 * @param [in] newCapacity
 *
 * @return ErrorCode
 *
 * @see String
 * @see ErrorCode
 */
INLINE Error_code string_realloc(String* restrict this, size_t newCapacity);

INLINE Str str_ctor(const char* restrict string)
{
    return str_ctor_size(string, strlen(string));
}

INLINE Str str_ctor_size(const char* restrict string, size_t size)
{
    if (!string) return (Str){};
    if (size == 0) return (Str){};

    return (Str)
    {
        .data = string,
        .size = size,
    };
}

INLINE Str str_ctor_string(const String string)
{
    return (Str)
    {
        .data = string.data,
        .size = string.size,
    };
}

INLINE void str_print(const Str string, FILE* out)
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

INLINE Result_String string_ctor(const char* restrict string)
{
    return string_ctor_str(str_ctor(string));
}

INLINE Result_String string_ctor_capacity(size_t capacity)
{
    ERROR_CHECKING();

    assert(capacity != 0 && "Capacity can't be zero!");

    char* data = calloc(capacity + 1, 1);

    if (!data)
    {
        HANDLE_ERRNO_ERROR(ERROR_NO_MEMORY, "Failed to create string with capacity %zu: %s", capacity);
    }

ERROR_CASE
    return Result_String_ctor(
        (String)
        {
            .data = data,
            .size = 0,
            .capacity = data ? capacity : 0,
        },
        err
    );
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

INLINE void string_dtor(String* restrict this)
{
    if (!this) return;
    free(this->data);
    this->data = NULL;
}

INLINE Result_String string_copy(const String string)
{
    return string_ctor_str(str_ctor_string(string));
}

INLINE Error_code string_realloc(String* restrict this, size_t newCapacity)
{
    ERROR_CHECKING();

    assert(this);
    assert(newCapacity);

    char* newData = NULL;

    if (this->capacity >= newCapacity) return EVERYTHING_FINE;

    newData = realloc(this->data, newCapacity + 1);

    if (!newData)
    {
        HANDLE_ERRNO_ERROR(ERROR_NO_MEMORY, "Failed to realloc string: %s");
    }

    *this = (String)
    {
        .data = newData,
        .size = this->size,
        .capacity = newCapacity
    };

ERROR_CASE
    return err;
}

INLINE void string_clear(String* restrict this)
{
    if (this->data) this->data[0] = '\0';
    this->size = 0;
}

INLINE Error_code string_append(String* restrict this, const char* restrict string)
{
    if (!string) return EVERYTHING_FINE;
    return string_append_str(this, str_ctor(string));
}

INLINE Error_code string_append_str(String* restrict this, Str string)
{
    ERROR_CHECKING();

    assert(this);

    if (!string.data) return EVERYTHING_FINE;
    if (string.size == 0) return EVERYTHING_FINE;

    size_t newSize = this->size + string.size;

    if (newSize > this->capacity)
    {
        if ((err = string_realloc(this, newSize)))
            return err;
    }

    memcpy(this->data + this->size, string.data, string.size);

    this->size = newSize;
    this->data[newSize] = '\0';

    return EVERYTHING_FINE;
}

INLINE Error_code string_append_string(String* restrict this, const String string)
{
    return string_append_str(this, str_ctor_string(string));
}

INLINE Error_code string_append_char(String* restrict this, char ch)
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

INLINE Result_Str string_slice(const String* restrict this, size_t startIdx, size_t endIdx)
{
    ERROR_CHECKING();

    assert(this);

    if (startIdx >= this->size || endIdx >= this->size
        || endIdx < startIdx)
    {
        err = ERROR_BAD_ARGS;
        log_error("Failed to create slice:\n"
                 "startIdx: %zu, endIdx: %zu",
                 startIdx, endIdx);
        return Result_Str_ctor((Str){}, err);
    }

    return Result_Str_ctor(
        (Str){ .data = this->data + startIdx, .size = endIdx - startIdx },
        err
    );
}

INLINE Result_String read_file(const char* restrict path)
{
    ERROR_CHECKING();

    assert(path);

    FILE* file = NULL;
    String string = {};

    file = fopen(path, "r");

    if (!file)
    {
        HANDLE_ERRNO_ERROR(ERROR_BAD_FILE, "Failed to open file %s: %s", path);
    }

    struct stat st = {};
    if (fstat(fileno(file), &st) == -1)
    {
        HANDLE_ERRNO_ERROR(ERROR_BAD_FILE, "fstat error: %s");
    }

    size_t fileSize = st.st_size;

    Result_String stringRes = string_ctor_capacity(fileSize + 1);
    if ((err = stringRes.error_code))
    {
        ERROR_LEAVE();
    }

    string = stringRes.value;

    if (fread(string.data, 1, fileSize, file) != fileSize)
    {
        HANDLE_ERRNO_ERROR(ERROR_BAD_FILE, "Failed to read file: %s");
    }
    fclose(file);

    string.size = fileSize;

    return (Result_String)
    {
        string,
        EVERYTHING_FINE,
    };

ERROR_CASE
    if (file) fclose(file);
    string_dtor(&string);

    return (Result_String){ {}, err };
}

INLINE Result_String string_vprintf(const char* restrict format, va_list args)
{
    ERROR_CHECKING();

    assert(format);

    String string = {};

    size_t formatLength = strlen(format);

    if (formatLength == 0)
    {
        return Result_String_ctor(CMLIB_EMPTY_STRING, EVERYTHING_FINE);
    }

    size_t capacity = formatLength * 2;

    Result_String stringRes = string_ctor_capacity(capacity);
    CHECK_ERROR(stringRes.error_code);
    string = stringRes.value;

    // Try until it fits
    while (true)
    {
        va_list cpargs = {};
        va_copy(cpargs, args);

        int written = vsnprintf(string.data, capacity, format, cpargs);

        if (written < 0)
        {
            HANDLE_ERRNO_ERROR(ERROR_STD, "Error vsnprintf: %s");
        }
        else if (written <= (int)capacity)
        {
            string.size = written;
            break;
        }

        capacity = written + 1;
        CHECK_ERROR(string_realloc(&string, capacity));
    }

    return Result_String_ctor(string, EVERYTHING_FINE);

ERROR_CASE
    string_dtor(&string);

    return Result_String_ctor(CMLIB_EMPTY_STRING, err);
}

INLINE Result_String string_printf(const char* restrict format, ...)
{
    va_list args = {};
    va_start(args, format);

    return string_vprintf(format, args);
}

#endif // CMLIB_STRING_H_
