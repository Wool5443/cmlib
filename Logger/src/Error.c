#include "../Error.h"

Error error_ctor(Error_code error_code,
                 const char* file_name,
                 const char* line_number,
                 const char* function_name);
void error_print(Error error, FILE* file);
const char* get_error_name(Error_code error_code);
