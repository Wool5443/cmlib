/**
 * @file IO.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for input/output utility functions for path and filename
 * handling.
 *
 * This header defines functions for manipulating file and directory paths. The
 * provided functions allow users to retrieve the real path, file name, and
 * directory from a given path. These utilities can help in parsing and handling
 * paths in a system-agnostic manner. The functions are designed to provide
 * error-safe operations and return results in a standardized format using
 * `Result` structures.
 *
 * The functions support working with both string (`Str`) and `const char*` path
 * representations, offering flexibility in handling file system paths.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_IO_UTILS_H_
#define CMLIB_IO_UTILS_H_

#include <limits.h>

#include "String.h"

/**
 * @brief Retrieves the real, absolute path of a given file or directory.
 *
 * This function takes a file path as input and returns the real, absolute path
 * after resolving any symbolic links, relative paths, and ensuring it's fully
 * qualified.
 *
 * @param path The input path to resolve.
 *
 * @return A `Result_String` containing either the resolved absolute path or an
 * error code. The `Result_String` will contain a string representation of the
 * real path or an error message.
 *
 * @see String
 * @see Error_code
 */
Result_String real_path(const char* path);

/**
 * @brief Retrieves the file name from a given path.
 *
 * This function extracts the file name from the given file path, removing any
 * directory structure or path components.
 *
 * @param path The input path from which the file name should be extracted.
 *
 * @return A `Result_Str` containing either the extracted file name or an error
 * code. The `Result_Str` will contain the name of the file or an error message.
 *
 * @see Str
 * @see Error_code
 */
Result_Str get_filename_str(const Str path);

/**
 * @brief Retrieves the file name from a given path.
 *
 * This function is similar to `get_filename_str`, but it works with a `const
 * char*` path representation. It extracts the file name from the path by
 * removing any directory structure.
 *
 * @param path The input path from which the file name should be extracted.
 *
 * @return A `Result_Str` containing either the extracted file name or an error
 * code. The `Result_Str` will contain the name of the file or an error message.
 *
 * @see Str
 * @see Error_code
 */
INLINE Result_Str get_file_name(const char* path);

/**
 * @brief Retrieves the directory portion (folder) from a given path.
 *
 * This function extracts the directory portion from the path, effectively
 * removing the file name.
 *
 * @param path The input path from which the directory (folder) should be
 * extracted.
 *
 * @return A `Result_Str` containing either the extracted folder path or an
 * error code. The `Result_Str` will contain the folder path or an error
 * message.
 *
 * @see Str
 * @see Error_code
 */
INLINE Result_Str get_folder_str(const Str path);

/**
 * @brief Retrieves the directory portion (folder) from a given path.
 *
 * This function works similarly to `get_folder_str`, but it works with a `const
 * char*` path representation. It extracts the folder portion from the path by
 * removing the file name.
 *
 * @param path The input path from which the folder should be extracted.
 *
 * @return A `Result_Str` containing either the extracted folder path or an
 * error code. The `Result_Str` will contain the folder path or an error
 * message.
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

INLINE Result_Str get_folder(const char* path)
{
    return get_folder_str(str_ctor(path));
}

#endif // CMLIB_IO_UTILS_H_
