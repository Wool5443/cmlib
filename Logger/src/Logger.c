#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../Logger.h"

#undef LoggerInitPath
#undef LoggerInitFile
#undef LoggerInitConsole
#undef LoggerFinish

#undef LogInfo
#undef LogDebug
#undef LogError

#undef HANDLE_ERRNO_ERROR

static void loggerFinish_();

Logger cmlibLogger_ = {};

void LoggerInitFile(FILE* file)
{
    assert(file);

    setbuf(file, NULL);

    cmlibLogger_.file = file;
}

void LoggerInitPath(const char* path)
{
    FILE* file = fopen(path, "w");

    if (!file)
    {
        fprintf(stderr, "Logger could not initialize\n%s was a bad file", path);
    }

    LoggerInitFile(file);

    if (atexit(loggerFinish_) != 0)
    {
        fprintf(stderr, "Failed to atexit(loggerFinish_): %s", strerror(errno));
    }
}

void LoggerInitConsole()
{
    LoggerInitFile(stderr);
}

static void loggerFinish_()
{
    if (!cmlibLogger_.file) return;

    if (isatty(fileno(cmlibLogger_.file))) return;

    fclose(cmlibLogger_.file);
}

Logger* GetLogger();
const char* getTypeString_(LogType type);
ConsoleColor getTypeColor_(LogType type);
