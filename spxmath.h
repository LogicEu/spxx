/*

Copyright (c) 2023 Eugenio Arteaga A.

Permission is hereby granted, free of charge, to any 
person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the 
Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice 
shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef SIMPLE_PIXEL_MATH_H
#define SIMPLE_PIXEL_MATH_H

/******************
***** spxmath *****
 Simple PiXel Math
*******************
@Eugenio Arteaga A.
*******************

C99 header only math and linear algebra solution for
graphics, physics and game development.

****************************************************/

/* Simple Pixel Math */

typedef struct ivec2 { int x, y; } ivec2;
typedef struct ivec3 { int x, y, z; } ivec3;
typedef struct ivec4 { int x, y, z, w; } ivec4;
typedef struct vec2 { float x, y; } vec2;
typedef struct vec3 { float x, y, z; } vec3;
typedef struct vec4 { float x, y, z, w; } vec4;

float absf(float n);
float signf(float n);
float maxf(float n, float m);
float minf(float n, float m);
float clampf(float n, float min, float max);
float lerpf(float a, float b, float t);
float ilerpf(float min, float max, float n);
float smootlerpf(float a, float b, float t);
float remapf(float min, float max, float a, float b, float n);
float randf(void);

vec2 vec2_rand(void);
vec4 vec4_rand(void);
vec2 vec2_new(float x, float y);
vec2 vec2_add(vec2 p, vec2 q);
vec2 vec2_sub(vec2 p, vec2 q);
vec2 vec2_mult(vec2 p, float n);
vec2 vec2_div(vec2 p, float n);
vec2 vec2_norm(vec2 p);
vec2 vec2_cross(vec2 p, vec2 q);
vec2 vec2_lerp(vec2 a, vec2 b, float t);
vec2 rad2vec2(float rad);
float vec2_dot(vec2 p, vec2 q);
float vec2_sqmag(vec2 p);
float vec2_mag(vec2 p);
float vec2_to_rad(vec2 p);
float rad2deg(float rad);
float deg2rad(float deg);

vec2 ivec2_to_vec2(ivec2 p);
ivec2 vec2_to_ivec2(vec2 p);

#ifdef SPXM_APPLICATION

/******************
***** spxmath *****
 Simple PiXel Math
*******************
* IMPLEMENTATION  *
*******************/

#include <stdlib.h>
#include <math.h>

float absf(float n) { return n < 0.0F ? -n : n; }
float signf(float n) { return n < 0.0F ? -1.0F : 1.0F; }
float minf(float n, float m) { return n < m ? n : m; }
float maxf(float n, float m) { return n > m ? n : m; }
float clampf(float n, float min, float max) { return n > max ? max : n < min ? min : n; }
float lerpf(float a, float b, float t) { return a + t * (b - a); }
float smootlerpf(float a, float b, float t) { return a + (t * t * (3.0 - 2.0 * t)) * (b - a); }
float ilerpf(float min, float max, float n) { return (max - min) != 0.0F ? (n - min)
    / (max - min) : 0.0F; }
float remapf(float min, float max, float a, float b, float n) { return lerpf(a, b, ilerpf(min, max, n)); }
float randf(void) { return (float)rand() / (float)RAND_MAX; }

vec2 vec2_rand(void) { return (vec2) {randf(), randf()}; }
vec4 vec4_rand(void) { return (vec4) {randf(), randf(), randf(), 1.0F}; }
vec2 vec2_new(float x, float y)  { vec2 p = {x, y}; return p; }
vec2 vec2_add(vec2 p, vec2 q) { return (vec2) {p.x + q.x, p.y + q.y}; }
vec2 vec2_sub(vec2 p, vec2 q) { return (vec2) {p.x - q.x, p.y - q.y}; }
vec2 vec2_mult(vec2 p, float n) { return (vec2) {p.x * n, p.y * n}; }
vec2 vec2_div(vec2 p, float n) { n = n == 0.0F ? 0.0F : 1.0F / n; return (vec2) {p.x * n, p.y * n}; }
vec2 vec2_norm(vec2 p) { return vec2_div(p, sqrtf(p.x * p.x + p.y * p.y)); }
vec2 vec2_cross(vec2 p, vec2 q) { return (vec2) {-(p.y - q.y), p.x - q.x};}
vec2 vec2_lerp(vec2 a, vec2 b, float t) { return (vec2) {lerpf(a.x, b.x, t), lerpf(a.y, b.y, t)}; }
vec2 rad2vec2(float rad) { return (vec2){cosf(rad), sinf(rad)}; }
float vec2_dot(vec2 p, vec2 q) { return p.x * q.x + p.y * q.y; }
float vec2_sqmag(vec2 p) { return p.x * p.x + p.y * p.y; }
float vec2_mag(vec2 p) { return sqrtf(p.x * p.x + p.y * p.y); }
float vec2_to_rad(vec2 p) { return atan2f(p.y, p.x); }
float rad2deg(float rad) { return rad * (180.0F / M_PI); }
float deg2rad(float deg) { return deg / (180.0F / M_PI); }

vec2 ivec2_to_vec2(ivec2 p) { return (vec2) {(float)p.x, (float)p.y}; }
ivec2 vec2_to_ivec2(vec2 p) { return (ivec2) {(int)p.x, (int)p.y}; }

#endif /* SPXM_APPLICATION */
#endif /* SIMPLE_PIXEL_MATH_H */

