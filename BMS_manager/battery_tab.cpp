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
#include "battery_tab.h"
//
#include "env.h"
#include "SombreroBMS.h"
#include "shared.h"
#include "main.h"

using namespace std;

GLBatCurvWindow::GLBatCurvWindow(TTF_Font *pfont, int X, int Y, int W, int H, const char *L, void *pshared) : Fl_Gl_Window(X, Y, W, H, L)
{
  m_pshared = pshared;
  m_pfont = pfont;
  m_prenderer = new CGL2Dprimitives(W, H);
}

GLBatCurvWindow::~GLBatCurvWindow()
{
  delete m_prenderer;
}

float GLBatCurvWindow::V2pos(float Vbat, float maxv, float minv, float h)
{
  float bot, span, ret, diff;

  diff = (maxv - minv);
  span = diff * 1.5;
  bot = minv - (span - diff) / 2.;
  //
  ret = (Vbat - bot) * h / span;
  ret = ret < 0? 0 : ret;
  ret = ret > h? h : ret;
  ret = h - ret; // inverse
  return ret;
}

void GLBatCurvWindow::draw_batteries_thresholds(float x, float y, float w, float h)
{
  t_shared_data *pshared = (t_shared_data*)m_pshared;
  const int      txtsz = 512;
  char           txt[txtsz];
  bool           blended = true;
  float          xsta, yp, width;
  float          vlimit, minv, maxv;
  t_fcoord       pos;
  int            color;

  LOCK;
  // Vmin Vcharged Vmax
  color = 0xAFADADAD;
  minv = pshared->pBMS->m_params.EltVmin;
  maxv = pshared->pBMS->m_params.EltVmax;
  yp = y + V2pos(minv, maxv, minv, h);
  xsta = x;
  width = w;
  m_prenderer->hline(xsta, yp, width, color);
  pos.x = 4;
  pos.y = yp;
  snprintf(txt, txtsz, "%.3fV", minv);
  m_prenderer->print(txt, m_pfont, pos, color, blended);
  //
  yp = y + V2pos(maxv, maxv, minv, h);
  m_prenderer->hline(xsta, yp, width, color);
  pos.x = 4;
  pos.y = yp;
  snprintf(txt, txtsz, "%.3fV", maxv);
  m_prenderer->print(txt, m_pfont, pos, color, blended);
  if (pshared->pBMS->m_params.BatElements > 0)
    {
      color = 0xAF3DAD3D;
      vlimit = pshared->pBMS->m_params.MaxVbat / pshared->pBMS->m_params.BatElements;
      yp = y + V2pos(vlimit, maxv, minv, h);
      m_prenderer->hline(xsta, yp, width, color);
      pos.x = 4;
      pos.y = yp;
      snprintf(txt, txtsz, "%.3fV", vlimit);
      m_prenderer->print(txt, m_pfont, pos, color, blended);
    }
  UNLOCK;
}

int get_bat_color(int bat)
{
  unsigned int colors[] = {0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFF00CC00,
			   0xFF999900, 0xFFFF00FF, 0xFFFFFF00, 0xFF00CCCC,
			   0xFF433C76, 0xFF137AE1, 0xFF21CB7C, 0xFF455323,
			   0xFFCC0066, 0xFF631C89, 0xFFA40A14, 0xFFA4750A,
			   0xFF5E5E5E, 0xFF10426E};

  if (bat >= (int)(sizeof(colors) / sizeof(unsigned int)))
    return 0xFF000000;
  return colors[bat];
}

void GLBatCurvWindow::draw_battery(int i, float x, float y, float w, float h)
{
  float             Vbat, minv, maxv;
  int               color;
  t_shared_data    *pshared = (t_shared_data*)m_pshared;
  CSombreroBMS     *pbms = pshared->pBMS;
  t_batmeasurement *pbm;
  t_batmeasurement *pbmn;
  int               elts;
  int               bat;
  float             xsta, ysta, xsto, ysto;

  minv = pbms->m_params.EltVmin;
  maxv = pbms->m_params.EltVmax;
  elts = pbms->get_history_sz();
  bat = elts < w? 0 : elts - w;
  while (bat < elts - 1)
    {
      pbm  = pbms->get_hitory_elt(bat);
      pbmn = pbms->get_hitory_elt(bat + 1);
      if (pbm != NULL && pbmn != NULL)
	{
	  color = get_bat_color(i);
	  xsta = bat;
	  xsto = bat + 1;
	  Vbat = pbm->Vbat[i];
	  ysta = y + V2pos(Vbat, maxv, minv, h);
	  Vbat = pbmn->Vbat[i];
	  ysto = y + V2pos(Vbat, maxv, minv, h);
	  m_prenderer->line(xsta, ysta, xsto, ysto, color);
	}
      bat++;
    }
}

void GLBatCurvWindow::draw_batteries(float x, float y, float w, float h)
{
  t_shared_data *pshared = (t_shared_data*)m_pshared;
  t_report      *preport = &pshared->pBMS->m_report;
  int            i;

  LOCK;
  for (i = 0; i < preport->elements; i++)
    {
      draw_battery(i, x, y, w, h);
    }
  UNLOCK;
}

void GLBatCurvWindow::draw()
{
  t_coord  pos, dim;
  t_fcoord orthodim;
  int      y;

  //printf("Charge GlBatCurWindow draw() is called.\n");
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
      //
      pos.x = pos.y = 0;
      dim.x = orthodim.x = w();
      dim.y = orthodim.y = h();
      m_prenderer->set_viewport(pos, dim, orthodim);
      m_prenderer->set_clear_color(0xFFFEFEFE); 
    }
  // Clear the window
  m_prenderer->Clear();
  //
  // Draw everything
  y  = 0;
  draw_batteries(0, y, w(), h());
  draw_batteries_thresholds(0, y, w(), h());
}

int GLBatCurvWindow::handle(int event)
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

void update_battery_tab(void *app_data)
{
  t_app_data *papp_data = (t_app_data*)app_data;

  // Redraw
  papp_data->pGLBatCurvWindow->redraw();
}

void create_battery_tab(int x, int y, int w, int h, void *app_data)
{
  t_app_data *papp_data = (t_app_data*)app_data;
  int         border;

  Fl_Group *group1 = new Fl_Group(x, y, w, h, "History");
  {
    border = 10;
    papp_data->pGLBatCurvWindow = new GLBatCurvWindow(papp_data->pfont, x + border, y + border, w - 3 * border, h - 3 * border, "BatVbGl", &papp_data->shared);
    update_battery_tab(papp_data);
  }
  group1->end();
}

