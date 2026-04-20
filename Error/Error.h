/**
 * @file Error.h
 * @brief Error model and flow-control macros used across cmlib.
 *
 * This header defines project error codes, rich error metadata, formatting
 * helpers, and structured error-handling macros (`ERROR_CHECKING`,
 * `CHECK_ERROR`, `ERROR_LEAVE`, `ERROR_CASE`).
 */

#ifndef CMLIB_ERROR_H_
#define CMLIB_ERROR_H_

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "../common.h"

/** @brief Project-wide error codes generated from `include/Error_gen.h`. */
typedef enum
{
#define DEF_ERROR(code) code,

#include "include/Error_gen.h"

#undef DEF_ERROR
} Error_code;

/** @brief Captures an error code with source location and timestamp metadata.
 */
typedef struct
{
    Error_code code;      /**< @brief Error code. */
    const char* file;     /**< @brief Source file. */
    const char* line;     /**< @brief Source line. */
    const char* function; /**< @brief Source function. */
    time_t time;          /**< @brief Error timestamp. */
} Error;

/**
 * @brief Builds an `Error` record from explicit fields.
 * @param error_code Error code to attach.
 * @param file_name Source file name, typically `__FILE__`.
 * @param line_number Source line, typically stringified `__LINE__`.
 * @param function_name Source function name.
 * @return Fully initialized error object.
 */
INLINE Error error_ctor(Error_code error_code,
    const char* file_name,
    const char* line_number,
    const char* function_name);

/**
 * @brief Returns symbolic name of an `Error_code`.
 * @return Name from `Error_gen.h`, or `"ERROR_BAD_ERROR_CODE"` for unknown
 * codes.
 */
INLINE const char* get_error_name(Error_code error_code);

/**
 * @brief Formats an `Error` to a stream.
 * @param error Error record to print.
 * @param file Destination stream, must be non-NULL.
 *
 * Output contains local timestamp, optional symbolic code (when non-zero), and
 * source location (`file:line in function`).
 */
INLINE void error_print(Error error, FILE* file);

INLINE Error error_ctor(Error_code error_code,
    const char* file_name,
    const char* line_number,
    const char* function_name)
{
    return (Error) {
        .code = error_code,
        .file = file_name,
        .line = line_number,
        .function = function_name,
        .time = time(NULL),
    };
}

INLINE const char* get_error_name(Error_code error_code)
{
    switch (error_code)
    {
#define DEF_ERROR(code)                                                        \
    case code:                                                                 \
        return #code;

#include "include/Error_gen.h"

#undef DEF_ERROR

        default:
            return "ERROR_BAD_ERROR_CODE";
    }
}

INLINE void error_print(Error error, FILE* file)
{
    assert(file);

    char time_string[sizeof("dd-mm-yyyy:hh:mm:ss MSK ")];
    strftime(time_string,
        sizeof(time_string),
        "%d-%m-%Y:%H:%M:%S %Z",
        localtime(&error.time));

    fprintf(file, "%s: ", time_string);

    if (error.code)
    {
        fprintf(file, "%s ", get_error_name(error.code));
    }

    fprintf(file, "in %s:%s in %s", error.file, error.line, error.function);
}

/** @brief Declares local `err` initialized to `EVERYTHING_FINE`. */
#define ERROR_CHECKING() UNUSED Error_code err = EVERYTHING_FINE

/** @brief Declares the canonical error label target (`ERROR_CASE_:`). */
#define ERROR_CASE                                                             \
    ERROR_CASE_:;

/** @brief Jumps to the `ERROR_CASE_` label in the current function. */
#define ERROR_LEAVE() goto ERROR_CASE_

/** @brief Current source file as string literal. */
#define GET_FILE_NAME() __FILE__

/** @brief Current source line as string literal. */
#define GET_LINE() STRINGIFY_VALUE(__LINE__)

#if defined(__GNUC__) || defined(__clang__)
/** @brief Current function signature (GCC/Clang extension). */
#define GET_FUNCTION() __PRETTY_FUNCTION__
#else
/** @brief Current function name (portable fallback). */
#define GET_FUNCTION() __func__
#endif

/**
 * @brief Creates an `Error` using current source location macros.
 * @param error_code__ Error code to embed.
 */
#define CREATE_ERROR(error_code)                                               \
    error_ctor(error_code, GET_FILE_NAME(), GET_LINE(), GET_FUNCTION())

/**
 * @brief Evaluates expression into `err`; jumps to `ERROR_CASE_` on failure.
 * @param expr__ Expression returning `Error_code`.
 *
 * Requires `ERROR_CHECKING()` and `ERROR_CASE` usage in the same function.
 */
#define CHECK_ERROR(expr)                                                      \
    do                                                                         \
    {                                                                          \
        if ((err = (expr)))                                                    \
        {                                                                      \
            ERROR_LEAVE();                                                     \
        }                                                                      \
    } while (0)

#endif // CMLIB_ERROR_H_
