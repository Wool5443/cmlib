#ifndef CMLIB_LOGGER_H_
#define CMLIB_LOGGER_H_

#include <errno.h> // IWYU pragma: export
#include <stdio.h> // IWYU pragma: export
#include <stdarg.h> // IWYU pragma: export

#include "include/ConsoleColor.h" // IWYU pragma: export
#include "Error.h" // IWYU pragma: export

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

extern Logger LOGGER_;

void LoggerInitPath(const char* path);

void LoggerInitFile(FILE* file);

void LoggerInitConsole();

void LoggerFinish();

INLINE Logger* GetLogger()
{
    return &LOGGER_;
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

INLINE void log_(LogType type, Error error, const char* format, ...)
{
    FILE* logFile = GetLogger()->file;
    if (!logFile) return;

    SetConsoleColor(logFile, getTypeColor_(type));

    fprintf(logFile, "[%s] ", getTypeString_(type));

    ErrorPrint(error, logFile);

    if (format)
    {
        va_list args;
        va_start(args, format);

        fprintf(logFile, "\n");
        vfprintf(logFile, format, args);
    }

    fprintf(logFile, "\n\n");
    SetConsoleColor(logFile, CONSOLE_COLOR_WHITE);
}

#define LOG_(type, error, ...)                                          \
log_(                                                                   \
type,                                                                   \
error,                                                                  \
SWITCH_EMPTY(NULL, FIRST(__VA_ARGS__), __VA_ARGS__)                     \
VA_OPT_BUT_FIRST(__VA_ARGS__)                                           \
EXPAND_BUT_FIRST(__VA_ARGS__))

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

#define DECLARE_RESULT_HEADER(Type)                                     \
typedef struct Result ## Type                                           \
{                                                                       \
    Type value;                                                         \
    ErrorCode errorCode;                                                \
} Result ## Type;                                                       \
INLINE Result ## Type Result ## Type ## Ctor                            \
(Type value, ErrorCode errorCode)                                       \
{                                                                       \
    return (Result ## Type){ .value = value, .errorCode = errorCode };  \
}

#define DECLARE_RESULT_SOURCE(Type)                                     \
Result ## Type Result ## Type ## Ctor(Type value, ErrorCode errorCode);


#define ERROR_CHECKING()                                                \
    UNUSED ErrorCode err = EVERYTHING_FINE

#define ERROR_CASE ERROR_CASE_:;

#define ERROR_LEAVE() goto ERROR_CASE_

#define GET_FILE_NAME() __FILE__
#define GET_LINE()      __LINE__

#if defined(__GNUC__) || defined(__clang__)
    #define GET_FUNCTION()  __PRETTY_FUNCTION__
#else
    #define GET_FUNCTION()  __func__
#endif

#define CREATE_ERROR(errorCode) \
ErrorCtor(errorCode, GET_FILE_NAME(), GET_LINE(), GET_FUNCTION())

#define CHECK_ERROR(expr, ...)                                          \
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

#endif // CMLIB_LOGGER_H_
