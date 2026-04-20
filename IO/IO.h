/**
 * @file IO.h
 * @brief Filesystem path helpers and file reader.
 *
 * APIs in this header report failure with `Error_code` or `Result_*` values.
 * Successful calls use `EVERYTHING_FINE`.
 */

#ifndef CMLIB_IO_UTILS_H_
#define CMLIB_IO_UTILS_H_

#include "Error.h"
#include "String.h"

/**
 * @brief Resolves path to canonical absolute form.
 * @param out Destination string; must already have a memory resource.
 * @param path Input path, must be non-NULL and resolvable by `realpath`.
 * @return `EVERYTHING_FINE` or error code.
 *
 * If resolved target is a directory, a trailing `/` is appended in @p out.
 */
Error_code real_path(String* out, const char* path);

/**
 * @brief Reads full file contents into an existing owned string.
 * @param out Destination string; must already have a memory resource.
 * @param path File path.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code read_file(String* out, const char* path);

/**
 * @brief Returns filename view from path.
 * @param path Path view. `path.data` may be NULL.
 * @return Borrowed `Str` into @p path on success.
 *
 * Behavior details:
 * - Performs `stat(path.data)`; on failure returns `ERROR_LINUX`.
 * - If path points to directory, returns empty `Str` with success code.
 * - Returned view is non-owning; input memory must outlive use.
 *
 */
Result_Str get_filename_str(const Str path);

/**
 * @brief Convenience wrapper around `get_filename_str(str_ctor(path))`.
 */
Result_Str get_filename(const char* path);

/**
 * @brief Returns folder part of a path as borrowed slice.
 * @param path Path view.
 * @return Result with `Str` `[path.data, path.size - filename.size)`.
 */
Result_Str get_folder_str(const Str path);

/**
 * @brief Convenience wrapper around `get_folder_str(str_ctor(path))`.
 */
Result_Str get_folder(const char* path);

#endif // CMLIB_IO_UTILS_H_
