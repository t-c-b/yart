#pragma once
// Single header image library supporting PPM
// Based on Peter Shirley's ray tracer in one weekend.

#include <fstream>
#include <memory>

struct Color {
  unsigned char r,g,b;
};

class Image {
public:
  Image(unsigned int width, unsigned int height)
  : width(width), height(height) {
    img = std::make_unique<Color[]>(width*height);
  }

  //Assign pixel
  void setPixel(unsigned int x, unsigned int y, Color val) {
    img[y*width +x] = val;
  }

  //Convert value from float triplet
  void setPixel(unsigned int x, unsigned int y, const float* val) {
    setPixel(x, y, Color {
        .r = static_cast<unsigned char>(clamp(val[0], 0, 1) * 255.99),
        .g = static_cast<unsigned char>(clamp(val[1], 0, 1) * 255.99),
        .b = static_cast<unsigned char>(clamp(val[2], 0, 1) * 255.99)
      }
    );
  }

  void writeTextPPM(const char * path) {
    std::ofstream fs(path);
    fs << "P3\n" << width << " " << height << "\n255\n";
    for (int i=0; i<height; ++i) {
      for (int j=0; j<width; ++j) {
        auto px = img[i*width + j];
        fs << 
          static_cast<int>(px.r) << " " << 
          static_cast<int>(px.g) << " " << 
          static_cast<int>(px.b) << "\n";
      }
    }
  }

  private:

  float clamp(float val, float min, float max) {
    val = val < min ? min : val;
    val = val > max ? max : val;
    return val;
  }

  unsigned int width, height;
  std::unique_ptr<Color[]> img;
};
