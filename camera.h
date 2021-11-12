#pragma once

#include "ray.h"
#include "rand.h"

class Camera {
public:
  Camera(vec4 lookfrom, vec4 lookat, vec4 up, float vfov, float aspect,
    float aperture, float focus_dist
  ) {
    lens_radius = aperture/2.f;

    float theta = vfov*M_PI/180;
    float half_height = tan(theta/2);
    float half_width = aspect * half_height;

    origin = lookfrom;
    w = norm3(lookfrom - lookat);
    u = norm3(cross3(up,w));
    v = cross3(w,u);
    //upper_left = vec4(-half_width, half_height, -1.0,0);
    upper_left = origin - half_width*focus_dist*u
      + half_height*focus_dist*v - focus_dist*w;
    horizontal = 2*half_width*focus_dist*u;
    vertical = -2*half_height*focus_dist*v;
  }

  Ray getRay(float s, float t) {
    vec4 rd = lens_radius*randDisk();
    vec4 offset = u*rd.x() + v*rd.y();
    return Ray(origin + offset, upper_left + s*horizontal + t*vertical - origin - offset);
  }

  vec4 origin, upper_left, horizontal, vertical;
  vec4 u, v, w;
  float lens_radius;
};
