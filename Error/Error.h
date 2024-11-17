#ifndef UTILS_H
#define UTILS_H

#include <stdio.h> // IWYU pragma: keep
#include <stddef.h>

typedef enum
{

#define DEF_ERROR(code) \
code,

#include "ErrorGen.h"

#undef DEF_ERROR

} ErrorCode;

const char* GetErrorName(ErrorCode err);

#define DECLARE_RESULT(Type)                        \
typedef struct                                      \
{                                                   \
    ErrorCode error;                                \
    Type value;                                     \
} Result ## Type

#define ERROR_CHECKING()                            \
    ErrorCode err __attribute__((unused)) = EVERYTHING_FINE

#define ERROR_CASE _ERROR_CASE_:;

#define ERROR_LEAVE() goto _ERROR_CASE_

#define LOG(...) fprintf(stderr, __VA_ARGS__)

#define LOG_ERROR()                                 \
do                                                  \
{                                                   \
    fprintf(stderr, "%s in %s:%zu in %s\n",         \
            GetErrorName(err),                      \
            __FILE__,                               \
            (size_t)__LINE__,                       \
            __PRETTY_FUNCTION__                     \
           );                                       \
} while (0)

#define LOG_IF_ERROR()                              \
do                                                  \
{                                                   \
    if (err)                                        \
        LOG_ERROR();                                \
} while (0)

#define CHECK_ERROR(expr)                           \
do                                                  \
{                                                   \
    if ((err = (expr)))                             \
    {                                               \
        LOG_ERROR();                                \
        ERROR_LEAVE();                              \
    }                                               \
} while (0)

#define RETURN(retval)                              \
do                                                  \
{                                                   \
    LOG_IF_ERROR();                                 \
    return retval;                                  \
} while (0)

#define RETURN_ERROR_IF()                           \
do                                                  \
{                                                   \
    if (err)                                        \
        RETURN(err);                                \
} while (0)

#endif // UTILS_H
