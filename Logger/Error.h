/**
 * @file Error.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for error handling utilities.
 *
 * This header defines structures and functions used for error reporting and
 * handling in the system. It provides a mechanism to store detailed error
 * information, including the error code, file, line number, function name, and
 * timestamp of when the error occurred. The error handling utilities can be
 * used for efficient error logging and tracking.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_ERROR_H_
#define CMLIB_ERROR_H_

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "../common.h"

/**
 * @enum Error_code
 * @brief Enum for defining various error codes.
 *
 * This enum defines the different types of errors that can occur in the system.
 * The error codes are generated dynamically from an external header file
 * `Error_gen.h` and used to represent specific error conditions.
 */
typedef enum
{
#define DEF_ERROR(code) code,

#include "include/Error_gen.h"

#undef DEF_ERROR
} Error_code;

/**
 * @struct Error
 * @brief Structure to hold detailed error information.
 *
 * This structure stores information about an error, including the error code,
 * the file and line where the error occurred, the function where the error
 * happened, and the timestamp of when the error was created.
 */
typedef struct
{
    Error_code code;      /**< The error code indicating the type of error. */
    const char* file;     /**< The file where the error occurred. */
    const char* line;     /**< The line number where the error occurred. */
    const char* function; /**< The function where the error occurred. */
    time_t time;          /**< The timestamp when the error occurred. */
} Error;

/**
 * @brief Constructor for the `Error` structure.
 *
 * This function creates and initializes an `Error` structure with the given
 * parameters.
 *
 * @param error_code The error code representing the type of error.
 * @param file_name The name of the file where the error occurred.
 * @param line_number The line number where the error occurred.
 * @param function_name The name of the function where the error occurred.
 * @return A fully initialized `Error` structure.
 */
INLINE Error error_ctor(Error_code error_code,
                        const char* file_name,
                        const char* line_number,
                        const char* function_name);

/**
 * @brief Gets the name of the error code as a string.
 *
 * This function converts an error code to its string representation, which is
 * useful for logging and debugging.
 *
 * @param error_code The error code to convert.
 * @return The string representation of the error code.
 */
INLINE const char* get_error_name(Error_code error_code);

/**
 * @brief Prints detailed error information.
 *
 * This function prints the error information, including the error code, file,
 * line, function name, and timestamp, to the specified output stream.
 *
 * @param error The `Error` structure containing the error information.
 * @param file The output stream to print the error to (e.g., `stdout` or
 * `stderr`).
 */
INLINE void error_print(Error error, FILE* file);

INLINE Error error_ctor(Error_code error_code,
                        const char* file_name,
                        const char* line_number,
                        const char* function_name)
{
    return (Error) {
        .time = time(NULL),
        .code = error_code,
        .file = file_name,
        .line = line_number,
        .function = function_name,
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

/**
 * @brief Macro to initialize an error checking block.
 *
 * This macro initializes an error variable (`err`) to `EVERYTHING_FINE`, which
 * is used to check if any error occurs in the subsequent code.
 */
#define ERROR_CHECKING() UNUSED Error_code err = EVERYTHING_FINE

/**
 * @brief Macro to handle error cases and jump to the error handler.
 */
#define ERROR_CASE                                                             \
    ERROR_CASE_:;

/**
 * @brief Macro to exit the error checking block.
 *
 * This macro is used to exit an error-checking block and jump to the error
 * handling section of the code.
 */
#define ERROR_LEAVE() goto ERROR_CASE_

/**
 * @brief Macro to get the current file name.
 */
#define GET_FILE_NAME() __FILE__

/**
 * @brief Macro to get the current line number.
 */
#define GET_LINE() STRINGIFY_VALUE(__LINE__)

#if defined(__GNUC__) || defined(__clang__)
/**
 * @brief Macro to get the current function name.
 */
#define GET_FUNCTION() __PRETTY_FUNCTION__
#else
/**
 * @brief Macro to get the current function name (for non-GCC/Clang compilers).
 */
#define GET_FUNCTION() __func__
#endif

/**
 * @brief Macro to create an error object.
 *
 * This macro creates an `Error` object by capturing the current error code,
 * file, line, and function name.
 *
 * @param error_code The error code representing the type of error.
 * @return A fully initialized `Error` object.
 */
#define CREATE_ERROR(error_code__)                                             \
    error_ctor(error_code__, GET_FILE_NAME(), GET_LINE(), GET_FUNCTION())

/**
 * @brief Macro to check for errors in an expression.
 *
 * This macro evaluates the given expression and, if it returns a non-zero value
 * (indicating an error), it triggers the error handling mechanism.
 *
 * @param expr The expression to evaluate.
 */
#define CHECK_ERROR(expr__)                                                    \
    do                                                                         \
    {                                                                          \
        if ((err = (expr__)))                                                  \
        {                                                                      \
            ERROR_LEAVE();                                                     \
        }                                                                      \
    } while (0)

#endif // CMLIB_ERROR_H_
