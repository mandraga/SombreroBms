//#include <iostream>
#include <unistd.h>
#include <string>
#include <list>

#include <time.h>
#include <pthread.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "gfxareas.h"
#include "keypress.h"
#include "app.h"
#include "sdlcalls.h"

using namespace std;

void main_loop(Cappdata *app)
{
  while (!app->m_bquit)
    {
      //printf("loop\n");
      handle_input_events(app);
      app->render_gui();
      usleep(7000);
    }
}

int main(int argc, char **argv)
{
  int            width;
  int            height;
  Cappdata       *app;

  width = 1024;
  height = 758;
  // Central class
  app = new Cappdata(width, height);
  main_loop(app);
  delete app;
}

