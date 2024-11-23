#ifndef CMLIB_LOGGER_H
#define CMLIB_LOGGER_H

#include <unistd.h>

#include "implementation/ErrorStruct.h" // IWYU pragma: keep
#include "implementation/LoggerStruct.h" // IWYU pragma: keep
#include "implementation/LoggerMacros.h" // IWYU pragma: keep

/**
 * @brief Set the console color
 *
 * @param [in] file stdout or stderr
 * @param [in] color
 */
INLINE void SetConsoleColor(FILE* file, ConsoleColor color)
{
    assert(file);
    if (isatty(fileno(file)))
        fprintf(file, "\033[0;%dm", (int)color);
}

#endif // CMLIB_LOGGER_H
