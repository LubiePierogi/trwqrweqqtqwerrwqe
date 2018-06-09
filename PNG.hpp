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
    class Empty{};
  public:
    static constexpr FromFile fromFile{};
    static constexpr Empty empty{};
  public:
    PNG(); // Ten konstruktor daje pusty obiekt.
    PNG(FromFile,std::string);
    PNG(Empty,unsigned,unsigned);
    PNG(const PNG&);
    PNG&operator=(const PNG&);
    ~PNG();
    unsigned int getWidth()const;
    unsigned int getHeight()const;
    Pixel*getPixels();
    void save(std::string);
  private:
    unsigned int width;
    unsigned int height;
    Pixel*pixels;
  };
}
