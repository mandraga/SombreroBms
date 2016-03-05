/*
 SombreroBMS - BMS_manager
 Copyright (C) 2016 Patrick Areny
 All Rights Reserved.

 BMS_manager is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <unistd.h>
#include <string>
#include <list>
#include <vector>
#include <termios.h>

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

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Icon.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>

#include "serial/serialport.h"
#include "cfgfile.h"
//
#include "port_tab.h"
#include "about_tab.h"
#include "battery_tab.h"
#include "charge_tab.h"
#include "info_tab.h"
#include "setup_tab.h"
//
#include "SombreroBMS.h"
#include "shared.h"
#include "main.h"
#include "thrSerial.h"

#define REFRESHDELAYS 1.0

void refresh_timer_callback(void *user_data)
{
  t_app_data                           *papp_data = (t_app_data*)user_data;
  t_shared_data                        *pshared = &papp_data->shared;
  //int                                   currenttick;
  std::list<t_serial_request>::iterator iter;
  bool                                  bupdate = false;
  etab                                  active_tab;
  Fl_Widget                            *pactive_tab_widget;

  active_tab = tabunknown;
  //printf("-------------- timer -----------------\n");
  pactive_tab_widget = papp_data->tabs->value();
  if (strcmp("BMS Parameters", pactive_tab_widget->label()) == 0)
    {
      printf("Active tab: BMS Parameters.\n");
      active_tab = tabparam;      
    }
  if (strcmp("Batteries", pactive_tab_widget->label()) == 0)
    {
      printf("Active tab: Batteries.\n");
      active_tab = tabcharge;
    }
  LOCK;
  pshared->active_tab = active_tab;
  // If anything was updated then refresh the current tab
  iter = pshared->request.begin();
  while (iter != pshared->request.end())
    {
      if ((*iter).type == eupdategui)
	{
	  bupdate = true;
	  iter = pshared->request.erase(iter);
	}
      else
	iter++;
    }
  switch (pshared->active_tab)
    {
    case tabcharge:
      {
	add_command_in_locked_area(ereport, " ", pshared);
	if (bupdate)
	  {
	    UNLOCK;
	    update_charge_tab(papp_data);
	    LOCK;
	  }
      }
      break;
    case tabparam:
      {
	//add_command_in_locked_area(eping, " ", pshared);
	add_command_in_locked_area(einformation, " ", pshared);
	if (bupdate)
	  {
	    UNLOCK;
	    update_information_tab(papp_data);
	    LOCK;
	  }
      }
      break;
    case tabhist:
    case tabunknown:
    case tabport:
    case tababout:
    case tabsetup:
    default:
      break;
    };
  UNLOCK;
  Fl::repeat_timeout(REFRESHDELAYS, refresh_timer_callback, papp_data);
}

// This timer will send commands to the BMS and refresh the viewed tab
void install_timer(t_app_data *pappdata)
{
  double timeout;

  timeout = REFRESHDELAYS;
  Fl::add_timeout(timeout, refresh_timer_callback, pappdata);
}

