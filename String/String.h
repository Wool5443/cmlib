#ifndef CMLIB_STRING_H_
#define CMLIB_STRING_H_

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "Logger.h"

/**
 * @struct String
 *
 * @brief A heap-allocated string
 */
typedef struct String
{
    char*  data; ///< data
    size_t size; ///< size
    size_t capacity; ///< capacity
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

DECLARE_RESULT(String);
DECLARE_RESULT(Str);

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
INLINE Str StrCtorSize(const char* string, size_t size)
{
    if (!string) return (Str){};
    if (size == 0) return (Str){};

    return (Str)
    {
        .data = string,
        .size = size,
    };
}

/**
 * @brief Str constructor
 *
 * @param [in] string
 *
 * @return Str
 *
 * @see Str
 */
INLINE Str StrCtor(const char* string)
{
    return StrCtorSize(string, strlen(string));
}

/**
 * @brief Str constructor from String
 *
 * @param [in] string
 *
 * @return Str
 *
 * @see Str
 */
INLINE Str StrCtorFromString(const String string)
{
    return (Str)
    {
        .data = string.data,
        .size = string.size,
    };
}

INLINE void StrPrint(const Str string, FILE* out)
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

/**
 * @brief String constructor with sufficient capacity
 *
 * @param [in] capacity
 *
 * @return ResultString
 *
 * @see String
 * @see ErrorCode
 */
INLINE ResultString StringCtorCapacity(size_t capacity)
{
    ERROR_CHECKING();

    assert(capacity != 0 && "Capacity can't be zero!");

    char* data = calloc(capacity + 1, 1);

    if (!data)
    {
        HANDLE_ERRNO_ERROR(ERROR_NO_MEMORY, "Failed to create string with capacity %zu: %s");
    }

ERROR_CASE
    return ResultStringCtor(
        (String)
        {
            .data = data,
            .size = 0,
            .capacity = data ? capacity : 0,
        },
        err
    );
}

/**
 * @brief String constructor from Str
 *
 * @param [in] string
 *
 * @return ResultString
 *
 * @see String
 * @see ErrorCode
 */
INLINE ResultString StringCtorFromStr(Str string)
{
    if (!string.data) return (ResultString){};
    if (string.size == 0) return (ResultString){};

    ResultString stringRes = StringCtorCapacity(string.size);

    if (stringRes.errorCode)
        return stringRes;

    memcpy(stringRes.value.data, string.data, string.size);

    stringRes.value.size = string.size;

    return stringRes;
}

/**
 * @brief String constructor from c-style string
 *
 * @param [in] string
 *
 * @return ResultString
 *
 * @see String
 * @see ErrorCode
 */
INLINE ResultString StringCtor(const char* string)
{
    return StringCtorFromStr(StrCtor(string));
}

/**
 * @brief String destructor
 *
 * Safe to call twice or on a NULL
 *
 * @param [in, out] string
 *
 * @see String
 */
INLINE void StringDtor(String* string)
{
    if (!string) return;
    free(string->data);
    string->data = NULL;
}

/**
 * @brief Copies a string
 *
 * @param [in] string
 *
 * @return ResultString
 *
 * @see String
 * @see ErrorCode
 */
INLINE ResultString StringCopy(const String string)
{
    return StringCtorFromStr(StrCtorFromString(string));
}


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
INLINE ErrorCode StringRealloc(String* this, size_t newCapacity)
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

INLINE void StringClear(String* string)
{
    if (string->data) string->data[0] = '\0';
    string->size = 0;
}

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
INLINE ErrorCode StringAppendStr(String* this, Str string)
{
    ERROR_CHECKING();

    assert(this);

    if (!string.data) return EVERYTHING_FINE;
    if (string.size == 0) return EVERYTHING_FINE;

    size_t newSize = this->size + string.size;

    if (newSize > this->capacity)
    {
        if ((err = StringRealloc(this, newSize)))
            return err;
    }

    memcpy(this->data + this->size, string.data, string.size);

    this->size = newSize;
    this->data[newSize] = '\0';

    return EVERYTHING_FINE;
}

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
INLINE ErrorCode StringAppend(String* this, const char* string)
{
    if (!string) return EVERYTHING_FINE;
    return StringAppendStr(this, StrCtor(string));
}

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
INLINE ErrorCode StringAppendString(String* this, const String string)
{
    return StringAppendStr(this, StrCtorFromString(string));
}

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
INLINE ErrorCode StringAppendChar(String* this, char ch)
{
    ERROR_CHECKING();

    static const double STRING_GROW_RATE = 3.0 / 2.0;

    if (this->size == this->capacity)
    {
        if ((err = StringRealloc(this, this->capacity * STRING_GROW_RATE)))
        {
            return err;
        }
    }
    this->data[this->size++] = ch;
    this->data[this->size] = '\0';

    return EVERYTHING_FINE;
}

/**
 * @brief Slices a String
 *
 * Unsafe to append to NULL
 *
 * @param [in] this
 * @param [in] startIdx
 * @param [in] endIdx
 *
 * @return ResultStr
 *
 * @see String
 * @see Str
 * @see ErrorCode
 */
INLINE ResultStr StringSlice(const String* this, size_t startIdx, size_t endIdx)
{
    ERROR_CHECKING();

    assert(this);

    if (startIdx >= this->size || endIdx >= this->size
        || endIdx < startIdx)
    {
        err = ERROR_BAD_ARGS;
        LogError("Failed to create slice:\n"
                 "startIdx: %zu, endIdx: %zu",
                 startIdx, endIdx);
        return ResultStrCtor((Str){}, err);
    }

    return ResultStrCtor(
        (Str){ .data = this->data + startIdx, .size = endIdx - startIdx },
        err
    );
}

/**
 * @brief Reads file's contents to a String
 *
 * @param [in] path
 *
 * @return ResultString
 *
 * @see String
 * @see ErrorCode
 */
INLINE ResultString StringReadFile(const char* path)
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

    ResultString stringRes = StringCtorCapacity(fileSize + 1);
    if ((err = stringRes.errorCode))
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

    return (ResultString)
    {
        string,
        EVERYTHING_FINE,
    };

ERROR_CASE
    if (file) fclose(file);
    StringDtor(&string);

    return (ResultString){ {}, err };
}

#endif // CMLIB_STRING_H_
