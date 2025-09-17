#include "Scratch_buf.h"

#include "Logger.h"

#define CHECK_SCRATCH_STATE()                                                  \
    do                                                                         \
    {                                                                          \
        if (!scratch_string.data)                                              \
        {                                                                      \
            UNUSED Error_code err = ERROR_UNINITIALIZED;                       \
            log_error("PLEASE, INITIALIZE SCRATCH BUFFER FIRST!!!!\n");        \
            abort();                                                           \
        }                                                                      \
    } while (0)

static String scratch_string = {};

String* get_scratch_buffer_unsafe_()
{
    return &scratch_string;
}

Error_code scratch_init(size_t capacity)
{
    ERROR_CHECKING();

    if (capacity == 0)
    {
        err = ERROR_BAD_VALUE;
        log_error("Error bad scratch buf capacity of 0");
        ERROR_LEAVE();
    }

    Result_String str_res = string_ctor_capacity(capacity);

    err = str_res.error_code;
    if (err)
    {
        log_error();
        return err;
    }

    scratch_string = str_res.value;

    ERROR_CASE;
    return err;
}

void scratch_dtor()
{
    string_dtor(&scratch_string);
}

size_t scratch_get_size()
{
    CHECK_SCRATCH_STATE();
    return scratch_string.size;
}

char* scratch_get()
{
    CHECK_SCRATCH_STATE();
    return scratch_string.data;
}

Str scratch_get_str()
{
    CHECK_SCRATCH_STATE();
    return str_ctor_string(scratch_string);
}

Result_String scratch_copy_string()
{
    return string_copy(scratch_string);
}

void scratch_pop(size_t count)
{
    CHECK_SCRATCH_STATE();

    if (count > scratch_string.size)
        return;

    scratch_string.size -= count;

    memset(scratch_string.data + scratch_string.size - count, '\0', count);
}

void scratch_clear()
{
    CHECK_SCRATCH_STATE();

    string_clear(&scratch_string);
}

Error_code scratch_vprintf(const char* format, va_list args)
{
    CHECK_SCRATCH_STATE();

    ERROR_CHECKING();

    if (!format)
    {
        return ERROR_NULLPTR;
    }

    char* buffer = scratch_string.data + scratch_string.size;
    size_t left_capacity = scratch_string.capacity - scratch_string.size;

    while (true)
    {
        va_list cpyargs = {};
        va_copy(cpyargs, args);

        int written = vsnprintf(buffer, left_capacity, format, cpyargs);

        if (written < 0)
        {
            HANDLE_ERRNO_ERROR(ERROR_STD,
                               "Error vsnrprintf(%p, %zu, %s, ...): %s",
                               buffer,
                               left_capacity,
                               format);
        }
        else if (written <= (int)left_capacity)
        {
            scratch_string.size += written;
            return EVERYTHING_FINE;
        }

        CHECK_ERROR(
            string_realloc(&scratch_string, scratch_string.capacity + written));
    }

    return EVERYTHING_FINE;

    ERROR_CASE
    return err;
}

Error_code scratch_printf(const char* format, ...)
{
    va_list args = {};
    va_start(args, format);

    return scratch_vprintf(format, args);
}

Error_code scratch_append(const char* string)
{
    CHECK_SCRATCH_STATE();
    return string_append(&scratch_string, string);
}

Error_code scratch_append_str(Str string)
{
    CHECK_SCRATCH_STATE();

    return string_append_str(&scratch_string, string);
}

Error_code scratch_append_string(const String string)
{
    CHECK_SCRATCH_STATE();
    return string_append_string(&scratch_string, string);
}

Error_code scratch_append_char(char c)
{
    CHECK_SCRATCH_STATE();
    return string_append_char(&scratch_string, c);
}
