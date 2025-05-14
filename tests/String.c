#include "String.h"

int main()
{
    logger_init_console();

    String s = string_ctor("hi\n").value;

    String s2 = {};
    string_printf(&s2, "Hello %d worlds!\n", 10);

    // str_print(str_ctor_string(s), stdout);
    // str_print(str_ctor_string(s2), stdout);

    string_dtor(&s);
    s = string_ctor_printf("Hello, I want to replace HAHAHAHAHA with HAHA here\n").value;
    str_print(str_ctor_string(s), stdout);
    string_replace_all(&s, STR_LITERAL("HAHAHAHAHA"), STR_LITERAL("HAHA"));
    str_print(str_ctor_string(s), stdout);

    string_dtor(&s2);
    s2 = string_ctor_printf("Hello, I want to replace BE with HAHAHHAHAHAH here\n").value;
    str_print(str_ctor_string(s2), stdout);
    string_replace_all(&s2, STR_LITERAL("BE"), STR_LITERAL("HAHAHHAHAHAH"));
    str_print(str_ctor_string(s2), stdout);

    string_dtor(&s);
    string_dtor(&s2);
}
