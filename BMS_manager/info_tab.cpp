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
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Tabs.H>
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
#include "battery_tab.h"
#include "charge_tab.h"
#include "info_tab.h"
//
#include "env.h"
#include "SombreroBMS.h"
#include "shared.h"
#include "main.h"

using namespace std;

// FIXME reorder the includes so that void will be the real type and not void
void update_information_tab(void *app_data)
{
  t_app_data      *papp_data = (t_app_data*)app_data;
  t_shared_data   *pshared = &papp_data->shared;
  t_params         BMSparams;
  char             text[4096];
  string           out;

  // Get the text from the BMS
  LOCK;
  BMSparams = papp_data->shared.pBMS->m_params;
  UNLOCK;
  // Print them
  if (strlen(pshared->bms_version) == 0)
    {
      LOCK;
      if (papp_data->shared.pserial == NULL)
	sprintf(text, "\n  Configure the serial port.\n");
      else
	sprintf(text, "\n  No BMS was detected.\n\n");
      UNLOCK;
      out =  string(text);
    }
  else
    {
      sprintf(text, "\n\%s\n\n", pshared->bms_version);
      out =  string(text);
      sprintf(text, "\n\%s\n", pshared->param_msg);
      //sprintf(text, "\n\%s\n", pshared->report_msg);
      out += string(text);
    }
  //
  papp_data->pinfo_buffer->text(out.c_str());
}

void create_information_tab(int x, int y, int w, int h, void *app_data)
{
  t_app_data      *papp_data = (t_app_data*)app_data;
  int              border;
  Fl_Text_Display *ptext;
  Fl_Text_Buffer  *pbuff;

  Fl_Group *group1 = new Fl_Group(x, y, w, h, "BMS Parameters");
  {
    border = TAB_BORDER;
    ptext = new Fl_Text_Display(x + border, y + border, w - 3 * border, h - 3 * border);
    pbuff = new Fl_Text_Buffer();
    ptext->buffer(pbuff);
    ptext->textcolor(fl_rgb_color(85, 170, 255));
    ptext->color(fl_rgb_color(8, 7, 5));
    papp_data->pinfo_buffer = pbuff;
    update_information_tab(papp_data);
  }
  group1->end();
}

