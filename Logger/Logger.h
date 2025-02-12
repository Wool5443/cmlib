#ifndef CMLIB_LOGGER_H_
#define CMLIB_LOGGER_H_

#include <errno.h> // IWYU pragma: export
#include <stdio.h> // IWYU pragma: export
#include <stdarg.h> // IWYU pragma: export

#include "include/ConsoleColor.h" // IWYU pragma: export
#include "Error.h" // IWYU pragma: export
#include "Result.h" // IWYU pragma: export

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

extern Logger cmlibLogger_;

void LoggerInitPath(const char* path);

void LoggerInitFile(FILE* file);

void LoggerInitConsole();

INLINE Logger* GetLogger();

#define LOG_(type, error, ...)                                          \
do                                                                      \
{                                                                       \
    FILE* logFile = GetLogger()->file;                                  \
    if (!logFile) break;                                                \
                                                                        \
    SetConsoleColor(logFile, getTypeColor_(type));                      \
                                                                        \
    fprintf(logFile, "[%s] ", getTypeString_(type));                    \
                                                                        \
    ErrorPrint(error, logFile);                                         \
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

#define LogInfo(...) LOG_(LOG_TYPE_INFO, CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(,) __VA_ARGS__)
#define LogDebug(...) LOG_(LOG_TYPE_DEBUG, CREATE_ERROR(EVERYTHING_FINE) __VA_OPT__(,) __VA_ARGS__)
#define LogError(...) LOG_(LOG_TYPE_ERROR, CREATE_ERROR(err) __VA_OPT__(,) __VA_ARGS__)

#else

#define LoggerInitPath(...)
#define LoggerInitFile(...)
#define LoggerInitConsole(...)
#define LoggerFinish(...)

#define LogInfo(...)
#define LogDebug(...)
#define LogError(...)

#endif // #ifndef DISABLE_LOGGING

#define CHECK_ERROR_LOG(expr, ...)                                      \
do                                                                      \
{                                                                       \
    if ((err = (expr)))                                                 \
    {                                                                   \
        LogError(__VA_ARGS__);                                          \
        ERROR_LEAVE();                                                  \
    }                                                                   \
} while (0)

#define HANDLE_ERRNO_ERROR(error, ...)                                  \
do                                                                      \
{                                                                       \
    UNUSED int ern = errno;                                             \
    err = error;                                                        \
    LogError(__VA_ARGS__, strerror(ern));                               \
    ERROR_LEAVE();                                                      \
} while (0)

INLINE Logger* GetLogger()
{
    return &cmlibLogger_;
}

INLINE const char* getTypeString_(LogType type)
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

INLINE ConsoleColor getTypeColor_(LogType type)
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
