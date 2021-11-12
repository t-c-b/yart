#pragma once
// Single Header Vector math library
// Based on Peter Shirley's ray tracer in one weekend.
// Extended to vec4's

#include <cmath>

struct vec4 {
  vec4() {}

  vec4(float xr, float yg, float zb, float wa) {
    d[0]=xr; d[1]=yg; d[2]=zb; d[3]=wa;
  }

  inline float x() const {return d[0];}
  inline float y() const {return d[1];}
  inline float z() const {return d[2];}
  inline float w() const {return d[3];}
  inline float length() const {
    return sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]+d[3]*d[3]);
  }

  inline const vec4 operator-() const {return vec4(-d[0],-d[1],-d[2],-d[3]);}
  inline float operator[](int i) const {return d[i];}
  inline float& operator[](int i) {return d[i];}


  float d[4];
};


//Elementwise vector operations
inline vec4 operator+ (const vec4 &v1, const vec4 &v2) {
  return vec4(v1.d[0] + v2.d[0], v1.d[1] + v2.d[1], v1.d[2] + v2.d[2], v1.d[3]+v2.d[3]);
}
inline vec4 operator- (const vec4 &v1, const vec4 &v2) {
  return vec4(v1.d[0] - v2.d[0], v1.d[1] - v2.d[1], v1.d[2] - v2.d[2], v1.d[3]-v2.d[3]);
}
inline vec4 operator* (const vec4 &v1, const vec4 &v2) {
  return vec4(v1.d[0] * v2.d[0], v1.d[1] * v2.d[1], v1.d[2] * v2.d[2], v1.d[3]*v2.d[3]);
}
inline vec4 operator/ (const vec4 &v1, const vec4 &v2) {
  return vec4(v1.d[0] / v2.d[0], v1.d[1] / v2.d[1], v1.d[2] / v2.d[2], v1.d[3]/v2.d[3]);
}

//Scalar Operations
inline vec4 operator+ (const vec4 &v, const float &k) {
  return vec4(v.d[0] + k, v.d[1] + k, v.d[2] + k, v.d[3] + k);
}
inline vec4 operator- (const vec4 &v, const float &k) {
  return vec4(v.d[0] - k, v.d[1] - k, v.d[2] - k, v.d[3] - k);
}
inline vec4 operator* (const vec4 &v, const float &k) {
  return vec4(v.d[0] * k, v.d[1] * k, v.d[2] * k, v.d[3] * k);
}
inline vec4 operator/ (const vec4 &v, const float &k) {
  return vec4(v.d[0] / k, v.d[1] / k, v.d[2] / k, v.d[3] / k);
}

inline vec4 operator+ (const float &k, const vec4 &v) {
  return vec4(v.d[0] + k, v.d[1] + k, v.d[2] + k, v.d[3] + k);
}
inline vec4 operator- (const float &k, const vec4 &v) {
  return vec4(v.d[0] - k, v.d[1] - k, v.d[2] - k, v.d[3] - k);
}
inline vec4 operator* (const float &k, const vec4 &v) {
  return vec4(v.d[0] * k, v.d[1] * k, v.d[2] * k, v.d[3] * k);
}
inline vec4 operator/ (const float &k, const vec4 &v) {
  return vec4(v.d[0] / k, v.d[1] / k, v.d[2] / k, v.d[3] / k);
}

//Must be declared after operators
inline vec4 norm3 (const vec4 &v) {
  vec4 vv = v*v;
  float mag = sqrt(vv.d[0]+vv.d[1]+vv.d[2]);
  return v / mag;
}

//other vector operations
inline float dot3 (const vec4& v1, const vec4 &v2) {
  return v1.d[0]*v2.d[0] + v1.d[1]*v2.d[1] + v1.d[2]*v2.d[2];
}
inline vec4 cross3 (const vec4& v1, const vec4 &v2) {
  return vec4(
    v1.d[1]*v2.d[2] - v1.d[2]*v2.d[1],
    v1.d[2]*v2.d[0] - v1.d[0]*v2.d[2],
    v1.d[0]*v2.d[1] - v1.d[1]*v2.d[0],
    0.f
  );
}
struct mat4 {
  inline mat4 operator* (const mat4 &m) {
    mat4 result;
    for(int r=0; r<4; ++r) {
      for(int c=0; c<4; ++c) {
        result.d[r][c] =
          d[r][0] * m.d[0][c] + 
          d[r][1] * m.d[1][c] + 
          d[r][2] * m.d[2][c] + 
          d[r][3] * m.d[3][c];
      }
    }
    return result;
  }

  inline vec4 operator* (const vec4 &v) {
    vec4 result;
    for(int c=0; c<4; ++c){
      result.d[c] = 
        d[0][c]*v.d[0] +
        d[1][c]*v.d[1] +
        d[2][c]*v.d[2] + 
        d[3][c]*v.d[3];
    }
    return result;
  }

  float d[4][4];
};
