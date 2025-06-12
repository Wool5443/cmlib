#include "../Error.h"

Error error_ctor(Error_code errorCode,
                 const char* fileName,
                 const char* lineNumber,
                 const char* functionName);
void error_print(Error error, FILE* file);
const char* get_error_name(Error_code errorCode);
