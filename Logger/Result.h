#ifndef CMLIB_RESULT_H_
#define CMLIB_RESULT_H_

#include "Error.h"

#define DECLARE_RESULT_HEADER(Type)                                     \
typedef struct Result ## Type                                           \
{                                                                       \
    Type value;                                                         \
    ErrorCode errorCode;                                                \
} Result ## Type;                                                       \
INLINE Result ## Type Result ## Type ## Ctor                            \
(Type value, ErrorCode errorCode)                                       \
{                                                                       \
    return (Result ## Type){ .value = value, .errorCode = errorCode };  \
}

#define DECLARE_RESULT_SOURCE(Type)                                     \
Result ## Type Result ## Type ## Ctor(Type value, ErrorCode errorCode);

#endif // CMLIB_RESULT_H_
