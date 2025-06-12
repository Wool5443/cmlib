#include "ScratchBuf.h"

#include "Logger.h"

#define CHECK_SCRATCH_STATE()                                                  \
    do                                                                         \
    {                                                                          \
        if (!scratchString.data)                                               \
        {                                                                      \
            UNUSED Error_code err = ERROR_UNINITIALIZED;                       \
            log_error("PLEASE, INITIALIZE SCRATCH BUFFER FIRST!!!!\n");        \
            abort();                                                           \
        }                                                                      \
    } while (0)

static String scratchString = {};

String* GetScratchBuffer_UNSAFE_()
{
    return &scratchString;
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

    Result_String strRes = string_ctor_capacity(capacity);

    err = strRes.error_code;
    if (err)
    {
        log_error();
        return err;
    }

    scratchString = strRes.value;

    ERROR_CASE;
    return err;
}

void scratch_dtor()
{
    string_dtor(&scratchString);
}

size_t scratch_get_size()
{
    CHECK_SCRATCH_STATE();
    return scratchString.size;
}

char* scratch_get()
{
    CHECK_SCRATCH_STATE();
    return scratchString.data;
}

Str scratch_get_str()
{
    CHECK_SCRATCH_STATE();
    return str_ctor_string(scratchString);
}

Result_String scratch_copy_string()
{
    return string_copy(scratchString);
}

void scratch_pop(size_t count)
{
    CHECK_SCRATCH_STATE();

    if (count > scratchString.size)
        return;

    scratchString.size -= count;

    memset(scratchString.data + scratchString.size - count, '\0', count);
}

void scratch_clear()
{
    CHECK_SCRATCH_STATE();

    string_clear(&scratchString);
}

Error_code scratch_vprintf(const char* format, va_list args)
{
    CHECK_SCRATCH_STATE();

    ERROR_CHECKING();

    if (!format)
    {
        return ERROR_NULLPTR;
    }

    char* buffer = scratchString.data + scratchString.size;
    size_t leftCapacity = scratchString.capacity - scratchString.size;

    while (true)
    {
        va_list cpyargs = {};
        va_copy(cpyargs, args);

        int written = vsnprintf(buffer, leftCapacity, format, cpyargs);

        if (written < 0)
        {
            HANDLE_ERRNO_ERROR(ERROR_STD,
                               "Error vsnrprintf(%p, %zu, %s, ...): %s",
                               buffer,
                               leftCapacity,
                               format);
        }
        else if (written <= (int)leftCapacity)
        {
            scratchString.size += written;
            return EVERYTHING_FINE;
        }

        CHECK_ERROR(
            string_realloc(&scratchString, scratchString.capacity + written));
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
    return string_append(&scratchString, string);
}

Error_code scratch_append_str(Str string)
{
    CHECK_SCRATCH_STATE();

    return string_append_str(&scratchString, string);
}

Error_code scratch_append_string(const String string)
{
    CHECK_SCRATCH_STATE();
    return string_append_string(&scratchString, string);
}

Error_code scratch_append_char(char c)
{
    CHECK_SCRATCH_STATE();
    return string_append_char(&scratchString, c);
}
