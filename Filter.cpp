#include<iostream>
#include<string>
#include<SDL2/SDL.h>
#include<GL/glew.h>
#include"function.h"
#include"Filter.hpp"
#include"Exception.hpp"

namespace Arko
{
  constexpr const char*Filter::vertexShader;
  constexpr const char*Filter::fragmentShader;
  constexpr Filter::Vertex Filter::vertices[];
  constexpr Filter::TexCoord Filter::texCoords[];
  Filter::Filter(int c,char**v):
    argc(c),
    argv(v),
    status_SDL_Init(0),
    window(nullptr),
    context(nullptr),
    status_glewInit(0),
    vsh(0),
    fsh(0),
    shp(0),
    attrVertex(0),
    attrTexCoord(0),
    texture(0),
    drawTexturePointer(nullptr),
    image(),
    imageNew(),
    imageWidth(0),
    imageHeight(0)
  {}
  Filter::~Filter()
  {
    if(shp!=0)
    {
      glDeleteProgram(shp);
    }
    if(context)
    {
      SDL_GL_DeleteContext(context);
    }
    if(window)
    {
      SDL_DestroyWindow(window);
    }
    SDL_Quit();
  }
  int Filter::run()
  {
    if(argc<2)
    {
      std::cerr<<"Nie podano nazwy pliku wejściowego.\n";
      return -1;
    }
    try
    {
      loadImageFromFile(argv[1]);
    }
    catch(Exception& exc)
    {
      std::cerr<<exc.what()<<'\n';
      return -1;
    }
    int status_SDL_Init=SDL_Init(SDL_INIT_VIDEO);
    if(status_SDL_Init)
    {
      std::cerr
        <<"Nie udało się zainicjalizować SDL:\n"
        <<SDL_GetError()
        <<'\n'
      ;
      return -1;
    }
    window=SDL_CreateWindow
    (
      "werrtwerwerr",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800,
      600,
      SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
    );
    if(!window)
    {
      std::cerr
        <<"Nie udało się stworzyć okna:'\n'"
        <<SDL_GetError()
        <<'\n'
      ;
      return -1;
    }
    context=SDL_GL_CreateContext(window);
	  if(context=nullptr)
	  {
		  std::cerr
        <<"Nie udało się zrobić kontekstu OpenGL:\n"
        <<SDL_GetError()
        <<'\n'
      ;
	  	return -1;
	  }
    status_glewInit=glewInit();
    if(status_glewInit!=GLEW_OK)
    {
      std::cerr
        <<"Nie udało się zainicjalizować GLEW:\n"
        <<glewGetErrorString(status_glewInit)
        <<'\n'
      ;
		  return -1;
    }
  	try
  	{
	  	loadShadersAndPrograms();
	  }
  	catch(Exception& exc)
  	{
	  	std::cerr
        <<"Błąd:\n"
        <<exc.what()
        <<'\n'
      ;
      return -1;
	  }
    setOpenGL();
    mainLoop();
    unsetOpenGL();
    return 0;
  }
  void Filter::loadShadersAndPrograms()
  {
	  GLint status;

    vsh=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsh,1,&vertexShader,0);
    glCompileShader(vsh);

    glGetShaderiv(vsh,GL_COMPILE_STATUS,&status);
    if(!status)
    {
      GLint d=0;
      glGetShaderiv(vsh,GL_INFO_LOG_LENGTH,&d);
      char*q=new char[d];
      glGetShaderInfoLog(vsh,d,&d,q);
      std::string msg(std::string("Vertex shader\n")+q);
      delete[] q;
      throw Exception(msg);
    }

    fsh=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsh,1,&fragmentShader,0);
    glCompileShader(fsh);

    glGetShaderiv(fsh,GL_COMPILE_STATUS,&status);
    if(!status)
    {
      GLint d=0;
      glGetShaderiv(fsh,GL_INFO_LOG_LENGTH,&d);
      char*q=new char[d];
      glGetShaderInfoLog(fsh,d,&d,q);
      std::string msg(std::string("Fragment shader\n")+q);
      delete[] q;
      throw Exception(msg);
    }

    shp=glCreateProgram();
    glAttachShader(shp,vsh);
    glAttachShader(shp,fsh);
    glLinkProgram(shp);

    glGetProgramiv(shp,GL_LINK_STATUS,&status);
    if(!status)
    {
      GLint d=0;
      glGetProgramiv(shp,GL_INFO_LOG_LENGTH,&d);
      char*q=new char[d];
      glGetProgramInfoLog(shp,d,&d,q);
      std::string msg(std::string("Program\n")+q);
      delete[] q;
      throw Exception(msg);
    }


    attrVertex=glGetAttribLocation(shp,"vertex");
    if(attrVertex==-1)
      throw Exception("vertex nie działa");
    attrTexCoord=glGetAttribLocation(shp,"texCoord");
    if(attrTexCoord==-1)
      throw Exception("texCoord nie działa");

/*

 SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,1);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 petla(okno);
 glDeleteProgram(program);
 glDeleteProgram(program1);
 //glDeleteTextures(1,&texture_litera);
}
*/
  }
  void Filter::mainLoop()
  {
    SDL_Event ev;
    bool notQuit=true;
    while(notQuit)
    {
      draw();
      while(SDL_PollEvent(&ev))
      {
        switch(ev.type)
        {
          case SDL_QUIT:
            notQuit=false;
            break;
          case SDL_WINDOWEVENT:
            switch(ev.window.event)
            {
              case SDL_WINDOWEVENT_RESIZED:
                resizeViewport(ev.window.data1,ev.window.data2);
                break;
              default:;
            }
            break;
          case SDL_KEYDOWN:
            switch(ev.key.keysym.sym)
            {
              case SDLK_SPACE:
                editImage();
                break;
              case SDLK_RETURN:
                saveImageToFile("output.png");
                break;
              default:;
            }
            break;
          default:;
        }
      }
    }
  }
  void Filter::setOpenGL()
  {
    glClearColor(0.0,0.0,0.0,1.0);

    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shp);

    glEnableVertexAttribArray(attrVertex);
    glEnableVertexAttribArray(attrTexCoord);
    glVertexAttribPointer
    (
      attrVertex,
      2,
      GL_FLOAT,
      GL_FALSE,
      0,
      vertices
    );
    glVertexAttribPointer
    (
      attrTexCoord,
      2,
      GL_FLOAT,
      GL_FALSE,
      0,
      texCoords
    );
  }
  void Filter::draw()
  {
    do
    {


      glGenTextures(1,&texture);
      glBindTexture(GL_TEXTURE_2D,texture);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      glTexImage2D
      (
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        image.getWidth(),
        image.getHeight(),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image.getPixels()
      );
    }while(false);





    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    SDL_GL_SwapWindow(window);

    glDeleteTextures(1,&texture);
  }
  void Filter::unsetOpenGL()
  {
    glDisableVertexAttribArray(attrVertex);
    glDisableVertexAttribArray(attrTexCoord);
  }
  void Filter::loadImageFromFile(std::string name)
  {
    image=PNG(PNG::fromFile,name);
  }
  void Filter::saveImageToFile(std::string name)
  {

  }
  void Filter::editImage()
  {

  }
  void Filter::resizeViewport(int width,int height)
  {
    glViewport(0,0,width,height);
  }
}
