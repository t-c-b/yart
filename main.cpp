#include "img.h"
#include "vec.h"

#include "ray.h"
#include "hitables.h"
#include "camera.h"
#include "rand.h"
#include "material.h"

/*
float hitSphere(const vec4 &center, float radius, const Ray& r) {
  vec4 oc = r.origin-center;
  float a = dot3(r.direction, r.direction);
  //float a = r.direction.dot3(r.direction);//passing this discards const
  float b = 2.0 * dot3(oc, r.direction);
  float c = dot3(oc, oc) - radius*radius;
  float discriminant = b*b - 4*a*c;
  if (discriminant < 0)
    return -1.0;
  else
    return (-b - sqrt(discriminant)) / (2.f*a);
}

vec4 color(Ray r) {
  float t = hitSphere(vec4(0,0,-1,0), 0.5, r);
  if( t > 0.0) {
    vec4 normal = norm3(r.pointAtParam(t) - vec4(0,0,-1,0));
    return 0.5*vec4(normal.x()+1, normal.y()+1, normal.z()+1, 0);
  }
  vec4 unit_dir = norm3(r.direction);
  t = 0.5*(unit_dir.y() + 1.f);
  return (1.f-t)*vec4(1.f,1.f,1.f,0) + t*vec4(0.5,0.7,1.0,0);
}
*/

//ref to
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

    //vec4 target = rec.point + rec.normal + randSphere();
    //return 0.5*color(Ray(rec.point, target-rec.point), world);
    //return 0.5*vec4(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1, 0);
  }
  else {
    vec4 unit_dir = norm3(r.direction);
    float t = 0.5*(unit_dir.y() + 1.f);
    return (1.f-t)*vec4(1.f,1.f,1.f,0) + t*vec4(0.5,0.7,1.0,0);
  }
  
}

int main() {
  Image img(200, 100);

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
    vec4(1,0,-1,0), 0.5,
    std::make_unique<Metallic>(vec4(0.8,0.8,0.8,0), 0.1)
  ));
  world.addObject(std::make_unique<Sphere>(
    vec4(-1,0,-1,0), 0.5,
    std::make_unique<Dielectric>(1.5)
  ));
  //world.addObject(std::make_unique<Sphere>(
    //vec4(-1,0,-1,0), -0.45,
    //std::make_unique<Dielectric>(1.5)
  //));
  /*
  float R = cos(M_PI/4);
  world.addObject(std::make_unique<Sphere>(
    vec4(-R,0,-1,0), R,
    std::make_unique<Lambertian>(vec4(0,0,1,0))
  ));
  world.addObject(std::make_unique<Sphere>(
    vec4(R,0,-1,0), R,
    std::make_unique<Lambertian>(vec4(1,0,0,0))
  ));
  */

  auto lookfrom = vec4(-3,3,2,0);
  auto lookat = vec4(0,0,-1,0);
  Camera cam(
    lookfrom,
    lookat,
    vec4(0,1,0,0),
    20, 200.f/100.f,
    2.f, (lookfrom-lookat).length()
  );
  const int ns = 64;
  
  for (int i=0; i<100; ++i) {
    for (int j=0; j<200; ++j) {
      vec4 c(0,0,0,0);
      for (int s=0; s<ns; ++s) {
        float u = (j+randUniform()) / 200.f;
        float v = (i+randUniform()) / 100.f;
        Ray r = cam.getRay(u,v);
        /*
        vec4 v(j/200.f, i/100.f, 0, 0);
        Ray r(origin, top_left + v*dimensions);
        img.setPixel(j,i, color(r, &world).d);
        */
        c = c + color(r, &world, 0);
      }
      c = c/static_cast<float>(ns);
      img.setPixel(j,i,c.d);
    }
  }
  img.writeTextPPM("test.ppm");
  return 0;
}
