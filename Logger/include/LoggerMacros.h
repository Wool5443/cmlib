#ifndef CMLIB_ERROR_MACROS_H
#define CMLIB_ERROR_MACROS_H

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

#endif // CMLIB_ERROR_MACROS_H
