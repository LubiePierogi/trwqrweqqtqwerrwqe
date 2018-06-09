#pragma once

#include<string>
#include"Pixel.hpp"
#include"Exception.hpp"

namespace Arko
{
  class PNG final
  {
  private:
    class FromFile{};
  public:
    static constexpr FromFile fromFile{};
  public:
    PNG(); // Ten konstruktor daje pusty obiekt.
    PNG(FromFile,std::string);
    PNG(const PNG&);
    PNG&operator=(const PNG&);
    ~PNG();
    unsigned int getWidth()const;
    unsigned int getHeight()const;
    Pixel*getPixels();
  private:
    unsigned int width;
    unsigned int height;
    Pixel*pixels;
  };
}
