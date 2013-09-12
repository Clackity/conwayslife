#pragma once

#ifndef MIN
#define MIN(a, b) (a <= b ? a : b)
#endif

#ifndef MAX
#define MAX(a, b) (a >= b ? a : b)
#endif

#ifndef CLAMP
#define CLAMP(min, val, max) (MAX(min, MIN(max, val)))
#endif
