#ifndef CMLIB_SCRATCH_BUF_H_
#define CMLIB_SCRATCH_BUF_H_

#include "String.h"

ErrorCode scratch_ctor(void* memory_resource, size_t capacity);

void scratch_dtor();

size_t scratch_get_size();

char* scratch_get();

Str scratch_get_str();

Result_String scratch_copy_string(void* memory_resource);

void scratch_pop(size_t count);

void scratch_clear();

ErrorCode scratch_printf(const char* format, ...);

ErrorCode scratch_vprintf(const char* format, va_list args);

ErrorCode scratch_append(const char* string);

ErrorCode scratch_append_str(Str slice);

ErrorCode scratch_append_string(const String string);

ErrorCode scratch_append_char(char c);

#endif // CMLIB_SCRATCH_BUF_H_
