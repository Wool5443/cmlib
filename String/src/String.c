#include "String.h"

#include <stdarg.h>
#include <string.h>

#include "../../common.h"

DECLARE_RESULT_SOURCE(String);
DECLARE_RESULT_SOURCE(Str);

Str str_ctor_size(const char* string, size_t size);
Str str_ctor(const char* string);
Str str_ctor_string(const String string);
int str_compare(const Str lhs, const Str rhs);
void str_print(const Str string, FILE* out);
Result_Str str_slice(const Str string, size_t start_idx, size_t end_idx);
Result_String string_ctor(void* memory_resource, const char* string);
Result_String string_ctor_str(void* memory_resource, Str string);
void string_dtor(String* this);
Result_String string_copy(void* memory_resource, const String string);
void string_clear(String* this);
int string_compare(const String lhs, const String rhs);
ErrorCode string_append(String* this, const char* string);
ErrorCode string_append_string(String* this, const String string);
ErrorCode string_append_char(String* this, char ch);
Result_Str string_slice(const String this, size_t start_idx, size_t end_idx);
ErrorCode string_printf(String* this, const char* format, ...);
Result_String
string_ctor_printf(void* memory_resource, const char* format, ...);
Result_String
string_ctor_vprintf(void* memory_resource, const char* format, va_list args);

Str str_ctor(const char* string)
{
    return string ? str_ctor_size(string, strlen(string)) : (Str) {};
}

Str str_ctor_size(const char* string, size_t size)
{
    if (!string || size == 0)
    {
        return (Str) {};
    }

    return (Str) {
        .data = string,
        .size = size,
    };
}

Str str_ctor_string(String string)
{
    return (Str) {
        .data = string.data,
        .size = string.size,
    };
}

int str_compare(Str lhs, Str rhs)
{
    return memcmp(lhs.data, rhs.data, MIN(lhs.size, rhs.size));
}

void str_print(Str string, FILE* out)
{
    if (!string.data || !out)
    {
        return;
    }
    fwrite(string.data, 1, string.size, out);
}

Result_Str str_slice(Str string, size_t start, size_t end)
{
    if (start >= string.size || end > string.size || end < start)
    {
        return Result_Str_ctor((Str) {}, ERROR_BAD_ARGS);
    }

    return Result_Str_ctor(
        (Str) {.data = string.data + start, .size = end - start},
        EVERYTHING_FINE);
}

Result_String string_ctor(void* memory_resource, const char* string)
{
    return string_ctor_str(memory_resource, str_ctor(string));
}

Result_String string_ctor_str(void* memory_resource, Str string)
{
    if (!string.data || string.size == 0)
    {
        return Result_String_ctor(
            (String) {
                .memory_resource = (MemoryResource*)memory_resource,
            },
            memory_resource ? EVERYTHING_FINE : ERROR_NULLPTR);
    }

    Result_String string_res = string_ctor_capacity(memory_resource,
        string.size);
    if (string_res.error_code)
    {
        return string_res;
    }

    memcpy(string_res.value.data, string.data, string.size);
    string_res.value.size = string.size;

    return string_res;
}

void string_dtor(String* this)
{
    if (!this)
    {
        return;
    }

    if (this->memory_resource)
    {
        this->memory_resource->deallocate(this->memory_resource, this->data);
    }
    *this = (String) {};
}

Result_String string_copy(void* memory_resource, String string)
{
    return string_ctor_str(memory_resource, str_ctor_string(string));
}

int string_compare(String lhs, String rhs)
{
    return str_compare(str_ctor_string(lhs), str_ctor_string(rhs));
}

void string_clear(String* this)
{
    if (!this)
    {
        return;
    }
    if (this->data)
    {
        this->data[0] = '\0';
    }
    this->size = 0;
}

ErrorCode string_append(String* this, const char* string)
{
    if (!string)
    {
        return EVERYTHING_FINE;
    }
    return string_append_str(this, str_ctor(string));
}

ErrorCode string_append_string(String* this, String string)
{
    return string_append_str(this, str_ctor_string(string));
}

ErrorCode string_append_char(String* this, char ch)
{
    if (!this)
    {
        return ERROR_NULLPTR;
    }
    if (this->size == this->capacity)
    {
        ErrorCode err = string_realloc(this, this->capacity * 2);
        if (err)
        {
            return err;
        }
    }
    this->data[this->size++] = ch;
    this->data[this->size] = '\0';

    return EVERYTHING_FINE;
}

Result_Str string_slice(String this, size_t start, size_t end)
{
    return str_slice(str_ctor_string(this), start, end);
}

ErrorCode string_printf(String* this, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    ErrorCode res = string_vprintf(this, format, args);
    va_end(args);

    return res;
}

Result_String string_ctor_printf(void* memory_resource, const char* format, ...)
{
    va_list args = {};
    va_start(args, format);

    Result_String res = string_ctor_vprintf(memory_resource, format, args);
    va_end(args);

    return res;
}

