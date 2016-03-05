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
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Tabs.H>
#define USE_GLEW 1
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>

#include "serial/serialport.h"
#include "cfgfile.h"
//
#include "charge_tab.h"
//
#include "env.h"
#include "SombreroBMS.h"
#include "shared.h"
#include "main.h"

using namespace std;

GLBatWindow::GLBatWindow(int X, int Y, int W, int H, const char *L, void *pshared) : Fl_Gl_Window(X, Y, W, H, L)
{
  m_pshared = pshared;
  m_pfont = TTF_OpenFont(DATAPATH MAINFONT, 14);
  if (m_pfont == NULL)
    {
      printf("Error: font \"%s\" is missing.\n", DATAPATH MAINFONT);
      exit(EXIT_FAILURE);
    }
  m_prenderer = new CGL2Dprimitives(W, H);
}

GLBatWindow::~GLBatWindow()
{
  if (m_pfont)
    TTF_CloseFont(m_pfont);
  delete m_prenderer;
}

void GLBatWindow::draw_info_lines(int w, int h)
{
  const int      txtsz = 512;
  char           txt[txtsz];
  t_shared_data *pshared = (t_shared_data*)m_pshared;
  t_report      *preport = &pshared->pBMS->m_report;
  float          border = 4;
  float          texth  = 16;
  t_fcoord       pos;
  int            color;
  bool           blended;

  blended = true;
  color = 0xFFEFFEE7;
  LOCK;
  snprintf(txt, txtsz, "Pack status: \"%s\"", preport->state);
  pos.x = 20;
  pos.y = border;
  m_prenderer->print(txt, m_pfont, pos, color, blended);
  //
  snprintf(txt, txtsz, "Current: %.2fA", (float)preport->currentmA / 1000.);
  pos.y += texth;
  m_prenderer->print(txt, m_pfont, pos, color, blended);
  //
  pos.x = w / 2;
  pos.y = border;
  snprintf(txt, txtsz, "%d°C", preport->temperature);
  m_prenderer->print(txt, m_pfont, pos, color, blended);
  //
  pos.y += texth;
  snprintf(txt, txtsz, "Vbat: %.3fV", preport->Vbat);
  m_prenderer->print(txt, m_pfont, pos, color, blended);
  //
  pos.x = 20;
  pos.y += texth;
  snprintf(txt, txtsz, "Charge: %d%% (%.3fAH)", preport->charge_percent, (float)(preport->chargemAH) / 1000.);
  m_prenderer->print(txt, m_pfont, pos, color, blended);
  UNLOCK;
}

void draw_battery(CGL2Dprimitives *prenderer, TTF_Font *pfont, t_shared_data *pshared, int i, float x, float y, float w, float h)
{
  char         text[4096];
  float        Vbat, minv, maxv, bot, span;
  float        border = 0;
  t_fcoord     pos, dim;
  int          color;
  bool         blended = true;
  t_report    *prep = &pshared->pBMS->m_report;
  t_batreport *preport = &prep->element_array[i];

  // Draw the charge box
  pos.x = x;
  pos.y = y;
  dim.x = w;
  dim.y = h;
  prenderer->box(pos, dim, 0xFF7E7E7E);
  //
  Vbat = preport->Velement;
  minv = pshared->pBMS->m_params.EltVmin;
  maxv = pshared->pBMS->m_params.EltVmax;
  //
  if (Vbat > minv &&
      Vbat < maxv)
    color = 0xFF08EF00;
  else
    color = 0xFF0000FF;
  // Draw the vbat
  pos.x = x;
  pos.y = y;
  //
  span = (maxv - minv) * 1.5;
  bot = minv - span / 6;
  //top = maxv + span / 6;
  dim.x = (Vbat - bot) * w / span;
  dim.x = dim.x < 0? 0 : dim.x;
  dim.x = dim.x > w? w : dim.x;
  //
  dim.y = h;
  prenderer->box(pos, dim, color);
  //
  // Write the values
  //
  color = 0xFF000000;
  sprintf(text, "%2d  %.3fV (Lowest %.3fV)     Lowbat: %dEvents", i + 1, Vbat, preport->Vlowest, preport->events);
  pos.x = x + 8;
  pos.y = y + border;
  prenderer->print(text, pfont, pos, color, blended);
  //
  sprintf(text, "Average Charge time: %ds", preport->average_charging_time);
  pos.y += gl_height();
  prenderer->print(text, pfont, pos, color, blended);
  //
  sprintf(text, "Balancing %d", preport->balancing? 1 : 0);
  pos.x += w * 0.65;
  prenderer->print(text, pfont, pos, color, blended);
}

