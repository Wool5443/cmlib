#include "String.h"

DECLARE_RESULT_SOURCE(String);
DECLARE_RESULT_SOURCE(Str);

Allocator Current_string_allocator = CALLOC_ALLOCATOR;

void string_set_allocator(Allocator allocator);
void string_reset_allocator();
Str str_ctor_size(const char* string, size_t size);
Str str_ctor(const char* string);
Str str_ctor_string(const String string);
int str_compare(const Str lhs, const Str rhs);
void str_print(const Str string, FILE* out);
Result_Str str_slice(const Str string, size_t start_idx, size_t end_idx);
Result_String string_ctor_str(Str string);
Result_String string_ctor(const char* string);
void string_dtor(String* this);
Result_String string_copy(const String string);
void string_clear(String* this);
int string_compare(const String lhs, const String rhs);
Error_code string_append(String* this, const char* string);
Error_code string_append_string(String* this, const String string);
Error_code string_append_char(String* this, char ch);
Result_Str string_slice(const String this, size_t start_idx, size_t end_idx);
Error_code string_printf(String* this, const char* format, ...);
Result_String string_ctor_printf(const char* format, ...);
Result_String string_ctor_vprintf(const char* format, va_list args);

Result_String string_ctor_capacity(size_t capacity)
{
    ERROR_CHECKING();

    char* data = NULL;

    if (capacity == 0)
    {
        err = ERROR_NULLPTR;
        log_error("0 passed as capacity");
        ERROR_LEAVE();
    }

    data = Current_string_allocator.allocate(capacity + 1);

    if (!data)
    {
        HANDLE_ERRNO_ERROR(ERROR_NO_MEMORY,
                           "Failed to create string with capacity %zu: %s",
                           capacity);
    }

    ERROR_CASE
    return Result_String_ctor(
        (String) {
            .allocator = err == EVERYTHING_FINE ? Current_string_allocator
                                                : (Allocator) {},
            .data = data,
            .size = 0,
            .capacity = data ? capacity : 0,
        },
        err);
}

Error_code string_realloc(String* this, size_t new_capacity)
{
    ERROR_CHECKING();

    if (!this)
    {
        err = ERROR_NULLPTR;
        log_error("NULL passed as this");
        ERROR_LEAVE();
    }

    if (new_capacity == 0)
    {
        err = ERROR_BAD_VALUE;
        log_error("0 passed as new_capacity");
        ERROR_LEAVE();
    }

    char* new_data = NULL;

    if (this->capacity >= new_capacity)
        return EVERYTHING_FINE;

    Allocator allocator = this->allocator.allocate ? this->allocator
                                                   : Current_string_allocator;
    new_data = allocator.allocate(new_capacity + 1);

    if (!new_data)
    {
        HANDLE_ERRNO_ERROR(ERROR_NO_MEMORY, "Failed to realloc string: %s");
    }

    if (this->data)
    {
        memcpy(new_data, this->data, this->size);
    }
    new_data[this->size] = '\0';

    allocator.free(this->data);

    *this = (String) {.allocator = allocator,
                      .data = new_data,
                      .size = this->size,
                      .capacity = new_capacity};

    ERROR_CASE
    return err;
}

Error_code string_append_str(String* this, Str string)
{
    ERROR_CHECKING();

    if (!this)
    {
        err = ERROR_NULLPTR;
        log_error("NULL passed as this");
        ERROR_LEAVE();
    }

    if (!string.data)
        return EVERYTHING_FINE;
    if (string.size == 0)
        return EVERYTHING_FINE;

    size_t new_size = this->size + string.size;

    if (new_size > this->capacity)
    {
        if ((err = string_realloc(this, new_size)))
            return err;
    }

    memcpy(this->data + this->size, string.data, string.size);

    this->size = new_size;
    this->data[new_size] = '\0';

    ERROR_CASE;
    return err;
}

