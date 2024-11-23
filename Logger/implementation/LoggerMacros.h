#ifndef CMLIB_ERROR_MACROS_H
#define CMLIB_ERROR_MACROS_H

#define DECLARE_RESULT(Type)                                            \
typedef struct                                                          \
{                                                                       \
    Type value;                                                         \
    ErrorCode errorCode;                                                \
} Result ## Type;                                                       \
INLINE Result ## Type Result ## Type ## Ctor                            \
(Type value, ErrorCode errorCode)                                       \
{                                                                       \
    return (Result ## Type){ .value = value, .errorCode = errorCode };  \
}

#if defined(__GNUC__) || defined(__clang__)
#define ERROR_CHECKING()                                            \
    ErrorCode err __attribute__((unused)) = EVERYTHING_FINE
#else
    ErrorCode err = EVERYTHING_FINE
#endif

#define ERROR_CASE _ERROR_CASE_:;

#define ERROR_LEAVE() goto _ERROR_CASE_

#define GET_FILE_NAME() __FILE__
#define GET_LINE()      __LINE__

#if defined(__GNUC__) || defined(__clang__)
    #define GET_FUNCTION()  __PRETTY_FUNCTION__
#else
    #define GET_FUNCTION()  __func__
#endif

#define CREATE_ERROR(errorCode) \
ErrorCtor(errorCode, GET_FILE_NAME(), GET_LINE(), GET_FUNCTION())

#define _LOG_DONT_USE_(type, color, errorCode, ...)                 \
do                                                                  \
{                                                                   \
    FILE* _logFile_ = GetLogger()->file;                            \
    if (!_logFile_) break;                                          \
    SetConsoleColor(_logFile_, (color));                            \
    fprintf(_logFile_, "[" type "] ");                              \
    ErrorPrint(CREATE_ERROR(errorCode), _logFile_);                 \
    __VA_OPT__(fprintf(_logFile_, "\n");)                           \
    __VA_OPT__(fprintf(_logFile_, __VA_ARGS__);)                    \
    fprintf(_logFile_, "\n\n");                                     \
    SetConsoleColor(_logFile_, CONSOLE_COLOR_WHITE);                \
} while (0)

#define LOG_INFO(...) \
_LOG_DONT_USE_("INFO", CONSOLE_COLOR_CYAN, EVERYTHING_FINE, __VA_ARGS__)

#define LOG_DEBUG(...) \
_LOG_DONT_USE_("DEBUG", CONSOLE_COLOR_YELLOW, EVERYTHING_FINE, __VA_ARGS__)

#define LOG_ERROR(...) \
_LOG_DONT_USE_("ERROR", CONSOLE_COLOR_RED, err, __VA_ARGS__)

#define LOG_ERROR_IF(...)                           \
do                                                  \
{                                                   \
    if (err)                                        \
        LOG_ERROR(__VA_ARGS__);                     \
} while (0)

#define CHECK_ERROR(expr)                           \
do                                                  \
{                                                   \
    if ((err = (expr)))                             \
    {                                               \
        LOG_ERROR();                                \
        ERROR_LEAVE();                              \
    }                                               \
} while (0)

#define RETURN(retval)                              \
do                                                  \
{                                                   \
    LOG_ERROR_IF();                                 \
    return retval;                                  \
} while (0)

#define RETURN_ERROR_IF()                           \
do                                                  \
{                                                   \
    if (err)                                        \
        RETURN(err);                                \
} while (0)

#endif // CMLIB_ERROR_MACROS_H
