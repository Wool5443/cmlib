#ifndef CMLIB_IO_UTILS_H_
#define CMLIB_IO_UTILS_H_

#include <limits.h>

#include "String.h"

/**
 * @brief Get real path
 *
 * @param [in] path
 *
 * @return Result_String
 *
 * @see String
 * @see Error_code
 */
Result_String RealPath(const char* path);

/**
 * @brief Get file name
 *
 * @param [in] path
 *
 * @return Result_String
 *
 * @see Str
 */
Str GetFileNameStr(const Str path);

/**
 * @brief Get file name
 *
 * @param [in] path
 *
 * @return Str
 *
 * @see Str
 */
INLINE Str GetFileName(const char* path);

/**
 * @brief Get folder
 *
 * @param [in] path
 *
 * @return Str
 *
 * @see Str
 */
INLINE Str GetFolderStr(const Str path);

/**
 * @brief Get folder
 *
 * @param [in] path
 *
 * @return Str
 *
 * @see Str
 */
INLINE Str GetFolder(const char* path);

INLINE Str GetFileName(const char* path)
{
    return GetFileNameStr(str_ctor(path));
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
    return GetFolderStr(str_ctor(path));
}

#endif // CMLIB_IO_UTILS_H_
