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
Result_String real_path(const char* path);

/**
 * @brief Get file name
 *
 * @param [in] path
 *
 * @return Result_Str
 *
 * @see Str
 * @see Error_code
 */
Result_Str get_filename_str(const Str path);

/**
 * @brief Get file name
 *
 * @param [in] path
 *
 * @return Result_Str
 *
 * @see Str
 * @see Error_code
 */
INLINE Result_Str get_file_name(const char* path);

/**
 * @brief Get folder
 *
 * @param [in] path
 *
 * @return Result_Str
 *
 * @see Str
 * @see Error_code
 */
INLINE Result_Str get_folder_str(const Str path);

/**
 * @brief Get folder
 *
 * @param [in] path
 *
 * @return Result_Str
 *
 * @see Str
 * @see Error_code
 */
INLINE Result_Str get_folder(const char* path);

INLINE Result_Str get_file_name(const char* path)
{
    return get_filename_str(str_ctor(path));
}

INLINE Result_Str get_folder_str(const Str path)
{
    if (!path.data)
    {
        return (Result_Str){};
    }

    Result_Str name = get_filename_str(path);
    if (name.error_code)
    {
        return name;
    }

    return Result_Str_ctor(
        (Str)
        {
            .data = path.data,
            .size = path.size - name.value.size,
        }, EVERYTHING_FINE
    );
}

INLINE Result_Str get_folder(const char* path)
{
    return get_folder_str(str_ctor(path));
}

#endif // CMLIB_IO_UTILS_H_
