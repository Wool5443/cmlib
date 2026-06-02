#ifndef CMLIB_SCRATCH_BUF_H_
#define CMLIB_SCRATCH_BUF_H_

#include "String.h"

Error_code scratch_ctor(void* memory_resource, size_t capacity);

void scratch_dtor();

size_t scratch_get_size();

char* scratch_get();

Str scratch_get_str();

Result_String scratch_copy_string(void* memory_resource);

void scratch_pop(size_t count);

void scratch_clear();

Error_code scratch_printf(const char* format, ...);

Error_code scratch_vprintf(const char* format, va_list args);

Error_code scratch_append(const char* string);

Error_code scratch_append_str(Str slice);

Error_code scratch_append_string(const String string);

Error_code scratch_append_char(char c);

#endif // CMLIB_SCRATCH_BUF_H_
