#ifndef CMLIB_LOGGER_H_
#define CMLIB_LOGGER_H_

#include <errno.h> // IWYU pragma: export
#include <stdio.h> // IWYU pragma: export
#include <stdarg.h> // IWYU pragma: export
#include <string.h> // IWYU pragma: export

#include "include/ConsoleColor.h" // IWYU pragma: export
#include "Error.h" // IWYU pragma: export
#include "Result.h" // IWYU pragma: export

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

extern Logger cmlibLogger_;

void logger_init_path(const char* path);

void logger_init_file(FILE* file);

void logger_init_console();

INLINE Logger* get_logger();

#define LOG_(type, error, ...)                                          \
do                                                                      \
{                                                                       \
    FILE* logFile = get_logger()->file;                                 \
    if (!logFile) break;                                                \
                                                                        \
    SetConsoleColor(logFile, get_log_type_color_(type));                \
                                                                        \
    fprintf(logFile, "[%s] ", get_log_type_string_(type));              \
                                                                        \
    error_print(error, logFile);                                        \
                                                                        \
    SWITCH_EMPTY(,                                                      \
            (fprintf(logFile, "\n"),                                    \
             fprintf(logFile,                                           \
              "" FIRST(__VA_ARGS__) ""                                  \
                 VA_OPT_BUT_FIRST(__VA_ARGS__)                          \
                 EXPAND_BUT_FIRST(__VA_ARGS__))),                       \
            __VA_ARGS__);                                               \
                                                                        \
    fprintf(logFile, "\n\n");                                           \
    SetConsoleColor(logFile, CONSOLE_COLOR_WHITE);                      \
} while (0)

#define log_info(...) LOG_(LOG_TYPE_INFO, CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(,) __VA_ARGS__)
#define log_debug(...) LOG_(LOG_TYPE_DEBUG, CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(,) __VA_ARGS__)
#define log_error(...) LOG_(LOG_TYPE_ERROR, CREATE_ERROR(err) __VA_OPT__(,) __VA_ARGS__)

#else

#define logger_init_path(...)
#define logger_init_file(...)
#define logger_init_console(...)

#define log_info(...)
#define log_debug(...)
#define log_error(...)

#endif // #ifndef DISABLE_LOGGING

#define CHECK_ERROR_LOG(expr, ...)                                      \
do                                                                      \
{                                                                       \
    if ((err = (expr)))                                                 \
    {                                                                   \
        log_error(__VA_ARGS__);                                         \
        ERROR_LEAVE();                                                  \
    }                                                                   \
} while (0)

#define HANDLE_ERRNO_ERROR(error, ...)                                  \
do                                                                      \
{                                                                       \
    UNUSED int ern = errno;                                             \
    err = error;                                                        \
    log_error(__VA_ARGS__, strerror(ern));                              \
    ERROR_LEAVE();                                                      \
} while (0)

INLINE Logger* get_logger()
{
    return &cmlibLogger_;
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

INLINE ConsoleColor get_log_type_color_(Log_type type)
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

#endif // CMLIB_LOGGER_H_
