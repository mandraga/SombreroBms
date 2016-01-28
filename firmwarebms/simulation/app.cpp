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
#include "../main.h"
#include "../AD7280A.h"
#include "../inout.h"
#include "../eeprom.h"
#include "../state_machine.h"
//
#include "fake_battery.h"
#include "fake_AD7280A.h"
#include "fake_charger.h"
//
#include "app.h"
#include "sdlcalls.h"

extern t_pack_variable_data g_appdata;
extern t_eeprom_data        g_edat;
extern t_ad7280_state       g_ad7280;
extern t_eeprom_battery     g_bat[MAXBATTERY];

extern Cappdata  *g_papp;

using namespace std;

// Called at start, sets the IO port direction, defaults the variables, 
// Initialises the SPI devices and configures the interrupts
void init()
{
  /*
  // IO ports
  // Port B:
  DDRB  = 0x00 | (1 << CSBAT) | (1 << BUZZER) | (1 << PD) | (1 << MOSI) | (1 << SCLK);
  PORTB = 0x00 | (1 << CSBAT) | (0 << BUZZER) | (1 << PD) | (0 << MOSI) | (0 << SCLK);
  // Port C:
  DDRC  = 0x00 | (1 << CNVSTART) | (1 << CTS) | (1 << RELON);
  PORTC = 0x00 | (1 << CNVSTART) | (0 << CTS) | (0 << RELON);
  // Port D:
  DDRD  = 0x00 | (1 << TXD) | (1 << CSDAC) | (1 << STOP_CHARGER) | (1 << LED_BALANCING) | (1 << LED_ERROR);
  PORTD = 0x00 | (1 << TXD) | (1 << CSDAC) | (1 << STOP_CHARGER) | (1 << LED_BALANCING) | (1 << LED_ERROR);

  // Wait 50ms
  _delay_ms(50);
  */

  // Default values
  memset(&g_appdata, 0, sizeof(g_appdata));
  // Start point
  g_appdata.app_state = STATE_START;
  g_appdata.charging_started = 0;

  // Init UART
  //uart_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
  //uart_puts("startig the shit\n");

  // Init SPI
  //init_spi_master();

  // Init the SPI device AD7820A
  if (init_AD7820A(&g_ad7280))
    {
      printf("AD7820A failed\n");
      exit(EXIT_FAILURE);
    }
  // Initialises the ADC channel 7
  //init_adc();

  setled_balancing(0);

  // If something was stored in the EEPROM, read it
  read_cfg_from_EEPROM();
  read_bat_values_from_EEPROM(g_bat, g_edat.bat_elements);

  // Enable interrupts
  //sei();
}

Cappdata::Cappdata(int width, int height):
  m_width(width),
  m_height(height),
  m_bquit(false),
  m_vbat(36),
  m_fake_AD7280A(CFGAD728AMODULES)
{
  t_coord window_size;
  int     i;
  
  g_papp = this; // Ouch!
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
  
  // Set the initiali charge
  for (i = 0; i < CFGBATNUMBER; i++)
    {
      m_bats[i].set_capa(CAPACITYMAH, 40000);
    }
  m_bats[ 1].set_capa(CAPACITYMAH + CAPACITYMAH / 20, 56060);
  m_bats[ 2].set_capa(CAPACITYMAH - CAPACITYMAH / 20, 44000);
  m_bats[ 4].set_capa(CAPACITYMAH                   , 42320);
  m_bats[ 5].set_capa(CAPACITYMAH + CAPACITYMAH / 15, 50323);
  m_bats[ 7].set_capa(CAPACITYMAH + CAPACITYMAH / 22, 39021);
  m_bats[ 9].set_capa(CAPACITYMAH - CAPACITYMAH / 18, 41432);
  m_bats[10].set_capa(CAPACITYMAH                   , 37200);

  init();
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
  int       i, h;
  char      barname[4096];

  wsize.x = m_width;
  wsize.y = m_height;
  m_layout = new CgfxareaList();
  h = (100. / (float)(CFGBATNUMBER + 4));
  for (i = 0; i < CFGBATNUMBER; i++)
    {
      set_coord(&pos, 2., (h + 1) * (i + 1));
      set_coord(&dimensions, 60., h);
      sprintf(barname, "bat%d", i);
      pw = new Cgfxarea(pos, dimensions, wsize, barname);
      m_layout->add(pw);
    }
}

