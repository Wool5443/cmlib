#include "String.h"

int main()
{
    String s = string_ctor("hi\n").value;

    String s2 = string_printf("Hello %d worlds!\n", 10).value;

    str_print(str_ctor_string(s), stdout);
    str_print(str_ctor_string(s2), stdout);
}
