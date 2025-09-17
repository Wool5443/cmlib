/**
 * @file Logger.h
 * @author Your Name (youremail@example.com)
 * @brief Header file for logging utilities in the system.
 *
 * This header defines logging functionalities for the system, providing various
 * log levels such as INFO, DEBUG, and ERROR. The logger supports logging to
 * different outputs (file, console, or a specific path). It allows logging
 * messages with different types and colors for easy identification, as well as
 * detailed error reporting with context. The logging system is designed to be
 * easily toggled off in release builds using the `DISABLE_LOGGING` macro.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_LOGGER_H_
#define CMLIB_LOGGER_H_

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h> // IWYU pragma: export

#include "Error.h"
#include "Result.h"
#include "include/Console_color.h"

/**
 * @enum Log_type
 * @brief Enumeration for different log types.
 *
 * This enum defines three log types:
 * - `LOG_TYPE_INFO` for informational messages.
 * - `LOG_TYPE_DEBUG` for debug messages.
 * - `LOG_TYPE_ERROR` for error messages.
 */
typedef enum
{
    LOG_TYPE_INFO,  /**< Log type for informational messages. */
    LOG_TYPE_DEBUG, /**< Log type for debug messages. */
    LOG_TYPE_ERROR, /**< Log type for error messages. */
} Log_type;

/**
 * @struct Logger
 * @brief Structure for logger configuration.
 *
 * This structure represents the logger configuration, including the output file
 * where logs are written.
 */
typedef struct
{
    FILE* file; /**< The file stream where log messages will be written (could
                   be stdout, stderr, or a file). */
} Logger;

#ifndef DISABLE_LOGGING

extern Logger cmlib_logger_; /**< The global logger instance */

/**
 * @brief Initializes the logger to log to a file specified by the given path.
 *
 * This function sets the logger to write log messages to a file at the given
 * path.
 *
 * @param path The path to the log file.
 */
void logger_init_path(const char* path);

/**
 * @brief Initializes the logger to log to a specific file pointer.
 *
 * This function sets the logger to write log messages to the provided `FILE*`.
 *
 * @param file The `FILE*` where log messages will be written.
 */
void logger_init_file(FILE* file);

/**
 * @brief Initializes the logger to log to the console.
 *
 * This function sets the logger to output log messages to the console (stdout).
 */
void logger_init_console();

/**
 * @brief Retrieves the global logger instance.
 *
 * This inline function returns a pointer to the global `Logger` instance.
 *
 * @return A pointer to the global `Logger` instance.
 */
INLINE Logger* get_logger();

/**
 * @brief Retrieves the string representation of a log type.
 *
 * This inline function returns a string corresponding to the provided log type,
 * used for formatting log messages.
 *
 * @param type The log type (`LOG_TYPE_INFO`, `LOG_TYPE_DEBUG`,
 * `LOG_TYPE_ERROR`).
 * @return A string representing the log type.
 */
INLINE const char* get_log_type_string_(Log_type type);

/**
 * @brief Retrieves the color associated with a log type.
 *
 * This inline function returns the appropriate color for the console based on
 * the log type, for visual distinction.
 *
 * @param type The log type (`LOG_TYPE_INFO`, `LOG_TYPE_DEBUG`,
 * `LOG_TYPE_ERROR`).
 * @return The `Console_color` representing the color for the log type.
 */
INLINE Console_color get_log_type_color_(Log_type type);

INLINE Logger* get_logger()
{
    return &cmlib_logger_;
}

INLINE const char* get_log_type_string_(Log_type type)
{
    switch (type)
    {
        case LOG_TYPE_INFO:
            return "INFO";
        case LOG_TYPE_DEBUG:
            return "DEBUG";
        case LOG_TYPE_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN LOG TYPE";
    }
}

INLINE Console_color get_log_type_color_(Log_type type)
{
    switch (type)
    {
        case LOG_TYPE_INFO:
            return CONSOLE_COLOR_CYAN;
        case LOG_TYPE_DEBUG:
            return CONSOLE_COLOR_YELLOW;
        case LOG_TYPE_ERROR:
            return CONSOLE_COLOR_RED;
        default:
            return CONSOLE_COLOR_MAGENTA;
    }
}

