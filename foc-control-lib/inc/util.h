#ifndef UTIL_H
#define UTIL_H
#include <math.h>

#define M_PI_FLOAT 3.14159265F

#define DEG_TO_RAD(x) ((x) * M_PI_FLOAT / 180.0F)
#define RAD_TO_DEG(x) ((x) * 180.0F / M_PI_FLOAT)

#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define ABS_MACRO(x) ((x) < 0 ? -(x) : (x))

#define SQRT_3 (1.73205080757F)
#define ONE_OVER_SQRT_3 (0.57735026919F)

#endif  // UTIL_H