Result_String read_file(const char* path)
{
    ERROR_CHECKING();

    FILE* file = NULL;
    String string = {};

    if (!path)
    {
        err = ERROR_BAD_FILE;
        log_error("NULL passed as file path");
        ERROR_LEAVE();
    }

    file = fopen(path, "r");

    if (!file)
    {
        HANDLE_ERRNO_ERROR(ERROR_BAD_FILE, "Failed to open file %s: %s", path);
    }

    struct stat st = {};
    if (fstat(fileno(file), &st) == -1)
    {
        HANDLE_ERRNO_ERROR(ERROR_BAD_FILE, "fstat error: %s");
    }

    size_t file_size = st.st_size;

    Result_String string_res = string_ctor_capacity(file_size + 1);
    if ((err = string_res.error_code))
    {
        ERROR_LEAVE();
    }

    string = string_res.value;

    if (fread(string.data, 1, file_size, file) != file_size)
    {
        HANDLE_ERRNO_ERROR(ERROR_BAD_FILE, "Failed to read file: %s");
    }
    fclose(file);

    string.size = file_size;

    return (Result_String) {
        string,
        EVERYTHING_FINE,
    };

    ERROR_CASE
    if (file)
        fclose(file);
    string_dtor(&string);

    return (Result_String) {{}, err};
}

Error_code string_vprintf(String* this, const char* format, va_list args)
{
    ERROR_CHECKING();

    if (!format)
    {
        err = ERROR_NULLPTR;
        log_error("NULL passed as format string");
        ERROR_LEAVE();
    }

    size_t format_length = strlen(format);

    if (format_length == 0)
    {
        return EVERYTHING_FINE;
    }

    size_t capacity = format_length * 2;

    CHECK_ERROR(string_realloc(this, this->capacity + capacity));

    // Try until it fits
    while (true)
    {
        va_list cpargs = {};
        va_copy(cpargs, args);

        int written =
            vsnprintf(this->data + this->size, capacity, format, cpargs);

        if (written < 0)
        {
            HANDLE_ERRNO_ERROR(ERROR_STD, "Error vsnprintf: %s");
        }
        else if (written <= (int)capacity)
        {
            this->size += written;
            break;
        }

        capacity = written + 1;
        CHECK_ERROR(string_realloc(this, this->capacity + capacity));
    }

    return EVERYTHING_FINE;

    ERROR_CASE

    return err;
}

Error_code string_replace_all(String* this, Str from, Str to)
{
    ERROR_CHECKING();

    if (!this)
    {
        err = ERROR_NULLPTR;
        log_error("NULL passed as this");
        ERROR_LEAVE();
    }

    if (!from.data)
    {
        return err;
    }

    if (to.data == NULL)
    {
        to.data = "";
    }

    ptrdiff_t change = to.size - from.size;

    size_t count_occurences = 0;
    const char* found = strstr(this->data, from.data);
    while (found)
    {
        count_occurences++;
        found = strstr(found + 1, from.data);
    }
    if (count_occurences == 0)
    {
        return err;
    }
    size_t new_size = this->size + count_occurences * change;

    if (change < 0)
    {
        char* writer = this->data;
        const char* reader = writer;

        while (reader < this->data + this->size + 1)
        {
            if (str_compare(str_ctor_size(reader, from.size), from) == 0)
            {
                memcpy(writer, to.data, to.size);
                writer += to.size;
                reader += from.size;
            }
            else
            {
                *(writer++) = *(reader++);
            }
        }
        this->size = new_size;
    }
    else
    {
        // AAAAAAAAAAAAABBBAAAA BBB -> DDDDD
        //              DDDDDAAAA BBB -> DDDDD
        String s = {};
        if (this->capacity >= new_size)
        {
            s = *this;
        }
        else
        {
            Result_String new_string_res = string_ctor_capacity(new_size);
            CHECK_ERROR(new_string_res.error_code);
            s = new_string_res.value;
            memcpy(s.data, this->data, this->size);
            s.size = this->size;
            string_dtor(this);
        }

        for (char *old = s.data + s.size, *new = s.data + new_size;
             old >= s.data;
             old--, new --)
        {
            if (str_compare(str_ctor_size(old, from.size), from) == 0)
            {
                memcpy(new - change, to.data, to.size);
                new -= change;
            }
            else
            {
                *new = *old;
            }
        }
        s.size = new_size;
        *this = s;
    }

    ERROR_CASE;
    return err;
}
