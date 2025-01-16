#include "ScratchBuf.h"
#include "Logger.h"

#define CHECK_SCRATCH_STATE()                                       \
do                                                                  \
{                                                                   \
    if (!scratchString.data)                                        \
    {                                                               \
        UNUSED ErrorCode err = ERROR_UNINITIALIZED;                 \
        LogError("PLEASE, INITIALIZE SCRATCH BUFFER FIRST!!!!\n");  \
        abort();                                                    \
    }                                                               \
} while (0)

static String scratchString = {};

String* GetScratchBuffer_UNSAFE_()
{
    return &scratchString;
}

ErrorCode ScratchInit(size_t capacity)
{
    ERROR_CHECKING();

    assert(capacity != 0 && "Capacity can't be zero!");

    ResultString strRes = StringCtorCapacity(capacity);

    err = strRes.errorCode;
    if (err)
    {
        LogError();
        return err;
    }

    scratchString = strRes.value;

    return err;
}

void ScratchDtor()
{
    StringDtor(&scratchString);
}

size_t ScratchGetSize()
{
    CHECK_SCRATCH_STATE();
    return scratchString.size;
}

char* ScratchGet()
{
    CHECK_SCRATCH_STATE();
    return scratchString.data;
}

Str ScratchGetStr()
{
    CHECK_SCRATCH_STATE();
    return StrCtorFromString(scratchString);
}

ResultString ScratchCopyString()
{
    return StringCopy(scratchString);
}

void ScratchPop(size_t count)
{
    CHECK_SCRATCH_STATE();

    if (count > scratchString.size) return;

    scratchString.size -= count;

    memset(scratchString.data + scratchString.size - count, '\0', count);
}

void ScratchClear()
{
    CHECK_SCRATCH_STATE();

    StringClear(&scratchString);
}

ErrorCode ScratchVPrintf(const char* format, va_list args)
{
    CHECK_SCRATCH_STATE();

    ERROR_CHECKING();

    if (!format)
    {
        return ERROR_NULLPTR;
    }

    char* buffer = scratchString.data + scratchString.size;
    size_t leftCapacity = scratchString.capacity - scratchString.size;

    while (true)
    {
        va_list cpyargs = {};
        va_copy(cpyargs, args);

        int written = vsnprintf(buffer, leftCapacity, format, cpyargs);

        if (written < 0)
        {
            HANDLE_ERRNO_ERROR(ERROR_STD,
                               "Error vsnrprintf(%p, %zu, %s, ...): %s",
                               buffer, leftCapacity, format);
        }
        else if (written <= CMLIB_CAST(int, leftCapacity))
        {
            scratchString.size += written;
            return EVERYTHING_FINE;
        }

        CHECK_ERROR(StringRealloc(&scratchString, scratchString.capacity + written));
    }

    return EVERYTHING_FINE;

ERROR_CASE
    return err;
}

ErrorCode ScratchPrintf(const char* format, ...)
{
    va_list args = {};
    va_start(args, format);

    return ScratchVPrintf(format, args);
}

ErrorCode ScratchAppend(const char* string)
{
    CHECK_SCRATCH_STATE();
    return StringAppend(&scratchString, string);
}

ErrorCode ScratchAppendStr(Str string)
{
    CHECK_SCRATCH_STATE();

    return StringAppendStr(&scratchString, string);
}

ErrorCode ScratchAppendString(const String string)
{
    CHECK_SCRATCH_STATE();
    return StringAppendString(&scratchString, string);
}

ErrorCode ScratchAppendChar(char c)
{
    CHECK_SCRATCH_STATE();
    return StringAppendChar(&scratchString, c);
}
