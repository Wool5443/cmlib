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
} LogType;

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

INLINE const char* get_log_type_string_(LogType type);

INLINE Console_color get_log_type_color_(LogType type);

INLINE Logger* get_logger()
{
    return &cmlib_logger_;
}

INLINE const char* get_log_type_string_(LogType type)
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

INLINE Console_color get_log_type_color_(LogType type)
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

#define LOG_(type, error, ...)                                                 \
    do                                                                         \
    {                                                                          \
        LogType cmlib_log_type__ = (type);                                     \
        Error cmlib_log_error__ = (error);                                     \
        FILE* cmlib_log_log_file__ = get_logger()->file;                       \
        if (!cmlib_log_log_file__)                                             \
        {                                                                      \
            break;                                                             \
        }                                                                      \
        set_console_color(cmlib_log_log_file__,                                \
            get_log_type_color_(cmlib_log_type__));                            \
        fprintf(cmlib_log_log_file__,                                          \
            "[%s] ",                                                           \
            get_log_type_string_(cmlib_log_type__));                           \
        error_print(cmlib_log_error__, cmlib_log_log_file__);                  \
        SWITCH_EMPTY(,                                                         \
            (fprintf(cmlib_log_log_file__, "\n"),                              \
                fprintf(cmlib_log_log_file__,                                  \
                    "" FIRST(__VA_ARGS__) "" VA_OPT_BUT_FIRST(__VA_ARGS__)     \
                        EXPAND_BUT_FIRST(__VA_ARGS__))),                       \
            __VA_ARGS__);                                                      \
        fprintf(cmlib_log_log_file__, "\n\n");                                 \
        set_console_color(cmlib_log_log_file__, CONSOLE_COLOR_WHITE);          \
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

#define CHECK_ERROR_LOG(expression, ...)                                       \
    do                                                                         \
    {                                                                          \
        if ((err = (expression)))                                              \
        {                                                                      \
            log_error(__VA_ARGS__);                                            \
            ERROR_LEAVE();                                                     \
        }                                                                      \
    } while (0)

#define HANDLE_ERRNO_ERROR(error, ...)                                         \
    do                                                                         \
    {                                                                          \
        int cmlib_handle_errno_error_ern__ = errno;                            \
        err = error;                                                           \
        log_error(__VA_ARGS__, strerror(cmlib_handle_errno_error_ern__));      \
        ERROR_LEAVE();                                                         \
    } while (0)

#endif // CMLIB_LOGGER_H_