void GLBatWindow::draw_batteries(float x, float y, float wf, float hf)
{
  t_shared_data *pshared = (t_shared_data*)m_pshared;
  //float          border = 10;
  t_report      *preport = &pshared->pBMS->m_report;
  int            i;
  float          by, hy;

  LOCK;
  for (i = 0; i < preport->elements; i++)
    {
      hy = (float)hf / (float)preport->elements;
      by = y + (float)(preport->elements - 1 - i) * hy;
      hy -= 5.;
      draw_battery(m_prenderer, m_pfont, pshared, i, x, by, wf, hy);
    }
  UNLOCK;
}

void GLBatWindow::draw()
{
  t_coord  pos, dim;
  t_fcoord orthodim;
  int      y;

  printf("Charge GlWindow draw() is called.\n");
  // valid() is turned on by FLTK after draw() returns
  if (!valid())
    {
      printf("Creating an OpenGl context.\n");
      if (m_prenderer->init_OpenGL())
	{
	  printf("Error: OpenGL 2 not enabled in fltk.\n");
	  exit(EXIT_FAILURE);
	}
      m_prenderer->SetLogicalSize(w(), h());
      m_prenderer->set_clear_color(0xFF020302);
      //
      pos.x = pos.y = 0;
      dim.x = orthodim.x = w();
      dim.y = orthodim.y = h();
      m_prenderer->set_viewport(pos, dim, orthodim);
      m_prenderer->set_clear_color(0xFF050203); 
    }
  // Clear the window
  m_prenderer->Clear();
  //
  // Draw everything
  y  = h() / 7;
  draw_info_lines(w(), y);
  draw_batteries(20, y, w() - 40, h() - y);
}

int GLBatWindow::handle(int event)
{
  switch(event)
    {
/*
    case FL_ENTER:
      cursor(FL_CURSOR_CROSS);
      break;
    case FL_LEAVE:
      cursor(FL_CURSOR_DEFAULT);
      break;
    case FL_PUSH:
      ... mouse down event ...
	... position in Fl::event_x() and Fl::event_y()
	return 1;
    case FL_DRAG:
      ... mouse moved while down event ...
	return 1;
    case FL_RELEASE:
      ... mouse up event ...
	return 1;
    case FL_FOCUS :
    case FL_UNFOCUS :
      ... Return 1 if you want keyboard events, 0 otherwise
	return 1;
    case FL_KEYBOARD:
      ... keypress, key is in Fl::event_key(), ascii in Fl::event_text()
	... Return 1 if you understand/use the keyboard event, 0 otherwise...
	return 1;
    case FL_SHORTCUT:
      ... shortcut, key is in Fl::event_key(), ascii in Fl::event_text()
	... Return 1 if you understand/use the shortcut event, 0 otherwise...
	return 1;
*/
    default:
      // pass other events to the base class...
      return Fl_Gl_Window::handle(event);
    }
}

void update_charge_tab(void *app_data)
{
  t_app_data *papp_data = (t_app_data*)app_data;

  // Redraw
  papp_data->pGLBatWindow->redraw();
}

void create_charge_tab(int x, int y, int w, int h, void *app_data)
{
  t_app_data      *papp_data = (t_app_data*)app_data;
  t_shared_data   *pshared   = &papp_data->shared;
  int              border;

  Fl_Group *group1 = new Fl_Group(x, y, w, h, "Batteries");
  {
    border = TAB_BORDER;
    papp_data->pGLBatWindow = new GLBatWindow(x + border, y + border, w - 3 * border, h - 3 * border, "BatGl", pshared);
    update_charge_tab(papp_data);
  }
  group1->end();
}

