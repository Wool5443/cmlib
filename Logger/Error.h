#ifndef CMLIB_ERROR_H_
#define CMLIB_ERROR_H_

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "../common.h"

typedef enum
{

#define DEF_ERROR(code) \
code,

#include "include/ErrorGen.h"

#undef DEF_ERROR

} ErrorCode;

typedef struct
{
    ErrorCode   code;
    const char* file;
    size_t      line;
    const char* function;
    time_t      time;
} Error;

INLINE Error ErrorCtor(ErrorCode errorCode,
                       const char* fileName,
                       size_t lineNumber,
                       const char* functionName)
{
    return (Error)
    {
        .time = time(NULL),
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

#include "include/ErrorGen.h"

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

    fprintf(
        file,
        "in %s:%zu in %s",
        error.file,
        error.line,
        error.function
   );
}

#define ERROR_CHECKING()                                                \
    UNUSED ErrorCode err = EVERYTHING_FINE

#define ERROR_CASE ERROR_CASE_:;

#define ERROR_LEAVE() goto ERROR_CASE_

#define GET_FILE_NAME() __FILE__
#define GET_LINE()      __LINE__

#if defined(__GNUC__) || defined(__clang__)
    #define GET_FUNCTION()  __PRETTY_FUNCTION__
#else
    #define GET_FUNCTION()  __func__
#endif

#define CREATE_ERROR(errorCode) \
ErrorCtor(errorCode, GET_FILE_NAME(), GET_LINE(), GET_FUNCTION())

#define CHECK_ERROR(expr)                                               \
do                                                                      \
{                                                                       \
    if ((err = (expr)))                                                 \
    {                                                                   \
        ERROR_LEAVE();                                                  \
    }                                                                   \
} while (0)

#endif // CMLIB_ERROR_H_
