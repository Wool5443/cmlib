#include <assert.h>
#include <unistd.h>

#include "../Logger.h"

Logger LOGGER_ = {};

void LoggerInit(const char path[static 1])
{
    FILE* file = fopen(path, "w");

    if (!file)
        fprintf(stderr, "Logger could not initialize\n%s was a bad file",
                path);

    LoggerInitFile(file);
}

void LoggerInitFile(FILE file[static 1])
{
    assert(file);

    setbuf(file, NULL);

    LOGGER_.file = file;
}

void LoggerInitConsole()
{
    LoggerInitFile(stderr);
}

void LoggerFinish()
{
    if (!LOGGER_.file) return;

    if (isatty(fileno(LOGGER_.file))) return;

    fclose(LOGGER_.file);
}
