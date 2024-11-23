#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "Error.h"
#include "../common.h"

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
        RETURN(((ResultString){ err, (String){} }));
    }

    return (ResultString)
    {
        .error = err,
        .value = (String)
        {
            .data = data,
            .size = 0,
            .capacity = capacity,
        },
    };
}

INLINE ResultString StringCtorFromStr(Str string)
{
    ERROR_CHECKING();

    if (!string.data) return (ResultString){};
    if (string.size == 0) return (ResultString){};

    ResultString stringRes = StringCtorCapacity(string.size);

    if ((err = stringRes.error))
        RETURN(stringRes);

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
    ERROR_CHECKING();

    ResultString stringRes = StringCtorFromStr(StrCtorFromString(string));

    if ((err = stringRes.error))
        RETURN(stringRes);

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
        RETURN(err);
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
        err = StringRealloc(this, this->capacity * 3 / 2);
        if (err) RETURN(err);
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
        RETURN(((ResultStr){ err, (Str){} }));
    }

    return (ResultStr)
    {
        err,
        (Str){ .data = this->data + startIdx, .size = endIdx - startIdx },
    };
}

#endif // STRING_H_
