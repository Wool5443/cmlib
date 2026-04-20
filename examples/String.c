#include "String.h"

#include <stdio.h>

int main(void)
{
    Result_String res = string_ctor(get_calloc_resource(), "hello");
    if (res.error_code != EVERYTHING_FINE)
    {
        return 1;
    }

    String text = res.value;
    string_append(&text, ", cmlib ");
    string_append(&text, "and cmlib");
    string_append_char(&text, '!');

    printf("%s\n", text.data);

    string_replace_all(&text, STR_LITERAL("cmlib"), STR_LITERAL("world"));
    printf("%s\n", text.data);

    string_dtor(&text);
    return 0;
}
