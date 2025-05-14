#include "IO.h"

Str GetFileName(const char* path);
Str GetFolderStr(const Str path);
Str GetFolder(const char* path);

Result_String RealPath(const char* path)
{
    ERROR_CHECKING();

    if (!path)
    {
        err = ERROR_NULLPTR;
        log_error("NULL passed as path");
        ERROR_LEAVE();
    }

    char goodPath[PATH_MAX + 1] = "";

    if (!realpath(path, goodPath))
    {
        HANDLE_ERRNO_ERROR(ERROR_LINUX, "Error realpath for %s: %s", path);
    }

    struct stat st = {};
    if (stat(goodPath, &st) == -1)
    {
        HANDLE_ERRNO_ERROR(ERROR_LINUX, "Error stat for %s: %s", path);
    }

    size_t size = strlen(goodPath);

    if ((st.st_mode & S_IFMT) == S_IFDIR)
    {
        goodPath[size++] = '/';
    }

    return string_ctor_str(str_ctor_size(goodPath, size));

ERROR_CASE
    return Result_String_ctor((String){}, err);
}

Str GetFileNameStr(const Str path)
{
    if (!path.data)
    {
        return (Str){};
    }

    size_t nameStart = 0;

    for (size_t i = path.size - 1; i > 0; i--)
    {
        if (path.data[i] == '/')
        {
            nameStart = i + 1;
            break;
        }
    }

    return (Str)
    {
        .data = path.data + nameStart,
        .size = path.size - nameStart,
    };
}
