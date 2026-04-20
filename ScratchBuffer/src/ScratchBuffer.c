#include "ScratchBuffer.h"

static String scratch_string = {};

String* get_scratch_buffer_unsafe_()
{
    return &scratch_string;
}

Error_code scratch_ctor(void* memory_resource, size_t capacity)
{
    string_dtor(&scratch_string);
    Result_String res = string_ctor_capacity(memory_resource, capacity);
    scratch_string = res.value;
    return res.error_code;
}

void scratch_dtor()
{
    string_dtor(&scratch_string);
}

size_t scratch_get_size()
{
    return scratch_string.size;
}

char* scratch_get()
{
    return scratch_string.data;
}

Str scratch_get_str()
{
    return str_ctor_string(scratch_string);
}

Result_String scratch_copy_string(void* memory_resource)
{
    return string_copy(memory_resource, scratch_string);
}

void scratch_pop(size_t count)
{
    if (count > scratch_string.size)
    {
        return;
    }

    scratch_string.size -= count;
    memset(scratch_string.data + scratch_string.size, '\0', count);
}

void scratch_clear()
{
    string_clear(&scratch_string);
}

Error_code scratch_vprintf(const char* format, va_list args)
{
    return string_vprintf(&scratch_string, format, args);
}

Error_code scratch_printf(const char* format, ...)
{
    va_list args = {};
    va_start(args, format);

    Error_code res = scratch_vprintf(format, args);

    va_end(args);

    return res;
}

Error_code scratch_append(const char* string)
{
    return string_append(&scratch_string, string);
}

Error_code scratch_append_str(Str string)
{
    return string_append_str(&scratch_string, string);
}

Error_code scratch_append_string(const String string)
{
    return string_append_string(&scratch_string, string);
}

Error_code scratch_append_char(char c)
{
    return string_append_char(&scratch_string, c);
}
