#include "String.h"

DECLARE_RESULT_SOURCE(String);
DECLARE_RESULT_SOURCE(Str);

extern Allocator Current_string_allocator;

Str str_ctor_size(const char* string, size_t size);
Str str_ctor(const char* string);
Str str_ctor_string(const String string);
void str_print(const Str string, FILE* out);
Result_String string_ctor_capacity(size_t capacity);
Result_String string_ctor_str(Str string);
Result_String string_ctor(const char* string);
void string_dtor(String* this);
Result_String string_copy(const String string);
Error_code string_realloc(String* this, size_t newCapacity);
void string_clear(String* this);
Error_code string_append_str(String* this, Str string);
Error_code string_append(String* this, const char* string);
Error_code string_append_string(String* this, const String string);
Error_code string_append_char(String* this, char ch);
Result_Str string_slice(const String* this, size_t startIdx, size_t endIdx);
Result_String read_file(const char* path);
Result_String string_vprintf(const char* format, va_list args);
Result_String string_printf(const char* format, ...);
