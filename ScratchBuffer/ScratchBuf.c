#include "ScratchBuf.h"

#define CHECK_SCRATCH_STATE()                                       \
do                                                                  \
{                                                                   \
    if (!scratchString.data)                                        \
    {                                                               \
        ErrorCode err = ERROR_UNINITIALIZED;                        \
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

void ScratchClean()
{
    CHECK_SCRATCH_STATE();

    memset(scratchString.data, '\0', scratchString.capacity);

    scratchString.size = 0;
}

void ScratchPop(size_t count)
{
    CHECK_SCRATCH_STATE();

    if (count > scratchString.size) return;

    scratchString.size -= count;

    memset(scratchString.data + scratchString.size - 1, '\0', count);
}

ErrorCode ScratchAppendStr(Str string)
{
    CHECK_SCRATCH_STATE();

    return StringAppendStr(&scratchString, string);
}
