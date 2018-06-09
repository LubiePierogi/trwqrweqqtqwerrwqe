#include<iostream>


#include"PNG.hpp"

#include<png.h>

namespace Arko
{
  PNG::PNG():
    width(0),
    height(0),
    pixels(nullptr)
  {}
  PNG::PNG(PNG::FromFile,std::string name):
    PNG()
  {
    // Tutaj będzie zięcie obrazka przez libpng.

    //pixels=new Pixel[24*24];
  //  width=24;
//    height=24;

    bool good=false;
    char header[8];

    unsigned int w,h;
    int color_type;
    int bit_depth;
    png_structp png_ptr;
    png_infop info_ptr;
    png_infop end_ptr;
    png_bytep*row_pointers;
    int interlace_type;


    std::FILE*fp=fopen(name.c_str(),"rb");
    if(!fp)
      throw Exception("Nie udało się otworzyć pliku '"+name+"'.");
    std::fread(header,1,8,fp);
    if(png_sig_cmp(reinterpret_cast<png_const_bytep>(header),0,8))
    {
      std::fclose(fp);
      throw Exception("Plik '"+name+"' nie został rozpoznany jako plik PNG.");
    }
    png_ptr=
      png_create_read_struct(PNG_LIBPNG_VER_STRING,nullptr,nullptr,nullptr);
    if(!png_ptr)
    {
      std::fclose(fp);
      throw Exception("Nie poszło png_create_read_struct.");
    }
    info_ptr=png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
      png_destroy_read_struct(&png_ptr,nullptr,nullptr);
      std::fclose(fp);
      throw Exception("Nie poszło png_create_info_struct.");
    }
    if(setjmp(png_jmpbuf(png_ptr)))
    {
      png_destroy_read_struct(&png_ptr,&info_ptr,nullptr);
      std::fclose(fp);
      throw Exception("Jakiś tam błąd init_io, nie wiem, co to jest.");
    }
    png_init_io(png_ptr,fp);
    png_set_sig_bytes(png_ptr,8);
    png_read_info(png_ptr,info_ptr);

    std::cerr<<"zxczxc\n";

    png_get_IHDR
    (
      png_ptr,
      info_ptr,
      &w,
      &h,
      &bit_depth,
      &color_type,
      &interlace_type,
      nullptr,
      nullptr
    );

//    png_set_scale_16(png_ptr);
  //  png_set_strip_16(png_ptr);
//    png_set_packswap(png_ptr);
    // Read file?!

    std::cerr<<"zxczxccxzxc\n";

    row_pointers=static_cast<png_bytep*>(std::malloc(sizeof(png_bytep)*h));
    for(unsigned int y=0;y<h;++y)
      row_pointers[y]=
        static_cast<png_byte*>(std::malloc(png_get_rowbytes(png_ptr,info_ptr)));

    std::cerr<<"ghghghghg\n";
    png_read_image(png_ptr,row_pointers);

    std::cerr<<"okdoskad\"\n";

    png_read_end(png_ptr,info_ptr);

    std::cerr<<"cvvcvcvc\n";

    if
    (
      color_type==PNG_COLOR_TYPE_RGBA &&
      bit_depth==8
    )
    {
      std::cerr<<"qwer\n";
      good=true;
      width=w;
      height=h;
      pixels=new Pixel[w*h];
      for(unsigned int y=0;y<h;++y)
      {
        for(unsigned int x=0;x<w;++x)
        {
          pixels[y*w+x]=
            *(reinterpret_cast<Pixel*>(row_pointers[y]+x*sizeof(Pixel)));
        }
      }
    }
    std::cerr<<"qewqweqewqwqewq\n";


    for(unsigned int y=0;y<h;++y)
      std::free(row_pointers[y]);
    std::free(row_pointers);

    if(!good)
      throw Exception
      (
        std::string()+
        "Zły format obrazu:\n"+
        std::to_string(png_get_color_type(png_ptr,info_ptr))+
        '\n'+
        std::to_string(bit_depth)+
        '\n'+
        std::to_string(w)+
        '\n'+
        std::to_string(h)
      );

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
