#ifndef CMLIB_LOGGER_H_
#define CMLIB_LOGGER_H_

#include <errno.h>
#include <stdio.h>
#include <string.h> // IWYU pragma: export

#include "../common.h"
#include "Error.h"
#include "include/Console_color.h"

typedef enum
{
    LOG_TYPE_INFO,
    LOG_TYPE_DEBUG,
    LOG_TYPE_ERROR,
} Log_type;

typedef struct
{
    FILE* file;
} Logger;

#ifndef DISABLE_LOGGING

extern Logger cmlib_logger_;

void logger_init_path(const char* path);

void logger_init_file(FILE* file);

void logger_init_console();

INLINE Logger* get_logger();

INLINE const char* get_log_type_string_(Log_type type);

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

#define log_info(...)                                                          \
    LOG_(LOG_TYPE_INFO,                                                        \
        CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(, ) __VA_ARGS__)

#define log_debug(...)                                                         \
    LOG_(LOG_TYPE_DEBUG,                                                       \
        CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(, ) __VA_ARGS__)

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

#define CHECK_ERROR_LOG(cmlib_macroarg_expr, ...)                              \
    do                                                                         \
    {                                                                          \
        if ((err = (cmlib_macroarg_expr)))                                     \
        {                                                                      \
            log_error(__VA_ARGS__);                                            \
            ERROR_LEAVE();                                                     \
        }                                                                      \
    } while (0)

#define HANDLE_ERRNO_ERROR(cmlib_macroarg_error, ...)                          \
    do                                                                         \
    {                                                                          \
        UNUSED int ern = errno;                                                \
        err = cmlib_macroarg_error;                                            \
        log_error(__VA_ARGS__, strerror(ern));                                 \
        ERROR_LEAVE();                                                         \
    } while (0)

#endif // CMLIB_LOGGER_H_
