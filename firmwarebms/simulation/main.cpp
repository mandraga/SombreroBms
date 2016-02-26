//#include <iostream>
#include <unistd.h>
#include <string>
#include <list>
#include <termios.h>

#include <time.h>
#include <pthread.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../env.h"
#include "../main.h"
#include "../init.h"
#include "../AD7280A.h"
#include "../balancing.h"
#include "../state_machine.h"
//
#include "fake_battery.h"
#include "fake_AD7280A.h"
#include "fake_charger.h"
#include "serialport.h"
#include "fake_uart.h"
//
#include "gfxareas.h"
#include "keypress.h"
#include "app.h"
#include "thrSerial.h"
#include "sdlcalls.h"

using namespace std;

Cappdata  *g_papp;

t_pack_variable_data g_appdata;
t_eeprom_data        g_edat;
t_eeprom_battery     g_bat[MAXBATTERY];
t_ad7280_state       g_ad7280;
t_balancing          g_balancing;

void main_loop(Cappdata *app)
{
  while (!app->m_bquit)
    {
      //printf("loop\n");
      handle_input_events(app);
      app->render_gui();
      //usleep(7000);
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
  g_papp = app;
  start_serial_thread(app);
  main_loop(app);
  stop_serial_thread(app);
  delete app;
}

