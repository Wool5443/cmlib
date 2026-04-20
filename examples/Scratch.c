#include <stdio.h>

#include "ScratchBuffer.h"

int main(void)
{
    if (scratch_ctor(get_calloc_resource(), 32) != EVERYTHING_FINE)
    {
        return 1;
    }

    scratch_append("temporary");
    scratch_append_char(' ');
    scratch_append("text");

    puts(scratch_get());

    scratch_clear();
    scratch_append("reused");
    puts(scratch_get());

    scratch_dtor();
    return 0;
}
