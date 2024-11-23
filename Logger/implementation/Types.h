#ifndef CMLIB_LOGGER_TYPES_H
#define CMLIB_LOGGER_TYPES_H

#include <time.h>
#include <stdio.h>

typedef enum
{

#define DEF_ERROR(code) \
code,

#include "ErrorGen.h"

#undef DEF_ERROR

} ErrorCode;

typedef enum
{
    CONSOLE_COLOR_BLACK = 30,
    CONSOLE_COLOR_RED,
    CONSOLE_COLOR_GREEN,
    CONSOLE_COLOR_YELLOW,
    CONSOLE_COLOR_BLUE,
    CONSOLE_COLOR_MAGENTA,
    CONSOLE_COLOR_CYAN,
    CONSOLE_COLOR_WHITE,
} ConsoleColor;

typedef struct
{
    ErrorCode   code;
    const char* file;
    size_t      line;
    const char* function;
    time_t      time;
} Error;

typedef struct
{
    FILE* file;
} Logger;

#endif // CMLIB_LOGGER_TYPES_H
