#pragma once

// TODO: only include this in debug builds
#include <stdio.h>
#define ASSERT(condition) \
{\
    if (!(condition)) {\
        printf("assertion failed at %s:%d\n", __FILE__, __LINE__);\
        abort();\
    }\
}

