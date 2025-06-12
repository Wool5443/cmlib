#include "../Logger.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void logger_init_file(FILE* file)
{
    assert(file);

    setbuf(file, NULL);

    cmlibLogger_.file = file;
}

void logger_init_path(const char* path)
{
    FILE* file = fopen(path, "w");

    if (!file)
    {
        fprintf(stderr, "Logger could not initialize\n%s was a bad file", path);
    }

    logger_init_file(file);

    if (atexit(loggerFinish_) != 0)
    {
        fprintf(stderr, "Failed to atexit(loggerFinish_): %s", strerror(errno));
    }
}

void logger_init_console()
{
    logger_init_file(stderr);
}

static void loggerFinish_()
{
    if (!cmlibLogger_.file)
        return;

    if (isatty(fileno(cmlibLogger_.file)))
        return;

    fclose(cmlibLogger_.file);
}

Logger* get_logger();
const char* get_log_type_string_(Log_type type);
ConsoleColor get_log_type_color_(Log_type type);
