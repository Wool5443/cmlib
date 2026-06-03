#include "../Error.h"

Error error_ctor(ErrorCode, const char*, const char*, const char*);
void error_print(Error, FILE*);
const char* get_error_name(ErrorCode);
