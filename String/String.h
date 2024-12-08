#ifndef STRING_H_
#define STRING_H_

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "Logger.h"

typedef struct
{
    char*  data;
    size_t size;
    size_t capacity;
} String;

typedef struct
{
    const char* data;
    size_t size;
} Str;

DECLARE_RESULT(String);
DECLARE_RESULT(Str);

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

INLINE Str StrCtor(const char* string)
{
    return StrCtorSize(string, strlen(string));
}

INLINE Str StrCtorFromString(const String string)
{
    return (Str)
    {
        .data = string.data,
        .size = string.size,
    };
}

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

INLINE ResultString StringCtor(const char* string)
{
    return StringCtorFromStr(StrCtor(string));
}

INLINE void StringDtor(String* string)
{
    if (!string) return;

    free(string->data);
}

INLINE ResultString StringCopy(const String string)
{
    ResultString stringRes = StringCtorFromStr(StrCtorFromString(string));

    return stringRes;
}

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

INLINE ErrorCode StringAppend(String this[static 1], const char* string)
{
    if (!string) return EVERYTHING_FINE;
    return StringAppendStr(this, StrCtor(string));
}

INLINE ErrorCode StringAppendString(String this[static 1], const String string)
{
    return StringAppendStr(this, StrCtorFromString(string));
}

INLINE ErrorCode StringAppendChar(String this[static 1], char ch)
{
    char chstr[] = { ch, '\0'};
    return StringAppendStr(this, (Str){ chstr, 1 });
}

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
