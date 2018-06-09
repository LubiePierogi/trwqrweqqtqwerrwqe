#include"PNG.hpp"
namespace Arko
{
  PNG::PNG():
    width(0),
    height(0),
    pixels(nullptr)
  {}
  PNG::PNG(PNG::FromFile,std::string name)
  {
    // Tutaj będzie zięcie obrazka przez libpng.

    pixels=new Pixel[24*24];
    width=24;
    height=24;
  }
  PNG::~PNG()
  {
    if(pixels)
      delete[] pixels;
  }
  PNG::PNG(const PNG&other)
  {
    width=other.width;
    height=other.height;
    pixels=new Pixel[width*height];
    for(unsigned int i=0;i<height;++i)
      for(unsigned int j=0;j<width;++j)
        pixels[j*width+i]=other.pixels[j*width+i];
  }
  PNG&PNG::operator=(const PNG&other)
  {
    if(this==&other)
      return *this;
    if(pixels)
      delete[] pixels;
    width=other.width;
    height=other.height;
    pixels=new Pixel[width*height];
    for(unsigned int i=0;i<height;++i)
      for(unsigned int j=0;j<width;++j)
        pixels[j*width+i]=other.pixels[j*width+i];
    return *this;
  }
  unsigned int PNG::getWidth()const
  {
    return width;
  }
  unsigned int PNG::getHeight()const
  {
    return height;
  }
  Pixel*PNG::getPixels()
  {
    return pixels;
  }
}
