#pragma once
#include "vec.h"

class Ray
{
public:
  Ray() {}
  Ray(const vec4& src, const vec4& dir) : origin(src), direction(dir){}
  vec4 pointAtParam(float t) const {return origin + t*direction;}
  vec4 origin;
  vec4 direction;
};
