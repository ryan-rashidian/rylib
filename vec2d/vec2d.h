/* 
 * vec2d - Simple Vector 2D library
 *
 * This library was written as an exercise and review of vector math. The
 * interface is inspired from using the Raylib raymath.h library.
 */

#ifndef VEC_H
#define VEC_H

typedef struct {
    float x, y;
} Vec2;

Vec2  vec2_subtract(Vec2 v1, Vec2 v2);
Vec2  vec2_add(Vec2 v1, Vec2 v2);
Vec2  vec2_multiply(Vec2 v1, Vec2 v2);
Vec2  vec2_divide(Vec2 v1, Vec2 v2);
float vec2_dot_product(Vec2 v1, Vec2 v2);
float vec2_cross_product(Vec2 v1, Vec2 v2);
float vec2_length(Vec2 v);
float vec2_length_sqr(Vec2 v);
float vec2_distance(Vec2 v1, Vec2 v2);
float vec2_distance_sqr(Vec2 v1, Vec2 v2);
Vec2  vec2_normalize(Vec2 v);
Vec2  vec2_scale(Vec2 v, float scale);
Vec2  vec2_min(Vec2 v1, Vec2 v2);
Vec2  vec2_max(Vec2 v1, Vec2 v2);

#endif

