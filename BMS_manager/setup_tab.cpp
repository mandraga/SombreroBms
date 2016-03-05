#include <stdlib.h>
#include <string>
#include <list>
#include <vector>
#include <termios.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <time.h>
#include <tinyxml.h>

// Linux and windows
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
//
#include "gfxareas.h"
#include "mesh.h"
#include "gl2Dprimitives.h"

#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>

#include "serial/serialport.h"
#include "cfgfile.h"
//
#include "charge_tab.h"
#include "setup_tab.h"
//
#include "env.h"
#include "SombreroBMS.h"
#include "shared.h"
#include "main.h"
#include "thrSerial.h"


using namespace std;

void setup_date_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared = &papp_data->shared;
  char           command[MAX_MSG_SIZE];

  if (fl_choice("Do you really want to program the value?", fl_no, fl_yes, 0))
    {
      snprintf(command, MAX_MSG_SIZE, "-date \"%s\"", papp_data->input_setup_date->value());
      LOCK;
      add_command_in_locked_area(eprogram, command, pshared);
      UNLOCK;
    }
}

void client_name_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared = &papp_data->shared;
  char           command[MAX_MSG_SIZE];
  char           client_name[CLIENT_STR_SIZE];

  if (fl_choice("Do you really want to program the value?", fl_no, fl_yes, 0))
    {
      strncpy(client_name, papp_data->input_client_name->value(), SERNUM_SIZE - 1);
      client_name[SERNUM_SIZE - 1] = '\0';
      snprintf(command, MAX_MSG_SIZE, "-client \"%s\"", client_name);
      LOCK;
      add_command_in_locked_area(eprogram, command, pshared);
      UNLOCK;
    }
}

void serial_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared = &papp_data->shared;
  char           command[MAX_MSG_SIZE];
  char           serial[MAX_MSG_SIZE];

  if (fl_choice("Do you really want to program the value?", fl_no, fl_yes, 0))
    {
      strncpy(serial, papp_data->input_serial->value(), SERNUM_SIZE - 1);
      serial[SERNUM_SIZE - 1] = '\0';
      snprintf(command, MAX_MSG_SIZE, "-serial \"%s\"", serial);
      LOCK;
      add_command_in_locked_area(eprogram, command, pshared);
      UNLOCK;
    }
}

void capacity_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared = &papp_data->shared;
  char           command[MAX_MSG_SIZE];

  if (fl_choice("Do you really want to program the value?", fl_no, fl_yes, 0))
    {
      snprintf(command, MAX_MSG_SIZE, "-fullcharge %d", 1000 * atoi(papp_data->input_capacityAH->value()));
      LOCK;
      add_command_in_locked_area(eprogram, command, pshared);
      UNLOCK;
    }
}

void full_pack_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared = &papp_data->shared;
  char           command[MAX_MSG_SIZE];

  if (fl_choice("Do you really want to program the value?", fl_no, fl_yes, 0))
    {
      snprintf(command, MAX_MSG_SIZE, "-fullVbat %d", (int)(1000. * atof(papp_data->input_FullpackV->value())));
      LOCK;
      add_command_in_locked_area(eprogram, command, pshared);
      UNLOCK;
    }
}

void minV_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared = &papp_data->shared;
  char           command[MAX_MSG_SIZE];

  if (fl_choice("Do you really want to program the value?", fl_no, fl_yes, 0))
    {
      snprintf(command, MAX_MSG_SIZE, "-batVmin %d", (int)(1000. * atof(papp_data->input_minV->value())));
      LOCK;
      add_command_in_locked_area(eprogram, command, pshared);
      UNLOCK;
    }
}

void maxV_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared = &papp_data->shared;
  char           command[MAX_MSG_SIZE];

  if (fl_choice("Do you really want to program the value?", fl_no, fl_yes, 0))
    {
      snprintf(command, MAX_MSG_SIZE, "-batVmax %d", (int)(1000. * atof(papp_data->input_maxV->value())));
      LOCK;
      add_command_in_locked_area(eprogram, command, pshared);
      UNLOCK;
    }
}

void minT_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared = &papp_data->shared;
  char           command[MAX_MSG_SIZE];

  if (fl_choice("Do you really want to program the value?", fl_no, fl_yes, 0))
    {
      snprintf(command, MAX_MSG_SIZE, "-tmin %d", atoi(papp_data->input_minT->value()));
      LOCK;
      add_command_in_locked_area(eprogram, command, pshared);
      UNLOCK;
    }
}

