#include "Logger.h"

#include <stdio.h>

#include "Error.h"

int main(void)
{
    ERROR_CHECKING();

    logger_init_console();

    log_info("application started");
    log_debug("value = %d", 42);

    err = ERROR_NO_MEMORY;
    log_error("huh?");
    err = EVERYTHING_FINE;

    FILE* f = fopen("NONODANODAODNADADADAIDAIDADAda", "r");

    if (!f)
    {
        HANDLE_ERRNO_ERROR(ERROR_BAD_FILE,
            "Could not open such a strange file: %s");
    }

    ERROR_CASE

    if (f)
    {
        fclose(f);
    }

    return err;
}
