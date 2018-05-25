#ifndef ALPINI_H
#define ALPINI_H

#ifdef __cplusplus
// C++ code
using namespace std;

namespace helpers {
    long long get_elapsed_time(void);
    bool has_reached_timeout(void);

    void setup(void);
}

#else
// C code
#include <stdbool.h>

long long get_elapsed_time(void);
bool has_reached_timeout(void);

void setup(void);

#error "Only C++ is supported"
#endif

#endif
#pragma once
