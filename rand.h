#pragma once
//Single header random

#include <random>
#include "vec.h"

float randUniform(float min = 0.f, float max = 1.f){
  static std::random_device rd;
  static std::default_random_engine gen(rd());
  std::uniform_real_distribution<float> rand(min,max);
  return rand(gen);
}

vec4 randSphere(){
  vec4 v;
  do {
    v = vec4(randUniform(-1,1), randUniform(-1,1), randUniform(-1,1), 0);
  } while(dot3(v,v) > 1);
  return v;
}

vec4 randDisk(){
  vec4 v;
  do {
    v = vec4(randUniform(-1,1), randUniform(-1,1), 0, 0);
  } while(dot3(v,v) > 1);
  return v;
}
