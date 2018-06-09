#pragma once

#include<SDL2/SDL.h>
#include<GL/glew.h>
#include<string>
#include"Pixel.hpp"
#include"PNG.hpp"

namespace Arko
{
  class Filter
  {
  private:
    constexpr static const char*vertexShader=
      "#version 120\n"
      "attribute vec2 vertex;\n"
      "attribute vec2 texCoord;\n"
      "varying vec2 texCoordF;\n"
      "void main(void)\n"
      "{\n"
      "  gl_Position=vec4(vertex,0.0,1.0);\n"
      "  texCoordF=texCoord;\n"
      "}\n"
    ;
    constexpr static const char*fragmentShader=
      "#version 120\n"
      "varying vec2 texCoordF;\n"
      "uniform sampler2D texture;\n"
      "void main(void)\n"
      "{\n"
      "  gl_FragColor=texture2D(texture,texCoordF);\n"
      "}\n"
    ;
  private:
    struct Vertex final
    {
      float x;
      float y;
      // Te zmienne muszą być po sobie i w takiej kolejności, bo inaczej
      // nie będzie działać tak jak trzeba.
    };
    struct TexCoord final
    {
      float x;
      float y;
      // Tutaj tak samo jak wyżej.
    };

    constexpr static Vertex vertices[4]
    {
      {-1.f,-1.f},
      { 1.f,-1.f},
      { 1.f, 1.f},
      {-1.f, 1.f}
    };

    constexpr static TexCoord texCoords[4]
    {
      {0.f,0.f},
      {1.f,0.f},
      {1.f,1.f},
      {0.f,1.f}
    };


  private:
    void loadShadersAndPrograms();
    void setOpenGL();
    void unsetOpenGL();
    void mainLoop();
    void draw();
    void resizeViewport(int,int);

    void loadImageFromFile(std::string name);

    void saveImageToFile(std::string name);

    void editImage();

  private:

    int argc;
    char**argv;

  private:
    int status_SDL_Init;
    SDL_Window*window;
    SDL_GLContext context;
    GLenum status_glewInit;

    GLuint vsh;
    GLuint fsh;
    GLuint shp;

    GLint attrVertex;
    GLint attrTexCoord;

    GLuint texture;

    void*drawTexturePointer;

  private:

    PNG image;
    PNG imageNew;

    unsigned int imageWidth;
    unsigned int imageHeight;

  public:
    Filter(int,char**);
    ~Filter();
    int run();
  };
}
