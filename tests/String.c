#include "String.h"

int main()
{
    ERROR_CHECKING();

    logger_init_console();

    String s = string_ctor("hi\n").value;

    String s2 = string_printf("Hello %d worlds!\n", 10).value;

    str_print(str_ctor_string(s), stdout);
    str_print(str_ctor_string(s2), stdout);

ERROR_CASE
    string_dtor(&s);
    string_dtor(&s2);
}
