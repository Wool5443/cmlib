#ifndef SCRATCH_BUF_H
#define SCRATCH_BUF_H

#include "String.h"

ErrorCode ScratchInit(size_t capacity);
void      ScratchDtor();
void      ScratchClean();

size_t       ScratchGetSize();
char*        ScratchGet();
Str          ScratchGetStr();
ResultString ScratchGetString();

ErrorCode ScratchAppendStr(Str slice);

INLINE ErrorCode ScratchAppendChar(char c)
{
    char chstr[] = { c, '\0'};
    Str chslice = StrCtor(chstr);

    return ScratchAppendStr(chslice);
}

INLINE ErrorCode ScratchAppendString(const String string)
{
    return ScratchAppendStr(StrCtorFromString(string));
}

INLINE ErrorCode ScratchAppend(const char* string)
{
    return ScratchAppendStr(StrCtor(string));
}

void ScratchPop(size_t count);

#endif
