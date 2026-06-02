#include "IO.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "details/CountingMalloc.h"

Error_code real_path(String* out, const char* path)
{
    ERROR_CHECKING();

    if (!out || !out->memory_resource || !path)
    {
        err = ERROR_NULLPTR;
        ERROR_LEAVE();
    }

    char good_path[PATH_MAX + 1] = "";

    if (!realpath(path, good_path))
    {
        err = ERROR_LINUX;
        ERROR_LEAVE();
    }

    struct stat st = {};
    if (stat(good_path, &st) == -1)
    {
        err = ERROR_LINUX;
        ERROR_LEAVE();
    }

    size_t size = strlen(good_path);

    if ((st.st_mode & S_IFMT) == S_IFDIR)
    {
        good_path[size++] = '/';
    }

    string_clear(out);
    CHECK_ERROR(string_append_str(out, str_ctor_size(good_path, size)));
    return EVERYTHING_FINE;

    ERROR_CASE
    return err;
}

Error_code read_file(String* out, const char* path)
{
    ERROR_CHECKING();

    FILE* file = NULL;

    if (!out || !out->memory_resource || !path)
    {
        err = ERROR_BAD_FILE;
        ERROR_LEAVE();
    }

    file = fopen(path, "r");

    if (!file)
    {
        err = ERROR_BAD_FILE;
        ERROR_LEAVE();
    }

    struct stat st = {};
    if (fstat(fileno(file), &st) == -1)
    {
        err = ERROR_BAD_FILE;
        ERROR_LEAVE();
    }

    size_t file_size = st.st_size;

    if ((err = string_realloc(out, file_size + 1)))
    {
        ERROR_LEAVE();
    }

    if (fread(out->data, 1, file_size, file) != file_size)
    {
        err = ERROR_BAD_FILE;
        ERROR_LEAVE();
    }
    fclose(file);

    out->size = file_size;
    out->data[out->size] = '\0';

    return EVERYTHING_FINE;

    ERROR_CASE
    if (file)
    {
        fclose(file);
    }

    return err;
}

Result_Str get_filename_str(const Str path)
{
    ERROR_CHECKING();

    constexpr size_t MAX_PATH_STACK_SIZE = 1023;
    static char path_buf[MAX_PATH_STACK_SIZE + 1] = {};
    char* null_terminated_path = path_buf;

    Result_Str res = {};

    if (!path.data)
    {
        err = ERROR_NULLPTR;
        ERROR_LEAVE();
    }

    if (path.size > MAX_PATH_STACK_SIZE)
    {
        null_terminated_path = (char*)cmlib_details_calloc(path.size + 1, 1);
        if (!null_terminated_path)
        {
            err = ERROR_STD;
            ERROR_LEAVE();
        }
    }
    memcpy(null_terminated_path, path.data, path.size);
    null_terminated_path[path.size] = '\0';

    struct stat st = {};
    if (stat(null_terminated_path, &st) == -1)
    {
        err = ERROR_LINUX;
        ERROR_LEAVE();
    }

    if ((st.st_mode & S_IFMT) == S_IFDIR)
    {
        err = ERROR_BAD_FOLDER;
        ERROR_LEAVE();
    }

    size_t name_start = 0;

    for (size_t i = path.size - 1; i-- > 0;)
    {
        if (path.data[i] == '/')
        {
            name_start = i + 1;
            break;
        }
    }

    res.value = (Str) {
        .data = path.data + name_start,
        .size = path.size - name_start,
    };

    ERROR_CASE

    if (null_terminated_path != path_buf)
    {
        cmlib_details_free(null_terminated_path);
    }

    res.error_code = err;

    return res;
}

Result_Str get_filename(const char* path)
{
    return get_filename_str(str_ctor(path));
}

Result_Str get_folder_str(const Str path)
{
    if (!path.data)
    {
        return (Result_Str) {};
    }

    Result_Str name = get_filename_str(path);
    if (name.error_code)
    {
        return name;
    }

    return Result_Str_ctor(
        (Str) {
            .data = path.data,
            .size = path.size - name.value.size,
        },
        EVERYTHING_FINE);
}

Result_Str get_folder(const char* path)
{
    return get_folder_str(str_ctor(path));
}
