#include "String.h"

int main()
{
    logger_init_console();

    String s = string_ctor("hi\n").value;

    String s2 = {};
    string_printf(&s2, "Hello %d worlds!\n", 10);

    str_print(str_ctor_string(s), stdout);
    str_print(str_ctor_string(s2), stdout);

    string_dtor(&s);
    string_dtor(&s2);
}