/**
 * @brief Macro to log a message of a specific type.
 *
 * This macro is used to log messages of various types (INFO, DEBUG, ERROR) with
 * support for custom messages. The log message is printed with the appropriate
 * color and format.
 *
 * @param type The log type (e.g., `LOG_TYPE_INFO`, `LOG_TYPE_DEBUG`,
 * `LOG_TYPE_ERROR`).
 * @param error The error associated with the log message.
 * @param ... Additional arguments for the log message.
 */
#define LOG_(type__, error__, ...)                                             \
    do                                                                         \
    {                                                                          \
        Log_type type = (type__);                                              \
        Error error = (error__);                                               \
        FILE* log_file = get_logger()->file;                                   \
        if (!log_file)                                                         \
        {                                                                      \
            break;                                                             \
        }                                                                      \
        set_console_color(log_file, get_log_type_color_(type));                \
                                                                               \
        fprintf(log_file, "[%s] ", get_log_type_string_(type));                \
                                                                               \
        error_print(error, log_file);                                          \
                                                                               \
        SWITCH_EMPTY(                                                          \
            ,                                                                  \
            (fprintf(log_file, "\n"),                                          \
             fprintf(log_file,                                                 \
                     "" FIRST(__VA_ARGS__) "" VA_OPT_BUT_FIRST(__VA_ARGS__)    \
                         EXPAND_BUT_FIRST(__VA_ARGS__))),                      \
            __VA_ARGS__);                                                      \
                                                                               \
        fprintf(log_file, "\n\n");                                             \
        set_console_color(log_file, CONSOLE_COLOR_WHITE);                      \
    } while (0)

/**
 * @brief Logs an informational message.
 *
 * This macro logs an informational message using the `LOG_TYPE_INFO` log type.
 *
 * @param ... Additional arguments for the log message.
 */
#define log_info(...)                                                          \
    LOG_(LOG_TYPE_INFO,                                                        \
         CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(, ) __VA_ARGS__)

/**
 * @brief Logs a debug message.
 *
 * This macro logs a debug message using the `LOG_TYPE_DEBUG` log type.
 *
 * @param ... Additional arguments for the log message.
 */
#define log_debug(...)                                                         \
    LOG_(LOG_TYPE_DEBUG,                                                       \
         CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(, ) __VA_ARGS__)

/**
 * @brief Logs an error message.
 *
 * This macro logs an error message using the `LOG_TYPE_ERROR` log type.
 *
 * @param ... Additional arguments for the log message.
 */
#define log_error(...)                                                         \
    LOG_(LOG_TYPE_ERROR, CREATE_ERROR(err) __VA_OPT__(, ) __VA_ARGS__)

#else

#define logger_init_path(...)
#define logger_init_file(...)
#define logger_init_console(...)

#define get_logger(...)

#define log_info(...)
#define log_debug(...)
#define log_error(...)

#endif // #ifndef DISABLE_LOGGING

/**
 * @brief Macro to check an error and log it if it occurs.
 *
 * This macro evaluates an expression, and if it evaluates to an error, it logs
 * the error and exits the function.
 *
 * @param expr The expression to evaluate.
 * @param ... Additional arguments for the error log.
 */
#define CHECK_ERROR_LOG(expr__, ...)                                           \
    do                                                                         \
    {                                                                          \
        if ((err = (expr__)))                                                  \
        {                                                                      \
            log_error(__VA_ARGS__);                                            \
            ERROR_LEAVE();                                                     \
        }                                                                      \
    } while (0)

/**
 * @brief Macro to handle `errno` errors and log them.
 *
 * This macro retrieves the `errno` value and logs it using `log_error`. It then
 * triggers an error leave.
 *
 * @param error The error code to log.
 * @param ... Additional arguments for the error log.
 */
#define HANDLE_ERRNO_ERROR(error__, ...)                                       \
    do                                                                         \
    {                                                                          \
        UNUSED int ern = errno;                                                \
        err = error__;                                                         \
        log_error(__VA_ARGS__, strerror(ern));                                 \
        ERROR_LEAVE();                                                         \
    } while (0)

#endif // CMLIB_LOGGER_H_
