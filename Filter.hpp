#pragma once

#include<SDL2/SDL.h>
#include<GL/glew.h>

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
		void loadShadersAndPrograms();
		void mainLoop();
		void draw();

	private:
		int status_SDL_Init;
		SDL_Window*window;
		SDL_GLContext context;
		GLenum status_glewInit;

		GLuint vsh;
		GLuint fsh;
		GLuint shp;

	public:
		Filter();
		~Filter();
		int run(int,char**);
	};
}
