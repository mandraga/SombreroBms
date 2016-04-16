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
#include "app.h"
#include "sdlcalls.h"
#include "vbat.h"

using namespace std;

Cappdata::Cappdata(int width, int height):
  m_width(width),
  m_height(height),
  m_vbat(36),
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
  m_font = TTF_OpenFont(FONT_FILE, 64);
  if (m_font == NULL)
    {
      printf("Dialog practice error: font not found\n");
      exit(1);
    }
  setlooptime();
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
  for (i = 0; i < 10; i++)
    {
      set_coord(&pos, 10., 8. * (i + 1));
      pos.y += 5.;
      set_coord(&dimensions, 20., 6.);
      sprintf(barname, "bar%d", i);
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

extern int g_charge;
extern int g_discharge_accumul;
extern int g_state;
extern int g_current;
extern int g_relax_counter;
extern int g_relax_in_Vbat;
extern int g_average;

void display_state(char *str)
{
  switch (g_state)
    {
    case STATE_CHARGED:
      sprintf(str, "charged");
      break;
    case STATE_RELAX:
      sprintf(str, "relax");
      break;
    case STATE_CHARGING:
      sprintf(str, "charging");
      break;
    case STATE_TOO_LOW:
      sprintf(str, "toolow");
      break;
    case STATE_INUSE:
      sprintf(str, "inuse");
      break;
    default:
      sprintf(str, "unknown");
      break;
    }
}

void Cappdata::render_gui()
{
  Cgfxarea   *pw;
  int         i;
  char        arr[256];
  int         color;
  t_coord     pos;
  int         offset = 80;

  fill_vbat_arr(arr, (int)(m_vbat * 1000));
  //
  glClear(GL_COLOR_BUFFER_BIT);
  pw = m_layout->get_first();
  i = 0;
  while (pw != NULL && i < 10)
    {
      if (arr[i] == 1)
	color = 0xFFFF0000;
      else
	color = 0xFF4F4F4F;
      draw_basic_rectangle(pw, color);
      i++;
      pw = m_layout->get_next();
    }
  sprintf(arr, "%f", m_vbat);
  pos.x = 500;
  pos.y = 20;
  rendertext(pos, color, arr);
  sprintf(arr, "charge    %d", g_charge);
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
  sprintf(arr, "relax counter %d", g_relax_counter);
  pos.x = 500;
  pos.y += offset;
  rendertext(pos, color, arr);
  sprintf(arr, "relax in vbat %d", g_relax_in_Vbat);
  pos.x = 500;
  pos.y += offset;
  rendertext(pos, color, arr);
  sprintf(arr, "averageV %d", g_average);
  pos.x = 500;
  pos.y += offset;
  rendertext(pos, color, arr);

  // Update the screen
  SDL_RenderPresent(m_sdlRenderer);
  usleep(100000);
}

