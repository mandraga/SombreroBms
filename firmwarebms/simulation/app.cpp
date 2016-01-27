#include <unistd.h>
#include <iterator>
#include <list>
#include <vector>
#include <string>

//#include <ctime>
#include <sys/time.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "gfxareas.h"
#include "keypress.h"
//
#include "../env.h"
//
#include "fake_AD7289A.h"
#include "fake_battery.h"
#include "fake_charger.h"
//
#include "app.h"
#include "sdlcalls.h"
#include "vbat.h"

using namespace std;

Cappdata::Cappdata(int width, int height):
  m_width(width),
  m_height(height),
  m_vbat(36),
  m_fake_AD7280(CFGAD728AMODULES),
  m_bquit(false)
{
  t_coord window_size;
  
  window_size.x = width;
  window_size.y = height;
  // Init gfx zones parameters
  create_layout();
  m_layout->resize(window_size);
  // Init all the SDL functions
  init_SDL(&m_sdlwindow, &m_sdlRenderer, m_window_xpos, m_window_ypos, m_width, m_height);
  TTF_Init();
  m_font = TTF_OpenFont(FONT_FILE, 16);
  if (m_font == NULL)
    {
      printf("Dialog practice error: font not found\n");
      exit(1);
    }
  setlooptime();

  m_errorled = false;
  m_balancingled = false;
  m_buzer = false;
  m_cutmains = false;

  m_run_current = 0;
}

Cappdata::~Cappdata()
{
  int xpos, ypos;

  // Close everything related to ui
  TTF_CloseFont(m_font);
  TTF_Quit();
  SDL_GetWindowPosition(m_sdlwindow, &xpos, &ypos);
  close_SDL(m_sdlwindow, m_sdlRenderer);
  delete m_layout;
}

void Cappdata::quit()
{
  m_bquit = true;
}

void Cappdata::setlooptime()
{
  m_last_time = (double)0.001 * SDL_GetTicks();
}

void Cappdata::set_coord(t_fcoord *coord, float prx, float pry)
{
  coord->x = prx;
  coord->y = pry;
}

void Cappdata::create_layout()
{
  Cgfxarea *pw;
  t_fcoord  pos;
  t_fcoord  dimensions;
  t_coord   wsize;
  int       i;
  char      barname[4096];

  wsize.x = m_width;
  wsize.y = m_height;
  m_layout = new CgfxareaList();
  for (i = 0; i < CFGBATNUMBER; i++)
    {
      set_coord(&pos, 10., (100. / (float)CFGBATNUMBER) * (i + 1));      
      pos.y += 5. + m_height / (100. / (float)CFGBATNUMBER);
      set_coord(&dimensions, 20., (100. / (1.5 * (float)CFGBATNUMBER)));
      sprintf(barname, "bat%d", i);
      pw = new Cgfxarea(pos, dimensions, wsize, barname);
      m_layout->add(pw);
    }
}

void Cappdata::key_on(int code)
{
  //t_shared_data   *pshared_data = (t_shared_data*)&m_shared_data;

  m_kstates.key_on(code);
  // Space bar = play/pause
  //printf("%c on\n", code);
  if (m_kstates.is_pressed('R'))
    {
      m_vbat += 0.1;
    }
  if (m_kstates.is_pressed('Q'))
    {
      m_vbat -= 0.1;
    }
}

void Cappdata::key_off(int code)
{
  m_kstates.key_off(code);
}

void Cappdata::set_render_color(int color)
{
  SDL_SetRenderDrawColor(m_sdlRenderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
}

void Cappdata::draw_basic_rectangle(Cgfxarea *pw, int color)
{
  SDL_Rect rectToDraw;
  int      err;
  t_coord  pos;
  t_coord  dimension;

  set_render_color(color);
  pw->get_pos(&pos);
  rectToDraw.x = pos.x;
  rectToDraw.y = pos.y;
  pw->get_dim(&dimension);
  rectToDraw.w = dimension.x;
  rectToDraw.h = dimension.y;
  err = SDL_RenderFillRect(m_sdlRenderer, &rectToDraw);
  if (err)
    {
      printf("SDL error rendering a rectangle: %s\n", SDL_GetError());
    }
}

void Cappdata::set_error_led(bool v)
{
  m_errorled = v;
}

void Cappdata::set_balancing_led(bool v)
{
  m_balancingled = v;
}

void set_buzer(bool v)
{
  m_buzer = v;
}

void set_cutmains(bool cutmains)
{
  m_cutmains = cutmains;
}

void set_run_current(int current);
{
  m_run_current = current;
}

int get_run_current();
{
  return m_run_current;
}

void Cappdata::renderTextBlended(t_coord pos, int color, string text, bool blended)
{
  SDL_Color   c;
  //SDL_Color   cbg;
  int         w, h;
  SDL_Rect    position;
  SDL_Surface *surface;
  SDL_Texture *texture;

  c.r = (color >> 16) & 0xFF;
  c.g = (color >> 8)  & 0xFF;
  c.b = color & 0xFF;
  /*
  cbg.r = (backgroundColor >> 16) & 0xFF; 
  cbg.g = (backgroundColor >>  8) & 0xFF;
  cbg.b = backgroundColor & 0xFF;
  */
  if (blended)
    surface = TTF_RenderText_Blended(m_font, text.c_str(), c);
  else
    surface = TTF_RenderText_Solid(m_font, text.c_str(), c);
  texture = SDL_CreateTextureFromSurface(m_sdlRenderer, surface);
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);
  position.x = pos.x;
  position.y = pos.y;
  position.w = w;
  position.h = h;
  SDL_RenderCopy(m_sdlRenderer, texture, NULL, &position);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}

