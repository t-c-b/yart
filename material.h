#pragma once
#include "rand.h"

class Material {
public:
  virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec4& attenuation, Ray& scattered) const = 0;
  int emissive=0;
  vec4 color={0,0,0,0};
};

class Emissive : public Material {
public:
  Emissive(const vec4& c) {
    color = c;
    emissive = 1;
  }
  virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec4& attenuation, Ray& scattered) const {
    return false;
  }
};

class Lambertian : public Material {
public:
  Lambertian(const vec4& a) : albedo(a) {}
  virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec4& attenuation, Ray& scattered) const {
    vec4 target = rec.point + rec.normal + randSphere();
    scattered = Ray(rec.point, target-rec.point);
    attenuation = albedo;
    return true;
  }

  vec4 albedo;
};

vec4 reflect(const vec4& v, const vec4& n) {return v- 2.f*dot3(v,n)*n;}

class Metallic : public Material {
public:
  Metallic(const vec4& a, float fuzz) : albedo(a), roughness(fuzz) {}
  virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec4& attenuation, Ray& scattered) const {
    vec4 reflected =  reflect(r_in.direction, rec.normal);
    scattered = Ray(rec.point, reflected + roughness*randSphere());
    attenuation = albedo;
    return (dot3(scattered.direction, rec.normal) > 0);
  }
  
  vec4 albedo;
  float roughness;
};

bool refract (const vec4& v, const vec4& n, float ni_over_nt, vec4& refracted) {
  vec4 uv = norm3(v);
  float dt = dot3(uv,n);
  float discriminant = 1.f - ni_over_nt*ni_over_nt*(1-dt*dt);
  if (discriminant > 0) {
    refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
    return true;
  }
  else
    return false;
}

float schlick(float cosine, float ref_idx) {
  float r0 = (1-ref_idx) / (1+ref_idx);
  r0 = r0*r0;
  return r0 + (1-r0)*pow((1-cosine),5);
}

class Dielectric : public Material {
public:
  Dielectric(float ri) : ref_idx(ri) {}
  virtual bool scatter(const Ray& r_in, const HitRecord& rec, vec4& attenuation, Ray& scattered) const {
    vec4 outward_normal;
    vec4 reflected = reflect(r_in.direction,rec.normal);
    float ni_over_nt;
    attenuation = vec4(1,1,1,0);
    vec4 refracted;
    
    float cosine;
    if(dot3(r_in.direction, rec.normal) > 0){
      outward_normal = -rec.normal;
      ni_over_nt = ref_idx;
      cosine = ref_idx * dot3(r_in.direction,rec.normal) / r_in.direction.length();
    }
    else {
      outward_normal = rec.normal;
      ni_over_nt = 1.f / ref_idx;
      cosine = -dot3(r_in.direction,rec.normal) / r_in.direction.length();
    }

    float reflect_prob;
    if (refract(r_in.direction, outward_normal, ni_over_nt, refracted)){
      //scattered = Ray(rec.point, refracted);
      reflect_prob = schlick(cosine, ref_idx);
    }
    else {
      //scattered = Ray(rec.point, reflected);
      reflect_prob = 1.0;
    }

    if(randUniform() <= reflect_prob)
      scattered = Ray(rec.point, reflected);
    else
      scattered = Ray(rec.point, refracted);
    return true;
  }
      
  float ref_idx;
};
