#ifndef CMLIB_LOGGER_CONSOLE_COLOR_H_
#define CMLIB_LOGGER_CONSOLE_COLOR_H_

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "../../common.h"

/** @brief ANSI base colors supported by logger output. */
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
} Console_color;

/**
 * @brief Emits ANSI color sequence to a terminal stream.
 *
 * @param file Target stream, must be non-NULL.
 * @param color Color code to emit.
 *
 * Writes to @p file only when `isatty(fileno(file))` is true. For non-terminal
 * streams this function is a no-op.
 */
INLINE void set_console_color(FILE* file, Console_color color)
{
    assert(file);
    if (isatty(fileno(file)))
    {
        fprintf(file, "\033[0;%dm", (int)color);
    }
}

#endif // CMLIB_LOGGER_CONSOLE_COLOR_H_
