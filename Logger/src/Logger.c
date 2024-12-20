#include <assert.h>
#include <unistd.h>

#include "../Logger.h" // IWYU pragma: keep

#undef LoggerInitPath
#undef LoggerInitFile
#undef LoggerInitConsole
#undef LoggerFinish

#undef LogInfo
#undef LogDebug
#undef LogError

#undef HANDLE_ERRNO_ERROR

Logger LOGGER_ = {};

void LoggerInitFile(FILE file[static 1])
{
    assert(file);

    setbuf(file, NULL);

    LOGGER_.file = file;
}

void LoggerInitPath(const char path[static 1])
{
    FILE* file = fopen(path, "w");

    if (!file)
        fprintf(stderr, "Logger could not initialize\n%s was a bad file",
                path);

    LoggerInitFile(file);
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