void Cappdata::rendertext(t_coord pos, int color, char *text)
{
  renderTextBlended(pos, color, text, true);
}

#define TRSTRINGSZ 128

void display_state(char *str)
{
  switch (g_state)
    {
    case STATE_START:
      snprintf(str, TRSTRINGSZ, ("State: init\n"));
      break;
    case STATE_CHARGEON:
      snprintf(str, TRSTRINGSZ, ("State: chargeon\n"));
      break;
    case STATE_CHARGING:
      snprintf(str, TRSTRINGSZ, ("State: charging\n"));
      break;
    case STATE_IDLE:
      snprintf(str, TRSTRINGSZ, ("State: idle\n"));
      break;
    case STATE_RUN:
      snprintf(str, TRSTRINGSZ, ("State: run\n"));
      break;
    case STATE_RELAPSE:
      snprintf(str, TRSTRINGSZ, ("State: relapse\n"));
      break;
    case STATE_SECURITY:
      snprintf(str, TRSTRINGSZ, ("State: security\n"));
      break;
    case STATE_CRITICAL_FAILURE:
    default:
      snprintf(str, TRSTRINGSZ, ("State: failure\n"));
      break;
    }
}

void Cappdata::render_bats()
{
  Cgfxarea   *pw;
  int         i;
  char        arr[256];
  int         color;
  t_coord     pos, dim;
  int         offset = 80;
  int         tcolor = 0xFFFFFFFF;

  pw = m_layout->get_first();
  i = 0;
  while (pw != NULL && i < 10)
    {
      pw->get_pos(&pos);
      pw->get_dim(&dim);
      if (m_bats[i].get_mV() > &&
	  m_bats[i].get_mV() <)
	color = 0xFF00EF0F;
      else
	color = 0xFFFF0000;

      sprintf(arr, "charge: %f", m_bats[i].m_charge);
      rendertext(pos, tcolor, arr);
      pos.y += 12;
      sprintf(arr, "V: %f", m_bats[i].get_mV());
      rendertext(pos, tcolor, arr);
      pos.y += dim.x / 2;
      sprintf(arr, "balance: %d", m_fake_AD7280.get_balancing(i));
      rendertext(pos, tcolor, arr);
      
      draw_basic_rectangle(pw, color);
      i++;
      pw = m_layout->get_next();
    }  
}

void Cappdata::render_gui()
{
  int         sleep_delay = 100; // 100ms
  Cgfxarea   *pw;
  int         i;
  char        arr[256];
  int         color;
  t_coord     pos;
  int         offset = 80;
  float       Vpack;

  //----------------------------------------------------------
  // Simulated from the original code
  //----------------------------------------------------------
  State_machine();
  //----------------------------------------------------------
  //
  glClear(GL_COLOR_BUFFER_BIT);
  render_bats();
  sprintf(arr, "Vbat: %f", m_fake_charger.get_Vpack(pbats, batnum));
  pos.x = 500;
  pos.y = 20;
  rendertext(pos, color, arr);
  sprintf(arr, "charge: %f", m_fake_charger.get_TotalCharge(pbats, batnum));
);
  pos.x = 500;
  pos.y += offset;
  rendertext(pos, color, arr);
  sprintf(arr, "discharge %d", g_discharge_accumul);
  pos.x = 500;
  pos.y += offset;
  rendertext(pos, color, arr);
  sprintf(arr, "current %d", g_current);
  pos.x = 500;
  pos.y += offset;
  rendertext(pos, color, arr);
  display_state(arr);
  pos.x = 500;
  pos.y += offset;
  rendertext(pos, color, arr);
  // Update the screen
  SDL_RenderPresent(m_sdlRenderer);
  usleep(sleep_delay / 10);
  // Balance discharge depending on the balancing selection
  m_fake_AD7280.balancing_shunt(pbats, CFGBATNUMBER, sleep_delay);
}

