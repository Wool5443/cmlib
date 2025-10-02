#include "String.h"

int main()
{
    logger_init_console();

    String s = string_ctor("hi\n").value;

    String s2 = {};
    string_printf(&s2, "Hello %d worlds!", 10);
    string_printf(&s2, " Hmmm, what if I add more text? %g", 3.1241);
    string_printf(&s2, " And even more? %g\n", 3.1241);
    printf("%s", s2.data);

    String s3 = string_ctor_str(STR_LITERAL("NODE_NAME")).value;
    string_printf(&s3, "(%s)", "printf");
    printf("%s\n", s3.data);

    // str_print(str_ctor_string(s), stdout);
    // str_print(str_ctor_string(s2), stdout);

    string_dtor(&s);
    s = string_ctor_printf("Hello, I want to replace HAHAHAHAHA with HAHA "
                           "here\n").value;
    str_print(str_ctor_string(s), stdout);
    string_replace_all(&s, STR_LITERAL("HAHAHAHAHA"), STR_LITERAL("HAHA"));
    str_print(str_ctor_string(s), stdout);

    string_dtor(&s2);
    s2 = string_ctor_printf("Hello, I want to replace BE with HAHAHHAHAHAH "
                            "here\n")
             .value;
    str_print(str_ctor_string(s2), stdout);
    string_replace_all(&s2, STR_LITERAL("BE"), STR_LITERAL("HAHAHHAHAHAH"));
    str_print(str_ctor_string(s2), stdout);

    string_dtor(&s);
    string_dtor(&s2);
}
