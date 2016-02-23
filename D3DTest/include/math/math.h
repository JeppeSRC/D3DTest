#pragma once

#define TM_PI 3.1415926535897932384626433832795
#define TM_1_PI 0.318309886183790671538
#define TM_PRECALC_TO_RADIANS 0.01745329251994329576923690768489
#define TM_PRECALC_TO_DEGREES 57.295779513082320876798154814105

#define TO_RADIANS(x) (x * TM_PRECALC_TO_RADIANS)
#define TO_DEGREES(x) (x * TM_PRECALC_TO_DEGREES)
#define TO_RADIANS_F(x) float(x * TM_PRECALC_TO_RADIANS)
#define TO_DEGREES_F(x) float(x * TM_PRECALC_TO_DEGREES)
#define SIN(x) sinf(x)
#define COS(x) cosf(x)

#define MAGIC_NUMBER_64BIT 0x5fe6eb50c7b537a9
#define MAGIC_NUMBER_32BIT 0x5f3759df

#define _USE_MATH_DEFINES
#include <math.h>

#include "mat4.h"
#include "mat3.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"


float invSqrtf(float number);
//float invSqrtf2(float number);
double invSqrt(double number);
void matrix_mul4(float* a, float* b);
void matrix_mul3(float* a, float* b);
//double invSqrt2(double number);