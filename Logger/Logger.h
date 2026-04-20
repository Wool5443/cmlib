/**
 * @file Logger.h
 * @brief Logging API and logging-aware error helper macros.
 *
 * Unless `DISABLE_LOGGING` is defined, logging writes formatted records to the
 * global logger stream. When disabled, logging init/getter/log macros become
 * no-ops. Error codes and non-logging error flow live in `Error.h`.
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

/** @brief Severity/type tag used by logger formatting helpers. */
typedef enum
{
    LOG_TYPE_INFO,  /**< @brief Informational message. */
    LOG_TYPE_DEBUG, /**< @brief Debug message. */
    LOG_TYPE_ERROR, /**< @brief Error message. */
} Log_type;

/** @brief Global logger state. */
typedef struct
{
    FILE* file; /**< @brief Output stream for log records. */
} Logger;

#ifndef DISABLE_LOGGING

extern Logger cmlib_logger_; /**< @brief Global logger instance. */

/**
 * @brief Opens/assigns logger output to file at @p path.
 * @param path Path passed to `fopen(path, "w")`.
 *
 * On open failure, writes a diagnostic to `stderr` and leaves current logger
 * stream unchanged. On success, output is unbuffered and cleanup is registered
 * via `atexit`.
 */
void logger_init_path(const char* path);

/**
 * @brief Assigns logger output stream directly.
 * @param file Non-NULL target stream.
 *
 * Side effect: disables buffering on @p file with `setbuf(file, NULL)`.
 */
void logger_init_file(FILE* file);

/** @brief Sends logs to `stderr`. */
void logger_init_console();

/** @brief Returns pointer to global logger storage. */
INLINE Logger* get_logger();

/** @brief Returns stable string tag for a log type. */
INLINE const char* get_log_type_string_(Log_type type);

/** @brief Returns console color used for a log type. */
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
 * @brief Core logging macro used by `log_info`, `log_debug`, and `log_error`.
 *
 * Format:
 * `[TYPE] <error metadata>`
 * `<optional user message>`
 *
 * If logger stream is NULL, this macro is a no-op. Requires `err` variable only
 * when caller passes `CREATE_ERROR(err)` (as `log_error` does).
 *
 * Writes formatted log records to configured logger stream when available.
 */
#define LOG_(cmlib_macroarg_type, cmlib_macroarg_error, ...)                   \
    do                                                                         \
    {                                                                          \
        Log_type type = (cmlib_macroarg_type);                                 \
        Error error = (cmlib_macroarg_error);                                  \
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
        SWITCH_EMPTY(,                                                         \
            (fprintf(log_file, "\n"),                                          \
                fprintf(log_file,                                              \
                    "" FIRST(__VA_ARGS__) "" VA_OPT_BUT_FIRST(__VA_ARGS__)     \
                        EXPAND_BUT_FIRST(__VA_ARGS__))),                       \
            __VA_ARGS__);                                                      \
                                                                               \
        fprintf(log_file, "\n\n");                                             \
        set_console_color(log_file, CONSOLE_COLOR_WHITE);                      \
    } while (0)

/**
 * @brief Logs a message with `LOG_TYPE_INFO` and `EVERYTHING_FINE` metadata.
 */
#define log_info(...)                                                          \
    LOG_(LOG_TYPE_INFO,                                                        \
        CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(, ) __VA_ARGS__)

/**
 * @brief Logs a message with `LOG_TYPE_DEBUG` and `EVERYTHING_FINE` metadata.
 */
#define log_debug(...)                                                         \
    LOG_(LOG_TYPE_DEBUG,                                                       \
        CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(, ) __VA_ARGS__)

/**
 * @brief Logs with `LOG_TYPE_ERROR`, capturing current `err`.
 *
 * Requires local variable `err` to be visible in scope.
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
 * @brief Evaluates expression into `err`; logs and jumps on failure.
 * @param expr__ Expression returning `Error_code`.
 * @param ... Optional message payload for `log_error`.
 *
 * Requires `ERROR_CHECKING()` and `ERROR_CASE` in the same function.
 *
 * Logs ERROR when evaluated expression returns non-zero.
 */
#define CHECK_ERROR_LOG(cmlib_macroarg_expr, ...)                              \
    do                                                                         \
    {                                                                          \
        if ((err = (cmlib_macroarg_expr)))                                     \
        {                                                                      \
            log_error(__VA_ARGS__);                                            \
            ERROR_LEAVE();                                                     \
        }                                                                      \
    } while (0)

/**
 * @brief Converts `errno` path into project error and jumps to error label.
 * @param error__ Project error code assigned to `err`.
 * @param ... Format string + args; appends `strerror(errno)` automatically.
 *
 * Logs ERROR before jumping to error label.
 */
#define HANDLE_ERRNO_ERROR(cmlib_macroarg_error, ...)                          \
    do                                                                         \
    {                                                                          \
        UNUSED int ern = errno;                                                \
        err = cmlib_macroarg_error;                                            \
        log_error(__VA_ARGS__, strerror(ern));                                 \
        ERROR_LEAVE();                                                         \
    } while (0)

#endif // CMLIB_LOGGER_H_
