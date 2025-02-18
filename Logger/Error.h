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

} Error_code;

typedef struct
{
    Error_code   code;
    const char* file;
    const char* line;
    const char* function;
    time_t      time;
} Error;

INLINE Error error_ctor(Error_code error_code,
                        const char* fileName,
                        const char* lineNumber,
                        const char* functionName)
{
    return (Error)
    {
        .time = time(NULL),
        .code = error_code,
        .file = fileName,
        .line = lineNumber,
        .function = functionName,
    };
}

INLINE const char* get_error_name(Error_code error_code)
{
    switch (error_code)
    {

#define DEF_ERROR(code) \
case code: return #code;

#include "include/ErrorGen.h"

#undef DEF_ERROR

        default:
            return "ERROR_BAD_ERROR_CODE";

    }
}

INLINE void error_print(Error error, FILE* file)
{
    assert(file);

    char timeString[sizeof("dd-mm-yyyy:hh:mm:ss MSK ")];
    strftime(timeString, sizeof(timeString), "%d-%m-%Y:%H:%M:%S %Z",
             localtime(&error.time));

    fprintf(file, "%s: ", timeString);

    if (error.code) fprintf(file, "%s ", get_error_name(error.code));

    fprintf(
        file,
        "in %s:%s in %s",
        error.file,
        error.line,
        error.function
   );
}

#define ERROR_CHECKING() UNUSED Error_code err = EVERYTHING_FINE

#define ERROR_CASE ERROR_CASE_:;

#define ERROR_LEAVE() goto ERROR_CASE_

#define GET_FILE_NAME() __FILE__
#define GET_LINE()      STRINGIFY_VALUE(__LINE__)

#if defined(__GNUC__) || defined(__clang__)
    #define GET_FUNCTION()  __PRETTY_FUNCTION__
#else
    #define GET_FUNCTION()  __func__
#endif

#define CREATE_ERROR(error_code) \
error_ctor(error_code, GET_FILE_NAME(), GET_LINE(), GET_FUNCTION())

#define CHECK_ERROR(expr)                                               \
do                                                                      \
{                                                                       \
    if ((err = (expr)))                                                 \
    {                                                                   \
        ERROR_LEAVE();                                                  \
    }                                                                   \
} while (0)

#endif // CMLIB_ERROR_H_
