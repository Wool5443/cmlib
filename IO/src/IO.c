#include "IO.h"

Result_Str get_file_name(const char* path);
Result_Str get_folder_str(const Str path);
Result_Str get_folder(const char* path);

Result_String real_path(const char* path)
{
    ERROR_CHECKING();

    if (!path)
    {
        err = ERROR_NULLPTR;
        log_error("NULL passed as path");
        ERROR_LEAVE();
    }

    char good_path[PATH_MAX + 1] = "";

    if (!realpath(path, good_path))
    {
        HANDLE_ERRNO_ERROR(ERROR_LINUX, "Error realpath for %s: %s", path);
    }

    struct stat st = {};
    if (stat(good_path, &st) == -1)
    {
        HANDLE_ERRNO_ERROR(ERROR_LINUX, "Error stat for %s: %s", path);
    }

    size_t size = strlen(good_path);

    if ((st.st_mode & S_IFMT) == S_IFDIR)
    {
        good_path[size++] = '/';
    }

    return string_ctor_str(str_ctor_size(good_path, size));

    ERROR_CASE
    return Result_String_ctor((String){}, err);
}

Result_Str get_filename_str(const Str path)
{
    ERROR_CHECKING();

    if (!path.data)
    {
        return Result_Str_ctor((Str){}, err);
    }

    struct stat st = {};
    if (stat(path.data, &st) == -1)
    {
        HANDLE_ERRNO_ERROR(ERROR_LINUX, "Error stat for %s: %s", path.data);
    }

    if ((st.st_mode & S_IFMT) == S_IFDIR)
    {
        return Result_Str_ctor((Str){}, err);
    }

    size_t name_start = 0;

    for (size_t i = path.size - 1; i > 0; i--)
    {
        if (path.data[i] == '/')
        {
            name_start = i + 1;
            break;
        }
    }

    return Result_Str_ctor(
        (Str){
            .data = path.data + name_start,
            .size = path.size - name_start,
        },
        err);

    ERROR_CASE
    return Result_Str_ctor((Str){}, err);
}
