#include "String.h"

DECLARE_RESULT_SOURCE(String);
DECLARE_RESULT_SOURCE(Str);

Str StrCtorSize(const char* string, size_t size);
Str StrCtor(const char* string);
Str StrCtorFromString(const String string);
void StrPrint(const Str string, FILE* out);
ResultString StringCtorCapacity(size_t capacity);
ResultString StringCtorFromStr(Str string);
ResultString StringCtor(const char* string);
void StringDtor(String* this);
ResultString StringCopy(const String string);
ErrorCode StringRealloc(String* this, size_t newCapacity);
void StringClear(String* this);
ErrorCode StringAppendStr(String* this, Str string);
ErrorCode StringAppend(String* this, const char* string);
ErrorCode StringAppendString(String* this, const String string);
ErrorCode StringAppendChar(String* this, char ch);
ResultStr StringSlice(const String* this, size_t startIdx, size_t endIdx);
ResultString StringReadFile(const char* path);
ResultString StringVPrintf(const char* format, va_list args);
ResultString StringPrintf(const char* format, ...);
