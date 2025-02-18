#ifndef CMLIB_RESULT_H_
#define CMLIB_RESULT_H_

#include "Error.h" // IWYU pragma: export

#define DECLARE_RESULT_HEADER(type)                                         \
typedef struct Result_ ## type                                              \
{                                                                           \
    type value;                                                             \
    Error_code error_code;                                                  \
} Result_ ## type;                                                          \
INLINE Result_ ## type Result_ ## type ## _ctor                             \
(type value, Error_code error_code)                                         \
{                                                                           \
    return (Result_ ## type){ .value = value, .error_code = error_code };   \
}

#define DECLARE_RESULT_SOURCE(type)                                         \
Result_ ## type Result_ ## type ## _ctor(type value, Error_code error_code);

#endif // CMLIB_RESULT_H_