void maxT_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data    *papp_data = (t_app_data*)pdata;
  t_shared_data *pshared = &papp_data->shared;
  char           command[MAX_MSG_SIZE];

  if (fl_choice("Do you really want to program the value?", fl_no, fl_yes, 0))
    {
      snprintf(command, MAX_MSG_SIZE, "-tmax %d", atoi(papp_data->input_maxT->value()));
      LOCK;
      add_command_in_locked_area(eprogram, command, pshared);
      UNLOCK;
    }
}

void get_date(char *date, int size)
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  snprintf(date, size, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

#define YPOS (i++ * (buttonh + 5) + y + 4 * border)

void create_setup_tab(int x, int y, int w, int h, void *pdata)
{
  Fl_Group   *group1;
  t_app_data *papp_data = (t_app_data*)pdata;
  int         border;
  int         buttonw;
  int         buttonh;
  int         start, i;
  char        date[MAX_MSG_SIZE];

  group1 = new Fl_Group(x, y, w, h, "Factory Setup");
  {
    border = 10;
    buttonh = 24;
    buttonw = w / 8;
    start = w - buttonw;
    //
    i = 0;
    papp_data->but_program_setup_date = new Fl_Button(start, YPOS, buttonw, buttonh, "Program");
    set_button(papp_data->but_program_setup_date, papp_data);
    papp_data->but_program_setup_date->callback(setup_date_callback);
    //
    papp_data->but_program_client_name = new Fl_Button(start, YPOS, buttonw, buttonh, "Program");
    set_button(papp_data->but_program_client_name, papp_data);
    papp_data->but_program_client_name->callback(client_name_callback);
    //
    papp_data->but_program_serial = new Fl_Button(start, YPOS, buttonw, buttonh, "Program");
    set_button(papp_data->but_program_serial, papp_data);
    papp_data->but_program_serial->callback(serial_callback);
    i++;
    //
    papp_data->but_program_capacityAH = new Fl_Button(start, YPOS, buttonw, buttonh, "Program");
    set_button(papp_data->but_program_capacityAH, papp_data);
    papp_data->but_program_capacityAH->callback(capacity_callback);
    //
    papp_data->but_program_FullpackV = new Fl_Button(start, YPOS, buttonw, buttonh, "Program");
    set_button(papp_data->but_program_FullpackV, papp_data);
    papp_data->but_program_FullpackV->callback(full_pack_callback);
    //
    papp_data->but_program_minV = new Fl_Button(start, YPOS, buttonw, buttonh, "Program");
    set_button(papp_data->but_program_minV, papp_data);
    papp_data->but_program_minV->callback(minV_callback);
    //
    papp_data->but_program_maxV = new Fl_Button(start, YPOS, buttonw, buttonh, "Program");
    set_button(papp_data->but_program_maxV, papp_data);
    papp_data->but_program_maxV->callback(maxV_callback);
    //
    papp_data->but_program_minT = new Fl_Button(start, YPOS, buttonw, buttonh, "Program");
    set_button(papp_data->but_program_minT, papp_data);
    papp_data->but_program_minT->callback(minT_callback);
    //
    papp_data->but_program_maxT = new Fl_Button(start, YPOS, buttonw, buttonh, "Program");
    set_button(papp_data->but_program_maxT, papp_data);
    papp_data->but_program_maxT->callback(maxT_callback);
    //
    // Edition windows
    start = w  / 2;
    i = 0;
    buttonw = w * 0.35;
    papp_data->input_setup_date = new Fl_Input(start, YPOS, buttonw, buttonh, "Setup date");
    get_date(date, MAX_MSG_SIZE);
    papp_data->input_setup_date->value(date);
    papp_data->input_client_name = new Fl_Input(start, YPOS, buttonw, buttonh, "Client");
    papp_data->input_serial = new Fl_Input(start, YPOS, buttonw, buttonh, "Serial Number");
    i++;
    papp_data->input_capacityAH = new Fl_Int_Input(start, YPOS, buttonw, buttonh, "Capacity AH");
    papp_data->input_FullpackV = new Fl_Float_Input(start, YPOS, buttonw, buttonh, "Full Voltage (charger stops + 100% charge)");
    papp_data->input_minV = new Fl_Float_Input(start, YPOS, buttonw, buttonh, "Minimum battery V");
    papp_data->input_maxV = new Fl_Float_Input(start, YPOS, buttonw, buttonh, "Maximum battery V");
    papp_data->input_minT = new Fl_Int_Input(start, YPOS, buttonw, buttonh, "Min tempereature (like -5°C)");
    papp_data->input_minT->value("-1");
    papp_data->input_maxT = new Fl_Int_Input(start, YPOS, buttonw, buttonh, "Max tempereature (like 40°C)");
    papp_data->input_maxT->value("40");
  }
  group1->end();
}

