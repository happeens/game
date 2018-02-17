#pragma once

// TODO: only include this in debug builds
#include <stdio.h>
#define ASSERT(condition) \
{\
    if (!(condition)) {\
        printf(\
            "assertion failed at %s:%s in %s",\
            __FILE__,\
            __LINE__,\
            __func__\
        );\
        abort();\
    }\
}

