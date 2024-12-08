#ifndef STRING_H_
#define STRING_H_

#include <assert.h>
#include <errno.h>
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
        err = ERROR_NO_MEMORY;
        LogError();
        ResultStringCtor((String){}, err);
    }

    return ResultStringCtor(
        (String)
        {
            .data = data,
            .size = 0,
            .capacity = capacity,
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
    ResultString stringRes = StringCtorFromStr(StrCtorFromString(string));

    return stringRes;
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
INLINE ErrorCode StringRealloc(String this[static 1], size_t newCapacity)
{
    ERROR_CHECKING();

    assert(this);
    assert(newCapacity);

    char* newData = NULL;

    if (this->capacity >= newCapacity) return EVERYTHING_FINE;

    newData = realloc(this->data, newCapacity);

    if (!newData)
    {
        err = ERROR_NO_MEMORY;
        LogError();
        return err;
    }

    *this = (String)
    {
        .data = newData,
        .size = this->size,
        .capacity = newCapacity
    };

    return err;
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
INLINE ErrorCode StringAppendStr(String this[static 1], Str string)
{
    ERROR_CHECKING();

    assert(this);

    if (!string.data) return EVERYTHING_FINE;
    if (string.size == 0) return EVERYTHING_FINE;

    size_t newSize = this->size + string.size;

    if (newSize > this->capacity)
    {
        if ((err = StringRealloc(this, this->capacity * 3 / 2)))
            return err;
    }

    memcpy(this->data + this->size, string.data, string.size);

    this->size = newSize;

    return err;
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
INLINE ErrorCode StringAppend(String this[static 1], const char* string)
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
INLINE ErrorCode StringAppendString(String this[static 1], const String string)
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
INLINE ErrorCode StringAppendChar(String this[static 1], char ch)
{
    char chstr[] = { ch, '\0'};
    return StringAppendStr(this, (Str){ chstr, 1 });
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
INLINE ResultStr StringSlice(const String this[static 1], size_t startIdx, size_t endIdx)
{
    ERROR_CHECKING();

    assert(this);

    if (startIdx >= this->size || endIdx >= this->size
        || endIdx < startIdx)
    {
        err = ERROR_BAD_ARGS;
        LogError();
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
INLINE ResultString StringReadFile(const char path[static 1])
{
    ERROR_CHECKING();

    assert(path);

    FILE* file = NULL;
    String string = {};

    file = fopen(path, "r");

    if (!file)
    {
        int ern = errno;
        err = ERROR_LINUX;
        LogError("Failed to open file %s: %s", path, strerror(ern));
        ERROR_LEAVE();
    }

    struct stat st = {};
    if (fstat(fileno(file), &st) == -1)
    {
        int ern = errno;
        err = ERROR_LINUX;
        LogError("fstat error: %s", strerror(ern));
        ERROR_LEAVE();
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
        int ern = errno;
        err = ERROR_LINUX;
        LogError("Failed to read file: %s", strerror(ern));
        ERROR_LEAVE();
    }

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

#endif // STRING_H_
