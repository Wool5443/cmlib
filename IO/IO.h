#ifndef CMLIB_IO_UTILS_H_
#define CMLIB_IO_UTILS_H_

#include "String.h"

Error_code real_path(String* out, const char* path);

Error_code read_file(String* out, const char* path);

Result_Str get_filename_str(const Str path);

Result_Str get_filename(const char* path);

Result_Str get_folder_str(const Str path);

Result_Str get_folder(const char* path);

#endif // CMLIB_IO_UTILS_H_
