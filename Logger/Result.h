/**
 * @file Result.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for result handling utilities.
 *
 * This header defines macros and structures used for handling results of
 * operations in the system. It provides a way to represent both the result of
 * an operation (such as a computed value) and an associated error code. The
 * `Result_<type>` structure is used to hold both the value and the error code,
 * which helps in distinguishing successful results from errors in a clean and
 * structured way.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_RESULT_H_
#define CMLIB_RESULT_H_

#include "Error.h" // IWYU pragma: export

/**
 * @brief Macro to declare a result structure and its constructor for a given
 * type.
 *
 * This macro declares a `Result_<type>` structure that holds a value of the
 * specified type and an error code. It also defines a constructor function
 * (`Result_<type>_ctor`) to initialize the result structure with a value and an
 * error code.
 *
 * @param type The type of the value that the result will hold.
 */
#define DECLARE_RESULT_HEADER(type)                                            \
    typedef struct Result_##type                                               \
    {                                                                          \
        type value;                                                            \
        Error_code error_code;                                                 \
    } Result_##type;                                                           \
    INLINE Result_##type Result_##type##_ctor(type value,                      \
                                              Error_code error_code)           \
    {                                                                          \
        return (Result_##type){.value = value, .error_code = error_code};      \
    }

/**
 * @brief Macro to declare a constructor for a result structure of a given type.
 *
 * This macro declares the constructor function prototype for the
 * `Result_<type>` structure. It is intended to be used when defining the
 * constructor in the source file.
 *
 * @param type The type of the value that the result will hold.
 */
#define DECLARE_RESULT_SOURCE(type)                                            \
    Result_##type Result_##type##_ctor(type value, Error_code error_code);

#endif // CMLIB_RESULT_H_
