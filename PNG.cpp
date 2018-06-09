#include<iostream>


#include"PNG.hpp"

#include<png.h>

namespace Arko
{
  constexpr PNG::FromFile PNG::fromFile;
  constexpr PNG::Empty PNG::empty;
  PNG::PNG():
    width(0),
    height(0),
    pixels(nullptr)
  {}
  PNG::PNG(const FromFile&,std::string name)
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
;
    // Read file?!

    /*if(bit_depth==16)
      png_set_strip_16(png_ptr);
    if(color_type==PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb(png_ptr);
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_expand_gray_1_2_4_to_8(png_ptr);
    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
      png_set_tRNS_to_alpha(png_ptr);
    if
    (
      color_type == PNG_COLOR_TYPE_RGB ||
      color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE
    )
      png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
    if
    (
      color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA
    )
      png_set_gray_to_rgb(png_ptr);

    png_read_update_info(png_ptr,info_ptr);

*/

    row_pointers=static_cast<png_bytep*>(std::malloc(sizeof(png_bytep)*h));
    for(unsigned int y=0;y<h;++y)
      row_pointers[y]=
        static_cast<png_byte*>(std::malloc(png_get_rowbytes(png_ptr,info_ptr)));

    png_read_image(png_ptr,row_pointers);


    png_read_end(png_ptr,info_ptr);


    png_destroy_read_struct(&png_ptr,&info_ptr,nullptr);
    std::fclose(fp);

    if
    (
      color_type==PNG_COLOR_TYPE_RGBA &&
      bit_depth==8
    )
    {
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


    for(unsigned int y=0;y<h;++y)
      std::free(row_pointers[y]);
    std::free(row_pointers);

    if(!good)
    {
      throw Exception
      (
        std::string()+
        "Zły format obrazu:\n"+
        std::to_string(color_type)+
        '\n'+
        std::to_string(bit_depth)+
        '\n'+
        std::to_string(w)+
        '\n'+
        std::to_string(h)
      );
    }
  }
  PNG::PNG(const Empty&,unsigned w,unsigned h):
    width(w),
    height(h)
  {
    pixels=new Pixel[w*h];
    for(int i=0;i<w*h;++i)
      pixels[i]=Pixel{0,0,0,0};
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
    for(unsigned int y=0;y<height;++y)
      for(unsigned int x=0;x<width;++x)
        pixels[y*width+x]=other.pixels[y*width+x];
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
    for(unsigned int y=0;y<height;++y)
      for(unsigned int x=0;x<width;++x)
        pixels[y*width+x]=other.pixels[y*width+x];
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
  void PNG::save(std::string name)
  {
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep*row_pointers;
    row_pointers=new png_bytep[height];
    for(unsigned y=0;y<height;++y)
    {
      row_pointers[y]=reinterpret_cast<png_bytep>(pixels+width*y);
    }
    std::FILE*fp=std::fopen(name.c_str(),"wb");
    if(!fp)
      throw Exception("Nie udało się otworzyć pliku '"+name+"' do zapisu.\n");
    png_ptr=
      png_create_write_struct(PNG_LIBPNG_VER_STRING,nullptr,nullptr,nullptr);
    if(!png_ptr)
    {
      std::fclose(fp);
      throw Exception("Nie zrobiono struktury do zapisu.");
    }
    info_ptr=png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
      png_destroy_write_struct(&png_ptr,nullptr);
      std::fclose(fp);
      throw Exception("Nie poszło png_create_info_struct.");
    }
    if(setjmp(png_jmpbuf(png_ptr)))
    {
      png_destroy_write_struct(&png_ptr,&info_ptr);
      std::fclose(fp);
      throw Exception("Jakiś tam błąd init_io, nie wiem, co to jest.");
    }

    png_init_io(png_ptr,fp);

    png_set_IHDR
    (
      png_ptr,
      info_ptr,
      width,
      height,
      8,
      PNG_COLOR_TYPE_RGBA,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT,
      PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png_ptr,info_ptr);
    png_write_image(png_ptr,row_pointers);
    png_write_end(png_ptr,nullptr);

    delete[] row_pointers;
    png_destroy_write_struct(&png_ptr,&info_ptr);
    std::fclose(fp);

  }
}