Result_String
string_ctor_vprintf(void* memory_resource, const char* format, va_list args)
{
    Result_String res = string_ctor_capacity(memory_resource, 1);
    if (res.error_code)
    {
        return res;
    }

    res.error_code = string_vprintf(&res.value, format, args);
    if (res.error_code)
    {
        string_dtor(&res.value);
    }

    return res;
}

Result_String string_ctor_capacity(void* memory_resource, size_t capacity)
{
    MemoryResource* resource = (MemoryResource*)memory_resource;
    if (!resource)
    {
        return Result_String_ctor((String) {}, ERROR_NULLPTR);
    }
    if (capacity == 0)
    {
        return Result_String_ctor((String) {}, ERROR_BAD_VALUE);
    }

    char* data = resource->allocate(resource, capacity + 1, alignof(char));
    if (!data)
    {
        return Result_String_ctor((String) {}, ERROR_NO_MEMORY);
    }

    data[capacity] = '\0';
    return Result_String_ctor(
        (String) {
            .memory_resource = resource,
            .data = data,
            .size = 0,
            .capacity = capacity,
        },
        EVERYTHING_FINE);
}

ErrorCode string_realloc(String* this, size_t new_capacity)
{
    if (!this || !this->memory_resource)
    {
        return ERROR_NULLPTR;
    }
    if (new_capacity == 0)
    {
        return ERROR_BAD_VALUE;
    }
    if (this->capacity >= new_capacity)
    {
        return EVERYTHING_FINE;
    }

    char* new_data = this->memory_resource->allocate(this->memory_resource,
        new_capacity + 1,
        alignof(char));
    if (!new_data)
    {
        return ERROR_NO_MEMORY;
    }

    if (this->data && this->size)
    {
        memcpy(new_data, this->data, this->size);
    }
    new_data[this->size] = '\0';

    this->memory_resource->deallocate(this->memory_resource, this->data);
    this->data = new_data;
    this->capacity = new_capacity;

    return EVERYTHING_FINE;
}

ErrorCode string_append_str(String* this, Str string)
{
    if (!this || !this->memory_resource)
    {
        return ERROR_NULLPTR;
    }
    if (!string.data || string.size == 0)
    {
        return EVERYTHING_FINE;
    }

    size_t new_size = this->size + string.size;
    if (new_size > this->capacity)
    {
        ErrorCode err = string_realloc(this, new_size);
        if (err)
        {
            return err;
        }
    }

    memcpy(this->data + this->size, string.data, string.size);
    this->size = new_size;
    this->data[new_size] = '\0';

    return EVERYTHING_FINE;
}

ErrorCode string_vprintf(String* this, const char* format, va_list args)
{
    if (!this || !this->memory_resource || !format)
    {
        return ERROR_NULLPTR;
    }

    size_t format_length = strlen(format);
    if (format_length == 0)
    {
        return EVERYTHING_FINE;
    }

    va_list cpargs;
    va_copy(cpargs, args);
    int print_size = vsnprintf(NULL, 0, format, cpargs);
    va_end(cpargs);

    if (print_size < 0)
    {
        return ERROR_STD;
    }

    ErrorCode err = string_realloc(this, this->size + (size_t)print_size);
    if (err)
    {
        return err;
    }

    va_copy(cpargs, args);
    print_size = vsnprintf(this->data + this->size,
        (size_t)print_size + 1,
        format,
        cpargs);
    va_end(cpargs);

    if (print_size < 0)
    {
        return ERROR_STD;
    }

    this->size += (size_t)print_size;
    return EVERYTHING_FINE;
}

ErrorCode string_replace_all(String* this, Str from, Str to)
{
    if (!this || !this->memory_resource)
    {
        return ERROR_NULLPTR;
    }
    if (!from.data || from.size == 0)
    {
        return EVERYTHING_FINE;
    }
    if (!to.data)
    {
        to = STR_LITERAL("");
    }

    size_t count = 0;
    for (char* found = strstr(this->data, from.data); found;
        found = strstr(found + from.size, from.data))
    {
        count++;
    }
    if (count == 0)
    {
        return EVERYTHING_FINE;
    }

    size_t new_size = this->size + count * to.size - count * from.size;
    Result_String out = string_ctor_capacity(this->memory_resource,
        new_size ? new_size : 1);
    if (out.error_code)
    {
        return out.error_code;
    }

    const char* reader = this->data;
    while (*reader)
    {
        if (strncmp(reader, from.data, from.size) == 0)
        {
            memcpy(out.value.data + out.value.size, to.data, to.size);
            out.value.size += to.size;
            reader += from.size;
        }
        else
        {
            out.value.data[out.value.size++] = *reader++;
        }
    }
    out.value.data[out.value.size] = '\0';

    this->memory_resource->deallocate(this->memory_resource, this->data);
    *this = out.value;
    return EVERYTHING_FINE;
}
