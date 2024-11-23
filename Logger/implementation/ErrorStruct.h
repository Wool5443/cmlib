#ifndef CMLIB_ERROR_STRUCT_H
#define CMLIB_ERROR_STRUCT_H

#include <assert.h>

#include "Types.h"
#include "../../common.h"

INLINE Error ErrorCtor(ErrorCode errorCode,
                       const char* fileName,
                       size_t lineNumber,
                       const char* functionName)
{
    time_t t = time(NULL);

    return (Error)
    {
        .time = t,
        .code = errorCode,
        .file = fileName,
        .line = lineNumber,
        .function = functionName,
    };
}

INLINE const char* GetErrorName(ErrorCode errorCode)
{
    switch (errorCode)
    {

#define DEF_ERROR(code) \
case code: return #code;

#include "ErrorGen.h"

#undef DEF_ERROR

        default:
            return "ERROR_BAD_ERROR_CODE";

    }
}

INLINE void ErrorPrint(Error error, FILE* file)
{
    assert(file);

    char timeString[sizeof("dd-mm-yyyy:hh:mm:ss MSK ")];
    strftime(timeString, sizeof(timeString), "%d-%m-%Y:%H:%M:%S %Z",
            localtime(&error.time));

    fprintf(file, "%s: ", timeString);

    if (error.code) fprintf(file, "%s ", GetErrorName(error.code));

    fprintf(file,
            "in %s:%zu in %s",
            error.file,
            error.line,
            error.function
           );
}

#endif // CMLIB_ERROR_STRUCT_H