#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <math.h>
#include "types.h"

static inline Vec2 vec2_add(Vec2 a, Vec2 b){
	return (Vec2){a.x + b.x, a.y + b.y};
}

static inline Vec2 vec2_sub(Vec2 a, Vec2 b) {
	return (Vec2){a.x - b.x, a.y - b.y};
}

static inline Vec2 vec2_scale(Vec2 v, float s){
	return (Vec2){v.x * s, v.y * s};
}

static inline float vec2_dot(Vec2 a, Vec2 b){
		return a.x * b.x + a.y * b.y;
}

static inline float vec2_lenSq(Vec2 v){
	return v.x * v.x + v.y * v.y;
}

static inline float vec2_len(Vec2 v){
       return sqrtf(vec2_lenSq(v));
}

static inline float vec2disSq(Vec2 a, Vec2 b){
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return dx*dx + dy*dy;
}

#endif
