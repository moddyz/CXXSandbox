#pragma once

#include <stdint.h>
#include <stdio.h>

#define _ASSERT(file, line, expr)                                              \
    if (!(expr)) {                                                             \
        fprintf(stderr,                                                        \
                "Assertion '%s' failed at %s:%d! Aborting.\n",                 \
                #expr,                                                         \
                file,                                                          \
                line);                                                         \
        exit(EXIT_FAILURE);                                                    \
    }

/// \def ASSERT
///
/// Exits the program with EXIT_FAILURE if the expression evaluates to \p false.
#define ASSERT(expr) _ASSERT(__FILE__, __LINE__, expr);
