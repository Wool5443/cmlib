#ifndef CMLIB_LOGGER_CONSOLE_COLOR_H_
#define CMLIB_LOGGER_CONSOLE_COLOR_H_

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "../../common.h"

/** @enum Console_color
 * @brief enum of possible console colors
 */
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
 * @brief Set the console color
 *
 * @param [in] file stdout or stderr
 * @param [in] color
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
