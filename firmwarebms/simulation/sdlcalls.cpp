
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <iterator>
#include <list>
 
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h> 

#include <GL/gl.h>
#include <GL/glu.h>

#include "gfxareas.h"
#include "keypress.h"
#include "app.h"
#include "sdlcalls.h"

int init_SDL_image()
{
  int imgFlags = IMG_INIT_PNG;

  if (!(IMG_Init(imgFlags) & imgFlags))
    {
      printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
      exit(0);
      //return 0;
    }
  return 0;
}

int init_SDL(SDL_Window **psdlwindow, SDL_Renderer **sdlRenderer, int x, int y, int width, int height)
{
  SDL_RendererInfo rinfo;
  int              back_color;

  atexit(SDL_Quit);
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  init_openGL(width, height);
  *psdlwindow = SDL_CreateWindow("vbat",
				 x,
				 y,
				 width, height,
				 /*SDL_WINDOW_FULLSCREEN | */SDL_WINDOW_OPENGL);
  if (*psdlwindow == NULL)
    {
      fprintf(stderr, "SDL video initialization failed: %s\n", SDL_GetError( ));
      exit(0);
    }

  *sdlRenderer = SDL_CreateRenderer(*psdlwindow, -1, SDL_RENDERER_PRESENTVSYNC);
  if (*sdlRenderer == NULL)
    {
      fprintf(stderr, "SDL video initialization failed: %s\n", SDL_GetError( ));
      exit(0);
    }
  SDL_GetRendererInfo(*sdlRenderer, &rinfo);
  printf("Renderer: %s %s\n", rinfo.name, (rinfo.flags | SDL_RENDERER_ACCELERATED) ? "(Accelerated)" : "");

  // Clear the window buffer
  back_color = 0xFF000000;
  SDL_SetRenderDrawColor(*sdlRenderer, (back_color >> 16) & 0xFF, (back_color >> 8) & 0xFF, back_color & 0xFF, 255);
  SDL_RenderClear(*sdlRenderer);
  SDL_RenderPresent(*sdlRenderer);
  // Anti aliasing configuration
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
  SDL_RenderSetLogicalSize(*sdlRenderer, width, height);
  SDL_RenderSetViewport(*sdlRenderer, NULL);

  //SDL_ShowCursor(SDL_DISABLE);
  SDL_ShowCursor(SDL_ENABLE);
  return (init_SDL_image());
}

int close_SDL(SDL_Window *psdlwindow, SDL_Renderer *sdlRenderer)
{
  SDL_DestroyRenderer(sdlRenderer);
  SDL_DestroyWindow(psdlwindow);
  SDL_Quit();
  return 0;
}

void init_openGL(int width, int height)
{
  //glEnable(GL_CULL_FACE);
  //glDisable(GL_FOG);
  //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //glViewport(0, 0, width, height);  

  /* Upload GL settings */
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 3);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 3);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 2);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 1);
}

void inject_time_pulse(double& last_time_pulse)
{
  // get current "run-time" in seconds
  double t = 0.001 * SDL_GetTicks();

  // store the new time as the last time
  last_time_pulse = t;
}

void handle_mouse_down(int button)
{
}

void handle_mouse_up(int button)
{
}

void handle_input_events(Cappdata *app)
{
  SDL_Event           e;

  app->setlooptime();
  //SDL_WaitEvent(&event);
  while (SDL_PollEvent(&e))
    {
      switch (e.type)
	{
	case SDL_MOUSEWHEEL:
	  break;
	case SDL_FINGERMOTION:
	  break;
	  // mouse motion handler 
	case SDL_MOUSEMOTION:
	  break;
	  // mouse down handler
	case SDL_MOUSEBUTTONDOWN:
	  break;
	  // mouse up handler
	case SDL_MOUSEBUTTONUP:
	  break;
	  // key down
	case SDL_KEYDOWN:
	  //printf("%c %c\n", e.key.keysym.sym, SDL_GetScancodeFromKey(e.key.keysym.sym));
	  //switch (e.key.keysym.scancode)
	  app->key_on(e.key.keysym.sym);
	  break;
	  // key up
	case SDL_KEYUP:
	  app->key_off(e.key.keysym.sym);
	  break;
	  // WM quit event occured
	case SDL_QUIT:
	  printf("Quit event receives\n");
	  app->quit();
	  break;
	/*
	case SDL_VIDEORESIZE:
	  printf("Resize video input\n");
	  break;*/
	}
    }
}

