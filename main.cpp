#include "img.h"
#include "vec.h"

#include <sstream>

#include "ray.h"
#include "hitables.h"
#include "camera.h"
#include "rand.h"
#include "material.h"

vec4 color(const Ray &r, Hitable *world, int depth){
  HitRecord rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    Ray scattered;
    vec4 attenuation;
    if (depth < 50 && rec.mat->scatter(r,rec,attenuation,scattered)){
      return attenuation*color(scattered,world,depth+1);
    }
    else
      return vec4(0,0,0,0);

  }
  else {
    vec4 unit_dir = norm3(r.direction);
    float t = 0.5*(unit_dir.y() + 1.f);
    return (1.f-t)*vec4(1.f,1.f,1.f,0) + t*vec4(0.5,0.7,1.0,0);
  }
  
}

int main() {
  const int w = 420;
  const int h = 560;
  const int quilts = 8*6;

  HitableList world;
  world.addObject(std::make_unique<Sphere>(
    vec4(0,-100.5,-1,0), 100,
    std::make_unique<Lambertian>(vec4(0.8,0.8,0,0))
  ));
  world.addObject(std::make_unique<Sphere>(
    vec4(0,0,-1,0), 0.5,
    std::make_unique<Lambertian>(vec4(0.8,0.3,0.3,0))
  ));
  world.addObject(std::make_unique<Sphere>(
    vec4(1,0,-1.25,0), 0.5,
    std::make_unique<Metallic>(vec4(0.8,0.8,0.8,0), 0.1)
  ));
  world.addObject(std::make_unique<Sphere>(
    vec4(-1,0,-.75,0), 0.5,
    std::make_unique<Dielectric>(1.5)
  ));


    //auto lookfrom = vec4(-3,3,2,0);
    auto lookat = vec4(0,0,-1,0);// (0,0,-1,0)
    auto lookfrom = vec4(0,3,20,0);
    auto focal_length =(lookfrom-lookat).length(); 
    Camera cam(
      lookfrom,
      lookat,
      vec4(0,1,0,0),
      14,
      float(w)/float(h),
      0.25f, 
      focal_length
    );

  for (int q=0; q<quilts; ++q)
  {
    Image img(w, h);
    const int ns = 16;
    float shift = 0.315f * focal_length;
    auto off = (static_cast<float>(q)/(quilts-1) - 0.5) * 2 * vec4(shift,0,0,0);
    
    for (int i=0; i<h; ++i) {
      for (int j=0; j<w; ++j) {
        vec4 c(0,0,0,0);
        for (int s=0; s<ns; ++s) {
          float u = (j+randUniform()) / w;
          float v = (i+randUniform()) / h;
          Ray r = cam.getRay(u,v,off);
          c = c + color(r, &world, 0);
        }
        c = c/static_cast<float>(ns);
        img.setPixel(j,i,c.d);
      }
    }
    std::stringstream filename;
    filename << (q%8) + (5 - q/8)*8 << ".ppm";
    img.writeTextPPM(filename.str().c_str());
  }
  return 0;
}
