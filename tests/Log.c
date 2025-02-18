#include "Logger.h"

int main(void)
{
    logger_init_console();

    char s[] = "hello world, %d\n";
    log_info("%s\n", "hi");

    printf("hello world, %d\n", 10);
}
