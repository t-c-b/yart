#pragma once

#include <vector>
#include <memory>

#include "ray.h"

class Material;

struct HitRecord {
  float t;
  vec4 point;
  vec4 normal;
  Material* mat;
};

class Hitable {
public:
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &rec) const=0;
};

class Sphere: public Hitable {
public:
  Sphere() {}
  Sphere(vec4 cen, float ro, std::unique_ptr<Material>m) : 
    center(cen), radius(ro), mat(std::move(m)) {}
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &rec) const;
  vec4 center;
  float radius;
  std::unique_ptr<Material> mat;
};

class HitableList: public Hitable {
public:
  HitableList() {}
  void addObject(std::unique_ptr<Hitable> &&obj) {
    objects.push_back(std::move(obj));
  }
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord &rec) const;
  std::vector<std::unique_ptr<Hitable>> objects;
};

bool Sphere::hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const{
  vec4 oc = ray.origin-center;
  float a = dot3(ray.direction, ray.direction);
  float b = 2.0 * dot3(oc, ray.direction);
  float c = dot3(oc, oc) - radius*radius;
  float discriminant = b*b - 4*a*c;

  if (discriminant < 0)
    return false;

  rec.mat = mat.get();

  float t = (-b - sqrt(discriminant)) / (2.f*a);
  if (t < t_max && t > t_min){
    rec.t = t;
    rec.point = ray.pointAtParam(t);
    rec.normal = norm3(rec.point - center);
    return true;
  }
  //Important to have back for dielectrics!
  t = (-b + sqrt(discriminant)) / (2.f*a);
  if (t < t_max && t > t_min){
    rec.t = t;
    rec.point = ray.pointAtParam(t);
    rec.normal = norm3(rec.point - center);
    return true;
  }

  return false;

}

bool HitableList::hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
  HitRecord tmpRec;
  bool hit = false;
  float closest = t_max;
  for(const auto& obj: objects) {
    if (obj->hit(r,t_min, closest, tmpRec)) {
      hit = true;
      closest = tmpRec.t;
      rec = tmpRec;
    }
  }
  return hit;
}
