#include "ScratchBuf.h"

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
