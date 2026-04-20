/**
 * @file Result.h
 * @brief Generic result wrapper macros.
 *
 * A generated `Result_T` stores `value` and `error_code`. The convention used
 * by this project is: `error_code == EVERYTHING_FINE` means `value` is valid.
 * Non-zero `error_code` means operation failed and `value` may be defaulted.
 */

#ifndef CMLIB_RESULT_H_
#define CMLIB_RESULT_H_

#include "Error.h" // IWYU pragma: export

/**
 * @brief Declares `Result_<type>` and an inline constructor.
 * @param type Payload type stored in the result.
 *
 * Generated constructor has no side effects and performs plain field
 * assignment.
 */
#define DECLARE_RESULT_HEADER(cmlib_macroarg_type)                             \
    typedef struct Result_##cmlib_macroarg_type                                \
    {                                                                          \
        cmlib_macroarg_type value;                                             \
        Error_code error_code;                                                 \
    } Result_##cmlib_macroarg_type;                                            \
    INLINE Result_##cmlib_macroarg_type Result_##cmlib_macroarg_type##_ctor(   \
        cmlib_macroarg_type value, Error_code error_code)                      \
    {                                                                          \
        return (Result_##cmlib_macroarg_type) {                                \
            .value = value, .error_code = error_code};                         \
    }

/**
 * @brief Declares a non-inline constructor prototype for `Result_<type>`.
 * @param type Payload type stored in the result.
 */
#define DECLARE_RESULT_SOURCE(cmlib_macroarg_type)                             \
    Result_##cmlib_macroarg_type Result_##cmlib_macroarg_type##_ctor(          \
        cmlib_macroarg_type value, Error_code error_code);

#endif // CMLIB_RESULT_H_