// R = run the golf cart
// T = stop the golf cart
// P = plug the charger
// U = unplug the charger
// K = temperature up
// L = temperature down
void Cappdata::key_on(int code)
{
  //t_shared_data   *pshared_data = (t_shared_data*)&m_shared_data;

  m_kstates.key_on(code);
  // Space bar = play/pause
  //printf("%c on\n", code);
  if (m_kstates.is_pressed('r'))
    {
      m_run_current = 25000;
    }
  if (m_kstates.is_pressed('t'))
    {
      m_run_current = 5;
    }
  if (m_kstates.is_pressed('p'))
    {
      m_fake_charger.plug_charger(true);
    }
  if (m_kstates.is_pressed('u'))
    {
      m_fake_charger.plug_charger(false);
    }
  if (m_kstates.is_pressed('k'))
    {
      m_fake_AD7280A.set_temp(m_fake_AD7280A.get_temp() + 1);
    }
  if (m_kstates.is_pressed('l'))
    {
      m_fake_AD7280A.set_temp(m_fake_AD7280A.get_temp() - 1);
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

void Cappdata::draw_rectangle(t_coord pos, t_coord dim, int color)
{
  SDL_Rect rectToDraw;
  int      err;

  set_render_color(color);
  rectToDraw.x = pos.x;
  rectToDraw.y = pos.y;
  rectToDraw.w = dim.x;
  rectToDraw.h = dim.y;
  err = SDL_RenderFillRect(m_sdlRenderer, &rectToDraw);
  if (err)
    {
      printf("SDL error rendering a rectangle: %s\n", SDL_GetError());
    }
}

Cfake_battery *Cappdata::get_fake_bats()
{
  return m_bats;
}

Cfake_AD7280A *Cappdata::get_fake_AD7280()
{
  return &m_fake_AD7280A;
}

Cfake_charger *Cappdata::get_fake_charger()
{
  return &m_fake_charger;
}

void Cappdata::set_error_led(bool v)
{
  m_errorled = v;
}

void Cappdata::set_balancing_led(bool v)
{
  m_balancingled = v;
}

void Cappdata::set_buzer(bool v)
{
  m_buzer = v;
}

void Cappdata::set_cutmains(bool cutmains)
{
  m_cutmains = cutmains;
}

void Cappdata::set_run_current(int current)
{
  m_run_current = current;
}

int Cappdata::get_run_current()
{
  return m_run_current;
}

void Cappdata::Run_Shunt(int batnumber, int delayms)
{
  int   i;

  for (i = 0; i < batnumber; i++)
    {
	m_bats[i].shunt(m_run_current, delayms);
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

#define TRSTRINGSZ 128

void display_state(char *str)
{
  switch (g_appdata.app_state)
    {
    case STATE_START:
      snprintf(str, TRSTRINGSZ, ("State: init"));
      break;
    case STATE_CHARGEON:
      snprintf(str, TRSTRINGSZ, ("State: chargeon"));
      break;
    case STATE_CHARGING:
      snprintf(str, TRSTRINGSZ, ("State: charging"));
      break;
    case STATE_IDLE:
      snprintf(str, TRSTRINGSZ, ("State: idle"));
      break;
    case STATE_RUN:
      snprintf(str, TRSTRINGSZ, ("State: run"));
      break;
    case STATE_RELAPSE:
      snprintf(str, TRSTRINGSZ, ("State: relapse"));
      break;
    case STATE_SECURITY:
      snprintf(str, TRSTRINGSZ, ("State: security"));
      break;
    case STATE_CRITICAL_FAILURE:
    default:
      snprintf(str, TRSTRINGSZ, ("State: failure."));
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
  int         tcolor = 0xFF000000;

  pw = m_layout->get_first();
  i = 0;
  while (pw != NULL && i < CFGBATNUMBER)
    {
      pw->get_pos(&pos);
      pw->get_dim(&dim);
      // Background
      draw_basic_rectangle(pw, 0xFF7E7E7E);
      //
      if (m_bats[i].get_mV() > (int)g_edat.bat_minv &&
	  m_bats[i].get_mV() < (int)g_edat.bat_maxv)
	color = 0xFF00EF0F;
      else
	color = 0xFFFF0000;
      // Draw the charge level
      dim.x = dim.x * m_bats[i].m_charge / m_bats[i].m_capacity;
      draw_rectangle(pos, dim, color);
      //
      pw->get_pos(&pos);
      pw->get_dim(&dim);
      sprintf(arr, "charge: %f", m_bats[i].m_charge);
      rendertext(pos, tcolor, arr);
      pos.y += 20;
      sprintf(arr, "V: %d", m_bats[i].get_mV());
      rendertext(pos, tcolor, arr);
      pos.x += dim.x / 2;
      sprintf(arr, "balance: %d", m_fake_AD7280A.get_balancing(i));
      rendertext(pos, tcolor, arr);
      //
      i++;
      pw = m_layout->get_next();
    }
}

void Cappdata::render_gui()
{
  const int   ofx = 700;
  int         sleep_delay = 100; // 100ms
  char        arr[256];
  int         color;
  t_coord     pos, dim;
  int         offset = 20;

  //----------------------------------------------------------
  // Simulated from the original code
  //----------------------------------------------------------
  State_machine();
  //----------------------------------------------------------
  //
  glClear(GL_COLOR_BUFFER_BIT);
  render_bats();
  color = 0xFF8F8F8F;
  sprintf(arr, "R/T= run stop a golf car P/U= plug/unplug the charger K/L= PCB temperature+/-");
  pos.x = 10;
  pos.y = 10;
  rendertext(pos, color, arr);
  color = 0xFFFFFFFF;
  sprintf(arr, "Vbat: %d", (int)g_appdata.total_vbat);
  pos.x = ofx;
  pos.y = 30;
  rendertext(pos, color, arr);
  sprintf(arr, "fake Vbat: %f", m_fake_charger.get_Vpack(m_bats, CFGBATNUMBER));
  pos.x = ofx;
  pos.y += offset;
  rendertext(pos, color, arr);
  sprintf(arr, "charge: %fAH", (float)(g_appdata.state_of_charge) / 1000);
  pos.x = ofx;
  pos.y += offset;
  rendertext(pos, color, arr);
  sprintf(arr, "discharge: %dmA", (int)g_appdata.c_discharge);
  pos.x = ofx;
  pos.y += offset;
  rendertext(pos, color, arr);
  //
  sprintf(arr, "temperature: %d°C", (int)g_appdata.temperature[0]);
  pos.x = ofx;
  pos.y += offset;
  rendertext(pos, color, arr);
  display_state(arr);
  pos.x = ofx;
  pos.y += offset;
  rendertext(pos, color, arr);
  sprintf(arr, "Charger Pluged: %d", m_fake_charger.m_bpluged? 1 : 0);
  pos.x = ofx;
  pos.y += 2 * offset;
  rendertext(pos, color, arr);
  sprintf(arr, "Charging: %d", m_fake_charger.m_active? 1 : 0);
  pos.x = ofx;
  pos.y += offset;
  rendertext(pos, color, arr);

  // Update the screen
  SDL_RenderPresent(m_sdlRenderer);
  //usleep(sleep_delay / 10);
  usleep(1000000 / 60);
  //------------------------------------------------------------------
  // Balance discharge depending on the balancing selection
  m_fake_AD7280A.balancing_shunt(m_bats, CFGBATNUMBER, sleep_delay);
  // Charge, it the cahrger is on
  m_fake_charger.add_charge(m_bats, CFGBATNUMBER, sleep_delay);
  //
  if (m_buzer)
    {
      printf("%c", BELLCODE);
    }
  if (!m_cutmains)
    {
      Run_Shunt(CFGBATNUMBER, sleep_delay);
    }
  // Error led
  color = m_errorled? 0xFF0000FF : 0xFF808080;
  pos.x = ofx;
  pos.y = 10;
  dim.x = 10;
  dim.y = 10;
  draw_rectangle(pos, dim, color);
  // Balancing led
  color = m_balancingled? 0xFF00FF00 : 0xFF808080;
  pos.x += 20;
  draw_rectangle(pos, dim, color);
}

