#include "Error.h"

Error ErrorCtor(ErrorCode errorCode, const char* fileName, size_t lineNumber, const char* functionName);
void ErrorPrint(Error error, FILE* file);
const char* GetErrorName(ErrorCode errorCode);
