#include <string>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <time.h>
#include <tinyxml.h>

#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>

#include "serialport.h"
#include "main.h"

using namespace std;

#define VERBOSE

void serial_apply_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data *papp_data = (t_app_data*)pdata;
  int         selection;

  if (papp_data->pserial != NULL)
    {
      papp_data->close_serial_port();
      delete papp_data->pserial;
      papp_data->pserial = NULL;
    }
  selection = papp_data->serial_port_choice->value(0);
  papp_data->pserial = new CSerialPort();
  papp_data->open_serial_port(papp_data->vserial_ports[selection].c_str());
  papp_data->pBMS->set_port(papp_data->pserial);
}

void get_choices(t_app_data *papp_data)
{
  t_portaudio_api *apiptr;
  int serial_port;

  serial_port = papp_data->serialport_choice->value();
}

void fill_lists(t_app_data *papp_data)
{
  int         i;
  bool        bfound;
  const int   cpathsz = 1024;
  char        serial_port_path[cpathsz];
  struct stat st;
  int         res;

  i = 0;
  bfound = true;
  // Add all the USB serial ports.
  while (bfound)
    {
      snprintf(serial_port_path, cpathsz, "/dev/ttyUSB%d", i);
      res = stat(serial_port_path, &st);
      if (res == 0)
	{
#ifdef VERBOSE
	  printf("Adding serial \"%s\"\n", serial_port_path);
#endif
	  papp_data->serial_port_choice->add(serial_port_path);
	  papp_data->vserial_ports[i] = std::string(serial_port_path);
	}
      else
	bfound = false;
      i++;
    }
  // Add the named pipe used with the simulator
  snprintf(serial_port_path, cpathsz, "/tmp/BMSsim_serialFIFO");
  res = stat(serial_port_path, &st);
  if (res == 0)
    {
#ifdef VERBOSE
      printf("Adding serial \"%s\"\n", serial_port_path);
#endif
      papp_data->serial_port_choice->add(serial_port_path);
      papp_data->vserial_ports[i] = std::string(serial_port_path);
    }
  papp_data->serial_port_choice->value(0);
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
  int         buttonh;
  int         text_width;

  Fl_Group *group1 = new Fl_Group(x, y, w, h, "Serial ports");
  {
    border = 10;
    buttonw = 300;
    buttonh = 50;
    text_width = 120;
    papp_data->serialport_choice = new Fl_Choice(x + text_width + border, y + border, buttonw, 3 * border, " USB serial port      ");
    papp_data->serialport_choice->value(papp_data->bstartrecord);
    papp_data->serialport_choice->user_data(papp_data);
    papp_data->serialport_choice->callback(serialport_callback);
    //
    fill_lists(papp_data);
    //
    buttonw = 90;
    buttonh = 50;
    papp_data->but_ser_apply = new Fl_Button(x + w - buttonw - border, y + h - buttonh - 2 * border, buttonw, buttonh, "Apply");
    set_button(papp_data->but_ser_apply, papp_data);
    papp_data->but_ok->callback(serial_apply_callback);
    //
  }
  group1->end();
}

