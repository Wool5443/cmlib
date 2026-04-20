#include "IO.h"

#include <stdio.h>

int main(void)
{
    ERROR_CHECKING();

    String *path = {}, *contents = {};

    Result_String path_res = string_ctor_capacity(get_malloc_resource(), 1);
    path = &path_res.value;

    CHECK_ERROR(path_res.error_code);
    CHECK_ERROR(real_path(&path_res.value, "."));

    path = &path_res.value;
    printf("cwd: %s\n", path->data);

    CHECK_ERROR(real_path(path, "README.md"));

    Result_Str name_res = get_filename_str(str_ctor_string(*path));
    CHECK_ERROR(name_res.error_code);

    printf("file name: ");
    str_print(name_res.value, stdout);
    printf("\n");

    Result_Str folder_res = get_folder_str(str_ctor_string(*path));
    CHECK_ERROR(folder_res.error_code);

    printf("folder name: ");
    str_print(folder_res.value, stdout);
    printf("\n");

    Result_String contents_res = string_ctor_capacity(get_malloc_resource(), 1);
    CHECK_ERROR(contents_res.error_code);
    contents = &contents_res.value;
    CHECK_ERROR(read_file(&contents_res.value, "README.md"));

    printf("README bytes: %zu\n", contents->size);

    ERROR_CASE

    string_dtor(path);
    string_dtor(contents);

    return err;
}
