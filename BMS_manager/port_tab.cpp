#include <string>
#include <list>
#include <vector>
#include <termios.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <time.h>
#include <tinyxml.h>

#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>

// Linux and windows
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
//
#include "gfxareas.h"
#include "mesh.h"
#include "gl2Dprimitives.h"

#include "serial/serialport.h"
#include "cfgfile.h"
//
#include "charge_tab.h"
#include "port_tab.h"
//
#include "SombreroBMS.h"
#include "shared.h"
#include "main.h"

using namespace std;

#define VERBOSE

void serial_apply_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared   = &papp_data->shared;
  int            selection;

  LOCK;
  if (pshared->pserial != NULL)
    {
      pshared->pserial->close_serial_port();
      delete pshared->pserial;
      pshared->pserial = NULL;
    }
  selection = papp_data->serialport_choice->value(0);
  if (selection < (int)papp_data->vserial_ports.size())
    {
      papp_data->ser_path = papp_data->vserial_ports[selection];
      if (strncmp(papp_data->vserial_ports[selection].c_str(), "/tmp/BMSsim_serialFIFO", strlen("/tmp/BMSsim_serialFIFO")) == 0)
	{
	  printf("Opening the local serial port simulation.\n");
	  bool bmaster = false;
	  pshared->pserial = new CSerialPortLocalSim(bmaster);
	}
      else
	{
	  printf("opening the serial port.\n");
	  pshared->pserial = new CSerialPort();
	}
      if (pshared->pserial->open_serial_port(papp_data->vserial_ports[selection].c_str()) == 0)
	printf("Failed to open the serial port: %s\n", papp_data->vserial_ports[selection].c_str());
    }
  UNLOCK;
}

int get_choices(t_app_data *papp_data)
{
  int serial_port;

  serial_port = papp_data->serialport_choice->value();
  return serial_port;
}

void fill_lists(t_app_data *papp_data)
{
  int         i;
  bool        bfound;
  const int   cpathsz = 1024;
  char        serialport_path[cpathsz];
  struct stat st;
  int         res;
  int         sel = 0;

  i = 0;
  bfound = true;
  // Add all the USB serial ports.
  while (bfound)
    {
      snprintf(serialport_path, cpathsz, "/dev/ttyUSB%d", i);
      res = stat(serialport_path, &st);
      if (res == 0)
	{
#ifdef VERBOSE
	  printf("Adding serial \"%s\"\n", serialport_path);
#endif
	  papp_data->serialport_choice->add(serialport_path);
	  papp_data->vserial_ports.push_back(std::string(serialport_path));
	  if (papp_data->ser_path == std::string(serialport_path))
	    sel  = i;
	  i++;
	}
      else
	bfound = false;
    }
  // Add the named pipe used with the simulator
  snprintf(serialport_path, cpathsz, "/tmp/BMSsim_serialFIFO");
  res = stat("/tmp/BMSsim_serialFIFO_TX", &st);
  if (res == 0)
    {
#ifdef VERBOSE
      printf("Adding serial \"%s\"\n", serialport_path);
#endif
      papp_data->serialport_choice->add(serialport_path);
      papp_data->vserial_ports.push_back(std::string(serialport_path));
      if (papp_data->ser_path == std::string(serialport_path))
	sel  = i;
    }
  papp_data->serialport_choice->value(sel);
}

void serialport_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data *papp_data = (t_app_data*)pdata;

  printf("api choice == %d\n", papp_data->serialport_choice->value());
}

void create_serialport_tab(int x, int y, int w, int h, void *pdata)
{
  t_app_data *papp_data = (t_app_data*)pdata;
  int         border;
  int         buttonw;
  int         text_width;

  Fl_Group *group1 = new Fl_Group(x, y, w, h, "Serial port");
  {
    border = 10;
    buttonw = 300;
    text_width = 120;
    papp_data->serialport_choice = new Fl_Choice(x - text_width + border + w / 2, y - border + h / 3, buttonw, 3 * border, " USB serial port      ");
    //papp_data->serialport_choice->value();
    papp_data->serialport_choice->user_data(papp_data);
    papp_data->serialport_choice->callback(serialport_callback);
    //
    fill_lists(papp_data);
    //
    //buttonw = 90;
    papp_data->but_ser_apply = new Fl_Button(x - text_width + 2 * border + w / 2 + buttonw, y - border + h / 3, 90, 3 * border, "Apply");
    set_button(papp_data->but_ser_apply, (void*)papp_data);
    papp_data->but_ser_apply->callback(serial_apply_callback);
    // Initialise the serial port if already in the config file
    if (papp_data->ser_path != string(""))
      {
	serial_apply_callback(NULL, papp_data);
      }
  }
  group1->end();
}

