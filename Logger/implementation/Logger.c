#include <assert.h>

#include "LoggerStruct.h"

Logger LOGGER_ = {};

void LoggerInit(const char path[static 1])
{
    FILE* file = fopen(path, "w");

    if (!file)
        fprintf(stderr, "Logger could not initialize\n%s was a bad file",
                path);
}

void LoggerInitFile(FILE file[static 1])
{
    assert(file);

    setbuf(file, NULL);

    LOGGER_.file = file;
}

void LoggerInitConsole()
{
    LOGGER_.file = stderr;
}

void LoggerFinish()
{
    if (!LOGGER_.file) return;

    fclose(LOGGER_.file);
}

Logger* GetLogger()
{
    return &LOGGER_;
}
