#include "../Logger.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void logger_finish_();

Logger cmlib_logger_ = {};

void logger_init_file(FILE* file)
{
    assert(file);

    setbuf(file, NULL);

    cmlib_logger_.file = file;
}

void logger_init_path(const char* path)
{
    FILE* file = fopen(path, "w");

    if (!file)
    {
        fprintf(stderr, "Logger could not initialize\n%s was a bad file", path);
    }

    logger_init_file(file);

    if (atexit(logger_finish_) != 0)
    {
        fprintf(stderr, "Failed to atexit(logger_finish_): %s", strerror(errno));
    }
}

void logger_init_console()
{
    logger_init_file(stderr);
}

static void logger_finish_()
{
    if (!cmlib_logger_.file)
    {
        return;
    }

    if (isatty(fileno(cmlib_logger_.file)))
    {
        return;
    }

    fclose(cmlib_logger_.file);
}

Logger* get_logger();
const char* get_log_type_string_(Log_type type);
Console_color get_log_type_color_(Log_type type);
