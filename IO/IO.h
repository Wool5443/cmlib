#ifndef CMLIB_IO_UTILS_H_
#define CMLIB_IO_UTILS_H_

#include <limits.h>

#include "String.h"

INLINE ResultString RealPath(const char* path)
{
    ERROR_CHECKING();

    assert(path);

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

    return StringCtorFromStr(StrCtorSize(goodPath, size));

ERROR_CASE
    return ResultStringCtor((String){}, err);
}

INLINE Str GetFileNameStr(const Str path)
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

INLINE Str GetFileName(const char* path)
{
    return GetFileNameStr(StrCtor(path));
}

INLINE Str GetFolderStr(const Str path)
{
    if (!path.data)
    {
        return (Str){};
    }

    Str name = GetFileNameStr(path);

    return (Str)
    {
        .data = path.data,
        .size = path.size - name.size,
    };
}

INLINE Str GetFolder(const char* path)
{
    return GetFolderStr(StrCtor(path));
}

#endif // CMLIB_IO_UTILS_H_
